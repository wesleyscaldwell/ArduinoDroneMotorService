/*
 Name:		ArduinoDroneMotorService.ino
 Created:	12/26/2020 9:48:32 AM
 Author:	Wesley Caldwell
*/
#include <Servo.h>
Servo ESC;     // create servo object to control the ESC


String readString;
int inByte = 0;         // incoming serial byte

// the setup function runs once when you press reset or power the board
void setup() {
	Serial.begin(115200);
	while (!Serial) {
		; // wait for serial port to connect. Needed for native USB port only
	}
	Serial.print("Starting");
	readString = "";
}

// the loop function runs over and over again until power down or reset
void loop() {
	//if (Serial.available() > 0) {
	//	inByte = Serial.read();
	//	Serial.write(inByte);
	//}

	while (Serial.available()) {
		delay(3);  //delay to allow buffer to fill
		if (Serial.available() > 0) {
			char c = Serial.read();  //gets one byte from serial buffer
			readString += c; //makes the string readString
		}
	}

	if (readString.length() > 0) {
		String eventList = readString;
		String eventListArray[99];
		int ActionCount = SplitString(eventList, eventListArray);
		if (ActionCount < 0) {
			Serial.print("No Action Found, received: ");
			Serial.println(readString);
		}
		else {
			for (int i = 0; i <= ActionCount; i++)   //sizeof(eventListArray)
			{
				String x = eventListArray[i];
				x.trim();
				String eventType = x.substring(0, 2);
				if (eventType == "MV") //Motor Value
				{
					int pinNum = x.substring(2, 4).toInt(); //get the first two characters - pin number
					String actionType = x.substring(4, 5);
					if (actionType == "S") {
						String executeValue = x.substring(5, 9); //get the four characters - 3 - 7
						Serial.print("Setting Pin:");
						Serial.print(pinNum);
						Serial.print(" Execute value: ");
						Serial.println(executeValue);

						int potValue = executeValue.toInt();   // reads the value of the potentiometer (value between 0 and 1023)
						potValue = map(potValue, 0, 1023, 0, 180);   // scale it to use it with the servo library (value between 0 and 180)
						ESC.write(potValue);    // Send the signal to the ESC
					}
				}
				else 
				{
					Serial.print("Event Type: ");
					Serial.print(eventType);
					Serial.println(" Is Unknown");
				}
			}
		}
		

		////Serial.println(readString); //see what was received
		//
		////readString = "Returning: " + readString;
		////Serial.print(readString);
		//Serial.write('A');
		//delay(1000);
		//Serial.print(readString);
		//delay(1000);
		//Serial.write('Z');
		//// expect a string like 07002100 containing the two servo positions     
		////servo1 = readString.substring(0, 4); //get the first four characters
		////servo2 = readString.substring(4, 8); //get the next four characters
		////
		////Serial.println(servo1);  //print to serial monitor to see parsed results
		////Serial.println(servo2);
		////
		////int n1 = servo1.toInt();
		////int n2 = servo2.toInt();
		////
		////Serial.println("the numbers are :");
		////Serial.println(n1);  //print to serial monitor to see number results
		////Serial.println(n2);
		////
		////myservo1.writeMicroseconds(n1); //set servo position
		////myservo2.writeMicroseconds(n2);
		readString = "";
	}



	//Serial.write("Serial Available");
	//Serial.write("3rd write");
}


int SplitString(String oneLine, String sa[])
{
	oneLine.trim();
	int ArrayCount = 0;
	if (oneLine.length() > 1) {
		int lnIndex = oneLine.length() - 1;
		String lastChar = oneLine.substring(lnIndex, (lnIndex + 1));
		if (lastChar == ";")
			oneLine = oneLine.substring(0, lnIndex);
	}

	for (int i = 0; i < oneLine.length(); i++)
	{
		if (oneLine.charAt(i) == ';') 
			ArrayCount++;
	}

	//sa[ArrayCount];
	int r = 0, t = 0;
	int LineLength = oneLine.length();
	for (int i = 0; i < oneLine.length(); i++)
	{
		if (oneLine.charAt(i) == ';')
		{
			String lineValue = oneLine.substring(r, i);
			lineValue.trim();
			sa[t] = lineValue;
			r = (i + 1);
			t++;
		}
		else
		{
			int checkVal = i + 1;
			if (checkVal == LineLength)
			{
				String lineValue = oneLine.substring(r, checkVal);
				lineValue.trim();
				sa[t] = lineValue;
				r = (i + 1);
				t++;
			}
		}
	}
	return ArrayCount;
}
