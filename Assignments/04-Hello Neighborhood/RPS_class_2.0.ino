
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
  
  // TODO: share our choice with neighbor(s)
  // TODO: listen to neighbor(s) to respond to their choices
    
  // Display our choice
  setColor(handColors[myHandValue]);

  if(myHandValue = ROCK;) and(neighborValue = SCISSORS;)
    setColor(GREEN)
    else setColor(RED)
  

  
  // TODO: display the result of a matchup when being matched up
}
