#include <stdlib.h>
#include <stdio.h>
#include <PicoTM1637.h>
#include <pico/stdlib.h>
#define CLK_PIN 26
#define DIO_PIN 27

int main()
{   
    TM1637_init(CLK_PIN, DIO_PIN);  
    TM1637_set_brightness(7);
    TM1637_display(8888, true); 
    sleep_ms(1000);
    TM1637_set_brightness(0);    // brightness is not updated automatically, 
    TM1637_display(8888, true);  // something new needs to be displayed.
    sleep_ms(1000);
    
    TM1637_clear();
    sleep_ms(500);
    
    // Count up from 0
    int count = 0;
    while(count <= 200) {
      printf("TEST\n");
      TM1637_display(count, false);
      count++;
      // The display can not update too often. So even though there is no
      // sleep, this will take a couple of moments.
    }
    
    sleep_ms(1000);

    // Demo a clock5
    int seconds = 0;
    int minutes = 0;
    TM1637_display_left(minutes, true);
    TM1637_display_right(seconds, true);
    while (true) {
      sleep_ms(1000);
      seconds++;
      if (seconds == 60) {
        seconds = 0;
        minutes++;
        TM1637_display_left(minutes, true);
      }
      TM1637_display_right(seconds, true);
    }


    return 0;
} 
