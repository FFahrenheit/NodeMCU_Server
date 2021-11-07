#include <ESP8266WiFi.h>
const char WiFiPassword[] = "12345678";
const char AP_NameChar[] = "LEDControl" ;
 
WiFiServer server(80);
 
String header = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
String html_body = 
"<!DOCTYPE html>"
"<html>"
""
"<head>"
"    <meta name='viewport' content='width=device-width, initial-scale=1.0' />"
"    <meta charset='utf-8'>"
"    <style>"
"        body {"
"            font-size: 140%;"
"            font-family: Verdana, sans-serif;"
"            text-align: center;"
"        }"
""
"        #main {"
"            display: table;"
"            margin: auto;"
"            padding: 0 10px 0 10px;"
"        }"
""
"        h2 {"
"            text-align: center;"
"        }"
""
"        .button {"
"            padding: 10px 10px 10px 10px;"
"            width: 100%;"
"            background-color: #4CAF50;"
"            font-size: 120%;"
"        }"
"    </style>"
"    <title>LED Control</title>"
"</head>"
""
"<body>"
"    <div id='main'>"
"        <h2>LED Control</h2>"
"        <form id='form' action='LED' method='GET'>"
"            <p>Brillo"
"            </p>"
"            <input type='range' min='0' max='255' name='brillo' id='brillo'>"
"            <p>"
"                Tiempo alto"
"            </p>"
"            <input type='range' min='0' max='1800' name='on' id='on'>"
""
"            <p>"
"                Tiempo bajo"
"            </p>"
"            <input type='range' min='0' max='1800' name='off' id='off'>"
""
"        </p>"
""
"            <input class='button' type='submit' value='Configurar'>"
"        </form><br>"
"    </div>"
"    <script>"
"        const brillo = document.getElementById('brillo');"
"        const on = document.getElementById('on');"
"        const off = document.getElementById('off');"
""
"        window.onload = () => {"
"            let params = (new URL(document.location)).searchParams;"
"            brillo.value = params.get('brillo') || 128;"
"            on.value = params.get('on') || 900;"
"            off.value = params.get('off') || 900;"
""
"        }"
"    </script>"
"</body>"
""
"</html";

String request = "";
int LED_Pin = D1;
int brillo = 0;
int t_on = 0;
int t_off = 0;
long last_change;
bool isOn = false;
bool need_reset = false;

void setup() 
{
    pinMode(LED_Pin, OUTPUT); 
 
    boolean conn = WiFi.softAP(AP_NameChar, WiFiPassword);
    server.begin();
    Serial.begin(9600);
 
} // void setup()
 
 
 
void loop() 
{
  long elapsed = millis() - last_change;

  if(isOn){
        
    if(elapsed > t_on || need_reset){
      Serial.println("Turning off");
      last_change = millis();
      analogWrite(LED_Pin, 0);
      need_reset = false;
      isOn = false;
    }
  }else{
    if(elapsed > t_off || need_reset){
      Serial.println("Turning on");
      last_change = millis();
      analogWrite(LED_Pin, brillo);
      need_reset = false;
      isOn = true;
    }
  }

    // Check if a client has connected
    WiFiClient client = server.available();
    

    if (!client)  {  return;  }
 
    // Read the first line of the request
    request = client.readStringUntil('\r');

    // Serial.println(request);
 
    if(request.indexOf("LED") > 0 )  {
      request = request.substring(request.indexOf('?') + 1);
      request = request.substring(0,request.indexOf(' '));
      Serial.println(request);

      request = request.substring(request.indexOf('=') + 1);
      brillo = request.substring(0, request.indexOf('&')).toInt();

      request = request.substring(request.indexOf('=') + 1);
      t_on = request.substring(0, request.indexOf('&')).toInt();

      request = request.substring(request.indexOf('=') + 1);
      t_off = request.toInt();

      Serial.println("Brillo = " + String(brillo));
      Serial.println("Tiempo on = " + String(t_on));
      Serial.println("Tiempo off = " + String(t_off));

      last_change = millis();
      isOn = false;
      need_reset = true;
    }
  
    client.flush();
 
    client.print( header );
    client.print( html_body );   

    delay(5);
  // The client will actually be disconnected when the function returns and 'client' object is detroyed
 
} // void loop()