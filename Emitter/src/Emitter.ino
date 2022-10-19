#include "Particle.h"

SYSTEM_MODE(MANUAL);

const uint MICRO_SWITCH_PIN = WKP;
const uint CLOSED_DURATION_SLEEP = 10;// If the door stays closed for this number of seconds, the boards goes to sleep
const long SLEEP_DURATION = 60;// Number of seconds the board goes to sleep after the door stayed closed
const char *BLE_OPENED_NAME = "letterbox-opened";// Name of the BLE device when door is openeed
const char *BLE_CLOSED_NAME = "letterbox-closed";// Name of the BLE device when door is closed
const char *BLE_UUID = "4f2cb1d0-c600-40f0-8d23-94643bbb65ce";// BlueTooth service ID

bool closed = false;
uint closed_at = 0;

void setup()
{
	Serial1.begin(9600);
	RGB.control(true);

	// pinMode(reedPin, INPUT_PULLUP);
	pinMode(PWR, INPUT);
	pinMode(CHG, INPUT);
	RGB.color(0,0,0);
	
	// attachInterrupt(WKP, doorInterruptHandler, RISING);

	advertiseCurrentState(true);

	pinMode(WKP, INPUT_PULLDOWN);
	Serial.println(digitalRead(WKP));
	if(digitalRead(WKP) == LOW) {
		System.sleep(D8, RISING, SLEEP_DURATION);
		// System.sleep(SLEEP_MODE_DEEP, SLEEP_DURATION);
	}
}

void loop()
{
	advertiseCurrentState(false);

	if(closed && millis() - closed_at > CLOSED_DURATION_SLEEP * 1000) {
		if(digitalRead(WKP) == LOW) {
			//Door closed for the last CLOSED_DURATION_SLEEP seconds.
			//Go to deep sleep and wake up in SLEEP_DURATION seconds.
			//If door is opened it will also wake up the board.
			Serial.print("zzz Nap time for ");
			Serial.println(SLEEP_DURATION);
			// System.sleep(SLEEP_MODE_DEEP, SLEEP_DURATION);
			System.sleep(D8, RISING, SLEEP_DURATION);
			delay(10000);//Leave it time to the receiver to detect us
		}
	}

	delay(500);
}

void advertiseCurrentState(bool isInit) {
	bool doorOpened = digitalRead(MICRO_SWITCH_PIN) == 0 ? false : true;

	// long batteryPercent = digitalRead(PWR) && !digitalRead(CHG);
	// Serial.print("Charging? ");
	// Serial.println(batteryPercent);

	BleAdvertisingData advData;
	if (doorOpened) {
		closed = false;
		closed_at = millis();
		Serial.println("Door opened");
		advData.appendLocalName(BLE_OPENED_NAME);
	}else{
		Serial.print("Door closed - WKP state:");
		Serial.println(digitalRead(WKP) );
		advData.appendLocalName(BLE_CLOSED_NAME);
		if(!closed) {
			closed_at = millis();
			closed = true;
		}
	}
	advData.appendServiceUUID(BLE_UUID);
	//Update advertising label
	if(isInit) {
		BLE.setAdvertisingData(&advData);
	}else{
		BLE.setAdvertisingInterval(50);
		BLE.advertise(&advData);
	}
}

// void doorInterruptHandler() {
// 	Serial.print("Door state changed:");
// 	closed = digitalRead(reedPin) == 0 ? false : true;
// 	closed_at = millis();
// 	if(closed) Serial.println(" closed");
// 	else       Serial.println(" opened");
// }