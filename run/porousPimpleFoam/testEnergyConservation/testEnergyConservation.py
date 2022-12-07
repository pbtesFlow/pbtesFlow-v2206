import numpy as np
import matplotlib.pyplot as plt

time, Tf, Ts = np.loadtxt('postProcessing/volFieldValue1/0/volFieldValue.dat', unpack=True)

rhof = 1.2
rhos = 3000

cpf = 700
cps = 0.1

print("Total enthalpy:", Tf*rhof*cpf + Ts*rhos*cps)

plt.plot(time,Tf*rhof*cpf, time,Ts*rhos*cps, time, (Ts*rhos*cps+Tf*rhof*cpf)/2)
plt.show()