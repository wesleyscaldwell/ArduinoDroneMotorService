/*
 Name:		ArduinoDroneMotorService.ino
 Created:	12/26/2020 9:48:32 AM
 Author:	Wesley Caldwell
*/

// the setup function runs once when you press reset or power the board
void setup() {
	Serial.begin(115200);
	while (!Serial) {
		; // wait for serial port to connect. Needed for native USB port only
	}
}

// the loop function runs over and over again until power down or reset
void loop() {
	if (Serial.available() > 0) {
		inByte = Serial.read();
		Serial.write(inByte);
	}
	//Serial.write("Serial Available");
	//Serial.write("3rd write");
}
