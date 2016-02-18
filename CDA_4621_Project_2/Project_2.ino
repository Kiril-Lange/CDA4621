/*******************************************************************************
* Project_2.ino
*
* This file contains functions that are relevant to Project 2.
* It contains the main menu, submenus, and functions to perform each part of 
* the project.
*
* Kiril Lange, Andres Izurieta
*
* 2016/2/18
*******************************************************************************/


//numChoice is defined in the main project file

String Choices[numChoice] = {
	"Wall Distance",
	"Wall Follow",
	"Corridor Nav",
	"Calibrate"
};


void mainMenu(){
	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.print("Project 1 - Menu");
	lcd.setCursor(0, 1);
	lcd.print(Choices[Choice]);
	lcd.setBacklight(YELLOW);
	refreshLCD = false;
}

void choiceFunction(){
	if (Choice == 0)
		WallDistance();
	if (Choice == 1)
		WallFollow();
	if (Choice == 2)
		CorridorNav();
	if (Choice == 3)
		calibrate();
}

void WallDistance();

void WallFollow();

void CorridorNav();
