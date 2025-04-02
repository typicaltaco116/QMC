#include "QMC.h"
#include <Arduino.h>

QMC::QMC(){ // default constructor
  
}

bool QMC::begin(short int address = 13,  char odr = ODR_10HZ, char scale = MAG_SCALE_2G, char osr = OSR_128, char mode = MODE_CONT){
  dev_adr = address; // set the private member 

  switch(scale){ // set the scale according to the datasheet
    case MAG_SCALE_2G:
      sensitivity = 12000;
      break;
    case MAG_SCALE_8G:
      sensitivity = 3000;
      break;
  }

  switch(odr){
    case ODR_10HZ:
      freqInv = 100; // 100 ms
      break;
    case ODR_50HZ:
      freqInv = 20; // 20 ms
      break;
    case ODR_100HZ:
      freqInv = 10;
      break;
    case ODR_200HZ:
      freqInv = 5;
      break;
  }
  
  char control_val = (osr << 6) | (scale << 4) | (odr << 2) | mode; // combine all binary values into one byte to be written

  Wire.begin();
  Wire.setClock(400000); // begin high speed communication with i2c device
  delay(100); // power on time
  
  // now we need to set up the control register and the set/period register
  Wire.beginTransmission(dev_adr);
  Wire.write(byte(0x09)); // address of the control register
  Wire.write(byte(control_val)); // write the value to the control register
  Wire.endTransmission(); // send the stop bit following the final slave acknowledge
  
  Wire.beginTransmission(dev_adr);
  Wire.write(byte(0x0B)); // address of the set/period register
  Wire.write(byte(0x01));
  Wire.endTransmission();
  
  delay(100); // give enough time to apply may or may not be necessary
  
  //ready to read data from registers
}

void QMC::get_offsets(double& errorX, double& errorY, double& errorZ){
  // basic idea is to find the average of values from the magnetometer

  errorX = 0.0;
  errorY = 0.0;
  errorZ = 0.0;

  int count = 0;
  while (count < 12000 / freqInv){ // average of 60 values
    QMC::getMagData();
    delay(freqInv); // update frequency
    
    errorX += magX;
    errorY += magY;
    errorZ += magZ;
    count++;
  }

  errorX = errorX / count;
  errorY = errorY / count;
  errorZ = errorZ / count;

  // apply the offsets to the private members of the class
  offsetX = errorX;
  offsetY = errorY;
  offsetZ = errorZ;


}

void QMC::apply_offsets(double& errorX, double& errorY, double& errorZ){
  offsetX = errorX;
  offsetY = errorY;
  offsetZ = errorZ;
}

short int QMC::getMagData(){
  Wire.beginTransmission(dev_adr);
  Wire.write(byte(0x00)); // address of the first mag data register X lsb register
  Wire.endTransmission(false); // don't send a stop bit because we want the communication to continue
  
  Wire.requestFrom(dev_adr, 6); // read all six registers at once the pointer should allow this
  short int data[3];
  for (int i = 0; i < 3; i++){
    short int lsb = Wire.read();
    short int msb = Wire.read() << 8; // shift left one byte
    data[i] = msb | lsb; // bitwise or to combine the values
  }
  
  // the requestFrom() should automatically send a stop bit after all 6 are read
  
  magX = double(data[0]) / double(sensitivity); // sensitivity is a private member set by the scale of the readings in QMC::begin()
  magY = double(data[1]) / double(sensitivity);
  magZ = double(data[2]) / double(sensitivity);

  magX -= offsetX; // offset all readings by the calibration amount
  magY -= offsetY;
  magZ -= offsetZ;
}