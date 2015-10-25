#ifndef LEDTERNE_H_
#define LEDTERNE_H_

#include <inttypes.h>

#define MAX_INTENSITY 31
#define NUM_PIXELS 5

void setIntensity( uint8_t pixelIndex, uint8_t r, uint8_t g, uint8_t b );
void setIntensityRaw( uint8_t pixelIndex, uint8_t r, uint8_t g, uint8_t b );


#endif // LEDTERNE_H_

