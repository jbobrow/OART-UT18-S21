/*  --------------------
    03-Hue
    by Jonathan Bobrow
    Move38, Inc. 2021
    --------------------
*/

void setup() { 
}

void loop() {

  byte hue = millis() / 20; // shift the hue over time (dividing millis() by 20 results in going from 0-255 in ~5 seconds)
  
  setColor( makeColorHSB(hue, 255, 255) ); // display a color with a hue, saturation, and brightness
}
