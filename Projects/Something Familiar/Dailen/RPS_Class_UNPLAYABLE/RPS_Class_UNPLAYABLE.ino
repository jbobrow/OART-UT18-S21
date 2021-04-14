/*
 * RPS Work In Progress
 */
enum HandValues {
  ROCK,  // 0
  PAPER, // 1
  SCISSORS, // 2
};

byte myHandValue = ROCK;

Color handColors[3] = {
  YELLOW,   // ROCK
  BLUE,     // PAPER
  MAGENTA   // SCISSORS
};


void setup() {
  // put your setup code here, to run once:
  randomize();
}

void loop() {
  // ROCK, PAPER, SCISSORS

  // User inputs their choice using different clicks
  // single click for rock
  if (buttonSingleClicked()) {
    myHandValue = ROCK;
  }
  // double click for paper
  if (buttonDoubleClicked()) {
    myHandValue = PAPER;
  }
  // more than 2 clicks for scissors
  if (buttonMultiClicked()) {
    myHandValue = SCISSORS;
  }


  // Display our choice
  setColor(handColors[myHandValue]);

  //Sets the value sent on face 0 to ROCK
  if (myHandValue == ROCK)
  {
    setValueSentOnFace(ROCK, 0);
    
    if (!isAlone()) { // check if we are a winner only if we are connected to another
      
      if (getLastValueReceivedOnFace(0) == SCISSORS) {
        setColor(GREEN);
      }
      else {
        setColor(RED);
      }
    }
  }

  //Sets the value sent on face 0 to PAPER.
  if (myHandValue == PAPER)
  {
    setValueSentOnFace(PAPER, 0);

    if (!isAlone()) { // check if we are a winner only if we are connected to another

      if (getLastValueReceivedOnFace(0) == ROCK) {
        setColor(GREEN);
      }
      else {
        setColor(RED);
      }
    }
  }


  //Sets the value sent on face 0 to Scissors is true.
  if (myHandValue == SCISSORS)
  {
    setValueSentOnFace(SCISSORS, 0);

    if (!isAlone()) { // check if we are a winner only if we are connected to another
      
      if (getLastValueReceivedOnFace(0) == PAPER) {

        setColor(GREEN);
      }
      else {
        setColor(RED);
      }
    }
  }

  // show the face to connect on
  if (isValueReceivedOnFaceExpired(0))
  {
    setColorOnFace(WHITE, 0);
  }

}
