add_executable(
  rgb_keypad_JHGE
  demo.cpp
)

# Pull in pico libraries that we need
target_link_libraries(rgb_keypad_JHGE pico_stdlib pico_rgb_keypad)

# create map/bin/hex file etc.
pico_add_extra_outputs(rgb_keypad_JHGE)
