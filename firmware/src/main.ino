/*
 * Original code is
 *
 */

/*----------------------------------------------
 *  M5Atom Pulse Serial Recieve Sample (M5ATOM Lite)
 *    #2020/03/27 - #2020/04/15
 *    
 *    Board Select -> M5StackC
 *    Upload Speed -> 500000
 *    writeing tool -> arduinoISP
  -----------------------------------------------*/
#include <M5Atom.h>
//#include <WiFi.h>
#include "BluetoothSerial.h"

#define DELIMITCODE  0x0a       // Delimit Code 

static byte          swmode;          // 0:LED+Send    1:LED   2:Send
static unsigned long offtime;

BluetoothSerial blespp;

void setup() {
    M5.begin(true,false,true);
//    M5.begin();
//--- 省電力でWifiOFFする場合に下記入れる
//    WiFi.mode(WIFI_OFF);
    blespp.begin("M5ATOM-Lite-Pulse");      //PC側で確認するときのBluetooth名前
//--
    M5.dis.drawpix(0, 0xf00000);      // GREEN
    delay(1000);
    M5.dis.drawpix(0, 0x00f000);      // RED
    delay(1000);
    M5.dis.drawpix(0, 0x0000f0);      // BLUE
    delay(1000);
    M5.dis.drawpix(0, 0x000000);
//    Serial2.begin(19200, SERIAL_8N1, 19, 22);         // RX=19   TX=22
    Serial2.begin(19200, SERIAL_8N1, 22, 19);         // RX=22   TX=19
    Serial2.print("@OF30");           // Offset
    Serial2.write(0x0a);
    Serial2.print("@RG2");            // MAG=2
    Serial2.write(0x0a);
    Serial2.print("@MD11");           // Mode 
    Serial2.write(0x0a);
   
    offtime = 0;
    
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
        switch(swmode){
          case 0:
          case 1:
            plsrate = 60000/val;                     // Interval(ms)を1分間の脈拍数(bpm)に変換
            if (plsrate<=70){
              M5.dis.drawpix(0, 0x0000f0);      // BLUE
            }else{
              if (plsrate<=90){
                M5.dis.drawpix(0, 0xf0f000);    // YELLOW
              }else{
                M5.dis.drawpix(0, 0x00f000);    // RED
              }
            }
            break;
        }
        offtime = millis()+250;
      }
    }else{                
  // 脈波形データ処理
      val = strInput.toInt();       
      switch(swmode){
        case 0:
        case 2:
          blespp.println(val);      
          break;
      }
    }
  }
//--
  if (offtime!=0){
    if (millis()>=offtime){
      M5.dis.drawpix(0, 0x000000);
      offtime = 0;
    }
  }
//--
  M5.update();
  if (M5.Btn.wasPressed()){
    swmode++;          // 0:LED+Send    1:LED   2:Send
    if (swmode>2) swmode=0;
  }
}

  
