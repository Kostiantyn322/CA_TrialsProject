#include <stm32f031x6.h>
#include <stdlib.h>
#include "display.h"
#include "sound.h"
#include "musical_notes.h"

#define MVE_DELAY 100 // game tick interval in milliseconds
#define BEAT_MS 300	  // duration of music beats in milliseconds
#define CHEAT_LIMIT 3 // maximum amount of cheaters
#define TIME_LIMIT 30 // time limit in seconds



// system functions
void initClock(void);
void initSysTick(void);
void initSerial();
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
void printDecimal(int32_t Value);

// input/output functions
int rightpressed();
int leftpressed();
int downpressed();
int uppressed();
int altpressed(); // ALT Button
void redLED(int); // LED control function, 1 for on, 0 for off

// game functions
int PlayGame(int);
int drawmap(); // Draws the map and resets any dead or cheating students
void displayScore(int); // Displays the current score
int AttemptCheater(int); // Attempts to randomly make a student a cheater
void drawcomputer(int x, int y, int Scolor); // Draws a computer at the given x and y coordinates. Scolor specifies color of the screen
int checkLED(int x, int y); // Function to check the current player pos. Returns 0 for empty, 1 for cheater(LED turns on), 2 for student and restore the image.
int checkpos(int x, int y); // Function to check the current player pos. Returns 0 for empty, 1 for cheater, 2 for student. This one doesnt restore

// menu functions
int GameMenu();
void Getname();
void resetNameField();





// game sprites
const uint16_t teachH[]= {0,8046,8046,8046,8046,8046,8046,8046,13294,13294,13294,13294,0,0,0,0,8046,8046,8046,8046,8046,8046,8046,13294,13294,13294,13294,0,0,0,0,8046,8046,8046,8046,8046,8046,8046,13294,13294,13294,13294,0,0,0,0,0,0,0,0,0,0,0,13294,13294,13294,13294,0,0,0,0,0,0,0,0,0,8046,8046,8046,8046,8046,8046,8046,8046,0,0,0,0,0,0,0,8046,40224,8046,22355,8046,8046,8046,8046,0,0,0,0,0,0,0,8046,0,22355,22355,8046,8046,8046,8046,0,0,0,0,0,0,0,8046,8046,8046,8046,8046,8046,8046,8046,0,0,0,0,0,0,0,8046,0,22355,22355,8046,8046,8046,8046,0,0,0,0,0,0,0,8046,40224,8046,22355,8046,8046,8046,8046,0,0,0,0,0,0,0,8046,8046,8046,8046,8046,8046,8046,8046,0,0,0,0,0,0,0,0,0,13294,13294,13294,13294,0,0,0,0,8046,8046,8046,8046,8046,8046,8046,13294,13294,13294,13294,0,0,0,0,8046,8046,8046,8046,8046,8046,8046,13294,13294,13294,13294,0,0,0,0,8046,8046,8046,8046,8046,8046,8046,13294,13294,13294,13294,0,0,0,};
const uint16_t teachV[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8046,8046,8046,8046,8046,8046,8046,0,0,0,0,0,0,0,0,8046,8046,8046,8046,8046,8046,8046,0,0,0,0,13294,13294,13294,13294,8046,8046,8046,8046,8046,8046,8046,13294,13294,13294,13294,13294,13294,13294,13294,8046,8046,8046,8046,8046,8046,8046,13294,13294,13294,13294,13294,13294,13294,13294,8046,22355,22355,8046,22355,22355,8046,13294,13294,13294,13294,13294,13294,13294,13294,8046,8046,22355,8046,22355,8046,8046,13294,13294,13294,13294,8046,8046,8046,0,8046,40224,0,8046,0,40224,8046,0,8046,8046,8046,8046,8046,8046,0,8046,8046,8046,8046,8046,8046,8046,0,8046,8046,8046,8046,8046,8046,0,0,0,0,0,0,0,0,0,8046,8046,8046,8046,8046,8046,0,0,0,0,0,0,0,0,0,8046,8046,8046,8046,8046,8046,0,0,0,0,0,0,0,0,0,8046,8046,8046,8046,8046,8046,0,0,0,0,0,0,0,0,0,8046,8046,8046,8046,8046,8046,0,0,0,0,0,0,0,0,0,8046,8046,8046,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,};
const uint16_t studentV[]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,40375,40375,40375,40375,0,0,0,0,0,0,0,40375,40375,40375,40375,40375,40375,40375,40375,0,0,0,0,0,0,0,40375,40375,40375,40375,40375,40375,40375,40375,0,0,0,0,0,0,0,40375,40375,40375,40375,40375,40375,40375,40375,0,0,0,0,0,0,0,40375,40375,40375,40375,40375,40375,40375,40375,0,0,0,0,0,0,0,40375,40375,40375,40375,40375,40375,40375,40375,56685,56685,56685,56685,56685,56685,56685,40375,40375,40375,40375,16621,16621,16621,16621,56685,65535,0,56685,0,65535,56685,16621,16621,16621,16621,16621,16621,16621,16621,56685,56685,56685,56685,56685,56685,56685,16621,16621,16621,16621,16621,16621,16621,16621,45056,45056,45056,45056,45056,45056,45056,16621,16621,16621,16621,0,0,0,0,45056,45056,45056,45056,45056,45056,45056,0,0,0,0,0,0,0,0,45056,45056,45056,45056,45056,45056,45056,0,0,0,0,0,0,0,0,45056,0,45056,0,45056,0,45056,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,};
const uint16_t blud[]= {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,4,0,0,0,0,0,0,0,0,0,0,4,4,0,0,0,0,0,0,0,0,0,4,4,4,4,0,0,0,0,0,0,0,4,4,4,4,4,4,0,0,0,0,7936,7936,4,4,4,4,4,4,7936,7936,0,0,65535,65535,4,4,4,4,4,4,65535,65535,0,0,7936,7936,4,4,4,4,4,4,7936,7936,0,0,0,0,0,4,4,4,4,0,0,0,0,0,0,0,0,0,24327,24327,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,};

