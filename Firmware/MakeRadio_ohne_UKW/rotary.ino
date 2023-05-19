void wlan_rotary_loop()
{
  if (rotaryEncoder.encoderChanged())
  {
    uint16_t v = rotaryEncoder.readEncoder();
    //set new currtent station and show its name
    if (v < STATIONS) {
      Stationsnummer = v;
      showText(0,0,String(Stationsnummer+1));
      showText(4,0,stationname[Stationsnummer]);
      lastchange = millis();
    }
  }
 
  //nichts passiert?
  if ((lastchange > 0) && ((millis()-lastchange) > 10000)){
    lastchange = 0;
    Serial.println("zurück");
    Stationsnummer = actStation;
    showText(0,0,String(Stationsnummer+1));
    showText(4,0,stationname[Stationsnummer]);
  }
  
  //Encoder gedrückt
  if (rotaryEncoder.isEncoderButtonClicked())
  {
    actStation = Stationsnummer;
    lastchange = 0;
    /*if (Stationsnummer == STATIONS-1 && mode == "wlan") {
        mode = "ukw";
        Serial.println("Wechsel auf:");
        Serial.print(mode);
        lcd.clear();
        showText(0,0,"Wechsel auf UKW");
        delay(1000);
        UKW_loop();
    }
    else if (Stationsnummer == STATIONS-1 && mode == "ukw") {
        mode = "wlan";
        Serial.println("Wechsel auf:");
        Serial.print(mode);
        lcd.clear();
        showText(0,0,"Wechsel auf WLAN");
        Stationsnummer = 0;
        Serial.print(Stationsnummer);
        audio.connecttohost(stationurl[Stationsnummer]);
    }
    else {*/   
    if (Stationsnummer < STATIONS && mode == "wlan") {
        //mode = "wlan";
        Serial.printf("Active station %s\n",stationname[Stationsnummer]);
        lcd.clear();
        showText(0,0,String(Stationsnummer+1));
        showText(4,0,stationname[Stationsnummer]);
        audio.connecttohost(stationurl[Stationsnummer]);
//    }
  }
 }
}

/*void ukw_rotary_loop()
{
    int16_t encoderDelta = rotaryEncoder.encoderChanged();
    //Encoder im Uhrzeigersinn gedreht
    if (encoderDelta>0) {
      Frequenz = Frequenz + UKW_STEP;
      radio.setFrequency(Frequenz);
      UKW_Frequenz();
      Serial.println(float(radio.getFrequency()/100));
    }

    //Encoder entgegen Uhrzeigersinjn gedreht
    if (encoderDelta<0) {
      Frequenz = Frequenz - UKW_STEP;
      radio.setFrequency(Frequenz);
      UKW_Frequenz();
      Serial.println(float(radio.getFrequency()/100));
    }
    if (Frequenz < UKW_MIN) {
      Frequenz = UKW_MAX;
    }
    if (Frequenz > UKW_MAX) {
      Frequenz = UKW_MIN;
    }

  //Dreh-Encoder gedrückt
  if (rotaryEncoder.isEncoderButtonClicked())
  {
    if (mode == "ukw") {
        mode = "wlan";
        Serial.println("Wechsel auf:");
        Serial.print(mode);
        lcd.clear();
        showText(0,0,"Wechsel auf WLAN");
        delay(500);
        Serial.print("Modus: ");
        Serial.println(mode);
        Stationsnummer = 0;
        rotaryEncoder.reset(Stationsnummer);
        Serial.print(Stationsnummer);
        audio.connecttohost(stationurl[Stationsnummer]);
    }
  }
}*/

//Interrupt-Routine für Encoder
void IRAM_ATTR readEncoderISR()
{
  rotaryEncoder.readEncoder_ISR();
}

void setup_rotary()
{
  rotaryEncoder.begin();
  rotaryEncoder.setup(readEncoderISR);
  rotaryEncoder.setBoundaries(0, STATIONS, true); 
  rotaryEncoder.disableAcceleration();  
}
