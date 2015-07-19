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



