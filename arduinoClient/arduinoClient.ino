/* Basic communication between arduino (esp8266 board) and raspberry pi
 * Author: Ivan Moreno
 *	May 2020
 */
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

const char* ssid     = "rpi-hotspot";
const char* password = "rasp-p@s2w0rd";

const char* host = "192.168.7.1";
const uint16_t port = 35000;

ESP8266WiFiMulti WiFiMulti;

void setup() {
  Serial.begin(115200);
  while (!Serial); // Wait serial

  // We start by connecting to a WiFi network
  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP(ssid, password);

  Serial.println();
  Serial.println();
  Serial.print("Wait for WiFi... ");

  while (WiFiMulti.run() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  delay(500);
}


void loop() {
  Serial.print("Connecting to ");
  Serial.print(host);
  Serial.print(':');
  Serial.println(port);

  // Use WiFiClient class to create TCP connections
  WiFiClient client;

  if (!client.connect(host, port)) {
    Serial.println("Connection failed");
    Serial.println("Wait 5 sec...");
    delay(5000);
    return;
  }

  // This will send the request to the server
  client.print("Init connection from ESP8266");
  Serial.println("Wait response from server ...");

  String serialBuff, clientBuff;

   while (true) {

    while (client.available()) {
      clientBuff += String((char)client.read()); 
    }
    while (Serial.available()) {
      char auxc = Serial.read();
      Serial.write(auxc);
      serialBuff += String(auxc); 
    }
    
    if (clientBuff.length() > 0 ) {
      if (clientBuff.indexOf("quit!") >= 0) 
	break;

      Serial.println("Server response: " + clientBuff);
      Serial.print("You response -> ");
      clientBuff = "";
    }
    if (serialBuff.endsWith("\r")) {
      client.print(serialBuff);
      Serial.println();
      serialBuff = "";
    }
  } 

  Serial.println("\nClose connection...");
  client.stop();

  Serial.println("Wait 5 sec...");
  delay(5000);
}
