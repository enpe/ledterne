#ifndef ANIMATIONS_H_
#define ANIMATIONS_H_

#include <inttypes.h>


void rampUp( uint8_t* value, uint8_t maxValue, uint8_t stepSize );


struct _RampUpDownAnimation;
typedef struct _RampUpDownAnimation RampUpDownAnimation;

RampUpDownAnimation* RampUpDown_create( uint8_t maxValue );
void RampUpDown_destroy( RampUpDownAnimation* ani );
void RampUpDown_step( RampUpDownAnimation* ani, uint8_t* value, uint8_t stepSize );


struct _MixedColorBlendingProgram;
typedef struct _MixedColorBlendingProgram MixedColorBlendingProgram;

MixedColorBlendingProgram* MixedColorBlending_create( uint8_t maxValue );
void MixedColorBlending_destroy( MixedColorBlendingProgram* prog );
uint8_t MixedColorBlending_execute( MixedColorBlendingProgram* prog );


struct _KnightRiderProgram;
typedef struct _KnightRiderProgram KnightRiderProgram;

KnightRiderProgram* KnightRider_create();
void KnightRider_destroy( KnightRiderProgram* prog );
uint8_t KnightRider_execute( KnightRiderProgram* prog );


#endif // ANIMATIONS_H_

