########################################
# LTEBS - A.Y. 2021/2022, II semester  #
# Project 01                           #
# Authors:                             #
# Emanuele Falli                       #
# Federico Monterosso                  #
# Francesca Terranova                  #
########################################

'''
READ ME
Two graphical elements are present
The lines of code related to them are commented
if you to visualize the logo and the image you need to uncomment the following lines and insert the
location of the images that are uploaded on GitHub:
- from line 243 to 249
- line 170
'''

######################
# IMPORT LIBRARIES  #
#####################

import unicodedata
import text_unidecode
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg, NavigationToolbar2Tk
from matplotlib.figure import Figure
from tkinter import *
import tkinter.messagebox
import customtkinter
import numpy as np
import serial.tools.list_ports
import serial
from math import sin, cos
from PIL import ImageTk, Image

#################################
# AUTOMATIC COM PORT CONNECTION  #
#################################
''' 
It allows to use the pySerial library in order to analyse which are the ports open on the PC, to identify if the
KIT-059 is connected and - if this happens - then performing the actual serial connecton with it.
'''

class ReadLine:
    def __init__(self, s):
        self.buf = bytearray()
        self.s = s

    def readline(self):
        i = self.buf.find(b"\n")
        if i >= 0:
            r = self.buf[:i + 1]
            self.buf = self.buf[i + 1:]
            return r
        while True:
            i = max(1, min(2048, self.s.in_waiting))
            data = self.s.read(i)
            i = data.find(b"\n")
            if i >= 0:
                r = self.buf + data[:i + 1]
                self.buf[0:] = data[i + 1:]
                return r
            else:
                self.buf.extend(data)

def findPsoC(portsFound):
    commPort = 'None'
    n_connections = len(portsFound)

    for i in range(0, n_connections):
        port = portsFound[i]
        strPort = str(port)
        
        if 'KitProg' in strPort:  
            splitPort = strPort.split(' ')
            commPort = (splitPort[0])

    return commPort


######################
# READ AND PLOT DATA #
######################
'''
The plot_data function allows to read serial data and distinguish the different types (IR channel, red channel, SpO2 
values, HR values) on the basis of a label that is transmitted together with the data
The IR channel and red channel values are shown in a graph while SpO2 and HR values are computed numerically.
'''

ir = np.array([])
red = np.array([])
cond = False
flag = 0

def plot_data():
    global cond, ir, red, flag

    if (cond == True):

        a = s.readline() 
        a.decode('utf-8')
        b = a.split(b',')

        # IR channel
        if int(b[0]) == 1:   
            if len(ir) < 200:
                ir = np.append(ir, int(b[1][0:7]))

            else:
                ir[0:199] = ir[1:200]
                ir[199] = int(b[1][0:7])

            lines_ir.set_xdata(np.arange(0, len(ir)))
            lines_ir.set_ydata(ir)

        # Red channel
        if int(b[2]) == 2:
            if len(red) < 200:
                red = np.append(red, int(b[3][0:7]))

            else:
                red[0:199] = red[1:200]
                red[199] = int(b[3][0:7])

            lines_red.set_xdata(np.arange(0, len(red)))
            lines_red.set_ydata(red)

        # SpO2 values
        if int(b[4]) == 3:
            root.label_values_SPO2.configure(text=str(int(b[5])))

        # HR values
        if int(b[6]) == 4:
            root.label_values_HR.configure(text=str(int(b[7])))

        # print(data)
        canvas.draw()

    root.after(1, plot_data)


'''
plot_start and plot_stop allows to start and stop the visualization of the graphs
They are connected to the corresponding buttons
'''

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
''''
The GUI is developed using Tk interface
The window is composd by a grid layout made of 2 main columns
The grid structure is used to decide where to place the different GUI elements
'''

root = Tk()
root.title("Photodetector")
#root.iconbitmap("hr_icon.ico")  # icon
root.geometry("1077x700")       # window size
root.minsize(1077, 700)         # to maintain dimensions fixed
root.maxsize(1077, 700)
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
                                          fg_color='#292929', )
root.frame_right.grid(row=0, column=1, sticky="nswe", padx=20, pady=20)

# configure grid layout (1x11)
root.frame_left.grid_rowconfigure(0, minsize=10)
root.frame_left.grid_rowconfigure(4, minsize=245)
root.frame_right.grid_rowconfigure(0, weight=60)
root.frame_right.grid_columnconfigure(0, weight=3)
root.frame_right.grid_columnconfigure(1, weight=24)  

