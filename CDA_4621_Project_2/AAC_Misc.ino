/*******************************************************************************
* AAC_Misc.ino
*
* This file contains functions that do not directly affect Movement and sensing.
* It should be included in Projects to provide extra capability.
*
* Kiril Lange, Andres Izurieta
*
* 2016/2/4
*******************************************************************************/

void countDown(int x, String message){
	for (int i = x; i > 0; i--){
		lcd.clear();
		lcd.setCursor(0, 0);
		lcd.print(message);
		lcd.setCursor(8, 1);
		lcd.print(i);
		delay(1000);
	}
	lcd.clear();
	lcd.setCursor(0, 0);
	lcd.print(message);
	lcd.setCursor(4, 1);
	lcd.print("Running");
	lcd.setBacklight(GREEN);
}