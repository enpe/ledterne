#include "animations.h"
#include "ledterne.h"

#include <stdlib.h>


void rampUp( uint8_t* value, uint8_t maxValue, uint8_t stepSize )
{
	*value = ( *value < ( maxValue + 1 - stepSize ) )
		? *value + stepSize
		: 0;
}


struct _RampUpDownAnimation
{
	uint8_t maxValue;
	uint8_t up;
};

RampUpDownAnimation* RampUpDown_create( uint8_t maxValue )
{
	RampUpDownAnimation* ani = (RampUpDownAnimation*) malloc( sizeof( RampUpDownAnimation ) );

	if( ani )
	{
		ani->maxValue = maxValue;
		ani->up       = 1;
	}

	return ani;
}

void RampUpDown_destroy( RampUpDownAnimation* ani )
{
	free( ani );
}

void RampUpDown_step( RampUpDownAnimation* ani, uint8_t* value, uint8_t stepSize )
{
	if( ani->up )
	{
		if( *value <= ani->maxValue - stepSize )
		{
			*value += stepSize;
			if( *value == ani->maxValue )
			{
				ani->up = 0;
			}
		}
		else
		{
			ani->up = 0;
			*value = ani->maxValue;
		}
	}
	else
	{
		if( *value >= stepSize )
		{
			*value -= stepSize;
			if( *value == 0 )
			{
				ani->up = 1;
			}
		}
		else
		{
			ani->up = 1;
			*value = 0;
		}
	}
}


struct _MixedColorBlendingProgram
{
	RampUpDownAnimation* aniR;
	RampUpDownAnimation* aniG;
	RampUpDownAnimation* aniB;
	uint8_t r;
	uint8_t g;
	uint8_t b;
};

MixedColorBlendingProgram* MixedColorBlending_create( uint8_t maxValue )
{
	MixedColorBlendingProgram* prog
		= (MixedColorBlendingProgram*) malloc( sizeof( MixedColorBlendingProgram ) );

	if( prog )
	{
		prog->aniR = RampUpDown_create( maxValue );
		prog->aniG = RampUpDown_create( maxValue );
		prog->aniB = RampUpDown_create( maxValue );

		// intial LED intensities
		prog->r =  0;
		prog->g = 10;
		prog->b = 21;
	}

	return prog;
}

void MixedColorBlending_destroy( MixedColorBlendingProgram* prog )
{
	RampUpDown_destroy( prog->aniR );
	RampUpDown_destroy( prog->aniG );
	RampUpDown_destroy( prog->aniB );
	free( prog );
}

void MixedColorBlending_execute( MixedColorBlendingProgram* prog )
{
	uint8_t i;

	// update LED colors for display
	for( i = 0; i < NUM_PIXELS; i++ )
	{
		setIntensity( i, prog->r, prog->g, prog->b );
	}

	// advance color animation one step
	RampUpDown_step( prog->aniR, &prog->r, 2 );
	RampUpDown_step( prog->aniG, &prog->g, 1 );
	RampUpDown_step( prog->aniB, &prog->b, 3 );

}



