from PIL import Image
import numpy as np
from bleak import BleakScanner
import struct
import asyncio
import csv
from math import sqrt, atan, pi
import tago
import time


filename = input("Please Enter A Filename: ")
action = input("Please Enter Your Activity: ")
duration = input(
    "Please Enter How Many Seconds You Want To Collect Data For: ")

dictArr = []

start = 0

aidenThingy = "EF:B9:B8:80:14:B7"
georgeThingy = "CD:B0:C1:93:02:48"
aidenBool = False
georgeBool = False
tempDictArr = np.empty(2, dtype=object)

my_device = tago.Device('013dc384-56e6-4dc2-8a86-380701c5ad4e')


def write_data():
    csv_file = f"{filename}.csv"
    csv_columns = ['Accelerometor_Data', 'Action']
    try:
        with open(csv_file, 'a', newline='') as csvfile:
            writer = csv.DictWriter(csvfile, fieldnames=csv_columns)
            for data in dictArr:
                writer.writerow(data)
    except IOError:
        print("I/O error")


def upload_to_dashboard(x, y, z, action):
    pitch = atan(x / sqrt(y * y + z * z)) * 180.0 / pi
    roll = atan(y / sqrt(x * x + z * z)) * 180.0 / pi
    dictArr = []
    data = {
        'variable': f"x_Accel",
        'unit': 'ms^-2',
        'value': x,
    }
    dictArr.append(data)
    data = {
        'variable': f"y_Accel",
        'unit': 'ms^-2',
        'value': y,
    }
    dictArr.append(data)
    data = {
        'variable': f"z_Accel",
        'unit': 'ms^-2',
        'value': z,
    }
    dictArr.append(data)
    data = {
        'variable': "Pitch",
        'unit': '°',
        'value': pitch,
    }
    dictArr.append(data)
    data = {
        'variable': "Roll",
        'unit': '°',
        'value': roll,
    }
    result = my_device.insert(dictArr)


def detection_callback(device, advertisement_data):
    global aidenBool
    global georgeBool
    try:
        if (device.address == aidenThingy or device.address == georgeThingy) and advertisement_data.service_data:
            a = next(iter(advertisement_data.service_data.values()))
            x = struct.unpack('f', a[:4])[0]
            y = struct.unpack('f', a[4:8])[0]
            z = struct.unpack('f', a[8:12])[0]
            accelArr = [x, y, z]
    except Exception as e:
        print("Advertisement Data")
        print(advertisement_data.service_data)
        print(e)
    if device.address == aidenThingy and advertisement_data.service_data:
        tempDictArr[0] = accelArr
        aidenBool = True
    elif device.address == georgeThingy and advertisement_data.service_data:
        tempDictArr[1] = accelArr
        georgeBool = True
    if georgeBool and aidenBool:
        percentage = ((time.time() - start)/float(duration))*100
        percentage = round(percentage, 2)
        if percentage > 100:
            percentage = 100.00
        print(f"Current Percentage {percentage}%")
        trainData = {"Accelerometor_Data": [
            tempDictArr[0], tempDictArr[1]], "Action": action}
        dictArr.append(trainData)
        aidenBool = False
        georgeBool = False


async def run():
    global start
    scanner = BleakScanner()
    scanner.register_detection_callback(detection_callback)
    start = time.time()
    await scanner.start()
    await asyncio.sleep(float(duration))
    await scanner.stop()
    end = time.time()

loop = asyncio.get_event_loop()
loop.run_until_complete(run())
write_data()
print("Data has been successfully written to the file")
