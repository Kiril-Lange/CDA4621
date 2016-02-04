/*******************************************************************************
* AAA_Movement.ino
* 
* This file contains functions that are relevant to the robot's movement.
* It needs to be included in any Project that requires the use of the servos
*
* Kiril Lange, Andres Izurieta
*
* 2016/2/4
*******************************************************************************/


//Servo forward function
int ServoFwdLookup(double rate){
	double temp = (rate + 14.107) / 2.2139;
	return (int)temp;
}
//Servo reverse function
int ServoRevLookup(double rate){
	double temp = (rate - 1.4333) / 2.1186;
	return (int)temp;
}

//This funtion calculates a path from the current position to a destination
void calculatePath(int destx, int desty, int rate){
	//calculate any turns

	//drive in straight line to point
}

//Describes turning the robot about it's center  
//Theta Direction: Positive | Negative
void movePivot(int theta, int rate){
	//We need to know some details of the robot
	//specifically wheelbase and the wheel function
	double perDegree = 102.0 * 3.14159 / 360.0;
	//calculate runtime
	double time = perDegree * abs(theta) / rate * 1000;
	//write to servo
	//left 
	if ((int)theta > 0){
		int leftFreq = ServoRevLookup(rate);
		int rightFreq = ServoFwdLookup(rate);

		LServo.writeMicroseconds(1500 - leftFreq);
		RServo.writeMicroseconds(1500 - rightFreq);
	}

	else if ((int)theta < 0){
		int leftFreq = ServoFwdLookup(rate);
		int rightFreq = ServoRevLookup(rate);

		LServo.writeMicroseconds(1500 + leftFreq);
		RServo.writeMicroseconds(1500 + rightFreq);
	}
	else{
		return;
	}

	delay((unsigned long)time);
	LServo.writeMicroseconds(1500);
	RServo.writeMicroseconds(1500);
	//loop until milis have been reached

	//write to servo
}

//Interactive controls to move the robot at a given speed for 2 seconds
void calibrate(){
	int trefreshLCD = true;
	int freq = 0;
	delay(100);
	while (1){

		uint8_t buttons = lcd.readButtons();
		if (trefreshLCD){
			lcd.clear();
			lcd.setCursor(0, 0);
			lcd.print("Dist Cal");
			lcd.setCursor(0, 1);
			lcd.print(freq);
			lcd.setBacklight(RED);
			trefreshLCD = false;
		}
		if (buttons) {
			if (buttons & BUTTON_UP) {
				trefreshLCD = true;
				freq = freq + 5;
			}
			if (buttons & BUTTON_DOWN) {
				trefreshLCD = true;
				freq = freq - 5;
			}
			if (buttons & BUTTON_LEFT) {
				return;
			}
			if (buttons & BUTTON_SELECT) {
				trefreshLCD = true;
				countDown(3, "Dist Cal");
				LServo.writeMicroseconds(1500 + freq);
				RServo.writeMicroseconds(1500 - freq);
				delay(2000);
				LServo.writeMicroseconds(1500);
				RServo.writeMicroseconds(1500);
			}
		}
		delay(100);
	}
}

//Moves the robot a given distance at a given speed
void moveLine(int distance, int rate){
	//calculate runtime
	unsigned long time = (double)abs(distance) / (double)abs(rate) * 1000;
	//write to servo
	if (rate > 0){
		int temp = ServoFwdLookup(rate);
		LServo.writeMicroseconds(1500 + temp);
		RServo.writeMicroseconds(1500 - temp);
	}
	else if (rate < 0){
		int temp = ServoRevLookup(abs(rate));
		LServo.writeMicroseconds(1500 - temp);
		RServo.writeMicroseconds(1500 + temp);
	}
	else
		return;
	delay(time);
	LServo.writeMicroseconds(1500);
	RServo.writeMicroseconds(1500);

}

//Moves the robot a certain number of degrees around a circle
//       x
//   1,1 |  1,-1
// y-----|------
//  -1,1 | -1,-1
void moveArc(int radius, double theta, int x, int y, int rate){
	//Calculate circle circuference per degree
	double outerpd = (radius * 2.0 * 3.14159) / 360.0;
	double innerpd = ((radius - 102) * 2.0 * 3.14159) / 360.0;
	//calculate inner rate in mm/s
	double innerRate = innerpd / outerpd * rate;

	//calculate runtime in ms
	double time = outerpd * theta / rate * 1000;
	//write to servo
	//FCCW
	if (x > 0 && y > 0){
		int outerFreq = ServoFwdLookup(rate);
		int innerFreq = ServoFwdLookup((int)innerRate);

		LServo.writeMicroseconds(1500 + innerFreq);
		RServo.writeMicroseconds(1500 - outerFreq);
	}
	//FCW
	else if (x > 0 && y < 0){
		int outerFreq = ServoFwdLookup(rate);
		int innerFreq = ServoFwdLookup((int)innerRate);

		LServo.writeMicroseconds(1500 + outerFreq);
		RServo.writeMicroseconds(1500 - innerFreq);
	}
	//RCCW
	else if (x < 0 && y < 0){
		int outerFreq = ServoRevLookup(rate);
		int innerFreq = ServoRevLookup((int)innerRate);

		LServo.writeMicroseconds(1500 - outerFreq);
		RServo.writeMicroseconds(1500 - innerFreq);
	}
	//RCW
	else if (x < 0 && y > 0){
		int outerFreq = ServoRevLookup(rate);
		int innerFreq = ServoRevLookup((int)innerRate);

		LServo.writeMicroseconds(1500 - innerFreq);
		RServo.writeMicroseconds(1500 - outerFreq);
	}

	delay((unsigned long)time);
	LServo.writeMicroseconds(1500);
	RServo.writeMicroseconds(1500);
}
