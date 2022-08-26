#include <Arduino.h>

#define DIN 2
#define CS 3
#define CLK 4

#define SHUTDOWN 0xC
#define DISPLAY_TEST 0xF
#define INTENSITY 0xA
#define SCAN_LIMIT 0xB
#define DECODE_MODE 0x9
#define DIGIT_0 0x1
#define DIGIT_1 0x2
#define DIGIT_2 0x3
#define DIGIT_3 0x4
#define DIGIT_4 0x5
#define DIGIT_5 0x6
#define DIGIT_6 0x7
#define DIGIT_7 0x8


#define CODE_B_ALL 0xff
#define NO_DECODE 0x00

#define NUM_DIGITS 5


class MAX7219{
    private:
    uint8_t din, cs, clk; /* by default, the values are pins 2, 3, 4 */
    void write(uint8_t addr, uint8_t data);
    public:
    MAX7219(const uint8_t ch_din = 2, const uint8_t ch_cs = 3, const uint8_t ch_clk = 4);
    void writeCharacter(const uint8_t digit, const uint8_t value, const bool dp = false);
    void writeCustom(const uint8_t digit, const uint8_t value, const bool dp = false);
    void writeString(const char str[]);
    void writeFloat(float num);
    void clearDigits();
};
