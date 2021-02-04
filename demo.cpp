#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "pico/stdlib.h"

#include "pico_rgb_keypad.hpp"

#include <time.h>

using namespace pimoroni;

// HSV Conversion expects float inputs in the range of 0.00-1.00 for each channel
// Outputs are rgb in the range 0-255 for each channel
//void from_hsv(float h, float s, float v, uint8_t &r, uint8_t &g, uint8_t &b) {
uint8_t r, g, b;
void from_hsv(float h, float s, float v) {
  float i = floor(h * 6.0f);
  float f = h * 6.0f - i;
  v *= 255.0f;
  uint8_t p = v * (1.0f - s);
  uint8_t q = v * (1.0f - f * s);
  uint8_t t = v * (1.0f - (1.0f - f) * s);

  switch (int(i) % 6) {
    case 0: r = v; g = t; b = p; break;
    case 1: r = q; g = v; b = p; break;
    case 2: r = p; g = v; b = t; break;
    case 3: r = p; g = q; b = v; break;
    case 4: r = t; g = p; b = v; break;
    case 5: r = v; g = p; b = q; break;
  }
}

long random(long howbig)
{
  if (howbig == 0) {
    return 0;
  }
  return rand() % howbig;
}

PicoRGBKeypad pico_keypad;

int main() {
  pico_keypad.init();
  pico_keypad.set_brightness(0.3f);

  uint16_t lit = 0;
  uint16_t last_button_states = 0;
  uint8_t colour_index = 0;

	int m[16][256];
	srand(time(NULL));   // use current time as seed for random generator
	//int s = rand();      // Returns a pseudo-random integer between 0 and RAND_MAX.
	int s = random(8);			//liefert zahlenwerte von 0 bis 7!
	
	pico_keypad.clear();
  while(true) {
		s = random(1000);
		from_hsv(s/1000.0,1,1);	//setzt direkt die globalen variablen r,g,b
		s = random(16);
		pico_keypad.illuminate(s, r, g, b);
		pico_keypad.update();	//Update the RGB keypad lights with your changes.
		sleep_ms(100);
	}
  while(true) {
		
		for( int i=0; i < 16; i++)
		{
			
		}
    // read button states from i2c expander
    uint16_t button_states = pico_keypad.get_button_states();

    if(last_button_states != button_states && button_states) {
      last_button_states = button_states;
      if(button_states) {
        if(lit == 0xffff) {
          // all buttons are already lit, reset the test
          lit = 0;
          colour_index++;
          if(colour_index >= 6) {
            colour_index = 0;
          }
        }else{
          uint8_t button = 0;
          for(uint8_t find = 0; find < pico_keypad.NUM_PADS; find++) {
            // check if this button is pressed and no other buttons are pressed
            if(button_states & 0x01) {
              if(!(button_states & (~0x01))) {
                lit |= 1 << button;
              }
              break;
            }
            button_states >>= 1;
            button++;
          }
        }
      }
    }

    last_button_states = button_states;

    for(uint8_t i = 0; i < PicoRGBKeypad::NUM_PADS; i++) {
      if((lit >> i) & 0x01) {
        switch(colour_index)
        {
          case 0: pico_keypad.illuminate(i, 0x00, 0x20, 0x00);  break;
          case 1: pico_keypad.illuminate(i, 0x20, 0x20, 0x00);  break;
          case 2: pico_keypad.illuminate(i, 0x20, 0x00, 0x00);  break;
          case 3: pico_keypad.illuminate(i, 0x20, 0x00, 0x20);  break;
          case 4: pico_keypad.illuminate(i, 0x00, 0x00, 0x20);  break;
          case 5: pico_keypad.illuminate(i, 0x00, 0x20, 0x20);  break;
        }
      }else{
        pico_keypad.illuminate(i, 0x05, 0x05, 0x05);
      }
    }

    pico_keypad.update();

    sleep_ms(100);
  }

  return 0;
}
