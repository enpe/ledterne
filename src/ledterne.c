/**
 * Dim an array of 5 RGB LEDs using software PWM
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

#include "animations.h"
#include "ledterne.h"

#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>
#include <util/delay.h>


// LED outputs (connected to the anodes)
#define PORT_0 PORTB
#define PIN_R0 PB2
#define PIN_G0 PB1
#define PIN_B0 PB0
#define PORT_1 PORTD
#define PIN_R1 PD7
#define PIN_G1 PD6
#define PIN_B1 PD5
#define PORT_2 PORTD
#define PIN_R2 PD2
#define PIN_G2 PD1
#define PIN_B2 PD0
#define PORT_3 PORTC
#define PIN_R3 PC2
#define PIN_G3 PC1
#define PIN_B3 PC0
#define PORT_4 PORTB
#define PIN_R4 PB5
#define PIN_G4 PB4
#define PIN_B4 PB3

// determine data direction register for port p
#define DDR( p ) ( *( &p - 1 ) )


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

ledIntensity_t g_intensity[ NUM_PIXELS ];

// flag for updating the frame (i.e. for advancing the color animation one step)
volatile int g_frameUpdateRequired = 0;


/**
 * @brief Set RGB LED intensities for display
 */
void setIntensity( uint8_t pixelIndex, uint8_t r, uint8_t g, uint8_t b )
{
	if( pixelIndex >= NUM_PIXELS )
	{
		return;
	}

	if( r <= MAX_INTENSITY )
	{
		g_intensity[ pixelIndex ].r = g_pwm[ r ];
	}

	if( g <= MAX_INTENSITY )
	{
		g_intensity[ pixelIndex ].g = g_pwm[ g ];
	}

	if( b <= MAX_INTENSITY )
	{
		g_intensity[ pixelIndex ].b = g_pwm[ b ];
	}
}


/**
 * @brief Interrupt handler for a single PWM step
 *
 * This updates the LED outputs to achieve the currently selected intensities for each channel.
 */
ISR( TIMER2_COMP_vect )
{
	static uint8_t pwmStep = 0;

	if( pwmStep < g_intensity[ 0 ].g ) { PORT_0 |= (1<<PIN_G0); } else { PORT_0 &= ~(1<<PIN_G0); }
	if( pwmStep < g_intensity[ 0 ].r ) { PORT_0 |= (1<<PIN_R0); } else { PORT_0 &= ~(1<<PIN_R0); }
	if( pwmStep < g_intensity[ 0 ].b ) { PORT_0 |= (1<<PIN_B0); } else { PORT_0 &= ~(1<<PIN_B0); }

	if( pwmStep < g_intensity[ 1 ].g ) { PORT_1 |= (1<<PIN_G1); } else { PORT_1 &= ~(1<<PIN_G1); }
	if( pwmStep < g_intensity[ 1 ].r ) { PORT_1 |= (1<<PIN_R1); } else { PORT_1 &= ~(1<<PIN_R1); }
	if( pwmStep < g_intensity[ 1 ].b ) { PORT_1 |= (1<<PIN_B1); } else { PORT_1 &= ~(1<<PIN_B1); }

	if( pwmStep < g_intensity[ 2 ].g ) { PORT_2 |= (1<<PIN_G2); } else { PORT_2 &= ~(1<<PIN_G2); }
	if( pwmStep < g_intensity[ 2 ].r ) { PORT_2 |= (1<<PIN_R2); } else { PORT_2 &= ~(1<<PIN_R2); }
	if( pwmStep < g_intensity[ 2 ].b ) { PORT_2 |= (1<<PIN_B2); } else { PORT_2 &= ~(1<<PIN_B2); }

	if( pwmStep < g_intensity[ 3 ].g ) { PORT_3 |= (1<<PIN_G3); } else { PORT_3 &= ~(1<<PIN_G3); }
	if( pwmStep < g_intensity[ 3 ].r ) { PORT_3 |= (1<<PIN_R3); } else { PORT_3 &= ~(1<<PIN_R3); }
	if( pwmStep < g_intensity[ 3 ].b ) { PORT_3 |= (1<<PIN_B3); } else { PORT_3 &= ~(1<<PIN_B3); }

	if( pwmStep < g_intensity[ 4 ].g ) { PORT_4 |= (1<<PIN_G4); } else { PORT_4 &= ~(1<<PIN_G4); }
	if( pwmStep < g_intensity[ 4 ].r ) { PORT_4 |= (1<<PIN_R4); } else { PORT_4 &= ~(1<<PIN_R4); }
	if( pwmStep < g_intensity[ 4 ].b ) { PORT_4 |= (1<<PIN_B4); } else { PORT_4 &= ~(1<<PIN_B4); }

	// Since it is an uint8, this counter overflows at 255. This is desired behaviour. It makes the
	// counter run from 0 to 255, i.e. it makes a complete PWM cycle consist of 256 single steps.
	pwmStep += 1;
}


/**
 * @brief Interrupt handler for frame tick
 *
 * This signals the main program that the animations needs to be advanced one step
 */
ISR( TIMER1_COMPA_vect )
{
	g_frameUpdateRequired = 1;
}


/**
 * @Brief Set up timer that triggers an interrupt for every single PWM step
 */
