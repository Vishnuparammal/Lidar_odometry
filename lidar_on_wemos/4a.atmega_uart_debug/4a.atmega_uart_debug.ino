/*
BOT FORWARD
RECIEVEING LIDAR DATA FROM WEMOS
UART PROBLEM SOLVED USING A CHECK NUMBER 0
*/
#include <SRA16.h>
int a[4],i,sum_dist = 0,init_dist = 0, calib = 1 , calibRound = 0, diff = 0;
int dest = 100 ; // from bluetooth user input
volatile long int x;


void setup() 
{
 
  DDRB |= 0b00000011;
  Serial.begin(115200);
  pwm1_init();            //PWM pins of the atmega are initialized
  bot_motion_init();      //Bot motion is initialized
  switch_init();          //Switch can be accessed
  bot_stop(); //Bot's direction is set to forward
  set_pwm1a(291);
  set_pwm1b(291);
  

}


void loop() 
{

 while (Serial.available() >= 3)         // distance control using lidar
      {
        Serial.println("recieving data");

        if (Serial.read() == 0)
        {
          for (int k = 0; k < 2; k++)
          { a[k] = Serial.read();
          }
          x = 100 * a[1] + a[0];

 if (x<=1200)
 {
  Serial.println("inside x<1200");
  if (calib==1)
     {
     Serial.println("inside calib==1");
     calibRound++;
     sum_dist+= x;
     if (calibRound == 50)
      {
        Serial.println("inside calibRound==50");
        init_dist = sum_dist/50;
        diff = init_dist - dest;
        calib = 0;  
      }
     }
  else
  {
    Serial.println("inside else");
    // motor code 
    
    Serial.println("init_dist");
    Serial.println(init_dist);
    Serial.println("Diff");
    Serial.println(diff);
    Serial.println("x");
    Serial.println(x);
    
    
    if(x>diff)
    {
      Serial.println("bot forward");
      bot_forward();
    }
    else
      {
        Serial.println("bot stop");
        bot_brake();
      }
  }
 }
 else
 {
  PORTB = 0b00000001;
  bot_brake();
 }
 
  }}}
