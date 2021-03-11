enum HandValues {
  ROCK,  // 0
  PAPER, // 1
  SCISSORS, // 2
};

byte myHandValue = ROCK;

Color handColors[3] = {YELLOW, BLUE, MAGENTA};


void setup() {
  // put your setup code here, to run once:
randomize();
}

void loop() {
  // ROCK, PAPER, SCISSORS

//ROCK is yellow
if (buttonSingleClicked()){
myHandValue = 1
    
  ;setColor(YELLOW);
}
//PAPER is blue
if (buttonDoubleClicked()){
myHandValue = 2
  
  ;setColor(BLUE);
}
//SCISSORS
if (buttonMultiClicked()){
myHandValue = 3
 
  ;setColor(MAGENTA);
}

}
