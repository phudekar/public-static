#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

const char *ssid = "ssid";
const char *password = "password";
const char* host = "rawgit.com";
const char* baseUrl = "/phudekar/public-static/master/esp-lights";

ESP8266WebServer server ( 80 );
WiFiClient client;
const int httpPort = 80;

int red_pin = 12;
int yellow_pin = 14;
int green_pin = 16;

int red_status = 0;
int yellow_status = 0;
int green_status = 0;

void handleRoot() {
  server.send ( 200, "text/html", getDataFromHost("/index.html"));
}

void handleNotFound() {
  server.send ( 404, "text/plain", "Not Found" );
}

int parseState(String state) {
  if (state == "ON")
    return 1;
  return 0;
}

String getDataFromHost(String url){
  String response = "";
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return response;
  }
  
  client.print(String("GET ") + baseUrl + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  delay(10);
  
  while(client.available()){
    String line = client.readStringUntil('\r');
    response += line;
  }
  return response;
}

void handleLight() {
  String light = server.arg("light");
  if (server.method() == HTTP_POST) {
    String state = server.arg("state");
    int stateValue = parseState(state);
    if (light == "red") {
      digitalWrite(red_pin, stateValue);
      red_status = stateValue;
    }
    if (light == "yellow") {
      digitalWrite(yellow_pin, stateValue);
      yellow_status = stateValue;
    }
    if (light == "green") {
      digitalWrite(green_pin, stateValue);
      green_status = stateValue;
    }
    char data[200];
    snprintf ( data, 200, "{'%s':%d}", light.c_str(), stateValue);
    server.send ( 200, "application/json", data  );
  } else {
    char data[0];
    server.send ( 200, "application/json", data  );
  }
}

void setup ( void ) {
  pinMode ( red_pin, OUTPUT );
  pinMode ( yellow_pin, OUTPUT );
  pinMode ( green_pin, OUTPUT );

  digitalWrite ( green_pin, 0 );
  digitalWrite ( red_pin, 0 );
  digitalWrite ( yellow_pin, 0 );
  
  Serial.begin ( 115200 );
  WiFi.begin ( ssid, password );
  Serial.println ( "" );

  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
  }

  Serial.println ( "" );
  Serial.print ( "Connected to " );
  Serial.println ( ssid );
  Serial.print ( "IP address: " );
  Serial.println ( WiFi.localIP() );

  server.on ( "/", handleRoot );
  server.on ( "/light", handleLight );
  server.on ( "/status", []() {
    char data[400];
    snprintf ( data, 400, "{\"red\":%d, \"yellow\":%d, \"green\":%d}", red_status, yellow_status, green_status);
    server.send ( 200, "application/json", data  );
  } );
  server.onNotFound ( handleNotFound );
  server.begin();
  Serial.println ( "HTTP server started" );
}

void loop ( void ) {
  server.handleClient();
}