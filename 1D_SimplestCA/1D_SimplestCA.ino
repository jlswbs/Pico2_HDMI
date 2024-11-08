// Simplest 1D cellular automaton // 

#include "hardware/structs/rosc.h"
#include <PicoDVI.h>

DVIGFX1 display(DVI_RES_640x480p60, false, pimoroni_demo_hdmi_cfg);

#define WIDTH   640
#define HEIGHT  480

  bool state[WIDTH];

void rndrule(){

  state[rand()%WIDTH] = 1;

}

static inline void seed_random_from_rosc(){
  
  uint32_t random = 0;
  uint32_t random_bit;
  volatile uint32_t *rnd_reg = (uint32_t *)(ROSC_BASE + ROSC_RANDOMBIT_OFFSET);

  for (int k = 0; k < 32; k++) {
    while (1) {
      random_bit = (*rnd_reg) & 1;
      if (random_bit != ((*rnd_reg) & 1)) break;
    }

    random = (random << 1) | random_bit;
  }

  srand(random);

}

void setup() {

  seed_random_from_rosc();
  display.begin();

  rndrule();

}

void loop() {

  for(int y=0; y<HEIGHT; y++) {

    for(int x=0; x<WIDTH; x++) {
      
      bool k = k ^ state[x];
      state[x] = k;

      display.drawPixel(x, y, state[x]);
 
    }

    delay(10);

  }

}