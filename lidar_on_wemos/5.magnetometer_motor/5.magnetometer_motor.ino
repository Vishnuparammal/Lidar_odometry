/*
BOT TURN USING MAGNETOMETER
*/
#include <SRA16.h>

// Reference the I2C Library
#include <Wire.h>
// Reference the QMC5883L Compass Library
#include <QMC5883L.h>

// Store our compass as a variable.
QMC5883L compass;
float True_degree=0,dest=20,init_angle=0,i=0;

// Out setup routine, here we will configure the microcontroller and compass.
void setup()
{
  port_init();          //Initialization of all ports
  bot_motion_init();    //Initialization of the motor pins as output pins
  switch_init();        //Initialization of switch D3 D2 D1 D0 as input
  set_pwm1a(291);
  set_pwm1b(291);
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
  if(i==10)
  {
    init_angle=headingDegrees;
    Serial.println("init_angle");
    Serial.println(init_angle);
  }
  Serial.println(i);
  i++;
  
  // Output the data via the serial port.
  Output(headingDegrees);
  if(dest+init_angle>=360)
    {
       
        if(headingDegrees>dest+init_angle-360)
        {
           Serial.println("inside bot spot left");
           bot_spot_right();
//            bot_forward/();
        }
        else
        {
            Serial.print("chutiya wala bot stop");
            bot_stop();  
        }
    }
    else
    {
        if(headingDegrees< dest+init_angle)
        {
          Serial.println("bot spot right");
          bot_spot_left();
//          bot_forwa/rd();
        }
         
        else
        {
            Serial.println("bot stop");
            bot_stop();
        }

}

  delay(66);//of course it can be delayed longer.
}

// Output the data down the serial port.
void Output(float headingDegrees)
{
   //Serial.print("Heading:");
   Serial.println(" Degrees");
   Serial.print(headingDegrees);
   
}
 
     
     
    
