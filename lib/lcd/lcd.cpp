#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define I2C_LCD_ADDRESS 0x27
LiquidCrystal_I2C lcd(I2C_LCD_ADDRESS,20,4);

void lcd_setup() {
  lcd.init();

  lcd.backlight();
  lcd.setCursor(0,0);
  //lcd.print("I'm Alive!");
}

void find_i2c(){
  Serial.println ();
  Serial.println ("Search I2C Address ...");
  byte count = 0;
  
  Wire.begin();
  for (byte i = 8; i <120; i++)
  {
    Wire.beginTransmission (i);
    if (Wire.endTransmission () == 0)
      {
      Serial.println ("get it");
      Serial.print ("Address is: ");
      Serial.print ("0x");
      Serial.println (i, HEX);
      count++;
      delay (1);  
      } 
  } 
  Serial.println ("Done.");
  Serial.print ("Found ");
  Serial.print (count, DEC);
  Serial.println (" Device.");   
}
