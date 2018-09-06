/*
CODE TO CHECK BOT FORWARD ONLY
MAGNETO DATA HAS BEEM SIMULATED
NO NNED TO CONNECT MAGNETOMETER
*/
#include <SRA16.h>
#include <Wire.h>

float dest=27,init_angle=0,prevDegrees=0,True_degrees=0,headingDegrees=0;
int x,a[2],k = 0,sum_dist = 0,init_dist = 0, calib = 1 , calibRound = 0, diff = 0;
int dest_lidar = 100 ; // from bluetooth user input

void setup() {
  // put your setup code here, to run once:
  DDRB |= 0b00000011;
  port_init();          
  pwm1_init();
  bot_motion_init();   
  set_pwm1a(260);
  set_pwm1b(260);
  bot_stop();
  Serial.begin(115200);
}

void loop() 
{
  headingDegrees++;
  
  switch(k)
  {
    
    case 0:  if(abs(prevDegrees-headingDegrees)<15)
                {
                  True_degrees=headingDegrees;
                  prevDegrees=headingDegrees;
                }
              Serial.println("case 11");
              Serial.println("init_angle");
             
              if(dest+init_angle>=360)                  
              {                    
                if(True_degrees>dest+init_angle-360)
                {
                  bot_spot_left();
                  Serial.println("bot spot left");
                }
                else
                {
                  bot_stop();
                  Serial.println("left bot stop");
                  k++;  
                }
              }
              else
              {
                if(True_degrees< dest+init_angle)
                {
                  bot_spot_right();
                  Serial.println("bot spot right");
                }     
                else
                {
                  bot_stop();
                  Serial.println("right bot stop");
                  k++;
                }
              }
              break;
  
    case 1:  Serial.println("case 12");
              while(Serial.available()>=3)            // distance control using lidar
              {
                if(Serial.read() == 0)
                {
                  for(int i=0;i<2;i++)
                    a[i]=Serial.read();
                  x=100*a[1]+a[0];
                  Serial.println(x);
                  if (x<=1200)
                  {
                    //PORTB = 0b00000010;
                    if (calib==1)
                    {
                      calibRound++;
                      sum_dist+= x;
                      //Serial.println(x);
                      
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
                        k++;
                      }
                    }
                  }
                  else
                  {
                    //PORTB = 0b00000001;
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
