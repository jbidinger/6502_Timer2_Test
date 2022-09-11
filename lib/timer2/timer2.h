#ifndef timer2_h
#define timer2_h

extern const int ocr2aval;
extern volatile unsigned char clockpinstate;
extern const unsigned char clockBits;
extern const int freqOutputPin;

void timer2_setup();
void timer2_stop();
void timer2_resume();

#endif