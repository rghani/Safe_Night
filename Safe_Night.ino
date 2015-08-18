#include <LiquidCrystal.h>
#include <SoftwareSerial.h>
//#include <Time.h>
#include <Servo.h>


LiquidCrystal screen(12, 11, 5, 4, 3, 2);
Servo lock;

SoftwareSerial BT(9,10); //BT(RX pin on Arduino, TX pin on Arduino)

#define temperaturePin A0
#define piezoPin 7
#define lockPin 6
#define lockButtonPin 10

bool lockState = true;
float temperature = 25.;
float temperatureReading = 0;
char inComingMessage;

void setup() {
  BT.begin(9600);
  screen.begin(16, 2);
  lock.attach(lockPin);
  lock.write(90);
  screen.noCursor();
  screen.noBlink();
  pinMode(13, OUTPUT);
  pinMode(piezoPin, OUTPUT);
  pinMode(temperaturePin, INPUT);
  String welcome = "Welcome to Safe Night";
  screen.clear();
  screen.home();
  screen.print(welcome);
  delay(1000);
  for (int i = 0; i < welcome.length(); i++)
  {
    screen.scrollDisplayLeft();
  
    delay(250);
  }
  delay(1000);
  screen.clear();
}

void loop() {
  // put your main code here, to run repeatedly:
  if (BT.available()) {
      inComingMessage =BT.read();
      if (inComingMessage == '1' && lockState == false){
        lock.write(90);
        lockState = true;
      }
      else if (inComingMessage == '0' && lockState == true){
        lock.write(0);
        lockState == false;
      }
      else if (inComingMessage == '1' && lockState == true){
        lockState = true;
        lock.write(90);
      }
      else{
        lockState = false;
        lock.write(0);
      }
      Serial.println(lockState);
   
  }
   //temperatureReading = analogRead(temperaturePin);
//temperature = temperatureReading/9.31; 

 
   screen.home();
   screen.print("Status:");
   screen.print("  ");
   if (lockState == true){
    screen.setCursor(7,0);
    screen.print("  ");
    screen.setCursor(9,0);
    screen.print("Locked");
   }
   else{
    screen.setCursor(7,0);
    screen.print("Un");
    screen.setCursor(9,0);
    screen.print("l");
   }
   screen.setCursor(0,1);
   screen.print("Temperature");
   if (temperature < 10 && temperature > -10)
   {
     screen.setCursor(12,1);
     screen.print((int)temperature);
   }
   else
   {
     screen.setCursor(12,1);
     screen.print((int)temperature);
   }
   screen.print((char)223);
   screen.print("C");

}
