#include <Arduino.h> 
#include "QMC5883L.h"

MagnetometerRaw QMC5883L::readRawAxis()
{
  uint8_t* buffer = read(DATA_REGISTER_BEGIN, 6);
  MagnetometerRaw raw = MagnetometerRaw();
  raw.XAxis = (buffer[1] << 8) | buffer[0];
  raw.YAxis = (buffer[3] << 8) | buffer[2];
  raw.ZAxis = (buffer[5] << 8) | buffer[4];
  return raw;
}


int QMC5883L::setReg(uint8_t reg,uint8_t mode)
{
	write(reg, mode);
}

void QMC5883L::write(int address, int data)
{
  Wire.beginTransmission(QMC5883L_ADDRESS);
  Wire.write(address);
  Wire.write(data);
  Wire.endTransmission();
}

uint8_t* QMC5883L::read(int address, int length)
{
  Wire.beginTransmission(QMC5883L_ADDRESS);
  Wire.write(address);
  Wire.endTransmission();
  
  Wire.beginTransmission(QMC5883L_ADDRESS);
  Wire.requestFrom(QMC5883L_ADDRESS, length);

  uint8_t buffer[length];
  if(Wire.available() == length)
  {
	  for(uint8_t i = 0; i < length; i++)
	  {
		  buffer[i] = Wire.read();
	  }
  }
  Wire.endTransmission();

  return buffer;
}

