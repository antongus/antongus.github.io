//===================================================================
// power-reg (for lamp)                                             =
// (c) 2004 AHTOXA (http://ahtoxa.net)                              =
//                                                                  =
#pragma small
#include <AT892051.H>
#include <intrins.h>	// for _nop_, ...

#define	Quartz 12000000
#define	FALSE	0
#define	TRUE	!FALSE

#define	TIMER1_FREQ	200		// timer1 frequency (Hz)
#define	TIMER1_RELOAD_LO ((0xFFFF-(Quartz/12/TIMER1_FREQ)+1) & 0xFF)
#define	TIMER1_RELOAD_HI ((0xFFFF-(Quartz/12/TIMER1_FREQ)+1) >> 8)

#define SEG_A_PIN	P1_2
#define SEG_B_PIN	P1_3
#define SEG_C_PIN	P1_5
#define SEG_D_PIN	P3_5
#define SEG_E_PIN	P1_4
#define SEG_F_PIN	P3_2
#define SEG_G_PIN	P3_3
#define SEG_H_PIN	P3_4
#define SEG_ON		(0)
#define SEG_OFF		(1)

#define	DIGIT2		P1_6
#define	DIGIT1		P1_7
#define DIGIT_ON	(0)
#define DIGIT_OFF	(1)

#define	IRON		P3_7
#define IRON_ON		(0)
#define IRON_OFF	(1)

#define KEY_UP		P3_1
#define KEY_DOWN	P3_0

#define	COMPARATOR	AOUT


#define SEG_A	(0x01<<0)
#define SEG_B	(0x01<<1)
#define SEG_C	(0x01<<2)
#define SEG_D	(0x01<<3)
#define SEG_E	(0x01<<4)
#define SEG_F	(0x01<<5)
#define SEG_G	(0x01<<6)
#define SEG_H	(0x01<<7)

#define DIGIT_0  (SEG_A + SEG_B + SEG_C + SEG_D + SEG_E + SEG_F        )
#define DIGIT_1  (        SEG_B + SEG_C                                )
#define DIGIT_2  (SEG_A + SEG_B         + SEG_D + SEG_E         + SEG_G)
#define DIGIT_3  (SEG_A + SEG_B + SEG_C + SEG_D                 + SEG_G)
#define DIGIT_4  (        SEG_B + SEG_C                 + SEG_F + SEG_G)
#define DIGIT_5  (SEG_A         + SEG_C + SEG_D         + SEG_F + SEG_G)
#define DIGIT_6  (SEG_A         + SEG_C + SEG_D + SEG_E + SEG_F + SEG_G)
#define DIGIT_7  (SEG_A + SEG_B + SEG_C                                )
#define DIGIT_8  (SEG_A + SEG_B + SEG_C + SEG_D + SEG_E + SEG_F + SEG_G)
#define DIGIT_9  (SEG_A + SEG_B + SEG_C + SEG_D         + SEG_F + SEG_G)

static const char code digits[] =
	{DIGIT_0, DIGIT_1, DIGIT_2, DIGIT_3, DIGIT_4, DIGIT_5, DIGIT_6, DIGIT_7, DIGIT_8, DIGIT_9 };

const unsigned int code len_array[100] = {
	55635, 56273, 56538, 56743, 56917, 57071, 57210, 57340, 57461, 57575,
	57683, 57787, 57887, 57983, 58076, 58167, 58255, 58341, 58424, 58506,
	58587, 58666, 58743, 58819, 58894, 58968, 59041, 59113, 59185, 59255,
	59325, 59394, 59463, 59531, 59598, 59665, 59732, 59798, 59864, 59929,
	59994, 60059, 60124, 60188, 60252, 60316, 60380, 60444, 60508, 60571,
	60635, 60699, 60762, 60826, 60890, 60954, 61018, 61082, 61146, 61211,
	61276, 61341, 61406, 61472, 61538, 61605, 61672, 61739, 61807, 61876,
	61945, 62015, 62085, 62157, 62229, 62302, 62376, 62451, 62527, 62604,
	62683, 62764, 62846, 62929, 63015, 63103, 63194, 63287, 63383, 63483,
	63587, 63695, 63809, 63930, 64060, 64199, 64353, 64527, 64732, 64997
};


const unsigned char code version[] = "Version 1.1";

static char digit1 = 0;
static char digit2 = 0;
static char value  = 0;
static char new_value  = 75;
static char save_value = 75;
static char first_dig  = TRUE;
static char debounce = 5;
static char off_ticks_lo = 0;
static char off_ticks_hi = 0;

void timer0_isr (void) interrupt TF0_VECTOR using 1
{
	IRON = IRON_ON;		// fire TRIAC
	TR0 = 0;			// stop timer
}

void set_value(unsigned char val)
{
	if (val>99) val = 99;
	value = val;

	off_ticks_lo = len_array[value];
	off_ticks_hi = len_array[value]>>8;

	digit2 = digits[val % 10];
	val /= 10;
	digit1 = digits[val % 10];
}

