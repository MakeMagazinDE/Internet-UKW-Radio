/*void UKW_Frequenz() {
  char s[12];
  radio.formatFrequency(s, sizeof(s));
//  Serial.print("FREQ:");
  showText(4,0,s);
}

void RDSAnzeige(const char *name) {
  Serial.print("RDS:");
  Serial.println(name);
  showText(0,1,name);
}

void RDS_process(uint16_t block1, uint16_t block2, uint16_t block3, uint16_t block4) {
  rds.processData(block1, block2, block3, block4);
}

void UKW_setup() {
  Serial.println("UKW-Setup");
  radio.setup(RADIO_RESETPIN, RESET_PIN);
  radio.setup(RADIO_MODEPIN, MODE_PIN);
  radio.setup(RADIO_FMSPACING, RADIO_FMSPACING_100);   // for EUROPE
  radio.setup(RADIO_DEEMPHASIS, RADIO_DEEMPHASIS_50);  // for EUROPE
  if (!radio.initWire(Wire)) {
    Serial.println("UKW-Tuner nicht gefunden!");
    delay(4000);
    while (1) {};
  };
  radio.setBandFrequency(RADIO_BAND_FM, UKW_MIN);
  radio.setMono(false);
  radio.setMute(false);
  radio.setVolume(radio.getMaxVolume());
  radio.attachReceiveRDS(RDS_process);
  rds.attachServiceNameCallback(RDSAnzeige);
  Serial.println("UKW-Tuner ok!");
} 

void UKW_loop() {
  digitalWrite(RELAIS_PIN, HIGH);
  Serial.println("UKW-Radio startet");
  showText(0,0,"UKW ");
  UKW_Frequenz();
  while (mode == "ukw") {
    unsigned long now = millis();
    static unsigned long nextFreqTime = 0;
    radio.checkRDS();
    ukw_rotary_loop();
  }
  digitalWrite(RELAIS_PIN, LOW);
  Serial.println("UKW-Radio beendet");
}*/
