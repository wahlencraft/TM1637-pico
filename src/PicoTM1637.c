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
bool colon = true;

static const uint8_t digitToSegment[] = {
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

static const uint8_t segmentsArr[] = {
#include "../data/char_table.txt"
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

void set_display_on() {
  pio_sm_put_blocking(pio, sm, BRIGHTNESS_BASE + brightness);
}

void TM1637_put_2_bytes(uint start_pos, uint data) {
  uint address = WRITE_ADDRESS + start_pos;
  pio_sm_put_blocking(pio, sm, (data << 16) + (address << 8) +  SET_WRITEMODE);
  set_display_on();
}

void TM1637_put_4_bytes(uint start_pos, uint data) {
  uint address = WRITE_ADDRESS + start_pos;
  uint data1 = data & 0xffff;  // first two bytes
  uint data2 = data >> 16;     // last two bytes
  pio_sm_put_blocking(pio, sm, (data1 << 16) + (address << 8) + SET_WRITEMODE);
  pio_sm_put_blocking(pio, sm, data2 << 16);  // I have no idea why this has to be shifted
  set_display_on();
}

/* Convert a number to something readable for the 'put bytes' functions.
 *
 * Warning, input must not be more than 4 digits. Then least significant digits
 * will be cut of.
 *
 * You can also cut of parts with a bitmask. If bitMask = 0 nothing will
 * happen.*/
unsigned int num_to_hex(int num, uint bitMask) {
  unsigned int hex = 0x0, seg;
  if (num == 0) {
    // singular case
    hex = digitToSegment[0];
  } else {
    while (num) {
      seg = digitToSegment[num % 10];  // extract last digit as 7 segment byte
      num /= 10;  // remove last digit from num
      hex = seg + (hex << 8);  // Put new segment to the right in hex
    }
  }
  if (bitMask) {
    hex &= bitMask;
  }
  return hex;
}


/* Helper for converting a char to the 7-segment representation. */
uint fetch_char_encoding(char charToFind) {
  short i = 0;
  uint8_t c = 1;
  while (c != '\0') {
    c = segmentsArr[i];
    if (c == charToFind) {
      return segmentsArr[i+1];
    }
    i += 2;  // only every other element is a characer to look for
  }
  // Could not find charToFind, try to find lower/upper case of same letter.
  // note that every english letter exists in one of these forms.
  if ((0x40 < charToFind) && (charToFind < 0x5b)) {
    // this is upper case
    return fetch_char_encoding(charToFind + 0x20);
  } else if ((0x60 < charToFind) && (charToFind < 0x7b)) {
    // this is lower case
    return fetch_char_encoding(charToFind - 0x20);
  }
  // Still not found, return empty
  printf("Warning! char %c not found by fetch_char_encoding()\n", charToFind);
  return fetch_char_encoding(' '); 
}

void TM1637_display(int number, bool leadingZeros) { 
  // Is number positive or negative?
  int isPositive;
  int isNegative;
  if (number >= 0) {
    isPositive = 1;
  } else {
    isPositive = 0;
    number = -1*number;
  }
  // Determine length of number
  int len = 0;
  int numberCopy = number;
  while (numberCopy) {
    len++;
    numberCopy /= 10;
  }
  if (len > 3 + isPositive) {
    printf("Warning number %d too long\n", number);
    len = 3 + isPositive;
    // least signigicant digits will be lost
  }

  // Get hex
  unsigned int hex = num_to_hex(number, 0);
  if (!isPositive) {
    hex = (hex << 8) + 0x40;  // add a negative sign
    len++;  // count negative sign in length
  } 
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
}

void TM1637_display_word(char *word, bool leftAlign) {
  // Find the binary representation of the word
  uint bin = 0;
  int i = 0;
  char c = word[0];
  int col = -1;
  int len = 0;
  while ((c != '\0') && (len < MAX_DIGITS)) {
    if (c == ':') {
      // remember colon
      col = i - 1;
      if ((col < 0) || (col > MAX_DIGITS)) {
        printf("Warning, TM1637_display_word colon out of bounds.\n");
      }
    } else {
      bin += (fetch_char_encoding(c)<< 8*len);
      len++;
    }
    i++;
    c = word[i];
  }
  // Display word
  uint startIndex;
  if (leftAlign) {
    startIndex = 0;
  } else {
    startIndex = MAX_DIGITS - len;
  }
  // add colon
  if (col >= 0) {
    bin |= (0x80 << col*8);
  }
  printf("0x%x\n", bin);
  TM1637_put_4_bytes(startIndex, bin);
}

/* Helper for getting the segment representation for a 2 digit number. */
uint two_digit_to_segment(int num, bool leadingZeros, bool useColon) {
  uint hex = num_to_hex(num, 0xffff);

  int numDiv = num / 10;  // determine length of number
  
  if (!numDiv && leadingZeros) {
    // num is just 1 digit and we want leadning zeros
    hex = digitToSegment[0] + (hex << 8);
  } else if (!numDiv) {
    // num is just 1 digit  
    hex = hex << 8;
  }

  if(useColon) {
    hex |= 0x8000;
  }
  
  return hex;
}

void TM1637_display_left(int num, bool leadingZeros) {
  uint hex = two_digit_to_segment(num, leadingZeros, colon);  
  TM1637_put_2_bytes(0, hex);
}

void TM1637_display_right(int num, bool leadingZeros) {
  uint hex = two_digit_to_segment(num, leadingZeros, false);
  TM1637_put_2_bytes(2, hex);
}

void TM1637_display_both(int leftNum, int rightNum, bool leadingZeros) {
  uint leftHex = two_digit_to_segment(leftNum, leadingZeros, colon);
  uint rightHex = two_digit_to_segment(rightNum, leadingZeros, false);  

  uint hex = leftHex + (rightHex << 16);
  TM1637_put_4_bytes(0, hex);
}

void TM1637_set_colon(bool on) {
  colon = on;
}

void TM1637_set_brightness(int value) {
  brightness = value;
}

void TM1637_clear() {
  pio_sm_put_blocking(pio, sm, 0x80);
  pio_sm_put_blocking(pio, sm, 0xc040);
  pio_sm_put_blocking(pio, sm, 0x0);
}

void TM1637_wait() {
  while (!pio_sm_is_tx_fifo_empty(pio, sm)) {
    // Wait while there is something in tx fifo (on the way out). 
    if (!pio_sm_is_rx_fifo_empty(pio, sm)) {
      // Found some response from the sm. Since the tx still isn't empty
      // we just want to throw this away.
      pio_sm_get_blocking(pio, sm);
    }
  }
  int d = 0;
  while (d != 1) {
    // Now there is nothing in tx fifo, but there might still be work to do.
    // The status 1 is sent as a response when done.
    d = pio_sm_get_blocking(pio, sm);
  }
  uart_default_tx_wait_blocking();
}
