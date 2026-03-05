#include <stm32f031x6.h>
#include "display.h"
void initClock(void);
void initSysTick(void);
void SysTick_Handler(void);
void delay(volatile uint32_t dly);
void setupIO();
int isInside(uint16_t x1, uint16_t y1, uint16_t w, uint16_t h, uint16_t px, uint16_t py);
void enablePullUp(GPIO_TypeDef *Port, uint32_t BitNumber);
void pinMode(GPIO_TypeDef *Port, uint32_t BitNumber, uint32_t Mode);

volatile uint32_t milliseconds;

const uint16_t student[]=
{
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,16621,16621,16621,40375,40375,40375,40375,40375,0,0,0,0,0,0,0,0,0,0,0,0,16621,16621,16621,40375,40375,40375,40375,40375,0,0,0,0,0,0,0,0,0,0,0,0,16621,16621,16621,40375,40375,40375,40375,40375,0,0,0,0,0,0,0,0,0,0,0,0,16621,16621,16621,40375,40375,40375,40375,40375,0,0,0,0,0,0,0,0,45056,45056,45056,45056,45056,56685,56685,56685,0,0,0,0,0,0,0,0,0,0,0,0,0,45056,45056,45056,45056,56685,65535,56685,0,0,0,0,0,0,0,0,0,0,0,0,45056,45056,45056,45056,45056,56685,0,56685,0,0,0,0,0,0,0,0,0,0,0,0,0,45056,45056,45056,45056,56685,56685,56685,0,0,0,0,0,0,0,0,0,0,0,0,45056,45056,45056,45056,45056,56685,0,56685,0,0,0,0,0,0,0,0,0,0,0,0,0,45056,45056,45056,45056,56685,65535,56685,0,0,0,0,0,0,0,0,0,0,0,0,45056,45056,45056,45056,45056,56685,56685,56685,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,16621,16621,16621,40375,40375,40375,40375,40375,0,0,0,0,0,0,0,0,0,0,0,0,16621,16621,16621,40375,40375,40375,40375,40375,0,0,0,0,0,0,0,0,0,0,0,0,16621,16621,16621,40375,40375,40375,40375,40375,0,0,0,0,0,0,0,0,0,0,0,0,16621,16621,16621,40375,40375,40375,40375,40375,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,};
const uint16_t teach[]= 
	{
0,0,0,0,0,4,4,4,4,4,0,0,0,0,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,65415,65415,65415,248,65415,0,0,0,0,0,0,0,65415,65415,65415,65415,65415,8068,0,0,0,0,0,0,65415,65415,65415,4096,4096,0,0,0,0,0,0,0,0,65415,65415,65415,0,0,0,0,0,0,0,0,7936,7936,7936,0,0,0,0,0,0,0,0,7936,7936,65535,7936,0,0,0,0,0,0,0,0,7936,7936,65535,7936,7936,7936,7936,0,0,0,0,0,7936,7936,65535,65535,65535,65535,7936,0,0,0,0,0,7936,7936,7936,7936,7936,7936,7936,0,0,0,0,0,7936,7936,7936,7936,0,0,0,0,0,0,0,0,0,40224,7936,65535,7936,0,0,0,0,0,0,0,40224,40224,7936,65535,7936,0,0,0,0,0,0,65315,40224,40224,7936,65535,40224,0,0,0,0,0,0,0,65315,0,65315,65315,65315,65315,0,0,
	};
const uint16_t blud[]= 
{
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,4,0,0,0,0,0,0,0,0,0,0,4,4,0,0,0,0,0,0,0,0,0,4,4,4,4,0,0,0,0,0,0,0,4,4,4,4,4,4,0,0,0,0,7936,7936,4,4,4,4,4,4,7936,7936,0,0,65535,65535,4,4,4,4,4,4,65535,65535,0,0,7936,7936,4,4,4,4,4,4,7936,7936,0,0,0,0,0,4,4,4,4,0,0,0,0,0,0,0,0,0,24327,24327,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
};
const uint16_t Lstudent[]=
{
	0,0,16142,16142,16142,16142,16142,16142,16142,16142,0,0,0,0,0,16142,16142,16142,16142,16142,16142,0,0,0,0,0,16142,16142,16142,16142,16142,16142,16142,16142,0,0,0,0,16142,16142,16142,1994,1994,16142,16142,16142,0,0,0,0,16142,16142,16142,1994,16142,1994,16142,16142,0,0,0,0,16142,16142,16142,1994,16142,1994,16142,16142,0,0,0,0,16142,16142,16142,1994,16142,1994,16142,16142,0,0,0,0,16142,16142,16142,1994,1994,16142,16142,16142,0,0,0,0,16142,16142,16142,16142,16142,16142,16142,16142,0,0,0,0,16142,16142,16142,1994,1994,1994,16142,16142,0,0,0,0,16142,16142,16142,1994,16142,16142,16142,16142,0,0,0,0,16142,16142,16142,1994,16142,16142,16142,16142,0,0,0,0,16142,16142,16142,1994,16142,1994,16142,16142,0,0,0,0,16142,16142,16142,1994,1994,1994,16142,16142,0,0,0,0,0,16142,16142,16142,16142,16142,16142,0,0,0,0,0,0,16142,16142,16142,16142,16142,16142,0,0,0,
};


