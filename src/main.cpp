#define BLYNK_TEMPLATE_ID "TMPL3ik--bOz5"
#define BLYNK_TEMPLATE_NAME "exam paper leakage"
#define BLYNK_AUTH_TOKEN "R5zwb03III9vVQxDbbLM1M695iAWDcU1"

#include <Arduino.h> // Include the Arduino core library for ESP32
#include <WiFi.h>    // Include the ESP32 WiFi library
#include <WiFiClient.h>
#include <Adafruit_Fingerprint.h> // https://github.com/adafruit/Adafruit-Fingerprint-Sensor-Library
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <BlynkSimpleEsp32.h>

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&Serial2);

int relayPin = 25;
// int buzzerPin = 26;
int additionalRelayPin = 26;

// bool toggleState_1 = LOW; // Define integer to remember the toggle state for relay 1

// BLYNK_WRITE(V1)
// { // Virtual pin to control the additional relay
//   toggleState_1 = param.asInt();
//   digitalWrite(additionalRelayPin, !toggleState_1);
//   // digitalWrite(RelayPin1, !toggleState_1);
// }

// BLYNK_CONNECTED()
// {
//   // Request the latest state from the server
//   if (fetch_blynk_state)
//   {
//     Blynk.syncVirtual(VPIN_BUTTON_1);

//   }
// }

LiquidCrystal_I2C lcd(0x27, 16, 2); // Set the LCD address to 0x27 for a 16 chars and 2 line display

// Replace with your SSID and Password
const char *ssid = "KSP";
const char *password = "9550421866";
// returns -1 if failed, otherwise returns ID #
int getFingerprintIDez()
{
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Waiting For");
    lcd.setCursor(0, 1);
    lcd.print("Finger");
    return -1;
  }

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)
  {
    lcd.clear();
    lcd.print("Messy Image Try Again");
    delay(3000);
    return -1;
  }

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)
  {
    lcd.clear();
    lcd.print("Not Valid Finger");
    Blynk.logEvent("attempt_unsuccessful");
    delay(3000);
    return -1;
  }

  // found a match!
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Box Opend Unlocked");
  lcd.setCursor(0, 1);
  lcd.print("successfully");
  Blynk.logEvent("box_opened");
  digitalWrite(relayPin, HIGH);
  // digitalWrite(buzzerPin, HIGH);
  delay(3000);
  digitalWrite(relayPin, LOW);
  // digitalWrite(buzzerPin, LOW);
  delay(3000);
  return finger.fingerID;
}

void setup()
{
  pinMode(relayPin, OUTPUT);
  pinMode(additionalRelayPin, OUTPUT);
  // pinMode(buzzerPin, OUTPUT);

  digitalWrite(relayPin, LOW);
  digitalWrite(additionalRelayPin, LOW);

  // digitalWrite(buzzerPin, LOW);

  Serial.begin(57600);
  Serial2.begin(115200);

  lcd.init();      // Initialize the LCD
  lcd.backlight(); // Turn on the backlight

  lcd.print("Fingerprint Lock");
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, password);

  finger.begin(57600);

  if (finger.verifyPassword())
  {
    lcd.clear();
    lcd.print("Fingerprint Sensor Connected");
  }
  else
  {
    lcd.clear();
    lcd.print("Sensor Not Found");
    while (1)
    {
      delay(1);
    }
  }
}

void loop()
{
  Blynk.run();
  getFingerprintIDez();
  delay(50); // don't need to run this at full speed.
}

BLYNK_WRITE(V1)
{ // Virtual pin to control the additional relay
  int relayState = param.asInt();
  digitalWrite(additionalRelayPin, relayState);
}