

#include <WiFi.h>
#include <WebServer.h>
#define RXD2 16
#define TXD2 17
#include <HardwareSerial.h> 

/*Put your SSID & Password*/
const char* ssid = "Erdem";  // Enter SSID here
const char* password = "erdo0671";  //Enter Password here

WebServer server(80);


bool MOD=LOW;
uint8_t ROLE = 5;
bool ROLEDURUMU=LOW;

void setup() {
  Serial.begin(9600);
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
  delay(100);
  pinMode(ROLE, OUTPUT);
  digitalWrite(ROLE ,0);

  Serial.println("Connecting to ");
  Serial.println(ssid);
  
  //connect to your local wi-fi network
  WiFi.begin(ssid, password);

  //check wi-fi is connected to wi-fi network
  while (WiFi.status() != WL_CONNECTED) {
  delay(1000);
  Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected..!");
  Serial.print("Got IP: ");  Serial.println(WiFi.localIP());

  server.on("/", handle_OnConnect);
  server.on("/led1on", handle_led1on);
  server.on("/led1off", handle_led1off);
  server.on("/led2on", handle_led2on);
  server.on("/led2off", handle_led2off);
  server.onNotFound(handle_NotFound);

  server.begin();
  Serial.println("HTTP server started");
}
void loop() {
  server.handleClient();
  Serial2.print(MOD);
  delay(1000);
  
  
}

void handle_OnConnect() {
  MOD = LOW;
  ROLEDURUMU = LOW;
  Serial.println("MOD : MANUEL | ROLE DURUMU : KAPALI");
  server.send(200, "text/html", SendHTML(MOD,ROLEDURUMU)); 
}

void handle_led1on() {
  MOD = HIGH;
  Serial.println("MOD: OTO");
  server.send(200, "text/html", SendHTML(true,ROLEDURUMU)); 
}

void handle_led1off() {
  MOD = LOW;
  Serial.println("MOD : MANUEL");
  server.send(200, "text/html", SendHTML(false,ROLEDURUMU)); 
}

void handle_led2on() {
  ROLEDURUMU = HIGH;
  Serial.println("ROLE DURUMU : ACIK");
  server.send(200, "text/html", SendHTML(MOD,true)); 
}

void handle_led2off() {
  ROLEDURUMU = LOW;
  Serial.println("ROLE DURUMU: KAPALI");
  server.send(200, "text/html", SendHTML(MOD,false)); 
}

void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}

String SendHTML(uint8_t led1stat,uint8_t led2stat){
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr +="<title>LED Control</title>\n";
  ptr +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr +="body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
  ptr +=".button {display: block;width: 80px;background-color: #3498db;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
  ptr +=".button-on {background-color: #3498db;}\n";
  ptr +=".button-on:active {background-color: #2980b9;}\n";
  ptr +=".button-off {background-color: #34495e;}\n";
  ptr +=".button-off:active {background-color: #2c3e50;}\n";
  ptr +="p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  ptr +="</style>\n";
  ptr +="</head>\n";
  ptr +="<body>\n";
  ptr +="<h1>ESP32 Web Server</h1>\n";
    ptr +="<h3>Using Station(STA) Mode</h3>\n";
  
   if(led1stat)
  {ptr +="<p>MOD : OTO</p><a class=\"button button-off\" href=\"/led1off\">MANUEL</a>\n";}
  else
  {ptr +="<p>MOD : MANUEL</p><a class=\"button button-on\" href=\"/led1on\">OTO</a>\n";}

  if(led2stat)
  {ptr +="<p>ROLE DURUMU: ACIK</p><a class=\"button button-off\" href=\"/led2off\">KAPA</a>\n";}
  else
  {ptr +="<p>ROLE DURUMU: KAPALI</p><a class=\"button button-on\" href=\"/led2on\">AC</a>\n";}

  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}
