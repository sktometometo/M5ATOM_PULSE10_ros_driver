# M5ATOM_PULSE10_ros_driver

This repository contains ros driver firmware for [noruLab's PULSE10 Pulse meter](https://www.switch-science.com/catalog/6339/).

## Requirements

Assumed devices are

- M5 ATOM Lite
- [PULSE10](https://www.switch-science.com/catalog/6339/)

And required development environment is

- Ubuntu 18.04 + ROS Melodic
- platformio

## How to build and burn

```bash
roscd M5ATOM_PULSE10_ros_driver
cd firmware
cd lib
rosrun rosserial_arduino make_libraries.py .
cd ..
pio run -t upload --upload-port <serial port to M5 ATOM Lite>
```

## How to use

TODO
