/*
ATMEGA UART READ
FOR ONE BYTE NUMBERS ONLY
i.e TRANSFER OF NUMBERS UPTO 255 ONLY
*/
int a;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

}

void loop() {
  // put your main code here, to run repeatedly:
  a= Serial.read();
  Serial.println(a);
}
