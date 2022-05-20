# VERSIONE 1
# utilizzo nome del manufacturer
 
#name_kit = "Cypress"  
#ports = list(serial.tools.list_ports.comports())
#for p in ports:
 #   manufacturer = p.manufacturer
  #  if manufacturer == name_kit:
   #     print (p)

##################################

# VERSIONE 2
# utilizzo parte stringa del nome + implementazione connessione

import serial.tools.list_ports

ports = serial.tools.list_ports.comports()

def findPsoC(portsFound):
    commPort = 'None'
    n_connections = len(portsFound)

    for i in range(0,n_connections):
        port = portsFound[i]
        strPort = str(port)

        if 'Intel' in strPort: # poi sostituire con Cypress! 
            splitPort = strPort.split(' ')
            commPort = (splitPort[0])

    return commPort

#foundPorts = get_ports()
connectPort = findPsoC(ports)

if connectPort != 'None':
    ser = serial.Serial(connectPort, baudrate = 115200, timeout = 1)
    print('Connected to ' + connectPort)

else: 
    print('Error in the connection with PSoC')

print('code finished')