#include <TinyGPS.h>
#include <SoftwareSerial.h>
#include <MPU6050_tockn.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);
float xaxis;
MPU6050 mpu6050(Wire);
//LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3);
SoftwareSerial Gsm(10,11);
char phone_no[] = "+919892245891"; //replace with phone no. to get sms
TinyGPS gps;  //Creates a new instance of the TinyGPS object
boolean newData=false;
boolean angle=false;
boolean sens=false;
int knockSensor = 0;               
byte val = 0;
int THRESHOLD = 100;


void setup() 

{
  // initialize the LCD, 
  lcd.begin(16,2);
  // Turn on the blacklight.
  lcd.backlight();   
  
  
  Serial.begin(9600);
  Gsm.begin(9600);  
  Wire.begin();
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);



}

void loop() 
{


  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Accident");
  lcd.setCursor(0,1);
  lcd.print("Detection sys");
  
  
  val = analogRead(knockSensor);     

  
  
  
  
  if (val >= THRESHOLD) 
{
  lcd.clear();
    lcd.setCursor(1,0);
    lcd.print("Accident Detected"); 
    lcd.clear();
    lcd.setCursor(1,0);
    lcd.print("Are u Safe"); 
    lcd.setCursor(0,1);
    lcd.print("Press Reset"); 
    delay(10000);
  sens=true;
 }

  delay(100);  // we have to make a delay to avoid overloading the serial port
//////////////////////////////
  mpu6050.update();
  xaxis=mpu6050.getAngleX();
  if(xaxis >=30 || xaxis<=-30 )
  {
  lcd.clear();// clearn previous values from screen
  lcd.setCursor(1,0);
  //lcd.print(mpu6050.getAngleX());
  lcd.print("Angle Changed");
  angle=true;
  }
////////////////////////////
if(sens==true || angle==true)
{
    lcd.clear();
    lcd.setCursor(1,0);
    
    lcd.print("Sending SMS");
    delay(100);
   //newData=true;
//bool newData = false;
  unsigned long chars;
  unsigned short sentences, failed;

  // For one second we parse GPS data and report some key values

  for (unsigned long start = millis(); millis() - start < 1000;)

  {

    while (Serial.available())

    {
      char c = Serial.read();
      Serial.print(c);
      if (gps.encode(c))
        newData = true;  

    }

  }

  if (newData==true)      //If newData is true
  {
    float flat, flon;
    unsigned long age;
    gps.f_get_position(&flat, &flon, &age);
    Gsm.print("AT+CMGF=1\r");
    //Serial.print("Hre");
    delay(400);
    Gsm.print("AT+CMGS=\"");
    Gsm.print(phone_no);
    Gsm.println("\"");

    delay(300);
    Gsm.print("http://maps.google.com/maps?q=loc:");
   // Gsm.print("Latitude = ");
    Gsm.print(flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat, 6);
    //Gsm.print(" Longitude = ");
    Gsm.print(",");
    Gsm.print(flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon, 6);
    delay(200);
    Gsm.println((char)26); // End AT command with a ^Z, ASCII code 26
    delay(200);
    Gsm.println();
    delay(200000);
  }

  Serial.println(failed);
 // if (chars == 0)
   // Serial.println("** No characters received from GPS: check wiring **");
}

    
}
