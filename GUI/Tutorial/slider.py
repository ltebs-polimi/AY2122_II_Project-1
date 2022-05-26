# https://www.youtube.com/watch?v=6vrnGHfBnEQ

from PyQt5 import QtCore, QtGui, QtWidgets
from PyQt5.QtWidgets import QApplication, QMainWindow, QWidget, QLineEdit, QSlider, QVBoxLayout
#import sys

class Ui_MainWindow(object):
    def setupUi(self, MainWindow):
        MainWindow.setObjectName("MainWindow")
        MainWindow.resize(800, 600)
        MainWindow.setStyleSheet("background-color: rgb(85, 0, 127);")
        self.centralwidget = QtWidgets.QWidget(MainWindow)
        self.centralwidget.setObjectName("centralwidget")
        self.horizontalSlider = QtWidgets.QSlider(self.centralwidget)
        self.horizontalSlider.setEnabled(True)
        self.horizontalSlider.setGeometry(QtCore.QRect(250, 180, 231, 16))
        self.horizontalSlider.setCursor(QtGui.QCursor(QtCore.Qt.PointingHandCursor))
        self.horizontalSlider.setMouseTracking(False)
        self.horizontalSlider.setTabletTracking(False)
        self.horizontalSlider.setStyleSheet("")

        self.horizontalSlider.setMinimum(1) # minimum value
        self.horizontalSlider.setMaximum(4) # maximum value
        self.horizontalSlider.setTickInterval(10) # interval

        #self.slider.setValue(20) #per settare il valore 

        self.horizontalSlider.setOrientation(QtCore.Qt.Horizontal)
        self.horizontalSlider.setInvertedAppearance(False)
        self.horizontalSlider.setTickPosition(QtWidgets.QSlider.TicksBelow)

        self.horizontalSlider.setObjectName("horizontalSlider")
        self.label_LED_pulse_width = QtWidgets.QLabel(self.centralwidget)
        self.label_LED_pulse_width.setGeometry(QtCore.QRect(272, 160, 191, 16))
        self.label_LED_pulse_width.setStyleSheet("color: rgb(212, 228, 255);\n"
"font: 9pt \"Adobe Heiti Std\";")
        self.label_LED_pulse_width.setAlignment(QtCore.Qt.AlignCenter)
        self.label_LED_pulse_width.setObjectName("label_LED_pulse_width")
        self.label_3 = QtWidgets.QLabel(self.centralwidget)
        self.label_3.setGeometry(QtCore.QRect(252, 200, 281, 16))
        self.label_3.setMouseTracking(False)
        self.label_3.setStyleSheet("color: rgb(212, 228, 255);\n"
"font: 8pt \"Adobe Heiti Std\";")
        self.label_3.setObjectName("label_3")
        MainWindow.setCentralWidget(self.centralwidget)
        self.menubar = QtWidgets.QMenuBar(MainWindow)
        self.menubar.setGeometry(QtCore.QRect(0, 0, 800, 26))
        self.menubar.setObjectName("menubar")
        MainWindow.setMenuBar(self.menubar)
        self.statusbar = QtWidgets.QStatusBar(MainWindow)
        self.statusbar.setObjectName("statusbar")
        MainWindow.setStatusBar(self.statusbar)

        self.label_PROVA = QtWidgets.QLabel(self.centralwidget)
        self.label_PROVA.setGeometry(QtCore.QRect(300, 300, 281, 20))
        self.label_PROVA.setObjectName("label_PROVA")
        self.label_PROVA.setStyleSheet("color: rgb(212, 228, 255);\n"
        "font: 8pt \"Adobe Heiti Std\";")

        self.label_PROVA2 = QtWidgets.QLabel(self.centralwidget)
        self.label_PROVA2.setGeometry(QtCore.QRect(400, 400, 281, 20))
        self.label_PROVA2.setObjectName("label_PROVA")
        self.label_PROVA2.setStyleSheet("color: rgb(212, 228, 255);\n"
        "font: 8pt \"Adobe Heiti Std\";")

        # For changes in values of the slider
        self.horizontalSlider.valueChanged.connect(self.changedValue)

        self.retranslateUi(MainWindow)
        QtCore.QMetaObject.connectSlotsByName(MainWindow)

    def retranslateUi(self, MainWindow):
        _translate = QtCore.QCoreApplication.translate
        MainWindow.setWindowTitle(_translate("MainWindow", "MainWindow"))
        self.label_LED_pulse_width.setText(_translate("MainWindow", "LED Pulse Width (μs)"))
        self.label_3.setText(_translate("MainWindow", "69                 118                 215                   411 "))

# How to give a functionality to the slider?
# for example write the value in a label
    def changedValue(self):
        size = str(self.horizontalSlider.value())
        self.label_PROVA.setText(size)
        if size == '1':
            self.label_PROVA2.setText('69')
        if size == '2':
            self.label_PROVA2.setText('118')
        if size == '3':
            self.label_PROVA2.setText('215')
        if size == '4':
            self.label_PROVA2.setText('411')

if __name__ == "__main__":
    import sys
    app = QtWidgets.QApplication(sys.argv)
    MainWindow = QtWidgets.QMainWindow()
    ui = Ui_MainWindow()
    ui.setupUi(MainWindow)
    MainWindow.show()
    sys.exit(app.exec_())
