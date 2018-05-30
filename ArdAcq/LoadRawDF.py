

import numpy as np
import pandas as pd
from os import listdir
from os.path import isfile, join
import matplotlib.pyplot as plt


def Load_DataFrames(directory, keys):
    files = [f for f in listdir(directory) if isfile(join(directory, f))]
    for i, item in enumerate(files):
        if i == 0:
            df_temp = pd.read_hdf(directory+'/'+item, key=keys)
        else:
            df_temp = pd.concat([df_temp, pd.read_hdf(directory+'/'+item, key=keys)])

    return df_temp

def Get_Max_List(df_raw, directory, keys):
    max_list = np.array(df_raw.iloc[:, 21])
    middle_sum = max_list+np.array(df_raw.iloc[:,20])+np.array(df_raw.iloc[:,22])
    plt.figure(0)
    plt.hist(max_list)
    plt.xlabel('Volts')
    plt.ylabel('Counts')
    plt.title('Max Height of '+keys)
    plt.savefig(directory+'/MaxHeightHist'+keys+'.png')
    plt.figure(1)
    plt.hist(middle_sum)
    plt.xlabel('Volts')
    plt.ylabel('Counts')
    plt.title('Middle Sum of '+keys)
    plt.savefig(directory+'/SummedHeightHist'+keys+'.png')
    plt.close()


CDir = '/home/kelly/Data/Photo_Data/2018-05-30'

# DF_Raw = Load_DataFrames(CDir, 'Air')
DF_Raw = pd.concat([pd.read_hdf(CDir+'/Peaks54.hdf5', key='Air'), pd.read_hdf(CDir+'/Peaks55.hdf5', key='Air')])

Get_Max_List(DF_Raw, CDir, 'Air')

# DF_Raw2 = Load_DataFrames(CDir, 'Vacuum')
DF_Raw2 = pd.concat([pd.read_hdf(CDir+'/Peaks18.hdf5',key='Vacuum'), pd.read_hdf(CDir+'/Peaks19.hdf5',key='Vacuum')])

Get_Max_List(DF_Raw2, CDir, 'Vacuum')


