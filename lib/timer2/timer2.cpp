#include <Arduino.h>
#include "timer2.h"

//const int ocr2aval  = 255;  // Max for timer2 (255)
const int ocr2aval  = 1;  // Max for timer2 (255)
volatile unsigned char clockpinstate = LOW;
volatile bool clockPinChange = false;
const unsigned char clockBits = ( ( 1 << CS22) | ( 1 << CS21) | ( 1 << CS20) );  // prescalar
const int clockOutputPin = 10;
const int clockInterruptPin = 19;

/*
ISR(TIMER2_COMPA_vect) {
  clockpinstate = digitalRead(clockOutputPin);
  Serial.print("* Interrupt: ");
  Serial.println(clockpinstate);
}
*/


void clockInterruptISR() {
  cli();
  //Serial.println("ISR entered");
  clockpinstate = digitalRead(clockInterruptPin);
  clockPinChange = true;

  //Serial.print("* Interrupt: ");
  //Serial.println(clockpinstate);
  sei();
}


void timer2_setup() {
    pinMode(clockOutputPin, OUTPUT);
    pinMode(clockInterruptPin, INPUT);

    cli(); // pause interrupts while we setup. This may not be needed. *shrug*
 
    // Set Timer 2 CTC mode with prescaling.  OC2A toggles on compare match
    //
    // WGM22:0 = 010: CTC Mode, toggle OC 
    // WGM2 bits 1 and 0 are in TCCR2A,
    // WGM2 bit 2 is in TCCR2B
    // COM2A0 sets OC2A (arduino pin 11 on Uno or Duemilanove) to toggle on compare match
    //
    TCCR2A = ((1 << WGM21) | (1 << COM2A0));

    // Set Timer 2  No prescaling  (i.e. prescale division = 1)
    //
    // CS22:0 = 001: Use CPU clock with no prescaling
    // CS2 bits 2:0 are all in TCCR2B
    TCCR2B = clockBits; // prescalar

    // Make sure Compare-match register A interrupt for timer2 is disabled
    //TIMSK2 = 0;
    //TIMSK2 = (1 << OCIE2A);   // Enable interrupt
    // This value determines the output frequency
    OCR2A = ocr2aval;

    // After all that we're going to pause and undo some of the above. We want to
    // start in paused mode. If we still get a pwm, we'll have to implement the pause
    // above.
    timer2_stop();

    sei();

  /* In pause mode we don't get the timer interrupt. The solution was to loop back
   * the timer output to another pin that we can set the interrupt on.
  */
  attachInterrupt(digitalPinToInterrupt(clockInterruptPin),clockInterruptISR,RISING);
}

void timer2_stop(){
    while(digitalRead(clockOutputPin) == HIGH) {
    delay(10);
  }
  OCR2A = 0; // Compare value to zero
  TCCR2A = ((1 << WGM21) | (0 << COM2A0));  // Disconnect OC2A so we can control the pin
  TCCR2B = (1 << WGM13); // clockbits (prescalar) to zero and no clock source.
}

void timer2_resume() {
  OCR2A = ocr2aval;  // compare value
  TCCR2A = ((1 << WGM21) | (1 << COM2A0)); // reconnnect OC2A
  TCCR2B = (1 << WGM13) | clockBits; // enaable clock and prescalar
}
