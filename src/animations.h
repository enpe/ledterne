#ifndef ANIMATIONS_H_
#define ANIMATIONS_H_

#include <inttypes.h>

void rampUp( uint8_t* value, uint8_t maxValue, uint8_t stepSize );

struct _RampUpDownAnimation;
typedef struct _RampUpDownAnimation RampUpDownAnimation;

RampUpDownAnimation* RampUpDown_create( uint8_t maxValue );
void RampUpDown_destroy( RampUpDownAnimation* ani );
void RampUpDown_step( RampUpDownAnimation* ani, uint8_t* value, uint8_t stepSize );

#endif // ANIMATIONS_H_

