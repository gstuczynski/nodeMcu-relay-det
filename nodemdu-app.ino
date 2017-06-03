#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>

const char *ssid = "GrzesioNet";
const char *password = "UBPUKBQB";
IPAddress ip(192, 168, 0, 101);
IPAddress gateway(192, 168, 0, 1);
IPAddress subnet(255, 255, 255, 0);
int timer;
int mot;
WiFiServer server(80);

void setup()
{
  Serial.begin(9600);
  delay(10);
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  WiFi.begin(ssid, password);
  WiFi.config(ip, gateway, subnet);
  server.begin();
  pinMode(15, INPUT);
}

void loop()
{

  if (digitalRead(15) == 1 && timer < millis())
  {
    digitalWrite(13, HIGH);
    HTTPClient http;
    http.begin("http://192.168.0.100:3000/motion");
    http.GET();
    http.end();
    timer = millis() + 60000;
    mot = digitalRead(15);
  }
  else
  {
    digitalWrite(13, LOW);
  }
  WiFiClient client = server.available();
  if (!client)
  {
    return;
  }

  while (!client.available())
  {
    delay(1);
  }

  String request = client.readStringUntil('\r');
  client.flush();
  if (request.indexOf("/bulbOn") > 0)
  {
    digitalWrite(13, HIGH);
  }
  if (request.indexOf("/bulbOff") > 0)
  {
    digitalWrite(13, LOW);
  }
  delay(1);
}
