# REQUIREMENTS
# 1. graph + value of SPO2 and HR
# 2. Automatic detection of the COM port: this can be performed by sending a custom command(e.g. v) from the host side, 
# to which the PSoC must answer in a determinate way (e.g. HeartRate $$$). Upon detection of the expected answer, 
# then the COM port is open andconnection occurs.
# 3. The GUI should allow to start/stop data streaming from the device side. A command of bshould start data streaming,
#  a command of s should stop data streaming.
# 4. options:
# a. SpO2 Sample Rate control
# b. LED Pulse Width control
# c. SpO2 ADC Range control
# d. LED Current control

