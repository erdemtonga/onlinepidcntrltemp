#include <WiFi.h>
#include <WebServer.h>

WebServer server(80);  //Port-number

const char* ssid="Erdem";
const char* password="erdo0671";

String Website, dat, XML, Javascript;
int inc=0;

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
  Javascript +="xmldoc = xmlResponse.getElementsByTagName('dat');\n";
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
  Website = "<html>\n";

  Website += "<body onload='process()'>";
  Website += "<h6>" + dat + "</body></html><h6>";  
  Website+=Javascript;
  server.send(200, "text/html", Website);  
}

void  XMLcontent() {
  XML = "<?xml version ='1.0'?>";
  XML+="<dat>";
  XML+=dat;
  XML+="</dat>";

  server.send(200,"text/xml",XML);
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while(WiFi.status()!=WL_CONNECTED) delay(500);
  WiFi.mode(WIFI_STA);

  Serial.print(WiFi.localIP());

  server.on("/", WebsiteContent);
  server.on("/xml", XMLcontent);
  server.begin();
}

void loop() {
  inc++;
  dat=(String)inc;
  server.handleClient();
}
