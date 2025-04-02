#include <Wire.h>

enum modes {MAG_SCALE_2G = 0b00, MAG_SCALE_8G = 0b01, 
    ODR_10HZ = 0b00, ODR_50HZ = 0b01, ODR_100HZ = 0b10, ODR_200HZ = 0b11, 
    OSR_512 = 0b00, OSR_256 = 0b01, OSR_128 = 0b10, OSR_64 = 0b10,
    MODE_STDBY = 0b00, MODE_CONT = 0b01, MODE_DEFAULT = 0x81};

class QMC
{
  private:
    short int dev_adr;
    double offsetX, offsetY, offsetZ;
    int sensitivity;
    int freqInv;
    
  public:
    QMC(); // default constructor
    
    bool begin(short int address = 13,  char odr = ODR_10HZ, char scale = MAG_SCALE_2G, char osr = OSR_128, char mode = MODE_CONT);

    void get_offsets(double& errorX, double& errorY, double& errorZ);
    
    void apply_offsets(double& errorX, double& errorY, double& errorZ);

    short int getMagData();
    
    double magX;
    double magY;
    double magZ;
};