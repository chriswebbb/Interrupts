#define __ASSERT_USE_STDERR

#include <avr/io.h>
#include <util/delay.h>
#include <assert.h>
#include <stdio.h>
#include <avr/interrupt.h>

#define DEBUG_BAUD  9600

int uputchar0(char c, FILE *stream);
int ugetchar0(FILE *stream);
void init_debug_uart0(void);

volatile uint8_t i = 0;	//used for counting

ISR(TIMER1_COMPA_vect){
	PINB|=_BV(PB7);	//toggles the led on the il matto
	printf("%i\n", i);	//prints the number
		//increments the value and if it goes above 9 to loop it round
		if(i<9){
			i++;
		}else{
			i=0;
			}

}

int main(void){
	
	init_debug_uart0();	//allows communication via the uart
	
	DDRA = 0xFF;	//sets pins to be outputs for port a
	
	DDRB |= _BV(PB7);	//sets the led to be off initially
	
	//Initialize the counter IN FAST PWM mode
	TCCR1A = 0;
	TCCR1B = _BV(WGM12);
	TCCR1B |= _BV(CS12)|_BV(CS10); 
	
	//array of the numbers
	const uint8_t segments[10]={0xFC, 0x60, 0xDA, 0xF2, 0x66, 0xB6, 0xBE, 0xE4, 0xFE, 0xF6};

	OCR1A = 11718;	//sets the top of the counter to be a certain value so that when it reaches the top 1 second has passed, uses the 1024 prescaler
	
	//enables the interrupts
	TIMSK1 |= _BV(OCIE1A) ; 
	sei();
	
	while(1){
		PORTA = segments[i];	//continuously outputting the seven seg value from the indexed point
	}
}

int uputchar0(char c, FILE *stream)
{
	if (c == '\n') uputchar0('\r', stream);
	while (!(UCSR0A & _BV(UDRE0)));
	UDR0 = c;
	return c;
}

int ugetchar0(FILE *stream)
{
	while(!(UCSR0A & _BV(RXC0)));
	return UDR0;
}

void init_debug_uart0(void)
{
	/* Configure UART0 baud rate, one start bit, 8-bit, no parity and one stop bit */
	UBRR0H = (F_CPU/(DEBUG_BAUD*16L)-1) >> 8;
	UBRR0L = (F_CPU/(DEBUG_BAUD*16L)-1);
	UCSR0B = _BV(RXEN0) | _BV(TXEN0);
	UCSR0C = _BV(UCSZ00) | _BV(UCSZ01);

	/* Setup new streams for input and output */
	static FILE uout = FDEV_SETUP_STREAM(uputchar0, NULL, _FDEV_SETUP_WRITE);
	static FILE uin = FDEV_SETUP_STREAM(NULL, ugetchar0, _FDEV_SETUP_READ);

	/* Redirect all standard streams to UART0 */
	stdout = &uout;
	stderr = &uout;
	stdin = &uin;
}