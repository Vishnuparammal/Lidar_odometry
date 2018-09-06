/*
CODE TO CHECK BOT FORWARD
*/

#include<SRA16.h>
uint16_t dist= 0;// LiDAR actually measured distance value
uint16_t check;// check numerical value storage
int i,dest_lidar=100,init_dist,sum_dist,calib=1,calibRound,diff;
byte uart[9];// store data measured by LiDAR
byte HEADER=0x59;// data package frame header (check datasheet)

void setup() 
{
 Serial.begin(115200);
 port_init();
 switch_init();
 pwm1_init();            //PWM pins of the atmega are initialized
 bot_motion_init();      //Bot motion is initialized
 bot_stop();
 set_pwm1a(256);
 set_pwm1b(261);
}

void loop() 
{
   if (Serial.available())//check whether the serial port has data input
  {

            delay(1); // this delay stabilizes code
            uart[0] = Serial.read();
            
    if(uart[0]==HEADER)// determine data package frame header 0x59
    {
              uart[1] = Serial.read();
              
      if(uart[1]==HEADER)//determine data package frame header 0x59
      {
        for(i=2;i<9;i++)// store data to array
        {
          uart[i]=Serial.read();
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
                bot_forward();
   
              }
              else
              {
                bot_brake();
       
              }
            }
          }
       else
          {

            bot_brake();
          }
        }
      }

     }
}

   
 

