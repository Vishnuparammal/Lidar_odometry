
// Reference the I2C Library
#include <Wire.h>
// Reference the QMC5883L Compass Library
#include <QMC5883L.h>

// Store our compass as a variable.
QMC5883L compass;

// Out setup routine, here we will configure the microcontroller and compass.
void setup()
{
  // Initialize the serial port.
  Serial.begin(9600);

  Serial.println("Starting the I2C interface.");
  Wire.begin(); // Start the I2C interface.
 
  compass.setReg(SET_RESET_REGISTER,0x01);
 
  Serial.println("Setting measurement mode to Continous_200HZ_2G.");
  compass.setReg(CONTROL_REGISTER,MOD_CONTINUOUS|ODR_200HZ|RNG_2G); // Set the measurement mode to Continuous,Output Data Rate 200HZ and Scale Range as 2G
}

// Our main program loop.
void loop()
{
  // Retrive the raw values from the compass (not scaled).
  MagnetometerRaw raw = compass.readRawAxis();
  
   // Calculate heading when the magnetometer is level.
  float headingDegrees = atan2(raw.YAxis, raw.XAxis)*180/3.14+180;;
  
  // Output the data via the serial port.
  Output(headingDegrees);

  delay(66);//of course it can be delayed longer.
}

// Output the data down the serial port.
void Output(float headingDegrees)
{
   //Serial.print("Heading:");
   Serial.print(headingDegrees);
   Serial.println(" Degrees");
}
