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

const byte pins_addr[] = {22,24,26,28,30,32,34,36, 38,40,42,44,46,48,50,52};
const byte pins_data[] = {23,25,27,29,31,33,35,37};
const byte pins_rw     = 41;
const byte pins_OE         = 51; // Output Enable
const byte pins_CS0        = 45; // RAM
const byte pins_CS1        = 47; // ROM
const byte pins_CS2        = 49; // IO


word addr_line = 0x00;
byte data_line = 0x0000;
byte data_rw   = 0x00;
byte data_CS0  = 0;
byte data_CS1  = 0;
byte data_CS2  = 0;
byte data_OE   = 0;

byte print_header = 0;

#define LED_PIN LED_BUILTIN

void read_data_and_addr_init() {
  for(int x=0; x < 8; x++) {
    pinMode(pins_addr[x], INPUT);
    pinMode(pins_data[x], INPUT);
  }
  for(int x=8; x<16; x++) {
    pinMode(pins_addr[x], INPUT);
  }
  pinMode(pins_rw, INPUT);
}

void setup()
{
    Serial.begin(115200);

    timer2_setup();
    lcd_setup();
    button_setup();
    read_data_and_addr_init();
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


void read_data_and_addr() {
  addr_line = 0x00;
  data_line = 0x0000;
  int y;  // Input from pins
  char print_scratch[80] = "";

  for(int x = 7; x >= 0; x--) {  // Read MSB to LSB so we can shift
    y=digitalRead(pins_data[x]);
    if(y==HIGH) {
      data_line = data_line | (1 << x);
    }
  }
  for(int x = 15; x >= 0; x--) {
    y = digitalRead(pins_addr[x]);
    if(y==HIGH) {
      addr_line = addr_line | (1 << x);
    }
  }
  data_rw = digitalRead(pins_rw);
  data_CS0 = digitalRead(pins_CS0);
  data_CS1 = digitalRead(pins_CS1);
  data_CS2 = digitalRead(pins_CS2);
  data_OE  = digitalRead(pins_OE);
  if(print_header == 0) {
    Serial.println("Addr Data RW OE CS_RAM CS_ROM CS_IO");
  }
  print_header++;
  if(print_header > 10) { print_header = 0; }
  sprintf(print_scratch, "%04X   %02X %2s %2x %6x %6x %5x", addr_line, data_line, data_rw == LOW ? "W" : "R", data_OE, !data_CS0, !data_CS1, !data_CS2);
  Serial.println(print_scratch);
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
    //Serial.println("Single Step");
    delay(20);
    digitalWrite(clockOutputPin, HIGH);
//    while(clockPinChange == 0) {  // Wait for interrupt
//     delay(2);
//    }
    
    clockPinChange = 0;  // Need to get this into a common routine that acts as a ISR.
    delay(20);
    read_data_and_addr();
    digitalWrite(clockOutputPin, LOW);
    //clockpinstate = 0;
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
