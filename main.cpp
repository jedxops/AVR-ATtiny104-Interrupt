/*
 * Interrupt New.cpp
 *
 * Created: 10/21/2021 1:22:13 PM
 * Author : zombi
 *
 * There are 3 configurations for the output pins: high, low, and floating.
 * Floating basically means the pin is disconnected, which is why if PORTA 
 * is left unconfigured as output, the output pins are basically left in a 
 * disconnected, 'floating' state, and the LED stays off
 *
 * AVR pins can have one of four states:
 *
 * DDR  PORT
 * 0    0   - input, high impedance
 * 0    1   - input, pulled up
 * 1    0   - output, driven low
 * 1    1   - output, driven high
 */

#include<avr/io.h>
#include<avr/interrupt.h>

// Use PortA Bit 5 to toggle LED 
#define LED PA5

// Timer0 ISR
ISR (TIM0_OVF_vect) {
  PORTA ^= (1 << LED);
  // For 1 second at 16 MHz
  TCNT0 = 63974;
}

int main(void) {

  // Configure PORTA as output
  DDRA = (0x01 << LED);

  // For 1 sec at 16 MHz
  TCNT0 = 63974;
  TCCR0A = 0x00;

  // Timer mode with 1024 pre-scaler (CLKio / 1024)
  TCCR0B = (1<<CS00) | (1<<CS02);

  // WGM control (default value is 0 for each bit, signifying 'normal mode'):
  // TCCR0B = TCCR0B | ((1 << 3) | (1 << 4));

  // Enable timer0 overflow interrupt (TOIE0)
  TIMSK0 = (1 << TOIE0);

  // Enable global interrupts by setting global interrupt enable bit in SREG
  sei();
  while(1);
  return 0;
}
