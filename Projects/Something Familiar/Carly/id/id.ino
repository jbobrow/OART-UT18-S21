//all im trying to do is assign ids to each blink
//there are 10 blinks all set up in one line and the user will have to single click 
//on one of the blinks on either end of the line and then the blinks will listen to 
//each other and take an id from the array of ids
//the display colors are just to test and show the blinks id 

 /* 
 single click: set self to index 0, tell neighbor my index, they set themselves to next index, they tell next neighbor & so on...
 display color based on index of id
*/
//what jonathan told me to do ^^ ???? but like how???

byte id [10] = {1, 1, 2, 2, 3, 3, 4, 4, 5, 5};

Color PURPLE = makeColorRGB(180, 74, 255);

void setup() {
  // put your setup code here, to run once:
   setValueSentOnAllFaces(0);

}

void loop() { 

// this is all the shit that i can't figure out :), so essentially all of it and none of it works
   FOREACH_FACE(f) {
     if (buttonSingleClicked() ) {
       id[0];
       setValueSentOnAllFaces(1); 
      }
    if (!isValueReceivedOnFaceExpired(f)) {
      if (getLastValueReceivedOnFace(f) == 1) {
        id[1];
        setValueSentOnAllFaces(2);
       }
       if (getLastValueReceivedOnFace(f) == 2) {
        id[2];
        setValueSentOnAllFaces(3);
       }
       if (getLastValueReceivedOnFace(f) == 3) {
        id[3];
        setValueSentOnAllFaces(4);
       }
       if (getLastValueReceivedOnFace(f) == 4) {
        id[4];
        setValueSentOnAllFaces(5);
       }
       if (getLastValueReceivedOnFace(f) == 5) {
        id[5];
        setValueSentOnAllFaces(6);
       }
       if (getLastValueReceivedOnFace(f) == 6) {
        id[6];
        setValueSentOnAllFaces(7);
       }
       if (getLastValueReceivedOnFace(f) == 7) {
        id[7];
        setValueSentOnAllFaces(8);
       }
       if (getLastValueReceivedOnFace(f) == 8) {
        id[8];
        setValueSentOnAllFaces(9);
       }
       if (getLastValueReceivedOnFace(f) == 9) {
        id[9];
        setValueSentOnAllFaces(10);
       }
     }
   }
  

   //display colors 
   FOREACH_FACE(f) { 
      byte len = sizeof(id)/sizeof(id[0]);
      for (byte i = 0 ; i < len ; i++) {
        if (id[i] = 1) {
          setColor(MAGENTA);
        } else if (id[i] = 2) {
          setColor(BLUE);
        } else if (id[i] = 3) {
          setColor(GREEN);
        } else if (id[i] = 4) {
          setColor(YELLOW);
        } else if (id[i] = 5) {
          setColor(PURPLE);
        }
      } 
    }
}
