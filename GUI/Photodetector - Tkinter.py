# pip install unicodedata
# pip install text-unidecode
# pip3 install customtkinter

import unicodedata
import text_unidecode
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
from matplotlib.figure import Figure
from tkinter import *
import tkinter.messagebox 
import customtkinter
import numpy as np
import serial as sr
import serial.tools.list_ports

from PIL import ImageTk,Image   # for image management 

#################################
# AUTOMATIC COM PORT CONNECTION #
#################################
def findPsoC(portsFound):
    commPort = 'None'
    n_connections = len(portsFound)

    for i in range(0,n_connections):
        port = portsFound[i]
        strPort = str(port)
        #print(strPort)

        if 'COM3' in strPort: #KitProg #COM3
            splitPort = strPort.split(' ')
            commPort = (splitPort[0])

    return commPort

#############
# PLOT DATA #
#############
data = np.array([])
cond = False

def plot_data():
    global cond, data

    if (cond == True):

        a = s.readline()
        a.decode()
        print(int(a[0:4]))

        if (len(data) < 100):
            data = np.append(data, int(a[0:4]))
        else:
            data[0:99] = data[1:100]
            data[99] = int(a[0:4])

        lines.set_xdata(np.arange(0, len(data)))
        lines.set_ydata(data)

        canvas.draw()

    root.after(1, plot_data)

def plot_start():
    global cond
    cond = True
    s.reset_input_buffer()

def plot_stop():
    global cond
    cond = False

############
# MAIN GUI #
############
root =  Tk()
root.title("Photodetector")
root.iconbitmap("D:\Polimi\LAB\PROJECT\AY2122_II_Project-1\GUI/hr_icon.ico")
root.geometry("1077x700")  # set the window size 
root.configure(background='#1f1f1f')

# -----FRAMES----#
# configure grid layout (2x1)
root.grid_columnconfigure(1, weight=1)
root.grid_rowconfigure(0, weight=1)

root.frame_left = customtkinter.CTkFrame(master=root,
                                         width=180,
                                         fg_color='#292929',
                                         corner_radius=0)
root.frame_left.grid(row=0, column=0, sticky="nswe")

root.frame_right = customtkinter.CTkFrame(master=root,
                                          fg_color='#292929',)
root.frame_right.grid(row=0, column=1, sticky="nswe", padx=20, pady=20)

# configure grid layout (1x11)
root.frame_left.grid_rowconfigure(0, minsize=10)   # empty row with minsize as spacing
root.frame_left.grid_rowconfigure(5, weight=1)     # empty row as spacing
root.frame_left.grid_rowconfigure(8, minsize=20)   # empty row with minsize as spacing
root.frame_left.grid_rowconfigure(11, minsize=10)  # empty row with minsize as spacing

# Start serial port
ports = serial.tools.list_ports.comports()
connectPort = findPsoC(ports)
if connectPort != 'None':
    s = serial.Serial(connectPort, baudrate = 115200, timeout = 1)
    print('Connected to ' + connectPort)
    root.label_connected = customtkinter.CTkLabel(master=root.frame_left,
                                                text="Connected",
                                                text_color='white',
                                                text_font=("Roboto Medium", -12))  # font name and size in px
    root.label_connected.grid(row=1, column=0, pady=10, padx=10)
    s.reset_input_buffer()
else: 
    print('Error in the connection with PSoC')



# -----START BUTTON----#
root.update()
button_start = customtkinter.CTkButton(master=root.frame_left,
                                       height=25,
                                       text="Start",
                                       text_font=("Roboto Medium",-12),             
                                       text_color='white',
                                       fg_color='#4d4d4d', #azzurro #3373b8 
                                       hover_color='#1d538d',
                                       command=lambda: plot_start()          
                                       )
button_start.grid(row=2, column=0, pady=10, padx=20)

# -----STOP BUTTON----#
root.update()
button_stop = customtkinter.CTkButton(master=root.frame_left,
                                      height=25,
                                      text="Stop",
                                      text_font=("Roboto Medium",-12),             
                                      text_color='white',
                                      fg_color='#4d4d4d', #azzurro #3373b8 
                                      hover_color='#1d538d', 
                                      command=lambda: plot_stop()          
                                      )
button_stop.grid(row=3, column=0, pady=10, padx=20)                                                   

# ---- HEART RATE LOGO ----#
logo_image = Image.open("D:\Polimi\LAB\PROJECT\AY2122_II_Project-1\GUI/heart_rate_line_reduced.png")
#resize_logo_image = logo_image.resize((180, 92))
img = ImageTk.PhotoImage(logo_image)
image_label = Label(image=img,borderwidth=0) # define label containing the image
image_label.image = img
image_label.place(x=4, y=160)

# ------PLOT OBJECT------#
# Add figure canvas
fig = Figure()
ax = fig.add_subplot(111)

# ax = plt.axes(xlim=(0,100),ylim=(0, 120)); #displaying only 100 samples
ax.set_title('Serial Data')
ax.set_xlabel('Sample')
ax.set_ylabel('Voltage')
ax.set_xlim(0, 100)
ax.set_ylim(-0.5, 3500)
lines = ax.plot([], [])[0]

canvas = FigureCanvasTkAgg(fig, master=root)  # A tk.DrawingArea.
canvas.get_tk_widget().place(x=300, y=10, width=500, height=400)
canvas.draw()


root.after(1, plot_data)
root.mainloop()