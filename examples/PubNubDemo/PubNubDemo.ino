/*
  PubNub sample client

  This sample client will use just the minimal-footprint raw PubNub
  interface where it is your responsibility to deal with the JSON encoding.

  It will just send a hello world message and retrieve one back, reporting
  its deeds on serial console.

  To get this to work, in the spark.io web IDE,
  * Cut'n'paste this code
  * Instead of the #include <PubNub.h> line, paste PubNub.h contents
  * At the end, paste PubNub.cpp contents

  Circuit:
  * (Built-in.) LED on pin D7 for reception indication.
  * (Optional.) LED on pin D6 for publish indication.

  created 22 January 2014
  by Petr Baudis

  https://github.com/pubnub/sparkCore
  This code is in the public domain.
  */

#include <PubNub.h>

const int subLedPin = D7;
const int pubLedPin = D6;

char pubkey[] = "demo";
char subkey[] = "demo";
char channel[] = "hello_world";

void setup()
{
	pinMode(subLedPin, OUTPUT);
	pinMode(pubLedPin, OUTPUT);
	digitalWrite(subLedPin, LOW);
	digitalWrite(pubLedPin, LOW);

	Serial.begin(9600);
	Serial.println("Serial set up");

	PubNub.begin(pubkey, subkey);
	Serial.println("PubNub set up");
}

void flash(int ledPin)
{
	/* Flash LED three times. */
	for (int i = 0; i < 3; i++) {
		digitalWrite(ledPin, HIGH);
		delay(100);
		digitalWrite(ledPin, LOW);
		delay(100);
	}
}

void loop()
{
	TCPClient *client;

	Serial.println("publishing a message");
	client = PubNub.publish(channel, "\"\\\"Hello world!\\\" she said.\"");
	if (!client) {
		Serial.println("publishing error");
		delay(1000);
		return;
	}
	while (client->connected()) {
		while (client->connected() && !client->available()) ; // wait
		char c = client->read();
		Serial.print(c);
	}
	client->stop();
	Serial.println();
	flash(pubLedPin);

	Serial.println("waiting for a message (subscribe)");
	PubSubClient *pclient = PubNub.subscribe(channel);
	if (!pclient) {
		Serial.println("subscription error");
		delay(1000);
		return;
	}
	while (pclient->wait_for_data()) {
		char c = pclient->read();
		Serial.print(c);
	}
	pclient->stop();
	Serial.println();
	flash(subLedPin);

	Serial.println("retrieving message history");
	client = PubNub.history(channel);
	if (!client) {
		Serial.println("history error");
		delay(1000);
		return;
	}
	while (client->connected()) {
		while (client->connected() && !client->available()) ; // wait
		char c = client->read();
		Serial.print(c);
	}
	client->stop();
	Serial.println();

	delay(10000);
}
