#include <Homie.h>
 
const int PIN_BELL = A0;
unsigned long lastNotify = 0;
unsigned long intervalNotification = 8000; // millisecs
bool sendMessage = false;
// bool lastState = false; // false silence // true ring

HomieNode bellNode("bell", "Bell", "bell");

void loopHandler() {
  int bellValue = analogRead(A0);
  //Serial.println(bellValue);

  if(bellValue < 1000) {
    sendMessage = true;
    if ((lastNotify + intervalNotification) < millis()) {
      lastNotify = millis();
      Homie.getLogger() << "Ring is now ringing" << endl;
      bellNode.setProperty("ring").send("true");
    }
  } else if(sendMessage) {
    if ((lastNotify + intervalNotification) < millis()) {
      lastNotify = millis();
      Homie.getLogger() << "Ring is now in silence" << endl;
      bellNode.setProperty("ring").send("false");
      sendMessage = false;
    }
  }
}

void setup() {
  Serial.begin(115200);
  Serial << endl << endl;
  delay(intervalNotification);
 
  Homie_setFirmware("bell-detector", "1.0.0");
  Homie.setLoopFunction(loopHandler);
 
  bellNode.advertise("ring").setName("isRinging").setDatatype("boolean");
 
  Homie.setup();
}

void loop() {
  Homie.loop();
  delay(100);
}