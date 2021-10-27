#include<avr/io.h>
#include<avr/interrupt.h>
#include "debug.h"

volatile uint8_t i = 0;

ISR(INT0_vect){
	PORTB |= _BV(PB7);	//turns on pin 7
	if(i<128){i++;}	//checks if less then 255 and if so increments
	//printf("%d\n", i); //prints the value of i
}

ISR(INT1_vect){
	PORTB &= ~_BV(PB7);	//Turns off the pin 7
	if(i>0){i--;}	//checks if greater than 0 and if true decrement
	//printf("%d\n", i);	//print the value in i
}

ISR(ADC_vect){
	PINB |= _BV(PB7);
	//printf("%4d \n",ADC);
}

int main(void){
	
	int prescaler = 2;
	
	init_debug_uart0();
	
	DDRB |= _BV(PB7);	//enables the pin as an output
	PORTB &= ~_BV(PB7);	//sets it's initial value to be low
	
	EICRA |= _BV(ISC01) | _BV(ISC00);	//Trigger INT0 to be a rising edge
	EICRA |= _BV(ISC11);				//Trigger INT1 to be falling edge
	
	ADCSRA = _BV(ADPS0);//Prescaler is set to 2
	
	ADMUX = 0x00;			//Selects the channel to read from
	
	ADCSRA |= _BV(ADATE) | _BV(ADEN);	
	ADCSRB = 0;
	
	//enables the interrupts
	EIMSK |= _BV(INT0);
	EIMSK |= _BV(INT1);	
	ADCSRA |= _BV(ADIE);
	sei();
	
	//Starts the first conversion in order to start the automatic
	ADCSRA |= _BV(ADSC);	 
	
	//As the interrupts alter the variable i before outputting this is necessary for the initial value
	//printf("%d\n", i);
	
	while(1){
		switch(i){
			case 2:	prescaler = 2;
			break;
			case 4:	prescaler = 4;
			break;
			case 8: prescaler = 8;
			break;
			case 16: prescaler = 16;
			break;
			case 32: prescaler = 32;
			break;
			case 64: prescaler = 64;
			break;
			case 128: prescaler = 128;
			break;
			default: prescaler = prescaler;
		}
		
		switch(prescaler){
			case 2:	ADCSRA = _BV(ADPS0);
			break;
			case 4: ADCSRA = _BV(ADPS1);
			break;
			case 8: ADCSRA = _BV(ADPS0) | _BV(ADPS1);
			break;
			case 16: ADCSRA = _BV(ADPS2);
			break;
			case 32: ADCSRA = _BV(ADPS2) | _BV(ADPS0);
			break;
			case 64: ADCSRA = _BV(ADPS2) |  _BV(ADPS1);
			break;
			case 128: ADCSRA = _BV(ADPS2) | _BV(ADPS0) | _BV(ADPS1);
			break;
			default: prescaler = prescaler;
		}
	}
}