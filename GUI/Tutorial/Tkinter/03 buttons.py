from tkinter import * # * is used to import everything

root =  Tk()

# Funcion realted to the button
def myClick():
    myLabel = Label(root,text='Button was clicked!')
    myLabel.pack()

myButton = Button(root,text ='Click Me', padx = 20, pady = 20, command = myClick, fg='#ffffff', bg = 'black') 
# padx --> size along x
# pady --> size along y
# state = DISABLED --> at the beginning the button is disabled
# command = myClick --> to call the function by clicking the button
# fg = foreground color --> for text color
# bg = background color 
myButton.pack()

root.mainloop()