#include <avr/interrupt.h>
#include <util/delay.h>

volatile int timer_overflow_count = 0;
volatile int red = 0;
volatile int redinc = 1;
volatile int green = 0;

volatile int blue = 0;
volatile int blueinc = 3;

volatile int RESOLUTION = 255;

ISR(TIM0_OVF_vect) {

  if( (PINB & (1<<PB2)) == 0){
    green = 0;
    red += redinc;
    blue += blueinc;
    
    if (red > RESOLUTION){
      redinc *= -1;
    } else if (red < 0){
      redinc *= -1;
    }
    if (blue > RESOLUTION){
      blueinc *= -1;
    } else if (blue < 0){
      blueinc *= -1;
    }
  } else {
    red = 0;
    blue = 0;
    green = 255;
  }
  

  if (++timer_overflow_count > RESOLUTION) {   // a timer overflow occurs 4.6 times per second
    // Toggle Port B pin 4 output state
    timer_overflow_count = 0;
  }
}

int main(void) {
  // Set up Port B pin 4 mode to output
  int counter = 0;

  // SET RGB TO OUTPUT
  DDRB |= (1<<DDB4);
  DDRB |= (1<<DDB3);
  DDRB |= (1<<DDB0);

  // SET PB2 TO INPUT (CLEAR THE BIT)
  DDRB &= ~(1<<DDB2);
  // ENSURE NO PULL UP AS IS PULLED DOWN
  PORTB &= ~(1<<PB2);
  

  /// prescale timer to 1/256th the clock rate
  TCCR0B |= (1<<CS02); 

  // enable timer overflow interrupt
  TIMSK |=1<<TOIE0;
  // set enable interrupt ?
  sei();

  while(1) {
    for(counter=0; counter < RESOLUTION; counter++){    
      if(counter < red){
	PORTB |= (1<<PB4);
      } else {
	PORTB &= ~(1<<PB4);
      }
      if(counter < green){
	PORTB |= (1<<3);
      } else {
	PORTB &= ~(1<<3);
      }
      if(counter < blue){
	PORTB |= (1<<0);
      } else {
	PORTB &= ~(1<<0);
      }
    }
  }
}
