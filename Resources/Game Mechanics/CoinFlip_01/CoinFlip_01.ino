/*
 * Coin Flip Example
 */

// 0 is going to represent HEADS
// 1 is going to represent TAILS

byte myCoinValue = 0; // HEADS

Color coinColors[2] = {YELLOW, WHITE};  // an array of colors where YELLOW is in the 0 index = HEADS and WHITE is in the 1 index = TAILS

void setup() {
  // put your setup code here, to run once:
  randomize();  // Seed random so that it is different on each Blink
}

void loop() {
  // put your main code here, to run repeatedly:

  // on button pressed, flip the coin to choose a random value
  if(buttonPressed()) {
    myCoinValue = random(1); // chooses a value 0 or 1 (HEADS or TAILS) randomly
  }

  // share my coin value with neighbors (not used for anything yet, but could be)
  setValueSentOnAllFaces(myCoinValue);

  // display my coin value with a color
  setColor(coinColors[myCoinValue]);  
}
