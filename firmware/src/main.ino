/*
 * Original code is from noruLab https://www.switch-science.com/catalog/6339/
 * 
 * modified by sktometometo
 */

#include <M5Atom.h>

#include "ros/node_handle.h"
#include "BluetoothHardware.h"
#include <std_msgs/Int32.h>


#define DELIMITCODE  0x0a       // Delimit Code 

static unsigned long offtime;

ros::NodeHandle_<BluetoothHardware, 25, 25, 4096, 4096> nh;

std_msgs::Int32 msg_data;
std_msgs::Int32 msg_pulse_rate;
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
    nh.initNode("M5Atom Pulse Sensor ROS");
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