void pwmTimerInit()
{
	// prescaler for 8 bit timer: 1/8 clock frequency
	TCCR2 |= (1<<CS21);

	// Clear Timer on Compare (CTC):
	// reset counter TCNT2 when it reaches the value in OCR2
	TCCR2 |= (1<<WGM21);

	// frequency of timer interrupts: ca. 25.6 kHz
	//
	//    f = F_CPU / (PRESCALER * (1 + OCR2))
	// OCR2 = F_CPU / (f * PRESCALER) - 1
	//
	// Since a complete PWM cycle consists of 256 single steps (see the PWM step counter in the
	// associated interrupt handler) this leaves us with a PWM frequency of roughly
	//
	// f_PWM = 25.6 kHz / 256
	//       = 100 Hz .
	//
	OCR2 = 38;

	// enable interrupt on reaching the reference value in OCR2
	TIMSK |= (1<<OCIE2);
}


void animationTimerInit()
{
	// prescaler for 16 bit timer: 1/1024 clock frequency
	TCCR1B |= (1<<CS12) | (1<<CS10);

	// Clear Timer on Compare (CTC):
	// reset counter TCNT1 when it reaches the value in OCRA1
	TCCR1B |= (1<<WGM12);

	// frequency of timer interrupts: ca. 15 Hz
	//
	//     f = F_CPU / (PRESCALER * (1 + OCR1A))
	// OCRA1 = F_CPU / (f * PRESCALER) - 1
	OCR1A = 520;

	// enable interrupt on reaching the reference value in OCR1A
	TIMSK |= (1<<OCIE1A);
}


void setAnimationTimer( uint16_t v )
{
	OCR1A = v;
}


int main( void )
{
	// configure LED pins as outputs, disable by default
	DDR( PORT_0 ) |=    (1<<PIN_R0) | (1<<PIN_G0) | (1<<PIN_B0);
	PORT_0        &= ~( (1<<PIN_R0) | (1<<PIN_G0) | (1<<PIN_B0) );
	DDR( PORT_1 ) |=    (1<<PIN_R1) | (1<<PIN_G1) | (1<<PIN_B1);
	PORT_1        &= ~( (1<<PIN_R1) | (1<<PIN_G1) | (1<<PIN_B1) );
	DDR( PORT_2 ) |=    (1<<PIN_R2) | (1<<PIN_G2) | (1<<PIN_B2);
	PORT_2        &= ~( (1<<PIN_R2) | (1<<PIN_G2) | (1<<PIN_B2) );
	DDR( PORT_3 ) |=    (1<<PIN_R3) | (1<<PIN_G3) | (1<<PIN_B3);
	PORT_3        &= ~( (1<<PIN_R3) | (1<<PIN_G3) | (1<<PIN_B3) );
	DDR( PORT_4 ) |=    (1<<PIN_R4) | (1<<PIN_G4) | (1<<PIN_B4);
	PORT_4        &= ~( (1<<PIN_R4) | (1<<PIN_G4) | (1<<PIN_B4) );

	pwmTimerInit();
	animationTimerInit();

	uint8_t i;

	// intially switch all pixels off
	for( i = 0; i < NUM_PIXELS; i++ )
	{
		setIntensity( i, 0, 0, 0 );
	}

	// globally enable interrupts
	sei();


	AnimationModule const animation[] =
	{
		{
			.programType = ColoredConveyor,
			.repetitions = 12,
			.timerPeriod = 520,
		},
		{
			.programType = MixedColorBlending,
			.repetitions = 1,
			.timerPeriod = 520,
		},
		{
			.programType = KnightRider,
			.repetitions = 2,
			.timerPeriod = 520 * 2,
		},
#if 0
		{
			.programType = TestDisplays,
			.repetitions = 1,
			.timerPeriod = 520 * 2,
		},
#endif
	};

	// compute number of modules in the animation
	uint8_t numModules = sizeof( animation ) / sizeof( AnimationModule );

	uint8_t currentModuleIndex = numModules - 1; // HACK: needed to start with the first module
	uint8_t repetitions = 0;
	AnimationModule const* currentModule = NULL;
	void* currentProgram = NULL;

	void (*programDestroyFunc)( void* ) = NULL;
	uint8_t (*programExecuteFunc)( void* ) = NULL;

	while( 1 )
	{
		if( g_frameUpdateRequired )
		{
			g_frameUpdateRequired = 0;

			// load next module if the current one has finished playing completely
			if( repetitions == 0 )
			{
				// destroy the previous module's program if one exists
				if( currentProgram )
				{
					(*programDestroyFunc)( currentProgram );
				}

				// select next module (start at beginning if we reached the module list's end)
				currentModuleIndex += 1;
				if( currentModuleIndex == numModules )
				{
					currentModuleIndex = 0;
				}

				currentModule = &animation[ currentModuleIndex ];
				repetitions = currentModule->repetitions;

				// instantiate the current module's program
				switch( currentModule->programType )
				{
					case MixedColorBlending:
						currentProgram = MixedColorBlending_create();
						programDestroyFunc = &MixedColorBlending_destroy;
						programExecuteFunc = &MixedColorBlending_execute;
						break;

					case KnightRider:
						currentProgram = KnightRider_create();
						programDestroyFunc = &KnightRider_destroy;
						programExecuteFunc = &KnightRider_execute;
						break;

					case ColoredConveyor:
						currentProgram = ColoredConveyor_create();
						programDestroyFunc = &ColoredConveyor_destroy;
						programExecuteFunc = &ColoredConveyor_execute;
						break;

					case TestDisplays:
						currentProgram = TestDisplays_create();
						programDestroyFunc = &TestDisplays_destroy;
						programExecuteFunc = &TestDisplays_execute;
						break;

				}

				setAnimationTimer( currentModule->timerPeriod );
			}

			// execute the current module's program
			uint8_t programFinished = (*programExecuteFunc)( currentProgram );


			if( programFinished )
			{
				repetitions -= 1;
			}
		}
	}

	return 0;
}

