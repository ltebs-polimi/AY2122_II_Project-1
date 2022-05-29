from struct import pack
from tkinter import *
from PIL import ImageTk,Image

root = Tk()
root.title("Slider")
root.geometry("400x400")

valuelist = [0,30,89,110,245,301,360,400]

# Funzione per selezionare solo alcuni valori
def valuecheck(value):
	newvalue=min(valuelist, key=lambda x:abs(x-float(value)))
	horizontal.set(newvalue)

horizontal = Scale(root,from_=min(valuelist),to=max(valuelist), command=valuecheck, orient=HORIZONTAL) # resolution used to set the delta
# tick interval = 50 mette i valori numerici ogni 50, ma secondo me non serve
horizontal.pack()

my_label = Label(root,text=horizontal.get()).pack() # show value in a label
# NB the function scale.get serve per prendere valori da slider

def slide():
	my_label = my_label = Label(root,text=horizontal.get()).pack()

my_btn = Button(root, text= "Click me", command=slide).pack()
root.mainloop()