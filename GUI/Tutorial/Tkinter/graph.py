# How to plot real time serial data on Python: https://www.youtube.com/watch?v=0V-6pu1Gyp8

# import install library--> pip install matplotlib
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
from matplotlib.figure import Figure
import tkinter as tk
import numpy as np
import serial as sr

# global variables
data = np.array([])
cond = False

# plot data
# create a function that will be executed 
def plot_data():
    global cond, data # condition on the start button
    if (cond == True): # if it is true, plot data
        a = s.readline() # reading serial data on serial port
        a.decode() # decode serial data 
        if(len(data)<100): # it will append the data until the data lenght is 100
            # then after the first 100 values we are shifting the plot backwards
            data = np.append(data,float(a[0:4]))
        else:
            data[0:99] = data[1:100]
            data[99] = float(a[0:4])
        lines.set_xdata(np.arrange(0,len(data)))
        lines.set_ydata(data)

        canvas.draw()

    root.after(1,plot_data)

# Functions associated to Start and Stop buttons
def plot_start():
    global cond # we access the global variable "cond"
    cond = True # make condition equal to True
    s.reset_input_buffer() #if some garbage data is present on the serial port,we refresh it

def plot_stop():
    global cond
    cond = False

# Main GUI code
root = tk.Tk() # root object
root.title('Real Time Plot')
root.configure(background = 'lightblue') # background color
root.geometry("700x600") # set window size 

# Create a plot object on GUI
fig = Figure(); # create a figure object
ax = fig.add_subplot(111)

ax.set_title('Serial Data')
ax.set_xlabel('Sample')
ax.set_ylabel('Amplitude')
ax.set_xlim(0,100)
ax.set_ylim(-0.5,6)
lines = ax.plot([],[])[0] 

canvas = FigureCanvasTkAgg(fig, master = root) # because we want to display on root window
canvas.get_tk_widget().place(x = 10,y=10, width = 500, height = 400)
canvas.draw()

# Create button
root.update();
# start allows to plot when the button is clicked 
start = tk.Button(root,text = "Start", font = ('calibri',12), command = lambda: plot_start())
start.place(x=100,y=500) # positioning 

root.update()
stop = tk.Button(root,text = "Stop", font = ('calibri',12), command = lambda: plot_stop())
stop.place(x=start.winfo_x()+start.winfo_reqwidth()+20, y=500) # stop botton is positioned with respect to the start button
#(winfo_x gives the position location along x, winfo_reqwidt gives the object width, then we add 20)

# start serial port
s = sr.Serial('COM9',115200) # COM port and baudrate
s.reset_input_buffer() # ?? flash out


root.after(1,plot_data) # after 1 ms it starts executing the plot_data function
# (we need to implement it also inside plot_data)

root.mainloop() 




