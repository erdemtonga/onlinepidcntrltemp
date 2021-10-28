
#include <WiFi.h>
#include <WebServer.h>
#define RXD2 16
#define TXD2 17
#include <HardwareSerial.h> 

/*Put your SSID & Password*/
const char* ssid = "Erdem";  // Enter SSID here
const char* password = "erdo0671";  //Enter Password here
WebServer server(80);


int toplam;
bool MOD=LOW;
uint8_t ROLE = 5;
bool ROLEDURUMU=LOW;
String Website, temp, XML, Javascript;

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


  server.on("/", WebsiteContent);
  server.on("/xml", XMLcontent);
  server.on("/led1on", handle_led1on);
  server.on("/led1off", handle_led1off);
  server.on("/led2on", handle_led2on);
  server.on("/led2off", handle_led2off);
  server.onNotFound(handle_NotFound);

  server.begin();
  Serial.println("HTTP server started");
}
void loop() {
  temp=Serial2.parseFloat();
  server.handleClient();
  toplam = MOD+ROLEDURUMU;
  Serial2.print(toplam);
  Serial.print(toplam);

 
  delay(1000);
}

void javascriptContent() {
  Javascript = "<SCRIPT>\n";
  //Javascript += "alert('hello')\n";
  Javascript +="var xmlHttp=createXmlHttpObject();\n";
  Javascript +="function createXmlHttpObject(){\n";
  Javascript +="if(window.XMLHttpRequest){\n";
  Javascript +="xmlHttp=new XMLHttpRequest();\n";
  Javascript +="}else{\n";
  Javascript +="xmlHttp=new ActiveXObject('Microsoft.XMLHTTP');\n";
  Javascript +="}\n";
  Javascript +="return xmlHttp;\n";
  Javascript +="}\n";
  Javascript +="\n";
  Javascript +="function response(){\n";
  Javascript +="xmlResponse=xmlHttp.responseXML;\n";
  Javascript +="xmldoc = xmlResponse.getElementsByTagName('temp');\n";
  Javascript +="message = xmldoc[0].firstChild.nodeValue;\n";
  Javascript +="document.getElementById('div1').innerHTML=message;\n";
  Javascript +="}\n";
  Javascript +="function process(){\n";
  Javascript +="xmlHttp.open('PUT','xml',true);\n";
  Javascript +="xmlHttp.onreadystatechange=response;\n";
  Javascript +="xmlHttp.send(null);\n";
  Javascript +="setTimeout('process()',200);\n";
  Javascript +="}\n";
  
  Javascript += "</SCRIPT>\n";
}

void WebsiteContent() {
  javascriptContent();
  MOD = LOW;
  ROLEDURUMU = LOW;
  Serial.println("MOD : OTO | ROLE DURUMU : KAPALI");
  server.send(200, "text/html", SendHTML(false,false)); 
    
}

void  XMLcontent() {
  XML = "<?xml version ='1.0'?>";
  XML+="<temp>";
  XML+=temp;
  XML+="</temp>";

  server.send(200,"text/xml",XML);
}

void handle_led1on() {
  MOD = HIGH;
  Serial.println("MOD: MANUEL");
  server.send(200, "text/html", SendHTML(true,ROLEDURUMU)); 
}

void handle_led1off() {
  MOD = LOW;
  Serial.println("MOD : OTO");
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
  ptr +="<title>PID LAMBA Control</title>\n";
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
  ptr +="<body onload='process()'>";
  ptr +="<h1>ESP32 Web Server</h1>\n";
  ptr +="<h3>Using Station(STA) Mode</h3>\n";
  ptr +="<h4><div id='div1'>SICAKLIK : "+temp+" C </div></h4>\n";  
  
   if(led1stat)
  {ptr +="<p>MOD : MANUEL</p><a class=\"button button-off\" href=\"/led1off\">OTO</a>\n";}
  else
  {ptr +="<p>MOD : OTO</p><a class=\"button button-on\" href=\"/led1on\">MANUEL</a>\n";}

  if(led2stat)
  {ptr +="<p>LAMBA DURUMU: ACIK</p><a class=\"button button-off\" href=\"/led2off\">KAPA</a>\n";}
  else
  {ptr +="<p>LAMBA DURUMU: KAPALI</p><a class=\"button button-on\" href=\"/led2on\">AC</a>\n";}

  ptr +="</body>\n";
  ptr +="</html>";
  ptr +=Javascript;
  return ptr;
}
