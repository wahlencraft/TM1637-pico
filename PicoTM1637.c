#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <pico/stdlib.h>
#include <hardware/pio.h>
#include <PicoTM1637.h>
#include <PicoTM1637.pio.h>

#define SET_WRITEMODE 0x40
#define WRITE_ADDRESS 0xc0
#define BRIGHTNESS_BASE 0x88
#define MAX_DIGITS 4

/* Global variables */
PIO pio;
uint clkPin, dioPin, sm, brightness = 0;

const uint8_t digitToSegment[] = {
  0b00111111,    // 0
  0b00000110,    // 1
  0b01011011,    // 2
  0b01001111,    // 3
  0b01100110,    // 4
  0b01101101,    // 5
  0b01111101,    // 6
  0b00000111,    // 7
  0b01111111,    // 8
  0b01101111,    // 9
  0b01110111,    // A
  0b01111100,    // b
  0b00111001,    // C
  0b01011110,    // d
  0b01111001,    // E
  0b01110001     // F
  };


void TM1637_init(uint clk, uint dio) {
  // Choose which PIO instance to use (there are two instances)
  pio = pio0;
  sm = 0;

  clkPin = clk;
  dioPin = dio;

  // Our assembled program needs to be loaded into this PIO's instruction
  // memory. This SDK function will find a location (offset) in the
  // instruction memory where there is enough space for our program. We need
  // to remember this location!
  uint offset = pio_add_program(pio, &tm1637_program);

  // Find a free state machine on our chosen PIO (erroring if there are
  // none). Configure it to run our program, and start it, using the
  // helper function we included in our .pio file.
  tm1637_program_init(pio, sm, offset, clkPin, dioPin);

  stdio_init_all();
}

void TM1637_on() {
  pio_sm_put_blocking(pio, sm, BRIGHTNESS_BASE + brightness);
}

void TM1637_put_2_bytes(uint start_pos, uint data) {
  uint address = WRITE_ADDRESS + start_pos;
  pio_sm_put_blocking(pio, sm, (data << 16) + (address << 8) +  SET_WRITEMODE);
}

void TM1637_put_4_bytes(uint start_pos, uint data) {
  uint address = WRITE_ADDRESS + start_pos;
  uint data1 = data & 0xffff;  // first two bytes
  uint data2 = data >> 16;     // last two bytes
  pio_sm_put_blocking(pio, sm, (data1 << 16) + (address << 8) + SET_WRITEMODE);
  pio_sm_put_blocking(pio, sm, data2 << 16);  // I have no idea why this has to be shifted
}

/* Convert a number to something readable for the 'put bytes' functions.
 *
 * Warning, input must not be more than 4 digits. */
unsigned int num_to_hex(int num) {
  unsigned int hex = 0x0, seg;
  while (num) {
    seg = digitToSegment[num % 10];  // extract last digit as 7 segment byte
    num /= 10;  // remove last digit from num
    hex = seg + (hex << 8);  // Put new segment to the right in hex
  }
  return hex;
}

unsigned int process_hex(uint hex) {
}

void TM1637_display(int number, bool leadingZeros) { 
  // Determine length of number
  int len = 0;
  int numberCopy = number;
  while (numberCopy) {
    len++;
    numberCopy /= 10;
  }
  if (len > 4) {
    printf("Warning number %d too long\n", number);
    len = 4;
    // least signigicant digits will be cut of in num_to_hex
  }

  // Get hex
  unsigned int hex = num_to_hex(number);
  unsigned int startPos = 0;
  if (leadingZeros && (len < MAX_DIGITS)) {
    for (int i=len; i<MAX_DIGITS; i++) {
      hex = (hex << 8) + digitToSegment[0];
    }
  } else if (number == 0) {
    // Signular case
    hex = digitToSegment[0];
    startPos = MAX_DIGITS - 1;
  } else { 
    startPos = MAX_DIGITS - len;
  }
  
  // Display number
  TM1637_put_4_bytes(startPos, hex);
  TM1637_on();
}
    
/* Private base for TM1637_display_left and TM1637_display_right */
void display_half_base(int num, bool leadingZeros, bool useColon, int startPos) {
  uint hex;
  if (num == 0) {
    // Singular case
    hex = digitToSegment[0];
  } else {
    hex = num_to_hex(num);
  }

  int numDiv = num / 10;  // determine length of number
  
  if (!numDiv && leadingZeros) {
    // num is just 1 digit and we want leadning zeros
    hex = digitToSegment[0] + (hex << 8);
  } else if (!numDiv) {
    // num is just 1 digit  
    hex = hex << 8;
  }

  if (useColon) {
    hex |= 0x8000;
  }
  
  // Display digits
  TM1637_put_2_bytes(startPos, hex);
  TM1637_on();
}

void TM1637_display_left(int num, bool leadingZeros, bool useColon) {
  display_half_base(num, leadingZeros, useColon, 0);
}

void TM1637_display_right(int num, bool leadingZeros) {
  display_half_base(num, leadingZeros, false, 2);
}

void TM1637_clear() {
  pio_sm_put_blocking(pio, sm, 0x80);
  pio_sm_put_blocking(pio, sm, 0xc040);
  pio_sm_put_blocking(pio, sm, 0x0);
}
