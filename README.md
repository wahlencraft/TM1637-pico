TM1637-pico
=============
#### Controll TM1637 based 7-segment display from rasberry pi pico.

## Setup
Build project with
```
mkdir build
cd build
cmake ..
make -j4
```
Then exporting to pico can be done
  * Using uart: 
    ```
    openocd -f interface/raspberrypi-swd.cfg -f target/rp2040.cfg -c "program program_file.elf verify reset exit"
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
    
    Copy .uf2 file to pico, unmount
    ```
    cp program_file.uf2 /media/pico
    umount /dev/sdx1
    ``` 
