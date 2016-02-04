/*******************************************************************************
* Project_1.ino
*
* This file contains functions that are relevant to Project 1.
* It contains the main menu, submenus, and functions to perform each part of 
* the project.
*
* Kiril Lange, Andres Izurieta
*
* 2016/2/4
*******************************************************************************/


//numChoice is defined in the main project file

String Choices[numChoice] = {
	"Sensors",
	"F & R 50in",
	"CW Circle 20in",
	"CCW Circle 20in",
	"CW Square 20in",
	"CCW Square 20in",
	"Figure8 20-10-20",
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
		sensorFunction();
	if (Choice == 1)
		forwardReverse50();
	if (Choice == 2)
		CWCircle20in();
	if (Choice == 3)
		CCWCircle20in();
	if (Choice == 4)
		CWSquare20in();
	if (Choice == 5)
		CCWSquare20in();
	if (Choice == 6)
		Figure8_20_10_20();
	if (Choice == 7)
		calibrate();
}

void sensorFunction(){
	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.print("Right|Centr|Left");
	while (1){
		uint8_t buttons = lcd.readButtons();
		if (buttons) {
			if (buttons & BUTTON_LEFT) {
				return;
			}
		}
		updateIRSensors();
		printIRDistance();
		delay(500);
	}
}

void forwardReverse50(){
	countDown(5, "F & R 50in");
	moveLine(1270, 70);
	moveLine(1270, -70);
}

void CWCircle20in(){
	countDown(5, "CW Circle 20in");
	moveArc(254, 360, 1, -1, 70);
}

void CCWCircle20in(){
	countDown(5, "CCW Circle 20in");
	moveArc(254, 360, 1, 1, 70);
}

void CWSquare20in(){
	countDown(5, "CW Square 20in");
	moveLine(508, 70);
	movePivot(-90, 70);
	moveLine(508, 70);
	movePivot(-90, 70);
	moveLine(508, 70);
	movePivot(-90, 70);
	moveLine(508, 70);
}

void CCWSquare20in(){
	countDown(5, "CCW Square 20in");
	moveLine(508, 70);
	movePivot(90, 70);
	moveLine(508, 70);
	movePivot(90, 70);
	moveLine(508, 70);
	movePivot(90, 70);
	moveLine(508, 70);
}

void Figure8_20_10_20(){
	countDown(5, "Figure8 20-10-20");
}