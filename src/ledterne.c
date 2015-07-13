/**
 * Dim an RGB LED using software PWM
 *
 * Platform: ATmega8 @ 8Mhz
 *
 * The basic idea is to rapidly switch an LED on and off in order to change its brightness. The LED
 * is switched on at the beginning of a PWM cycle and switched off later in that cycle (or not
 * switched off at all if maximum brightness is desired). Depending on how long the LED remains lit
 * in that cycle, it will appear brighter or darker. The proportion of "on" time to the length of
 * the complete PWM cycle is the so-called duty cycle.
 *
 * Since the LED is basically flickering, the PWM frequency has to be high enough to make the LED
 * appear to glow with constant brightness to the human eye. We use a PWM frequency of roughly
 * 100 Hz here, with each PWM cycle consisting of 256 steps (i.e. a temporal resolution of 8 bit).
 *
 * A hardware timer is generating an interrupt for each PWM step. The interrupt handler then
 * decides, based on the current position in the PWM cycle and the desired LED brightness, whether
 * to switch the LED on or off.
 *
 * The human eye does not perceive linear changes in brightness as linear but rather
 * logarithmically. For a perceived linear change in LED brightness we thus need to map the desired
 * LED brightness to an appropriate PWM duty cycle by applying a power function. This mapping is
 * done using a precomputed lookup table.
 */

// clock frequency
#ifndef F_CPU
#define F_CPU 8000000L
#endif

#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>
#include <util/delay.h>


#define MAX_INTENSITY 31

// LED outputs (connected to the anodes)
#define PIN_G PB1
#define PIN_R PB2
#define PIN_B PB3


// lookup table for converting LED intensities to PWM steps (brighter LED requires the output to
// stay on for more steps)
uint8_t g_pwm[ MAX_INTENSITY + 1 ] =
{
	  0,   1,   1,   1,   2,   2,   3,   3,
	  4,   5,   6,   7,   9,  10,  12,  15,
	 17,  21,  25,  30,  36,  43,  51,  61,
	 73,  87, 104, 125, 149, 178, 213, 255,
};


typedef struct
{
	uint8_t r;
	uint8_t g;
	uint8_t b;	
}
ledIntensity_t;

ledIntensity_t g_intensity;

/**
 * @brief Set RGB LED intensities for display
 */
void setIntensity( uint8_t r, uint8_t g, uint8_t b )
{
	if( r <= MAX_INTENSITY )
	{
		g_intensity.r = g_pwm[ r ];
	}

	if( g <= MAX_INTENSITY )
	{
		g_intensity.g = g_pwm[ g ];
	}

	if( b <= MAX_INTENSITY )
	{
		g_intensity.b = g_pwm[ b ];
	}
}


/**
 * @brief Interrupt handler for a single PWM step
 *
 * This updates the LED outputs to achieve the currently selected intensities for each channel.
 */
ISR( TIMER1_COMPA_vect )
{
	static uint8_t pwmStep = 0;

	if( pwmStep < g_intensity.r ) { PORTB |= (1<<PIN_R); } else { PORTB &= ~(1<<PIN_R); }
	if( pwmStep < g_intensity.g ) { PORTB |= (1<<PIN_G); } else { PORTB &= ~(1<<PIN_G); }
	if( pwmStep < g_intensity.b ) { PORTB |= (1<<PIN_B); } else { PORTB &= ~(1<<PIN_B); }

	// Since it is an uint8, this counter overflows at 255. This is desired behaviour. It makes the
	// counter run from 0 to 255, i.e. it makes a complete PWM cycle consist of 256 single steps.
	pwmStep += 1;
}


/**
 * @Brief Set up timer that triggers an interrupt for every single PWM step
 */
void pwmTimerInit()
{
	// prescaler for 16 bit timer: 1/1 clock frequency (i.e. no prescaler)
	TCCR1B |= (1<<CS20);

	// Clear Timer on Compare (CTC):
	// reset counter TCNT1 when it reaches the value in OCRA1
	TCCR1B |= (1<<WGM12);

	// frequency of timer interrupts: ca. 25.6 kHz
	//
	//     f = F_CPU / (PRESCALER * (1 + OCR1A))
	// OCRA1 = F_CPU / (f * PRESCALER) - 1
	//
	// Since a complete PWM cycle consists of 256 single steps (see the PWM step counter in the
	// associated interrupt handler) this leaves us with a PWM frequency of roughly
	//
	// f_PWM = 25.5 kHz / 256
	//       = 100 Hz .
	//
	OCR1A = 312;

	// enable interrupt on reaching the reference value in OCR1A
	TIMSK |= (1<<OCIE1A);
}


void rampUp( uint8_t* intensity, uint8_t stepSize )
{
	*intensity = ( *intensity < ( MAX_INTENSITY + 1 - stepSize ) )
		? *intensity + stepSize
		: 0;
}


void rampUpDown( uint8_t* intensity, uint8_t* up, uint8_t stepSize )
{
	if( *up )
	{
		if( *intensity <= MAX_INTENSITY - stepSize )
		{
			*intensity += stepSize;
			if( *intensity == MAX_INTENSITY )
			{
				*up = 0;
			}
		}
		else
		{
			*up = 0;
			*intensity = MAX_INTENSITY;
		}
	}
	else
	{
		if( *intensity >= stepSize )
		{
			*intensity -= stepSize;
			if( *intensity == 0 )
			{
				*up = 1;
			}
		}
		else
		{
			*up = 1;
			*intensity = 0;
		}
	}
}


int main( void )
{
	// configure LED pins as outputs, disable by default
	DDRB  |=    (1<<PIN_R) | (1<<PIN_G) | (1<<PIN_B);
	PORTB &= ~( (1<<PIN_R) | (1<<PIN_G) | (1<<PIN_B) );

	pwmTimerInit();
	
	setIntensity( 0, 0, 0 );

	// globally enable interrupts
	sei();


	// initial LED intensities
	uint8_t intensityR = 0;
	uint8_t intensityG = 10;
	uint8_t intensityB = 21;

	// initial ramp directions
	uint8_t upR = 1;
	uint8_t upG = 1;
	uint8_t upB = 1;

	while( 1 )
	{
		// add small delay to simulate slower frame rate
		// TODO: replace this with a proper frame tick
		_delay_ms( 65 );

		setIntensity( intensityR, intensityG, intensityB );

		rampUpDown( &intensityR, &upR, 2 );
		rampUpDown( &intensityG, &upG, 1 );
		rampUpDown( &intensityB, &upB, 3 );
	}

	return 0;
}

