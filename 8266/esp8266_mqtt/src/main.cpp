#include <Arduino.h>
#include <WiFiClient.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

WiFiClient client;
PubSubClient mqtt_client(client);

// config wifi
const char *ssid = "P828";
const char *password = "21121997";
const char *server_mqtt = "192.168.1.6";
const int port_mqtt = 1883;
const char *mqtt_id = "esp8266";

// config message
const char *topic_subcribe = "to - esp8266";
const char *topic_publish = "from - esp8266";

// config message from esp8266 to server
int count = 0;
unsigned long lasttime = 0;

// put function declarations here:
void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  Serial.print("Message: ");
  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  Serial.println("-----------------------");
}

// function send message to server
void send_message()
{
  if (millis() - lasttime > 1000)
  {
    lasttime = millis();
    count++;
    char message[50];
    sprintf(message, "Hello from esp8266 %d", count);
    mqtt_client.publish(topic_publish, message);
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Connecting to wifi...");
  WiFi.begin(ssid, password);

  // wait until wifi connect
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to wifi");
  Serial.println(ssid);
  Serial.println("IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  delay(1000);
  mqtt_client.setServer(server_mqtt, port_mqtt);
  mqtt_client.setCallback(callback);

  // wait until mqtt connect
  while (!mqtt_client.connected())
  {
    Serial.println("Connecting to MQTT...");
    if (mqtt_client.connect("ESP8266Client"))
    {
      Serial.println("connected");
      mqtt_client.publish(topic_publish, "hello from esp8266");
      mqtt_client.subscribe(topic_subcribe);
    }
    else
    {
      Serial.print("failed with state ");
      Serial.print(mqtt_client.state());
      delay(2000);
    }
  }

}

void loop() {
  // put your main code here, to run repeatedly:
  mqtt_client.loop();
// put function definitions here:
  send_message();
}
