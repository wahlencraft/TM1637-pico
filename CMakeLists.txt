cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)

project(
  TM1637_Pico
  LANGUAGES C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
pico_sdk_init()

add_executable(TM1637_pico_test)

pico_generate_pio_header(TM1637_pico_test ${CMAKE_CURRENT_LIST_DIR}/TM1637.pio)

target_sources(TM1637_pico_test PRIVATE TM1637.c)

target_link_libraries(TM1637_pico_test PRIVATE
  pico_stdlib
  hardware_pio)

# Uncomment for .uf2 output
#pico_add_extra_outputs(TM1637_pico_test)
