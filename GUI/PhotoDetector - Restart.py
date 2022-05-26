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
class Ui_MainWindow(object):
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
        self.serialscan

    def setupUi(self, MainWindow):
        MainWindow.setObjectName("MainWindow")
        MainWindow.resize(1077, 559)
        MainWindow.setStyleSheet("background-color: rgb(14, 70, 159);")
        self.centralwidget = QtWidgets.QWidget(MainWindow)
        self.centralwidget.setObjectName("centralwidget")
        self.pushButton_Start = QtWidgets.QPushButton(self.centralwidget)
        self.pushButton_Start.setGeometry(QtCore.QRect(630, 490, 61, 41))
        self.pushButton_Start.setCursor(QtGui.QCursor(QtCore.Qt.PointingHandCursor))
        self.pushButton_Start.setAutoFillBackground(False)
        self.pushButton_Start.setStyleSheet("background-color: rgb(212, 228, 255);\n"
"color: rgb(14, 70, 159);\n"
"font: 10pt \"Adobe Heiti Std\";\n"
"border-color: rgb(10, 52, 116);\n"
"border: 4px solid #082859;\n"
"border-radius: 5px;\n"
"border-style: outset;\n"
"")
        self.pushButton_Start.setObjectName("pushButton_Start")
        self.pushButton_Stop = QtWidgets.QPushButton(self.centralwidget)
        self.pushButton_Stop.setGeometry(QtCore.QRect(780, 490, 61, 41))
        self.pushButton_Stop.setCursor(QtGui.QCursor(QtCore.Qt.PointingHandCursor))
        self.pushButton_Stop.setStyleSheet("background-color: rgb(212, 228, 255);\n"
"color: rgb(14, 70, 159);\n"
"font: 10pt \"Adobe Heiti Std\";\n"
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
        self.label_connection = QtWidgets.QLabel(self.centralwidget)
        self.label_connection.setGeometry(QtCore.QRect(10, 10, 351, 20))
        self.label_connection.setStyleSheet("color: rgb(212, 228, 255);\n"
"font: 8pt \"Adobe Heiti Std\";")
        self.label_connection.setAlignment(QtCore.Qt.AlignCenter)
        self.label_connection.setObjectName("label_connection")
        self.horizontalSlider = QtWidgets.QSlider(self.centralwidget)
        self.horizontalSlider.setEnabled(True)
        self.horizontalSlider.setGeometry(QtCore.QRect(60, 240, 231, 16))
        self.horizontalSlider.setCursor(QtGui.QCursor(QtCore.Qt.PointingHandCursor))
        self.horizontalSlider.setMouseTracking(False)
        self.horizontalSlider.setTabletTracking(False)
        self.horizontalSlider.setStyleSheet("")
        self.horizontalSlider.setMinimum(1)
        self.horizontalSlider.setMaximum(4)
        self.horizontalSlider.setOrientation(QtCore.Qt.Horizontal)
        self.horizontalSlider.setInvertedAppearance(False)
        self.horizontalSlider.setTickPosition(QtWidgets.QSlider.TicksBelow)
        self.horizontalSlider.setTickInterval(10)
        self.horizontalSlider.setObjectName("horizontalSlider")
        self.label_3 = QtWidgets.QLabel(self.centralwidget)
        self.label_3.setGeometry(QtCore.QRect(62, 260, 281, 16))
        self.label_3.setMouseTracking(False)
        self.label_3.setStyleSheet("color: rgb(212, 228, 255);\n"
"font: 8pt \"Adobe Heiti Std\";")
        self.label_3.setObjectName("label_3")
        self.label_choose_parameters = QtWidgets.QLabel(self.centralwidget)
        self.label_choose_parameters.setGeometry(QtCore.QRect(10, 160, 341, 31))
        font = QtGui.QFont()
        font.setFamily("Adobe Heiti Std")
        font.setPointSize(10)
        font.setBold(False)
        font.setItalic(False)
        font.setWeight(50)
        self.label_choose_parameters.setFont(font)
        self.label_choose_parameters.setStyleSheet("color: rgb(14, 70, 159);\n"
"background-color: rgb(212, 228, 255);\n"
"font: 10pt \"Adobe Heiti Std\";")
        self.label_choose_parameters.setAlignment(QtCore.Qt.AlignCenter)
        self.label_choose_parameters.setObjectName("label_choose_parameters")
        self.label_LED_pulse_width = QtWidgets.QLabel(self.centralwidget)
        self.label_LED_pulse_width.setGeometry(QtCore.QRect(82, 220, 191, 16))
        self.label_LED_pulse_width.setStyleSheet("color: rgb(212, 228, 255);\n"
"font: 9pt \"Adobe Heiti Std\";")
        self.label_LED_pulse_width.setAlignment(QtCore.Qt.AlignCenter)
        self.label_LED_pulse_width.setObjectName("label_LED_pulse_width")
        self.horizontalSlider_2 = QtWidgets.QSlider(self.centralwidget)
        self.horizontalSlider_2.setGeometry(QtCore.QRect(60, 330, 231, 16))
        self.horizontalSlider_2.setCursor(QtGui.QCursor(QtCore.Qt.PointingHandCursor))
        self.horizontalSlider_2.setMinimum(1)
        self.horizontalSlider_2.setMaximum(4)
        self.horizontalSlider_2.setOrientation(QtCore.Qt.Horizontal)
        self.horizontalSlider_2.setTickPosition(QtWidgets.QSlider.TicksBelow)
        self.horizontalSlider_2.setTickInterval(10)
        self.horizontalSlider_2.setObjectName("horizontalSlider_2")
        self.label_samples_per_second = QtWidgets.QLabel(self.centralwidget)
        self.label_samples_per_second.setGeometry(QtCore.QRect(82, 300, 191, 21))
        self.label_samples_per_second.setStyleSheet("color: rgb(212, 228, 255);\n"
"font: 9pt \"Adobe Heiti Std\";")
        self.label_samples_per_second.setAlignment(QtCore.Qt.AlignCenter)
        self.label_samples_per_second.setObjectName("label_samples_per_second")
        self.horizontalSlider_3 = QtWidgets.QSlider(self.centralwidget)
        self.horizontalSlider_3.setGeometry(QtCore.QRect(60, 410, 231, 16))
        self.horizontalSlider_3.setCursor(QtGui.QCursor(QtCore.Qt.PointingHandCursor))
        self.horizontalSlider_3.setMinimum(1)
        self.horizontalSlider_3.setMaximum(32)
        self.horizontalSlider_3.setOrientation(QtCore.Qt.Horizontal)
        self.horizontalSlider_3.setTickPosition(QtWidgets.QSlider.TicksBelow)
        self.horizontalSlider_3.setTickInterval(10)
        self.horizontalSlider_3.setObjectName("horizontalSlider_3")
        self.label_LED_current_control = QtWidgets.QLabel(self.centralwidget)
        self.label_LED_current_control.setGeometry(QtCore.QRect(82, 390, 191, 16))
        self.label_LED_current_control.setStyleSheet("color: rgb(212, 228, 255);\n"
"font: 9pt \"Adobe Heiti Std\";")
        self.label_LED_current_control.setAlignment(QtCore.Qt.AlignCenter)
        self.label_LED_current_control.setObjectName("label_LED_current_control")
        self.label_9 = QtWidgets.QLabel(self.centralwidget)
        self.label_9.setGeometry(QtCore.QRect(62, 430, 281, 16))
        self.label_9.setStyleSheet("color: rgb(212, 228, 255);\n"
"font: 8pt \"Adobe Heiti Std\";")
        self.label_9.setObjectName("label_9")
        self.label_SpO2_ADC_range = QtWidgets.QLabel(self.centralwidget)
        self.label_SpO2_ADC_range.setGeometry(QtCore.QRect(82, 461, 191, 20))
        self.label_SpO2_ADC_range.setStyleSheet("color: rgb(212, 228, 255);\n"
"font: 9pt \"Adobe Heiti Std\";")
        self.label_SpO2_ADC_range.setAlignment(QtCore.Qt.AlignCenter)
        self.label_SpO2_ADC_range.setObjectName("label_SpO2_ADC_range")
        self.horizontalSlider_4 = QtWidgets.QSlider(self.centralwidget)
        self.horizontalSlider_4.setGeometry(QtCore.QRect(60, 490, 231, 16))
        self.horizontalSlider_4.setCursor(QtGui.QCursor(QtCore.Qt.PointingHandCursor))
        self.horizontalSlider_4.setMinimum(1)
        self.horizontalSlider_4.setMaximum(4)
        self.horizontalSlider_4.setOrientation(QtCore.Qt.Horizontal)
        self.horizontalSlider_4.setTickPosition(QtWidgets.QSlider.TicksBelow)
        self.horizontalSlider_4.setTickInterval(10)
        self.horizontalSlider_4.setObjectName("horizontalSlider_4")
        self.label_4 = QtWidgets.QLabel(self.centralwidget)
        self.label_4.setGeometry(QtCore.QRect(62, 350, 281, 16))
        self.label_4.setMouseTracking(False)
        self.label_4.setStyleSheet("color: rgb(212, 228, 255);\n"
"font: 8pt \"Adobe Heiti Std\";")
        self.label_4.setObjectName("label_4")
        self.label_5 = QtWidgets.QLabel(self.centralwidget)
        self.label_5.setGeometry(QtCore.QRect(50, 510, 281, 16))
        self.label_5.setMouseTracking(False)
        self.label_5.setStyleSheet("color: rgb(212, 228, 255);\n"
"font: 8pt \"Adobe Heiti Std\";")
        self.label_5.setObjectName("label_5")
        self.tabWidget = QtWidgets.QTabWidget(self.centralwidget)
        self.tabWidget.setGeometry(QtCore.QRect(390, 20, 661, 451))
        self.tabWidget.setFocusPolicy(QtCore.Qt.NoFocus)
        self.tabWidget.setContextMenuPolicy(QtCore.Qt.NoContextMenu)
        self.tabWidget.setLayoutDirection(QtCore.Qt.LeftToRight)
        self.tabWidget.setAutoFillBackground(False)
        self.tabWidget.setStyleSheet("color: rgb(14, 70, 159);\n"
"font: 9pt \"Adobe Heiti Std\";\n"
"background-color: rgb(212, 228, 255);\n"
"")
        self.tabWidget.setTabPosition(QtWidgets.QTabWidget.North)
        self.tabWidget.setTabShape(QtWidgets.QTabWidget.Rounded)
        self.tabWidget.setIconSize(QtCore.QSize(20, 20))
        self.tabWidget.setElideMode(QtCore.Qt.ElideNone)
        self.tabWidget.setDocumentMode(False)
        self.tabWidget.setTabsClosable(False)
        self.tabWidget.setMovable(False)
        self.tabWidget.setTabBarAutoHide(False)
        self.tabWidget.setObjectName("tabWidget")
        self.tab_HR = QtWidgets.QWidget()
        self.tab_HR.setObjectName("tab_HR")
        self.label_HR_prova = QtWidgets.QLabel(self.tab_HR)
        self.label_HR_prova.setGeometry(QtCore.QRect(250, 150, 181, 101))
        self.label_HR_prova.setObjectName("label_HR_prova")
        self.line_3 = QtWidgets.QFrame(self.tab_HR)
        self.line_3.setGeometry(QtCore.QRect(-10, -5, 681, 10))
        self.line_3.setStyleSheet("background-color: rgb(14, 70, 159);")
        self.line_3.setFrameShape(QtWidgets.QFrame.HLine)
        self.line_3.setFrameShadow(QtWidgets.QFrame.Sunken)
        self.line_3.setObjectName("line_3")
        self.tabWidget.addTab(self.tab_HR, "")
        self.tab_spo2 = QtWidgets.QWidget()
        self.tab_spo2.setObjectName("tab_spo2")
        self.label_spo2_prova = QtWidgets.QLabel(self.tab_spo2)
        self.label_spo2_prova.setGeometry(QtCore.QRect(250, 140, 181, 101))
        self.label_spo2_prova.setObjectName("label_spo2_prova")
        self.line_2 = QtWidgets.QFrame(self.tab_spo2)
        self.line_2.setGeometry(QtCore.QRect(-10, -5, 681, 10))
        self.line_2.setStyleSheet("background-color: rgb(14, 70, 159);")
        self.line_2.setFrameShape(QtWidgets.QFrame.HLine)
        self.line_2.setFrameShadow(QtWidgets.QFrame.Sunken)
        self.line_2.setObjectName("line_2")
        self.tabWidget.addTab(self.tab_spo2, "")
        self.line_4 = QtWidgets.QFrame(self.centralwidget)
        self.line_4.setGeometry(QtCore.QRect(720, 20, 5, 30))
        self.line_4.setFrameShape(QtWidgets.QFrame.VLine)
        self.line_4.setFrameShadow(QtWidgets.QFrame.Sunken)
        self.line_4.setObjectName("line_4")
        self.label = QtWidgets.QLabel(self.centralwidget)
        self.label.setGeometry(QtCore.QRect(140, 50, 91, 91))
        self.label.setText("")
        #self.label.setPixmap(QtGui.QPixmap("hr_image.png"))
        self.label.setPixmap(QtGui.QPixmap("D:\Polimi\LAB\PROJECT\AY2122_II_Project-1\GUI\hr_image.png"))
        self.label.setScaledContents(True)
        self.label.setObjectName("label")
        self.tabWidget.raise_()
        self.pushButton_Start.raise_()
        self.pushButton_Stop.raise_()
        self.line.raise_()
        self.horizontalSlider.raise_()
        self.label_3.raise_()
        self.label_choose_parameters.raise_()
        self.label_LED_pulse_width.raise_()
        self.horizontalSlider_2.raise_()
        self.label_samples_per_second.raise_()
        self.horizontalSlider_3.raise_()
        self.label_LED_current_control.raise_()
        self.label_9.raise_()
        self.label_SpO2_ADC_range.raise_()
        self.horizontalSlider_4.raise_()
        self.label_connection.raise_()
        self.label_4.raise_()
        self.label_5.raise_()
        self.line_4.raise_()
        self.label.raise_()
        MainWindow.setCentralWidget(self.centralwidget)
        self.menubar = QtWidgets.QMenuBar(MainWindow)
        self.menubar.setGeometry(QtCore.QRect(0, 0, 1077, 26))
        self.menubar.setObjectName("menubar")
        MainWindow.setMenuBar(self.menubar)
        self.statusbar = QtWidgets.QStatusBar(MainWindow)
        self.statusbar.setObjectName("statusbar")
        MainWindow.setStatusBar(self.statusbar)

        self.retranslateUi(MainWindow)
        self.tabWidget.setCurrentIndex(1)
        QtCore.QMetaObject.connectSlotsByName(MainWindow)

    def retranslateUi(self, MainWindow):
        _translate = QtCore.QCoreApplication.translate
        MainWindow.setWindowTitle(_translate("MainWindow", "Photodetector"))
        self.pushButton_Start.setText(_translate("MainWindow", "Start"))
        self.pushButton_Stop.setText(_translate("MainWindow", "Stop"))
        self.label_connection.setText(_translate("MainWindow", "Connected to..."))
        self.label_3.setText(_translate("MainWindow", "69                 118                 215                   411 "))
        self.label_choose_parameters.setText(_translate("MainWindow", "Parameters setting"))
        self.label_LED_pulse_width.setText(_translate("MainWindow", "LED Pulse Width (μs)"))
        self.label_samples_per_second.setText(_translate("MainWindow", "Samples per second"))
        self.label_LED_current_control.setText(_translate("MainWindow", "LED Current Control (mA)"))
        self.label_9.setText(_translate("MainWindow", "0.2                                                                   6.2"))
        self.label_SpO2_ADC_range.setText(_translate("MainWindow", "SpO2 ADC Range"))
        self.label_4.setText(_translate("MainWindow", "50                 100                 200                   400 "))
        self.label_5.setText(_translate("MainWindow", "2048               4096               8192               16384 "))
        self.label_HR_prova.setText(_translate("MainWindow", "Heart Rate - prova"))
        self.tabWidget.setTabText(self.tabWidget.indexOf(self.tab_HR), _translate("MainWindow", "                                        Heart Rate                                     "))
        self.label_spo2_prova.setText(_translate("MainWindow", "Spo2 - prova"))
        self.tabWidget.setTabText(self.tabWidget.indexOf(self.tab_spo2), _translate("MainWindow", "                                             SpO2                                          "))

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

  ####################
    # SERIAL INTERFACE #
    ####################
    @pyqtSlot(bool)
    def serialscan(self,checked):
        if checked:
            # setup reading worker
            self.serial_worker = SerialWorker(self.port_text) # needs to be re defined
            # connect worker signals to functions
            self.serial_worker.signals.status.connect(self.check_serialport_status)
            self.serial_worker.signals.device_port.connect(self.connected_device)
            # execute the worker
            self.threadpool.start(self.serial_worker)
            print('ok') ## NON VA!!
        else:
            # kill thread
            self.serial_worker.is_killed = True
            self.serial_worker.killed()
            self.com_list_widget.setDisabled(False) # enable the possibility to change port
            self.conn_btn.setText(
                "Connect to port {}".format(self.port_text)
            )

       # self.pushButton_Start.clicked.connect(self.clicked)
        self.pushButton_Start.clicked.connect(lambda state, x=START: self.start_stop_control(state, x))
       # self.pushButton_Stop.clicked.connect(self.clicked)
        self.pushButton_Stop.clicked.connect(lambda state, x=STOP: self.start_stop_control(state, x))

    ##################
    # SERIAL SIGNALS #
    ##################
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
                        self.pushButton_Stop.setCheckable(False)
                        self.pushButton_Start.setCheckable(False)
                elif status == 1:
                        # enable all the widgets on the interface
                        #self.com_list_widget.setDisabled(
                        #    True)  # disable the possibility to change COM port when already connected
                        self.label_connection.setText('Connected to {}'.format(self.port_name))
                        self.label_connection.adjustSize()
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
                self.label_2.setText('Connected to {}'.format(self.port_name))
                self.label_2.adjustSize()
                logging.info('Connected to {}'.format(self.port_name))

            elif x==STOP:
                self.serial_worker.is_killed = True
                self.serial_worker.killed()
                self.label_2.setText('Stopped the connection with {}'.format(self.port_name))

        else:
            self.label_2.setText('Error in the connection with PSoC')
            self.label_2.adjustSize()
            logging.info('Error in the connection with PSoC')


#############
#  RUN APP  #
#############
if __name__ == "__main__":
    import sys
    app = QtWidgets.QApplication(sys.argv)
    MainWindow = QtWidgets.QMainWindow()
    ui = Ui_MainWindow()
    ui.setupUi(MainWindow)
    MainWindow.show()
    sys.exit(app.exec_())
