#define PULSE_LENGTH 1000
#define LEMON makeColorRGB(246, 250, 12)


void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:
  //   setColor(makeColorRGB(255, 183, 82));
  //  setColorOnFace(makeColorRGB(246, 250, 12), (millis()/250)%6);

  //get progress from 0 - MAX
  int pulseProgress = millis() % PULSE_LENGTH;

  //transform that progress to a byte (0-255)
  byte pulseMapped = map(pulseProgress, 0, PULSE_LENGTH, 0, 255);

  //transform that byte with sin
  byte dimness = sin8_C(pulseMapped);

  //set color

  if(buttonDown())
  {
    //setColor(dim(LEMON, dimness));
    setColor(LEMON);
  }

}
