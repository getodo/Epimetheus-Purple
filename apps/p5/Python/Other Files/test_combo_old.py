import numpy as np
from bleak import BleakScanner
import struct
import asyncio
import csv
from typing import NewType
import pandas as pd
import re
from sklearn.neighbors import KNeighborsClassifier
from sklearn.linear_model import LogisticRegression
from sklearn.naive_bayes import GaussianNB
from sklearn.ensemble import RandomForestClassifier, VotingClassifier
from sklearn import metrics

aidenThingy = "EF:B9:B8:80:14:B7"
georgeThingy = "CD:B0:C1:93:02:48"
aidenBool = False
georgeBool = False
tempDictArr = np.empty(2, dtype=object)

df = pd.ExcelFile(
    r"C:\Users\Aiden\csse4011\csse4011-s4529666\TrainingDataProject.xlsx")

dfs = df.parse()
s = np.array((dfs['Accelerometor Data'].str.translate(str.maketrans({'[': '', ']': ''})).tolist()))
newArr = []

for row in s:
    t = row.split(',')
    t = np.array(t)
    t = t.astype(float)
    newArr.append(t)

accelArr = np.array(newArr)
actionArr = np.array(dfs['Action'])

# Create KNN classifier
knn = KNeighborsClassifier(n_neighbors=3)
clf1 = LogisticRegression(multi_class='multinomial', random_state=1, max_iter=7600)
clf2 = RandomForestClassifier(n_estimators=50, random_state=1)
clf3 = GaussianNB()
eclf1 = VotingClassifier(estimators=[('lr', clf1), ('rf', clf2), ('gnb', clf3), ('knn', knn)], voting='hard')
eclf1 = eclf1.fit(accelArr, actionArr)
# knn.fit(accelArr, actionArr)


def detection_callback(device, advertisement_data):
    global aidenBool
    global georgeBool
    global knn
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
        tempAccel = np.append(tempDictArr[0], tempDictArr[1])
        print(eclf1.predict([tempAccel]))
        aidenBool = False
        georgeBool = False


async def run():
    scanner = BleakScanner()
    scanner.register_detection_callback(detection_callback)
    await scanner.start()
    await asyncio.sleep(float(60))
    await scanner.stop()
    devices = await scanner.get_discovered_devices()


loop = asyncio.get_event_loop()
# loop.create_task(run())
# loop.run_forever()
loop.run_until_complete(run())