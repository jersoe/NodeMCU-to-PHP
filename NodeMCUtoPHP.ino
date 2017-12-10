#include <MD5.h>


#include <ESP8266HTTPClient.h>

#include <ESP8266WiFiType.h>
#include <WiFiServer.h>
#include <ESP8266WiFiGeneric.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <ESP8266WiFiScan.h>
#include <ESP8266WiFiSTA.h>
#include <ESP8266WiFiMulti.h>
#include <WiFiClientSecure.h>
#include <WiFiClient.h>
#include <ESP8266WiFiAP.h>
#include <dht.h>

#define dht_apin D7 // Pin sensor is connected to
 
dht DHT;

const char* ssid = "";
const char* password = "";
const String secret_token = "sdhsd8sdhd";


int i = 0;
char letters[] = {'a', 'b', 'c', 'd', 'e', 'f','g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0'};
String randString = "";
String randStringCode = "";

 
void setup () {
  Serial.begin(9600);
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print("Connecting..");
  }
}

void loop() {
  randString="";
  randStringCode="";
  
  
  //Generate random string
    for(i = 0; i<10; i++)
    {
     randString = randString + letters[random(0, 35)];
    }
    
  //Print random string
    Serial.println("Random string: " + randString);

  //Add secret code
    randStringCode=randString+secret_token;

     //generate the MD5 hash for our string
    //char* randChar = (char*) randString.c_str();
    unsigned char* hash=MD5::make_hash((char*) randStringCode.c_str());
    //generate the digest (hex encoding) of our hash
    char *md5str = MD5::make_digest(hash, 16);

    //String hashmd5 = String(md5str);
    Serial.println("Hash: " + (String) md5str);

    //Get measurements
    DHT.read11(dht_apin);
  
  if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status
    HTTPClient http;  //Declare an object of class HTTPClient
    http.begin("http://www.server.com/nodemcutophp/index.php?temp="+(String) DHT.temperature+"&hum="+(String) DHT.humidity+"&code="+randString+"&hash="+(String) md5str+"&room_id=1");  //Specify request destination
    int httpCode = http.GET();                                                                  //Send the request
 
    if (httpCode > 0) { //Check the returning code
      String payload = http.getString();   //Get the request response payload
      Serial.println(payload);                     //Print the response payload
    }
    http.end();   //Close connection
  }
  //memory freeing
  free(hash);
  free(md5str);
  
  delay(30000);    //Send a request every 30 seconds
}
