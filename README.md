# **unofone**

The Arduino-based phone with an Arduino uno at its core.

Makes use of the following:

* Arduino Uno SMD Rev 3
* SIM 800L IoT module
* 1602 LCD
* 4*4 Keypad



__Phone Features__

    FEATURE         KEYPAD BUTTON
    Receive Call        A
    Send SMS            B
    Make Call           C
    Read SMS            D


---
## **BUILDING THE PROJECT**
## **Requirements**

1.  PlatformIO IDE or PlatformIO Core
2.  VSCode
3.  Either of Windows 7 or later, macOS 10.14 or later, or Ubuntu 16.04 or later.

## **Instructions**

1.  Connect the circuit as per the provided schematic.
2.  Open a terminal in the root folder of the project.
3.  Linux users can run ```./build.sh && ./upload.sh ```, provided they use ```bash``` as their shell.
4. Open the project in VSCode. Ensure that the PlatformIO extension is installed.
5.  Build and upload the project to the Arduino board.
