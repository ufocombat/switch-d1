/*
 * 01-11-2017 for arduino D1
*/

#include <ESP8266WiFi.h>

static const uint8_t PIN_RELE  = D7;
static const uint8_t LED_SCL   = D5;

const char* ssid     = "Enceladus";
const char* password = "1111111";

const char* host = "ufoiot.azurewebsites.net";
const char* streamId   = "....................";
const char* privateKey = "....................";

void setup() {
  pinMode(PIN_RELE, OUTPUT);  
  digitalWrite(PIN_RELE, LOW);
  
  Serial.begin(115200);
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
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

int value = 0;

void loop() {
  delay(5000);
  ++value;

  Serial.print("connecting to ");
  Serial.println(host);
  
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  
  // We now create a URI for the request
  String url = "http://ufoiot.azurewebsites.net/state";
/*  url += streamId;
  url += "?private_key=";
  url += privateKey;
  url += "&value=";
  url += value;*/
  
  Serial.print("Requesting URL: ");
  Serial.println(url);
  
  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }
  
  // Read all the lines of the reply from server and print them to Serial
  while(client.available()){
    String line = client.readStringUntil('\r');
    //Serial.print(line);
    
    if (line.indexOf("state=")>0)
    {
      Serial.print(line);
      if (line.indexOf("state=1")>0)
      {
        //Включить
        digitalWrite(PIN_RELE, HIGH);
      }
      else
      {
        //Выключить
        digitalWrite(PIN_RELE, LOW);
      }
    }
  }
  
  Serial.println();
  Serial.println("closing connection");
}

