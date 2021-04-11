TM1637-pico
=============
#### Control TM1637 based 7-segment display from raspberry pi pico.

<p align="center" width="100%">
  <img src="image.jpg" alt="drawing" width="33%" align="c"/>
</p>

## Features
* Display a positive 4 digit number or a negative 3 digit number.
* Display two positive 2 digit numbers with (or without) a colon in between.
* You can choose if you want leading zeros in both of the above cases.
* Display 4 or 2 digits of raw data.
* Choose the display brightness (on a 8 step scale).

The actual communication with the display is handled by the picos pio 
functionality. When a display function is called, some data is very quickly put 
into a buffer that the pio has access to. The pio will then, one bit at a time 
send the data to the TM1637 display. If you attempt to display too much 
in a short period of time, the buffer will become full. This will block further
execution in the main program if even more data is sent to the display. 

This library will only work with the 4 digit version of the display.

To get started have a look at the `examples/demo.c` file. For a full list of 
functions please refer to `PicoTM1637.h`

## Setup
1. Download the files from this repository and put them in a folder somewhere.
2. In your CMAKE file:
   `include(/path/to/folder/TM1637-pico/PicoTM1637.cmake)` and
   `target_link_libraries(your_target PicoTM1637)`
3. In your c file:
   `#include <PicoTM1637.h>`
## Wiring
| TM1637 | Target |
|:------:|:------:|
| CLK | A GPIO pin on the pico |
| DIO | A GPIO pin on the pico |
| VCC | 5V |
| GND | Ground |

## Try the demo
Make sure `CLK_PIN` and `PIO_PIN` in `demo.c` is the same as what you have
connected to your pico. Then build the project:
```
cd examples
mkdir build
cd build
cmake ..
make -j4
```
Then exporting to pico can be done
  * Using uart: 
    ```
    openocd -f interface/raspberrypi-swd.cfg -f target/rp2040.cfg -c "program TM1637_demo.elf verify reset exit"
    ```
  * Over USB (Linux):

    Enable extra output (.uf2) in `CMakeFiles.txt` (and rebuild)    

    Find what block device raspberry pi pico is connected to
    ```
    sudo fdisk -l
    ```
    Mount to file system
    ```
    mkdir /media/pico    
    sudo mount /dev/sdx1 /media/pico
    ```
    *Where x is a letter from a, b, c, ...*
    
    Copy `TM1637_demo.uf2` file to pico, unmount
    ```
    cp TM1637_demo.uf2 /media/pico
    umount /dev/sdx1
    ``` 
