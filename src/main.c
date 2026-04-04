#include <stm32f031x6.h>
#include <stdlib.h>
#include "display.h"
#include "sound.h"
#include "musical_notes.h"

//game speed
#define MVE_DELAY 100
#define BPM 300

void initClock(void);
void initSysTick(void);
void SysTick_Handler(void);
void delay(volatile uint32_t dly);
void setupIO();
void enablePullUp(GPIO_TypeDef *Port, uint32_t BitNumber);
void pinMode(GPIO_TypeDef *Port, uint32_t BitNumber, uint32_t Mode);
void eputchar(char c);
char egetchar(void);
void initSound();
void eputs(char *str);
volatile uint32_t milliseconds;

const uint16_t student[]=
{0,0,0,0,0,16621,16621,16621,40375,40375,40375,40375,40375,40375,0,0,0,0,0,0,16621,16621,16621,40375,40375,40375,40375,40375,40375,0,0,0,0,0,0,16621,16621,16621,40375,40375,40375,40375,40375,40375,0,0,0,0,0,0,16621,16621,16621,40375,40375,40375,40375,40375,40375,0,0,0,45056,45056,45056,45056,56685,56685,56685,0,0,0,0,0,0,0,0,0,45056,45056,45056,56685,65535,56685,0,0,0,0,0,0,0,0,45056,45056,45056,45056,56685,0,56685,0,0,0,0,0,0,0,0,0,45056,45056,45056,56685,56685,56685,0,0,0,0,0,0,0,0,45056,45056,45056,45056,56685,0,56685,0,0,0,0,0,0,0,0,0,45056,45056,45056,56685,65535,56685,0,0,0,0,0,0,0,0,45056,45056,45056,45056,56685,56685,56685,0,0,0,0,0,0,0,0,0,0,0,16621,16621,16621,40375,40375,40375,40375,40375,40375,0,0,0,0,0,0,16621,16621,16621,40375,40375,40375,40375,40375,40375,0,0,0,0,0,0,16621,16621,16621,40375,40375,40375,40375,40375,40375,0,0,0,0,0,0,16621,16621,16621,40375,40375,40375,40375,40375,40375,0,
};
const uint16_t studentV[]={
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,40375,40375,40375,40375,0,0,0,0,0,0,0,40375,40375,40375,40375,40375,40375,40375,40375,0,0,0,0,0,0,0,40375,40375,40375,40375,40375,40375,40375,40375,0,0,0,0,0,0,0,40375,40375,40375,40375,40375,40375,40375,40375,0,0,0,0,0,0,0,40375,40375,40375,40375,40375,40375,40375,40375,0,0,0,0,0,0,0,40375,40375,40375,40375,40375,40375,40375,40375,56685,56685,56685,56685,56685,56685,56685,40375,40375,40375,40375,16621,16621,16621,16621,56685,65535,0,56685,0,65535,56685,16621,16621,16621,16621,16621,16621,16621,16621,56685,56685,56685,56685,56685,56685,56685,16621,16621,16621,16621,16621,16621,16621,16621,45056,45056,45056,45056,45056,45056,45056,16621,16621,16621,16621,0,0,0,0,45056,45056,45056,45056,45056,45056,45056,0,0,0,0,0,0,0,0,45056,45056,45056,45056,45056,45056,45056,0,0,0,0,0,0,0,0,45056,0,45056,0,45056,0,45056,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
};
const uint16_t teachH[]= {0,8046,8046,8046,8046,8046,8046,8046,13294,13294,13294,13294,0,0,0,0,8046,8046,8046,8046,8046,8046,8046,13294,13294,13294,13294,0,0,0,0,8046,8046,8046,8046,8046,8046,8046,13294,13294,13294,13294,0,0,0,0,0,0,0,0,0,0,0,13294,13294,13294,13294,0,0,0,0,0,0,0,0,0,8046,8046,8046,8046,8046,8046,8046,8046,0,0,0,0,0,0,0,8046,40224,8046,22355,8046,8046,8046,8046,0,0,0,0,0,0,0,8046,0,22355,22355,8046,8046,8046,8046,0,0,0,0,0,0,0,8046,8046,8046,8046,8046,8046,8046,8046,0,0,0,0,0,0,0,8046,0,22355,22355,8046,8046,8046,8046,0,0,0,0,0,0,0,8046,40224,8046,22355,8046,8046,8046,8046,0,0,0,0,0,0,0,8046,8046,8046,8046,8046,8046,8046,8046,0,0,0,0,0,0,0,0,0,13294,13294,13294,13294,0,0,0,0,8046,8046,8046,8046,8046,8046,8046,13294,13294,13294,13294,0,0,0,0,8046,8046,8046,8046,8046,8046,8046,13294,13294,13294,13294,0,0,0,0,8046,8046,8046,8046,8046,8046,8046,13294,13294,13294,13294,0,0,0,};