# The label "Connected" is shown when the KIT-095 is detected on the COM port, while if it is not the label "Not connected" appears
ports = serial.tools.list_ports.comports()
connectPort = findPsoC(ports)
if connectPort != 'None':
    s = serial.Serial(connectPort, baudrate=115200, timeout=1)
    root.label_connected = customtkinter.CTkLabel(master=root.frame_left,
                                                  text="Connected",
                                                  text_color='#666666',
                                                  text_font=("Roboto Medium", -12))
    root.label_connected.grid(row=1, column=0, pady=10, padx=10)
    s.reset_input_buffer()
else:
    root.label_connected = customtkinter.CTkLabel(master=root.frame_left,
                                                  text="Not connected",
                                                  text_color='#666666',
                                                  text_font=("Roboto Medium", -12))
    root.label_connected.grid(row=1, column=0, pady=10, padx=10)

# -----START BUTTON----#
root.update()
button_start = customtkinter.CTkButton(master=root.frame_left,
                                       height=25,
                                       text="Start",
                                       text_font=("Roboto Medium", -12),
                                       text_color='white',
                                       fg_color='#4d4d4d',
                                       hover_color='#1d538d',
                                       command=lambda: plot_start()
                                       )
button_start.grid(row=2, column=0, pady=10, padx=20)

# -----STOP BUTTON----#
root.update()
button_stop = customtkinter.CTkButton(master=root.frame_left,
                                      height=25,
                                      text="Stop",
                                      text_font=("Roboto Medium", -12),
                                      text_color='white',
                                      fg_color='#4d4d4d',  
                                      hover_color='#1d538d',
                                      command=lambda: plot_stop()
                                      )
button_stop.grid(row=3, column=0, pady=10, padx=20)

'''
# ---- HEART RATE LOGO ----#
logo_image = Image.open("heart_rate_line.png")
#resize_logo_image = logo_image.resize((180, 92))
img = ImageTk.PhotoImage(logo_image)
image_label = Label(image=img,borderwidth=0) # define label containing the image
image_label.image = img
image_label.place(x=4, y=160)
'''

# ---- SLIDERS ----#

# LED pulse width (micros)
root.label_LED_PW_title = customtkinter.CTkLabel(master=root.frame_right,
                                                 text='LED Pulse Width (µs)',
                                                 text_color='#666666',
                                                 width=30,
                                                 text_font=("Roboto Medium", -12))  
root.label_LED_PW_title.place(x=104, y=481, anchor=W)


def values_LED_PW(value):
    #global value_LED_PW
    value_LED_PW = root.slider_LED_PW.get()
    if value_LED_PW == 69:
        value_LED_PW_to_use = 69
        value_PSOC = 'i'
    elif value_LED_PW == 183.0:
        value_LED_PW_to_use = 118
        value_PSOC = 'l'
    elif value_LED_PW == 297.0:
        value_LED_PW_to_use = 215
        value_PSOC = 'm'
    elif value_LED_PW == 411:
        value_LED_PW_to_use = 411
        value_PSOC = 'n'

    root.label_LED_PW = customtkinter.CTkLabel(master=root.frame_right,
                                               text=str(value_LED_PW_to_use),
                                               text_color='#dcd8d8',
                                               width=50,
                                               text_font=("Roboto Medium", -12))  
    root.label_LED_PW.place(x=302, y=498, anchor=W)
    s.write(str(value_PSOC).encode('ascii'))

    return value_LED_PW_to_use

root.slider_LED_PW = customtkinter.CTkSlider(master=root.frame_right,
                                             from_=69,
                                             to=411,
                                             number_of_steps=3,
                                             progress_color='#666666',
                                             button_color='#3373b8',
                                             fg_color='#000000',
                                             command=values_LED_PW
                                             )
root.slider_LED_PW.set(183.0)  # 118 (Initalize value)

root.slider_LED_PW.grid(row=2, column=0, columnspan=1, pady=15, padx=55, sticky="we")

# Samples per second
root.label_SAMPLES_title = customtkinter.CTkLabel(master=root.frame_right,
                                                  text='Samples per second',
                                                  text_color='#666666',
                                                  width=30,
                                                  text_font=("Roboto Medium", -12))  # font name and size in px
root.label_SAMPLES_title.place(x=100, y=527, anchor=W)

def values_SAMPLES(value):
    value_SAMPLES = root.slider_SAMPLES.get()
    if value_SAMPLES == 50.0:
        value_SAMPLES_to_use = 50
        value_PSOC = 'e'
    elif value_SAMPLES == 166.66666666666666:
        value_SAMPLES_to_use = 100
        value_PSOC = 'f'
    elif value_SAMPLES == 283.3333333333333:
        value_SAMPLES_to_use = 200
        value_PSOC = 'g'
    elif value_SAMPLES == 400.0:
        value_SAMPLES_to_use = 400
        value_PSOC = 'h'

    root.label_SAMPLES = customtkinter.CTkLabel(master=root.frame_right,
                                                text=str(value_SAMPLES_to_use),
                                                text_color='#dcd8d8',
                                                width=30,
                                                text_font=("Roboto Medium", -12))  # font name and size in px
    root.label_SAMPLES.place(x=305, y=545, anchor=W)
    s.write(str(value_PSOC).encode('ascii'))

    return value_SAMPLES_to_use


