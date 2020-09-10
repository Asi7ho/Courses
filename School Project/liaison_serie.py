import serial

ser = serial.Serial('COM15', 9600)
while True:
    line = ser.readline(100)
    print line
