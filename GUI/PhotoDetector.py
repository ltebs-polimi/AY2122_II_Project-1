# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'PhotoDetector.ui'
#
# Created by: PyQt5 UI code generator 5.15.4
#
# WARNING: Any manual changes made to this file will be lost when pyuic5 is
# run again.  Do not edit this file unless you know what you are doing.


from PyQt5 import QtCore, QtGui, QtWidgets


class Ui_MainWindow(object):
    def setupUi(self, MainWindow):
        MainWindow.setObjectName("MainWindow")
        MainWindow.resize(800, 593)
        MainWindow.setStyleSheet("background-color: rgb(14, 70, 159);")
        self.centralwidget = QtWidgets.QWidget(MainWindow)
        self.centralwidget.setObjectName("centralwidget")
        self.pushButton_Start = QtWidgets.QPushButton(self.centralwidget)
        self.pushButton_Start.setGeometry(QtCore.QRect(70, 400, 61, 41))
        self.pushButton_Start.setAutoFillBackground(False)
        self.pushButton_Start.setStyleSheet("background-color: rgb(212, 228, 255);\n"
"color: rgb(14, 70, 159);\n"
"font: 9pt \"Adobe Heiti Std\";\n"
"border-color: rgb(10, 52, 116);\n"
"border: 4px solid #082859;\n"
"border-radius: 5px;\n"
"border-style: outset;\n"
"")
        self.pushButton_Start.setObjectName("pushButton_Start")
        self.pushButton_Stop = QtWidgets.QPushButton(self.centralwidget)
        self.pushButton_Stop.setGeometry(QtCore.QRect(140, 400, 61, 41))
        self.pushButton_Stop.setStyleSheet("background-color: rgb(212, 228, 255);\n"
"color: rgb(14, 70, 159);\n"
"font: 9pt \"Adobe Heiti Std\";\n"
"border-color: rgb(10, 52, 116);\n"
"border: 4px solid #082859;\n"
"border-radius: 5px;\n"
"border-style: outset;\n"
"")
        self.pushButton_Stop.setObjectName("pushButton_Stop")
        self.line = QtWidgets.QFrame(self.centralwidget)
        self.line.setGeometry(QtCore.QRect(260, -20, 5, 591))
        self.line.setStyleSheet("background-color: rgb(212, 228, 255);")
        self.line.setFrameShape(QtWidgets.QFrame.VLine)
        self.line.setFrameShadow(QtWidgets.QFrame.Sunken)
        self.line.setObjectName("line")
        self.label = QtWidgets.QLabel(self.centralwidget)
        self.label.setGeometry(QtCore.QRect(80, 40, 91, 91))
        self.label.setText("")
        self.label.setPixmap(QtGui.QPixmap("D:\Polimi\LAB\PROJECT\AY2122_II_Project-1\GUI\hr_image.png")) # DA MODIFICARE in base a dove esegui codice
        self.label.setScaledContents(True)
        self.label.setObjectName("label")
        self.label_2 = QtWidgets.QLabel(self.centralwidget)
        self.label_2.setGeometry(QtCore.QRect(70, 140, 131, 16))
        self.label_2.setStyleSheet("color: rgb(212, 228, 255);\n"
"font: 10pt \"Adobe Heiti Std\";")
        self.label_2.setObjectName("label_2")
        MainWindow.setCentralWidget(self.centralwidget)
        self.menubar = QtWidgets.QMenuBar(MainWindow)
        self.menubar.setGeometry(QtCore.QRect(0, 0, 800, 26))
        self.menubar.setObjectName("menubar")
        MainWindow.setMenuBar(self.menubar)
        self.statusbar = QtWidgets.QStatusBar(MainWindow)
        self.statusbar.setObjectName("statusbar")
        MainWindow.setStatusBar(self.statusbar)

        self.retranslateUi(MainWindow)
        QtCore.QMetaObject.connectSlotsByName(MainWindow)

    def retranslateUi(self, MainWindow):
        _translate = QtCore.QCoreApplication.translate
        MainWindow.setWindowTitle(_translate("MainWindow", "MainWindow"))
        self.pushButton_Start.setText(_translate("MainWindow", "Start"))
        self.pushButton_Stop.setText(_translate("MainWindow", "Stop"))
        self.label_2.setText(_translate("MainWindow", "Connected to..."))


if __name__ == "__main__":
    import sys
    app = QtWidgets.QApplication(sys.argv)
    MainWindow = QtWidgets.QMainWindow()
    ui = Ui_MainWindow()
    ui.setupUi(MainWindow)
    MainWindow.show()
    sys.exit(app.exec_())
