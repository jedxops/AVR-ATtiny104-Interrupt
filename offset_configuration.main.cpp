/*
 * Inspired by: https://exploreembedded.com/wiki/AVR_Timer_programming
 *				https://exploreembedded.com/wiki/AVR_Timer_Interrupts
 * 
 * Interrupt New.cpp
 *
 * Created: 10/21/2021 1:22:13 PM
 * Author : zombi
 *
 * There are 3 configurations for the output pins: high, low, and floating.
 * Floating basically means the pin is disconnected, which is why if PORTA
 * is left un-configured as output, the output pins are basically left in a
 * disconnected, 'floating' state, and the LED stays off
 *
 * AVR pins can have one of four states:
 *
 * DDR  PORT
 * 0    0   - input, high impedance
 * 0    1   - input, pulled up
 * 1    0   - output, driven low
 * 1    1   - output, driven high
 * 
 * Timer 0 is a 16-bit timer => 2^16 ticks / overflow or 65536 ticks / overflow
 * Electrical Signal Frequency / pre-scalar = 16000000Hz / 1024 = 15.625 KHz
 * => 1 Tick = 1 / 15.625KHz = 1 tick / 0.000064 seconds 
 * 
 */

#include<avr/io.h>
#include<avr/interrupt.h>

// Use PortA Bit 5 to toggle LED
#define LED       PORTA5  // 5

// This is dereferencing the memory address 0x02, and substituting
// the value _stored_ at that memory address into 'PRTA':
#define PRTA      *((volatile uint8_t *) 0x02)

#define TIMER0    *((volatile uint16_t*) 0x28)
#define DRA       *((volatile uint8_t *) 0x01)
#define TCR0A     *((volatile uint8_t *) 0x2E)
#define TCR0B     *((volatile uint8_t *) 0x2D)
#define TMSK0     *((volatile uint8_t *) 0x2B)

// Timer0 ISR
ISR (TIM0_OVF_vect) {
  PRTA ^= (1 << LED);
  // Max is 65535 in which case you can't tell the LED is blinking
  TIMER0 = 65475;
}

int main(void) {

  // Configure PORTA as output
  DRA = (0x01 << LED);

  TIMER0 = 65475;
  TCR0A = 0x00;

  // Timer mode with 1024 pre-scaler (CLKio / 1024)
  TCR0B = (1<<CS00) | (1<<CS02);

  // WGM control (default value is 0 for each bit, signifying 'normal mode'):
  // TCR0B = TCR0B | ((1 << 3) | (1 << 4));

  // Enable timer0 overflow interrupt (TOIE0)
  TMSK0 = (1 << TOIE0);

  // Enable global interrupts by setting global interrupt enable bit in SREG
  sei();
  while(1);
  return 0;
}