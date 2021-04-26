/*
 * Original code is from noruLab https://www.switch-science.com/catalog/6339/
 * 
 * modified by sktometometo
 */

#include <M5Atom.h>
#include "BluetoothSerial.h"
#include "ros/node_handle.h"
#include <std_msgs/Int32.h>

// for usb serial
#include "ArduinoHardware.h"
ros::NodeHandle_<ArduinoHardware, 25, 25, 4096, 4096> nh;


#define DELIMITCODE  0x0a       // Delimit Code 

static unsigned long offtime;

std_msgs::Int32 msg_data;
std_msgs::Int32 msg_pulse_rate;
ros::Publisher publisher_data("~data", &msg_data);
ros::Publisher publisher_pulse_rate("~pulse_rate", &msg_pulse_rate);


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
    nh.initNode();
    nh.advertise(publisher_data);
    //nh.advertise(publisher_pulse_rate);
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
}

void publishPulseRate(int value) {
    msg_pulse_rate.data = value;
    publisher_pulse_rate.publish(&msg_pulse_rate);
}

void loop() {
    delay(10);
    nh.spinOnce();
    while(not nh.connected()){
        nh.spinOnce();
        delay(100);
        M5.dis.drawpix(0, 0x0000f0);      // BLUE
        delay(1000);
        M5.dis.drawpix(0, 0x000000);
        delay(1000);
    }
    M5.dis.drawpix(0, 0xf00000);      // GREEN

    if(Serial2.available() > 0) {
        String strInput = Serial2.readStringUntil(DELIMITCODE);
        if (strInput[0]=='#'){
            // pulse rate data
            //strInput[0]=' ';        // @をSpaceに置き換える
            //if (strInput[1]=='-'){
            //    // Error
            //}else{
            //    int val = strInput.toInt();
            //    publishPulseRate(val);
            //}
        }else{
            // pulse wave data
            int val = strInput.toInt();
            if ( val <= 5000 and val >= 0 ) {
                publishData(val);
            }
        }
    }

    M5.update();
}
