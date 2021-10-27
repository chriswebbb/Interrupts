#include<avr/io.h>
#include<avr/interrupt.h>
#include "debug.h"

volatile uint8_t i = 0;

ISR(INT0_vect){
	//PORTB |= _BV(PB7);	//turns on pin 7
	if(i<255){i++;}	//checks if less then 255 and if so increments
	OCR0A = i;
}

ISR(INT1_vect){
	//PORTB &= ~_BV(PB7);	//Turns off the pin 7
	if(i>0){i--;}	//checks if greater than 0 and if true decrement
	OCR0A = i;
}

ISR(ADC_vect){
	PINB |= _BV(PB7);
	
}

int main(void){
	
	init_debug_uart0();
	
	DDRB |= _BV(PB7);	//enables the pin as an output
	PORTB &= ~_BV(PB7);	//sets it's initial value to be low
	
	EICRA |= _BV(ISC01) | _BV(ISC00);	//Trigger INT0 to be a rising edge
	EICRA |= _BV(ISC11);				//Trigger INT1 to be falling edge
	
	//Modifies the ADC trigger source to be the counter 0 with compare match
	ADCSRB = _BV(ADTS0) | _BV(ADTS1);
	
	//Selects the channel to read from
	ADMUX = 0x00;
	
	//Enables free-running mode and then enables the ADC all together
	ADCSRA |= _BV(ADATE) | _BV(ADEN);
	ADCSRA |= _BV(ADSP1) | _BV(ADSP2);
	
	//Sets the behavior of clock 0
	TCCR0A = _BV(WGM01);
	
	//enables the interrupts
	EIMSK |= _BV(INT0);
	EIMSK |= _BV(INT1);
	ADCSRA |= _BV(ADIE);	
	sei();

	//As the interrupts alter the variable i before outputting this is necessary for the initial value
	//printf("%d\n", i);
	
	while(1){
	}
}