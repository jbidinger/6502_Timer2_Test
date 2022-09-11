#ifndef buttons_h
#define buttons_h

#define BUTTON1_PIN 2 
#define BUTTON2_PIN 3 
#define LED_PIN LED_BUILTIN

extern Bounce2::Button button1;
extern Bounce2::Button button2;

extern bool button1_status;
extern bool button2_status;

void button_setup();
void button_loop();

#endif