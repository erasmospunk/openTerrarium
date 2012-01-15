static unsigned long lastZc;
static boolean triacFired;

void initDimmer() {
  pinMode(TRIAC_PIN, OUTPUT);
  digitalWrite(TRIAC_PIN, LOW);
  attachInterrupt(ZC_INT, zc, FALLING);
}

void dimmerControl(int power) {
  if (triacFired == false && (long)(micros() - lastZc) > power) {
      digitalWrite(TRIAC_PIN, HIGH);
      delayMicroseconds(50);
      digitalWrite(TRIAC_PIN, LOW);
      triacFired = true;
  }
}

void zc() {
  lastZc = micros() + 600;
  triacFired = false;
}