void timer1_isr (void) interrupt TF1_VECTOR using 2   /* Frequency */
{

	static char scan_stage = TRUE;
	static char repeat = FALSE;

	TL1 = TIMER1_RELOAD_LO;
	TH1 = TIMER1_RELOAD_HI;

	if (debounce) debounce--;
	
	if(scan_stage)
	{
		scan_stage = FALSE;
		DIGIT2 = DIGIT_OFF;
		if (digit1 & SEG_A)	SEG_A_PIN = SEG_ON; else SEG_A_PIN = SEG_OFF;
		if (digit1 & SEG_B)	SEG_B_PIN = SEG_ON; else SEG_B_PIN = SEG_OFF;
		if (digit1 & SEG_C)	SEG_C_PIN = SEG_ON; else SEG_C_PIN = SEG_OFF;
		if (digit1 & SEG_D)	SEG_D_PIN = SEG_ON; else SEG_D_PIN = SEG_OFF;
		if (digit1 & SEG_E)	SEG_E_PIN = SEG_ON; else SEG_E_PIN = SEG_OFF;
		if (digit1 & SEG_F)	SEG_F_PIN = SEG_ON; else SEG_F_PIN = SEG_OFF;
		if (digit1 & SEG_G)	SEG_G_PIN = SEG_ON; else SEG_G_PIN = SEG_OFF;
		if (digit1 & SEG_H)	SEG_H_PIN = SEG_ON; else SEG_H_PIN = SEG_OFF;
		DIGIT1 = DIGIT_ON;
	}
	else
	{
		scan_stage = TRUE;
		DIGIT1 = DIGIT_OFF;
		if (digit2 & SEG_A)	SEG_A_PIN = SEG_ON; else SEG_A_PIN = SEG_OFF;
		if (digit2 & SEG_B)	SEG_B_PIN = SEG_ON; else SEG_B_PIN = SEG_OFF;
		if (digit2 & SEG_C)	SEG_C_PIN = SEG_ON; else SEG_C_PIN = SEG_OFF;
		if (digit2 & SEG_D)	SEG_D_PIN = SEG_ON; else SEG_D_PIN = SEG_OFF;
		if (digit2 & SEG_E)	SEG_E_PIN = SEG_ON; else SEG_E_PIN = SEG_OFF;
		if (digit2 & SEG_F)	SEG_F_PIN = SEG_ON; else SEG_F_PIN = SEG_OFF;
		if (digit2 & SEG_G)	SEG_G_PIN = SEG_ON; else SEG_G_PIN = SEG_OFF;
		if (digit2 & SEG_H)	SEG_H_PIN = SEG_ON; else SEG_H_PIN = SEG_OFF;
		DIGIT2 = DIGIT_ON;
	}

	if (KEY_UP == 0)
	{
		if (KEY_DOWN == 0)
		{
			if (!debounce)
			{
				if (value)
				{
					save_value = value;
					new_value = 0;
				}
				else
				{
					new_value = save_value;
				}
				debounce = 255;					
			}
		}
		else
		{
			if (!debounce)
			{
				new_value = value+1;
				if (repeat)
					debounce = 30;
				else
					debounce = 150;
				repeat = TRUE;
			}
		}
	}
	else 
	if (KEY_DOWN == 0)
	{
		if (!debounce)
		{
			if (value) new_value = value-1;
			if (repeat)
				debounce = 30;
			else
				debounce = 150;
			repeat = TRUE;
		}
	}
	else
	{
		debounce = 20;
		repeat = FALSE;
	}
}  
  
void main(void)
{
	P1 = P3 = 0xFF;
	TMOD = 0x11;
	TL1	= TIMER1_RELOAD_LO;
	TH1	= TIMER1_RELOAD_HI;
	ET1	= 1;						// Enable Timer1 interrupt
	TR1	= 1;						// Run Timer1
	PT0 = 1;						// Set high priority to timet0
	ET0 = 1;						// Enable timer 0 interrupt
	EA = 1;							// Enable all interrupts

	set_value(new_value);

	IRON = IRON_OFF;

	while (TRUE)
	{
		while (COMPARATOR);					// wait for zero-cross
		if (value != 99) IRON = IRON_OFF;	// turn TRIAC OFF
		if (value)
		{
			TL0 = off_ticks_lo; 
			TH0 = off_ticks_hi;
			TR0 = 1;						// run timer (will fire TRIAC)
		}

		// check for user input
		if (new_value != value)
			set_value(new_value);

		while (!COMPARATOR);				// wait for zero-cross
		if (value != 99) IRON = IRON_OFF;	// turn TRIAC OFF
		if (value)
		{
			TL0 = off_ticks_lo; 
			TH0 = off_ticks_hi;
			TR0 = 1;				// run timer
		}
	}
}
