/** @file */

#ifndef TM1637_H_
#define TM1637_H_

#include <stdbool.h>

/** 
 * Initiate TM1637 display
 *
 * @param clk is the clock GPIO pin number. 
 * @param dio is the data GPIO pin number. */
void TM1637_init(uint clk, uint dio);

/** Display one or two bytes of raw data on the display. 
 *
 * @param startPos The digit index to start at. Ranges from `0` to `3`, where 
 *        `0` is to the left
 * @param data The data for one or two bytes, the least significant byte will be 
 *        put to the left. 
 *        
 * For example `TM1637_put_2_bytes(2, 0x3f05)` will show the number 10 on the
 * right half of the display. */
void TM1637_put_2_bytes(uint startPos, uint data);

/** Display one to four bytes of raw data on the display. 
 *
 * @param startPos The digit index to start at. Ranges from `0` to `3`, where 
 *        `0` is to the left
 * @param data The data for one to four bytes, the least significant byte will 
 *        be put to the left. */
void TM1637_put_4_bytes(uint startPos, uint data);

/** Display a positive number with 4 digits or a negative number with 3 digits.
 * 
 *  @param number The number to display.
 *  @param leadingZeros If leading zeros should be displayed or not. */
void TM1637_display(int number, bool leadingZeros);    
  
/** Display a string of characters.
 *
 * @param word The word to display. May be at most 4 letters long.
 * @param leftAlign true if left alignment is desired, false for right 
 * alignment. Has no effect if all 4 chars are used. 
 *
 * All English alphabet letters are supported in lower or upper case. If
 * the desired case is not found, the other will be displayed instead.
 * If a character is not found at all it will be replaced by white space.
 * For a full list of supported characters, as well as their hexadecimal
 * representation please look at char_table.txt.
 *
 * You can also include a colon (:) in the string. This character is not
 * counted in the word length as the colon internaly belongs to the character
 * before it. Will only work if aligned with the colon spot on the display. */
void TM1637_display_word(char *word, bool leftAlign);

/** Display a positive number on the 2 leftmost digits on the display. 
 *
 * A colon is by default shown. To turn this off use 
 * TM1637_set_colon(bool false).
 *
 * __Avoid using this function.__ It will cause the right side to flicker. 
 * Instead use TM1637_display_both(). */
void TM1637_display_left(int number, bool leadingZeros);

/** Display a positive number on the 2 rightmost digits on the display. 
 * 
 * A colon is by default shown. To turn this off use 
 * TM1637_set_colon(bool false). */
void TM1637_display_right(int number, bool leadingZeros);

/** Display two (2 digit positive) numbers on the display. 
 * 
 * A colon is by default shown in between. To turn this off use 
 * TM1637_set_colon(bool false). */
void TM1637_display_both(int leftNumber, int rightNumber, bool leadingZeros);

/** Turn the colon led on or off. Default is on.
 *
 * The colon is not immediately updated, but will be next time something is 
 * displayed (with a colon supporting function). */
void TM1637_set_colon(bool on);

/** Set the display brightness.
 * 
 * Display brightness is not immediately updated, but next time something is
 * displayed it will have the new brightness.
 * @param val can be a value from `0` to `7`. The default brightness is 0. */
void TM1637_set_brightness(int val);

/** Get the current brightness level.
 *
 * Returns an integer from 0 to 7 represenging current brightness level. */
int TM1637_get_brightness();

/** Clear the display. */
void TM1637_clear();

/** Reset the frequency at which TM1637 recives data.
 *
 * Call this function in case the system clock frequency has been changed since
 * the call of `TM1637_init()`. */
void TM1637_refresh_frequency();

/** Wait for the TM1637 display.
 * 
 * When calling a function such as TM1637_display() the result is actually not
 * immediately sent to the display. This is because the PIO hardware on the pico
 * is running slower than the CPU. Usually this is fine, it's fast enough to
 * appear instant, but sometimes you want to wait for it to finish. For example
 * if you will enter sleep mode and want the display to update beforehand. */
void TM1637_wait();

#endif // TM1637_H_
