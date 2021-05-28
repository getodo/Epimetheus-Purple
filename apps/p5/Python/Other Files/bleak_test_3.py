import asyncio
from bleak import BleakScanner

def detection_callback(device, advertisement_data):
    print(device.address, "RSSI:", device.rssi, advertisement_data)

async def run():
    scanner = BleakScanner()
    scanner.register_detection_callback(detection_callback)
    while True:
        await scanner.start()
        await asyncio.sleep(0.5)
        await scanner.stop()
        # devices = await scanner.get_discovered_devices()

loop = asyncio.get_event_loop()
loop.run_until_complete(run())