/* 
 *  ANOTHER MAX7219 LIBRARY 
    @cultsauce 2022
    By default, the pins used for SPI connection are: DIN = 2, CS = 3, CLK = 4
 *  
 */



#include "max7219.hpp"

MAX7219 vfd;


void setup() {
}

void loop() {
  
  for (auto k = 0; k < 6; k++) {

    for (auto m = 0; m < 2; m++) {
      auto x = 0b10;
      for (auto i = 0; i < 6; i++) {
        for (auto j = k; j < 5; j++) {
          vfd.writeCustom(j, x);
        }
        x <<= 1;
        delay(100);
      }
    }
    vfd.writeCharacter(k, k, true);
  }

  vfd.writeCharacter(0, 0, true);
  for (int j = 0; j < 2; j++) {
    vfd.writeCharacter(1, j, true);
    for (int k = 0; k < 9; k++) {
      vfd.writeCharacter(2, k, true);
      for (int l = 0; l < 9; l++) {
        vfd.writeCharacter(3, l, true);
        for (int m = 0; m < 9; m++) {
          vfd.writeCharacter(4, m, true);
          delay(5);
        }
      }
    }
  }


  for (int j = 0; j < 2; j++) {
    auto x = 0b01000000;
    for (auto i = 0; i < 5; i++) {
      vfd.writeCustom(i, x);
      for (auto h = 0; h < 5; h++) {
        if (h != i) {
          vfd.writeCustom(h, 0x00);
        }
      }
      delay(80);
      vfd.writeCustom(i, 1 << 7);
    }
    for (auto i = 0; i < 2; i++) {
      vfd.writeCustom(4, x >> i);
      delay(80);
    }
    x = 0b00001000;
    for (auto i = 0; i < 5; i++) {
      vfd.writeCustom(4 - i, x);
      for (auto h = 0; h < 5; h++) {
        if (h != (4 - i)) {
          vfd.writeCustom(h, 0x00);
        }
      }
      delay(80);
    }
    for (auto i = 0; i < 2; i++) {
      vfd.writeCustom(0, x >> i);
      delay(80);
    }
  }

  vfd.writeString("abcde");
  delay(2000);
  vfd.writeString("fghij");
  delay(2000);
  vfd.writeString("klmno");
  delay(2000);
  vfd.writeString("pqrst");
  delay(2000);
  vfd.writeString("uvxyz");
  delay(2000);
  vfd.writeString("01234");
  delay(2000);
  vfd.writeString("56789");
  delay(2000);

}
