/**
	SRA library for SRA's development board
	with MCU: ATmega16
	This module provides APIs to use the following devices:
		1) PWM		(Timer 1, frequency 3kHz)
		2) ADC		(8 bit)
		3) USART	(works with CP2102)
		4) Motors

	For more information visit http://sra.vjti.info/

	This code is in the public domain.

	by Society Of Robotics And Automation, VJTI.
*/

/* I N C L U D E S */
#include<avr/io.h>
#include<stdlib.h>
#include<compat/deprecated.h>
#include<util/delay.h>
#include<avr/eeprom.h>
#include<IO16.h>
#include <inttypes.h>
/* E N D S */

unsigned char min[sensor_num],	max[sensor_num],	threshold[sensor_num];
unsigned char sensorbyte=0;

//PORT INITIALIZE
void port_init(void)
{
	DDRA  = 0x00;
	PORTA = 0xFF;
	DDRB  = 0x03;
	PORTB = 0xFC;  
	DDRC  = 0xFC;
	PORTC = 0x03;
	DDRD  = 0x00;
	PORTD = 0xFF;
}

//PWM1 INITIALIZE
void pwm1_init(void)
{
	PWM1A_DIR=1;
	PWM1B_DIR=1;
	TCCR1B = 0x00; //stop
	TCNT1H = 0x00; //setup
	TCNT1L = 0x00;
	OCR1AH = 0x00;
	OCR1AL = 0x00;
	OCR1BH = 0x00;
	OCR1BL = 0x00;
	ICR1H  = 0x01; //for 3khz frequency
	ICR1L  = 0x8F;
	TCCR1A = 0xA2; //prescalar set to 8
	TCCR1B = 0x1A; //start Timer
}

//ADC INITIALIZE
void adc_init(void)
{
 ADC_DIR=0X00;
 ADCSRA=0X00;
 ADMUX=0X60;//0x40 for 10 bits
 ADCSRA=0X87;
 ACSR=0X80;
}

//ADC START
unsigned char adc_start(unsigned char channel)
{
 unsigned char i;
 
     ADCH=0x00;

	 i=channel&0x07;
	 ADMUX=i|0x60;        //i|0x40 for 10 bits
	 ADCSRA|=1<<ADSC;
	   
		 while(ADCSRA & (1<<ADSC));       // wait for conv. to complete
		 unsigned char temp=ADCH;      //unsigned int temp=ADC;   for 10 bits
   
 return temp;
}
 

//LED FLICKER FUNCTION
void flick (void)
{
unsigned int i=0;

	
		PORTC |= 0xFC;
		PORTB |= 0x03;
		_delay_ms(100);
		PORTC &= 0x03;
		PORTB &= 0xFC;
		_delay_ms(100);
	

}

//SET PWM1A
void set_pwm1a(int a)
{
	OCR1A=a;
}

//SET PWM1B
void set_pwm1b(int b)
{
	OCR1B=b;
}

//BOT MOTIONS
void bot_motion_init(void)
{
	DDRC  |= 0xF0;
	PORTC |= 0xF0;

	pwm1_init();
}

void bot_left_forward(void)
{
	MOTOR1A=1;
	MOTOR1B=0;
	MOTOR2A=0;
	MOTOR2B=0;
}

void bot_left_backward(void)
{
	MOTOR1A=0;
	MOTOR1B=1;
	MOTOR2A=0;
	MOTOR2B=0;
}

void bot_right_forward(void)
{
	MOTOR1A=0;
	MOTOR1B=0;
	MOTOR2A=1;
	MOTOR2B=0; 
}

void bot_left(void)
{
	bot_right_forward();
}

void bot_right(void)
{
	bot_left_forward();
}

void bot_right_backward(void)
{
	MOTOR1A=0;
	MOTOR1B=0;
	MOTOR2A=0;
	MOTOR2B=1; 
}

void bot_forward(void)
{
	MOTOR1A=1;
	MOTOR1B=0;
	MOTOR2A=1;
	MOTOR2B=0;
}
void bot_backward(void)
{
	MOTOR1A=0;
	MOTOR1B=1;
	MOTOR2A=0;
	MOTOR2B=1;
}
void bot_spot_left(void)
{
	MOTOR1A=0;
	MOTOR1B=1;
	MOTOR2A=1;
	MOTOR2B=0;
}

