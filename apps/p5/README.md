# Practical 4

## My Details

First Name: Aiden  
Second Name: Kearney  
Student ID: 45296662  
  
First Name: George  
Second Name: Todorovic  
Student ID: 43537370  

## Tasks

### Part A

Task 1: Succuesfully collected training data and uploaded data to web dashboard.  
Task 2: Successfully used machine learning to allow beacons to identify where the mobile node is.  
Task 3: Web dashboard displaying RSSI information and ultrasonic information and is graphed.  
Task 4: Able to successfully track 2 nodes.  

## Folder Structure

📦p4  
 ┣ 📂Base\_Node  
 ┃ ┣ 📂src  
 ┃ ┃ ┗ 📜main.c  
 ┃ ┣ 📜CMakeLists.txt  
 ┃ ┗ 📜prj.conf  
 ┣ 📂beacon  
 ┃ ┣ 📂boards  
 ┃ ┃ ┗ 📜rv32m1\_vega\_ri5cy.overlay  
 ┃ ┣ 📂src  
 ┃ ┃ ┗ 📜main.c  
 ┃ ┣ 📜CMakeLists.txt  
 ┃ ┣ 📜prj.conf  
 ┃ ┣ 📜README.rst  
 ┃ ┗ 📜sample.yaml  
 ┣ 📂Disco\_Ultrasonic  
 ┃ ┣ 📂src  
 ┃ ┃ ┗ 📜main.c  
 ┃ ┣ 📜CMakeLists.txt  
 ┃ ┣ 📜disco\_l475\_iot1.overlay  
 ┃ ┗ 📜prj.conf  
 ┣ 📂GUI  
 ┃ ┣ 📂\_\_pycache\_\_  
 ┃ ┃ ┣ 📜beacon\_data\_pb2.cpython-39.pyc  
 ┃ ┃ ┣ 📜csse4011.cpython-39.pyc  
 ┃ ┃ ┗ 📜localisation\_manual.cpython-39.pyc  
 ┃ ┣ 📜3.1-8.4.csv  
 ┃ ┣ 📜4011.ui  
 ┃ ┣ 📜ASS.csv  
 ┃ ┣ 📜beacon\_data\_pb2.py  
 ┃ ┣ 📜csse4011.py  
 ┃ ┣ 📜decode2.py  
 ┃ ┣ 📜localisation\_manual.py  
 ┃ ┣ 📜mainGUI.py  
 ┃ ┣ 📜testsee.py  
 ┃ ┗ 📜upto3,0.csv  
 ┣ 📂lis2dh  
 ┃ ┣ 📂src  
 ┃ ┃ ┗ 📜main.c  
 ┃ ┣ 📜CMakeLists.txt  
 ┃ ┣ 📜prj.conf  
 ┃ ┣ 📜README.rst  
 ┃ ┗ 📜sample.yaml  
 ┣ 📂Mobile\_Node  
 ┃ ┣ 📂src  
 ┃ ┃ ┣ 📜main.c  
 ┃ ┣ 📜CMakeLists.txt  
 ┃ ┗ 📜prj.conf  
 ┣ 📂Particle\_Argon  
 ┃ ┣ 📂src  
 ┃ ┃ ┗ 📜main.c  
 ┃ ┣ 📜CMakeLists.txt  
 ┃ ┣ 📜particle\_argon.overlay  
 ┃ ┗ 📜prj.conf  
 ┣ 📂Python  
 ┃ ┣ 📂\_\_pycache\_\_  
 ┃ ┃ ┣ 📜beacon\_data\_pb2.cpython-39.pyc  
 ┃ ┃ ┣ 📜csse4011.cpython-39.pyc  
 ┃ ┃ ┣ 📜kalman.cpython-39.pyc  
 ┃ ┃ ┣ 📜knn\_util.cpython-39.pyc  
 ┃ ┃ ┣ 📜localisation\_manual.cpython-39.pyc  
 ┃ ┃ ┗ 📜tagoIO\_util.cpython-39.pyc  
 ┃ ┣ 📜3.1-8.4.csv  
 ┃ ┣ 📜4011.ui  
 ┃ ┣ 📜ASS.csv  
 ┃ ┣ 📜beacon\_data\_pb2.py  
 ┃ ┣ 📜collect\_data.py  
 ┃ ┣ 📜csse4011.py  
 ┃ ┣ 📜kalman.py  
 ┃ ┣ 📜knn\_util.py  
 ┃ ┣ 📜localisation\_manual.py  
 ┃ ┣ 📜mainGUI.py  
 ┃ ┣ 📜tagoIO\_util.py  
 ┃ ┣ 📜test\_abstraction.py  
 ┃ ┣ 📜test\_data\_collector.py  
 ┃ ┣ 📜test\_knn.py  
 ┃ ┣ 📜TrainingData.xlsx  
 ┃ ┗ 📜upto3,0.csv  
 ┣ 📂scan\_adv  
 ┃ ┣ 📂boards  
 ┃ ┃ ┗ 📜rv32m1\_vega\_ri5cy.overlay  
 ┃ ┣ 📂src  
 ┃ ┃ ┗ 📜main.c  
 ┃ ┣ 📜CMakeLists.txt  
 ┃ ┣ 📜prj.conf  
 ┃ ┣ 📜README.rst  
 ┃ ┗ 📜sample.yaml  
 ┗ 📜README.md  

## HCI Hardware

| Pins | Function | Board |
| -----| -------- | ----- |
| 11   | MISO     | PA    |
| 12   | MOSI     | PA    |
| 13   | SCK      | PA    |
| 14   | CS       | PA    |
| D11  | MOSI     | STM32 |
| D12  | MISO     | STM32 |
| D13  | SCK      | STM32 |
| D9   | CS       | STM32 |  
| D7   | ECHO     | STM32 |
| D8   | TRIG     | STM32 |

## References/Libraries

### Python Libaries

scipy  
numpy  
tago  
datetime  
sklearn  
binascii  
pyqtgraph  
PyQt5  

### Other Examples

kalman.py from blackboard  
<https://github.com/glucee/Multilateration> - Inspiration for our approach to multilateration.  