// game's soundtrack
const int notes[] = {E4, A4, A4, C4, C4, A4, E4, E4, B4, A4, G4, F4, E4, E4,};

// name of high score holder
char name[10] = {"________"};

// the map of the game. # is a computer, s is a student, ! is a cheater, - is a dead student, and . is an empty space
// at game start, there are no cheaters or dead students
// top row is out of bounds, reserved for score
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





int main() {
	int highscore = 0;
	int buffer = 0;
	
	// systems init
	initClock();
	initSound();
	initSysTick();
	setupIO();
	initSerial();

	fillRectangle(0,0,127,165,RGBToWord(0,0,0)); // set screen to black

	//Game menu section
	while (1) {
		buffer = GameMenu(); // call main menu function
		
		// act on main menu selection
		switch (buffer) {

			case 0:
				// run the game
				eputs("Starting the Game\n"); // serial output
				buffer = PlayGame(highscore);
				if (buffer > highscore) { // update high score if necessary
					highscore = buffer;
				} // if
				break;

			case 1:
				// view the high score
				eputs("Display Highscore\n"); // serial output
				fillRectangle(0,0,127,165,RGBToWord(0,0,0));
				printText("Current Highscore:", 1, 20, RGBToWord(225,225,225), RGBToWord(0,0,0));
				printText(name, 1, 30, RGBToWord(225,225,225), RGBToWord(0,0,0));
				printNumber(highscore, 80, 30, RGBToWord(225,225,225), RGBToWord(0,0,0));

				while (!rightpressed() || !leftpressed()) {} // hold until left and right are pressed
				break;

		} // switch
		fillRectangle(0,0,128,165,RGBToWord(0,0,0));
	} // while

	return 0;
} // main





