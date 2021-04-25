/*
 * Original code is from noruLab https://www.switch-science.com/catalog/6339/
 * 
 * modified by sktometometo
 */

#include <M5Atom.h>
#include "BluetoothSerial.h"
#include "ros/node_handle.h"
#include <std_msgs/Int64.h>

//#define USE_BLUETOOTH

#ifdef USE_BLUETOOTH
// for bluetooth serial
#include "BluetoothHardware.h"
ros::NodeHandle_<ESP32BluetoothHardware, 25, 25, 4096, 4096> nh;
#else
// for usb serial
#include "ArduinoHardware.h"
ros::NodeHandle_<ArduinoHardware, 25, 25, 4096, 4096> nh;
BluetoothSerial bluetooth_serial;
#endif


#define DELIMITCODE  0x0a       // Delimit Code 

static unsigned long offtime;

std_msgs::Int64 msg_data;
ros::Publisher publisher_data("~data", &msg_data);


void setup() {
    M5.begin(true,false,true);
    M5.dis.drawpix(0, 0x00f000);      // RED
    Serial2.begin(19200, SERIAL_8N1, 22, 19);         // RX=22   TX=19
    Serial2.print("@OF30");           // Offset
    Serial2.write(0x0a);
    Serial2.print("@RG2");            // MAG=2
    Serial2.write(0x0a);
    Serial2.print("@MD11");           // Mode 
    Serial2.write(0x0a);
    offtime = 0;

    // ros initialization
#ifdef USE_BLUETOOTH
    nh.initNode("PULSE10 ROS");
    Serial.begin(57600);
#else
    nh.initNode();
    bluetooth_serial.begin("PULSE10");
#endif
    nh.advertise(publisher_data);
    while(not nh.connected() ){
        nh.spinOnce();
        M5.dis.drawpix(0, 0x0000f0);      // BLUE
        delay(1000);
        M5.dis.drawpix(0, 0x000000);
        delay(1000);
    }

    M5.dis.drawpix(0, 0xf00000);      // GREEN
}

void publishData(int value) {
    msg_data.data = value;
    publisher_data.publish(&msg_data);
#ifdef USE_BLUETOOTH
    Serial.println(value);
#else
    bluetooth_serial.println(value);
#endif
}


void loop() {
    String   strInput;
    int      val;
    int      plsrate;

    delay(100);
    nh.spinOnce();
    while(not nh.connected() ){
        nh.spinOnce();
        delay(100);
        M5.dis.drawpix(0, 0x0000f0);      // BLUE
        delay(1000);
        M5.dis.drawpix(0, 0x000000);
        delay(1000);
    }
    M5.dis.drawpix(0, 0xf00000);      // GREEN

    if(Serial2.available() > 0) {
        strInput = Serial2.readStringUntil(DELIMITCODE);
        if (strInput[0]=='#'){
        // 脈拍数データ識別
            strInput[0]=' ';        // @をSpaceに置き換える
            if (strInput[1]=='-'){
              strInput[1]=' ';      // マイナスはErrorマークなのでSpaceに置き換えて色を赤にする
            }else{
                val = strInput.toInt();
                offtime = millis()+250;
            }
        }else{
        // 脈波形データ処理
            val = strInput.toInt();
            publishData(val);
        }
    }

    if (offtime!=0){
        if (millis()>=offtime){
          M5.dis.drawpix(0, 0x000000);
          offtime = 0;
        }
    }

    M5.update();
}
