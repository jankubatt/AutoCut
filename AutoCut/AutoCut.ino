/*
 __  __     __  __     ______     ______     ______  
/\ \/ /    /\ \/\ \   /\  == \   /\  __ \   /\__  _\ 
\ \  _"-.  \ \ \_\ \  \ \  __<   \ \  __ \  \/_/\ \/ 
 \ \_\ \_\  \ \_____\  \ \_____\  \ \_\ \_\    \ \_\ 
  \/_/\/_/   \/_____/   \/_____/   \/_/\/_/     \/_/
 */

#include <Servo.h>  //Servo library
#include <DHT.h>  //DHT
#include <LiquidCrystal.h>  //LCD

#define outputA 2 //definition of outputA from encoder
#define outputB A2  //definition of outputB from encoder
#define dhtPin A5 //DHT pin
#define dhtType DHT11   //DHT TYPE

int locMin = 10;  //Minimum angle
int locMax = 170; //Maximum angle
int btnSetpin = A1; //btnSet pin
int location = 10;  //location of servo
int btnSet = 10;  //Button for writing location to locSet
int locSet = 10;  //Saved location
int aState; //State of encoder
int aLastState; //Last state of encoder
int startLoc = 20;  //Start location

Servo servo;  //Servo
DHT dht (dhtPin, dhtType);  //DHT
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);  //LCD

//Keys on LCD
int lcd_key     = 0;
int adc_key_in  = 0;
#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

//Reading buttons on LCD
int read_LCD_buttons()
{
 int h = dht.readHumidity();
 int t = dht.readTemperature();
 adc_key_in = analogRead(0);      
 if (adc_key_in > 1000) return btnNONE;
 if (adc_key_in < 50)   return btnRIGHT;  
 if (adc_key_in < 250)  return btnUP; 
 if (adc_key_in < 450)  return btnDOWN; 
 if (adc_key_in < 650)  return btnLEFT; 
 if (adc_key_in > 850)  return btnSELECT;


 return btnNONE;
}


void setup() {
  Serial.begin(9600); //Serial monitor begin
  Serial.println("Choose your default position");

  lcd.begin(16, 2); //LCD start         
  
  servo.attach(A0); //Servo attachment
  servo.write(locSet);  //Write of last set angle

  dht.begin();

  pinMode(btnSetpin, INPUT);  //btnSet pinmode
  pinMode(outputB, INPUT);  //outputB pinmode
  pinMode(outputA, INPUT);  //outputA pinmode
  
  aLastState = digitalRead(outputA);

  servo.write(startLoc);  //Servo starting position

  lcd.setCursor(0,0);
    lcd.print("Loading");
      lcd.setCursor(7,0);
        delay(750);
          lcd.print(".");
            delay(750);
              lcd.setCursor(8,0);
                lcd.print(".");
                  delay(750);
                    lcd.setCursor(9,0);
                      lcd.print(".");
                        delay(750);

  lcd.setCursor(0,0);
  lcd.clear();
  lcd.print("Position:");
  lcd.setCursor(10, 0);
  lcd.print(location); 
}

void loop() {
  aState = digitalRead(outputA);
  btnSet = digitalRead(btnSetpin);
    dhtRun();
    if (aState != aLastState){  //Servo movement    
      if (digitalRead(outputB) != aState) { 
        if (location == 10) {
            location = 10;
        }
        else {
          location --;
          servo.write(location); 
          locSet = location;
        } 
       } 
        else if (location == 170){
                 location == 170;
        }
        else {
          location ++;
          servo.write(location); 
          locSet = location;
        }
          Serial.print("Position: ");
            Serial.println(location);
              locLcd(); //LCD display function
   } 
   
   aLastState = aState; 
    lcd.setCursor(0, 1); 
      btnlcdProcess();  //button with lcd function   
        read_LCD_buttons();
  }  

void btnlcdProcess() {
     if (btnSet == LOW){  //Servo cutting process + LCD
    servo.write(locSet);
      Serial.println("Cutting");
        lcd.print("Cutting");
          delay(750);
            servo.write(locMax);
              Serial.print(".");
                delay(750);
                  Serial.print(".");
                    delay(100);
                      servo.write(locMin);
                        Serial.print(".");
                          delay(100);
                        Serial.println(".");
                      delay(750);
                    servo.write(locSet);
                  delay(100);
                Serial.println("DONE"); 
              lcd.clear();
            lcd.print("Done");
          delay(1000);
        lcd.setCursor(0,0);
      lcd.print("Position:");
    lcd.setCursor(10, 0);
    lcd.print(location); 
 } 
}

void locLcd() {
    lcd.setCursor(0,0);
     lcd.clear();
      lcd.print("Position: ");
       lcd.setCursor(10,0);
        lcd.print(location);
}

void dhtRun() {
  int h = dht.readHumidity();
  int t = dht.readTemperature();

  if (adc_key_in > 850) {
     Serial.print("Temperature: ");
      Serial.print(t);
       Serial.println(" °C");
        Serial.print("Humidity: ");
         Serial.print(h);
          Serial.println(" %");
         lcd.setCursor(0,1);
        lcd.print("Temp: ");
       lcd.setCursor(5,1);
      lcd.print(t);
     lcd.setCursor(10,1);
      lcd.print("Hum: ");
       lcd.setCursor(14,1);
        lcd.print(h);
  } 
}

void loadingScr() {
  lcd.setCursor(0,0);
    lcd.print("Loading");
      lcd.setCursor(0,7);
        delay(750);
          lcd.print(".");
            delay(750);
              lcd.setCursor(0,8);
                lcd.print(".");
                  delay(750);
                    lcd.setCursor(0,9);
                      lcd.print(".");
                        delay(750);
  }
