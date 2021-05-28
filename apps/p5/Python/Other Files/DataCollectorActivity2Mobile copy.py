from PIL import Image
import numpy as np
from bleak import BleakScanner
import struct
import asyncio
import csv

filename = input("Please Enter A Filename: ")
action = input("Please Enter Your Activity: ")
duration = input(
    "Please Enter How Many Seconds You Want To Collect Data For: ")

dictArr = []

aidenThingy = "EF:B9:B8:80:14:B7"
georgeThingy = "CD:B0:C1:93:02:48"
aidenBool = False
georgeBool = False
tempDictArr = np.empty(2, dtype=object)


def write_data():
    csv_file = f"{filename}.csv"
    csv_columns = ['Accelerometor_Data', 'Action']
    try:
        with open(csv_file, 'a', newline='') as csvfile:
            writer = csv.DictWriter(csvfile, fieldnames=csv_columns)
            # writer.writeheader()
            for data in dictArr:
                writer.writerow(data)
    except IOError:
        print("I/O error")


def detection_callback(device, advertisement_data):
    global aidenBool
    global georgeBool
    # print(aidenBool, georgeBool)
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
        print("Still Alive")
        trainData = {"Accelerometor_Data": [
            tempDictArr[0], tempDictArr[1]], "Action": action}
        # print(trainData)
        dictArr.append(trainData)
        aidenBool = False
        georgeBool = False


async def run():
    scanner = BleakScanner()
    scanner.register_detection_callback(detection_callback)
    await scanner.start()
    await asyncio.sleep(float(duration))
    await scanner.stop()
    devices = await scanner.get_discovered_devices()


loop = asyncio.get_event_loop()
# loop.create_task(run())
# loop.run_forever()
loop.run_until_complete(run())
write_data()
print("Data has been successfully written to the file")
