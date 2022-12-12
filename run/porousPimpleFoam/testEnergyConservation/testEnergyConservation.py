import numpy as np
import matplotlib.pyplot as plt

time, Tf, Ts = np.loadtxt('postProcessing/volFieldValue1/0/volFieldValue.dat', unpack=True)

rhof = 1.2
rhos = 5150

cpf = 700
cps = 1130

V = 1

porosity = 0.5

print("Total enthalpy:", Tf*rhof*cpf + Ts*rhos*cps)

plt.plot(time,Tf*rhof*cpf*porosity*V, time,Ts*rhos*cps*(1-porosity)*V, time, (Ts*rhos*cps*porosity*V+Tf*rhof*cp*(1-porosity)*V)/2)
plt.savefig('foo.pdf')