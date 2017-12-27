#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/sleep.h>

#include "types.h"
#include "saddle.h"

enum{
	EV_TIMER0 = 1 << 0
};
typedef uchar evt;
volatile evt ievents;

evt
poll(evt events)
{
	evt revents = 0;
	
	set_sleep_mode(SLEEP_MODE_IDLE);
	while(revents == 0){
		cli();
		
		if(events & EV_TIMER0){
			TIMSK0 |= _BV(OCIE0A);
			if(ievents & EV_TIMER0){
				ievents &= ~EV_TIMER0;
				revents |= EV_TIMER0;
			}
		}else{
			TIMSK0 &= _BV(OCIE0A);
		}
		
		if(events == 0)
			break;
		if(revents == 0){
			sleep_enable();
			sei();
			sleep_cpu();
			sleep_disable();
			continue;
		}
	}
	sei();
	return revents;
}

void
init(void)
{
	PRR0 = _BV(PRTWI) | _BV(PRTIM1) | _BV(PRSPI);
	PRR1 = _BV(PRUSB) | _BV(PRTIM3) | _BV(PRUSART1);
	
	DDRB = _BV(PB5) | _BV(PB6);
	DDRC = _BV(PC6) | _BV(PC7);
	DDRD = _BV(PD6) | _BV(PD7);
	
	TCCR4A = _BV(COM4A0) | _BV(COM4B0) | _BV(PWM4A) | _BV(PWM4B);
	TCCR4C = _BV(COM4A0) | _BV(COM4B0) | _BV(COM4D0) | _BV(PWM4D);
	TCCR4D = _BV(WGM40);		/* phase and frequency correct PWM mode */
	OCR4C = 0xff;			/* 8-bit PWM */
	DT4 = 0xff;
	TCCR4B = _BV(CS40);
	
	TCCR0A = _BV(WGM01);		/* CTC mode */
	TCCR0B = _BV(CS02) | _BV(CS00);	/* /1024 prescaler */
	OCR0A = 124;			/* 16000000 / 1024 / (124 + 1) = 125 Hz */
}

void
torque_inc(void)
{
	evt revents;
	uchar torque = 0, phase = 0;
	
	while(1){
		revents = poll(EV_TIMER0);
		if(revents & EV_TIMER0){
			OCR4A = (ushort)(saddle[phase] * torque) >> 12;
			OCR4B = (ushort)(saddle[(uchar)(phase + 256 / 3)] * torque) >> 12;
			OCR4D = (ushort)(saddle[(uchar)(phase - 256 / 3)] * torque) >> 12;
			torque++;
		}
	}
}

ISR(TIMER0_COMPA_vect)
{
	ievents |= EV_TIMER0;
	return;
}

int
main(void)
{
	init();
	torque_inc();
	
	while(1);
}
