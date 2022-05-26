from turtle import onclick
from PyQt5 import QtCore, QtGui, QtWidgets
import serial
import serial.tools.list_ports
import logging
import time
from PyQt5.QtCore import (
    QObject,
    QThreadPool,
    QRunnable,
    pyqtSignal,
    pyqtSlot
)

from PyQt5.QtWidgets import (
    QApplication,
    QMainWindow,
    QPushButton,
    QComboBox,
    QHBoxLayout,
    QVBoxLayout,
    QWidget,
)

# Globals
CONN_STATUS = False
START = "b"
STOP = "s"

# Logging config
logging.basicConfig(format="%(message)s", level=logging.INFO)

#############################
# Find PsoC among the ports #
#############################
def findPsoC(portsFound):
    commPort = 'None'
    n_connections = len(portsFound)

    for i in range(0,n_connections):
        port = portsFound[i]
        strPort = str(port)

        if 'KitProg' in strPort:
            splitPort = strPort.split(' ')
            commPort = (splitPort[0])    

    if commPort != 'None':
        ser = serial.Serial(commPort, baudrate = 115200, timeout = 1)
        print('Connected to ' + commPort)

    else: 
        print('Error in the connection with PSoC')

    return commPort

ports = serial.tools.list_ports.comports()
findPsoC(ports)



#########################
# SERIAL_WORKER_SIGNALS #
#########################
class SerialWorkerSignals(QObject):
    """!
    @brief Class that defines the signals available to a serialworker.

    Available signals (with respective inputs) are:
        - device_port:
            str --> port name to which a device is connected
        - status:
            str --> port name
            int --> macro representing the state (0 - error during opening, 1 - success)
    """
    device_port = pyqtSignal(str)
    status = pyqtSignal(str, int)

#################
# SERIAL_WORKER #
#################
class SerialWorker(QRunnable):
    """!
    @brief Main class for serial communication: handles connection with device.
    """

    def __init__(self, connectPort):
        """!
        @brief Init worker.
        """
        self.is_killed = False
        super().__init__()
        # init port, params and signals
        self.port = serial.Serial()
        self.port_name = connectPort
        self.baudrate = 115200  # hard coded but can be a global variable, or an input param
        self.signals = SerialWorkerSignals()

    @pyqtSlot()
    def run(self):
        """!
        @brief Estabilish connection with desired serial port.
        """
        global CONN_STATUS

        if not CONN_STATUS:
            try:
                self.port = serial.Serial(port=self.port_name, baudrate=self.baudrate,
                                          write_timeout=0, timeout=2)

                if self.port.is_open:
                    CONN_STATUS = True
                    self.signals.status.emit(self.port_name, 1)
                    time.sleep(0.01)

            except serial.SerialException:
                logging.info("Error with port {}.".format(self.port_name))
                self.signals.status.emit(self.port_name, 0)
                time.sleep(0.01)

    @pyqtSlot()
    def send(self, char):
        """!
        @brief Basic function to send a single char on serial port.
        """
        try:
            self.port.write(char.encode('utf-8'))
            logging.info("Written {} on port {}.".format(char, self.port_name))
        except:
            logging.info("Could not write {} on port {}.".format(char, self.port_name))

    @pyqtSlot()
    def killed(self):
        """!
        @brief Close the serial port before closing the app.
        """
        global CONN_STATUS
        if self.is_killed and CONN_STATUS:
            self.port.close()
            time.sleep(0.01)
            CONN_STATUS = False
            self.signals.device_port.emit(self.port_name)

        logging.info("Killing the process")