void bot_spot_right(void)
{
	MOTOR1A=1;
	MOTOR1B=0;
	MOTOR2A=0;
	MOTOR2B=1;
}
void bot_stop(void)
{
	MOTOR1A=0;
	MOTOR1B=0;
	MOTOR2A=0;
	MOTOR2B=0;
}
void bot_brake(void)
{
	MOTOR1A=1;
	MOTOR1B=1;
	MOTOR2A=1;
	MOTOR2B=1;
}

void switch_init(void)
{
	DDRD	&=	0xF0;
	PORTD	|=	0x0F;
}

int pressed_switch0(void)
{
	if(bit_is_clear(PIND,0))
	return 1;
	else
	return 0;
}

int pressed_switch1(void)
{
	if(bit_is_clear(PIND,1))
	return 1;
	else
	return 0;
}

int pressed_switch2(void)
{
	if(bit_is_clear(PIND,2))
	return 1;
	else
	return 0;
}

int pressed_switch3(void)
{
	if(bit_is_clear(PIND,3))
	return 1;
	else
	return 0;
}



//USART functions

/* Functions for Serial communication(UART) .for using CP2102 too.
Call SerialBegin(long baudrate) to initialise the UART.
Call SerialWriteChar(char x) to print single character.
Call SerialWriteString(char x[]) to print string. Pass string in double inverted commas.
Call SerialWriteInt(int x) to print integer(signed only)
Call SerialWriteLong(unsigned long int x) to print unsigned long integers.

*/

//This function is used to read the available data
//from USART. This function will wait untill data is
//available.

//This function is used to initialize the USART
//at a given UBRR value
void SerialBegin(long baud_rate)
{

   //Set Baud rate
   uint16_t ubrr_value = (1000000/baud_rate) - 1;
   UBRRL = ubrr_value;
   UBRRH = (ubrr_value>>8);

   ///Set Frame Format
   //Asynchronous mode
   //No Parity
   //1 StopBit
   //char size 8


   UCSRC=(1<<URSEL)|(3<<UCSZ0);


   //Enable The receiver and transmitter

   UCSRB=(1<<RXEN)|(1<<TXEN);


}


char SerialReadChar()
{
   //Wait untill a data is available

   while(!(UCSRA & (1<<RXC)))
   {
		;
      //Do nothing
   }
   //Data recieved in USART
   return UDR;
}


//This fuction prints a single character.
void SerialWriteChar(char data)
{
   //Wait untill the transmitter is ready

   while(!(UCSRA & (1<<UDRE)))
   {
      //Do nothing
   }

   //Now write the data to USART buffer

   UDR=data;
}


//This function prints a string. Pass in double inverted commas or a character array.
void SerialWriteString(char string[])
{
	for (int i=0; string[i]; i++) 
	{
		SerialWriteChar(string[i]);
	}
	// for (int i=0; i<strlen(string); i++) 
	// {
	// 	SerialWriteChar(string[i]);
	// }	
}

//This function prints a signed integer.
void SerialWriteInt(int data)
{
	int i = 0, count = 0;
	
	if(data<0)
	{
		SerialWriteChar('-');
		data*=-1;
	}
	
	int x = data;
	
	//count number of digits
	while(x)
	{
		count++;
		x = x/10;
	}
	
   int temp[count];
   
   //store each digit in temp array
   while(data)
   {
		temp[i] = data%10;
		data = data/10;
		i++;
   }
   
   
   //print each digit by typecasting to character
   for(i = count-1; i>=0 ; i--)
   {
		SerialWriteChar(temp[i]+'0');
   }

  
}

//This function prints an unsigned long int.
void SerialWriteLong(unsigned long int data)
{
	int i = 0, count = 0;	
	unsigned long x = data;
	
	//count number of digits
	while(x)
	{
		count++;
		x = x/10;
	}
	
   int temp[count];
   
   //store each digit in temp array
   while(data)
   {
		temp[i] = data%10;
		data = data/10;
		i++;
   }
   
   
   //print each digit by typecasting to character
   for(i = count-1; i>=0 ; i--)
   {
		SerialWriteChar(temp[i]+'0');
   }
}
