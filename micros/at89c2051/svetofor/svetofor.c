//===================================================================
// svetofor                                                         =
// (c) 2005 AHTOXA (http://ahtoxa.net)                              =
//                                                                  =

#pragma small
#include <AT892051.H>
#include <intrins.h>	// for _nop_, ...

#define	Quartz	4000000
#define	FALSE	0
#define	TRUE	!FALSE

#define	TIMER1_FREQ	1000	// timer1 frequency (1KHz)

#define	TIMER1_RELOAD_LO ((0xFFFF-(Quartz/12/TIMER1_FREQ)+1) & 0xFF)
#define	TIMER1_RELOAD_HI ((0xFFFF-(Quartz/12/TIMER1_FREQ)+1) >> 8)

// LED DEFINES ======================================================
#define	LED_GREEN		P1_7
#define	LED_YELLOW		P1_6
#define	LED_RED			P3_7

#define	LED_ON		0
#define	LED_OFF		1

const unsigned char code version[] = "Svetofor v1.0 by AHTOXA";

volatile unsigned int ms_ticks = 0;

void init_Timer1(void);
void delay_ms(int ms);

void timer1_isr (void) interrupt TF1_VECTOR using 2   /* Frequency */
{
	TL1 = TIMER1_RELOAD_LO;
	TH1 = TIMER1_RELOAD_HI;
	if (ms_ticks) ms_ticks--;
}  
  
void delay_ms(int ms)
{
	ms_ticks = ms;
	while (ms_ticks);
}

void init_Timer1(void)
{
	TMOD = (TMOD & 0x0F) | 0x10;	// Timer1 mode 1, 16-bit.
	TL1	= TIMER1_RELOAD_LO;
	TH1	= TIMER1_RELOAD_HI;
	ET1	= 1;						// Enable Timer1 interrupt
	TR1	= 1;						// Run Timer1
}

void main(void)
{

	P1 = P3 = 0xFF;
	P1_0 = 0;
	P1_1 = 0;

	TMOD = 0x11;

	init_Timer1();
	
	PT0 = 1;	//SET HIGH PRIORITY TO TIMER1

	EA = 1;		// ENABLE GLOBAL INTERRUPT

	LED_RED		= LED_OFF;
	LED_GREEN	= LED_OFF;
	LED_YELLOW	= LED_OFF;

	while ( TRUE )
	{

		// red
		LED_YELLOW	= LED_OFF;
		LED_RED		= LED_ON;
		delay_ms(4000);

		// red and yellow
		LED_YELLOW	= LED_ON;
		delay_ms(1500);

		// green
		LED_RED		= LED_OFF;
		LED_YELLOW	= LED_OFF;
		LED_GREEN	= LED_ON;
		delay_ms(3000);

		LED_GREEN	= LED_OFF;
		delay_ms(400);
		LED_GREEN	= LED_ON;
		delay_ms(500);

		LED_GREEN	= LED_OFF;
		delay_ms(400);
		LED_GREEN	= LED_ON;
		delay_ms(500);

		LED_GREEN	= LED_OFF;
		delay_ms(400);
		LED_GREEN	= LED_ON;
		delay_ms(500);

		LED_GREEN	= LED_OFF;
		delay_ms(400);
		LED_GREEN	= LED_ON;
		delay_ms(500);


		// yellow
		LED_GREEN	= LED_OFF;
		LED_YELLOW	= LED_ON;
		delay_ms(1500);
	}

	
}
