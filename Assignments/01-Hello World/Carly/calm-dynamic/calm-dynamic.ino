enum Mode {
   START,
   ONE,
   TWO
};

byte mode = START;

Color autoColors[5] = {OFF, makeColorRGB(255, 0, 128), makeColorRGB(255, 255, 0), makeColorRGB(0, 128, 255), WHITE};

void setup() {

}

void loop() {
  //Dynamic Calm

  switch ( mode ) {
    case START:
      startLoop();
      break;
      
    case ONE:
      oneLoop();
      break;

    case TWO:
      twoLoop();
      break;
   }
}

void startLoop() {
  
  setColor(autoColors[random(3) + 1]);
  
  if(buttonSingleClicked()) {
    mode = ONE;
  }
}

void oneLoop() {

  // pulse info
  #define PULSE_LENGTH 6000
  int pulsetime = millis() % PULSE_LENGTH;
  byte pulsemap = map(pulsetime, 0, PULSE_LENGTH, 0, 255);
  byte dimness = sin8_C(pulsemap);

  // 3 pink faces pulse
  setColorOnFace(makeColorRGB(0,0,0),0);
  setColorOnFace(dim(makeColorRGB(255,115,241), dimness),1);
  setColorOnFace(makeColorRGB(0,0,0),2);
  setColorOnFace(dim(makeColorRGB(255,115,241), dimness),3);
  setColorOnFace(makeColorRGB(0,0,0),4);
  setColorOnFace(dim(makeColorRGB(255,115,241), dimness),5);

  if(buttonSingleClicked()) {
    mode = TWO;
   }
}

void twoLoop() {

  // pulse info
  #define PULSE_LENGTH 6000
  int pulsetime = millis() % PULSE_LENGTH;
  byte pulsemap = map(pulsetime, 0, PULSE_LENGTH, 0, 255);
  byte dimness = sin8_C(pulsemap);
  
  // blue pulses on 3 faces
  setColorOnFace(dim(makeColorRGB(38,204,255), dimness),0);
  setColorOnFace(makeColorRGB(0,0,0),1);
  setColorOnFace(dim(makeColorRGB(38,204,255), dimness),2);
  setColorOnFace(makeColorRGB(0,0,0),3);
  setColorOnFace(dim(makeColorRGB(38,204,255), dimness),4);
  setColorOnFace(makeColorRGB(0,0,0),5);
    
  if(buttonDoubleClicked()) {
    mode = START;
  }
}
