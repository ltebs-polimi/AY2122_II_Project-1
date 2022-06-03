# https://raspberrypi.stackexchange.com/questions/117711/read-serial-data-from-multiple-arduino

# https://stackoverflow.com/questions/64193693/how-to-read-serial-data-with-python-from-more-then-one-arduino-adc

# https://stackoverflow.com/questions/65654194/how-to-read-multiple-values-from-com-port-using-pyserial-with-python

# https://stackoverflow.com/questions/70168434/sending-multiple-sensor-data-from-arduino-to-python-any-idea

while(port.inwaiting >0):   # inwaiting = Get the number of bytes in the input buffer
    if (stato = header):
        header = read(1),decode('utf-8')
        header = struct.unpack('1B', header)[0]
            if(header == header_IR):
                flag_IR = 1 

    if (stato = dato):
        if(flag_IR):
            dato = read(200).decode('utf-8')
            dato = struct.unpack('200B',dato)

    if(stato = tail):
        tail....
            if(tail == tail_IR)

            ## plot dati 