root.slider_SAMPLES = customtkinter.CTkSlider(master=root.frame_right,
                                              from_=50,
                                              to=400,
                                              number_of_steps=3,
                                              progress_color='#666666',
                                              button_color='#3373b8',
                                              fg_color='#000000',
                                              command=values_SAMPLES
                                              )
root.slider_SAMPLES.set(50)  # 50 (Initalize value)
root.slider_SAMPLES.grid(row=3, column=0, columnspan=1, pady=15, padx=55, sticky="we")

# LED current control (mA)
root.label_LED_CURRENT_title = customtkinter.CTkLabel(master=root.frame_right,
                                                      text='LED Current Control (mA)',
                                                      text_color='#666666',
                                                      width=30,
                                                      text_font=("Roboto Medium", -12))  # font name and size in px
root.label_LED_CURRENT_title.place(x=90, y=574, anchor=W)

def values_LED_CURRENT(value):
    value_LED_CURRENT_to_use = root.slider_LED_CURRENT.get()
    value_LED_CURRENT_to_use = float(f'{value_LED_CURRENT_to_use:.2f}')
    if value_LED_CURRENT_to_use == 0.2:
        value_PSOC = 'o'
    if value_LED_CURRENT_to_use == 0.6:
        value_PSOC = 'p'
    if value_LED_CURRENT_to_use == 1.0:
        value_PSOC = 'q'        
    if value_LED_CURRENT_to_use == 1.4:
        value_PSOC = 'r'
    if value_LED_CURRENT_to_use == 1.8:
        value_PSOC = 's'
    if value_LED_CURRENT_to_use == 2.2:
        value_PSOC = 't'
    if value_LED_CURRENT_to_use == 2.6:
        value_PSOC = 'u'
    if value_LED_CURRENT_to_use == 3.0:
        value_PSOC = 'v'        
    if value_LED_CURRENT_to_use == 3.4:
        value_PSOC = 'z'
    if value_LED_CURRENT_to_use == 3.8:
        value_PSOC = 'A'
    if value_LED_CURRENT_to_use == 4.2:
        value_PSOC = 'B'
    if value_LED_CURRENT_to_use == 4.6:
        value_PSOC = 'C'        
    if value_LED_CURRENT_to_use == 5.0:
        value_PSOC = 'D'
    if value_LED_CURRENT_to_use == 5.4:
        value_PSOC = 'E'
    if value_LED_CURRENT_to_use == 5.8:
        value_PSOC = 'F'
    if value_LED_CURRENT_to_use == 6.2:
        value_PSOC = 'G'

    root.label_LED_CURRENT = customtkinter.CTkLabel(master=root.frame_right,
                                                    text=str(value_LED_CURRENT_to_use),
                                                    text_color='#dcd8d8',
                                                    width=30,
                                                    text_font=("Roboto Medium", -12))  
    root.label_LED_CURRENT.place(x=305, y=590, anchor=W)
    s.write(str(value_PSOC).encode('ascii'))

    return value_LED_CURRENT_to_use


root.slider_LED_CURRENT = customtkinter.CTkSlider(master=root.frame_right,
                                                  from_=0.2,
                                                  to=6.2,
                                                  number_of_steps=15,
                                                  progress_color='#666666',
                                                  button_color='#3373b8',
                                                  fg_color='#000000',
                                                  command=values_LED_CURRENT
                                                  )
root.slider_LED_CURRENT.set(2.0)  # 2 mA (Initalize value)
root.slider_LED_CURRENT.grid(row=4, column=0, columnspan=1, pady=15, padx=55, sticky="we")

# SpO2 ADC range
root.label_SPO2_title = customtkinter.CTkLabel(master=root.frame_right,
                                               text='SpO₂ ADC Range',
                                               text_color='#666666',
                                               width=30,
                                               text_font=("Roboto Medium", -12))  
root.label_SPO2_title.place(x=110, y=620, anchor=W)


