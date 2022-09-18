#include <Arduino.h>
#include <Bounce2.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#include "timer2.h"
#include "buttons.h"
#include "lcd.h"
#include "util.h"

#define MODESINGLESTEP  1
#define MODERUNSTOP     2
unsigned int mode=MODESINGLESTEP;

//void binaryToPaddedString(char *, byte);

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

/*
    byte x =  0b10111100;
    byte y = ~x;
    char padded[20];
    binaryToPaddedString(padded,x);
    Serial.println(padded);
    binaryToPaddedString(padded,y);
    Serial.println(padded);
*/

    Serial.print("Period    = ");
    Serial.print(period); 
    Serial.println(" microseconds");
    Serial.print("Frequency = ");
    Serial.print(freq); 
    Serial.println(" Hz");
    lcd_update_mode();
}

void lcd_update_mode() {
  lcd.clear();
  lcd.setCursor(0,0); // top left
  if(mode == MODERUNSTOP) {
    lcd.print("Mode: Run       ");
    lcd.setCursor(0,2);
    lcd.print("Red pause");
  } else {
    lcd.print("Mode: Singlestep");
    lcd.setCursor(0,2);
    lcd.print("Green singlestep");
    lcd.setCursor(0,3);
    lcd.print("Red resume");
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
    digitalWrite(clockOutputPin, HIGH);
    clockpinstate = 1; // The ISR doesn't run since it's based off the timer so we'll emulate it.
    delay(10);
    digitalWrite(clockOutputPin, LOW);
    clockpinstate = 0;
    delay(10);    
  }

  if( button2_status ) { // button2 always causes a change in state.
    if( button2_status && mode==MODERUNSTOP ) { // We're in running mode
      while(digitalRead(clockOutputPin) == HIGH) {
        delay(10);
      }
      Serial.println("-  Going to singlestep");
      timer2_stop();
      mode=MODESINGLESTEP;
    } else if( button2_status && mode==MODESINGLESTEP ) { // Go back to running mode
      Serial.println("-  Going to run");
      timer2_resume();
      mode=MODERUNSTOP;
    }
    lcd_update_mode();
  }

}
