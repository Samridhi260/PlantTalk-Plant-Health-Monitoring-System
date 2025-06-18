#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Pins
#define DHTPIN A0
#define DHTTYPE DHT11
#define SOILPIN A2
#define LDRPIN A1
#define REDPIN 13
#define GREENPIN 12
#define BLUEPIN 11
#define BUZZERPIN 2

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  dht.begin();

  pinMode(SOILPIN, INPUT);
  pinMode(LDRPIN, INPUT);
  pinMode(REDPIN, OUTPUT);
  pinMode(GREENPIN, OUTPUT);
  pinMode(BLUEPIN, OUTPUT);
  pinMode(BUZZERPIN, OUTPUT);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);

  display.setCursor(10, 10);
  display.println("Hi! Welcome to");
  display.setCursor(20, 25);
  display.println("PlantTalk :)");
  display.display();
  delay(2000);
}

void loop() {
  float temp = dht.readTemperature();
  float humidity = dht.readHumidity();
  int soil = analogRead(SOILPIN);
  int light = analogRead(LDRPIN);

  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("Temp: ");
  display.print(temp);
  display.print(" C");

  display.setCursor(0, 10);
  display.print("Humidity: ");
  display.print(humidity);

  display.setCursor(0, 20);
  display.print("Soil: ");
  display.print(soil);

  display.setCursor(0, 30);
  display.print("Light: ");
  display.print(light);
  display.display();
  delay(2000);

  // Sensor input to model decision
  int mood = predictMood(temp, humidity, soil, light);
  showMood(mood);
  delay(2500);
}

// ML Decision Logic (based on your exported DecisionTree)
int predictMood(float temp, float humidity, float soil, float light) {
  if (soil <= 349.35) {
    if (light <= 199.28) {
      return 1; // Thirsty
    } else {
      return 2; // Overheated
    }
  } else {
    if (light <= 203.51) {
      return 3; // Too Dark
    } else {
      return 0; // Happy
    }
  }
}

void showMood(int mood) {
  display.clearDisplay();
  display.setCursor(0, 0);
  switch (mood) {
    case 0:
      display.println("Mood: Happy :)");
      setRGB(0, 255, 0); // Green
      digitalWrite(BUZZERPIN, LOW);
      break;
    case 1:
      display.println("Mood: Thirsty");
      setRGB(0, 255, 255); // Cyan
      digitalWrite(BUZZERPIN, LOW);
      break;
    case 2:
      display.println("Mood: Overheated");
      setRGB(255, 255, 0); // Yellow
      digitalWrite(BUZZERPIN, HIGH);
      break;
    case 3:
      display.println("Mood: Too Dark");
      display.println("Give me some sunshine");
      setRGB(0, 0, 255); // Blue
      digitalWrite(BUZZERPIN, LOW);
      break;
    case 4:
      display.println("Mood: Stressed");
      display.println(":(");
      setRGB(255, 0, 0); // Red
      digitalWrite(BUZZERPIN, HIGH);
      delay(2000);
      digitalWrite(BUZZERPIN, LOW);
      break;
    default:
      display.println("Unknown Mood");
      setRGB(0, 0, 0); // Off
      digitalWrite(BUZZERPIN, LOW);
  }
  display.display();
}

void setRGB(int r, int g, int b) {
  analogWrite(REDPIN, r);
  analogWrite(GREENPIN, g);
  analogWrite(BLUEPIN, b);
}
