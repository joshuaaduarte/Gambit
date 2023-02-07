import serial
import time

arduinoData = serial.Serial('COM3',9600)
arduinoData.timeout = 1

# def selectDirection():
#     arduinoData.write(str(input("What direction?")))
#
# def selectRotations():
#     arduinoData.write(int(input("How many rotations?")))
#
#
# selectDirection()
# selectRotations()

while True:
    i = input("What Direction: ").strip()
    if i == 'done':
        print("Program done")
        break
    arduinoData.write(i.encode())
    time.sleep(.5)

    j = input("How many Rotations: ").strip()
    arduinoData.write(j.encode())
    time.sleep(1)


arduinoData.close()