const uint16_t blud[]= 
{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6400,0,0,5888,0,0,0,0,0,0,0,0,0,0,6400,3840,3840,3840,0,0,0,0,0,0,0,0,0,0,0,3840,3840,3840,3840,0,0,0,0,0,0,0,5888,0,0,5888,3840,3840,3840,3840,3840,0,0,0,0,0,0,0,0,5888,3840,3840,3840,3840,3840,0,0,0,0,0,0,0,0,5888,3840,3840,3840,3840,3840,3840,0,0,0,0,0,5888,0,3840,3840,3840,3840,3840,3840,3840,3840,0,5888,0,0,0,0,6400,3840,3840,3840,3840,3840,3840,3840,3840,0,0,0,0,6400,0,3840,3840,3840,3840,3840,3840,3840,3840,3840,3840,0,0,0,0,0,3840,3840,3840,3840,3840,3840,3840,3840,3840,3840,0,0,0,0,0,6400,3840,3840,3840,3840,3840,3840,3840,3840,3840,0,5888,0,5888,0,0,3840,3840,3840,3840,3840,3840,3840,3840,0,0,0,0,0,0,0,6400,0,3840,3840,6400,0,5888,0,0,0,0,0,0,0,0,0,0,3840,0,0,0,0,0,0,0,0,};
const uint16_t Lstudent[]=
{
	0,0,16142,16142,16142,16142,16142,16142,16142,16142,0,0,0,0,0,16142,16142,16142,16142,16142,16142,0,0,0,0,0,16142,16142,16142,16142,16142,16142,16142,16142,0,0,0,0,16142,16142,16142,1994,1994,16142,16142,16142,0,0,0,0,16142,16142,16142,1994,16142,1994,16142,16142,0,0,0,0,16142,16142,16142,1994,16142,1994,16142,16142,0,0,0,0,16142,16142,16142,1994,16142,1994,16142,16142,0,0,0,0,16142,16142,16142,1994,1994,16142,16142,16142,0,0,0,0,16142,16142,16142,16142,16142,16142,16142,16142,0,0,0,0,16142,16142,16142,1994,1994,1994,16142,16142,0,0,0,0,16142,16142,16142,1994,16142,16142,16142,16142,0,0,0,0,16142,16142,16142,1994,16142,16142,16142,16142,0,0,0,0,16142,16142,16142,1994,16142,1994,16142,16142,0,0,0,0,16142,16142,16142,1994,1994,1994,16142,16142,0,0,0,0,0,16142,16142,16142,16142,16142,16142,0,0,0,0,0,0,16142,16142,16142,16142,16142,16142,0,0,0,
};
const uint16_t teachV[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8046,8046,8046,8046,8046,8046,8046,0,0,0,0,0,0,0,0,8046,8046,8046,8046,8046,8046,8046,0,0,0,0,13294,13294,13294,13294,8046,8046,8046,8046,8046,8046,8046,13294,13294,13294,13294,13294,13294,13294,13294,8046,8046,8046,8046,8046,8046,8046,13294,13294,13294,13294,13294,13294,13294,13294,8046,22355,22355,8046,22355,22355,8046,13294,13294,13294,13294,13294,13294,13294,13294,8046,8046,22355,8046,22355,8046,8046,13294,13294,13294,13294,8046,8046,8046,0,8046,40224,0,8046,0,40224,8046,0,8046,8046,8046,8046,8046,8046,0,8046,8046,8046,8046,8046,8046,8046,0,8046,8046,8046,8046,8046,8046,0,0,0,0,0,0,0,0,0,8046,8046,8046,8046,8046,8046,0,0,0,0,0,0,0,0,0,8046,8046,8046,8046,8046,8046,0,0,0,0,0,0,0,0,0,8046,8046,8046,8046,8046,8046,0,0,0,0,0,0,0,0,0,8046,8046,8046,8046,8046,8046,0,0,0,0,0,0,0,0,0,8046,8046,8046,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,};
const int notes[] = {E4, A4, A4, C4, C4, A4, E4, E4, B4, A4, G4, F4, E4, E4,};


