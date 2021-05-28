import asyncio
import struct
from bleak import BleakScanner
import tago
from varname.helpers import Wrapper
import math

my_device = tago.Device('013dc384-56e6-4dc2-8a86-380701c5ad4e')


def create_dict(var):
    data = {
        'variable': f"{var.name}_Accel",
        'unit': 'ms^-2',
        'value': var.value,
    }
    return data


def detection_callback(device, advertisement_data):
    if device.address == "EF:B9:B8:80:14:B7" and advertisement_data.service_data:
        a = next(iter(advertisement_data.service_data.values()))
        x = struct.unpack('f', a[:4])[0]
        y = struct.unpack('f', a[4:8])[0]
        z = struct.unpack('f', a[8:12])[0]
        pitch = math.atan(x / math.sqrt(y * y + z * z)) * 180.0 / math.pi
        roll = math.atan(y / math.sqrt(x * x + z * z)) * 180.0 / math.pi
        dictArr = []
        x = Wrapper(x)
        y = Wrapper(y)
        z = Wrapper(z)
        dictArr.append(create_dict(x))
        dictArr.append(create_dict(y))
        dictArr.append(create_dict(z))
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
        dictArr.append(data)
        print(x, y, z, pitch, roll)

        result = my_device.insert(dictArr)
        print(result)


async def run():
    scanner = BleakScanner()
    scanner.register_detection_callback(detection_callback)
    await scanner.start()
    # await asyncio.sleep(10.0)
    # await scanner.stop()
    devices = await scanner.get_discovered_devices()

loop = asyncio.get_event_loop()
loop.create_task(run())
loop.run_forever()
# loop.run_until_complete(run())
