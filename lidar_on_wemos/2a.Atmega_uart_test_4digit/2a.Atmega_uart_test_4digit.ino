/*
ATMEGA UART READ
FOR UPTO 4 DIGIT NUMBERS
NUMBER BROKEN IN 2 TWO DIGITS AND TRANSFERED
*/
int x,a[4],i;

void setup() 
{
 
  Serial.begin(115200);

}


void loop() 
{
//  Serial.println(Serial.available());
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
   
 //}
 Serial.println(x);

// if (x<=1200)
// {Serial.println(x);}
// 
}
}
