#define SUNSET_PURPLE makeColorRGB(52,47,87)
#define SUNSET_LILAC makeColorRGB(88,71,113)
#define SUNSET_PURPLE2 makeColorRGB(130,87,117)
//#define SUNSET_PEACH makeColorRGB(209,119,118)
//#define SUNSET_YELLOW makeColorRGB(243,157,108)
#define SUNSET_ORANGE makeColorRGB(234,98,72)

bool dynamic;
int brightness;
Timer wait;

void setup() {
  // put your setup code here, to run once:
  dynamic = false;
  brightness = 255;
}

void loop() {
  // put your main code here, to run repeatedly:
  /*if (buttonLongPressed() && !dynamic) {
    dynamic = true; //want to switch between code for the dynamic and static assignment requirements
  }
  else {
    dynamic = false;
  }*/
  if (!dynamic) {
    //IMPLEMENTATION FOR STATIC PORTION OF ASSIGNMENT
    setColorOnFace(SUNSET_PURPLE, 0);
    setColorOnFace(SUNSET_ORANGE, 1);
    setColorOnFace(SUNSET_LILAC, 2);
    setColorOnFace(YELLOW, 3);
    setColorOnFace(SUNSET_PURPLE2, 4);
    setColorOnFace(ORANGE, 5);
  }
  else {
    //IMPLEMENTATION FOR DYNAMIC PORTION OF ASSIGNMENT
    if (brightness >= 0) {
      if (wait.isExpired()) {
        setColorOnFace(dim(SUNSET_PURPLE, brightness), 0);
        setColorOnFace(dim(SUNSET_ORANGE, brightness), 1);
        setColorOnFace(dim(SUNSET_LILAC, brightness), 2);
        setColorOnFace(dim(YELLOW, brightness), 3);
        setColorOnFace(dim(SUNSET_PURPLE2, brightness), 4);
        setColorOnFace(dim(ORANGE, brightness), 5);
        brightness--;

        wait.set(50);
      }
    }
    else {
      setColorOnFace(WHITE, 0); // the sun has set and the moon has risen
    }
    if (buttonDoubleClicked()){
      brightness = 255; // on double click reset the brightness to view the sunset again
    }
  }

  if (buttonLongPressed()) {
    if (!dynamic){
      dynamic = true; //want to switch between code for the dynamic and static assignment requirements
      //brightness = 255;
    }
    else {
      dynamic = false;
    } 
  } 
}
