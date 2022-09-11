#include <Bounce2.h>
#include "buttons.h"

bool button1_status = false;
bool button2_status = false;

Bounce2::Button button1 = Bounce2::Button();
Bounce2::Button button2 = Bounce2::Button();

void button_setup() {
  button1.attach( BUTTON1_PIN ,  INPUT_PULLUP ); // USE INTERNAL PULL-UP
  button2.attach( BUTTON2_PIN ,  INPUT_PULLUP ); // USE INTERNAL PULL-UP
  
  // DEBOUNCE INTERVAL IN MILLISECONDS
  button1.interval(5); 
  button2.interval(5); 
  // INDICATE THAT THE LOW STATE CORRESPONDS TO PHYSICALLY PRESSING THE BUTTON
  button1.setPressedState(LOW); 
  button2.setPressedState(LOW);   
}

bool button1_loop(){
  button1.update();
  button1_status = button1.pressed();
  return( button1_status );

}

bool button2_loop(){
  button2.update();
  button2_status = button2.pressed();
  return( button2_status );

}

void button_loop() {
  button1_loop();
  button2_loop();
}