// The map of the game. # is a computer, s is a student, ! is a cheater, and . is an empty space
char PCmap[10][9] = {
	"........",
	"###..###",
	"sss..sss",
	"##......",
	"ss####..",
	"..ssss..",
	"........",
	".....###",
	".....sss",
	"........"
	
};
char name[10] = {"________"};
//load bearing coconut

void resetNameField(void);


void printDecimal(int32_t Value);
void initSerial();


//Controls functions
int rightpressed();
int leftpressed();
int downpressed();
int uppressed();
int altPressed();//ALT Button
void redLED(int);//LED control function, 1 for on, 0 for off

//Aux Functions
void drawcomputer(int x, int y, int Scolor);//Draws a computer at the given x and y coordinates. Color specifies color of the screen
int checkpos(int x, int y);//Function to check the current player pos. Returns 0 for empty, 1 for cheater(LED turns on), 2 for student and restore the image.
int GameMenu();
int PlayGame(int);
void Getname();
//lwk need a new name for this one
//-----------------
int checkpos2(int x, int y);//Function to check the current player pos. Returns 0 for empty, 1 for cheater, 2 for student. This one doesnt restore
//-----------------

int drawmap();// Function to draw the map
void displayScore(int);// Function to display the score on the screen
int AttemptCheater(int);//Function attempts to select a cheater with a 10% chance. Returns 1 if successful, 0 if not.

int main()
{
	//Starting position of the player

//	int SelectedComputer = rand() % ComputerCount;

	initClock();
	initSound();

	initSysTick();
	setupIO();
	initSerial();
	fillRectangle(0,0,127,165,RGBToWord(0,0,0));
	int number = 0;
	int buffer = 0;
	

//Game menu section
	while (1)
	{

	
		buffer = GameMenu();
		switch (buffer)
		{
		case 0:
			eputs("Starting the Game\n");
			buffer = PlayGame(number);
			if(buffer > number)
			{
				number = buffer;
			}
			break;
		case 1:
			eputs("Display Highscore\n");
			fillRectangle(0,0,127,165,RGBToWord(0,0,0));
			printText("Current Highscore:", 1, 20, RGBToWord(225,225,225), RGBToWord(0,0,0));
			printText(name, 1, 30, RGBToWord(225,225,225), RGBToWord(0,0,0));
			printNumber(number, 80, 30, RGBToWord(225,225,225), RGBToWord(0,0,0));

			while(!rightpressed() && !leftpressed())
			{}
			break;
	
		default:
			fillRectangle(0,0,128,165,RGBToWord(0,0,0));
			break;
		}
		fillRectangle(0,0,128,165,RGBToWord(0,0,0));
	}


//Game init	


	//printNumber((int)PCmap[y/16][x/16], 45, 1, RGBToWord(225,225,225), RGBToWord(0,0,0));


	


//Game loop


	return 0;
}


//Game

