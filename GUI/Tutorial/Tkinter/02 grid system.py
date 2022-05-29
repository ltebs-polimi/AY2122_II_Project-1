from tkinter import * # * is used to import everything

# In "hello.py" we've used the function "pack"
# The alternative is the grid system --> it uses rows and columns

root =  Tk()

# Creating the label widget
myLabel1 = Label(root, text="Hello World")
myLabel2 = Label(root, text="How are you?")

# Showing the label widget onto the screen
myLabel1.grid(row=0, column=0)
myLabel2.grid(row=1, column=5)
# --> they will be in different columns and rows
# they are one near to the other because columns 1,2,3 and 4 are ignored

# If we add a column between the 1 and 2, there will be some space between
myLabel3 = Label(root, text="         ")
myLabel3.grid(row=1, column=1)

root.mainloop()