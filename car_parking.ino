#include <ESP8266WiFi.h>
#include <Servo.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
const char *ssid = "Redmi Note 8";     // WiFi SSID
const char *pass = "murghianda1";    // WiFi password

#define MQTT_SERV "io.adafruit.com"
#define MQTT_PORT 1883
#define MQTT_NAME "nihal_shaikh"
#define MQTT_PASS "aio_lRGI43gCytZLmLkvAblbAZizAdPQ"

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 19800, 60000);
Servo myservo;      // Servo for exit gate
Servo myservos;     // Servo for entry gate

int carEnter = D1;   // Entry sensor 
int carExited = D2;  // Exit sensor
int slot3 = D7;      // Slot 3 sensor
int slot2 = D2;      // Slot 2 sensor
int slot1 = D1;      // Slot 1 sensor
int slot4 = D4;      // Slot 4 sensor
int slot5 = D5;      // Slot 5 sensor
int slot6 = D6;      // Slot 6 sensor

int count = 0;

boolean entrysensor, exitsensor, s1, s2, s3, s4, s5, s6;
boolean s1_occupied = false;
boolean s2_occupied = false;
boolean s3_occupied = false;
boolean s4_occupied = false;
boolean s5_occupied = false;
boolean s6_occupied = false;


WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, MQTT_SERV, MQTT_PORT, MQTT_NAME, MQTT_PASS);

Adafruit_MQTT_Subscribe EntryGate = Adafruit_MQTT_Subscribe(&mqtt, MQTT_NAME "/f/EntryGate");
Adafruit_MQTT_Subscribe ExitGate = Adafruit_MQTT_Subscribe(&mqtt, MQTT_NAME "/f/ExitGate");

Adafruit_MQTT_Publish CarsParked = Adafruit_MQTT_Publish(&mqtt, MQTT_NAME "/f/CarsParked");
Adafruit_MQTT_Publish EntrySlot1 = Adafruit_MQTT_Publish(&mqtt, MQTT_NAME "/f/EntrySlot1");
Adafruit_MQTT_Publish EntrySlot4 = Adafruit_MQTT_Publish(&mqtt, MQTT_NAME "/f/EntrySlot4");
Adafruit_MQTT_Publish EntrySlot2 = Adafruit_MQTT_Publish(&mqtt, MQTT_NAME "/f/EntrySlot2");
Adafruit_MQTT_Publish EntrySlot5 = Adafruit_MQTT_Publish(&mqtt, MQTT_NAME "/f/EntrySlot5");
Adafruit_MQTT_Publish EntrySlot3 = Adafruit_MQTT_Publish(&mqtt, MQTT_NAME "/f/EntrySlot3");
Adafruit_MQTT_Publish EntrySlot6 = Adafruit_MQTT_Publish(&mqtt, MQTT_NAME "/f/EntrySlot6");

void setup() {
  delay(1000);
  Serial.begin(9600); 
  mqtt.subscribe(&EntryGate);
  mqtt.subscribe(&ExitGate);
  timeClient.begin(); 
  myservo.attach(D4);      // Servo pin to D4 for exit
  myservos.attach(D5);     // Servo pin to D5 for entry
  pinMode(carExited, INPUT);    // IR sensor as input
  pinMode(carEnter, INPUT);     // IR sensor as input
  pinMode(slot1, INPUT);
  pinMode(slot2, INPUT);
  pinMode(slot3, INPUT);
  pinMode(slot4, INPUT);
  pinMode(slot5, INPUT);
  pinMode(slot6, INPUT);

  WiFi.begin(ssid, pass);       // Try to connect with WiFi
  Serial.print("Connecting to ");
  Serial.print(ssid);           // Display SSID
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP Address is : ");
  Serial.println(WiFi.localIP()); // Print local IP address
}

void loop() {
  MQTT_connect();
  timeClient.update();

  entrysensor = !digitalRead(carEnter);
  exitsensor = !digitalRead(carExited);
  s1 = digitalRead(slot1);
  s2 = digitalRead(slot2);
  s3 = digitalRead(slot3);
  s4 = digitalRead(slot4);
  s5 = digitalRead(slot5);
  s6 = digitalRead(slot6);


  int totalSlots = 6; // Assuming you have 3 slots
  int occupiedSlots = 0;

  // Check each parking slot and update occupancy
  if (s1 == 0 && !s1_occupied) {                     
    EntrySlot1.publish("Occupied");
    s1_occupied = true;
  } else if (s1 == 1 && s1_occupied) {
    EntrySlot1.publish("Available");
    s1_occupied = false;
  }
  
  if (s2 == 0 && !s2_occupied) {                     
    EntrySlot2.publish("Occupied");
    s2_occupied = true;
  } else if (s2 == 1 && s2_occupied) {
    EntrySlot2.publish("Available");
    s2_occupied = false;
  }
  
  if (s3 == 0 && !s3_occupied) {                     
    EntrySlot3.publish("Occupied");
    s3_occupied = true;
  } else if (s3 == 1 && s3_occupied) {
    EntrySlot3.publish("Available");
    s3_occupied = false;
  } 
  if (s4 == 0 && !s4_occupied) {                     
    EntrySlot4.publish("Occupied");
    s4_occupied = true;
  } else if (s4 == 1 && s4_occupied) {
    EntrySlot4.publish("Available");
    s4_occupied = false;
  }
  
  if (s5 == 0 && !s5_occupied) {                     
    EntrySlot5.publish("Occupied");
    s5_occupied = true;
  } else if (s5 == 1 && s5_occupied) {
    EntrySlot5.publish("Available");
    s5_occupied = false;
  } 
  if (s6 == 0 && !s6_occupied) {                     
    EntrySlot6.publish("Occupied");
    s6_occupied = true;
  } else if (s6 == 1 && s6_occupied) {
    EntrySlot6.publish("Available");
    s6_occupied = false;
  } 

  // Calculate occupied slots
  if (s1 == 0) occupiedSlots++;
  if (s2 == 0) occupiedSlots++;
  if (s3 == 0) occupiedSlots++;
  if (s4 == 0) occupiedSlots++;
  if (s5 == 0) occupiedSlots++;
  if (s6 == 0) occupiedSlots++;


  // Calculate available slots
  int availableSlots = totalSlots - occupiedSlots;

  // Publish the total number of available slots
  CarsParked.publish(availableSlots);

  // Handle MQTT subscriptions
  Adafruit_MQTT_Subscribe * subscription;
  while ((subscription = mqtt.readSubscription(5000))) {
    if (subscription == &EntryGate) {
      if (!strcmp((char*) EntryGate.lastread, "ON")) {
        myservos.write(80); // Assuming OPEN_ANGLE is 80 (defined elsewhere)
        delay(3000);
        myservos.write(0); // Assuming CLOSE_ANGLE is 0 (defined elsewhere)
      }
    }
    if (subscription == &ExitGate) {
      if (!strcmp((char*) ExitGate.lastread, "ON")) {
        myservo.write(80); // Assuming OPEN_ANGLE is 80 (defined elsewhere)
        delay(3000);
        myservo.write(0); // Assuming CLOSE_ANGLE is 0 (defined elsewhere)
      }
    }
  }
}

void MQTT_connect() {
  int8_t ret;
  if (mqtt.connected()) {
    return;
  }
  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) {
    mqtt.disconnect();
    delay(5000);  // Wait 5 seconds
    retries--;
    if (retries == 0) {
      while (1); // Basically die and wait for WDT to reset
    }
  }
}