int PlayGame(int highscore)
{
	int chune = 0;
	int NoteCount = sizeof(notes)/sizeof(int);
	uint16_t x = 65;
	uint16_t y = 17;
	int score = 0;
	int CheaterCount = 0;
	int StudentCount = drawmap();
	int buffer = 0;
	milliseconds = 0;

	drawmap();
	putImage(x,y,15,15,teachH, 1, 0);
	delay(50);
	redLED(0);



	while(milliseconds < 30000){


//MUSIC
		if(milliseconds % BPM == 0)
		{
			playNote((notes[chune]));
			chune = (chune + 1) % NoteCount;
		}



		if(StudentCount == 0 && CheaterCount == 0)
		{
			score = score + (60000-milliseconds);
			fillRectangle(0,0,128,165,RGBToWord(0,0,0));
			printText("Score:", 33, 40, RGBToWord(225,225,225), RGBToWord(0,0,0));
			printNumber(score, 75, 40, RGBToWord(225,0,0), RGBToWord(0,0,0));
			playNote(0);
			redLED(0);
			if(score > highscore)
			{	printText("New Highscore!", 1, 80, RGBToWord(225,225,225), RGBToWord(0,0,0));	
				printText("Type your name:", 1, 90, RGBToWord(225,225,225), RGBToWord(0,0,0));
				Getname();
			}	
			delay(500);
			return score;
		}
		if((CheaterCount < 3 && StudentCount != 0 ) && (milliseconds%(MVE_DELAY*5)==0))
		{
			displayScore(score);
			buffer = AttemptCheater(StudentCount);
			StudentCount = StudentCount - buffer;
			CheaterCount = CheaterCount + buffer;
		}

		if(altPressed() && checkpos2(x,y) == 1)
		{
			CheaterCount -= 1;
			score += 1;
			PCmap[y/16][x/16] = '-';
			drawcomputer(x, (y-16), RGBToWord(0,0,255));
			playNote(C5);
			redLED(0);
		}

	//Movement section.
		if(uppressed() && (milliseconds%MVE_DELAY==0))
		{

			if ((y - 16) < 17)
				{y = 17;}
			else{
				checkpos(x,y);
				y=y-16;}

			if(PCmap[(y/16)][(x/16)]  == '#')
				{y = y + 16;}

			checkpos(x,y);
			putImage(x,y,15,15,teachV, 0, 1);
		}
		if(downpressed() && (milliseconds%MVE_DELAY==0))
		{

			if ((y + 16) > 145)
				{y = 145;
				}
			else{
				checkpos(x,y);
				y=y+16;
				}

			if(PCmap[(y/16)][(x/16)]  == '#')
				{y = y - 16;}

			checkpos(x,y);
			putImage(x,y,15,15,teachV, 0, 0);
		}
		if (leftpressed() && (milliseconds%MVE_DELAY==0))
		{

			if ((x - 16) < 1)
				{x = 1;}
			else{
				checkpos(x,y);
				x=x-16;}

			if(PCmap[(y/16)][(x/16)]  == '#')
				{x = x + 16;}

			checkpos(x,y);	
			putImage(x,y,15,15,teachH, 0, 0);
		}
		if (rightpressed()  && (milliseconds%MVE_DELAY==0))
		{
			if ((x + 16) > 113)
				{x = 113;
				}
			else{
				checkpos(x,y);
				x=x+16;
				}

			if(PCmap[(y/16)][(x/16)]  == '#')
				{x = x - 16;
				}

			checkpos(x,y);
			putImage(x,y,15,15,teachH, 1, 0);
		}
		if((rightpressed() && leftpressed() && altPressed()))
		{
			playNote(0);
			delay(500);
			return score;

		}


	}
	fillRectangle(0,0,128,165,RGBToWord(0,0,0));
	printText("Time's up!", 33, 20, RGBToWord(225,225,225), RGBToWord(0,0,0));
	printText("Score:", 1, 40, RGBToWord(225,225,225), RGBToWord(0,0,0));
	printNumber(score, 65, 40, RGBToWord(225,0,0), RGBToWord(0,0,0));
	playNote(0);
	redLED(0);
	if(score > highscore)
		{	
			printText("New Highscore!", 1, 80, RGBToWord(225,225,225), RGBToWord(0,0,0));	
			printText("Type your name:", 1, 90, RGBToWord(225,225,225), RGBToWord(0,0,0));
			Getname();		
		}
	else
		{
			printText("Press ALT for menu", 1, 80, RGBToWord(225,225,225), RGBToWord(0,0,0));
			while(!altPressed())
			{}
		}
	delay(500);
	return score;
}












//Game relatred functions

int drawmap()
{
	int temp = 0;
	fillRectangle(0,0,127,165,RGBToWord(0,0,0));
	for(int Row = 0; Row < 10; Row++)
	{
		for(int Col = 0; Col < 8; Col++)
		{
			switch(PCmap[Row][Col]) {
				case '#':
					drawcomputer((Col*16)+1, (Row*16)+1, RGBToWord(0,225,0));
					break;
				case 's':
					putImage(((Col)*16)+1, ((Row)*16)+1, 15, 15, studentV, 0, 0);
					temp++;
					break;
				case '!':
					PCmap[Row][Col] = 's';
					putImage(((Col)*16)+1, ((Row)*16)+1, 15, 15, studentV, 0, 0);
					temp++;
					break;
				case '-':
					PCmap[Row][Col] = 's';
					putImage(((Col)*16)+1, ((Row)*16)+1, 15, 15, studentV, 0, 0);
					temp++;
					break;
			}

		}
	}
	return temp;
}

