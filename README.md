TM1637-pico
=============
#### Controll TM1637 based 7-segment display from rasberry pi pico.

## Setup
1. Download the files from this repository and put them in a folder someware.
2. In your CMAKE file:
   `include(/path/to/folder/TM1637-pico/PicoTM1637.cmake)`
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
  * Over USB (linux):

    Enable extra output (.uf2) in `CMakeFiles.txt` (and rebuild)    

    Find what block device rasberry pi pico is connected to
    ```
    sudo fdisk -l
    ```
    Mount to filesystem
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
