import numpy as np
import matplotlib.pyplot as plt
import sys
import os
import pathlib

class PyFoamPlots(object):
    def __init__(self, *args):
        self.path_cwd = pathlib.Path(__file__).parent.resolve()
        self.markers=['o', '*']
        self.colours=['red','blue']

    def plot_sets(self, FO_name, file_name, columns):
        path_FO = self.path_cwd/'postProcessing'/FO_name
        n_timedirs = len(list(pathlib.Path(path_FO).iterdir()))
        timedirs = sorted(pathlib.Path(path_FO).iterdir(), key=os.path.getmtime)

        fig, axes = plt.subplots(nrows=2, ncols=1, sharex='col')
        plt.subplots_adjust(hspace=0.05)
        for t, time in enumerate(timedirs):
            data = np.loadtxt(time/file_name)
            for i in range(np.shape(data)[1]-1):
                axes[i].plot(data[:,0],data[:,i+1], color=self.colours[i], alpha=1-0.75*t/(n_timedirs-1))#, label='%i'%float(time.name))
                axes[i].grid(alpha=0.2,color="black")
                axes[i].set_xlabel(columns[0])
                axes[i].set_ylabel(columns[i+1])

        plt.savefig(self.path_cwd/'plots'/FO_name)
        
if __name__ == '__main__':
    OFPlots = PyFoamPlots()
    OFPlots.plot_sets(FO_name='sample', file_name='centre_Tf_Ts.xy', columns={0:'y-position (m)', 1:'Fluid temperature (K)', 2:'Solid temperature (K)'})