void drawmap();
int rightpressed();
int leftpressed();
int downpressed();
int uppressed();


int main()
{
	int x = 1;
	int y = 1;
//	int hinverted = 0;
//	int vinverted = 0;
/*		int toggle = 0;
	int hmoved = 0;
	int vmoved = 0;
	uint16_t x = 50;
	uint16_t y = 50;
	uint16_t oldx = x;
	uint16_t oldy = y;*/
	initClock();
	initSysTick();
	setupIO();
//	putImage(20,80,12,16,dg1,0,0);
	
//	putImage(0,10,20,20,student,0,1);
	drawmap();
		
	fillRectangle(x,y,15,15,RGBToWord(0,225,0));
	delay(50);
	while(1){

		printNumber(y, 60, 75, RGBToWord(225,225,225), RGBToWord(0,0,0));
		if(uppressed())
		{
			fillRectangle(x,y,15,15,RGBToWord(0,0,0));
			delay(50);
			y = y - 16;
			if (y < 1)
				y = 1;
			fillRectangle(x,y,15,15,RGBToWord(0,225,0));
			delay(50);
		}

		if(downpressed())
		{
			fillRectangle(x,y,15,15,RGBToWord(0,0,0));
			delay(50);
			y = y + 16;
			if (y > 145)
				y = 145;
			fillRectangle(x,y,15,15,RGBToWord(0,225,0));
			delay(50);
		}
		if (leftpressed())
		{
			fillRectangle(x,y,15,15,RGBToWord(0,0,0));
			delay(50);
			x = x - 16;
			if (x < 1)
				x = 1;
			fillRectangle(x,y,15,15,RGBToWord(0,225,0));
			delay(50);
		}
		if (rightpressed())
		{
			fillRectangle(x,y,15,15,RGBToWord(0,0,0));
			delay(50);
			x = x + 16;
			if (x > 113)
				x = 113;
			fillRectangle(x,y,15,15,RGBToWord(0,225,0));
			delay(50);
		}
		if(rightpressed() && leftpressed())
		{
			x = y = 1;
			drawmap();
			fillRectangle(x,y,15,15,RGBToWord(0,225,0));
			delay(500);
		}


/*		for(int y = 1; y < 150; y = y + 16)
		{
			for(int x = 1; x < 120; x = x + 16)
			{
				fillRectangle(x,y,15,15,RGBToWord(0,225,0));
				delay(125);
				fillRectangle(x,y,15,15,RGBToWord(0,0,0));
			}
		}
*/
	}
	/*
	while(1)
	{


	
		
		if ((GPIOB->IDR & (1 << 4))==0) // right pressed
		{					


			hinverted=0;
					
		}
		if ((GPIOB->IDR & (1 << 5))==0) // left pressed
		{			
			
			hinverted=1;
		
		}
		if ( (GPIOA->IDR & (1 << 11)) == 0) // down pressed
		{

			vinverted = 0;

		}
		if ( (GPIOA->IDR & (1 << 8)) == 0) // up pressed
		{			


			vinverted = 1;

		}



		putImage(0,10,20,20,student,hinverted,vinverted);

		
		if ((vmoved) || (hmoved))
		{
			// only redraw if there has been some movement (reduces flicker)
			fillRectangle(oldx,oldy,12,16,0);
			oldx = x;
			oldy = y;					
			if (hmoved)
			{
				if (toggle)
					putImage(x,y,12,16,deco1,hinverted,0);
				else
					putImage(x,y,12,16,deco2,hinverted,0);
				
				toggle = toggle ^ 1;
			}
			else
			{
				putImage(x,y,12,16,deco3,0,vinverted);
			}
			// Now check for an overlap by checking to see if ANY of the 4 corners of deco are within the target area
			if (isInside(20,80,12,16,x,y) || isInside(20,80,12,16,x+12,y) || isInside(20,80,12,16,x,y+16) || isInside(20,80,12,16,x+12,y+16) )
			{
				printTextX2("GLUG!", 10, 20, RGBToWord(0xff,0xff,0), 0);
			}
		}		
		delay(50);
	}
	}
	*/	
	return 0;
}
void initSysTick(void)
{
	SysTick->LOAD = 48000;
	SysTick->CTRL = 7;
	SysTick->VAL = 10;
	__asm(" cpsie i "); // enable interrupts
}

