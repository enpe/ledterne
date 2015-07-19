#include "animations.h"

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


struct _MixedColorBlendingAnimation
{
	RampUpDownAnimation* aniR;
	RampUpDownAnimation* aniG;
	RampUpDownAnimation* aniB;
	uint8_t r;
	uint8_t g;
	uint8_t b;
};

MixedColorBlendingAnimation* MixedColorBlending_create( uint8_t maxValue )
{
	MixedColorBlendingAnimation* ani
		= (MixedColorBlendingAnimation*) malloc( sizeof( MixedColorBlendingAnimation ) );

	if( ani )
	{
		ani->aniR = RampUpDown_create( maxValue );
		ani->aniG = RampUpDown_create( maxValue );
		ani->aniB = RampUpDown_create( maxValue );

		// intial LED intensities
		ani->r =  0;
		ani->g = 10;
		ani->b = 21;
	}

	return ani;
}

void MixedColorBlending_destroy( MixedColorBlendingAnimation* ani )
{
	RampUpDown_destroy( ani->aniR );
	RampUpDown_destroy( ani->aniG );
	RampUpDown_destroy( ani->aniB );
	free( ani );
}

void MixedColorBlending_step( MixedColorBlendingAnimation* ani )
{
	RampUpDown_step( ani->aniR, &ani->r, 2 );
	RampUpDown_step( ani->aniG, &ani->g, 1 );
	RampUpDown_step( ani->aniB, &ani->b, 3 );
}

void MixedColorBlending_getColor( MixedColorBlendingAnimation const* ani, uint8_t* r, uint8_t* g, uint8_t* b )
{
	if( ani )
	{
		*r = ani->r;
		*g = ani->g;
		*b = ani->b;
	}
}



