/*  --------------------
    02-Pulse
    by Jonathan Bobrow
    Move38, Inc. 2021
    --------------------
*/

byte brightness = 255;

void setup() { 
}

void loop() {
  
  brightness--; // subtract 1 from brightness (because it is a byte, 0 - 1 = 255, cyclical)
  
  Color c = dim(RED, brightness); // create a color by dimming a defined color
  
  setColor(c);  // display the color
}