int AttemptCheater(int x)// 33.33333333333% chance to select a cheater out of availibe students. Returns 1 if successful, 0 if not.
{
	int temp = 0;
	if(rand() % 3 == 1)
	{
		x = rand() % x;


		for (int Row = 0; Row < 10; Row++)
			{
				for(int Col = 0; Col < 8; Col++)
					{
						if(temp == x && PCmap[Row][Col] == 's')
						{
							drawcomputer((Col)*16+1, (Row-1)*16+1, RGBToWord(225, 0, 0));
							PCmap[Row][Col] = '!';
							return 1;
						}
						else if (PCmap[Row][Col] == 's')
						{
							temp++;
						}
						
					}
			}
			


	}
	else
	{
		return 0;
	}
}

void Getname()
{
	resetNameField();
	while (1)
	{
		fillRectangle(1,100,127,30,RGBToWord(0,0,0));
		printText(name, 1, 100, RGBToWord(225,225,225), RGBToWord(0,0,0));
		printText("UP to Y, DOWN to N", 1, 110, RGBToWord(225,225,225), RGBToWord(0,0,0));
		delay(200);
		for(int i = 0; i < 9; i++)
		{
			name[i] = egetchar();
				if(name[i] == '\n')
				{
					name[i] = '\0';
					return;
				}
			fillRectangle(1,100,127,10,RGBToWord(0,0,0));
			printText(name, 1, 100, RGBToWord(225,225,225), RGBToWord(0,0,0));
		}
		name[9] = '\0';

		while(1)
		{
			if(uppressed())
			{
				return;
			}

			if(downpressed())
			{
				resetNameField();
				fillRectangle(1,100,127,10,RGBToWord(0,0,0));
				printText(name, 1, 100, RGBToWord(225,225,225), RGBToWord(0,0,0));
				delay(200);
				break;
			}
		}

		
	}
}

void resetNameField(void)
{
	for(int i = 0; i < 9; i++)
	{
		name[i] = '_';
	}
	name[9] = '\0';
}

