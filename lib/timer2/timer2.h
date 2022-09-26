#ifndef timer2_h
#define timer2_h

extern const int ocr2aval;
extern volatile unsigned char clockpinstate;
extern volatile bool clockPinChange;
extern const unsigned char clockBits;
extern const int clockOutputPin;
extern const int clockInterruptPin;

void timer2_setup();
void timer2_stop();
void timer2_resume();
void clockInterruptISR();

#endif