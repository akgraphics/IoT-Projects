 #include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>
//I2C pins declaration
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
SoftwareSerial BTSerial(2,3); // RX | TX
int BULB1 = 13;
int BULB2 = 11;
int sensorValue;
int buzz=10;

void setup() 
{
  lcd.begin(16, 2); 
  Serial.begin(9600);
   BTSerial.begin(9600);
    pinMode(BULB1,OUTPUT); // Trigger for Bulb 1
    pinMode(BULB2,OUTPUT); // Trigger for Bulb 2

   
    lcd.print("....WELCOME.....");
       lcd.setCursor(0,1);
       delay(2000);
    lcd.clear();
    lcd.setCursor(0,1);
    lcd.print("Plz Wait....");
    delay(2000);
    lcd.clear();

   
}
void loop()
{
   int sensorValue = analogRead(A0); 
   if(sensorValue>350)
{  
   
  lcd.print("Gas detected ");
  lcd.setCursor(13,0);
     lcd.print(sensorValue);

  pinMode(buzz,OUTPUT);
  digitalWrite(buzz,1);
  delay(300);
    digitalWrite(buzz,0);
  delay(300);
  lcd.clear();
}else{
   int sensorValue = analogRead(A0); 
  lcd.print("No Leakage ");
    lcd.print(0,1);
    lcd.setCursor(10,0);
    digitalWrite(buzz,0);
    

      
    lcd.print(sensorValue); 
     lcd.setCursor(0,1);
    delay(500);
   lcd.clear();
}
 if(BTSerial.available())
{
    Serial.println("BTSerial is available");
    char data=BTSerial.read();
    switch (data) 
{
    case 'a':
      digitalWrite(BULB1,1);
      Serial.println("Changed the BULB1 value to HIGH");
      break;
    case 'b':
      digitalWrite(BULB1,0);
      Serial.println("Changed the BULB1 value to LOW");
      break;
        default: 
      // if nothing else matches, do the default
      // default is optional
    break;

    
    case 'c':
      digitalWrite(BULB2,1);
      Serial.println("Changed the BULB2 value to HIGH");
      break;
    case 'd':
      digitalWrite(BULB2,0);
     
      Serial.println("Changed the BULB2 value to LOW");
      break;
         
      // if nothing else matches, do the default
      // default is optional
    break;
 
}
}}
