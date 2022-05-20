# REQUIREMENTS
# 1. graph + value of SPO2 and HR
# 2. Automatic detection of the COM port: this can be performed by sending a custom command(e.g. v) from the host side, 
# to which the PSoC must answer in a determinate way (e.g. HeartRate $$$). Upon detection of the expected answer, 
# then the COM port is open and connection occurs.
# 3. The GUI should allow to start/stop data streaming from the device side. A command of b should start data streaming,
#  a command of s should stop data streaming.
# 4. options:
# a. SpO2 Sample Rate control
# b. LED Pulse Width control
# c. SpO2 ADC Range control
# d. LED Current control

import sys

import time

import logging

from PyQt5 import QtCore
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
    QLabel,         # Text or image display
    QPushButton,    # Command button
    QComboBox,
    QVBoxLayout,    # to construct vertical box layout objects
    QHBoxLayout,
    QWidget,        # base class of all user interface objects
)

import serial
import serial.tools.list_ports

# Globals
CONN_STATUS = False # global variable used to keep track of the connection status of the device 

START = "b"
STOP = "s"

# Logging config
logging.basicConfig(format="%(message)s", level=logging.INFO)
# when you use a logging library you have to configure the logging system, configuring it in the simplest way possible the logging model
# you can display different type of messages according to the severity of the error (if it is an information a warning, a severe error etc). 
# so you need to cofigure the logging module accordingly

#########################
# SERIAL_WORKER_SIGNALS #
#########################
# the only interaction possible between parallel strategies are signals; so we define a class which is a child class of QObject class; 
# Worker beacuse we use workers, that there are the systems to perform multi-threahding
# this class contains the signals available to the serial workers; 
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
# we have device port which is a signal carrying a string, while status is
# pyqt signal carrying a string and a integer. They are customizable. 
    device_port = pyqtSignal(str)
    status = pyqtSignal(str, int)


#################
# SERIAL_WORKER #
#################
# We define the class related to a QRunnable object; we subclass it to call it SerialWorker. 
# We want to personalize the QRunnable
class SerialWorker(QRunnable):
    """!
    @brief Main class for serial communication: handles connection with device.
    """
    def __init__(self, serial_port_name):
        """!
        @brief Init worker.
        """
        self.is_killed = False # boolean variable to keep track of when we want to kill down the thread; 
        # in this specific example the  thread is just a conenction between the port so the killing of the 
        # process is not really good beacuse if it doesnt connect, it stops; it is not a continous runnign class. 
        super().__init__()
        # init port, params and signals
        self.port = serial.Serial() # initialize the serial port with a name that is passed as an input 
        self.port_name = serial_port_name # when using this init we'll need to pass the name of the serial port to establish the connection
        self.baudrate = 115200 # hard coded but can be a global variable, or an input param; it is a unit of measure of the speed of the data
                             # transfer of devices connected in a serial way
        self.signals = SerialWorkerSignals() # initialize the signal class, defined above 


    @pyqtSlot()
    # it allwos a more efficient communication between threads
    # the run method is the method that is called when we actualyl will run indeed the worker thread; we'll see that in the main window
    # we'll use this serial worker thread and when we use it, it run the class called run 
    # what we want this tread to to is to create a connection between the pc and what is connected to the serial port 
    def run(self):
        """!
        @brief Estabilish connection with desired serial port.
        """
        global CONN_STATUS

        if not CONN_STATUS: # if nothing is conencted, we try to create a new serial port with the name in input beacuse we need to open a
        # serial port with that name, with that specific baudrate. If this connection happens and "is_open", then the connection has been 
        # established beacuse it mans that the computer reached to open the connection with that name and that baudrate.
            try:
                self.port = serial.Serial(port=self.port_name, baudrate=self.baudrate,
                                        write_timeout=0, timeout=2)                
                if self.port.is_open:
                    CONN_STATUS = True
                    self.signals.status.emit(self.port_name, 1) # line in which we establsih a conenction between the GUI thread and the
                    # task carried out in the parallel thread; we send a signal from this thead to the GUI thread by emitting manually
                    # So we need a specific signal, user-defined --> we need to define our own signal and to use it thanks to the emit
                    # method.
                    # we send the port name and then 1; where 1 - if we go in the documentation of the SerialWorkerSignals- we see that it                       
                    # represents the state that means success
                    time.sleep(0.01)   # it stops for just a tiny bit of time; time is a library that we have improted in the beginning
                    # it is needed because sometimes we need to wait some ms for synchronization; if we dont wait the thread doesnt work.
                    # it is to allow the hardware to stabilize to the condition of connection establsihed beacuse probably it is due to the
                    # fact that the execution of the code is very fast while the hardware connection need some time to be compelted. 
            
            # if we didnt succeed, we create an expection
            # in this case there have been an error! 
            # this allows to generate some system errors;
            # we dont use "print error" but we use logging.info since we want to display information (if you want you can write
            # "logging.critical"); all the things after info will be executed but not shown to the user.
            except serial.SerialException:
                logging.info("Error with port {}.".format(self.port_name)) # error with indicated the port
                self.signals.status.emit(self.port_name, 0) # here we emit the same sigal but we dont pass 1 but 0, beacsue the signal is                   
                # the same but now the connection has failed
                time.sleep(0.01) # once again, there is a tiny fraction of sleep 

    @pyqtSlot() # decorator 
    def send(self, char): #
        """!
        @brief Basic function to send a single char on serial port.
        """
        try:
            self.port.write(char.encode('utf-8'))
            logging.info("Written {} on port {}.".format(char, self.port_name))
        except:
            logging.info("Could not write {} on port {}.".format(char, self.port_name))

   
    @pyqtSlot()
    # it is the method that is run whenever in this case we want to close the serial port; we want the COM port to be closed.
    # If the user quit the application and the prot is open, we want to be sure to close it even if the user has forgotten about it.
    # in the main window we see that the kileld metod will be call two times: when the user close the application and when the user 
    # click on disconnect button. 
    def killed(self):
        """!
        @brief Close the serial port before closing the app.
        """
        global CONN_STATUS
        if self.is_killed and CONN_STATUS:
            self.port.close() # what it does is closing the port
            time.sleep(0.01) # tiny fraction of sleep
            CONN_STATUS = False # beacuse nothing is connected
            self.signals.device_port.emit(self.port_name) # to dispaly to the user in the terminal that that port has been closed

        logging.info("Killing the process") # signal just to show that upon the clsoe of the application this method is
        # actaully called (a sort of debugger line of code) 

