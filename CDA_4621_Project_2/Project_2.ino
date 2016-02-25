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
	lcd.print("Project 2 - Menu");
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

void WallDistance()
{
	//Vars
	double pLeft = 0;
	double pRight = 0;

	//Select K
	double K = selectK();
  lcd.setBacklight(GREEN);
	//select optimal distance 5 inches
	int distance = 5 * 25.4;
  // set start flag
  bool start = true;
  long int startTime = 0;
	//Begin loop
  countDown(3," ");
	while (1)
	{
		//update sensors
		updateIRSensors();
    //printIRDistance();
		//run control function on center
		double percent = ControlFWD(distance, K);
		pLeft = percent;
		pRight = percent;
		//go straight
		leftPercent = pLeft;
		rightPercent = pRight;
    //create a string to print to serial
    if (start)
    {
      startTime = millis();
      start = false;
    }
    //              time, distance, percent saturation
    String output = String( String((millis() - startTime)) + "," + String(centerDistance) + "," + String(percent));
    Serial.println(output);
    movePercent();
		//break loop when stopped
		uint8_t buttons = lcd.readButtons();

		if (buttons) {
			if (buttons & BUTTON_LEFT)
				return;
		}
		delay(100);
    if ((millis() - startTime) > 30000)
    {
      break;
    }
	}
}

void WallFollow()
{
	//Vars
	double pLeft = 0;
	double pRight = 0;
	bool Lwall = true;
	bool first = true;

	//Select K
	double K = selectK();
	//select optimal distance 5 inches
	int distance = 5 * 25.4;
	//Begin loop
	while (1)
	{

		//update sensors
		updateIRSensors();
    //printIRDistance();
		//if distance is nonexistent or >3* on all sensors
		if ((leftDistance == 0 || leftDistance > (3 * distance)) &&
			(rightDistance == 0 || rightDistance > (3 * distance)) &&
			(centerDistance == 0 || leftDistance > (3 * distance)))
		{
			//move forward optimal distance
			moveLine(distance * 1.2, MAXmms);
			//update sensors
			updateIRSensors;
			//if sensor data exists continue to next loop
			if (!((leftDistance == 0 || leftDistance > (3 * distance)) &&
				(rightDistance == 0 || rightDistance > (3 * distance)) &&
				(centerDistance == 0 || leftDistance > (3 * distance))))
			{
        first = true;
				continue;
			}
			//turn 90 degrees in the direction on the last known wall.
			if (Lwall)
				movePivot(90, MAXmms);
			else
				movePivot(-90, MAXmms);
			//if first time:
			if (first)
			{
				//move forward optimal distance
				moveLine(distance * 1.2, MAXmms);
				first = false;
			}
			//else:
			else
			{
				//move forward until L/R sensors are nonzero
				leftPercent = 1;
				rightPercent = 1;
				while ((leftDistance == 0 || leftDistance > (3 * distance)) &&
					(rightDistance == 0 || rightDistance > (3 * distance)))
				{
					movePercent();
					delay(100);
					updateIRSensors();
				}
				//reset first
				first = true;
			}
			//continue to next loop
			continue;
		}
		//determine what side the wall is on
		if (!rightDistance || leftDistance * 2 < rightDistance )
		{
			Lwall = true;
		}
		else if (!rightDistance || rightDistance * 2 < leftDistance )
		{
			Lwall = false;
		}
		//if front distance is less than or close to ( < 10%) optimal distance
    double pDist = distance * 1.1;
		if (centerDistance && centerDistance < pDist)
		{
			//pivot in oposite direction of closest wall
			if (Lwall)
				movePivot(-90, (MAXmms / 2));
			else
				movePivot(90, (MAXmms / 2));
			//continue to next loop
			continue;
		}
		//run control function on center
		pLeft = ControlFWD(distance, K);
		pRight = pLeft;
		//run control function on Left and right
		ControlLR(pLeft, pRight, distance, K);
		//assign global movement to local movement
		leftPercent = pLeft;
		rightPercent = pRight;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(leftPercent);
    lcd.setCursor(0, 1);
    lcd.print(rightPercent);
    //delay(1500);
		//go in a direction that optimises distance from the wall
		movePercent();
		//break loop when stopped
		uint8_t buttons = lcd.readButtons();

		if (buttons) {
			if (buttons & BUTTON_LEFT)
				return;
		}
    first = true;
		//delay(100);
	}
}

void CorridorNav()
{
	WallFollow();
}

double selectK()
{
  delay(500);
	bool refreshK = true;
	byte KChoice = 0;
	while (1)
	{
		uint8_t buttons = lcd.readButtons();
		if (refreshK){
			lcd.clear();
			lcd.setCursor(0, 0);
			lcd.print("Select K:");
			lcd.setCursor(0, 1);
			lcd.print((double)Kp[KChoice]/2.0);
			lcd.setBacklight(YELLOW);
			refreshK = false;
		}
		if (buttons) {
			if (buttons & BUTTON_UP) {
				refreshK = true;
				if (KChoice == 0)
					KChoice = numKp- 1;
				else
					KChoice = KChoice - 1;
			}
			if (buttons & BUTTON_DOWN) {
				refreshK = true;
				KChoice = KChoice + 1;
				if (KChoice == numKp)
					KChoice = 0;
			}
			if (buttons & BUTTON_SELECT) {
				return ((double)Kp[KChoice] / 2.0);
			}
		}
		delay(100);
	}
}

double ControlFWD(int distance, double K)
{
	//forward sensor value - optimal distance
	int error = centerDistance - distance;
  if (!centerDistance)
    error = 4000;
	//Saturation curve value
	int strength = K * error;
	//percent of maximum
	double percent = SatFunc(strength);

	return percent;
}

double SatFunc(int strength)
{
	//percent of maximum
	double percent = ((double)strength) / 25.4 / 10.0;
	
	//Check for max and min saturation
	if (percent > 1)
		percent = 1;
	else if (percent < -1)
		percent = -1;

	return percent;

}

void ControlLR(double &pLeft, double &pRight, int distance, double K)
{
  //if in a corridor
  bool isCRD = false;
  
  if ((leftDistance != 0 && leftDistance < (distance * 2)) &&
  (rightDistance != 0 && rightDistance < (distance * 2)))
  {
    distance = (leftDistance + rightDistance) / 2;
    isCRD = true;
  }
	//negative is too close, positive is too far
	int Lerror = leftDistance - distance;
	int Lstrength = K * Lerror;
	int Rerror = rightDistance - distance;
	int Rstrength = K * Rerror;
	//Using the left sensor
	if ((leftDistance != 0 && leftDistance < (distance * 3)) || 
	( isCRD && Lerror < 0 ))
	{
		double percent = SatFunc(Lstrength);
		if (Lerror > 0)
			pLeft = pLeft - percent;
		else
			pRight = pRight - abs(percent);
	}

	//Using the right sensor
	else if (rightDistance != 0 && rightDistance < (distance * 3) || 
  ( isCRD && Rerror < 0 ))
	{
		double percent = SatFunc(Rstrength);
		if (Rerror > 0)
			pRight = pRight - percent;
		else
			pLeft = pLeft - abs(percent);
	}
}
