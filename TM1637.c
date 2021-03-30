#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
// Our assembled program:
#include "TM1637.pio.h"

#define CLK_PIN 26
#define DIO_PIN 27
#define SET_WRITEMODE 0x40
#define WRITE_ADDRESS 0xc0

/* Global variables */
PIO pio;
uint clkPin, dioPin, sm;

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


/* Initiate TM1637 display
 *
 * Takes the clock and data pin names (integers) as input.
*/
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

/* Put one or two bytes on TM1637 display. 
 *
 * start_pos: uint from 0 to 3 where 0 is to the left
 * data: raw data for one or two bytes, the first byte will be but to the left
 */
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
  


int main()
{
    const uint LED_PIN = 25;
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    TM1637_init(CLK_PIN, DIO_PIN);


    // Light up all segments:
    //pio_sm_put_blocking(pio, 0, 0xffc0448a);
    //pio_sm_put_blocking(pio, 0, 0xffc1448a);
    //pio_sm_put_blocking(pio, 0, 0xffc2448a);
    //pio_sm_put_blocking(pio, 0, 0xffc3448a);

    //sleep_ms(3000);

    // Wait 3 seconds, then print 123 with max brightness:    
    pio_sm_put_blocking(pio, sm, 0x80);  // Clear display
    pio_sm_put_blocking(pio, sm, 0xc040);
    pio_sm_put_blocking(pio, sm, 0x0);

    sleep_ms(500);
    //pio_sm_put(pio, sm, 0x3f3fc140);
    //pio_sm_put(pio, sm, 0x773f0000);
    
    TM1637_put_4_bytes(0, 0x4f4fbf06);
    //sleep_ms(100);
    pio_sm_put_blocking(pio, sm, 0x8a);

    // just wait here and blink LED like usual...
    while(1) {
        gpio_put(LED_PIN, 1);
        sleep_ms(500);
        gpio_put(LED_PIN, 0);
        sleep_ms(500);
    }

    return 0;
}
