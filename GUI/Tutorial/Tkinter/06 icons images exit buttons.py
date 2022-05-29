from tkinter import *

root = Tk()
root.title("Icons, images and exit buttons")

#ICON
root.iconbitmap("D:\Polimi\LAB\PROJECT\AY2122_II_Project-1\GUI/hr_icon.ico")

# QUIT BUTTON
button_quit = Button(root, text="Exit Program", command=root.quit)
button_quit.pack()

# IMAGES
from PIL import ImageTk,Image # need to import pillow
my_img = ImageTk.PhotoImage(Image.open("D:\Polimi\LAB\PROJECT\AY2122_II_Project-1\GUI/hr_image.png"))
my_label = Label(image=my_img) # define label containing the image
my_label.pack()

root.mainloop()