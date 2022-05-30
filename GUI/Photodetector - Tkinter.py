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
from math import sin, cos

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
    global cond, button_SPO2, button_HR, label_parameters
    cond = True
    s.reset_input_buffer()

    # Buttons that appear when "Start" is clicked
    # ----- Title - Parameters ----#
    label_parameters = customtkinter.CTkLabel(master=root.frame_left,
                                                text="Parameters",
                                                width = 8,
                                                text_color='white',
                                                text_font=("Roboto Medium", -14),
                                                fg_color = '#3373b8',
                                                corner_radius = 6
                                                )  
    label_parameters.grid(row=5, column=0, columnspan=1, pady=5, padx=10)

    # -----SPO2 BUTTON----#
    root.update()
    button_SPO2 = customtkinter.CTkButton(master=root.frame_left,
                                        height=25,
                                        text="SpO₂",
                                        text_font=("Roboto Medium",-12),             
                                        text_color='white',
                                        fg_color='#4d4d4d',
                                        hover_color='#1d538d',
                                        #command=lambda: plot_start()          
                                        )
    button_SPO2.grid(row=6, column=0, pady=6, padx=20)

    # -----HEART RATE BUTTON----#
    root.update()
    button_HR = customtkinter.CTkButton(master=root.frame_left,
                                        height=25,
                                        text="Heart Rate",
                                        text_font=("Roboto Medium",-12),             
                                        text_color='white',
                                        fg_color='#4d4d4d',
                                        hover_color='#1d538d',
                                        #command=lambda: plot_start()          
                                        )
    button_HR.grid(row=7, column=0, pady=6, padx=20)
    

def plot_stop():
    global cond
    cond = False
    button_HR.grid_remove()
    button_SPO2.grid_remove()
    label_parameters.grid_remove()

############
# MAIN GUI #
############
root =  Tk()
root.title("Photodetector")
root.iconbitmap("D:\Polimi\LAB\PROJECT\AY2122_II_Project-1\GUI/hr_icon.ico")
root.geometry("1077x700")  # set the window size 
root.minsize(1077, 700) # to maintain dimensions fixed
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
                                          fg_color='#292929',)
root.frame_right.grid(row=0, column=1, sticky="nswe", padx=20, pady=20)

# configure grid layout (1x11)
root.frame_left.grid_rowconfigure(0, minsize=10)  
root.frame_left.grid_rowconfigure(4, minsize=245)   
root.frame_right.grid_rowconfigure(0, weight=60)
root.frame_right.grid_columnconfigure(0, weight=3)
root.frame_right.grid_columnconfigure(1, weight=24) #24

# Start serial port
ports = serial.tools.list_ports.comports()
connectPort = findPsoC(ports)
if connectPort != 'None':
    s = serial.Serial(connectPort, baudrate = 115200, timeout = 1)
    print('Connected to ' + connectPort)
    root.label_connected = customtkinter.CTkLabel(master=root.frame_left,
                                                text="Connected",
                                                text_color='#666666',
                                                text_font=("Roboto Medium", -12))  
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
                                       fg_color='#4d4d4d',
                                       hover_color='#1d538d',
                                       command=lambda: plot_start()          
                                       )
root.bind("b", plot_start)  # command of b start data streaming
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
root.bind("s", plot_stop)  # command of b start data streaming
button_stop.grid(row=3, column=0, pady=10, padx=20)    


# ---- HEART RATE LOGO ----#
logo_image = Image.open("D:\Polimi\LAB\PROJECT\AY2122_II_Project-1\GUI/heart_rate_line_reduced.png")
#resize_logo_image = logo_image.resize((180, 92))
img = ImageTk.PhotoImage(logo_image)
image_label = Label(image=img,borderwidth=0) # define label containing the image
image_label.image = img
image_label.place(x=4, y=160)


# ---- SLIDERS ----# 

# LED pulse width (micros)
root.label_LED_PW_title = customtkinter.CTkLabel(master=root.frame_right,
                                                text='LED Pulse Width (µs)',
                                                text_color='#666666',
                                                width = 30,
                                                text_font=("Roboto Medium", -12))  # font name and size in px
root.label_LED_PW_title.place(x=104, y=481, anchor = W)

