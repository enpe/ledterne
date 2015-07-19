#ifndef ANIMATIONS_H_
#define ANIMATIONS_H_

#include <inttypes.h>

void rampUp( uint8_t* value, uint8_t maxValue, uint8_t stepSize );
void rampUpDown( uint8_t* value, uint8_t* up, uint8_t maxValue, uint8_t stepSize );


#endif // ANIMATIONS_H_

