//#include <ESP8266WiFi.h>
#include <Adafruit_MLX90614.h>


#include <hidboot.h>
#include <usbhub.h>
#include <LiquidCrystal.h>
#include <Wire.h>

#include <SoftwareSerial.h>
SoftwareSerial mySerial(10,24);  //RX, TX

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

String AddedValue = "";
int Contrast = 20;
bool stick = false;
String temprature = "";
int redPin = 7;
int greenPin = 8;
int buzzer = 22;
int reset = 23;

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

class KbdRptParser : public KeyboardReportParser
{
    void PrintKey(uint8_t mod, uint8_t key);

  protected:
    void OnControlKeysChanged(uint8_t before, uint8_t after);

    void OnKeyDown  (uint8_t mod, uint8_t key);
    void OnKeyUp  (uint8_t mod, uint8_t key);
    void OnKeyPressed(uint8_t key);
};

void KbdRptParser::PrintKey(uint8_t m, uint8_t key)
{
  MODIFIERKEYS mod;
  *((uint8_t*)&mod) = m;
  Serial.print((mod.bmLeftCtrl   == 1) ? "C" : " ");
  Serial.print((mod.bmLeftShift  == 1) ? "S" : " ");
  Serial.print((mod.bmLeftAlt    == 1) ? "A" : " ");
  Serial.print((mod.bmLeftGUI    == 1) ? "G" : " ");

  Serial.print(" >");
  PrintHex<uint8_t>(key, 0x80);
  Serial.print("< ");

  Serial.print((mod.bmRightCtrl   == 1) ? "C" : " ");
  Serial.print((mod.bmRightShift  == 1) ? "S" : " ");
  Serial.print((mod.bmRightAlt    == 1) ? "A" : " ");
  Serial.println((mod.bmRightGUI    == 1) ? "G" : " ");
};

void KbdRptParser::OnKeyDown(uint8_t mod, uint8_t key)
{
  Serial.print("DN ");
  PrintKey(mod, key);
  uint8_t c = OemToAscii(mod, key);

  if (c)
    OnKeyPressed(c);
}

void KbdRptParser::OnControlKeysChanged(uint8_t before, uint8_t after) {

  MODIFIERKEYS beforeMod;
  *((uint8_t*)&beforeMod) = before;

  MODIFIERKEYS afterMod;
  *((uint8_t*)&afterMod) = after;

  if (beforeMod.bmLeftCtrl != afterMod.bmLeftCtrl) {
    Serial.println("LeftCtrl changed");
  }
  if (beforeMod.bmLeftShift != afterMod.bmLeftShift) {
    Serial.println("LeftShift changed");
  }
  if (beforeMod.bmLeftAlt != afterMod.bmLeftAlt) {
    Serial.println("LeftAlt changed");
  }
  if (beforeMod.bmLeftGUI != afterMod.bmLeftGUI) {
    Serial.println("LeftGUI changed");
  }

  if (beforeMod.bmRightCtrl != afterMod.bmRightCtrl) {
    Serial.println("RightCtrl changed");
  }
  if (beforeMod.bmRightShift != afterMod.bmRightShift) {
    Serial.println("RightShift changed");
  }
  if (beforeMod.bmRightAlt != afterMod.bmRightAlt) {
    Serial.println("RightAlt changed");
  }
  if (beforeMod.bmRightGUI != afterMod.bmRightGUI) {
    Serial.println("RightGUI changed");
  }

}

void KbdRptParser::OnKeyUp(uint8_t mod, uint8_t key)
{
  Serial.print("UP ");
  PrintKey(mod, key);
}

void KbdRptParser::OnKeyPressed(uint8_t key)
{
  String chare = "";
  
  if(AddedValue.length() >= 9){
    AddedValue = "";
  }
  
  Serial.print("ASCII: ");
  Serial.println((char)key);

  chare = (char)key;

  if(chare.indexOf(" ") < 0){
    AddedValue = AddedValue + ((char)key);
  }
  
  


  Serial.println(AddedValue);
};

USB     Usb;
//USBHub     Hub(&Usb);
HIDBoot<USB_HID_PROTOCOL_KEYBOARD>    HidKeyboard(&Usb);

