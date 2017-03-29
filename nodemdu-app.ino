#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>

//unsigned long time;
const char* ssid = "GrzesioNet";
const char* password = "UBPUKBQB";
IPAddress ip(192, 168, 0, 101);
IPAddress gateway(192, 168, 0, 1);
IPAddress subnet(255, 255, 255, 0);
int timer;
int mot;

WiFiServer server(80);
 
void setup() {

  
  Serial.begin(9600);
  delay(10);
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  
  // Connect to WiFi network
  WiFi.begin(ssid, password);
WiFi.config(ip, gateway, subnet);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  // Start the server
  server.begin();
  Serial.println("Server started");
 
  // Print the IP address
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");

//detector
pinMode(15, INPUT);
}
 
void loop() {

  if(digitalRead(15)==1 && timer<millis()){
    digitalWrite(13, HIGH);
    HTTPClient http;
    http.begin("http://192.168.0.100:3000/motion");
    http.GET();
    http.end();
    timer = millis()+60000;
     mot = digitalRead(15);
  }else{
     digitalWrite(13, LOW);
  }

  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
 
  // Wait until the client sends some data
  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }
 
  // Read the first line of the request
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();
  
  /*if (request.indexOf("/timer:") > 0)  {
     timerStr = request.substring(request.indexOf("/timer")+7,request.lastIndexOf(" "));
     timer = timerStr.toInt()+180;
  }*/
  if (request.indexOf("/bulbOn") > 0)  {
    digitalWrite(13, HIGH);
  }
  if (request.indexOf("/bulbOff") > 0)  {
    digitalWrite(13, LOW);
  }
// Set ledPin according to the request
//digitalWrite(ledPin, value);
 
  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); 
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println("<head>");
 client.println("</head>");

 
  client.println("<a href=\"/bulbOn\"\"><button>Turn On </button></a>");
  client.println("<a href=\"/bulbOff\"\"><button>Turn Off </button></a><br />");  

  
  client.println("</html>"); 
  delay(1);
  Serial.println("Client disonnected");
  Serial.println("");

 
}
