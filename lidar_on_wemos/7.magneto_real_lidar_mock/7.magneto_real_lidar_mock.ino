/*
CODE TO CHECK BOT TURN ONLY
LIDAR DATA HAS BEEN SIMULATED
NO NEED TO CONNECT LIDAR
*/
#include <SRA16.h>
#include <Wire.h>
#include <QMC5883L.h>

QMC5883L compass;
float dest=55,init_angle=0;
int x,a[2],i = 0,sum_dist = 0,init_dist = 0, calib = 1 , calibRound = 0, diff = 0,sim_dist = 1000;
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
  Wire.begin();
 
  compass.setReg(SET_RESET_REGISTER,0x01);
  compass.setReg(CONTROL_REGISTER,MOD_CONTINUOUS|ODR_200HZ|RNG_2G);
}

void loop() {

  
  MagnetometerRaw raw = compass.readRawAxis();
  float headingDegrees = atan2(raw.YAxis, raw.XAxis)*180/3.14+180;
  if(i<11)
  {i++;}
  switch(i)
  {
    case 10:  PORTB = 0b00000000;
              Serial.println("case 10");
              init_angle=headingDegrees;           // discard 10 readings and then calibrate
              Serial.println(init_angle);
              break;
    
    case 11:  
              Serial.println("case 11");
              Serial.println(headingDegrees);
              if(dest+init_angle>=360)              // angle control using magnetometer                
              {                    
                if(headingDegrees>dest+init_angle-360)
                {
                  bot_spot_left();
                  Serial.println("bot sopt left");
                }
                else
                {
                  bot_stop();
                  Serial.println("chutiya wala bot stop");
                  i++;  
                }
              }
              else
              {
                if(headingDegrees< dest+init_angle)
                {
                  bot_spot_right();
                  Serial.println("bot spot right");
                }     
                else
                {
                  bot_stop();
                  Serial.println("bot stop");
                  i++;
                }
              }
              break;
    case 12:  Serial.println("case 12");
              PORTB = 0b00000011;                     // 2 led on indicates lidar starts
              Serial.flush();                         // check it
              i++;
              break;
    case 13:  Serial.println("case 13");
              Sim_dist--;
              if(sim_dist>=dest_lidar)
              {
                Serial.println("bot forward");
                bot_forward();
              }
              else
              {
                Serial.println("bot stop");  
                bot_stop();
              }
//              while(Serial.available()>=3)            // distance control using lidar
//              {
//                if(Serial.read() == 0)
//                {
//                  for(i=0;i<2;i++)
//                    a[i]=Serial.read();
//                  x=100*a[1]+a[0];
//                  if (x<=1200)
//                  {
//                    PORTB = 0b00000010;
//                    if (calib==1)
//                    {
//                      calibRound++;
//                      sum_dist+= x;
//                      if (calibRound == 50)
//                      {
//                        init_dist = sum_dist/50;
//                        diff = init_dist - dest_lidar;
//                        calib = 0;  
//                      }
//                    }
//                    else
//                    {
//                      if(x>diff)
//                      {
//                        bot_forward();
//                      }
//                      else
//                      {
//                        bot_stop();
//                        i++;
//                      }
//                    }
//                  }
//                  else
//                  {
//                    PORTB = 0b00000001;
//                    bot_stop();
//                  } 
//                }
//              }
              break;
    default:  Serial.println("default");
              bot_stop();                             // 2 led on indicates program ends
              PORTB = 0b00000011;                        
  }
}


