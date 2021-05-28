from PIL import Image
import numpy as np
from bleak import BleakScanner
import struct
import asyncio
import csv

action = input("Please Enter Your Activity: ")
duration = input(
    "Please Enter How Many Seconds You Want To Collect Data For: ")
filename = input("Please Enter A Filename: ")

dictArr = []


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
    if device.address == "EF:B9:B8:80:14:B7" and advertisement_data.service_data:
        a = next(iter(advertisement_data.service_data.values()))
        x = struct.unpack('f', a[:4])[0]
        y = struct.unpack('f', a[4:8])[0]
        z = struct.unpack('f', a[8:12])[0]
        trainData = {"Accelerometor_Data": [x, y, z], "Action": action}
        dictArr.append(trainData)


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