def values_LED_PW(value):
    value_LED_PW = root.slider_LED_PW.get()  
    if value_LED_PW == 69:
        value_LED_PW_to_use = 69
    elif value_LED_PW == 183.0:
        value_LED_PW_to_use = 118
    elif value_LED_PW == 297.0:
        value_LED_PW_to_use = 215      
    elif value_LED_PW == 411:
        value_LED_PW_to_use = 411
 
    root.label_LED_PW = customtkinter.CTkLabel(master=root.frame_right,
                                                text=str(value_LED_PW_to_use),
                                                text_color='#dcd8d8',
                                                width = 50,
                                                text_font=("Roboto Medium", -12))  # font name and size in px
    root.label_LED_PW.place(x=275, y=498, anchor = W)
    return value_LED_PW_to_use

root.slider_LED_PW = customtkinter.CTkSlider(master=root.frame_right,
                                             from_=69,
                                             to=411,
                                             number_of_steps=3,
                                             progress_color='#666666',
                                             button_color='#3373b8',
                                             fg_color='#000000',
                                             command = values_LED_PW
                                            )
root.slider_LED_PW.set(183.0) #118 (Initalize value)
root.slider_LED_PW.grid(row=2, column=0, columnspan=1, pady=15, padx=55, sticky="we")

# Samples per second
root.label_SAMPLES_title = customtkinter.CTkLabel(master=root.frame_right,
                                                text='Samples per second',
                                                text_color='#666666',
                                                width = 30,
                                                text_font=("Roboto Medium", -12))  # font name and size in px
root.label_SAMPLES_title.place(x=100, y=527, anchor = W)

def values_SAMPLES(value):
    value_SAMPLES = root.slider_SAMPLES.get() 
    if value_SAMPLES == 50.0:
        value_SAMPLES_to_use = 50
    elif value_SAMPLES == 166.66666666666666:
        value_SAMPLES_to_use = 100
    elif value_SAMPLES == 283.3333333333333:
        value_SAMPLES_to_use = 200      
    elif value_SAMPLES == 400.0:
        value_SAMPLES_to_use = 400

    root.label_SAMPLES= customtkinter.CTkLabel(master=root.frame_right,
                                                text=str(value_SAMPLES_to_use),
                                                text_color='#dcd8d8',
                                                width = 30,
                                                text_font=("Roboto Medium", -12))  # font name and size in px
    root.label_SAMPLES.place(x=280, y=545, anchor = W)
    return value_SAMPLES_to_use

root.slider_SAMPLES = customtkinter.CTkSlider(master=root.frame_right,
                                              from_=50,
                                              to=400,
                                              number_of_steps=3,
                                              progress_color='#666666',
                                              button_color='#3373b8',
                                              fg_color='#000000',
                                              command = values_SAMPLES
                                             )
root.slider_SAMPLES.set(50) #50 (Initalize value)
root.slider_SAMPLES.grid(row=3, column=0, columnspan=1, pady=15, padx=55, sticky="we")

# LED current control (mA)
root.label_LED_CURRENT_title = customtkinter.CTkLabel(master=root.frame_right,
                                                text='LED Current Control (mA)',
                                                text_color='#666666',
                                                width = 30,
                                                text_font=("Roboto Medium", -12))  # font name and size in px
root.label_LED_CURRENT_title.place(x=90, y=574, anchor = W)

def values_LED_CURRENT(value):
    value_LED_CURRENT_to_use = root.slider_LED_CURRENT.get()
    value_LED_CURRENT_to_use = float(f'{value_LED_CURRENT_to_use:.2f}')

    root.label_LED_CURRENT= customtkinter.CTkLabel(master=root.frame_right,
                                                text=str(value_LED_CURRENT_to_use),
                                                text_color='#dcd8d8',
                                                width = 30,
                                                text_font=("Roboto Medium", -12))  # font name and size in px
    root.label_LED_CURRENT.place(x=280, y=590, anchor = W)
    return value_LED_CURRENT_to_use

root.slider_LED_CURRENT = customtkinter.CTkSlider(master=root.frame_right,
                                                  from_=0.2,
                                                  to=6.2,
                                                  number_of_steps = 15,
                                                  progress_color='#666666',
                                                  button_color='#3373b8',
                                                  fg_color='#000000',
                                                  command = values_LED_CURRENT
                                                 )
root.slider_LED_CURRENT.set(2.0) #2 mA (Initalize value)
root.slider_LED_CURRENT.grid(row=4, column=0, columnspan=1, pady=15, padx=55, sticky="we")

# SpO2 ADC range 
root.label_SPO2_title = customtkinter.CTkLabel(master=root.frame_right,
                                                text='SpO₂ ADC Range',
                                                text_color='#666666',
                                                width = 30,
                                                text_font=("Roboto Medium", -12))  # font name and size in px
