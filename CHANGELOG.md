# Changelog

## [1.2.1] - 2022-11-29
## Fixed
- Fix compilation error when making .uf2 target in demo

## [1.2.0] - 2021-06-15
### Added
- Ability to wait for everything to be sent to display with the `TM1637_wait()`
  function. 
- Ability to get current display brightness with the `TM1637_get_brightness()`
  function.
- Refresh the state machine clock with `TM1637_refresh_frequency()`.
### Changed
- `TM1637_display_word()` can now also display a colon.
- The state machine (that sends data to TM1637) clockspeed is now adjusted 
  for different system clocks.
## Fixed
- Changelog dates now has correct year.

## [1.1.0] - 2021-05-01
### Added
- Ability to display words with the
  [`TM1637_display_word()`](https://wahlencraft.github.io/TM1637-pico/html/PicoTM1637_8h.html#aa79d2b795727374b09dd278218381ecb)
  function.
- A [documentation](https://wahlencraft.github.io/TM1637-pico/html/PicoTM1637_8h.html)
  of the API.

## [1.0.0] - 2021-04-24
