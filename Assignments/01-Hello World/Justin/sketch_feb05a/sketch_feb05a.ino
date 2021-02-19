#define CRIMSON makeColorRGB(155, 0, 2);
#define STRONG_RED makeColorRGB(198, 1, 23);
#define DARK_RED makeColorRGB(155, 18, 23);




void setup() {
  // put your setup code here, to run once:
  setColor(CRIMSON);
  faceColor = random(1,2);
  
    FOREACH_FACE(f){
      if(faceColor == 1){
        setColor(STRONG_RED);
      }
      else if(faceColor == 2){
        setColor(DARK_RED);
      }
    }
}

void loop() {
  // put your main code here, to run repeatedly:
  
}
