add_library(PicoTM1637 INTERFACE)
target_sources(PicoTM1637 INTERFACE
  ${CMAKE_CURRENT_LIST_DIR}/PicoTM1637.c
)

pico_generate_pio_header(PicoTM1637 ${CMAKE_CURRENT_LIST_DIR}/PicoTM1637.pio)

target_include_directories(PicoTM1637 INTERFACE ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(PicoTM1637 INTERFACE pico_stdlib hardware_pio)
