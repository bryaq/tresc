#include <avr/io.h>

#include "types.h"
#include "saddle.h"

int
main(void)
{
	DDRB = _BV(PB5) | _BV(PB6);
	DDRC = _BV(PC6) | _BV(PC7);
	DDRD = _BV(PD6) | _BV(PD7);
	
	TCCR4A = _BV(COM4A0) | _BV(COM4B0) | _BV(PWM4A) | _BV(PWM4B);
	TCCR4C = _BV(COM4A0) | _BV(COM4B0) | _BV(COM4D0) | _BV(PWM4D);
	TCCR4D = _BV(WGM40);
	OCR4C = 0xff;
	OCR4A = saddle[0] >> 4;
	OCR4B = saddle[85] >> 4;
	OCR4D = saddle[170] >> 4;
	DT4 = 0xff;
	TCCR4B = _BV(CS40);
	
	while(1);
}
