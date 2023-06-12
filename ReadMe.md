[![Release Status](https://img.shields.io/github/release/homewsn/candleLight_fw-SavvyCAN-Windows-plugin.svg)](https://github.com/homewsn/candleLight_fw-SavvyCAN-Windows-plugin/releases)

### Qt5 plugin for the candleLight firmware compatible USB devices.

This project contains the source code for the Qt5 serial bus module plug-in for interfacing the Windows version of [SavvyCAN](https://github.com/collin80/SavvyCAN) with USB devices flashed with [candleLight_fw](https://github.com/candle-usb/candleLight_fw).<br>

#### Building (Windows only):
* Download [the latest stable version of Qt5](https://www.qt.io/download/), for example 5.15.2 (you need 5.14.x or later).
* Open the project file.
* Select the kit that matches your version of SavvyCAN. For example, if you are using the [SavvyCAN-Windows_x64_CIBuild.zip V213](https://github.com/collin80/SavvyCAN/releases/tag/V213), it will be Desktop 5.15.2 MSVC2019 64bit. Then choose a Release build configuration.
* Build the project.
* Copy the built qtcandlelightcanbus.dll to the canbus folder of your SavvyCAN installation.

#### Usage:
* Connect your device or devices to Windows via USB.
* The connected USB devices can be found in the New Connection dialog, Connection Type is QT SerialBus devices, SerialBus Device Type is candleLight_fw, Port is gs_usb0 etc.

Ready plug-in for [SavvyCAN-Windows_x64_CIBuild.zip V213](https://github.com/collin80/SavvyCAN/releases/tag/V213) can be downloaded from [Releases](https://github.com/homewsn/candleLight_fw-SavvyCAN-Windows-plugin/releases).