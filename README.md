# USB_Security
A C++ Application which helps you to protect you PC from unknown pendrives

## Overview

I utilized HWND of windows.h instead of any library for creating GUI for this application. Whenever a pendrive gets attached to the system, this app will detect it and shows a password screen which disables all other functions of the PC and you cannot close this window without typing correct password. You will get three chases to type password after which system will get halted and you have to restart and if app is added to startup it will automatically run on detection of pendrive

## Key Features
```
- Detect pendrives
- Has been added in system tray
- You can't close window without typing correct password
- Adjustable to any resolution without disturbing features of app
- System gets halted after three wrong tries
- Password is 'LightYagami'
```