def values_SPO2(value):
    value_SPO2 = root.slider_SPO2.get()
    value_SPO2_to_use = value_SPO2
    if value_SPO2 == 2048.0:
        value_SPO2_to_use = 2048
        value_PSOC = 'a'
    elif value_SPO2 == 6826.666666666666:
        value_SPO2_to_use = 4096
        value_PSOC = 'b'
    elif value_SPO2 == 11605.333333333332:
        value_SPO2_to_use = 8192
        value_PSOC = 'c'
    elif value_SPO2 == 16384.0:
        value_SPO2_to_use = 16384
        value_PSOC = 'd'


    root.label_SPO2 = customtkinter.CTkLabel(master=root.frame_right,
                                             text=str(value_SPO2_to_use),
                                             text_color='#dcd8d8',
                                             width=30,
                                             text_font=("Roboto Medium", -12))  
    root.label_SPO2.place(x=305, y=636, anchor=W)
    s.write(str(value_PSOC).encode('ascii'))

    return value_SPO2_to_use


root.slider_SPO2 = customtkinter.CTkSlider(master=root.frame_right,
                                           from_=2048,
                                           to=16384,
                                           number_of_steps=3,
                                           progress_color='#666666',
                                           button_color='#3373b8',
                                           fg_color='#000000',
                                           command=values_SPO2
                                           )
root.slider_SPO2.set(6826.666666666666)  # 4096 (Initalize value)
root.slider_SPO2.grid(row=5, column=0, columnspan=1, pady=15, padx=55, sticky="we")

# ------PLOT OBJECT------#
# Background plot
root.label_bg_plot = customtkinter.CTkLabel(master=root.frame_right,
                                            text="",
                                            height=360,
                                            width=750,
                                            fg_color='#f0f0f0', #dcd8d8
                                            justify=tkinter.LEFT)
root.label_bg_plot.place(x=66, y=230, anchor=W)

# Add figure canvas
fig = Figure()
ax = fig.add_subplot(111)
fig.patch.set_facecolor('#f0f0f0')
fig.subplots_adjust(bottom=0.19, right=0.94)


ax.title.set_visible(False)
ax.set_xlabel('Sample')
ax.set_ylabel('Digit')
ax.set_xlim(5, 220)
ax.set_ylim(0, 110000)
ax.set_facecolor('#f0f0f0')

lines_ir = ax.plot([], [], 'r')[0]
lines_red = ax.plot([], [], 'b')[0]

canvas = FigureCanvasTkAgg(fig, master=root)  
canvas.get_tk_widget().place(x=255, y=75, width=730, height=350)
canvas.draw()
toolbar = NavigationToolbar2Tk(canvas, root, pack_toolbar=False)
toolbar.update()
toolbar.place(x=500, y=70)

root.after(1, plot_data)

# VALUES
# -----SPO2 LABEL----#
root.label_SPO2_values_title = customtkinter.CTkLabel(master=root.frame_right,
                                                      height=25,
                                                      text="SpO₂",
                                                      text_font=("Roboto Medium", -12),
                                                      text_color='white',
                                                      fg_color='#4d4d4d',
                                                      )
root.label_SPO2_values_title.place(x=465, y=485)

# -----HEART RATE LABEL----#
root.label_HR_values_title = customtkinter.CTkLabel(master=root.frame_right,
                                                    height=25,
                                                    text="Heart Rate",
                                                    text_font=("Roboto Medium", -12),
                                                    text_color='white',
                                                    fg_color='#4d4d4d',
                                                    )
root.label_HR_values_title.place(x=650, y=485)

root.label_values_SPO2 = customtkinter.CTkButton(master=root.frame_right,
                                                 text=". . .",
                                                 height=90,
                                                 width=120,
                                                 fg_color='#666666',
                                                 hover_color='#666666',
                                                 )
root.label_values_SPO2.place(x=465, y=520)

root.label_values_HR = customtkinter.CTkButton(master=root.frame_right,
                                               text=". . .",
                                               height=90,
                                               width=120,
                                               fg_color='#666666',
                                               hover_color='#666666',
                                               )
root.label_values_HR.place(x=650, y=520)

# WIDGET TITLES
root.label_settings = customtkinter.CTkLabel(master=root.frame_right,
                                             text="Settings",
                                             text_color='white',
                                             text_font=("Roboto Medium", -14),
                                             fg_color='#3373b8',
                                             corner_radius=6
                                             )
root.label_settings.grid(row=1, column=0, columnspan=1, pady=15, padx=10)

# Data values
root.label_data_values = customtkinter.CTkLabel(master=root.frame_right,
                                                text="Values",
                                                text_color='white',
                                                text_font=("Roboto Medium", -14),
                                                fg_color='#3373b8',
                                                corner_radius=6
                                                )
root.label_data_values.grid(row=1, column=1, columnspan=1, pady=15, padx=10)

# Plot
root.label_data_values = customtkinter.CTkLabel(master=root.frame_right,
                                                text="Plot",
                                                text_color='white',
                                                text_font=("Roboto Medium", -14),
                                                fg_color='#3373b8',
                                                corner_radius=6
                                                )
root.label_data_values.place(x=375, y=25, anchor=W)

root.mainloop()