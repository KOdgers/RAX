# -*- coding: utf-8 -*-
"""
Load Arduino Txt
Created on Sun Apr 15 17:39:51 2018

@author: kelly
"""

import numpy as np
import matplotlib.pyplot as plt
import os, re

directory = '/home/kelly/Arduino/PythonFiles/2018-04-16/18-45'
#time= ''

files = os.listdir(directory)
runs = [int(re.split(r'(\d+)', s)[1]) for s in files if s[0]=='V']


runs = np.sort(runs)
events=0
for i in range(3,len(runs)-3):
    runEvents=0
    print('Processing Run Number: ' +str(i))
    V=np.loadtxt(directory+'/Volts'+str(i)+'.txt')
    T=np.loadtxt(directory+'/Time'+str(i)+'.txt')
    print(np.shape(V))
    print(np.argmax(V))
    Avg = np.mean(V)
    StDev= np.std(V)
    print(Avg,StDev)
    if max(V) >= 3*StDev+Avg:
        print('Strong Deviation')
        events=1
        runEvents+=1
        voltsEvent=V[np.argmax(V)-200:np.argmax(V)+200]        
        timeEvent=T[np.argmax(V)-200:np.argmax(V)+200]
        plt.figure()
        plt.plot(timeEvent,voltsEvent)
        plt.title('Run '+str(i)+': Event '+str(events))
        plt.xlabel('Time [ms]')
        plt.ylabel('Volts')
        plt.savefig(directory+'/Figure'+str(i)+'R-E'+str(events)+'.png')
        plt.close()
        
        
if events ==0:
    print('Nothing Eventful happened')