Timer newTime;
bool startGame = false;
bool setUp = false;
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
                setUp = false;
              } 
              else if (!newTime.isExpired())
              {
                byte timerProgress = newTime.getRemaining();
                int timePassed = millis();
                
                  if(timePassed > progressVal){
                    setColorOnFace(BLUE, 0);
                  }
                  if(timePassed > progressVal * 2){
                    setColorOnFace(BLUE, 1);
                  }
                  if(timePassed > progressVal * 3){
                    setColorOnFace(BLUE, 2);
                  }
                  if(timePassed > progressVal * 4){
                    setColorOnFace(BLUE, 3);
                  }
                  if(timePassed > progressVal * 5){
                    setColorOnFace(BLUE, 4);
                  }
                  if(timePassed > progressVal * 5.9){
                    setColorOnFace(BLUE, 5);
                  }

                if(timerProgress < 100 && buttonDoubleClicked()){
                  pWin = true;
                  setColor(YELLOW);
                  startGame = false;
                  setUp = false;
                  
                }
                
              }
            }
}
