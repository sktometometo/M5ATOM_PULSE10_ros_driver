/*
 * Original code is from noruLab https://www.switch-science.com/catalog/6339/
 * 
 * modified by sktometometo
 */

#include <M5Atom.h>
//#include "ArduinoHardware.h"
#include "BluetoothSerial.h"
#include "BluetoothHardware.h"
#include "ros/node_handle.h"
#include <std_msgs/Int64.h>

#define DELIMITCODE  0x0a       // Delimit Code 

static unsigned long offtime;

ros::NodeHandle_<ESP32BluetoothHardware, 25, 25, 4096, 4096> nh;
//ros::NodeHandle_<ArduinoHardware, 25, 25, 4096, 4096> nh;
std_msgs::Int64 msg_data;
ros::Publisher publisher_data("~data", &msg_data);


void setup() {
    M5.begin(true,false,true);
    M5.dis.drawpix(0, 0xf00000);      // GREEN
    delay(1000);
    M5.dis.drawpix(0, 0x00f000);      // RED
    delay(1000);
    M5.dis.drawpix(0, 0x0000f0);      // BLUE
    delay(1000);
    M5.dis.drawpix(0, 0x000000);
    Serial2.begin(19200, SERIAL_8N1, 22, 19);         // RX=22   TX=19
    Serial2.print("@OF30");           // Offset
    Serial2.write(0x0a);
    Serial2.print("@RG2");            // MAG=2
    Serial2.write(0x0a);
    Serial2.print("@MD11");           // Mode 
    Serial2.write(0x0a);
    offtime = 0;

    // ros initialization
    nh.initNode("PULSE10 ROS");
    nh.advertise(publisher_data);
    while(not nh.connected() ){
        nh.spinOnce();
    }
}

void publishData(int value) {
    msg_data.data = value;
    publisher_data.publish(&msg_data);
}


void loop() {
    String   strInput;
    int      val;
    int      plsrate;

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