###############
# MAIN WINDOW #
###############
class MainWindow(QMainWindow):
    def __init__(self):
        """!
        @brief Init MainWindow.
        """
        # define worker
        self.serial_worker = SerialWorker(None) # we have to isntantiate the thread; here None is 
        # passed beacuse SerialWorker need the port name but since it is an initialization I do not 
        # know which port I want to open. If I put () it generates an error beacuse it wants a parameter

        super(MainWindow, self).__init__()

        # title and geometry
        self.setWindowTitle("Port Selection")
        width = 500
        height = 350
        self.setMinimumSize(width, height)

        # create thread handler
        self.threadpool = QThreadPool() # # enviroment in which the driver leaves; so we isntatiate also this class

        self.connected = CONN_STATUS
        self.serialscan() # method that handles when it runs the app to see if there is the connection 
        self.initUI()

    #####################
    # GRAPHIC INTERFACE #
    #####################
    def initUI(self): # define the layout --> we need button and the dropdown menu and here we need only tos et the graphical inerface
        """!
        @brief Set up the graphical interface structure.
        """
        # layout
        # we need only the horizontal backgroudn ebacuse we want the 2 parts near to each other 
        button_hlay = QHBoxLayout()
        button_hlay.addWidget(self.com_list_widget)
        button_hlay.addWidget(self.conn_btn)
        widget = QWidget()
        widget.setLayout(button_hlay)
        self.setCentralWidget(widget)

   ####################
    # SERIAL INTERFACE #
    ####################
    def serialscan(self):
        """!
        @brief Scans all serial ports and create a list.
        """
        # create the combo box to host port list
        self.port_text = ""
        self.com_list_widget = QComboBox() # we want to change the text on the button; we want to read COM10 etc so we need to connect these
        # quantities --> we use signals
        self.com_list_widget.currentTextChanged.connect(self.port_changed) # when the user selects another option the text changes, it               generates a signal, it catches the signal and it will use it in this method 
        
        # create the connection button
        # the button has connected to port, which is parametric, it is not fixed but it depends on what ahs been found during the scan and 
        # on what the user selects in the menu
        self.conn_btn = QPushButton(
            text=("Connect to port {}".format(self.port_text)), 
            checkable=True,
            toggled=self.on_toggle # not on click, but on toggle (see the documentation); we link toogle to the method on toogle
            # toogle carries another information in addition to the click; it differentiate if you cikc the button to enable it or to 
            # disable it (Difference bettwen push and release) 
        )

        # acquire list of serial ports and add it to the combo box
        # we need to scan the serial prots; we create a list and take the name of the active serial port 
        name_kit = "Intel"  # da sostituire con Cypress
        ports = list(serial.tools.list_ports.comports())

        # Nella lista delle ports ora viene fuori solo quella in cui il manufacturer = name_kit 
        # --> da ottimizzare!
        for p in ports:
            manufacturer = p.manufacturer
            if manufacturer == name_kit:
               serial_ports = [
                p.name # put the name in the lsit         
                ]
        self.com_list_widget.addItems(serial_ports)


    ##################
    # SERIAL SIGNALS #
    ##################
    
    def port_changed(self):
        # signal used when the text was link to the current exchange on the menu; we need to change the text on the button according to
        # the choice made by the user; so we take the choice made by the user beacuse we have a fucntion that allows to take the ?? and put
        # onto the button, saving it in a varibale and use the attribute contained as parameter. 
        """!
        @brief Update conn_btn label based on selected port.
        """
        self.port_text = self.com_list_widget.currentText()
        self.conn_btn.setText("Connect to port {}".format(self.port_text))

    @pyqtSlot(bool)
    def on_toggle(self, checked):
        """!
        @brief Allow connection and disconnection from selected serial port.
        """
        if checked: # the user has pressed it to estalblsih the connection
            # setup reading worker
            self.serial_worker = SerialWorker(self.port_text) # needs to be redfined --> now we now the name of the port, which is the oe
            # selected by the user
            # connect worker signals to functions
            self.serial_worker.signals.status.connect(self.check_serialport_status) # we have to tell to the program which are the signals
            # used to catch the signal status; it is always the same procedure already seen for the button etc.
            self.serial_worker.signals.device_port.connect(self.connected_device) 
            # execute the worker
            self.threadpool.start(self.serial_worker)
        else:
            # kill thread
            self.serial_worker.is_killed = True
            self.serial_worker.killed()
            self.com_list_widget.setDisabled(False) # enable the possibility to change port
            self.conn_btn.setText(
                "Connect to port {}".format(self.port_text)
            )

    def check_serialport_status(self, port_name, status):
        """!
        @brief Handle the status of the serial port connection.

        Available status:
            - 0  --> Error during opening of serial port
            - 1  --> Serial port opened correctly
        """
        # if 0 is received, it means that the connection has failed; if the connection has failed we dont change anything in the application
        # the button will still dispaly "connect to port.."
        # if instead 1 has been recieved, the connection has been establsihed --> change the drop down menu 
        if status == 0:
            self.conn_btn.setChecked(False)
        elif status == 1:
            # enable all the widgets on the interface
            self.com_list_widget.setDisabled(True) # disable the possibility to change COM port when already connected
            self.conn_btn.setText(
                "Disconnect from port {}".format(port_name)
            )

    def connected_device(self, port_name):
        # it prints the infromation; it is passed the port name, which is passed by the signal 
        """!
        @brief Checks on the termination of the serial worker.
        """
        logging.info("Port {} closed.".format(port_name))


    def ExitHandler(self):
        # it is called when you close the application, when you press the cross.
        # also this action generates a signal; you can intercepts this signal and conenct to another method
        # as you can see, it set the boolean variable is_killed to true so that any parralell thread has stop and then it runs the key               method of the parallel worker 
        """!
        @brief Kill every possible running thread upon exiting application.
        """
        self.serial_worker.is_killed = True
        self.serial_worker.killed()

        
#############
#  RUN APP  #
#############
if __name__ == '__main__':
    app = QApplication(sys.argv)
    w = MainWindow()
    app.aboutToQuit.connect(w.ExitHandler) # this is the signal generated when you press the cross to quit; before closing the application
    # but after clicking on the button that is connected to the method ExitHandler. 
    # this allows to be sure that everything has closed 
    w.show()
    sys.exit(app.exec_())
    