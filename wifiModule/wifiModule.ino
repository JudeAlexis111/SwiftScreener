#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <ArduinoJson.h>

#define LED 2
#define FIREBASE_HOST "covidtemp-47905.firebaseio.com"
#define FIREBASE_AUTH "0b0DvzjWkzPNUIkaW5jDTKlAh7gZnMRdEPTCGlfe"

String incomingByte = "0";

void setup() {
  // put your setup code here, to run once:
  pinMode(LED, OUTPUT);
  Serial.begin(9600);
  digitalWrite(LED, LOW); // Turn the LED off by making the voltage HIGH

  WiFi.begin("JPHOME", "Slht1903"); 
  Serial.print("connecting"); 
  while (WiFi.status() != WL_CONNECTED) { 
   Serial.print("."); 
   delay(500); 
 } 
 Serial.println(); 
 Serial.print("connected: "); 
 Serial.println(WiFi.localIP()); 

 Firebase.begin(FIREBASE_HOST,FIREBASE_AUTH); 
}

void loop() {
  // put your main code here, to run repeatedly:
  //Serial.write("Hello from ESP");
  
  if (Serial.available()) {
    // read the incoming byte:
    incomingByte = Serial.readString();

    // say what you got:
    Serial.println("i got " + incomingByte);

      Firebase.setString("/temprature", incomingByte);
      if (Firebase.failed()) { 
        Serial.print("setting /number failed:"); 
        Serial.println(Firebase.error());   
        return; 
    } 
    
  }
 
  
  delay(1000); // Wait for a second

  Firebase.setFloat("/number", 52);
      if (Firebase.failed()) { 
        Serial.print("setting /number failed:"); 
        Serial.println(Firebase.error());   
        return; 
    }

  Serial.write("number: "); 
  Serial.println(Firebase.getFloat("number")); 
  delay(1000); 
  
  //digitalWrite(LED, LOW); // Turn the LED on (Note that LOW is the voltage level)
  
  //digitalWrite(LED, HIGH); // Turn the LED off by making the voltage HIGH
  //delay(1000); // Wait for two seconds
  Serial.println(WiFi.localIP()); 
}
