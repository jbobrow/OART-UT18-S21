enum signalStates {BALL, PLAY, RESOLVE}; //CODE TO SET UNIQUE COLORS
byte signalState = BALL;

bool ball = false; 
bool hole = false;
#define length 1000
Timer test;
 bool check = false; 
bool win = false; 

void setup() {
   //setColor(WHITE);
}

void loop() {
  switch (signalState) {
    case BALL:
      ballLoop();
      break;
    case PLAY:
      playLoop();
      break;
    case RESOLVE:
      resolveLoop();
      break;
  }

  displaySignalState();

}

void displaySignalState() {
  switch (signalState) {
    case BALL: //SET WHERE BALL + HOLE IS
      //setColor(CYAN);
      break;
    case PLAY:
      //setColor(RED);
      break;
    case RESOLVE:
     // setColor(YELLOW);
      break;
  }
}

void ballLoop() {
 
  if (buttonSingleClicked()) //SELECT WHICH BLINK IS BALL
  {
    
    ball = true;
    setColor(WHITE); 
  }
  if(buttonDoubleClicked() ) //SELECT WHICH BLINK IS HOLE
  {
    hole = true; 
    setColor(RED); 
    signalState = PLAY; //GO TO THE NEXT STATE
    setValueSentOnAllFaces(signalState);
  }

  FOREACH_FACE(f) {
    if (!isValueReceivedOnFaceExpired(f)) {//a neighbor!
      if (getLastValueReceivedOnFace(f) == PLAY) {//a neighbor saying PLAY
        signalState = PLAY;
        setValueSentOnAllFaces(signalState);
      }
    }
  }
    if(ball == false && hole == false) //SETS EVERYTHING THAT AINT HOLE OR BALL TO GREEN
  {
    setColor(GREEN);  
  } 
}

void playLoop() //PUT CODE TO PLAY THE GAME HERE!!
{
 if(buttonSingleClicked() && ball == true)
  {
   setValueSentOnAllFaces(2);
   test.set(length);
  }
  
  FOREACH_FACE(f) 
  {
    if (!isValueReceivedOnFaceExpired(f)) 
    {//a neighbor!
      if (getLastValueReceivedOnFace(f) == BALL) 
      {//This neighbor doesn't know it's PLAY time. Stay in PLAY
        signalState = PLAY;
        setValueSentOnAllFaces(signalState);
      }
     }
    
    if (getLastValueReceivedOnFace(f) == 2 && check == false) //everyone checks this
    {
      test.set(length);
      setColor(WHITE); 
      setValueSentOnAllFaces(2); //makes it so it goes the entire way
      check = true;
    }
    
     if(test.isExpired() && check == true)
     {
        setColor(GREEN); 
    }
    
     if(test.isExpired() && check == true && hole == true)
     {
         setValueSentOnAllFaces(1); 
         setColor(CYAN); 
    } 
    
    if (test.isExpired() && hole == true && check == true) //everyone checks this
    {
      win = true;
      signalState = RESOLVE; //GO TO THE NEXT STATE
      setValueSentOnAllFaces(signalState);
    }  
    else if (test.isExpired() && check == true)
      {
      signalState = RESOLVE; //GO TO THE NEXT STATE
      setValueSentOnAllFaces(signalState);
      }

   }
}

void resolveLoop() { // could not figure out how to reset the game. must reupload at this point
  
  FOREACH_FACE(f) 
  {
    if (!isValueReceivedOnFaceExpired(f)) 
    {//a neighbor!
      if (getLastValueReceivedOnFace(f) == PLAY) 
      {//This neighbor doesn't know it's PLAY time. Stay in PLAY
        signalState = RESOLVE;
      }
     }
   }    
  
  if(win == true) //WIN STATE
  {
   setColor(CYAN); 
  } 
}
