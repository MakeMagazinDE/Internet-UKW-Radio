/*
  verwendet ESP32-audioI2S Library - https://github.com/schreibfaul1/ESP32-audioI2S
*/
 
// Bibliotheken einbinden
#include "Arduino.h"
#include "WiFi.h"
#include "Audio.h"
#include <LiquidCrystal_I2C.h>
#include <SI4703.h>
#include <RDSParser.h>
#include <radio.h> /// Source Code auf https://github.com/mathertel/Radio
#include "AiEsp32RotaryEncoder.h"

#define RESET_PIN 18
#define MODE_PIN 21 
#define RELAIS_PIN 16
#define UKW_MIN 8750
#define UKW_MAX 10800
#define UKW_STEP 10
#define STATIONS 25
#define ROTARY_ENCODER_A_PIN 33
#define ROTARY_ENCODER_B_PIN 32
#define ROTARY_ENCODER_BUTTON_PIN 34
#define ROTARY_ENCODER_VCC_PIN -1 
#define ROTARY_ENCODER_STEPS 4

//Instanzen einrichten
//SI4703 radio;  
//RDSParser rds;
LiquidCrystal_I2C lcd(0x3f,20,4);
Audio audio(true, I2S_DAC_CHANNEL_BOTH_EN);
AiEsp32RotaryEncoder rotaryEncoder = AiEsp32RotaryEncoder(ROTARY_ENCODER_A_PIN, ROTARY_ENCODER_B_PIN, ROTARY_ENCODER_BUTTON_PIN, ROTARY_ENCODER_VCC_PIN, ROTARY_ENCODER_STEPS);

String ssid =    "Ihr WLAN-Name";
String password = "Ihr WLAN-Passwort";
String ssid2 = "Name des 2. WLANS";
String password2 = "Passwort des 2. Wlans";
String mode = "wlan";
uint8_t Stationsnummer = 0;
uint32_t lastchange = 0;
uint8_t actStation = Stationsnummer;
uint32_t Frequenz = UKW_MIN;
const char* stationurl[STATIONS];
String stationname[STATIONS];

void setup() {
  Serial.begin(115200);
  setup_rotary();
  setup_senderList();
//  UKW_setup();
  pinMode(RELAIS_PIN, OUTPUT);
  digitalWrite(RELAIS_PIN, LOW);
  lcd.init();
  lcd.backlight();
  //Station mode
  WiFi.disconnect();
  WiFi.mode(WIFI_STA);
  //Verbindung zu Netz1
  WiFi.begin(ssid.c_str(), password.c_str());
    showText(0,0,"Suche WLAN ");
    showText(0,1,ssid);
    delay(5000);
    if (WiFi.status() == WL_CONNECTED) {
      showText(0,0,"WLAN-Verbindung mit ");
      showText(0,1,ssid);
    }
   //Falls Netz1 nicht da ist, Versuch mit Netz2
   if (WiFi.status() != WL_CONNECTED) {
      lcd.clear();
      showText(0,0,"Suche WLAN");
      showText(0,1,ssid2);
      WiFi.disconnect();
      WiFi.begin(ssid2.c_str(), password2.c_str());
      while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
      }
      Serial.println("WLAN verbunden");
      showText(0,0,"WLAN-Verbindung mit ");
      showText(0,1,ssid2);
   }
  
  Serial.println("IP-Adresse: ");
  Serial.println(WiFi.localIP());
  delay(2000);
  lcd.clear();
  showText(0,0,"Radio gestartet");
  delay(500);
  audio.setVolume(100);
  audio.connecttohost(stationurl[Stationsnummer]);
}
 
void loop()
{
  // Run audio player
  if (mode == "wlan") {
    audio.loop();
    wlan_rotary_loop();
  }
/*  if (mode == "ukw") {
    ukw_rotary_loop();
  }*/
}
 
// Audio status functions
void audio_showstation(const char *info) {
    if (mode == "wlan") {
    Serial.print("station     "); 
    Serial.println(info);
    showText(0,0,"                    ");
    String Text = info;
    showText(0,0,String(Stationsnummer+1));
    showText(4,0,stationname[Stationsnummer]);
  }
}
void audio_showstreamtitle(const char *info) {
  if (mode == "wlan") {
    lcd.setCursor(0,1);
    lcd.print("                    ");
    lcd.setCursor(0,2);
    lcd.print("                    ");
    lcd.setCursor(0,3);
    lcd.print("                    ");
    Serial.print("Sendername "); 
    Serial.println(info);
    String Text = info;
    lcd.setCursor(0,1);
    lcd.setCursor(0,1);
    lcd.print(Text.substring(0,20));
    lcd.setCursor(0,2);
    lcd.print(Text.substring(20,40));
    lcd.setCursor(0,3);
    lcd.print(Text.substring(40,60));
  }
}
void showText(int Spalte, int Zeile, String Text) {
    lcd.setCursor(Spalte,Zeile);
    lcd.print("                    ");
    lcd.setCursor(Spalte,Zeile);
    lcd.print(Text);
}
