
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
  if (isValueReceivedOnFaceExpired(5))
{
  setColorOnFace(BLUE, 5);
}
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

  //Sets the value sent on face 0 to 1 if ROCK is true.
if (myHandValue = ROCK == true)
{
  setValueSentOnFace(1, 0);
}   
{ 
    if (getLastValueReceivedOnFace(0) == 3)
  setColor(GREEN);
  else setColor(RED);}

  //Sets the value sent on face 0 to 2 if PAPER is true.
if (myHandValue = PAPER == true)
{
  setValueSentOnFace(2, 0);
}    
{
    if (getLastValueReceivedOnFace(0) == 1)
  setColor(GREEN);
  else setColor(RED);


  //Sets the value sent on face 0 to 3 if Scissors is true.
if (myHandValue = SCISSORS == true)
{
  setValueSentOnFace(3, 0);
}    
{
    if (getLastValueReceivedOnFace(0) == 1)
  setColor(GREEN);
  else setColor(RED);}
}
}
