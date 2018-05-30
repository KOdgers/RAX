#!/home/kelly/Desktop/XePythonEnv/XeEnv/bin/python

"""
This is an arduino acquisition program for fast reading, parsing and saving as a dataframe.
It's intended for use with an arduino due with signal in to A0, and threshold for acquisition is dependent on your
input.
"""

import sys
import numpy as np
import serial.tools.list_ports
import os
from datetime import datetime
import multiprocessing
import pandas as pd
import time as time


def Initialize():
    """
    I plan to add more, but for now it finds the current directory and the serial port for the arduino
    """
    myports = [tuple(p) for p in list(serial.tools.list_ports.comports()) if p[2] != 'n/a']
    if len(myports)==1:
        SerialPort = myports[0]
    else:
        print('Error in finding arduino port')
        sys.exit()
    print(SerialPort)
    Directory = os.getcwd()
    print(Directory)
    
    return(SerialPort[0],Directory)


      
def SerialReader(port, q, exitFlag):

    chunkSize=1024
    chunks=5000
    buffer = np.zeros(chunks*chunkSize, dtype=np.uint16)
#        dataMutex = self.dataMutex=threading.Lock()

    ptr=0
    while True:

        # read one full chunk from the serial port
        data = port.read(chunkSize*2)
        # convert data to 16bit int numpy array
        data = np.fromstring(data, dtype=np.uint16)

        buffer[ptr:ptr+chunkSize] = data
        ptr = (ptr + chunkSize) % buffer.shape[0]

        if ptr == 0:
            q.put(buffer)
        if not exitFlag.empty():
            break


def DAQListener(q, directory, plist, threshold=.26):
    """
    This function should call PeakBuilding processes as well. So long as the queue isn't empty. 
    If the Queue is empty too long the function ends.
    
    """

    proc =0
    name = 1
    missedCalls = 0
    while True:
        if proc%5 == 0:
            name +=1
        if q.empty():  # is True
            time.sleep(5)
            missedCalls +=1
            print(missedCalls)
            if missedCalls >5:
                break
        else:
            missedCalls=0
            proc+=1
            p1 = multiprocessing.Process(target=PeakBuilding, args = (q, directory, name, proc))
            p1.start()
            plist.put(p1.pid)
            print(str(p1.pid))
            
            time.sleep(10)

    p1.join()
    return
    


def PeakBuilding(q, directory, name, proc, threshold=.027):
    """ This process gets from the queue. Makes a dictionary of peaks from the wave from. 
    Converts the dictionary to a dataframe and appends(or creates) to an h5 with a different key for each time it is called.
    """

    Volts = q.get()
    Volts = Volts.astype(np.float32) * (3.3 / 2**12)
    ind = [np.argsort(Volts)]
    ind = [i for i in ind[0] if Volts[i]>=threshold  ]#and (i > 20 or i<len(Volts)-20)
    peaks ={}
    print('starting'+str(proc))
    print('There are ' + str(len(ind))+' Events')
    while len(ind)>0:
        peaks[str(ind[0])] = Volts[ind[0]-20:ind[0]+20]
        badInd = np.where(np.logical_and(ind>=ind[0]-20, ind<=ind[0]+20))
        ind = np.delete(ind,badInd)
    print('Writing to h5')
    df_peaks = pd.DataFrame.from_dict(peaks,orient='index')
    df_peaks.to_hdf(directory+'/Peaks'+str(10+name)+'.hdf5',key = 'Vacuum', mode ='a')
    print('Done'+str(proc))    
    return
    
def RunDAQ(q,plist):
    """
    Should put it all together
    """
    print('Beginning setup of Arduino DAQ')
    sPort, CDir = Initialize()
    s = serial.Serial(sPort)
    CDir = '/home/kelly/Data/Photo_Data'
    print('Setup complete and succesful')
#    thread = SerialReader(s)
#    thread.start()

    while True:
        task = input("Would you like to Save Data or End?")
        if task[0] == 's' or task[0] == 'S':
            runtime = input("For how many minutes?")
            runtime=float(runtime)
            starttime= time.time()
            Now=datetime.now()
            if not os.path.exists(CDir+'/'+Now.strftime("%Y-%m-%d")):
                os.mkdir(CDir+'/'+Now.strftime("%Y-%m-%d"))
            exitFlag=multiprocessing.Queue()
            Reader = multiprocessing.Process(target=SerialReader,args=(s,q,exitFlag))
            listener = multiprocessing.Process(target = DAQListener,args=(q,CDir+'/'+Now.strftime("%Y-%m-%d"),plist))
            Reader.start()

            listener.start()
            time.sleep(2)
            while (time.time()-starttime)/60 <= runtime:
                pass
            exitFlag.put(1)
            Reader.join()
            print('Finished Acquiring Raw Data. Waiting for Processing to Finish')
            listener.join()
            print('Analysis and Peaks Dataframes finished. ')
        else:
            sys.exit()
if __name__=="__main__":
    q = multiprocessing.Queue()
    plist=multiprocessing.Queue()
    RunDAQ(q,plist)

