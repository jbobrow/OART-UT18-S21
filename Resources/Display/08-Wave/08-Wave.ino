/*  --------------------
    08-Wave
    by Jonathan Bobrow
    Move38, Inc. 2021
    --------------------
*/

void setup() { 
}

void loop() {
  
  byte value = millis() / 20; // change this value from 0-255 over 5 seconds

  FOREACH_FACE(f) {

    byte phase = 42 * f;  // offset the sine wave for each face
    
    byte bri = sin8_C(phase + value);  // create a sinusoidal brightness based on the current value and the phase offset
    
    setColorOnFace(dim(WHITE,bri), f);  // display on each face
    
  }

}
