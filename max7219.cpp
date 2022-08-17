
#include "max7219.hpp"

MAX7219::MAX7219(){
   pinMode(DIN, OUTPUT);
   pinMode(CS, OUTPUT);
   pinMode(CLK, OUTPUT);
   digitalWrite(CS, HIGH);
   
   write(SHUTDOWN, 0x1); // exit shutdown mode
   write(DISPLAY_TEST, 0x0); // return to normal op after test mode
   write(DECODE_MODE, NO_DECODE); // set no decode mode to all digits - allows for also displaying characters
   write(SCAN_LIMIT, NUM_DIGITS - 1); // set scan limit to number of available digits 
   write(INTENSITY, 0xf); // set intensity of the display (0x0 - 0xf)

   clearDigits(); // clear all data saved to the display before
}


void MAX7219::write(uint8_t addr, uint8_t data){
  digitalWrite(CS, LOW);
  shiftOut(DIN, CLK, MSBFIRST, addr);
  shiftOut(DIN, CLK, MSBFIRST, data);
  digitalWrite(CS, HIGH);
}

void MAX7219::writeDigit(const uint8_t digit, const uint8_t value, const bool dp = false){
  static const uint8_t DIGITS[] = {0b01111110, 0b00110000, 0b01101101, 0b01111001, 0b00110011, 0b01011011, 0b01011111, 0b01110000, 0b01111111, 0b01111011};
  if (value < 0 || value > 9 || digit < 0 || digit >= NUM_DIGITS) write(digit + 1, 0x00);;
  write(digit + 1, DIGITS[value] | dp << 7);
  
}

void MAX7219::writeCustom(const uint8_t digit, const uint8_t value, const bool dp = false){
  write(digit + 1, value | (dp << 7));
}

void MAX7219::clearDigits(){
  for (uint8_t i = 0; i < NUM_DIGITS; i++){
    write(i + 1, 0x00);
  }
}

void MAX7219::testAllOn(uint32_t del){
  write(DISPLAY_TEST, 0x1);
  delay(del);
  write(DISPLAY_TEST, 0x0);
}

void MAX7219::writeFloat(float num){
   ((int)num / 100 % 10) ? writeDigit(4, (int)num / 100 % 10) : write(DIGIT_4, 0x00); // display only if the digit is not zero
   writeDigit(3,(int)num / 10 % 10);
   writeDigit(2, (int)num % 10, true);  // display with decimal point
   writeDigit(1, (int)(num * 10) % 10);
   writeDigit(0, (int)(num * 100) % 10);
}
