#ifndef ANIMATIONS_H_
#define ANIMATIONS_H_

#include <inttypes.h>


void rampUp( uint8_t* value, uint8_t maxValue, uint8_t stepSize );


struct _RampUpDownAnimation;
typedef struct _RampUpDownAnimation RampUpDownAnimation;

RampUpDownAnimation* RampUpDown_create( uint8_t maxValue );
void RampUpDown_destroy( RampUpDownAnimation* ani );
void RampUpDown_step( RampUpDownAnimation* ani, uint8_t* value, uint8_t stepSize );


struct _MixedColorBlendingAnimation;
typedef struct _MixedColorBlendingAnimation MixedColorBlendingAnimation;

MixedColorBlendingAnimation* MixedColorBlending_create( uint8_t maxValue );
void MixedColorBlending_destroy( MixedColorBlendingAnimation* ani );
void MixedColorBlending_step( MixedColorBlendingAnimation* ani );
void MixedColorBlending_getColor( MixedColorBlendingAnimation const* ani, uint8_t* r, uint8_t* g, uint8_t* b );


#endif // ANIMATIONS_H_

