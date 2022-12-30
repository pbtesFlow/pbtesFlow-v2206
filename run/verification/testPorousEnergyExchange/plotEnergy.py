#!/usr/bin/env python

import matplotlib.pyplot as plt
import numpy as np
import sys
import math
import subprocess
import os

os.system("rm -rf energyDataDir")

#Extracting body trajectory data from log
rc = subprocess.call("./extractEnergy.sh")
t = np.loadtxt('energyDataDir/t', unpack=True)
Hf = np.loadtxt('energyDataDir/Hf', unpack=True)
Hs = np.loadtxt('energyDataDir/Hs', unpack=True)
Htot = np.loadtxt('energyDataDir/Htot', unpack=True)
Hf0 = Hf[0]
Hs0 = Hs[0]
Htot0 = Htot[0]
nEl = min(len(t),len(Hs))
t = t[0:nEl]
Hf = (Hf[0:nEl] - 0*Hf0)/Htot0
Hs = (Hs[0:nEl] - 0*Hs0)/Htot0
Htot = (Htot[0:nEl] - 0*Htot0)/Htot0

#Plotting
plt.plot(t, Hf,'-b', linewidth=3)
plt.plot(t, Hs,'-r', linewidth=3)
plt.plot(t, Htot,'-k', linewidth=3)
plt.xlabel('time')
plt.ylabel('Relative enthalpy change')
#plt.set_aspect('equal', 'datalim')
#plt.set(xlim=(-.5, 1), ylim=(0, 3))
plt.title('Relative enthalpy evolution in test tank')
#plt.title(r'Zero mass ellipse with $v_x(0) = 1, v_y(0) = 0, \omega(0) = 1$')
plt.legend([r'$H^f(t)/H^{tot}(0)$',r'$H^s(t)/H^{tot}(0)$',r'$H^{tot}(t)/H^{tot}(0)$'],loc='upper left')
plt.show()










fig, ax1 = plt.subplots(3)
fig.suptitle('Pressure force vs theoretical for given body velocity')
ax1[0].plot(t1, Fx1,'.r')
ax1[0].plot(t2, Fx2,'.b')
ax1[0].plot(t3, Fx3,'.g')
ax1[0].set_ylabel('Fx')
ax1[0].legend(['Theory for given motion','floaterFoam','Exact orbit'])
#ax1[0].legend(['Theory given motion','floaterFoam'])
ax1[0].grid(True)
ax1[1].plot(t1, Fy1,'.r')
ax1[1].plot(t2, Fy2,'.b')
ax1[1].plot(t3, Fy3,'.g')
ax1[1].set_ylabel('Fy')
ax1[1].grid(True)
ax1[2].plot(t1, tau1,'.r')
ax1[2].plot(t2, tau2,'.b')
ax1[2].plot(t3, tau3,'.g')
ax1[2].set_ylabel('tau')
ax1[2].set_xlabel('time')
ax1[2].grid(True)
plt.show()
