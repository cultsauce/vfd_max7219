
#include "max7219.hpp"

MAX7219::MAX7219(const uint8_t ch_din = 2, const uint8_t ch_cs = 3, const uint8_t ch_clk = 4){
   din = ch_din;
   cs = ch_cs;
   clk = ch_clk;
   pinMode(din, OUTPUT);
   pinMode(cs, OUTPUT);
   pinMode(clk, OUTPUT);
   digitalWrite(cs, HIGH);
   
   write(SHUTDOWN, 0x1); /* exit shutdown mode */
   write(DISPLAY_TEST, 0x0); /* return to normal op after test mode */
   write(DECODE_MODE, NO_DECODE); /* set no decode mode to all digits - allows for also displaying characters */
   write(SCAN_LIMIT, NUM_DIGITS - 1); /* set scan limit to number of available digits  */
   write(INTENSITY, 0xf); /* set intensity of the display (0x0 - 0xf) */

   clearDigits(); /* clear all data saved to the display before */
}


void MAX7219::write(uint8_t addr, uint8_t data){
  digitalWrite(cs, LOW);
  shiftOut(din, clk, MSBFIRST, addr);
  shiftOut(din, clk, MSBFIRST, data);
  digitalWrite(cs, HIGH);
}

void MAX7219::writeCharacter(const uint8_t digit, const uint8_t value, const bool dp = false){
  static uint8_t CHARS[256] = {0b01111110, 0b00110000, 0b01101101, 0b01111001, 0b00110011, 0b01011011, 0b01011111, 0b01110000, 0b01111111, 0b01111011};
  CHARS['A'] = 0b01110111; CHARS['B'] = 0b00011111; CHARS['C'] = 0b01001110; CHARS['D'] = 0b00111101; CHARS['E'] = 0b01001111; CHARS['F'] = 0b01000111; CHARS['G'] = 0b01011110; CHARS['H'] = 0b00010111;
  CHARS['I'] = 0b00000110; CHARS['J'] = 0b00111100; CHARS['K'] = 0b01010111; CHARS['L'] = 0b00001110; CHARS['M'] = CHARS['N'] = 0b01110110; CHARS['O'] = 0b01111110; CHARS['P'] = 0b01100111; 
  CHARS['Q'] = 0b01110011; CHARS['R'] = 0b01100110; CHARS['S'] = 0b01011011; CHARS['T'] = 0b00001111; CHARS['U'] = CHARS['V'] = CHARS['W'] = 0b00111110 ; 
  CHARS['X'] = 0b00110111; CHARS['Y'] = 0b00100111; CHARS['Z'] = 0b01101101;
  if (value >= 48 && value <= 57 ) write(digit + 1, CHARS[value - 48] | dp << 7);
  else if (value >= 'a' && value <= 'z') write(digit + 1, CHARS[value - ('a' - 'A')] | dp << 7);
  else write(digit + 1, CHARS[value] | dp << 7);
  
}

/* VALUE: MSB  -> DP A B C D E F G  <- LSB */
void MAX7219::writeCustom(const uint8_t digit, const uint8_t value, const bool dp = false){
  write(digit + 1, value | (dp << 7));
}

void MAX7219::clearDigits(){
  for (uint8_t i = 0; i < NUM_DIGITS; i++){
    write(i + 1, 0x00);
  }
}

void MAX7219::writeString(const char str[]){
  auto len = strlen(str);
  if (len > NUM_DIGITS) len = NUM_DIGITS;

  for (uint8_t i = 0; i < len; i++){
     writeCharacter(i, str[i]);
  }
}

void MAX7219::writeFloat(float num){

    bool blanked = true;
    const uint8_t dp_pos = 3;
    for (uint8_t i = 0, divide = pow(10, dp_pos - 1); i < dp_pos; i++, divide /= 10){
      ((int)num / divide % 10 || !blanked) ? blanked = false, writeCharacter(i, (int)num / divide % 10) : writeCustom(i, 0x00); /* display only if the digit is not zero */
      if (dp_pos - 1 == i) writeCharacter(i, (int)num / divide % 10, true);
    }
    for (uint8_t i = dp_pos, times = 10; i < NUM_DIGITS; i++, times *= 10){
      writeCharacter(i, (int)(num * times) % 10);
    }
}