// Runs the game. Takes the current highscore as argument to see if surpassed
int PlayGame(int highscore) {

	int tune = 0;							   // current note of the soundtrack to play
	int NoteCount = sizeof(notes)/sizeof(int); // amount of notes in the soundtrack
	uint16_t x = 65;						   // player's x coordinate
	uint16_t y = 17;						   // player's y coordinate
	int score = 0;							   // current score
	int CheaterCount = 0;					   // amount of cheaters
	int StudentCount = drawmap();			   // amount of students
	int buffer = 0;
	milliseconds = 0;						   // sets the global clock back to 0

	drawmap();						  // draw the global map
	putImage(x,y,15,15,teachV, 0, 0); // draw the player character
	delay(50);
	redLED(0);						  // set the LED to off



	// main game loop
	while ( milliseconds < (TIME_LIMIT*1000) ) {

		// soundtrack
		// if current note has played for long enough, move to next
		if (milliseconds % BEAT_MS == 0) {
			playNote((notes[tune]));
			tune = (tune + 1) % NoteCount; // increment to next note and loop if end reached
		} // if

		displayScore(score); // display score at top of screen

		if (StudentCount == 0 && CheaterCount == 0) { // check if every student was killed

			score = score + (TIME_LIMIT-(milliseconds/1000)); // add remaining time to the score
			fillRectangle(0,0,128,165,RGBToWord(0,0,0)); // clear screen

			// display score
			printText("Score:", 33, 40, RGBToWord(225,225,225), RGBToWord(0,0,0));
			printNumber(score, 75, 40, RGBToWord(225,0,0), RGBToWord(0,0,0));

			playNote(0); // turn off speaker
			redLED(0);   // turn off LED

			if (score > highscore) { // check if score was higher than high score
				printText("New Highscore!", 1, 80, RGBToWord(225,225,225), RGBToWord(0,0,0));	
				printText("Type your name:", 1, 90, RGBToWord(225,225,225), RGBToWord(0,0,0));
				Getname(); // update record holder name
			} else {
				printText("Press ALT for menu", 1, 80, RGBToWord(225,225,225), RGBToWord(0,0,0));
				while(!altpressed()) {} // hold until alt pressed
			} // if/else

			delay(500);
			return score;
		} // if

		// attempt to add a new cheater if below the limit and there are still students left
		if ((CheaterCount < CHEAT_LIMIT && StudentCount != 0 ) && (milliseconds%(MVE_DELAY*5)==0)) {
			buffer = AttemptCheater(StudentCount);
			StudentCount -= buffer;
			CheaterCount += buffer;
		} // if

		// attempt to kill any cheater the player might be hovering over
		if (altpressed() && checkpos(x,y) == 1) {
			CheaterCount -= 1;
			score += 1;
			PCmap[y/16][x/16] = '-'; // update cheater to dead student
			drawcomputer(x, (y-16), RGBToWord(0,0,255));
			playNote(C5);
			redLED(0); // turn off the LED
		} // if

		// movement section
		// player can move one grid square per game tick
		if (uppressed() && (milliseconds%MVE_DELAY==0)) { // upwards movement

			if ((y - 16) < 17) { // stop player moving out of bounds
				y = 17;
				eputchar('1');
			} else {
				if (checkLED(x,y) == 1) { // light up LED if player is standing on a cheater
					redLED(1);
				} else {
					redLED(0);
				} // if/else
				y = y - 16; // update position
			} // if/else

			if (PCmap[(y/16)][(x/16)]  == '#') { // stop the player moving over a computer
				y = y + 16;
				eputchar('2');
			} // if

			if (checkLED(x,y) == 1) { // update LED
				redLED(1);
			} else {
				redLED(0);
			} // if/else

			putImage(x,y,15,15,teachV, 0, 1); // draw teacher on new position

		} // if

		if (downpressed() && (milliseconds%MVE_DELAY==0)) { // downwards movement

			if ((y + 16) > 145) { // stop player moving out of bounds
				y = 145;
				eputchar('1');
			} else {
				if (checkLED(x,y) == 1) { // light up LED if player is standing on a cheater
					redLED(1);
				} else {
					redLED(0);
				} // if/else
				y = y + 16; // update position
			} // if/else

			if(PCmap[(y/16)][(x/16)]  == '#') { // stop the player moving over a computer
				y = y - 16;
				eputchar('2');
			} // if

			if (checkLED(x,y) == 1) { // update LED
				redLED(1);
			} else {
				redLED(0);
			} // if/else

			putImage(x,y,15,15,teachV, 0, 0); // draw teacher on new position

		} // if

		if (leftpressed() && (milliseconds%MVE_DELAY==0)) { // leftwards movement

			if ((x - 16) < 1) { // stop the player moving out of bounds
				x = 1;
				eputchar('1');
			} else {
				if (checkLED(x,y) == 1) { // light up LED if player is standing on a cheater
					redLED(1);
				} else {
					redLED(0);
				} // if/else
				x = x - 16; // update position
			} // if/else

			if(PCmap[(y/16)][(x/16)]  == '#') { // stop the player moving over a computer
				x = x + 16;
				eputchar('2');
			} // if

			if (checkLED(x,y) == 1) { // update LED
				redLED(1);
			} else {
				redLED(0);
			} // if/else

			putImage(x,y,15,15,teachH, 0, 0); // draw teacher on new position

		} // if

		if (rightpressed() && (milliseconds%MVE_DELAY==0)) { // rightwards movement

			if ((x + 16) > 113) { // stop the player moving out of bounds
				x = 113;
				eputchar('1');
			} else {
				if (checkLED(x,y) == 1) { // light up LED if player is standing on a cheater
					redLED(1);
				} else {
					redLED(0);
				} // if/else
				x = x + 16; // update position
			} // if/else

			if(PCmap[(y/16)][(x/16)]  == '#') { // stop the player moving over a computer
				x = x - 16;
				eputchar('2');
			} // if

			if (checkLED(x,y) == 1) { // update LED
				redLED(1);
			} else {
				redLED(0);
			} // if/else

			putImage(x,y,15,15,teachH, 1, 0); // draw teacher on new position

		} // if

		// exit to main menu
		if ( rightpressed() && leftpressed() && altpressed() ) {
			playNote(0); // turn off speaker
			delay(500);
			return score;
		} // if

	} // while - main game loop



	// end game screen
	fillRectangle(0,0,128,165,RGBToWord(0,0,0));
	printText("Time's up!", 33, 20, RGBToWord(225,225,225), RGBToWord(0,0,0));
	printText("Score:", 1, 40, RGBToWord(225,225,225), RGBToWord(0,0,0));
	printNumber(score, 65, 40, RGBToWord(225,0,0), RGBToWord(0,0,0));
	playNote(0); // turn off speaker
	redLED(0);	 // turn off LED

	if (score > highscore) {	
		printText("New Highscore!", 1, 80, RGBToWord(225,225,225), RGBToWord(0,0,0));	
		printText("Type your name:", 1, 90, RGBToWord(225,225,225), RGBToWord(0,0,0));
		Getname(); // upade record holder name
	} else {
		printText("Press ALT for menu", 1, 80, RGBToWord(225,225,225), RGBToWord(0,0,0));
		while (!altpressed()) {} // hold until alt pressed
	} // if/else

	delay(500);
	return score;
} // PlayGame





