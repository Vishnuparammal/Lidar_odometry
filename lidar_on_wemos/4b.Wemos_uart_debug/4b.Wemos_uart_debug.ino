/*
LIDAR DATA TRANSFER BY SOFT SERIAL
UART PROBLEM SOLVED WITH CHECK NUMBER 0
*/
#include<SoftwareSerial.h>// soft serial port header file
SoftwareSerial Serial2(0,2); // D3 as RX and D4 as TX _____ lidar
SoftwareSerial Serial3(5,4); // D1 as RX and D2 as TX _____ atmega
int a = 0,b = 0,c = 0;

/*For Arduino board with multiple serial ports such as DUE board, comment out the above two codes, and directly use Serial2 port*/
uint16_t dist= 0;// LiDAR actually measured distance value
uint16_t check;// check numerical value storage
int i;
byte uart[9];// store data measured by LiDAR
byte HEADER=0x59;// data package frame header (check datasheet)

void setup() 
{
  
 Serial.begin(115200);
 Serial2.begin(115200);
 Serial3.begin(115200);

}

void loop() 
{
  if (Serial2.available())//check whether the serial port has data input
  {
            delay(1); // this delay stabilizes code
            uart[0] = Serial2.read();
            
    if(uart[0]==HEADER)// determine data package frame header 0x59
    {
              uart[1] = Serial2.read();
              
      if(uart[1]==HEADER)//determine data package frame header 0x59
      {
        for(i=2;i<9;i++)// store data to array
        {
          uart[i]=Serial2.read();
        }
      check=uart[0]+uart[1]+uart[2]+uart[3]+uart[4]+uart[5]+uart[6]+uart[7];
      
      if(uart[8]==(check&0xff))// check the received data as per protocols
      {
        dist = 0;
        dist = dist | uart[3];// higher bits
        dist = dist<<8;
        dist = dist | uart[2];// lower bits
        if(dist<=1200) 
           {
           Serial.println(dist);
           a=dist%100;
           b=dist/100;
           
           Serial3.write(c);
           delay(2);
           Serial3.write(a);
           delay(2);
           Serial3.write(b);
           delay(2);
           }
       }
     }
   }
 }


 
}
