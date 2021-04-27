#include <ArduinoJson.h>
#include <ArduinoMqttClient.h>
#include <ESP8266WiFi.h>

#include <BME280I2C.h>
#include <Wire.h>

#include "secrets.h"

extern "C" {
#include "libb64/cdecode.h"
}

const char* ssid = SECRET_SSID;
const char* password = SECRET_WIFI_PASS;

// For the two certificate strings below paste in the text of your AWS 
// device certificate and private key, comment out the BEGIN and END 
// lines, add a quote character at the start of each line and a quote 
// and backslash at the end of each line:

// xxxxxxxxxx-certificate.pem.crt
const String certificatePemCrt = SECRET_PEM_CERT;

// xxxxxxxxxx-private.pem.key
const String privatePemKey = SECRET_PEM_KEY;

// This is the AWS IoT CA Certificate from: 
// https://docs.aws.amazon.com/iot/latest/developerguide/managing-device-certs.html#server-authentication
// This one in here is the 'RSA 2048 bit key: Amazon Root CA 1' which is valid 
// until January 16, 2038 so unless it gets revoked you can leave this as is:
const String caPemCrt = SECRET_CA_CERT;

// Find this awsEndpoint in the AWS Console: Manage - Things, choose your thing
// choose Interact, its the HTTPS Rest endpoint 
const char* awsEndpoint = SECRET_AWS_ENDPOINT;

const char* awsPubTopic = "sensor/greenhouse/tempratureHumidity/pub";
const char* awsSubTopic = "sensor/greenhouse/tempratureHumidity/sub";

unsigned long lastMillis = 0;
unsigned long waitTimeMs = 300000; //10 Mins

WiFiClientSecure wiFiClient;
MqttClient mqttClient(wiFiClient);

StaticJsonDocument<200> doc;

BME280I2C bme;

void setup() {
  
  Serial.begin(115200);
  
  delay(5000);
  
  Serial.println("ESP8266 AWS IoT Example");

  Serial.print("Connecting to "); Serial.print(ssid);
  WiFi.begin(ssid, password);
  WiFi.waitForConnectResult();
  Serial.print(", WiFi connected, IP address: "); Serial.println(WiFi.localIP());

  // get current time, otherwise certificates are flagged as expired
  setCurrentTime();

  uint8_t binaryCert[certificatePemCrt.length() * 3 / 4];
  int len = b64decode(certificatePemCrt, binaryCert);
  wiFiClient.setCertificate(binaryCert, len);
  
  uint8_t binaryPrivate[privatePemKey.length() * 3 / 4];
  len = b64decode(privatePemKey, binaryPrivate);
  wiFiClient.setPrivateKey(binaryPrivate, len);

  uint8_t binaryCA[caPemCrt.length() * 3 / 4];
  len = b64decode(caPemCrt, binaryCA);
  wiFiClient.setCACert(binaryCA, len);

  Wire.begin();

  while(!bme.begin()) {
    Serial.println("Could not find BME280 sensor!");
    delay(1000);
  }
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    connectWiFi();
  }

  if (!mqttClient.connected()) {
    // MQTT client is disconnected, connect
    connectMQTT();
  }

  printBME280Data(&Serial);

  // poll for new MQTT messages and send keep alives
  mqttClient.poll();

    // publish a message
  if (millis() - lastMillis > waitTimeMs) {
    lastMillis = millis();

    publishMessage();
  }

  delay(waitTimeMs);

}

int b64decode(String b64Text, uint8_t* output) {
  base64_decodestate s;
  base64_init_decodestate(&s);
  int cnt = base64_decode_block(b64Text.c_str(), b64Text.length(), (char*)output, &s);
  return cnt;
}

void setCurrentTime() {
  configTime(3 * 3600, 0, "pool.ntp.org", "time.nist.gov");

  Serial.print("Waiting for NTP time sync: ");
  time_t now = time(nullptr);
  while (now < 8 * 3600 * 2) {
    delay(1000);
    Serial.print(".");
    now = time(nullptr);
  }
  Serial.println("");
  struct tm timeinfo;
  gmtime_r(&now, &timeinfo);
  Serial.print("Current time: "); Serial.print(asctime(&timeinfo));
}

void connectWiFi() {
  Serial.print("Attempting to connect to SSID: ");
  Serial.print(ssid);
  Serial.print(" ");

  while (WiFi.begin(ssid, password) != WL_CONNECTED) {
    // failed, retry
    Serial.print(".");
    delay(5000);
  }
  Serial.println();

  Serial.println("You're connected to the network");
  Serial.println();
}

void connectMQTT() {
  Serial.print("Attempting to MQTT broker: ");
  Serial.print(awsEndpoint);
  Serial.println(" ");

  while (!mqttClient.connect(awsEndpoint, 8883)) {
    // failed, retry
    Serial.print(".");
    delay(5000);
  }
  Serial.println();

  Serial.println("You're connected to the MQTT broker");
  Serial.println();

  // subscribe to a topic
  mqttClient.subscribe(awsSubTopic);
}

void publishMessage() {
  Serial.println("Publishing message");
  
  String message;
  
  serializeJson(doc, message);

  Serial.println(message);

  // send message, the Print interface can be used to set the message contents
  mqttClient.beginMessage(awsPubTopic);
  mqttClient.print(message);
  mqttClient.endMessage();
}

void onMessageReceived(int messageSize) {
  // we received a message, print out the topic and contents
  Serial.print("Received a message with topic '");
  Serial.print(mqttClient.messageTopic());
  Serial.print("', length ");
  Serial.print(messageSize);
  Serial.println(" bytes:");

  // use the Stream interface to print the contents
  while (mqttClient.available()) {
    Serial.print((char)mqttClient.read());
  }
  Serial.println();
}

void printBME280Data(Stream* client) {
  
  float temp(NAN), hum(NAN), pres(NAN);

  BME280::TempUnit tempUnit(BME280::TempUnit_Celsius);
  BME280::PresUnit presUnit(BME280::PresUnit_Pa);
  
  bme.read(pres, temp, hum, tempUnit, presUnit);
  
  doc["humidity"] = hum;
  doc["temperature"] = temp;
  doc["preasure"] = pres;
  
  client->print("Temp: ");
  client->print(temp);
  client->print("°"+ String(tempUnit == BME280::TempUnit_Celsius ? 'C' :'F'));
  client->print("\t\tHumidity: ");
  client->print(hum);
  client->print("% RH");
  client->print("\t\tPressure: ");
  client->print(pres);
  client->println("Pa");
 
}
