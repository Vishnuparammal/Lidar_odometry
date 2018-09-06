/*
BOT FORWARD
RECIEVEING DATA FROM WEMOS
PROBLEM IN UART
PRESS RESET IN WEMOS IF WRONG VALUES OBTAINED
*/
#include <SRA16.h>
int x,a[4],i,sum_dist = 0,init_dist = 0, calib = 1 , calibRound = 0, diff = 0;
int dest = 100 ; // from bluetooth user input
#define led0 REGISTER_BIT(PORTB,0);
#define led1 REGISTER_BIT(PORTB,1);

void setup() 
{
 
  DDRB |= 0b00000011;
  Serial.begin(115200);
  pwm1_init();            //PWM pins of the atmega are initialized
  bot_motion_init();      //Bot motion is initialized
  switch_init();          //Switch can be accessed
  bot_stop(); //Bot's direction is set to forward
  set_pwm1a(399);
  set_pwm1b(399);
  

}


void loop() 
{
  
  while(Serial.available()>=2)
 {
  for(i=0;i<2;i++)
  {
    a[i]=Serial.read();
  }
//  if(a[1]==0)
//  {
//    x=a[0];
//  }
  //else
  //{

   x=100*a[0]+a[1];
   Serial.println(x);
   
 //}

 if (x<=1200)
 {
  PORTB = 0b00000010;
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
    if(x>diff)
    {
      Serial.println("bot forward");
      bot_forward();
    }
    else
      {
        Serial.println("bot stop");
        bot_stop();
      }
  }
 }
 else
 {
  PORTB = 0b00000001;
 }
 
}
}
