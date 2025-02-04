#include <Wire.h>
#include <LiquidCrystal_I2C.h>

int trigpin = 9;
int echopin = 10;
int pingtime;
float d;
int level;
const float depth = 20.0; // Hardcoded depth of the tank in cm
const float maxVolume = 314.0; // Maximum volume in milliliters
const float maxPercentage = 83.0; // Maximum percentage when the tank is full

// Initialize LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
    // Pin setup
    pinMode(trigpin, OUTPUT);
    pinMode(echopin, INPUT);

    // Start serial communication and LCD
    Serial.begin(9600);
    lcd.init();
    lcd.backlight();

    // Display initial message
    lcd.setCursor(0, 0);
    lcd.print("Water Tank Depth");
    lcd.setCursor(0, 1);
    lcd.print("Set to 20 cm");
    Serial.println("Water Tank Depth Set to 20 cm");
    delay(1000); // Display the message for 1 second
    lcd.clear();
}

void loop() {
    // Trigger ultrasonic sensor
    digitalWrite(trigpin, LOW);
    delayMicroseconds(10);
    digitalWrite(trigpin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigpin, LOW);

    // Measure pulse duration
    pingtime = pulseIn(echopin, HIGH);

    // Calculate distance (calibrated equation)
    d = (8.0 / 400.0) * pingtime;

    // Calculate water level percentage
    level = ((depth - d) / depth) * 100.0;

    // Clamp level between 0% and 100%
    level = constrain(level, 0, 100);

    // Calculate water volume in milliliters
    float volume = (level / maxPercentage) * maxVolume;

    // Display water level status on LCD
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Tank Level: ");

    if (level < 21) {
        lcd.print("Low");
    } else if (level >= 21 && level < 42) {
        lcd.print("Mid Low");
    } else if (level == 42) {
        lcd.print("Mid");
    } else if (level > 42 && level <= 63) {
        lcd.print("Mid High");
    } else {
        lcd.print("Full");
    }

    lcd.setCursor(0, 1);
    lcd.print(volume, 1);
    lcd.print(" ml ");
    lcd.print(level);
    lcd.print("%");

    // Output water level status to Serial Monitor
    Serial.print("Tank Level: ");
    if (level < 25) {
        Serial.print("Low");
    } else if (level >= 25 && level < 50) {
        Serial.print("Mid Low");
    } else if (level == 50) {
        Serial.print("Mid");
    } else if (level > 50 && level <= 80) {
        Serial.print("Mid High");
    } else {
        Serial.print("High");
    }
    Serial.print(" - ");
    Serial.print(volume, 1);
    Serial.print(" ml - ");
    Serial.print(level);
    Serial.println("%");

    delay(100); // Update every 100 milliseconds
}
