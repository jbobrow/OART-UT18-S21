/*  --------------------
    04-FaceOffset
    by Jonathan Bobrow
    Move38, Inc. 2021
    --------------------
*/

void setup() { 
}

void loop() {

  for( byte i=0; i<6; i++) {
   
    setColorOnFace( makeColorHSB(i * 42, 255, 255), i );  // display color on each face and offset the hue
  
  }

}
