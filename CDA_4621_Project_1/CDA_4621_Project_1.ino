/*******************************************************************************
* CDA_4621_Project_1.ino
*
* This is the main file for this project, it contains the includes, defines, and
* global variables for any associated file.
*
* This file should contan as little project specific code as possible and be
* should be reused for future projects.
*
* Kiril Lange, Andres Izurieta
*
* 2016/2/4
*******************************************************************************/

//Libraries
#include <Wire.h>
#include <Adafruit_RGBLCDShield.h>
#include <Adafruit_MCP23017.h>
#include <Servo.h>

//LCD screen
Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();

//LCD defines
#define RED 0x1
#define YELLOW 0x3
#define GREEN 0x2
#define TEAL 0x6
#define BLUE 0x4
#define VIOLET 0x5
#define WHITE 0x7

//Choice Dialogs
#define numChoice 8

//Servos
Servo LServo;
Servo RServo;

//IR sensors
const int SFSensor = A0;
const int SLSensor = A1;
const int SRSensor = A2;
const int LFSensor = A3;

//Global vars
int Select = 0;
bool refreshLCD = true;
int leftDistance = 0;
int rightDistance = 0;
int centerDistance = 0;
int Choice = 0;
//quick testing variables
int lsm = 1500;
int rsm = 1500;

void setup() {

	LServo.attach(2);
	RServo.attach(3);

	LServo.writeMicroseconds(1500);
	RServo.writeMicroseconds(1500);
	
	Serial.begin(9600);
	lcd.begin(16, 2);
}

void loop() {

	uint8_t buttons = lcd.readButtons();
	if (refreshLCD){
		mainMenu();
	}
	if (buttons) {
		if (buttons & BUTTON_UP) {
			refreshLCD = true;
			if (Choice == 0)
				Choice = numChoice - 1;
			else
				Choice = Choice - 1;
		}
		if (buttons & BUTTON_DOWN) {
			refreshLCD = true;
			Choice = Choice + 1;
			if (Choice == numChoice)
				Choice = 0;
		}
		if (buttons & BUTTON_LEFT) {
			lcd.setCursor(0, 1);
			lsm = lsm + 1;
			LServo.writeMicroseconds(lsm);
			lcd.print((String)lsm + "|" + (String)rsm);
		}
		if (buttons & BUTTON_RIGHT) {
			lcd.setCursor(0, 1);
			rsm = rsm - 1;
			RServo.writeMicroseconds(rsm);
			lcd.print((String)lsm + "|" + (String)rsm);
		}
		if (buttons & BUTTON_SELECT) {
			refreshLCD = true;
			choiceFunction();
			LServo.writeMicroseconds(1500);
			RServo.writeMicroseconds(1500);
		}
	}
	delay(100);
}