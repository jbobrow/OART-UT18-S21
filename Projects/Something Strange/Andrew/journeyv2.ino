byte faceBri[6];

#define DECREMENT 5
#define FADE_INTERVAL 20
#define VICTORYBLUE makeColorRGB(0, 180, 171)
#define PULSE_LENGTH 2000

Color colors[2] = {GREEN, RED};
byte myColorIndex;
byte index = 0;
byte prev_index = 0;
Timer fadeTimer;
byte saved;

enum Mode {
  START,  // = 0
  GOAL,  // = 1
  INPUT //=2
};
byte mode;
byte counter = 2;


void setup() {
  // put your setup code here, to run once:
  randomize();
  mode = START;
}

void loop() {

  switch ( mode ) {

    case START: //beforre gamme starts
      startLoop();
      break;

    case GOAL: //set the goal target
      goalLoop();
      break;

    case INPUT: //where player uses the compasss
      inputLoop();
      break;
  }
}

void startLoop()
{
  //myColorIndex = random(1);

  if (buttonMultiClicked())
  {
    //designate the target
    mode = GOAL;
  }

  if (buttonSingleClicked())
  {
    setColor(colors[random(1)]); //selects. a. random buff blinks
  }
  if (buttonLongPressed()) //RESETS  THE GAME
  {
    setColor(OFF);
    mode = START;
  }
  if (buttonDoubleClicked() )
  {
    mode = INPUT; //starts the game
    setValueSentOnAllFaces(1); //ACTIVATES ITS NEIGHBOR
  }

  FOREACH_FACE(f)
  {
    if (didValueOnFaceChange(f)) //THE NEIGHBOR BEING ACTIVATED
    {
      mode = INPUT;
    }
  }
}

void goalLoop()
{
  int pulseProgress = millis() % PULSE_LENGTH;
  byte pulseMapped = map(pulseProgress, 0, PULSE_LENGTH, 0, 255);

  if (isAlone()) //PLAYS ANIMATION
  {
    byte dimness = sin8_C(pulseMapped);
    setColor(dim(MAGENTA, dimness));
  }
  if (!isAlone())
  {
    //win anim here
    setColorOnFace(VICTORYBLUE, (millis() / 300) % 6);
  }
  if (buttonLongPressed()) //RESETS  THE GAME
  {
    setColor(OFF);
    mode = START;
  }
}

void inputLoop()
{
  if (buttonSingleClicked() || buttonDoubleClicked()) {
    setValueSentOnAllFaces(counter);
    counter++; //NEW NUMBER EACH CLICK
  }
  if (counter > 3) //keeps count in order
  {
    counter = 2;
  }
  FOREACH_FACE(f)
  { //PLAYS ANIM ON NEIGHBOR
    if (didValueOnFaceChange(f))
    {
      saved = f;
      while (index == prev_index)
      {
        index = random(5);
      }

      faceBri[index] = 255;
      prev_index = index;
    }
  }
  if (index == saved)
  {
    index = random(5);
  }

  if (fadeTimer.isExpired()) {

    FOREACH_FACE(f) {
      if (faceBri[f] > DECREMENT) {
        faceBri[f] -= DECREMENT;
      }
    }
    fadeTimer.set(FADE_INTERVAL);
  }
  if (buttonLongPressed()) //RESETS  THE GAME
  {
    setColor(OFF);
    mode = START;
  }

  // display our bri
  FOREACH_FACE(f) {
    setColorOnFace(OFF, saved); //TURNS OFF THE 180 DEGREE FACE FROM COLOR
    setColorOnFace(dim(ORANGE, faceBri[f]), f); //THE COLOR OF THE COMPASS
  }

}
