#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid = "YourWiFiSSID";
const char* password = "YourWiFiPassword";
const char* mqttServer = "mqtt.yourbroker.com";
const int mqttPort = 1883;
const char* mqttUser = "YourMQTTUser";
const char* mqttPassword = "YourMQTTPassword";
const char* topic = "your/topic";

const int buttonPin = 2; // Pin của nút bấm
int lastButtonState = HIGH; // Trạng thái trước của nút bấm
WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  pinMode(buttonPin, INPUT);
  digitalWrite(buttonPin, HIGH); // Kích hoạt resistor nội bộ
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqttServer, mqttPort);
}

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("WiFi connected - IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP8266Client", mqttUser, mqttPassword)) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  int buttonState = digitalRead(buttonPin);
  if (buttonState != lastButtonState) {
    lastButtonState = buttonState;
    Serial.println(buttonState);
    if (buttonState == LOW) {
      client.publish(topic, "1"); // Trạng thái ON
    } else {
      client.publish(topic, "0"); // Trạng thái OFF
    }
  }
}