root.label_SPO2_title.place(x=110, y=620, anchor = W)

def values_SPO2(value):
    value_SPO2 = root.slider_SPO2.get() 
    value_SPO2_to_use = value_SPO2
    if value_SPO2 == 2048.0:
        value_SPO2_to_use = 2048
    elif value_SPO2 == 6826.666666666666:
        value_SPO2_to_use = 4096
    elif value_SPO2 == 11605.333333333332:
        value_SPO2_to_use = 8192       
    elif value_SPO2 == 16384.0:
        value_SPO2_to_use = 16384  

    root.label_SPO2= customtkinter.CTkLabel(master=root.frame_right,
                                            text=str(value_SPO2_to_use),
                                            text_color='#dcd8d8',
                                            width = 30,
                                            text_font=("Roboto Medium", -12))  # font name and size in px
    root.label_SPO2.place(x=280, y=636, anchor = W)
    return value_SPO2_to_use

root.slider_SPO2 = customtkinter.CTkSlider(master=root.frame_right,
                                           from_=2048,
                                           to=16384,
                                           number_of_steps=3,
                                           progress_color='#666666',
                                           button_color='#3373b8',
                                           fg_color='#000000',
                                           command = values_SPO2
                                           )
root.slider_SPO2.set(11605.333333333332) #4096 (Initalize value)
root.slider_SPO2.grid(row=5, column=0, columnspan=1, pady=15, padx=55, sticky="we")


# ------PLOT OBJECT------#
# Background plot
root.label_bg_plot = customtkinter.CTkLabel(master=root.frame_right,
                                                   text ="",
                                                   height=360,
                                                   width = 750,
                                                   fg_color='#dcd8d8', 
                                                   justify=tkinter.LEFT)
root.label_bg_plot.place(x=66, y=230, anchor = W)

# Add figure canvas
fig = Figure()
ax = fig.add_subplot(111)
fig.patch.set_facecolor('#dcd8d8')
fig.subplots_adjust(bottom=0.19, right=0.94)

# ax = plt.axes(xlim=(0,100),ylim=(0, 120)); #displaying only 100 samples
ax.title.set_visible(False)
ax.set_xlabel('Sample')
ax.set_ylabel('Voltage')
ax.set_xlim(0, 150)
ax.set_ylim(-0.5, 4000)
ax.set_facecolor('#dcd8d8')

lines = ax.plot([], [])[0]

canvas = FigureCanvasTkAgg(fig, master=root)  # A tk.DrawingArea.
canvas.get_tk_widget().place(x=255, y=75, width=730, height=350)
canvas.draw()

root.after(1, plot_data)

# VALUES 

root.label_values = customtkinter.CTkLabel(master=root.frame_right,
                                                   text ="SpO₂ and heart rate values will appear here",
                                                   height=120,
                                                   width = 400,
                                                   fg_color='#666666',  #''
                                                   justify=tkinter.LEFT
                                                   # command = show_values)
                                           )
root.label_values.grid(row=2, column=1, rowspan=3, pady=5, padx=0)

# WIDGET TITLES


root.label_settings = customtkinter.CTkLabel(master=root.frame_right,
                                                text="Settings",
                                                text_color='white',
                                                text_font=("Roboto Medium", -14),
                                                fg_color = '#3373b8',
                                                corner_radius = 6
                                                )  
root.label_settings.grid(row=1, column=0, columnspan=1, pady=15, padx=10)

# Data values
root.label_data_values = customtkinter.CTkLabel(master=root.frame_right,
                                                text="Values",
                                                text_color='white',
                                                text_font=("Roboto Medium", -14),
                                                fg_color = '#3373b8',
                                                corner_radius = 6
                                                )  
root.label_data_values.grid(row=1, column=1, columnspan=1, pady=15, padx=10)

# Plot
root.label_data_values = customtkinter.CTkLabel(master=root.frame_right,
                                                text="Plot",
                                                text_color='white',
                                                text_font=("Roboto Medium", -14),
                                                fg_color = '#3373b8',
                                                corner_radius = 6
                                                )  
root.label_data_values.place(x=375, y=25, anchor = W)

# DA USARE PER VEDERE POSIZIONE OGGETTI PASSANDO SOPRA IL MOUSE
"""
def motion(event):
    x, y = event.x, event.y
    print('{}, {}'.format(x, y))

root.bind('<Motion>', motion)
root.mainloop()
"""

root.mainloop()