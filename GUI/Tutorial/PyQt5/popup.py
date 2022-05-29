# QMessageBox and Popup Windows
# https://www.youtube.com/watch?v=GkgMTyiLtWk&list=PLzMcBGfZo4-lB8MZfHPLTEHO9zJDDLpYj&index=6

from PyQt5 import QtCore, QtGui, QtWidgets
from PyQt5.QtWidgets import QMessageBox

class Ui_MainWindow(object):
    def setupUi(self, MainWindow):
        MainWindow.setObjectName("MainWindow")
        MainWindow.resize(664, 600)
        self.centralwidget = QtWidgets.QWidget(MainWindow)
        self.centralwidget.setObjectName("centralwidget")
        self.Button_Popup = QtWidgets.QPushButton(self.centralwidget)
        self.Button_Popup.setGeometry(QtCore.QRect(170, 127, 321, 151))
        self.Button_Popup.setObjectName("Button_Popup")
        MainWindow.setCentralWidget(self.centralwidget)
        self.menubar = QtWidgets.QMenuBar(MainWindow)
        self.menubar.setGeometry(QtCore.QRect(0, 0, 664, 26))
        self.menubar.setObjectName("menubar")
        MainWindow.setMenuBar(self.menubar)
        self.statusbar = QtWidgets.QStatusBar(MainWindow)
        self.statusbar.setObjectName("statusbar")
        MainWindow.setStatusBar(self.statusbar)

        self.retranslateUi(MainWindow)
        QtCore.QMetaObject.connectSlotsByName(MainWindow)

        # run function "show_popup" when the button is pressed
        self.Button_Popup.clicked.connect(self.show_popup)

    def retranslateUi(self, MainWindow):
        _translate = QtCore.QCoreApplication.translate
        MainWindow.setWindowTitle(_translate("MainWindow", "MainWindow"))
        self.Button_Popup.setText(_translate("MainWindow", "Show Popup"))

    def show_popup(self):
        msg = QMessageBox()
        msg.setWindowTitle("Popup") # name of the popup window
        msg.setText("This is the main text")
        msg.setIcon(QMessageBox.Critical) # set icon 
        # Al posto di Critical può esserci Warning, Information, Question
        
        # in order to put another button instead of "ok" inside the msg box
        msg.setStandardButtons(QMessageBox.Cancel|QMessageBox.Retry)
        # oppure Ok, Open, Save, Cancel, Close, Yes, No, Abort, Retry, Ignore
        
        # in order to define the default button (which has a blue contour)
        msg.setDefaultButton(QMessageBox.Retry)

        # add text
        msg.setInformativeText("Informative Text")

        #to connect the button clicked to the function popup_button
        msg.buttonClicked.connect(self.popup_button)

        x = msg.exec() # this shows the message box

    # function that happen when pushing the button
    def popup_button(self,i): # i will be the button that we click
        print(i.text())

if __name__ == "__main__":
    import sys
    app = QtWidgets.QApplication(sys.argv)
    MainWindow = QtWidgets.QMainWindow()
    ui = Ui_MainWindow()
    ui.setupUi(MainWindow)
    MainWindow.show()
    sys.exit(app.exec_())
