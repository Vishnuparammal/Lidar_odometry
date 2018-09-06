/*
UART DATA TRANSFER
4 DIGIT NUMBER
BROKEN IN 2 TWO DIGITS
*/
#include<SoftwareSerial.h>// soft serial port header file
SoftwareSerial Serial3(0,2); // D3 as RX, and D4 as TX
uint16_t x=30;
int a,b,c;


void setup() 
{
  
 Serial3.begin(115200);

}

void loop() 
{

 a=x%100;
 b=x/100;
 
 Serial3.write(a);
 delay(15);
 Serial3.write(b);
 
}