###############
# MAIN WINDOW #
###############
class Ui_MainWindow(QMainWindow):
    def __init__(self):
        """!
        @brief Init MainWindow.
        """
        # define worker
        self.serial_worker = SerialWorker(None)

        super(Ui_MainWindow, self).__init__()

        # create thread handler
        self.threadpool = QThreadPool()
        self.connected = CONN_STATUS
        self.setupUi(MainWindow)

    def setupUi(self, MainWindow):
        MainWindow.setObjectName("MainWindow")
        MainWindow.resize(1077, 571)
        MainWindow.setStyleSheet("background-color: rgb(14, 70, 159);")
        self.centralwidget = QtWidgets.QWidget(MainWindow)
        self.centralwidget.setObjectName("centralwidget")
        self.pushButton_Start = QtWidgets.QPushButton(self.centralwidget)
        self.pushButton_Start.setGeometry(QtCore.QRect(630, 480, 61, 41))
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
        self.pushButton_Stop.setGeometry(QtCore.QRect(780, 480, 61, 41))
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
        self.line.setGeometry(QtCore.QRect(360, -40, 5, 591))
        self.line.setStyleSheet("background-color: rgb(212, 228, 255);")
        self.line.setFrameShape(QtWidgets.QFrame.VLine)
        self.line.setFrameShadow(QtWidgets.QFrame.Sunken)
        self.line.setObjectName("line")
        self.label = QtWidgets.QLabel(self.centralwidget)
        self.label.setGeometry(QtCore.QRect(40, 20, 91, 91))
        self.label.setText("")
        #self.label.setPixmap(QtGui.QPixmap("C:\Users\Perro\Desktop\AY2122_II_Project-1\GUI\hr_image.png"))
        self.label.setPixmap(QtGui.QPixmap("D:\Polimi\LAB\PROJECT\AY2122_II_Project-1\GUI\hr_image.png"))
        self.label.setScaledContents(True)
        self.label.setObjectName("label")
        self.label_2 = QtWidgets.QLabel(self.centralwidget)
        self.label_2.setGeometry(QtCore.QRect(160, 30, 171, 71))
        self.label_2.setStyleSheet("color: rgb(212, 228, 255);\n"
                                    "font: 10pt \"Adobe Heiti Std\";")
        self.label_2.setAlignment(QtCore.Qt.AlignCenter)
        self.label_2.setObjectName("label_2")
        self.horizontalSlider = QtWidgets.QSlider(self.centralwidget)
        self.horizontalSlider.setGeometry(QtCore.QRect(50, 230, 231, 16))
        self.horizontalSlider.setMinimum(1)
        self.horizontalSlider.setMaximum(4)
        self.horizontalSlider.setOrientation(QtCore.Qt.Horizontal)
        self.horizontalSlider.setObjectName("horizontalSlider")
        self.label_3 = QtWidgets.QLabel(self.centralwidget)
        self.label_3.setGeometry(QtCore.QRect(50, 250, 281, 16))
        self.label_3.setObjectName("label_3")
        self.label_4 = QtWidgets.QLabel(self.centralwidget)
        self.label_4.setGeometry(QtCore.QRect(40, 170, 251, 21))
        font = QtGui.QFont()
        font.setPointSize(8)
        font.setBold(True)
        font.setWeight(75)
        self.label_4.setFont(font)
        self.label_4.setObjectName("label_4")
        self.label_5 = QtWidgets.QLabel(self.centralwidget)
        self.label_5.setGeometry(QtCore.QRect(70, 210, 191, 16))
        self.label_5.setAlignment(QtCore.Qt.AlignCenter)
        self.label_5.setObjectName("label_5")
        self.horizontalSlider_2 = QtWidgets.QSlider(self.centralwidget)
        self.horizontalSlider_2.setGeometry(QtCore.QRect(50, 310, 231, 16))
        self.horizontalSlider_2.setMinimum(1)
        self.horizontalSlider_2.setMaximum(4)
        self.horizontalSlider_2.setOrientation(QtCore.Qt.Horizontal)
        self.horizontalSlider_2.setObjectName("horizontalSlider_2")
        self.label_6 = QtWidgets.QLabel(self.centralwidget)
        self.label_6.setGeometry(QtCore.QRect(70, 280, 191, 21))
        self.label_6.setAlignment(QtCore.Qt.AlignCenter)
        self.label_6.setObjectName("label_6")
        self.label_7 = QtWidgets.QLabel(self.centralwidget)
        self.label_7.setGeometry(QtCore.QRect(50, 330, 241, 16))
        self.label_7.setObjectName("label_7")
        self.horizontalSlider_3 = QtWidgets.QSlider(self.centralwidget)
        self.horizontalSlider_3.setGeometry(QtCore.QRect(50, 390, 231, 21))
        self.horizontalSlider_3.setMinimum(1)
        self.horizontalSlider_3.setMaximum(32)
        self.horizontalSlider_3.setOrientation(QtCore.Qt.Horizontal)
        self.horizontalSlider_3.setObjectName("horizontalSlider_3")
        self.label_8 = QtWidgets.QLabel(self.centralwidget)
        self.label_8.setGeometry(QtCore.QRect(70, 370, 191, 16))
        self.label_8.setAlignment(QtCore.Qt.AlignCenter)
        self.label_8.setObjectName("label_8")
        self.label_9 = QtWidgets.QLabel(self.centralwidget)
        self.label_9.setGeometry(QtCore.QRect(50, 420, 281, 16))
        self.label_9.setObjectName("label_9")
        self.label_10 = QtWidgets.QLabel(self.centralwidget)
        self.label_10.setGeometry(QtCore.QRect(70, 441, 191, 20))
        self.label_10.setAlignment(QtCore.Qt.AlignCenter)
        self.label_10.setObjectName("label_10")
        self.label_11 = QtWidgets.QLabel(self.centralwidget)
        self.label_11.setGeometry(QtCore.QRect(50, 490, 241, 16))
        self.label_11.setObjectName("label_11")
        self.horizontalSlider_4 = QtWidgets.QSlider(self.centralwidget)
        self.horizontalSlider_4.setGeometry(QtCore.QRect(50, 470, 231, 16))
        self.horizontalSlider_4.setMinimum(1)
        self.horizontalSlider_4.setMaximum(4)
        self.horizontalSlider_4.setOrientation(QtCore.Qt.Horizontal)
        self.horizontalSlider_4.setObjectName("horizontalSlider_4")
        MainWindow.setCentralWidget(self.centralwidget)
        self.menubar = QtWidgets.QMenuBar(MainWindow)
        self.menubar.setGeometry(QtCore.QRect(0, 0, 1077, 26))
        self.menubar.setObjectName("menubar")
        MainWindow.setMenuBar(self.menubar)
        self.statusbar = QtWidgets.QStatusBar(MainWindow)
        self.statusbar.setObjectName("statusbar")
        MainWindow.setStatusBar(self.statusbar)

        self.retranslateUi(MainWindow)
        QtCore.QMetaObject.connectSlotsByName(MainWindow)

        self.connected = CONN_STATUS
        #self.serialscan()

        #self.pushButton_Start.clicked.connect(self.clicked)
        #self.pushButton_Start.clicked.connect(lambda state, x=START: self.start_stop_control(state, x))
        #self.pushButton_Stop.clicked.connect(self.clicked)
        #self.pushButton_Stop.clicked.connect(lambda state, x=STOP: self.start_stop_control(state, x))
    

    


    ####################
    # SERIAL INTERFACE #
    ####################
    def serialscan(self):
        self.pushButton_Start.clicked.connect(self.clicked)
        self.pushButton_Start.clicked.connect(lambda state, x=START: self.start_stop_control(state, x))
        self.pushButton_Stop.clicked.connect(self.clicked)
        self.pushButton_Stop.clicked.connect(lambda state, x=STOP: self.start_stop_control(state, x))

    ##################
    # SERIAL SIGNALS #
    ##################
    
    @pyqtSlot(bool)
    def on_toggle(self, checked):
        """!
        @brief Allow connection and disconnection from selected serial port.
        """
        if checked:
            # setup reading worker
            self.serial_worker = SerialWorker(self.port_text)  # needs to be re defined
            # connect worker signals to functions
            self.serial_worker.signals.status.connect(self.check_serialport_status)
            self.serial_worker.signals.device_port.connect(self.connected_device)
            # execute the worker
            self.threadpool.start(self.serial_worker)
        else:
            # kill thread
            self.serial_worker.is_killed = True
            self.serial_worker.killed()
            #self.com_list_widget.setDisabled(False)  # enable the possibility to change port
            #self.conn_btn.setText(
            #    "Connect to port {}".format(self.port_text)
            #)

    def check_serialport_status(self, port_name, status):
        """!
        @brief Handle the status of the serial port connection.

        Available status:
            - 0  --> Error during opening of serial port
            - 1  --> Serial port opened correctly
        """
        if status == 0:
            #self.conn_btn.setChecked(False)
            logging.info('Error during the opening of {}'.format(self.port_name))
            # FARE IN MODO CHE BOTTONI start e stop SIANO DISATTIVATI
            # usando tipo una cosa del genere "button.setCheckable(False)""
        elif status == 1:
            # enable all the widgets on the interface
            #self.com_list_widget.setDisabled(
            #    True)  # disable the possibility to change COM port when already connected
            self.label_2.setText('Connected to {}'.format(self.port_name))
            self.label_2.adjustSize()
            logging.info('Connected to {}'.format(self.port_name))

    def connected_device(self, port_name):
        """!
        @brief Checks on the termination of the serial worker.
        """
        logging.info("Port {} closed.".format(port_name))

    def ExitHandler(self):
        """!
        @brief Kill every possible running thread upon exiting application.
        """
        self.serial_worker.is_killed = True
        self.serial_worker.killed()

    @pyqtSlot()
    def start_stop_control(self, state, char):
        """!
        @brief Handle the ON/OFF status of the PSoC LED.

        @param state is the state of the button
        @param char is the char to be sent on serial port
        """
        self.serial_worker.send(char)

    def clicked(self, x):
        connectPort = findPsoC(ports)

        if connectPort != 'None':
            if x!=STOP:
                #ser = serial.Serial(connectPort, baudrate=115200, timeout=1)
                self.port_name = connectPort
                # setup reading worker
                self.serial_worker = SerialWorker(self.port_name)  # needs to be re defined
                # connect worker signals to functions
                #self.serial_worker.signals.status.connect(self.check_serialport_status)
                self.serial_worker.signals.device_port.connect(self.connected_device)
                # execute the worker
                self.threadpool.start(self.serial_worker)

            elif x==STOP:
                self.serial_worker.is_killed = True
                self.serial_worker.killed()

        else:
            self.label_2.setText('Error in the connection with PSoC')
            self.label_2.adjustSize()
            logging.info('Error in the connection with PSoC')


    def retranslateUi(self, MainWindow):
        _translate = QtCore.QCoreApplication.translate
        #MainWindow.setWindowTitle(_translate("MainWindow", "MainWindow"))
        MainWindow.setWindowTitle("Photodetector")
        self.pushButton_Start.setText(_translate("MainWindow", "Start"))
        self.pushButton_Stop.setText(_translate("MainWindow", "Stop"))
        self.label_2.setText(_translate("MainWindow", "Trying to connect"))
        self.label_3.setText(_translate("MainWindow", "69          118         215         411  μs"))
        self.label_4.setText(_translate("MainWindow", "Configura i diversi parametri"))
        self.label_5.setText(_translate("MainWindow", "LED Pulse Width"))
        self.label_6.setText(_translate("MainWindow", "Samples per second"))
        self.label_7.setText(_translate("MainWindow", "50          100         200         400"))
        self.label_8.setText(_translate("MainWindow", "Led Current Control"))
        self.label_9.setText(_translate("MainWindow", "0.2                                      6.2  mA"))
        self.label_10.setText(_translate("MainWindow", "SpO2 ADC Range"))
        self.label_11.setText(_translate("MainWindow", "2048      4096      8192     16384"))

        # change style of writings
        self.label_4.setAlignment(QtCore.Qt.AlignCenter)
        self.label_3.setStyleSheet("color: rgb(212, 228, 255);\n"                                   
                                   "font: 9pt \"Adobe Heiti Std\";\n")      
        self.label_4.setStyleSheet("color: rgb(212, 228, 255);\n"                                   
                                   "font: 9pt \"Adobe Heiti Std\";\n")      
        self.label_5.setStyleSheet("color: rgb(212, 228, 255);\n"                                   
                                   "font: 9pt \"Adobe Heiti Std\";\n") 
        self.label_6.setStyleSheet("color: rgb(212, 228, 255);\n"                                   
                                   "font: 9pt \"Adobe Heiti Std\";\n")
        self.label_7.setStyleSheet("color: rgb(212, 228, 255);\n"                                   
                                   "font: 9pt \"Adobe Heiti Std\";\n")      
        self.label_8.setStyleSheet("color: rgb(212, 228, 255);\n"                                   
                                   "font: 9pt \"Adobe Heiti Std\";\n")                                   
        self.label_9.setStyleSheet("color: rgb(212, 228, 255);\n"                                   
                                   "font: 9pt \"Adobe Heiti Std\";\n")      
        self.label_10.setStyleSheet("color: rgb(212, 228, 255);\n"                                   
                                   "font: 9pt \"Adobe Heiti Std\";\n")
        self.label_11.setStyleSheet("color: rgb(212, 228, 255);\n"                                   
                                   "font: 9pt \"Adobe Heiti Std\";\n") 

if __name__ == "__main__":
    import sys
    app = QtWidgets.QApplication(sys.argv)
    MainWindow = QtWidgets.QMainWindow()
    ui = Ui_MainWindow()
    ui.setupUi(MainWindow)
    MainWindow.show()
    sys.exit(app.exec_())
