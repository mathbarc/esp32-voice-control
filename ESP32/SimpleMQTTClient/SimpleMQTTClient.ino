/*
  SimpleMQTTClient.ino
  The purpose of this exemple is to illustrate a simple handling of MQTT and Wifi connection.
  Once it connects successfully to a Wifi network and a MQTT broker, it subscribe to a topic and send a message to it.
  It will also send a message delayed 5 seconds later.
*/

#include "EspMQTTClient.h"

EspMQTTClient client(
  "Quarto_Pais",
  "fazendasolar",
  "192.168.10.36",  // MQTT Broker server ip
  "",   // Can be omitted if not needed
  "",   // Can be omitted if not needed
  "esp32",     // Client name that uniquely identify your device
  1883              // The MQTT port, default to 1883. this line can be omitted
);

int outputPin = 15;
int outputPin2 = 2;

void setup()
{
  Serial.begin(9600);

  // Optionnal functionnalities of EspMQTTClient : 
  client.enableDebuggingMessages(); // Enable debugging messages sent to serial output
  client.enableHTTPWebUpdater(); // Enable the web updater. User and password default to values of MQTTUsername and MQTTPassword. These can be overrited with enableHTTPWebUpdater("user", "password").
  client.enableLastWillMessage("TestClient/lastwill", "I am going offline");  // You can activate the retain flag by setting the third parameter to true
  pinMode(outputPin, OUTPUT);
  digitalWrite(outputPin, HIGH);
  pinMode(outputPin2, OUTPUT);
  digitalWrite(outputPin2, HIGH);
}

// This function is called once everything is connected (Wifi and MQTT)
// WARNING : YOU MUST IMPLEMENT IT IF YOU USE EspMQTTClient
void onConnectionEstablished()
{
  // Subscribe to "mytopic/test" and display received message to Serial
  client.subscribe("pc/data/quarto", [](const String & payload) {
    digitalWrite(outputPin, (payload == "0")?(HIGH):(LOW));
    Serial.print("outputPin: ");
    Serial.println(payload);
  });

  client.subscribe("pc/data/escada", [](const String & payload) {
    digitalWrite(outputPin2, (payload == "0")?(HIGH):(LOW));
    Serial.print("outputPin2: ");
    Serial.println(payload);
  });

  // Subscribe to "mytopic/wildcardtest/#" and display received message to Serial
  //client.subscribe("mytopic/wildcardtest/#", [](const String & topic, const String & payload) {
  //  Serial.println(topic + ": " + payload);
  //});

  // Publish a message to "mytopic/test"
  //client.publish("esp32/data", "Message"); // You can activate the retain flag by setting the third parameter to true

  // Execute delayed instructions
  //client.executeDelayed(5 * 1000, []() {
  //  client.publish("mytopic/test", "This is a message sent 5 seconds later");
  //});
}

void loop()
{
  client.loop();
}
