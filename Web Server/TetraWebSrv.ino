#include <ESP8266WiFi.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>

#define Pin0 D1
#define Pin1 D2
#define Pin2 D5
#define Pin3 D7


AsyncWebServer server(80); //server port 80

void notFound(AsyncWebServerRequest *request)
{
  request->send(404, "text/plain", "Not Found");
}


void setup(void) 
{    
  //Check LittleFs mounted correctly with files available
  Serial.begin(115200);
  if(!LittleFS.begin()){
    Serial.println("An Error has occurred while mounting LittleFS");
}
  delay(1);
  Serial.println("Listing files:");
  Dir dir = LittleFS.openDir("/");
  while (dir.next()) {
    Serial.print("File: ");
    Serial.println(dir.fileName());
}
  //Initialize pins to communicate with Maker Pi
  pinMode(Pin0,OUTPUT);
  pinMode(Pin1,OUTPUT);
  pinMode(Pin2,OUTPUT);
  pinMode(Pin3,OUTPUT);

  WiFi.softAP("TetraWEBSrv", "");    //defines access point name and password
  Serial.print("softap ");
  Serial.println(WiFi.softAPIP());  //print IP of access point to serial monitor 
  
//Main page
  server.on("/home",HTTP_GET, [](AsyncWebServerRequest *request)
  {
    digitalWrite(Pin0, LOW);
    digitalWrite(Pin1, LOW);
    digitalWrite(Pin2, LOW);
    digitalWrite(Pin3, LOW);
    request->send(LittleFS, "/tetraWebSrv.html", "text/html");
  });

//formating and images
  server.on("/index.css",HTTP_GET, [](AsyncWebServerRequest *request)
  {
    request->send(LittleFS, "/index.css", "text/css");
  });
  server.on("/keyboard_key_up.png",HTTP_GET, [](AsyncWebServerRequest *request)
  {
    request->send(LittleFS, "/keyboard_key_up.png", "text/plain");
  });
  server.on("/keyboard_key_left.png",HTTP_GET, [](AsyncWebServerRequest *request)
  {
    request->send(LittleFS, "/keyboard_key_left.png", "text/plain");
  });
  server.on("/keyboard_key_right.png",HTTP_GET, [](AsyncWebServerRequest *request)
  {
    request->send(LittleFS, "/keyboard_key_right.png", "text/plain");
  });
  server.on("/keyboard_key_down.png",HTTP_GET, [](AsyncWebServerRequest *request)
  {
    request->send(LittleFS, "/keyboard_key_down.png", "text/plain");
  });
  server.on("/stop.png",HTTP_GET, [](AsyncWebServerRequest *request)
  {
    request->send(LittleFS, "/stop.png", "text/plain");
  });
  server.on("/autopilot.png",HTTP_GET, [](AsyncWebServerRequest *request)
  {
    request->send(LittleFS, "/autopilot.png", "text/plain");
  });
//Movement control
  //stop
  server.on("/stop", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    digitalWrite(Pin0, LOW);
    digitalWrite(Pin1, LOW);
    digitalWrite(Pin2, LOW);
    digitalWrite(Pin3, LOW);
    request->send(LittleFS, "/tetraWebSrv.html", "text/html");
  });
  //backwards
  server.on("/back", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    digitalWrite(Pin0, LOW);
    digitalWrite(Pin1, LOW);
    digitalWrite(Pin2, LOW);
    digitalWrite(Pin3, HIGH);
    request->send(LittleFS, "/tetraWebSrv.html", "text/html");
  });
  //Turn Left
  server.on("/left", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    digitalWrite(Pin0, LOW);
    digitalWrite(Pin1, LOW);
    digitalWrite(Pin2, HIGH);
    digitalWrite(Pin3, LOW);
    request->send(LittleFS, "/tetraWebSrv.html", "text/html");
  });
  //Turn right
  server.on("/right", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    digitalWrite(Pin0, LOW);
    digitalWrite(Pin1, LOW);
    digitalWrite(Pin2, HIGH);
    digitalWrite(Pin3, HIGH);
    request->send(LittleFS, "/tetraWebSrv.html", "text/html");
  });
  //forwards
  server.on("/forward", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    digitalWrite(Pin0, LOW);
    digitalWrite(Pin1, HIGH);
    digitalWrite(Pin2, LOW);
    digitalWrite(Pin3, LOW);
    request->send(LittleFS, "/tetraWebSrv.html", "text/html");
  });
  //sentry mode
  server.on("/auto", HTTP_GET, [](AsyncWebServerRequest *request)
  {
    digitalWrite(Pin0, LOW);
    digitalWrite(Pin1, HIGH);
    digitalWrite(Pin2, LOW);
    digitalWrite(Pin3, HIGH);
    request->send(LittleFS, "/tetraWebSrv.html", "text/html");
  });

  server.onNotFound(notFound);
  server.begin();
}

void loop() {}
