//===================================================================
// tahometer                                                        =
// (c) 2005 AHTOXA (http://ahtoxa.net)                              =
//                                                                  =

#pragma small
#include <AT892051.H>
#include <intrins.h>	// for _nop_, ...
#include "7seg.inc"

#define	Quartz 12000000

#define	FALSE 0
#define	TRUE !FALSE

#define LOW		0
#define HIGH	1

#define DIGIT_0		P3_0
#define DIGIT_1		P3_2
#define DIGIT_2		P3_3
#define DIGIT_3		P3_4

#define DATA_BUS	P1
#define DIGIT_ON	(0)
#define DIGIT_OFF	(1)


// reload values for timer 0 ticks every 1 ms
#define	TIMER0_FREQ		1000	// timer0 frequency (1 Khz)
#define	TIMER0_RELOAD_LO ((0xFFFF-(Quartz/12/TIMER0_FREQ)) & 0xFF)
#define	TIMER0_RELOAD_HI ((0xFFFF-(Quartz/12/TIMER0_FREQ)) >> 8)

//#if (TIMER0_RELOAD_HI!=0xFF)
//#error TIMER0_RELOAD_HI!=0xFF!
//#endif

// reload values for timer 1 ticks every 1 ms
#define	TIMER1_FREQ	1000	// timer1 frequency (Hz)
#define	TIMER1_RELOAD_LO ((0xFFFF-(Quartz/12/TIMER1_FREQ)) & 0xFF)
#define	TIMER1_RELOAD_HI ((0xFFFF-(Quartz/12/TIMER1_FREQ)) >> 8)

// Common Prototypes ================================================
void init_MCU(void);
void init_timer0(void);
void init_timer1(void);
void delay(unsigned	char time_100us);

volatile unsigned char  DELAY;

const unsigned char digit_codes[] = {LED_0,LED_1,LED_2,LED_3,LED_4,LED_5,LED_6,LED_7,LED_8,LED_9,LED_SPACE};

unsigned char	digit0 = LED_SPACE;
unsigned char	digit1 = LED_SPACE;
unsigned char	digit2 = LED_SPACE;
unsigned char	digit3 = LED_SPACE;
unsigned char	bar_lo = 0xFF;
unsigned char	bar_hi = 0xFF;

// END Globals ======================================================


void Timer0_ISR(void) interrupt 1 using 2
{
	static char digit = 0;
	static pause = TRUE;

	TH0	= TIMER0_RELOAD_LO;
	TH0	= TIMER0_RELOAD_HI;

	if (DELAY) DELAY--;		//	Timebase ticks for delay-routine

	DIGIT_0	= DIGIT_OFF;	// turn display off	
	DIGIT_1	= DIGIT_OFF;	
	DIGIT_2	= DIGIT_OFF;	
	DIGIT_3	= DIGIT_OFF;	
	DATA_BUS = LED_SPACE;

	if (pause)
	{
		pause = FALSE;
	}
	else
	{
		pause = TRUE;

		switch (digit)
		{
		case 0:
			DATA_BUS = digit0;
			DIGIT_0	= DIGIT_ON;	
			digit = 1;
			break;

		case 1:
			DATA_BUS = digit1;
			DIGIT_1	= DIGIT_ON;	
			digit = 2;
			break;

		case 2:
			DATA_BUS = digit2;
			DIGIT_2	= DIGIT_ON;	
			digit = 3;
			break;

		case 3:
		default:
			DATA_BUS = digit3;
			DIGIT_3	= DIGIT_ON;	
			digit = 0;
			break;
		}
	}
}

void Timer1_ISR(void) interrupt 3 using 3
{
	TL1 = TIMER1_RELOAD_LO;			// reload timer
	TH1 = TIMER1_RELOAD_HI;
}

void init_MCU(void)
{

	P1	= P3 =	0xFF;	// all pins to input-mode

	init_timer0();	// Initialize timer0.
	init_timer1();	// Initialize timer1.

	EA = 1;			// Enable all interrupts

}

// ---------------------------------------------------------------------------
//	Timer0 initialization - 8-bit auto-reload, 100 us period
// ---------------------------------------------------------------------------
void init_timer0( void )
{

	TMOD = (TMOD & 0xF0) | 0x01;	// Timer 0,	mode 1,	16-bit auto-reload.
	TL0	= 0;
	TH0	= TIMER0_RELOAD_LO;
	TH0	= TIMER0_RELOAD_HI;
	TR0	= 1;						// Start Timer0
	ET0	= 1;						// Enable Timer0 interrupt

}


// ---------------------------------------------------------------------------
//	Timer1 initialization - 16-bit, 1 ms
// ---------------------------------------------------------------------------
void init_timer1( void )
{
	TMOD = (TMOD & 0x0F) | 0x50;	// Timer1 mode, 16-bit counter.
	TR1	= 1;						// Start Timer1
	ET1	= 0;						// Disable Timer1 interrupt
}

void delay(unsigned	char time_100us)
{

	if (time_100us)
	{
		DELAY =	time_100us;
		while (DELAY);
	}
}

/*
void set_bar(int value)
{
	unsigned int bar = 0xFFFF;
	while (value--)
		bar <<= 1;
	bar_lo = bar;
	bar_hi = bar>>8;
}
*/

void set_value(int i)
{
	unsigned char f = FALSE;
	unsigned int j;

	j = i / 1000;
	if (j > 0)
	{
		digit0 = digit_codes[j];
		f = TRUE;
	}
	else
		digit0 = LED_SPACE;	

	i = i % 1000;
	j = i / 100;
	if ((j > 0) | f)
	{
		digit1 = digit_codes[j];
		f = TRUE;
	}
	else
		digit1 = LED_SPACE;	


	i = i % 100;
	j = i / 10;
	if ( (j>0) | f )
	{
		digit2 = digit_codes[j];
		f = TRUE;
	}
	else
		digit2 = LED_SPACE;	


	j = i % 10;
	digit3 = digit_codes[j];
}

typedef union
{
	unsigned int	word;
	unsigned char	bytes[2];
}CastStruct;
CastStruct CST;

void Welcome(void)
{
	delay(150);
	set_value(9999);
	delay(150);
	set_value(8888);
	delay(150);
	set_value(7777);
	delay(150);
	set_value(6666);
	delay(150);
	set_value(5555);
	delay(150);
	set_value(4444);
	delay(150);
	set_value(3333);
	delay(150);
	set_value(2222);
	delay(150);
	set_value(1111);
	delay(150);
	set_value(0);
	delay(400);
}


/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
// MAIN	=============================================================
void main( void	)
{
	int i = 1999;
	int bar = 0;
	int up = TRUE;
	init_MCU();				// Initialize MCU
	Welcome();
	while (TRUE)			// Main	loop
	{
		TH1 = TL1 = 0;		// clear counter
		TR1 = 1;			// start timer 1
		// count ticks 0.5 sec
		delay(250);			//
		delay(250);			//
		TR1 = 0;			// stop timer 1
		CST.bytes[1] = TL1;
		CST.bytes[0] = TH1;
		
		// 1 sec = CST.word * 2;
		// 1 min = CST.word * 2 * 60
		// rpm = CST.word * 2 * 60 / 4 = 30
		CST.word *= 20;

//		if (CST.word < 200) CST.word = 0;
		
		set_value(CST.word);
	}

} // END.

