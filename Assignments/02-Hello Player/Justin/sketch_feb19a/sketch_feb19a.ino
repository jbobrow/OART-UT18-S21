Timer newTime;
Timer progressTimer;
bool startGame = false;
bool setUp = true;
bool pWin = false;
int progressVal;
int timerVal;

void setup() {
  // put your setup code here, to run once:
    randomize();
}

void loop() {
  // put your main code here, to run repeatedly:
         if (buttonSingleClicked() && !startGame)
            {
              startGame = true;
              setUp = true;
            }
            
         if(startGame)   
            {

              if(setUp)
                {
                setColor(GREEN);
                timerVal = random(10000);
                progressVal = timerVal/6;
                newTime.set(timerVal);
                setUp = false;
                }
    
              if(newTime.isExpired() && !pWin)
              {
                setColor(RED);
                startGame = false;
                pWin = false;
              } 
              else if (!newTime.isExpired())
              {
                byte timerProgress = newTime.getRemaining();
                
                FOREACH_FACE(f){
                  progressTimer.set(progressVal);
                  if(progressTimer.isExpired()){
                    setColorOnFace(BLUE, f);
                  }
                }

                if(timerProgress < 100 && buttonSingleClicked()){
                  pWin = true;
                  setColor(YELLOW);
                }
                
              }
            }
}