int altPressed(){
	return (GPIOB->IDR & (1 << 0))==0;
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

void redLED(int e)
{
	if (e == 1)
	{
		GPIOA->ODR |= (1 << 0);
	}
	else{
		GPIOA->ODR &= ~(1 << 0);
	}
	
}

void drawcomputer(int x, int y, int Scolor)
{
	drawRectangle(x+2,y+1,11,7,RGBToWord(128,128,128));
	fillRectangle(x+3,y+2,10,6, Scolor);
	fillRectangle(x+6,y+9,5, 2,RGBToWord(128,128,128));

	fillRectangle(x+3,y+12,10,2,RGBToWord(128,128,128));
}
int checkpos(int x, int y)
{
				switch(PCmap[y/16][x/16]) {
				case 's':
					putImage(x, y, 15, 15, studentV, 0, 0);
					redLED(0);
					return 2;
					break;
				case '!':
					putImage(x, y, 15, 15, studentV, 0, 0);
					redLED(1);
					return 1;
					break;
				case '-':
					putImage(x,y, 15,15, blud, 0, 0);
					break;
				default:
					redLED(0);
					fillRectangle(x,y,15,15,RGBToWord(0,0,0));
					return 0;
					break;
			}
}
int checkpos2(int x, int y)
{
				switch(PCmap[y/16][x/16]) {
				case 's':
					return 2;
					break;
				case '!':
					return 1;
					break;
				case '-':
					return 3;
					break;
				default:
					return 0;
					break;
			}
}

void displayScore(int score)
{
	printText("Score:", 1, 1, RGBToWord(225,225,225), RGBToWord(0,0,0));
	printNumber(score, 45, 1, RGBToWord(225,225,225), RGBToWord(0,0,0));
}

int GameMenu()
{
	uint16_t CntSelection = 0;
	printText("CA Trials", 33, 1, RGBToWord(225,225,225), RGBToWord(0,0,0));
	
	while (1)
	{
	
	
		if (downpressed() && (milliseconds%MVE_DELAY==0))
		{
			CntSelection = (CntSelection + 1) % 2;
			delay(200);
		}
		if (uppressed() && (milliseconds%MVE_DELAY==0))
		{
			CntSelection = (CntSelection - 1) % 2;
			delay(200);
		}
		
		if((milliseconds%(MVE_DELAY)*2.5==0))
		{
			switch(CntSelection)
			{
				case 0:
					printText("Start", 33, 20, RGBToWord(225,0,0), RGBToWord(0,0,0));
					printText("Highscore", 33, 40, RGBToWord(128,128,128), RGBToWord(0,0,0));
					if(altPressed())
					{return 0;}
					break;
				case 1:
					printText("Start", 33, 20, RGBToWord(128,128,128), RGBToWord(0,0,0));
					printText("Highscore", 33, 40, RGBToWord(225,0,0), RGBToWord(0,0,0));
					if(altPressed())
					{return 1;}
					break;
			}

		}

	}
}


// System Functions
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

void initSysTick(void)
{
	SysTick->LOAD = 48000;
	SysTick->CTRL = 7;
	SysTick->VAL = 10;
	__asm(" cpsie i "); // enable interrupts
}



void setupIO()
{
	RCC->AHBENR |= (1 << 18) + (1 << 17); // enable Ports A and B
	display_begin();
	pinMode(GPIOB,0,0);
	pinMode(GPIOB,4,0);
	pinMode(GPIOB,5,0);
	pinMode(GPIOA,0,1);
	pinMode(GPIOA,8,0);
	pinMode(GPIOA,11,0);
	enablePullUp(GPIOB,0);
	enablePullUp(GPIOB,4);
	enablePullUp(GPIOB,5);
	enablePullUp(GPIOA,11);
	enablePullUp(GPIOA,8);
}

void initSerial()
{
	/* On the nucleo board, TX is on PA2 while RX is on PA15 */
	RCC->AHBENR |= (1 << 17); // enable GPIOA
	RCC->APB2ENR |= (1 << 14); // enable USART1
	pinMode(GPIOA,2,2); // enable alternate function on PA2
	pinMode(GPIOA,15,2); // enable alternate function on PA15
	// AF1 = USART1 TX on PA2
	GPIOA->AFR[0] &= 0xfffff0ff;
	GPIOA->AFR[0] |= (1 << 8);
	// AF1 = USART1 RX on PA15
	GPIOA->AFR[1] &= 0x0fffffff;
	GPIOA->AFR[1] |= (1 << 28);
	// De-assert reset of USART1 
	RCC->APB2RSTR &= ~(1u << 14);
	
	USART1->CR1 = 0; // disable before configuration
	USART1->CR3 |= (1 << 12); // disable overrun detection
	USART1->BRR = 48000000/9600; // assuming 48MHz clock and 9600 bps data rate
	USART1->CR1 |= (1 << 2) + (1 << 3); // enable Transmistter and receiver
	USART1->CR1 |= 1; // enable the UART

}

void eputchar(char c)
{
	while( (USART1->ISR & (1 << 6)) == 0); // wait for any ongoing transmission to finish
	USART1->ICR=0xffffffff; // clear any error that may be on the port
	USART1->TDR = c; // write the character to the Transmit Data Register
}
char egetchar()
{
	while( (USART1->ISR & (1 << 5)) == 0); // wait for a character
	return (char)USART1->RDR; // return the character that is waiting in the Receive Data Register
}
void eputs(char *String)
{
	while(*String) // keep printing until a NULL is found
	{
		eputchar(*String);
		String++;
	}
}
void printDecimal(int32_t Value)
{
	char DecimalString[12]; // a 32 bit value range from -2 billion to +2 billion approx
												// That's 10 digits
												// plus a null character, plus a sign
	DecimalString[11] = 0; // terminate the string;
	if (Value < 0)
	{
		DecimalString[0]='-';
		Value = -Value;
	}
	else
	{
		DecimalString[0]='+';
	}
	int index = 10;
	while(index > 0)
	{
		DecimalString[index]=(Value % 10) + '0';
		Value = Value / 10;
		index--;
	}
	eputs(DecimalString);
}