// draws the game layout on screen based on the global array, also resets dead or cheating students
int drawmap() {
	int student_count = 0;
	fillRectangle(0,0,127,165,RGBToWord(0,0,0));

	for (int Row = 0; Row < 10; Row++) {
		for(int Col = 0; Col < 8; Col++) {

			switch (PCmap[Row][Col]) {
				case '#':
					drawcomputer((Col*16)+1, (Row*16)+1, RGBToWord(225,0,0));
					break;
				case 's':
					putImage(((Col)*16)+1, ((Row)*16)+1, 15, 15, studentV, 0, 0);
					student_count++;
					break;
				case '!':
					PCmap[Row][Col] = 's';
					putImage(((Col)*16)+1, ((Row)*16)+1, 15, 15, studentV, 0, 0);
					student_count++;
					break;
				case '-':
					PCmap[Row][Col] = 's';
					putImage(((Col)*16)+1, ((Row)*16)+1, 15, 15, studentV, 0, 0);
					student_count++;
					break;
			} // switch

		} // for
	} // for

	return student_count; // return amount of students in map layout

} // drawmap

// 33% chance to select a cheater out of availibe students. Returns 1 if successful, 0 if not.
int AttemptCheater(int x) {
	// x is initially amount of available students
	int increment = 0; // increments until the selected student is reached

	// attempts to create a cheater
	if (rand() % 3 == 1) {
		x = rand() % x; // select random student

		for (int Row = 0; Row < 10; Row++) {
			for (int Col = 0; Col < 8; Col++) {

				if(increment == x && PCmap[Row][Col] == 's') { // selected student found
					drawcomputer((Col)*16+1, (Row-1)*16+1, RGBToWord(225, 0, 0)); // change cheater's screen color
					PCmap[Row][Col] = '!'; // update cheater on map
					return 1;
				} else if (PCmap[Row][Col] == 's') {
						increment++;
				} // if/elseif

			} // for
		} // for

	} else {
		// failed to create cheater
		return 0;
	} // if/else

} // AttemptCheater

// updates the name of the record holder
void Getname() {
	resetNameField(); // clear name
	
	while (1) {
		fillRectangle(1,100,127,30,RGBToWord(0,0,0));
		printText(name, 1, 100, RGBToWord(225,225,225), RGBToWord(0,0,0));
		printText("UP for accept, DOWN for reset", 1, 110, RGBToWord(225,225,225), RGBToWord(0,0,0));
		delay(200);

		// get player name from Serial Input, pressing enter confirms
		for (int i = 0; i < 9; i++) {
			name[i] = egetchar();

			if (name[i] == '\n') {
				name[i] = '\0';
				break;
			} // if

			fillRectangle(1,100,127,10,RGBToWord(0,0,0));
			printText(name, 1, 100, RGBToWord(225,225,225), RGBToWord(0,0,0));
		} // for
		name[9] = '\0';

		// approve or disapprove entered name
		while(1) {
			if (uppressed()) { // approved
				return;
			} // if

			if (downpressed()) { // disapproved
				resetNameField(); // reset field
				fillRectangle(1,100,127,10,RGBToWord(0,0,0));
				printText(name, 1, 100, RGBToWord(225,225,225), RGBToWord(0,0,0));
				delay(200);
				break;
			} // if
		} // while

	} // while
} // Getname

