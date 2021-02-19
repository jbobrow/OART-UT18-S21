/*
 *  Some notes for creating a pattern matching game
 */

// create names for 
enum gameInputs {
  SINGLE, // 0
  DOUBLE, // 1
  TRIPLE, // 2
  END     // 3
};

#define MAX_PATTERN_LENGTH 10
// a place to store the pattern
byte pattern[MAX_PATTERN_LENGTH];

// we need an index to show the player's progress

void setup() {
  // put your setup code here, to run once:
  randomize();
  // create our first pattern
  createPatternOfLength(4);
}

void loop() {
  // put your main code here, to run repeatedly:

}

// create random patterns of a specific length
void createPatternOfLength(byte n) {

  // loop through each of the indices of the pattern
    
    // create a random number
    byte randomNumber = random(3);  // a random number (0-2)
    
    // store random values in the pattern 
    pattern[index] = randomNumber;  // this index needs to increment for each item we add

    // place END in the nth location
}
