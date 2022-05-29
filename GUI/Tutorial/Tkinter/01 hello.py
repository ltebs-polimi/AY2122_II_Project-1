from tkinter import * # * is used to import everything

# In tkinter everything is a widget; the first thing you create is the root widget
# root has to appear at the beginning of the code when you wok with Tkinter
root =  Tk()

# to create something in Tkinter you have to define something + to show it into the screen

# Create label widget
myLabel = Label(root,text='Hello world') # root is where we want it to be

# now we have to put the label widget into the root window
myLabel.pack() 

# create an event loop = to always show the widget until the end of the program
root.mainloop()