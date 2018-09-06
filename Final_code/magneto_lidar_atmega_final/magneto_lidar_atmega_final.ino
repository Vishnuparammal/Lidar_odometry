/*
THE ULTIMATE FINAL WORKING CODE
BOT TURN AND FORWARD WITH GOOD ACCURACY
*/

#include<SRA16.h>
#include <QMC5883L.h>

QMC5883L compass;
float dest = 32, init_angle = 0, prevDegrees = 0, True_degrees = 0;
uint16_t dist= 300;// LiDAR actually measured distance value
uint16_t check;// check numerical value storage
int i=0,dest_lidar=100,init_dist=300,sum_dist,calib=1,calibRound,diff;
byte uart[9];// store data measured by LiDAR
byte HEADER=0x59;// data package frame header (check datasheet)

void setup() 
{
 Wire.begin();
 Serial.begin(115200);
 port_init();
 switch_init();
 pwm1_init();            //PWM pins of the atmega are initialized
 bot_motion_init();      //Bot motion is initialized
 bot_stop();
 set_pwm1a(259);
 set_pwm1b(261);
 compass.setReg(SET_RESET_REGISTER, 0x01);
  compass.setReg(CONTROL_REGISTER, MOD_CONTINUOUS | ODR_200HZ | RNG_2G);
}

void loop() 
{  
  MagnetometerRaw raw = compass.readRawAxis();
  float headingDegrees = atan2(raw.YAxis, raw.XAxis) * 180 / 3.14 + 180;
  if (i < 11)
  { i++;
  }
  switch (i)
  {
    case 10:  //PORTB = 0b00000000;
      
      init_angle = headingDegrees; // discard 10 readings and then calibrate
      prevDegrees = init_angle;

      
      break;

    case 11:  if (abs(prevDegrees - headingDegrees) < 15)
      {
        True_degrees = headingDegrees;
        prevDegrees = headingDegrees;
      }
      
      if (dest + init_angle >= 360)         // angle control using magnetometer
      {
        if (True_degrees > dest + init_angle - 360)
        {
          bot_spot_left();
//          Serial.println("bot spot left");
        }
        else
        {
          bot_stop();
//          Serial.println("left bot stop");
          i++;
          delay(5000);
        }
      }
      else
      {
        if (True_degrees < dest + init_angle)
        {
          bot_spot_right();
//          Serial.println("bot spot right");
        }
        else
        {
          bot_stop();
//          Serial.println("right bot stop");
          i++;
          delay(5000);
        }
      }
      break;
  case 12:
//  Serial.println("case 12");
   if (Serial.available())//check whether the serial port has data input
  {

            delay(1); // this delay stabilizes code
            uart[0] = Serial.read();
            
    if(uart[0]==HEADER)// determine data package frame header 0x59
    {
              uart[1] = Serial.read();
              
      if(uart[1]==HEADER)//determine data package frame header 0x59
      {
        for(int m=2;m<9;m++)// store data to array
        {
          uart[m]=Serial.read();
        }
      check=uart[0]+uart[1]+uart[2]+uart[3]+uart[4]+uart[5]+uart[6]+uart[7];
      
      if(uart[8]==(check&0xff))// check the received data as per protocols
      {
        dist = 0;
        dist = dist | uart[3];// higher bits
        dist = dist<<8;
        dist = dist | uart[2];// lower bits
      }

        if (dist <= 1200)
          {
//            Serial.println(dist);
            if (calib == 1)
            {
              calibRound++;
              sum_dist += dist;

              if (calibRound == 50)
              {
                init_dist = sum_dist / 50;
                diff = init_dist - dest_lidar;
           
                calib = 0;
              }
            }
            else
            { 
              if (dist > diff)
              {
//                Serial.println("forward");
              
                bot_forward();
   
              }
              else
              {
//                Serial.println("stop");
              
                bot_brake();
       
              }
            }
          }
       else
          {

            bot_brake();
          }
        
      

     break;
     default:  
//     Serial.println("default");
      bot_stop(); 
}
}
  }
  }
}
   
 

