#include <Wire.h>
#include <SparkFun_APDS9960.h>
#include <SoftwareSerial.h>

//Bluetooth
#define BT_RXD 8
#define BT_TXD 7
#define buttonPin 2
SoftwareSerial bluetooth(BT_RXD, BT_TXD); // 블루투스 설정 BTSerial(Tx, Rx)

//Gesture Sensor
#define APDS9960_INT    2 // Needs to be an interrupt pin
SparkFun_APDS9960 apds = SparkFun_APDS9960();
int isr_flag = 0;

void setup() {
  // Set interrupt pin as input
  pinMode(APDS9960_INT, INPUT);

  // Initialize Serial port
  Serial.begin(9600);
  Serial.println();
  Serial.println(F("--------------------------------"));
  Serial.println(F("SparkFun APDS-9960 - GestureTest"));
  Serial.println(F("--------------------------------"));
  
  // Initialize interrupt service routine
  attachInterrupt(0, interruptRoutine, FALLING);

  // Initialize APDS-9960 (configure I2C and initial values)
  if ( apds.init() ) {
    Serial.println(F("APDS-9960 initialization complete"));
  } else {
    Serial.println(F("Something went wrong during APDS-9960 init!"));
  }
  
  // Start running the APDS-9960 gesture sensor engine
  if ( apds.enableGestureSensor(true) ) {
    Serial.println(F("Gesture sensor is now running"));
  } else {
    Serial.println(F("Something went wrong during gesture sensor init!"));
  }
  bluetooth.begin(9600);// 블루투스 통신 시작
}

void loop() {
  if( isr_flag == 1 ) {
    detachInterrupt(0);
    handleGesture();
    isr_flag = 0;
    attachInterrupt(0, interruptRoutine, FALLING);
  }
}

void interruptRoutine() {
  isr_flag = 1;
}

void handleGesture() {
    if ( apds.isGestureAvailable() ) {
    switch ( apds.readGesture() ) {
      case DIR_UP:
        bluetooth.write("U");
        break;
      case DIR_DOWN:
        bluetooth.write("D");
        break;
      case DIR_LEFT:
        bluetooth.write("L");
        break;
      case DIR_RIGHT:
        bluetooth.write("R");
        break;
      case DIR_NEAR:
        bluetooth.write("o");
        break;
      case DIR_FAR:
        bluetooth.write("o");
        break;
    }
  }
}
