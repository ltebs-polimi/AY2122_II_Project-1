# Buttons 
# https://www.youtube.com/watch?v=HYV81L7qd6M&list=PLzMcBGfZo4-lB8MZfHPLTEHO9zJDDLpYj&index=4

from PyQt5 import QtCore, QtGui, QtWidgets


class Ui_MainWindow(object):
    def setupUi(self, MainWindow):
        MainWindow.setObjectName("MainWindow")
        MainWindow.resize(800, 600)
        self.centralwidget = QtWidgets.QWidget(MainWindow)
        self.centralwidget.setObjectName("centralwidget")
        self.pushButton = QtWidgets.QPushButton(self.centralwidget)
        self.pushButton.setGeometry(QtCore.QRect(310, 250, 121, 51))
        self.pushButton.setObjectName("pushButton")
        self.label = QtWidgets.QLabel(self.centralwidget)
        self.label.setGeometry(QtCore.QRect(350, 150, 111, 41))
        self.label.setObjectName("label")
        MainWindow.setCentralWidget(self.centralwidget)
        self.menubar = QtWidgets.QMenuBar(MainWindow)
        self.menubar.setGeometry(QtCore.QRect(0, 0, 800, 26))
        self.menubar.setObjectName("menubar")
        self.menuMenu1 = QtWidgets.QMenu(self.menubar)
        self.menuMenu1.setObjectName("menuMenu1")
        MainWindow.setMenuBar(self.menubar)
        self.statusbar = QtWidgets.QStatusBar(MainWindow)
        self.statusbar.setObjectName("statusbar")
        MainWindow.setStatusBar(self.statusbar)
        self.actionNew = QtWidgets.QAction(MainWindow)
        self.actionNew.setObjectName("actionNew")
        self.actionCancel = QtWidgets.QAction(MainWindow)
        self.actionCancel.setObjectName("actionCancel")
        self.menuMenu1.addAction(self.actionNew)
        self.menuMenu1.addAction(self.actionCancel)
        self.menubar.addAction(self.menuMenu1.menuAction())

        self.retranslateUi(MainWindow)
        QtCore.QMetaObject.connectSlotsByName(MainWindow)

        # to connect the function New to the clicked function and display the text you want
        self.actionNew.triggered.connect(lambda: self.clicked("New was clicked")) # triggered means that the button is pressed

    # Creation of a method that is called when button is pressed
    def clicked(self,text):
        self.label.setText(text) # so that the label will contain always the text passed as input argument
        self.label.adjustSize() # to be sure that the size is appropriate to the text

    def retranslateUi(self, MainWindow):
        _translate = QtCore.QCoreApplication.translate
        MainWindow.setWindowTitle(_translate("MainWindow", "MainWindow"))
        self.pushButton.setText(_translate("MainWindow", "Button"))
        self.label.setText(_translate("MainWindow", "Ciao"))
        self.menuMenu1.setTitle(_translate("MainWindow", "Menu1"))
        self.actionNew.setText(_translate("MainWindow", "New"))
        self.actionCancel.setText(_translate("MainWindow", "Cancel"))

        self.actionNew.setShortcut(_translate("MainWindow","Ctrl+N"))



if __name__ == "__main__":
    import sys
    app = QtWidgets.QApplication(sys.argv)
    MainWindow = QtWidgets.QMainWindow()
    ui = Ui_MainWindow()
    ui.setupUi(MainWindow)
    MainWindow.show()
    sys.exit(app.exec_())
