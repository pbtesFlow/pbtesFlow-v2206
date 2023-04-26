import numpy as np
import matplotlib.pyplot as plt
import pathlib
from natsort import natsorted

class PyFoamPlots(object):
    def __init__(self, *args):
        self.path_cwd = pathlib.Path(__file__).parent.resolve()

    def plot_sets_contour(self, FO_name, file_name, data, time_limits):
        path_FO = self.path_cwd/'postProcessing'/FO_name
        pathlib.Path(self.path_cwd/'plots'/FO_name).mkdir(parents=True, exist_ok=True)
        timedirs = natsorted(pathlib.Path(path_FO).iterdir(), key=str)

        for i,name in enumerate(data):
            if i==0:
                continue
                
            x = [float(x.parts[-1]) for x in timedirs]
            y = np.loadtxt(timedirs[0]/file_name)[:,0]
            xv,yv = np.meshgrid(x,y)
            zv = np.array([np.loadtxt(x/file_name)[:,i] for x in timedirs])

            cmap = plt.get_cmap('coolwarm')
            fig, ax = plt.subplots()
            CS = ax.contourf(xv,yv,zv.transpose(), cmap=cmap, levels=10)
            CS = ax.contour(xv,yv,zv.transpose(),levels=10, alpha=.5, colors='k')
            ax.clabel(CS, CS.levels, inline=True, fontsize=10)
            ax.grid()
            ax.set_title(name)
            ax.set_xlabel('Time (s)')
            ax.set_ylabel(data[0])
            filename_cleared = "".join(c for c in name if c.isalpha())
            plt.savefig(self.path_cwd/'plots'/FO_name/filename_cleared)

if __name__ == '__main__':
    pfp = PyFoamPlots()
    pfp.plot_sets_contour(FO_name='sample', 
                          file_name='centre_Tf_Ts.xy', 
                          data=['y-position (m)', 'Fluid Phase Temperature (K)', 'Solid Phase Temperature (K)'],
                          time_limits=[0,1500])