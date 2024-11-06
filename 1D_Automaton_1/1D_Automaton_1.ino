// 1D random cellular automaton 1 // 

#include "hardware/structs/rosc.h"
#include <PicoDVI.h>

DVIGFX1 display(DVI_RES_640x480p60, false, pimoroni_demo_hdmi_cfg);

#define WIDTH   640
#define HEIGHT  480

  bool state[WIDTH]; 
  bool newstate[WIDTH];
  bool rules[8] = {0, 1, 1, 1, 1, 0, 0, 0};
  uint8_t cnt;

void rndrule(){

  for (int i=0; i<8; i++) rules[i] = rand()%2;
  for (int i=0; i<WIDTH; i++) state[i] = rand()%2;

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

    if (cnt == 120) { cnt = 0; rndrule(); }
    cnt++;

    memset (newstate, 0, sizeof(newstate));

    for(int x=0; x<WIDTH; x++) {
      
      int k = 4 * state[(x-1+HEIGHT)%HEIGHT] + 2 * state[x] + state[(x+1)%WIDTH];
      newstate[x] = rules[k];

      display.drawPixel(x, y, state[x]);
 
    }

    memcpy (state, newstate, sizeof(state));

    delay(10);

  }

}