KbdRptParser Prs;

#include <Wire.h>
//#include <KeyboardController.h>

Adafruit_MLX90614 mlx = Adafruit_MLX90614();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(24, OUTPUT);
  mySerial.begin(9600);
  analogWrite(6,Contrast);
  lcd.begin(16, 2);

  #if !defined(__MIPSEL__)
    while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
  #endif
    Serial.println("Start");
  
    if (Usb.Init() == -1)
      Serial.println("OSC did not start.");
  
    delay( 200 );
  
    HidKeyboard.SetReportParser(0, &Prs);
  
  //Serial.println("test");
  mlx.begin();

  delay(3000);

 //WiFi.begin("JPGUEST", "Welcome2OurH0me"); 
 //Serial.print("connecting"); 
 //while (WiFi.status() != WL_CONNECTED) { 
 //  Serial.print("."); 
 //  delay(500); 
 //} 
 //Serial.println(); 
 //Serial.print("connected: "); 
 //Serial.println(WiFi.localIP()); 

}

void loop() {
  // put your main code here, to run repeatedly:

  String IncomingSerial = "";
  boolean StringReady = false;

  //mySerial.write("Hello");
  //delay(1000);
  
  //Serial.print("Ambient Temprature: "); Serial.println(mlx.readAmbientTempC());
  //Serial.println("Object Temprature: "); 
  //Serial.println(mlx.readObjectTempF());

  if(stick == false){
    temprature = String(mlx.readObjectTempF());
  }

  //Serial.println(mySerial.available());
  
  //while(mySerial.available() > 0){
  //  IncomingSerial = mySerial.readString();
  //  Serial.println(mySerial.read());
  //  StringReady = true;
  //}

  //mySerial.println("TEMP:"+temprature);
  //mySerial.println("CODE:"+AddedValue);
  
  //Serial.println(mySerial.readString());

  //if(StringReady){
  //  Serial.println("Recieved String " + IncomingSerial);
  //}
  
  if((temprature == "1899.59" or temprature == "-459.67") && stick == false){
    temprature = "Move Closer";
    lcd.setCursor(0, 0);
    lcd.print("Temp:" + temprature);
    
    lcd.setCursor(0, 1);
    lcd.print("Code: ");
  }

  else{

    if(stick == false){

      stick = true;

      Serial.println("stick");
      
      int starttime = millis();
      int endtime = starttime;
      
        lcd.setCursor(0, 0);
        lcd.print("Temp:" + temprature + "           ");
      
        lcd.setCursor(0, 1);
        lcd.print("Code: ");

        //temprature = String(mlx.readObjectTempF());
        endtime = millis();
      
    }
  }

  Usb.Task();


  if(AddedValue.length() >= 9){
     //lcd.clear();
     
     lcd.setCursor(0, 0);
     lcd.print("Temp:" + temprature);
     
     lcd.setCursor(0, 1);
     lcd.print("Code: " + AddedValue);

     delay(2000);

     if(temprature.toInt() >= 98){
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Denied");
      
      digitalWrite(redPin, HIGH);
      tone(buzzer, 450);
      delay(1000);
      digitalWrite(redPin, LOW);
      noTone(buzzer);

      //mySerial.begin(9600);
      //delay(1000);

      mySerial.println("CODE:"+AddedValue);
      mySerial.println("TEMP:"+temprature);

      AddedValue = "";
      temprature = "";

      lcd.clear();

      lcd.setCursor(0, 0);
      lcd.print("Temp:");
     
      lcd.setCursor(0, 1);
      lcd.print("Code: ");

      stick = false;
     }

     else{
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Admitted");
      
      digitalWrite(greenPin, HIGH);
      tone(buzzer, 900);
      delay(1000);
      digitalWrite(greenPin, LOW);
      noTone(buzzer);

      //mySerial.begin(9600);
      //delay(1000);

      mySerial.println("CODE:"+AddedValue);
      mySerial.println("TEMP:"+temprature);
      
      AddedValue = "";
      temprature = "";

      lcd.clear();

      lcd.setCursor(0, 0);
      lcd.print("Temp:");
     
      lcd.setCursor(0, 1);
      lcd.print("Code: ");

      stick = false;
     }
  }


}