// resets the name of the record holder
void resetNameField() {
	for (int i = 0; i < 9; i++) {
		name[i] = '_';
	} // for
	name[9] = '\0';
} // resetNameField

// alt button check
int altpressed() {
	return (GPIOB->IDR & (1 << 0))==0;
} // altpressed

// right button check
int rightpressed() {
	return (GPIOB->IDR & (1 << 4))==0;
} // rightpressed

// left button check
int leftpressed() {
	return (GPIOB->IDR & (1 << 5))==0;
} // leftpressed

// down button check
int downpressed() {
	return (GPIOA->IDR & (1 << 11)) == 0;
} // downpressed

// up button check
int uppressed() {
	return (GPIOA->IDR & (1 << 8)) == 0;
} // uppressed

// turns the LED on or off
void redLED(int on) {
	if (on == 1) {
		GPIOA->ODR |= (1 << 0);
	} else {
		GPIOA->ODR &= ~(1 << 0);
	} // if/else
} // redLED

// function to draw a computer, with arguments for colour rather than coding in sprites
void drawcomputer(int x, int y, int Scolor) {
	drawRectangle(x+2,y+1,11,7,RGBToWord(128,128,128));
	fillRectangle(x+3,y+2,10,6, Scolor);
	fillRectangle(x+6,y+9,5, 2,RGBToWord(128,128,128));
	fillRectangle(x+3,y+12,10,2,RGBToWord(128,128,128));
} // drawcomputer

// checks whether the LED should be turned on
// return values based on map characters
int checkLED(int x, int y) {
	switch (PCmap[y/16][x/16]) {
		case 's':
			putImage(x, y, 15, 15, studentV, 0, 0);
			eputchar('s');
			return 2;
			break;
		case '!':
			eputchar('!');
			return 1;
			break;
		case '-':
			fillRectangle(x,y,15,15,RGBToWord(0,0,225));
			eputchar('-');
			break;
		default:
			fillRectangle(x,y,15,15,RGBToWord(0,0,0));
			eputchar('.');
			return 0;
			break;
	} // switch
} // checkLED

// check what what entity the player is standing over, without drawing over anything
int checkpos(int x, int y) {
	switch (PCmap[y/16][x/16]) {
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
	} // switch
} // checkpos

// displays the current score at the top of the screen
void displayScore(int score) {
	printText("Score:", 1, 1, RGBToWord(225,225,225), RGBToWord(0,0,0));
	printNumber(score, 45, 1, RGBToWord(225,225,225), RGBToWord(0,0,0));
} // displayScore

// handles the main menu, returns 0 if the player selected start game, 1 if the player selected high score
int GameMenu() {
	uint16_t CntSelection = 0; // current selection
	printText("CA Trials", 33, 1, RGBToWord(225,225,225), RGBToWord(0,0,0)); // title
	
	while (1) {
		// menu navigation
		if (downpressed() && (milliseconds%MVE_DELAY==0)) {
			CntSelection = (CntSelection + 1) % 2;
			delay(200);
		} // if
		if (uppressed() && (milliseconds%MVE_DELAY==0)) {
			CntSelection = (CntSelection - 1) % 2;
			delay(200);
		} // if
		
		// option selection
		if ( milliseconds%(MVE_DELAY)*2.5==0 ) {
			switch(CntSelection) {
				case 0:
					// select game start
					printText("Start", 33, 20, RGBToWord(225,0,0), RGBToWord(0,0,0));
					printText("Highscore", 33, 40, RGBToWord(128,128,128), RGBToWord(0,0,0));
					if (altpressed()) { return 0; }
					break;
				case 1:
					// select view high score
					printText("Start", 33, 20, RGBToWord(128,128,128), RGBToWord(0,0,0));
					printText("Highscore", 33, 40, RGBToWord(225,0,0), RGBToWord(0,0,0));
					if (altpressed()) { return 1; }
					break;
			} // switch
		} // if
	} // while
} // GameMenu













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

