// let's make a few patterns
byte pattern1[3] = {1, 2, 3};
byte pattern2[3] = {2, 1, 3};
byte pattern3[3] = {3, 2, 1};

byte id;

byte step = 0;

Timer gameTimer;
Timer stepTimer;

void setup() {
  // put your setup code here, to run once:
  randomize();
  id = 1 + random(2);
}

void loop() {
    
  
  if(buttonDoubleClicked()) {
    // start game
    // tell neighbors let's start
    setValueSentOnAllFaces(1);  // this means start...
    // start our timer
    gameTimer.set(3000);
    step = 0;
    stepTimer.set(1000);
  }
  
  if(stepTimer.isExpired()) {
    step++;
    stepTimer.set(1000);
    
    if(step > 2) {
      step = 0;
    }
  }
  
 
  if(pattern1[step] == id ) {
    // light up when my ID is active
  setColor(WHITE);
  }
  else {
    setColor(OFF);
  }
  
  if(gameTimer.isExpired()) {
    setColor(GREEN);
  }
  
  FOREACH_FACE(f) {
    if(id > f) {
      setColorOnFace(WHITE, f);
    }
  }
  

  // TODO:
  // determine which ID I am
  // I am the master or ID 1
    // tell neighbors when to start
  // Start timer when I hear start
  // determine if it is my turn to light up
}
