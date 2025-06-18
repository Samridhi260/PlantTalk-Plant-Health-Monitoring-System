#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(32,16,2);

const int ldr = A1;
const int Soil= A2;
const int Motor= 7;
const int Buzzer= 6;

// RGB LED pins
const int redPin = 13;
const int greenPin = 11;
const int bluePin = 12;

void setup()
{
  Serial.begin(9600);
  pinMode(ldr, INPUT);
  pinMode(Soil,INPUT);
  pinMode(Motor,OUTPUT);
  pinMode(Buzzer,OUTPUT);

  // RGB pin setup
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  lcd.begin(16,2);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("   Hi...");
  lcd.setCursor(0, 1);
  lcd.print("   Welcome To ");
  delay(2000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(" Health ");
  lcd.setCursor(0, 1);
  lcd.print(" Monitoring ");
  delay(1000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("  System ");
  delay(1000);
}

void loop() 
{
  int Lm_35 = analogRead(A0);
  int LDR = analogRead(ldr);
  int Moisture = analogRead(Soil);

  float temperature = (Lm_35 * 5.0 / 1024.0 - 0.5 ) * 100.0;

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temperature: ");
  lcd.setCursor(0, 1);
  lcd.print(temperature);
  lcd.print(" C");
  delay(2000);

  // RGB logic based on temperature
  if (temperature < 20) {
    // Cold → Blue
    digitalWrite(redPin, LOW);
    digitalWrite(greenPin, LOW);
    digitalWrite(bluePin, HIGH);
  }
  else if (temperature >= 20 && temperature <= 30) {
    // Normal → Green
    digitalWrite(redPin, LOW);
    digitalWrite(greenPin, HIGH);
    digitalWrite(bluePin, LOW);
  }
  else {
    // Hot → Red
    digitalWrite(redPin, HIGH);
    digitalWrite(greenPin, LOW);
    digitalWrite(bluePin, LOW);
  }

  if(temperature<10||temperature>40)
  {
    digitalWrite(Buzzer,HIGH);
    delay(3000);
    digitalWrite(Buzzer,LOW);
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Darkness :   ");
  lcd.setCursor(0, 1);
  lcd.print(LDR);
  delay(2000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Soil Moisture : ");
  lcd.setCursor(0, 1);
  lcd.print(Moisture);
  delay(2000);

  if(Moisture<=200) 
  {
    digitalWrite(Motor,HIGH);
  }
  else
  {
    digitalWrite(Motor,LOW);
  }
}
