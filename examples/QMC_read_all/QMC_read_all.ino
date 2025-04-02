#include <Wire.h>
#include "QMC.h"

#define radToDeg 57.2957795


QMC mag; // declare magnetometer device


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  mag.begin(13, ODR_100HZ);
  double errX, errY, errZ;
  //mag.get_offsets(errX, errY, errZ);
  //errX = 0.13;
  //errY = 0.04;
  //errZ = -0.06;
  errX = 0.03;
  errY = 0.11;
  errZ = -0.10;
  Serial.println("Offsets:");
  Serial.print(errX); Serial.print('\t'); Serial.print(errY); Serial.print('\t'); Serial.println(errZ);
  
  /*
  Wire.begin(); Wire.setClock(400000);// begin the i2c device communication at high speed 400khz

  delay(100);

  // need to write to the control register to put the device into continous reading mode
  // control register 0x09
  // OSR 128 - 10
  // RNG 2g - 00
  // ODR 10hz - 00
  // MODE continous - 01
  // 1000 0001
  // 0x81 value to write the control register 0x09 address

  Wire.beginTransmission(13);
  Wire.write(byte(0x09)); // send the address of the control register
  Wire.write(byte(0x81)); // value to write to the control register
  Wire.endTransmission();

  Wire.beginTransmission(13);
  Wire.write(byte(0x0B)); // address of the set reset period reg
  Wire.write(byte(0x01)); //
  Wire.endTransmission();

  delay(200);
  
  Wire.beginTransmission(13);
  Wire.write(byte(0x07));
  Wire.endTransmission(false);

  Wire.requestFrom(13, 2);
  Serial.print(Wire.available()); Serial.print('\t');
  byte data1 = Wire.read();
  byte data2 = Wire.read();

  Serial.print("0x"); Serial.print(data1, HEX); Serial.print(" 0x"); Serial.println(data2, HEX);
  

  Wire.beginTransmission(13);
  Wire.write(byte(0x09));
  Wire.endTransmission(false);

  Wire.requestFrom(13, 1);
  byte control = Wire.read();


  Serial.println(control, HEX);
  */

  mag.getMagData();
  Serial.println(mag.magX); 
  


}

void loop() {
  // put your main code here, to run repeatedly:
  mag.getMagData();
  double magnitude = sqrt(mag.magX * mag.magX + mag.magY * mag.magY + mag.magZ * mag.magZ);
  double normX = mag.magX / magnitude;
  double normY = mag.magY / magnitude;
  double normZ = mag.magZ / magnitude;

  float heading = atan2(normX, normY) * radToDeg;
  Serial.print(mag.magX); Serial.print('\t'); Serial.print(mag.magY); Serial.print('\t'); Serial.print(mag.magZ); Serial.print('\t'); Serial.println(heading);
  delay(10); // update frequency of 100hz
}

