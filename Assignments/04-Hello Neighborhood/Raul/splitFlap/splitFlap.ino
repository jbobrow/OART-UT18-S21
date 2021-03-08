Color flapColors[6] = {RED, GREEN, ORANGE, BLUE, MAGENTA, CYAN};
byte landing;
enum signalTypes {INERT, GO, RESOLVE};
byte pushSignal = INERT;
byte goSignal = INERT;
enum widgets {DICE, SPINNER, COIN, TIMER};
byte currentWidget = COIN;
Timer animTimer;
byte framesRemaining = 0;
byte currentOutcome = 1;
bool bChange = false;
void pusLoop();
byte landingColor;
bool spinning = false;

//inert loop


#define COIN_FLIP_INTERVAL 150




void setup()
{
  randomize();
  startWidget();
}

void loop()
{
  FOREACH_FACE(f)
  {
    if (getLastValueReceivedOnFace(f) == 1 && spinning != true)
    {
      startWidget();
      if (didValueOnFaceChange(f))
      {
        setValueSentOnAllFaces(1);
      }
    }
  }

  //listen for button clicks
  if (buttonSingleClicked())
  {
    startWidget();
    setValueSentOnAllFaces(1);
  }
  coinLoop();
}

void startWidget()
{
  framesRemaining = random(6) + 22;
  if (animTimer.isExpired())
  { //reset the timer if it isn't currently going
    animTimer.set(COIN_FLIP_INTERVAL);
    currentOutcome = (currentOutcome + 1) % 6;
  }
  goSignal = GO;
}

void coinLoop() {
  if (animTimer.isExpired() && framesRemaining > 0) {
    framesRemaining--;
    animTimer.set(COIN_FLIP_INTERVAL);
    //change the color
    currentOutcome = (currentOutcome + 1) % 6;
  }

  if (framesRemaining == 0) {
    coinDisplay(true);
    spinning = false;
  } else {
    coinDisplay(false);
    spinning = true;
  }
}

void coinDisplay(bool finalFlip)
{
  Color faceColor;
  faceColor = flapColors[currentOutcome];

  byte animPosition = COIN_FLIP_INTERVAL - animTimer.getRemaining();
  byte leftStart = 0;
  byte centerStart = COIN_FLIP_INTERVAL / 6;
  byte rightStart = COIN_FLIP_INTERVAL / 3;
  byte edgeDuration = (COIN_FLIP_INTERVAL / 3) * 2;


  setColor(OFF);

  if (animPosition >= leftStart && animPosition <= leftStart + edgeDuration) {
    byte brightness = sin8_C(map(animPosition, leftStart, leftStart + edgeDuration, 0, 255));
    setColorOnFace(dim(faceColor, brightness), 0);
    setColorOnFace(dim(faceColor, brightness), 1);
  }

  if (finalFlip && animPosition >= leftStart + (edgeDuration / 2)) {
    setColorOnFace(faceColor, 0);
    setColorOnFace(faceColor, 1);
  }

  if (animPosition >= centerStart && animPosition <= centerStart + edgeDuration) {
    byte brightness = sin8_C(map(animPosition, centerStart, centerStart + edgeDuration, 0, 255));
    setColorOnFace(dim(faceColor, brightness), 2);
    setColorOnFace(dim(faceColor, brightness), 5);
  }

  if (finalFlip && animPosition >= centerStart + (edgeDuration / 2)) {
    setColorOnFace(faceColor, 2);
    setColorOnFace(faceColor, 5);
  }

  if (animPosition >= rightStart && animPosition <= rightStart + edgeDuration) {
    byte brightness = sin8_C(map(animPosition, rightStart, rightStart + edgeDuration, 0, 255));
    setColorOnFace(dim(faceColor, brightness), 3);
    setColorOnFace(dim(faceColor, brightness), 4);
  }

  if (finalFlip && animPosition >= rightStart + (edgeDuration / 2)) {
    setColorOnFace(faceColor, 3);
    setColorOnFace(faceColor, 4);
  }

}
