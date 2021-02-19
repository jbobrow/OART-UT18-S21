void setup() {
  // put your setup code here, to run once:
  
}

void loop() {
  // put your main code here, to run repeatedly:
  
  // Static Calm

  // pulse time & dimness 
  #define PULSE_LENGTH 6000
  int pulsetime = millis() % PULSE_LENGTH;
  byte pulsemap = map(pulsetime, 0, PULSE_LENGTH, 0, 255);
  byte dimness = sin8_C(pulsemap);

  //pulses on faces
  setColorOnFace(dim(makeColorRGB(38,204,255), dimness),0);
  setColorOnFace(dim(makeColorRGB(255,115,241), dimness),1);
  setColorOnFace(dim(makeColorRGB(38,204,255), dimness),2);
  setColorOnFace(dim(makeColorRGB(255,115,241), dimness),3);
  setColorOnFace(dim(makeColorRGB(38,204,255), dimness),4);
  setColorOnFace(dim(makeColorRGB(255,115,241), dimness),5);
}
