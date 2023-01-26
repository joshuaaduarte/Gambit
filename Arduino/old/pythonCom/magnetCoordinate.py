import serial
import time

arduinoData = serial.Serial('COM3',9600)
arduinoData.timeout = 1


while True:
    xCoordinateOne = input("Input x-coordinate one: ").strip()
    if xCoordinateOne == 'done':
        print("Program done")
        break
    arduinoData.write(xCoordinateOne.encode())
    time.sleep(.5)

    yCoordinateOne = input("Input y-coordinate one: ").strip()
    arduinoData.write(yCoordinateOne.encode())
    time.sleep(.5)

    xCoordinateTwo = input("Input x-coordinate two: ").strip()
    arduinoData.write(xCoordinateTwo.encode())
    time.sleep(.5)

    yCoordinateTwo = input("Input y-coordinate two: ").strip()
    arduinoData.write(yCoordinateTwo.encode())
    time.sleep(1)


arduinoData.close()