/*
 * BASICL.c
 *
 * Created: 1/16/2020 11:24:36 AM
 * Author : trans
 */ 

#include <avr/io.h>
#include <util/delay.h>


#define F_CPU 8000000UL


void PWM_init()
{
	/*set fast PWM mode with non-inverted output*/
	TCCR0 = (1<<WGM00) | (1<<WGM01) | (1<<COM01) | (1<<CS00);
	DDRB|=(1<<PB3);  /*set OC0 pin as output*/
	 DDRD |= (1<<PD7);                              //set PD7 as PWM output

	 TCCR2 |= (1<<WGM20)|(1<<WGM21);                //select Fast PWM mode by setting bits

	 TCCR2 |=(1<<COM21)|(1<<CS20)|(0<<CS21)|(0<<CS22);   //clear OC2 on compare match
}


int main ()
{
	unsigned char duty;
	DDRC=0xFF;
	
	PWM_init();
	PORTC|=(1<<PC0);
	_delay_ms(1000);
	PORTC|=~(1<<PC0);
	_delay_ms(1000);
			OCR0=150;
			OCR2=130;  /*increase the LED light intensity*/
			
	
	
}