Timer pressFadeTimer;
#define PRESS_FADE_DURATION 500
Timer dimTimer;
#define DIM_TIME 7000

void setup() {
  dimTimer.set(DIM_TIME);
}

void loop() {

  int fadeTime = dimTimer.getRemaining();

  byte brightness = map(fadeTime, 0, 7000, 0, 255);

  if (buttonSingleClicked())
  {
    dimTimer.set(DIM_TIME);
  }

  if (dimTimer.isExpired() == true)
  {
    setColor(RED);
  }
  else
  {
    setColor(dim(WHITE, brightness));
  }

}
