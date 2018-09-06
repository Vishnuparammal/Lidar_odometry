/*
UART DATA TRANSFER
1 BYTE TRANSFER ONLY
i.e NUMBER UPTO 255 ONLY
*/
#include<SoftwareSerial.h>// soft serial port header file
SoftwareSerial Serial2(0,2); // D3 as RX, and D4 as TX
int a = 0,b=1,c=23;

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
Serial2.begin(115200);

}

void loop() {
  // put your main code here, to run repeatedly:
//  b=a%100;
//  c=a/100;
// Serial.write(b);
// delay(1);
// Serial.write(c);
 Serial2.write(C);
 delay(2);
}
