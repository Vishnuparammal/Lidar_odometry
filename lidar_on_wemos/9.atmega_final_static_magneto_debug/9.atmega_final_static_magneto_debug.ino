/*
FINAL CODE WITH BOT TURN AND FORWARD
MAGNETOMETER FILTER INCLUDED
*/
#include <SRA16.h>
#include <Wire.h>
#include <QMC5883L.h>

QMC5883L compass;
float dest=25,init_angle=0,prevDegrees=0,True_degrees=0;
int x,a[2],sum_dist = 0,init_dist = 0, calib = 1 , calibRound = 0, diff = 0;
static int i = 0;
int dest_lidar = 100 ; // from bluetooth user input

void setup() {
  // put your setup code here, to run once:
  DDRB |= 0b00000011;
  port_init();          
  pwm1_init();
  bot_motion_init();   
  set_pwm1a(291);
  set_pwm1b(291);
  bot_stop();
  Serial.begin(115200);
  Wire.begin();
 
  compass.setReg(SET_RESET_REGISTER,0x01);
  compass.setReg(CONTROL_REGISTER,MOD_CONTINUOUS|ODR_200HZ|RNG_2G);
}

void loop() 
{
  MagnetometerRaw raw = compass.readRawAxis();
  float headingDegrees = atan2(raw.YAxis, raw.XAxis)*180/3.14+180;
  
  if(i<11)
  {i++;
  }
  switch(i)
  {
    case 10:  //PORTB = 0b00000000;
              Serial.println("case 10");
              init_angle=headingDegrees; // discard 10 readings and then calibrate
              prevDegrees=init_angle;
              
              Serial.println("init_angle");
              Serial.println(init_angle);
              break;
    
    case 11:  if(abs(prevDegrees-headingDegrees)<15)    // THIS IS MAGNETOMETER FILTER PART
                {
                  True_degrees=headingDegrees;
                  prevDegrees=headingDegrees;
                }
              Serial.println("case 11");
              Serial.println("init_angle");
              Serial.println(init_angle);
              Serial.println(True_degrees);
              if(dest+init_angle>=360)              // angle control using magnetometer                
              {                    
                if(True_degrees>dest+init_angle-360)
                {
                  bot_spot_right();
                  Serial.println("bot spot left");
                }
                else
                {
                  bot_stop();
                  Serial.println("left bot stop");
                  i++;  
                }
              }
              else
              {
                if(True_degrees< dest+init_angle)
                {
                  bot_spot_left();
                  Serial.println("bot spot right");
                }     
                else
                {
                  bot_stop();
                  Serial.println("right bot stop");
                  i++;
                }
              }
              break;
  
    case 12:  Serial.println("case 12");
              while(Serial.available()>=3)            // distance control using lidar
              {
                Serial.println("recieving data");
                if(Serial.read() == 0)
                {
                  for(int k=0;k<2;k++)
                    {a[k]=Serial.read();
                    }
                  x=100*a[1]+a[0];
                  Serial.println(x);
                  if (x<=1200)
                  {
                    PORTB = 0b00000010;
                    if (calib==1)
                    {
                      calibRound++;
                      sum_dist+= x;
                      Serial.println(x);
                      
                      if (calibRound == 50)
                      {
                        init_dist = sum_dist/50;
                        diff = init_dist - dest_lidar;
                        Serial.println(init_dist);
                        calib = 0;  
                      }
                    }
                    else
                    {
                      if(x>diff)
                      {
                        bot_forward();
                        Serial.println("Bot forward");
                      }
                      else
                      {
                        bot_stop();
                        Serial.println("Bot stop");
                        i++;
                      }
                    }
                  }
                  else
                  {
                    PORTB = 0b00000001;
                    bot_stop();
                  } 
                }
              }
              break;
    default:  Serial.println("default");
              bot_stop();                             // 2 led on indicates program ends
              //PORTB = 0b00000011;                        
  }
}


