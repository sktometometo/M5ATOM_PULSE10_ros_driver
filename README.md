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

First, please pair and connect your device

Scan your device
```bash
~$ sudo hcitool scan
Scanning ...
        94:B9:7E:8B:77:46       PULSE10 ROS
```

Check connection to your device
```bash
~ $ sudo l2ping -c 10 94:B9:7E:8B:77:46
Ping: 94:B9:7E:8B:77:46 from 04:ED:33:EE:D8:A0 (data size 44) ...
0 bytes from 94:B9:7E:8B:77:46 id 0 time 5.89ms
0 bytes from 94:B9:7E:8B:77:46 id 1 time 5.80ms
0 bytes from 94:B9:7E:8B:77:46 id 2 time 5.42ms
0 bytes from 94:B9:7E:8B:77:46 id 3 time 5.86ms
0 bytes from 94:B9:7E:8B:77:46 id 4 time 5.60ms
0 bytes from 94:B9:7E:8B:77:46 id 5 time 6.76ms
0 bytes from 94:B9:7E:8B:77:46 id 6 time 7.56ms
0 bytes from 94:B9:7E:8B:77:46 id 7 time 5.65ms
0 bytes from 94:B9:7E:8B:77:46 id 8 time 6.65ms
0 bytes from 94:B9:7E:8B:77:46 id 9 time 8.46ms
10 sent, 10 received, 0% loss
```

Check information about the device
```bash
~ $ sudo hcitool info 94:B9:7E:8B:77:46
Requesting information ...
        BD Address:  94:B9:7E:8B:77:46
        Device Name: PULSE10 ROS
        LMP Version: 4.2 (0x8) LMP Subversion: 0x30e
        Manufacturer: RivieraWaves S.A.S (96)
        Features page 0: 0xbf 0xee 0xcd 0xfe 0xdb 0xff 0x7b 0x87
                <3-slot packets> <5-slot packets> <encryption> <slot offset>
                <timing accuracy> <role switch> <sniff mode> <RSSI>
                <channel quality> <SCO link> <HV3 packets> <u-law log>
                <A-law log> <CVSD> <power control> <transparent SCO>
                <broadcast encrypt> <EDR ACL 2 Mbps> <EDR ACL 3 Mbps>
                <enhanced iscan> <interlaced iscan> <interlaced pscan>
                <inquiry with RSSI> <extended SCO> <EV4 packets> <EV5 packets>
                <AFH cap. slave> <AFH class. slave> <LE support>
                <3-slot EDR ACL> <5-slot EDR ACL> <sniff subrating>
                <pause encryption> <AFH cap. master> <AFH class. master>
                <EDR eSCO 2 Mbps> <EDR eSCO 3 Mbps> <3-slot EDR eSCO>
                <extended inquiry> <LE and BR/EDR> <simple pairing>
                <encapsulated PDU> <err. data report> <non-flush flag> <LSTO>
                <inquiry TX power> <EPC> <extended features>
        Features page 1: 0x07 0x00 0x00 0x00 0x00 0x00 0x00 0x00
        Features page 2: 0x5f 0x03 0x00 0x00 0x00 0x00 0x00 0x00
```

pair and connect the device

```bash
~$ bluetoothctl
# scan on
# scan off
# pair <MAC address>
# trust <MAC address>
# connect <MAC address>
# info <MAC address>
```
