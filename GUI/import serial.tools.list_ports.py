import serial.tools.list_ports
 
name_kit = "Cypress"
ports = list(serial.tools.list_ports.comports())
for p in ports:
    manufacturer = p.manufacturer
    if manufacturer == name_kit:
        print (p)

# KitProg USB-UART = nome visualizzato durante il collegamento

# row 96 = self.port = serial.Serial()

# https://www.programcreek.com/python/example/97508/serial.tools.list_ports.comports