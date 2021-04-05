#include <stdlib.h>
#include <stdio.h>
#include <PicoTM1637.h>
#include <pico/stdlib.h>
#define CLK_PIN 26
#define DIO_PIN 27

int main()
{   
    TM1637_init(CLK_PIN, DIO_PIN);  
    TM1637_display(8888, true); 
    sleep_ms(500);
    
    TM1637_clear();
    sleep_ms(500);

    // Demo a clock, 02:50 -> 03:05
    int seconds = 50;
    int minutes = 2;
    TM1637_display_left(minutes, true, true);
    TM1637_display_right(seconds, true);
    while (minutes < 3 || seconds < 5) {
      sleep_ms(1000);
      seconds++;
      if (seconds == 60) {
        seconds = 0;
        minutes++;
        TM1637_display_left(minutes, true, true);
      }
      TM1637_display_right(seconds, true);
    }

    sleep_ms(1000);

    // Count up from 0
    int count = 0;
    while(true) {
      printf("TEST\n");
      TM1637_display(count, false);
      count++;
      sleep_ms(1000);
    }

    return 0;
} 
