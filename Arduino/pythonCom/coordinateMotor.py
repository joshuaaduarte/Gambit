import serial
import time

arduinoData = serial.Serial('COM3',9600)
arduinoData.timeout = 1


while True:
    i = input("Input Coordinate One: ").strip()
    if i == 'done':
        print("Program done")
        break
    arduinoData.write(i.encode())
    time.sleep(.5)

    j = input("Input Coordinate Two ").strip()
    arduinoData.write(j.encode())
    time.sleep(1)


arduinoData.close()