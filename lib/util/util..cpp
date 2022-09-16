#include <Arduino.h>

void binaryToPaddedString(char *output, byte inputbinary) {
  int targetStrLen = 8; 
  char unpadded[20];
  itoa (inputbinary,unpadded,2); 

    char buf[21] = { 0 };
    char fill = '0';
  

    sprintf(output, "%s%s", (char*)memset(buf, fill, targetStrLen - strlen(unpadded)), unpadded);
}