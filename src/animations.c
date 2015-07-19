#include "animations.h"


void rampUp( uint8_t* value, uint8_t maxValue, uint8_t stepSize )
{
	*value = ( *value < ( maxValue + 1 - stepSize ) )
		? *value + stepSize
		: 0;
}


void rampUpDown( uint8_t* value, uint8_t* up, uint8_t maxValue, uint8_t stepSize )
{
	if( *up )
	{
		if( *value <= maxValue - stepSize )
		{
			*value += stepSize;
			if( *value == maxValue )
			{
				*up = 0;
			}
		}
		else
		{
			*up = 0;
			*value = maxValue;
		}
	}
	else
	{
		if( *value >= stepSize )
		{
			*value -= stepSize;
			if( *value == 0 )
			{
				*up = 1;
			}
		}
		else
		{
			*up = 1;
			*value = 0;
		}
	}
}



