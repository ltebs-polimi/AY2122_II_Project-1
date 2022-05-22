# https://www.youtube.com/watch?v=Vde5SH8e1OQ

import sys
from PyQt5 import QtWidgets
from PyQt5.QtWidgets import QApplication, QMainWindow

def window():
    app = QApplication(sys.argv)
    win = QMainWindow()

    xpos = 500 # where in the screen the window will show up
    ypos = 200 
    width = 300
    height = 300
    win.setGeometry(xpos,ypos,width,height) # to set geometry
    win.setWindowTitle("Photodetector") # to give a title to the window

    win.show() # to show the window
    sys.exit(app.exec_()) 

def center(self):
    frame_geo = self.frameGeometry()
    screen = self.window().windowHandle().screen()
    center_loc = screen.geometry().center()
    frame_geo.moveCenter(center_loc)
    self.move(frame_geo.topLeft())

window() 
