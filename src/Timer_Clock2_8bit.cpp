#include <Arduino.h>
#include <Bounce2.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#include "timer2.h"
#include "buttons.h"
#include "lcd.h"

#define MODESINGLESTEP  1
#define MODERUNSTOP     2
unsigned int mode=MODESINGLESTEP;

// Note that the prescale value is just for printing; changing it here
// does not change the clock division ratio for the timer!  To change
// the timer prescale division, use different bits for CS22:0 below
const int prescale  = 1024;   

// Period in microseconds
const float period    = 2.0 * prescale * (ocr2aval+1) / (F_CPU/1.0e6);

// Frequency in Hz
const float freq      = 1.0e6 / period;

#define LED_PIN LED_BUILTIN

void setup()
{
    Serial.begin(115200);

    timer2_setup();
    lcd_setup();
    button_setup();
    
    Serial.print("Period    = ");
    Serial.print(period); 
    Serial.println(" microseconds");
    Serial.print("Frequency = ");
    Serial.print(freq); 
    Serial.println(" Hz");
    lcd_update_mode();
}

void lcd_update_mode() {
  lcd.setCursor(0,0); // top left
  if(mode == MODERUNSTOP) {
    lcd.print("Mode: Run       ");
  } else {
    lcd.print("Mode: Singlestep");
  }
}
void loop()
{
  // button1 is the singlestep button
  // button2 is the pause/run button

  // Read the buttons. Ressults are in button1_status/button2_status.
  // true is pressed.
  button_loop(); 

  // single step pressed and in singlestep mode
  if( button1_status && mode == MODESINGLESTEP ) {
    Serial.println("Single Step");
    digitalWrite(freqOutputPin, HIGH);
    delay(10);
    digitalWrite(freqOutputPin, LOW);
    delay(10);    
  }

  if( button2_status ) { // button2 always causes a change in state.
    if( button2_status && mode==MODERUNSTOP ) { // We're in running mode
      while(digitalRead(freqOutputPin) == HIGH) {
        delay(10);
      }
      Serial.println("  Going to singlestep");
      timer2_stop();
      mode=MODESINGLESTEP;
    } else if( button2_status && mode==MODESINGLESTEP ) { // Go basck to running mode
      Serial.println("  Going to run");
      timer2_resume();
      mode=MODERUNSTOP;
    }
    lcd_update_mode();
  }
/*  
  delay(3000);
  // stop
  // Wait for low state since stopping the timer doesn't clear the output pin.
  while(digitalRead(freqOutputPin) == HIGH) {
    delay(10);
  }
  Serial.println("  Stopping Timer2");
  timer2_stop();
//  OCR2A = 0;
//  TCCR2A = ((1 << WGM21) | (0 << COM2A0));  // Disconnect OC2A so we can control the pin
//  TCCR2B = (1 << WGM13); // clockbits (prescalar) to zero and no clock source.

  // try pulsing the data pin to see if we have any control
  delay(1000); // a little delay between PWM and manual.
  Serial.println("Test pulses");
  for(int x=1; x < 250; x++) {
    digitalWrite(freqOutputPin, HIGH);
//    Serial.print(".");
    delay(5);
    digitalWrite(freqOutputPin, LOW);
    delay(5);
  }
//  Serial.println("");
  delay(3000);
  Serial.println("  Starting Timer2");
  timer2_resume();
//  OCR2A = ocr2aval;
//  TCCR2A = ((1 << WGM21) | (1 << COM2A0)); // reconnnect OC2A
//  TCCR2B = (1 << WGM13) | clockBits;
*/
}
