/*******************************************************************************
* AAB_Sensors.ino
*
* This file contains functions that are relevant to sensing the enviornment.
* It needs to be included in any Project that requires external sensing and 
* path planning.
*
* Kiril Lange, Andres Izurieta
*
* 2016/2/4
*******************************************************************************/

//Short range sensor distance values in milimeters
int getShortDistance(int voltage) {
	if (voltage < 50 || voltage > 600)
		return 0;
	//calculate distance in mm from formula
	double distance = 0.0;
	double coefficient = 3858.8;
	double power = 1.074;
	distance = coefficient / (pow((double)voltage, power)) * 10;
	if (distance < 0)
		return 0;
	return (int)distance;
}

//Short range sensor distance values in milimeters
int getLongDistance(int voltage) {
	if (voltage < 80 || voltage > 600)
		return 0;
	//calculate distance in mm from formula
	double coefficient = 22392.0;
	double power = 1.107;
	double distance = coefficient / (pow((double)voltage, power)) * 10;
	if (distance < 0)
		return 0;
	return (int)distance;
}

//Merges the short and long range distance sensor values
int mergeDistance(int shortDist, int longDist) {
	if (longDist >= 300 && (!shortDist || shortDist > 300))
		return longDist;
	if (shortDist <= 200)
		return shortDist;
	if (longDist < 300 && longDist > 200 && shortDist < 300 && shortDist > 200){
		double shortFactor = ((double)shortDist - 200.0) / 100.0;
		double longFactor = ((double)longDist - 200.0) / 100.0;
		double mergeFactor = (shortFactor + longFactor) / 2;
		double distance = shortDist*(1 - mergeFactor) + longDist*mergeFactor;
		return (int)distance;
	}
	else
	{
		if (longDist < 300 && longDist > 200)
			return longDist;
		if (shortDist < 300 && shortDist > 200)
			return shortDist;
	}
}

//Gets fresh data from all istance sensors
void updateIRSensors() {
  int ltemp = 0;
  int rtemp = 0;
  int ctemp = 0;
  //read sensors 5 times
  for (int i = 0; i < 5; i++)
  {
    ltemp = ltemp + getShortDistance(analogRead(SLSensor));
    rtemp = rtemp + getShortDistance(analogRead(SRSensor));
    int tempShort = getShortDistance(analogRead(SFSensor));
    int tempLong = getLongDistance(analogRead(LFSensor));
    ctemp = ctemp + mergeDistance(tempShort, tempLong);
  }
  //average readings
  ltemp = ltemp / 5;
  rtemp = rtemp / 5;
  ctemp = ctemp / 5;
  //store averages
	leftDistance = ltemp;
	rightDistance = rtemp;
	centerDistance = ctemp;
}

//Outputs the sensor data in a readable format
void printIRDistance() {
	lcd.setCursor(0, 1);
	String Right, Center, Left;
	IRDistToString(Right, rightDistance);
	IRDistToString(Center, centerDistance);
	IRDistToString(Left, leftDistance);
	String printDist = String("0" + Right + "|" + "0" + Center + "|" + Left);
	lcd.print(printDist);
}

//Converts the IR value to a printable string 10th of an inch
void IRDistToString(String &StrDist, int distance){
	double temp = distance * 0.393701;
	distance = temp;
	if (distance < 1000){
		StrDist = "0";
		if (distance < 100){
			StrDist = String(StrDist + "0");
			if (distance < 10){
				StrDist = String(StrDist + "0");
			}
		}
	}
	StrDist = String(StrDist + String(distance));
}
