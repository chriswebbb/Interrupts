#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "debug.h"

ISR(ADC_vect){
	PINB |= _BV(PB7);
	//printf("%4d \n",ADC);
}

int main(void)
{
	init_debug_uart0();		//initializes UART communication
	
	DDRB |= _BV(PB7);		//Sets pin 7 as output on port B
	PORTB &= ~_BV(PB7);		//sets the pin 7 as low initially 
	
	ADCSRA = _BV(ADPS0);	//Prescaler is set to 2
	
	ADMUX = 0x00;			//Selects the channel to read from
	
	//Enables free-running mode and then enables the ADC all together
	ADCSRA |= _BV(ADATE) | _BV(ADEN);	
	ADCSRB = 0;	
	
	//enables global and adc interrupt
	ADCSRA |= _BV(ADIE);	
	sei();	
	
	ADCSRA |= _BV(ADSC);	//Starts the first conversion in order to start the automatic 
	
	for (;;) 
	{
	}
}
