#!/usr/bin/env python

import matplotlib
matplotlib.use('TkAgg')


from mpl_toolkits.mplot3d import  axes3d,Axes3D
import matplotlib.pyplot as plt
from matplotlib import cm
import numpy as np
from numpy import arange, sin, pi
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg, NavigationToolbar2Tk
from matplotlib.figure import Figure
from matplotlib.ticker import LinearLocator, FixedLocator, FormatStrFormatter

import tkinter as tk
import sys

class E(tk.Tk):
    def __init__(self,parent):
        tk.Tk.__init__(self,parent)
        self.parent = parent


        self.protocol("WM_DELETE_WINDOW", self.dest)
        self.main()

    def main(self):
        self.fig = plt.figure()
        self.fig = plt.figure(figsize=(3.5,3.5))

        self.frame = tk.Frame(self)
        self.frame.pack(padx=15,pady=15)

        self.canvas = FigureCanvasTkAgg(self.fig, master=self.frame)

        self.canvas.get_tk_widget().pack(side='top', fill='both')

        self.canvas._tkcanvas.pack(side='top', fill='both', expand=1)

        self.toolbar = NavigationToolbar2Tk( self.canvas, self )
        self.toolbar.update()
        self.toolbar.pack()

        self.btn = tk.Button(self,text='button',command=self.alt)
        self.btn.pack(ipadx=250)

        self.draw_sphere()

    def alt (self):
        self.draw_sphere(5)
    def dest(self):
        self.destroy()
        sys.exit()
    def draw_sphere(self, prop=10):
        self.fig.clear()
        ax = Axes3D(self.fig, auto_add_to_figure=False)
        self.fig.add_axes(ax)

        u = np.linspace(0, 2 * np.pi, 100)
        v = np.linspace(0, np.pi, 100)

        x = prop * np.outer(np.cos(u), np.sin(v))
        y = prop * np.outer(np.sin(u), np.sin(v))
        z = prop * np.outer(np.ones(np.size(u)), np.cos(v))

        t = ax.plot_surface(x, y, z, rstride=4, cstride=4,color='lightgreen',linewidth=0)
        self.canvas.draw()



if __name__ == "__main__":
    app = E(None)
    app.title('Embedding in TK')
    app.mainloop()