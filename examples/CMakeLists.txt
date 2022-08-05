cmake_minimum_required(VERSION 3.13)

# Pull in Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(
  TM1637_demo
  LANGUAGES C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
pico_sdk_init()

include("../PicoTM1637.cmake")

add_executable(TM1637_demo demo.c)

target_link_libraries(TM1637_demo PicoTM1637 pico_stdlib)

# Uncomment for .uf2 output
#pico_add_extra_outputs(TM1637_demo)
