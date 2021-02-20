Timer countDown;
#define DURATION 10000

void setup() {
  // put your setup code here, to run once:
}

void loop() {
  // put your main code here, to run repeatedly:

  if (buttonPressed()) {
    countDown.set(DURATION);
  }

  uint16_t countUp = DURATION - countDown.getRemaining();

  byte bri = map(countUp, 0, DURATION, 0, 255);
  setColor(dim(MAGENTA,bri));
}
