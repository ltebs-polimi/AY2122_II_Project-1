# Images
# https://www.youtube.com/watch?v=D0iCHFXHb_g&list=PLzMcBGfZo4-lB8MZfHPLTEHO9zJDDLpYj&index=5

from PyQt5 import QtCore, QtGui, QtWidgets

class Ui_MainWindow(object):
    def setupUi(self, MainWindow):
        MainWindow.setObjectName("MainWindow")
        MainWindow.resize(800, 600)
        self.centralwidget = QtWidgets.QWidget(MainWindow)
        self.centralwidget.setObjectName("centralwidget")
        self.label = QtWidgets.QLabel(self.centralwidget)
        self.label.setGeometry(QtCore.QRect(190, 100, 421, 301))
        self.label.setText("")

        # qui è dove è inserita la foto
        self.label.setPixmap(QtGui.QPixmap("D:/Immagini/Sacro Monte di Varallo/_MG_5640.JPG"))
        self.label.setScaledContents(True)
        self.label.setObjectName("label")
        self.Button_Flowers = QtWidgets.QPushButton(self.centralwidget)
        self.Button_Flowers.setGeometry(QtCore.QRect(200, 460, 93, 28))
        self.Button_Flowers.setObjectName("Button_Flowers")
        self.Button_Other = QtWidgets.QPushButton(self.centralwidget)
        self.Button_Other.setGeometry(QtCore.QRect(440, 460, 93, 28))
        self.Button_Other.setObjectName("Button_Other")
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

        self.Button_Flowers.clicked.connect(self.show_flowers_photo)
        self.Button_Other.clicked.connect(self.show_other_photo)

    def retranslateUi(self, MainWindow):
        _translate = QtCore.QCoreApplication.translate
        MainWindow.setWindowTitle(_translate("MainWindow", "MainWindow"))
        self.Button_Flowers.setText(_translate("MainWindow", "Flowers"))
        self.Button_Other.setText(_translate("MainWindow", "Other"))

    # to show one photo
    def show_other_photo(self):
            self.label.setPixmap(QtGui.QPixmap("D:\Immagini/duomo.jpg"))
    # to show the other photo
    def show_flowers_photo(self):
            self.label.setPixmap(QtGui.QPixmap("D:/Immagini/Sacro Monte di Varallo/_MG_5640.JPG"))

if __name__ == "__main__":
    import sys
    app = QtWidgets.QApplication(sys.argv)
    MainWindow = QtWidgets.QMainWindow()
    ui = Ui_MainWindow()
    ui.setupUi(MainWindow)
    MainWindow.show()
    sys.exit(app.exec_())
