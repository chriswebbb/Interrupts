#include<avr/io.h>
#include<avr/interrupt.h>
#include "debug.h"

volatile uint8_t i = 0;

ISR(INT0_vect){
	PORTB |= _BV(PB7);	//turns on pin 7
	if(i<255){i++;}	//checks if less then 255 and if so increments
	printf("%d\n", i); //prints the value of i
}

ISR(INT1_vect){
	PORTB &= ~_BV(PB7);	//Turns off the pin 7
	if(i>0){i--;}	//checks if greater than 0 and if true decrement
	printf("%d\n", i);	//print the value in i
}

int main(void){
	
	init_debug_uart0();
	
	DDRB |= _BV(PB7);	//enables the pin as an output
	PORTB &= ~_BV(PB7);	//sets it's initial value to be low
	
	EICRA |= _BV(ISC01) | _BV(ISC00);	//Trigger INT0 to be a rising edge
	EICRA |= _BV(ISC11);				//Trigger INT1 to be falling edge
	
	//enables the interrupts
	EIMSK |= _BV(INT0);
	EIMSK |= _BV(INT1);	
	sei();
	
	//As the interrupts alter the variable i before outputting this is necessary for the initial value
	printf("%d\n", i);
	
	while(1);
}