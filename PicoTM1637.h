#ifndef TM1637_H_
#define TM1637_H_

#include <stdbool.h>

/* Initiate TM1637 display
 *
 * Takes the clock and data pin names (integers) as input. */
void TM1637_init(uint clk, uint dio);

/* Put one or two bytes on TM1637 display. 
 *
 * start_pos: uint from 0 to 3 where 0 is to the left
 * data: raw data for one or two bytes, the least significant byte will be put 
 *       to the left. 
 *        
 *       For example the two bytes 0x3f05 = 0b0011111100000110 will 
 *       result in: 10 */
void TM1637_put_2_bytes(uint start_pos, uint data);

/* Put one to four bytes on TM1637 display. 
 *
 * start_pos: uint from 0 to 3 where 0 is to the left
 * data: raw data for one to four bytes, the least significant byte will be put 
 *       to the left */
void TM1637_put_4_bytes(uint start_pos, uint data);

/* Display a positive number with 4 digits or a negative number with 3 digits.
 *
 * The least significat digit will be put to the right.
 * Arguments:
 *  int number: The number to display.
 *  bool leadingZeros: If leading Zeros should be displayed or not. */
void TM1637_display(int number, bool leadingZeros);    
  
/* Display a posetive number on the 2 leftmost digits on the display. A colon is
 * by default shown, to turn this off use TM1637_set_colon(false)
 *
 * !!! Avoid using this function since it will cause the right side to flicker. 
 * Instead use TM1637_display_both. */
void TM1637_display_left(int number, bool leadingZeros);

/* Display a positive number on the 2 rightmost digits on the display. */
void TM1637_display_right(int number, bool leadingZeros);

/* Display two (two digit positive) numbers on the display. By default there 
 * will be a colon between them. Disable this behaviour with 
 * TM1637_set_colon(false); */
void TM1637_display_both(int leftNumber, int rightNumber, bool leadingZeros);

/* Turn the colon led on or off. Default is on.
 *
 * Note that this not immeidiatly updated, but will be next time something is 
 * displayd with a colon supporting function. */
void TM1637_set_colon(bool on);

/* Set the display brightness.
 *
 * val can be a value from 0 to 7. 
 *
 * Display brightness is not immeidiatly updated, but next time something is
 * displayed it will have the new brightness. */
void TM1637_set_brightness(int val);

/* Clear the display. */
void TM1637_clear();
 
#endif // TM1637_H_
