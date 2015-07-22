#include "animations.h"
#include "ledterne.h"

#include <stdlib.h>
#include <string.h>


// return 1 if one cycle has been completed
uint8_t rampUp( uint8_t* value, uint8_t maxValue, uint8_t stepSize )
{
	if( *value < ( maxValue + 1 - stepSize ) )
	{
		*value += stepSize;
		return 0;
	}
	else
	{
		*value = 0;
		return 1;
	}
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

// return 1 if one complete cycle (first up, then down) has been completed
uint8_t RampUpDown_step( RampUpDownAnimation* ani, uint8_t* value, uint8_t stepSize )
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
				return 1;
			}
		}
		else
		{
			ani->up = 1;
			*value = 0;
			return 1;
		}
	}

	return 0;
}


struct _MixedColorBlendingProgram
{
	RampUpDownAnimation* aniR;
	RampUpDownAnimation* aniG;
	RampUpDownAnimation* aniB;
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t frame;
};

MixedColorBlendingProgram* MixedColorBlending_create()
{
	MixedColorBlendingProgram* prog
		= (MixedColorBlendingProgram*) malloc( sizeof( MixedColorBlendingProgram ) );

	if( prog )
	{
		prog->aniR = RampUpDown_create( MAX_INTENSITY );
		prog->aniG = RampUpDown_create( MAX_INTENSITY );
		prog->aniB = RampUpDown_create( MAX_INTENSITY );

		// intial LED intensities
		prog->r =  0;
		prog->g = 10;
		prog->b = 21;

		prog->frame = 0;
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

uint8_t MixedColorBlending_execute( MixedColorBlendingProgram* prog )
{
	// NOTE: This is just an arbitrary (relatively small) number to end the program at some defined
	//       point. The actual animation does only loop much later.
	static uint8_t const programLen = 100;  // total number of frames in this program

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

	// advance frame counter (automatically wraps around)
	return rampUp( &prog->frame, programLen - 1, 1 );
}


struct _KnightRiderProgram
{
	RampUpDownAnimation* ramp;
	uint8_t centerIndex;
	uint8_t frame;
	uint8_t fadeState[ NUM_PIXELS ];
};

KnightRiderProgram* KnightRider_create()
{
	KnightRiderProgram* prog = (KnightRiderProgram*) malloc( sizeof( KnightRiderProgram ) );

	if( prog )
	{
		prog->ramp = RampUpDown_create( NUM_PIXELS - 1 );
		prog->centerIndex = 0;
		prog->frame = 0;
		memset( prog->fadeState, 0, sizeof( prog->fadeState ) );
	}

	return prog;
}

void KnightRider_destroy( KnightRiderProgram* prog )
{
	RampUpDown_destroy( prog->ramp );
	free( prog );
}

/**
 * The basic idea is this: Let a pixel (called "center pixel") bounce back and forth using an
 * up/down ramp animation which determines the current center position. This center pixel is always
 * at maximum brightness while the other pixels slowly fade from their previous brightness down to
 * zero brightness. Since being the current center pixel is the only way to light up an unlit pixel,
 * the center pixel will automatically drag a tail of fading pixels behind it.
 *
 * The actual program is a couple of frames longer than the actual animation of the moving center
 * pixel. This will create a short pause in the bouncing and fading animation in which the LEDs can
 * fade to zero and remain completely off for a couple of frames before the next iteration starts.
 */
uint8_t KnightRider_execute( KnightRiderProgram* prog )
{
	#define NUM_FADE_STATES 4
	#define PROGRAM_TOTAL_LEN 25     // total number of frames in this program
	#define PROGRAM_MOVEMENT_LEN 17  // number of frames that the moving center pixel is animated

	static uint8_t fadeIntensities[ NUM_FADE_STATES ] =
	{
		0.000 * MAX_INTENSITY,
		0.162 * MAX_INTENSITY,
		0.710 * MAX_INTENSITY,
		1.000 * MAX_INTENSITY,
	};

	uint8_t lightCenterPixel = prog->frame < PROGRAM_MOVEMENT_LEN;
	uint8_t i;

	for( i = 0; i < NUM_PIXELS; i++ )
	{
		if( lightCenterPixel && ( i == prog->centerIndex ) )
		{
			// switch on the current center pixel
			prog->fadeState[ i ] = NUM_FADE_STATES - 1;
		}
		else
		{
			// fade out the other pixels
			if( prog->fadeState[ i ] > 0 )
			{
				prog->fadeState[ i ] -= 1;
			}
		}

		// make LEDs light up red only
		setIntensity( i, fadeIntensities[ prog->fadeState[ i ] ], 0, 0 );
	}

	// advance center position one step
	RampUpDown_step( prog->ramp, &prog->centerIndex, 1 );

	if( prog->frame < PROGRAM_TOTAL_LEN )
	{
		prog->frame += 1;
		return 0;
	}
	else
	{
		prog->frame = 0;
		prog->centerIndex = 0;

		// NOTE: If we want to be really tidy, we should reset the ramp and the fade states here to
		//       allow for arbitrary program lengths and animations lengths.

		return 1;
	}
}


struct _ColoredConveyorProgram
{
	RampUpDownAnimation* ramps[ NUM_PIXELS ];
	uint8_t r[ NUM_PIXELS ];
	uint8_t g[ NUM_PIXELS ];
	uint8_t b[ NUM_PIXELS ];
	uint8_t* p[ NUM_PIXELS ];
	uint8_t* colorList[ 3 ];
	uint8_t colorIndex[ NUM_PIXELS ];
	uint8_t frame;
};

ColoredConveyorProgram* ColoredConveyor_create()
{
	ColoredConveyorProgram* prog
		= (ColoredConveyorProgram*) malloc( sizeof( ColoredConveyorProgram ) );

	if( prog )
	{
		uint8_t i;

		for( i = 0; i < NUM_PIXELS; i++ )
		{
			prog->ramps[ i ] = RampUpDown_create( MAX_INTENSITY );

			// initial LED intensities
			prog->r[ i ] = i * ( MAX_INTENSITY / ( NUM_PIXELS - 1 ) );
			prog->g[ i ] = 0;
			prog->b[ i ] = 0;

			prog->colorIndex[ i ] = 0;

			prog->p[ i ] = &prog->r[ i ];
		}

		prog->colorList[ 0 ] = prog->r;
		prog->colorList[ 1 ] = prog->g;
		prog->colorList[ 2 ] = prog->b;

		prog->frame = 0;
	}

	return prog;
}

void ColoredConveyor_destroy( ColoredConveyorProgram* prog )
{
	uint8_t i;

	for( i = 0; i < NUM_PIXELS; i++ )
	{
		 RampUpDown_destroy( prog->ramps[ i ] );
	}

	free( prog );
}

uint8_t ColoredConveyor_execute( ColoredConveyorProgram* prog )
{
	static uint8_t const programLen = 56;  // total number of frames in this program

	uint8_t i;

	// update LED colors for display
	for( i = 0; i < NUM_PIXELS; i++ )
	{
		setIntensity( i, prog->r[ i ], prog->g[ i ], prog->b[ i ] );
	}

	for( i = 0; i < NUM_PIXELS; i++ )
	{
		// ramp up/down the colors pointed to by p
		if( RampUpDown_step( prog->ramps[ i ], prog->p[ i ], 3 ) )
		{
			// if one complete up/down cycle has been completed, select the next color (start at
			// beginning if we reached the color list's end)
			rampUp( &prog->colorIndex[ i ], 2, 1 );

			uint8_t currentColorIndex = prog->colorIndex[ i ];
			prog->p[ i ] = &( prog->colorList[ currentColorIndex ][ i ] );
		}
	}

	// advance frame counter (automatically wraps around)
	return rampUp( &prog->frame, programLen - 1, 1 );
}



