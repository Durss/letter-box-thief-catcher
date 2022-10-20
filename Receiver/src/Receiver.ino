#include "Particle.h"
SYSTEM_THREAD(ENABLED);
SYSTEM_MODE(MANUAL);

#define MP3_Start_Byte 0x7E
#define MP3_Version_Byte 0xFF
#define MP3_Command_Length 0x06
#define MP3_End_Byte 0xEF
#define MP3_Acknowledge 0x00 //Returns info with command 0x41 [0x01: info, 0x00: no info]

const uint LOST_TIMEOUT = 6 * 60 * 1000;//Duration after which the emitter is considered lost if it didn't show up
const char *BLE_OPENED_NAME = "letterbox-opened";//BLE name when door is opened
const char *BLE_CLOSED_NAME = "letterbox-closed";//BLE name when door is closed

bool doorOpened = false;
bool shouldScan = true;
uint lastAliveMs = 0;
int rPin = D2;
int gPin = D3;
int bPin = D4;


void setup()
{
	Serial.begin(9600);
	Serial1.begin(9600);
	Serial.println("Serial begin");
	delay(2000);
	//*
	Serial.println("Init player");
	MP3ExecuteCmd(0x3F, 0, 0); // Send request for initialization parameters

	/* Wait until initialization parameters are received (10 bytes)
	while (Serial1.available() < 10)
	{
		Serial.println(Serial1.read());
		delay(100); // Pretty long delays between succesive commands needed (not always the same)
	}
	//*/

	pinMode(rPin, OUTPUT);
	pinMode(gPin, OUTPUT);
	pinMode(bPin, OUTPUT);

	analogWrite(rPin, 0);
	analogWrite(gPin, 255);
	analogWrite(bPin, 255);

	MP3ExecuteCmd(0x06, 0, 33);//Set volume from 0 to 33

	analogWrite(rPin, 0);
	analogWrite(gPin, 0);
	analogWrite(bPin, 0);

	Serial.println("Play sound");
	MP3ExecuteCmd(0x0F, 1, 1);//Play start sound
	delay(2000);
}

void loop()
{
	/* Demo mode
	delay(5000);
	analogWrite(rPin, 0);
	analogWrite(gPin, 255);
	analogWrite(bPin, 255);
	delay(500);
	MP3ExecuteCmd(0x0F, 1, 2);//Play sound N°1

	delay(5000);

	analogWrite(rPin, 0);
	analogWrite(gPin, 0);
	analogWrite(bPin, 0);
	//*/

	//*
	MP3OnMessage();

	if(millis() - lastAliveMs > LOST_TIMEOUT) {
		Serial.println("Device lost !!");
		MP3ExecuteCmd(0x0F, 1, 3);//Play sound N°2
		MP3ExecuteCmd(0x19, 0, 0);//Loop sound
		analogWrite(rPin, 255);
		analogWrite(gPin, 0);
		analogWrite(bPin, 0);
	}
	
	if(shouldScan) {//This flag is probably useless (depends weither the BLE.scan() blocks the loop execution or not, too lazy to test for now)
		Serial.println("Start scan...");
		shouldScan = false;
		if(!doorOpened) {
			analogWrite(rPin, 255);
			analogWrite(gPin, 200);
			analogWrite(bPin, 255);
		}
		//Scan for devices
		(void) BLE.scan(scanResultCallback, NULL);
		shouldScan = true;
	}
	//*/
}

void scanResultCallback(const BleScanResult *scanResult, void *context)
{
	// Serial.println("Scan result...");
	String name = scanResult->advertisingData.deviceName();
	
	if(name.length() > 0) {
		//Check if a BLE device has the name open name
		if(name == BLE_OPENED_NAME) {
			Serial.println("Box opened");
			doorOpened = true;
			lastAliveMs = millis();
			analogWrite(rPin, 0);
			analogWrite(gPin, 255);
			analogWrite(bPin, 255);
			delay(500);
			MP3ExecuteCmd(0x0F, 1, 2);//Play sound N°1
			// MP3ExecuteCmd(0x19, 0, 0);//Loop sound
		}else
		if(name == BLE_CLOSED_NAME) {
			Serial.println("Box closed");
			doorOpened = false;
			analogWrite(rPin, 255);
			analogWrite(gPin, 0);
			analogWrite(bPin, 255);
			delay(500);
			lastAliveMs = millis();
		}
	}
}

void MP3ExecuteCmd(byte CMD, byte Par1, byte Par2) // Excecute the command and parameters
{
	// Calculate the checksum (2 bytes)
	int16_t checksum = -(MP3_Version_Byte + MP3_Command_Length + CMD + MP3_Acknowledge + Par1 + Par2);

	// Build the command line
	byte Command_line[10] = {MP3_Start_Byte, MP3_Version_Byte, MP3_Command_Length, CMD, MP3_Acknowledge, Par1, Par2, checksum >> 8, checksum & 0xFF, MP3_End_Byte};

	//Send the command line to the module
	for (byte k = 0; k < 10; k++)
	{
		Serial1.write(Command_line[k]);
	}
}

void MP3OnMessage()
{
	if (Serial1.available() >= 10) // There is at least 1 returned message (10 bytes each)
	{
		// Read the returned code
		byte Returned[10];
		for (byte k = 0; k < 10; k++) Returned[k] = Serial1.read();

		// Wtite the returned code to the screen
		Serial.print("Returned: 0x"); if (Returned[3] < 16) Serial.print("0"); Serial.print(Returned[3],HEX);
		Serial.print("("); Serial.print(Returned[3], DEC);
		Serial.print("); Parameter: 0x"); if (Returned[5] < 16) Serial.print("0"); Serial.print(Returned[5],HEX);
		Serial.print("("); Serial1.print(Returned[5], DEC);
		Serial.print("), 0x"); if (Returned[6] < 16) Serial.print("0"); Serial.print(Returned[6],HEX);
		Serial.print("("); Serial.print(Returned[6], DEC); Serial.println(")");
	}
}