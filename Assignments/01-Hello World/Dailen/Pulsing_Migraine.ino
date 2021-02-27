//headache

void setup() {
  // put your setup code here, to run once:
 #define DEEPPURPLE makeColorRGB (174, 0, 255)
  setColor(DEEPPURPLE);
  setColorOnFace(WHITE, 0);
#define PULSE_LENGTH 2000
#define TIMER_LENGTH 6000
Timer redTimer;
#define LIME makeColorRGB(124, 252, 0 )


}
void loop() {
  // put your main code here, to run repeatedly:
 bool longPressing = false;
  
  if (buttonLongPressed()) {
    longPressing = true;
  }
 
  if (buttonDown()) {
    byte dimness = millis() % 256;
  setColor(dim(ORANGE, dimness));
  if (longPressing) {
    byte dimness = millis() % 256;
  setColor(dim(LIME, dimness));
    
  }
  } else {
    //get progress from 0 - MAX
  int pulseProgress = millis() % PULSE_LENGTH;

  //transform that progress to a byte (0-255)
  byte pulseMapped = map(pulseProgress, 0, PULSE_LENGTH, 0, 255);

  //transform that byte with sin
  byte dimness = sin8_C(pulseMapped);

  //set color
  setColor(dim(GREEN, dimness));
  }
}
