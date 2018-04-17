# -*- coding: utf-8 -*-
"""
Created on Mon Apr 16 19:31:35 2018

@author: kelly
"""

import numpy as np
import matplotlib.pyplot as plt
import os, re
import scipy.fftpack

directory = '/home/kelly/Arduino/PythonFiles/2018-04-16/18-45'


files = os.listdir(directory)
runs = [int(re.split(r'(\d+)', s)[1]) for s in files if s[0]=='V']


runs = np.sort(runs)
events=0

for i in range(3,4):
    plt.figure()
    runEvents=0
    print('Processing Run Number: ' +str(i))
    V=np.loadtxt(directory+'/Volts'+str(i)+'.txt')
    T=np.loadtxt(directory+'/Time'+str(i)+'.txt')
 #   plt.hist(T,histtype='step',range=(0,.1))
    yf= scipy.fftpack.fft(V)
 #   xf = np.linspace(0.0, 1.0/(2.0*T), N/2)
    yf = [np.mean(np.abs(yf[i:i+20])) for i in range(0,int(np.floor(len(yf)/20)))]
    plt.plot(np.abs(yf))
    #plt.show()
    plt.show()


