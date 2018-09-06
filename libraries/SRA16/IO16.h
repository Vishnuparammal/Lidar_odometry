/**
	io_config .h
	
 */
#ifndef IO16_H
#define IO16_H

#include <stdint.h>
#include <util/delay.h>
#include <avr/io.h>

#ifndef F_CPU
	#define F_CPU 16000000UL
#endif



//BASIC I/O FILE SETTINGS


#define INPUT 0
#define OUTPUT 1

typedef enum _BOOL { FALSE = 0, TRUE } BOOL;

typedef struct
{
  unsigned int bit0:1;
  unsigned int bit1:1;
  unsigned int bit2:1;
  unsigned int bit3:1;
  unsigned int bit4:1;
  unsigned int bit5:1;
  unsigned int bit6:1;
  unsigned int bit7:1;
} _io_reg; 


#define REGISTER_BIT(rg,bt) ((volatile _io_reg*)&rg)->bit##bt




/** I N C L U D E S **/


//SENSOR NUMBER
#define sensor_num 4	//change this value for any other sensor

//MOTOR VALUES

#define MOTOR1A   	REGISTER_BIT(PORTC,4)
#define MOTOR1B    	REGISTER_BIT(PORTC,5)
#define MOTOR2A    	REGISTER_BIT(PORTC,6)
#define MOTOR2B    	REGISTER_BIT(PORTC,7)


//MOTOR DIRECTIONS

#define MOTOR1A_DIR   	REGISTER_BIT(DDRC,4)
#define MOTOR1B_DIR     REGISTER_BIT(DDRC,5)
#define MOTOR2A_DIR     REGISTER_BIT(DDRC,6)
#define MOTOR2B_DIR     REGISTER_BIT(DDRC,7)


//ADC 	DIRECTIONS

#define ADC			PORTA
#define ADC_DIR 	DDRA


//PWM pins

//PWM0

#define PWM0_DIR  	REGISTER_BIT(DDRB,3)
#define PWM0  		REGISTER_BIT(PORTB,3)

//PWM1A

#define PWM1A_DIR  	REGISTER_BIT(DDRD,4)
#define PWM1A 		REGISTER_BIT(PORTD,4)

//PWM1B

#define PWM1B_DIR  	REGISTER_BIT(DDRD,5)
#define PWM1B 		REGISTER_BIT(PORTD,5)

//PWM2

#define PWM2_DIR  	REGISTER_BIT(DDRD,7)
#define PWM2 		REGISTER_BIT(PORTD,7)


//FUNCTION PROTOTYPES
void port_init(void);
void pwm1_init(void);
void adc_init(void);
unsigned char adc_start(unsigned char channel);
void flick (void);
void set_pwm1a(int a);
void set_pwm1b(int b);
void bot_motion_init(void);
void bot_left_forward(void);
void bot_left_backward(void);
void bot_right_forward(void);
void bot_left(void);
void bot_right(void);
void bot_right_backward(void);
void bot_forward(void);
void bot_backward(void);
void bot_spot_left(void);
void bot_spot_right(void);
void bot_stop(void);
void bot_brake(void);
void switch_init(void);
int pressed_switch0(void);
int pressed_switch1(void);
int pressed_switch2(void);
int pressed_switch3(void);
void SerialBegin(long baud_rate);
char SerialReadChar();
void SerialWriteChar(char data);
void SerialWriteString(char string[]);
void SerialWriteInt(int data);
void SerialWriteLong(unsigned long int data);

#endif //IO_H
