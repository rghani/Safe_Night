#include <LiquidCrystal.h>
#include <SoftwareSerial.h>
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
  //Establish bluetooth serial connection
  BT.begin(9600);
  
  //Set up LCD
  screen.begin(16, 2);
  
  //Set up servo motor
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
  // check if Bluetooth serial communication is available
  if (BT.available()) {
      //reads Bluetooth message from paired deivce
      inComingMessage =BT.read();
      delay(200);
      if (inComingMessage == '1' && lockState == false){ //sending a 1 when unlocked will lock device
        lock.write(90);
        delay(200);
        lockState = true;
      }
      else if (inComingMessage == '0' && lockState == true){ //sending a 0 when locked will unlock device
        lock.write(0);
        delay(200);
        lockState == false;
      }
      else if (inComingMessage == '1' && lockState == true){ //sending a 1 when locked will lock device
        lockState = true;
        lock.write(90);
        delay(200);
      }
      else{
        lockState = false; // sending a 0 when unlocked will unlock device
        lock.write(0);
        delay(200);
      }
   
  }
  //reads input from temperature sensor and converts it to degrees celcius
   //temperatureReading = analogRead(temperaturePin);
//temperature = temperatureReading/9.31; 

 //status of lock is outputted to the LCD
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
   //temperature is outputted to LCD
   screen.print("Temperature");
   //This if/else block is to deal with the case where the temperature is 1 digit 
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
