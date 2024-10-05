#include <WiFi.h>
#include <PubSubClient.h>
#include <DHTesp.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <ESP32Servo.h>

#define DHT_Pin 15
#define Buzzer_Pin 13
#define LDR_1 32
#define LDR_2 33

DHTesp dhtSensor;

WiFiClient espClient;
PubSubClient mqttClient(espClient);

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

//arrays created
char tempAr[6];

char LDRValue1[6];
char LDRValue2[6];

bool isScheduledON = false;
unsigned long scheduledOnTime;

float controlFactor = 0.75;
int minAngleValue = 30; 

Servo myServo;
int p = 0;
float angle = 0;
int currentAngle = 0;

void setup() {
  Serial.begin(115200);
  delay(10);
  myServo.attach(4);

  setUpWifi();
  setupMqtt();
  dhtSensor.setup(DHT_Pin, DHTesp::DHT22);

  timeClient.begin();
  timeClient.setTimeOffset(5.5*3600);

  pinMode(Buzzer_Pin, OUTPUT);
  digitalWrite(Buzzer_Pin, LOW);

  pinMode(LDR_1,INPUT);
  pinMode(LDR_2,INPUT);
}

void loop() {
  if (!mqttClient.connected()){
    connectToBroker();
  }
  mqttClient.loop();

  updateTemperature();
  Serial.println(tempAr);
  mqttClient.publish("Dulan Admin Temp", tempAr); // Added missing semicolon
  delay(2000);

  checkSchedule();

  int a = analogRead(LDR_1);
  int b = analogRead(LDR_2);
  Serial.print("LDR 1 : ");
  Serial.println(String(a));
  Serial.print("LDR 2 : ");
  Serial.println(String(b));
  String(a).toCharArray(LDRValue1, 6);
  mqttClient.publish("Dulan Admin LDR-1", LDRValue1);
  String(b).toCharArray(LDRValue2, 6);
  mqttClient.publish("Dulan Admin LDR-2", LDRValue2);
  
  float maxIntensity;

  float D;
  if (b>a) {
    D = 1.5;
    maxIntensity = (float)(4063-a)/4063;
  } else {
    // right LDR is high
    D = 0.5;
    maxIntensity = (float)(4063-b)/4063;
  }
  Serial.print("Minimum Angle Value: ");
  Serial.println(minAngleValue);
  Serial.print("Control Factor: ");
  Serial.println(controlFactor);
  Serial.print("Testing angle is: ");
  Serial.println(minAngleValue * D + (180 - minAngleValue) * controlFactor * maxIntensity);

  

  if (minAngleValue * D + (180 - minAngleValue) * controlFactor * maxIntensity < 180){
    angle = minAngleValue * D + (180 - minAngleValue) * controlFactor * maxIntensity;
    Serial.print("Angle is: ");
    Serial.println(angle);
  }
  else{
    angle = 180;
    Serial.print("Angle is (180): ");
    Serial.println(int(angle));
  }
  
  moveServo();
  
  delay(1000);
}

void updateTemperature(){
  TempAndHumidity data = dhtSensor.getTempAndHumidity();
  String(data.temperature, 2).toCharArray(tempAr, 6);
}

void setUpWifi(){
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println("Wokwi-Guest");
  WiFi.begin("Wokwi-GUEST", "");

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void setupMqtt(){
  mqttClient.setServer("test.mosquitto.org", 1883);
  mqttClient.setCallback(receiveCallback);
}

void connectToBroker(){
  while (!mqttClient.connected()){
    Serial.println("Attempting to connect MQTT");
    String clientId = "ESP32-" + String(WiFi.macAddress());
    if(mqttClient.connect(clientId.c_str())){
      Serial.println("Connected");
      mqttClient.subscribe("ADMIN-MAIN-ON-OFF");
      mqttClient.subscribe("ADMIN-SCH-ON");
      mqttClient.subscribe("MIN-ANGLE"); 
      mqttClient.subscribe("CONTROL-FAC"); 
    } else{
      Serial.println("Failed");
      Serial.print(mqttClient.state());
      delay(5000); 
    }
  }
}
void buzzerOn(bool on){
  if(on){
    tone(Buzzer_Pin, 256);
  }else{
    noTone(Buzzer_Pin);
  }
}

void moveServo(){
  if (currentAngle<int(angle)){
    for (p=currentAngle;p<int(angle);p++)
  {
    myServo.write(p);
    delay(50);
    currentAngle = int(angle);
  }
  }
  else{
    for (p=currentAngle;p>int(angle);p--)
  {
    myServo.write(p);
    delay(50);
    currentAngle = int(angle);
  }
  }
  
}

void receiveCallback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("]");

  char payloadCharAr[length];

  for(int i = 0; i < length; i++){
    Serial.print((char)payload[i]);
    payloadCharAr[i] = (char)payload[i];
  }
  Serial.println();

  if (strcmp(topic, "ADMIN-MAIN-ON-OFF") == 0) {
    buzzerOn(payloadCharAr[0] == '1');
  }
  else if(strcmp(topic, "ADMIN-SCH-ON") == 0){
    if (payloadCharAr[0] == 'N') {
      isScheduledON = false;
    }else{
      isScheduledON = true;
      scheduledOnTime = atol(payloadCharAr);
    }
  }
  else if(strcmp(topic, "MIN-ANGLE") == 0){
    minAngleValue = atoi(payloadCharAr);
    Serial.print("Min Angle Value: ");
    Serial.println(minAngleValue);
  }
  else if(strcmp(topic, "CONTROL-FAC") == 0){
    controlFactor = atof(payloadCharAr);
    Serial.print("Control Factor: ");
    Serial.println(controlFactor);
  }
}

void checkSchedule() {
  if (isScheduledON) {
    unsigned long currentTime = getTime();
    if (currentTime > scheduledOnTime){
      buzzerOn(true);
      isScheduledON=false;
      mqttClient.publish("ADMIN-MAIN-ON-OFF-ESP", "1");
      Serial.println("Scheduled ON");
    }
  }
}

unsigned long getTime() {
  timeClient.update();
  return timeClient.getEpochTime();
}