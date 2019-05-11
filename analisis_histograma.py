#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Fri May 10 00:52:54 2019

@author: paz
"""

import numpy as np
import math as mt
import matplotlib.pyplot as plt
from subprocess import call





delta=np.loadtxt('Histograma_delta')
histograma=np.loadtxt('muestreo')

N=len(histograma[0,:])
k=len(histograma[:,1])



plt.figure(1)
plt.scatter(delta[:,0],delta[:,1])
plt.show()


#for i in range(0,k):


plt.figure(2)
plt.hist(histograma[29,:],bins=1000)
plt.show()