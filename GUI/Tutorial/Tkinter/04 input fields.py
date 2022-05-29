from tkinter import * 
root =  Tk()

e = Entry(root, width=50, bg='white', borderwidth=2) # box for input field
e.pack()
e.insert(0, "Enter your name ") # defaul text inside the box

# Funcion realted to the button
def myClick():
    hello = "Hello " + e.get()
    myLabel = Label(root,text=hello)
    myLabel.pack()

myButton = Button(root,text ='Enter your name', command = myClick) 
myButton.pack()

root.mainloop()