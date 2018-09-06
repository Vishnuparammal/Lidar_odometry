#ifndef __QMC5883L_H__
#define __QMC5883L_H__

#include <Arduino.h>
#include <Wire.h>



#define QMC5883L_ADDRESS 0x1A>>1

#define DATA_REGISTER_BEGIN 0x00
#define CONTROL_REGISTER 0x09
#define SET_RESET_REGISTER 0x0B

#define MOD_CONTINUOUS 0x01  //Mode Control, Set Continuous Measuring Mode
#define ODR_200HZ 0x03<<2    //Set Output Data Rate
#define RNG_2G 0x00<<4       //Set Scale as 2G


struct MagnetometerRaw
{
	int XAxis;
	int YAxis;
	int ZAxis;
};

class QMC5883L
{
	public:
	  MagnetometerRaw readRawAxis();
  
	  int setReg(uint8_t reg,uint8_t mode);

	protected:
	  void write(int address, int byte);
	  uint8_t* read(int address, int length);

};

#endif