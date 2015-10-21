#ifndef ANIMATIONS_H_
#define ANIMATIONS_H_

#include <inttypes.h>


enum AnimationProgram
{
	MixedColorBlending,
	KnightRider,
	ColoredConveyor,
	TestDisplays
};

typedef struct
{
	enum AnimationProgram programType;
	uint8_t repetitions;
}
AnimationModule;


uint8_t rampUp( uint8_t* value, uint8_t maxValue, uint8_t stepSize );


struct _RampUpDownAnimation;
typedef struct _RampUpDownAnimation RampUpDownAnimation;

RampUpDownAnimation* RampUpDown_create( uint8_t maxValue );
void RampUpDown_destroy( RampUpDownAnimation* ani );
uint8_t RampUpDown_step( RampUpDownAnimation* ani, uint8_t* value, uint8_t stepSize );


struct _MixedColorBlendingProgram;
typedef struct _MixedColorBlendingProgram MixedColorBlendingProgram;

MixedColorBlendingProgram* MixedColorBlending_create();
void MixedColorBlending_destroy( MixedColorBlendingProgram* prog );
uint8_t MixedColorBlending_execute( MixedColorBlendingProgram* prog );


struct _KnightRiderProgram;
typedef struct _KnightRiderProgram KnightRiderProgram;

KnightRiderProgram* KnightRider_create();
void KnightRider_destroy( KnightRiderProgram* prog );
uint8_t KnightRider_execute( KnightRiderProgram* prog );


struct _ColoredConveyorProgram;
typedef struct _ColoredConveyorProgram ColoredConveyorProgram;

ColoredConveyorProgram* ColoredConveyor_create();
void ColoredConveyor_destroy( ColoredConveyorProgram* prog );
uint8_t ColoredConveyor_execute( ColoredConveyorProgram* prog );

struct _TestDisplaysProgram;
typedef struct _TestDisplaysProgram TestDisplaysProgram;

TestDisplaysProgram* TestDisplaysProgram_create();
void TestDisplaysProgram_destroy( TestDisplaysProgram* prog );
uint8_t TestDisplaysProgram_execute( TestDisplaysProgram* prog );


#endif // ANIMATIONS_H_