void drawmap()
{
	fillRectangle(0,0,127,165,RGBToWord(0,0,0));
	for(int i=0; i < 168; i = i + 16 )
	{
		
		drawLine(0,i,127, i, RGBToWord(225,225,225));
	}
	for(int b=0; b < 127; b = b + 16 )
	{
		drawLine(b,0,b, 167, RGBToWord(225,225,225));
	}
	drawRectangle(0,0,127,159,RGBToWord(225,0,0));
}
int rightpressed()
{
	return (GPIOB->IDR & (1 << 4))==0;
}
int leftpressed()
{
	return (GPIOB->IDR & (1 << 5))==0;
}
int downpressed()
{
	return (GPIOA->IDR & (1 << 11)) == 0;
}
int uppressed()
{
	return (GPIOA->IDR & (1 << 8)) == 0;
}

void SysTick_Handler(void)
{
	milliseconds++;
}
void initClock(void)
{
// This is potentially a dangerous function as it could
// result in a system with an invalid clock signal - result: a stuck system
        // Set the PLL up
        // First ensure PLL is disabled
        RCC->CR &= ~(1u<<24);
        while( (RCC->CR & (1 <<25))); // wait for PLL ready to be cleared
        
// Warning here: if system clock is greater than 24MHz then wait-state(s) need to be
// inserted into Flash memory interface
				
        FLASH->ACR |= (1 << 0);
        FLASH->ACR &=~((1u << 2) | (1u<<1));
        // Turn on FLASH prefetch buffer
        FLASH->ACR |= (1 << 4);
        // set PLL multiplier to 12 (yielding 48MHz)
        RCC->CFGR &= ~((1u<<21) | (1u<<20) | (1u<<19) | (1u<<18));
        RCC->CFGR |= ((1<<21) | (1<<19) ); 

        // Need to limit ADC clock to below 14MHz so will change ADC prescaler to 4
        RCC->CFGR |= (1<<14);

        // and turn the PLL back on again
        RCC->CR |= (1<<24);        
        // set PLL as system clock source 
        RCC->CFGR |= (1<<1);
}
void delay(volatile uint32_t dly)
{
	uint32_t end_time = dly + milliseconds;
	while(milliseconds != end_time)
		__asm(" wfi "); // sleep
}

void enablePullUp(GPIO_TypeDef *Port, uint32_t BitNumber)
{
	Port->PUPDR = Port->PUPDR &~(3u << BitNumber*2); // clear pull-up resistor bits
	Port->PUPDR = Port->PUPDR | (1u << BitNumber*2); // set pull-up bit
}
void pinMode(GPIO_TypeDef *Port, uint32_t BitNumber, uint32_t Mode)
{
	/*
	*/
	uint32_t mode_value = Port->MODER;
	Mode = Mode << (2 * BitNumber);
	mode_value = mode_value & ~(3u << (BitNumber * 2));
	mode_value = mode_value | Mode;
	Port->MODER = mode_value;
}
int isInside(uint16_t x1, uint16_t y1, uint16_t w, uint16_t h, uint16_t px, uint16_t py)
{
	// checks to see if point px,py is within the rectange defined by x,y,w,h
	uint16_t x2,y2;
	x2 = x1+w;
	y2 = y1+h;
	int rvalue = 0;
	if ( (px >= x1) && (px <= x2))
	{
		// ok, x constraint met
		if ( (py >= y1) && (py <= y2))
			rvalue = 1;
	}
	return rvalue;
}

void setupIO()
{
	RCC->AHBENR |= (1 << 18) + (1 << 17); // enable Ports A and B
	display_begin();
	pinMode(GPIOB,4,0);
	pinMode(GPIOB,5,0);
	pinMode(GPIOA,8,0);
	pinMode(GPIOA,11,0);
	enablePullUp(GPIOB,4);
	enablePullUp(GPIOB,5);
	enablePullUp(GPIOA,11);
	enablePullUp(GPIOA,8);
}