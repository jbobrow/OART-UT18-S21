#include <util/delay.h>
#define CRYSTAL makeColorRGB(7,13,13)
byte randColor;
byte clicks = 1;
void setup() {
  // put your setup code here, to run once:
  randomize();

}

void loop() {
  // put your main code here, to run repeatedly:
  randColor = random(250);
 setColorOnFace(WHITE,0);
   setColorOnFace(WHITE,2);
   setColorOnFace(WHITE,4);
   setColorOnFace(CRYSTAL,1);
   setColorOnFace(CRYSTAL,3);
   setColorOnFace(CRYSTAL,5);; 
   if (buttonSingleClicked()) {
    clicks++ ;
 

  }  if(clicks < 5 && clicks > 2){
  setColor(makeColorRGB(randColor,randColor,randColor)); 
  }

  if (clicks == 5){
  setColor(RED);}





}
