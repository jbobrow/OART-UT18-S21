Timer pressure_time;
Timer aesthetic_time;
bool is_spinning = true;
byte face_position;
int game_state = 0;
Timer zzTimer;
bool clicked = false;
int counter = 1;


int dataReceived = 0;

void setup() {
  // put your setup code here, to run once:

}

// value received 1 = good, 2 = bad


void loop() {
  // put your main code here, to run repeatedly:

  //setups
  face_position = (millis() / 190) % 6;
  
  // SPINNY IDLE
  if (game_state == 0)
  {
    setColor(OFF);
    setColorOnFace(makeColorRGB(255, 255, 10), 5);
  	DrawBaskets();  
  }

  // START GAME
  if (buttonDoubleClicked())
  {
    //start the game
    is_spinning = true;
    pressure_time.set(15 * 1000);
    game_state = 1;
  }  
  
  if (game_state == 1)
  {
    	SendData(0);
    
      //is the game active
      if (!pressure_time.isExpired())
      {    
          //rotate blue light
          if (is_spinning)
          {
            setColor(makeColorRGB(237, 162, 0));
            setColorOnFace(makeColorRGB(10, 35, 255), face_position);

              if (buttonSingleClicked())
              {
                //stop the blue light
                is_spinning = false;
                pressure_time.never();

                  //if the light is on p0, then flash green and allow the game to be restarted, if not flash red
                  if (face_position <= 1)
                  {

                    setColor(makeColorRGB(0, 255, 1));
                    zzTimer.set((1 / counter) * 1000);
                    clicked = true;
                  }
                  else
                  {
                    setColor(makeColorRGB(255, 5, 5));
                  }
              }
          }      
      }
      // not initial
      else
      {
          if (zzTimer.isExpired())
          {
            setColor(makeColorRGB(millis(), 5, 5));
          }
      }

      // timer has expired
      if (zzTimer.isExpired() && clicked)
      {        
        SendData(1); 
        setColor(OFF);                
      }   
      else if (!zzTimer.isExpired() && clicked)
      {
        if (IsBasket() && pressure_time.isExpired())
        {
          if (buttonSingleClicked()) 
          {
            SendData(2);
          }
        }
      }
    
    CheckValueReceived();

    if (dataReceived == 2)
    {    
      counter++;
      game_state = 0;
      clicked = false;
      SendData(2);
    }
  }
  // Not in the game yet
  else
  {   
    
	CheckValueReceived();
    if (dataReceived == 1)
    {
      setColor(GREEN);
      zzTimer.set((1.5/counter) * 1000);
      clicked = true;
      game_state = 1;
    }
  } 
} 

void DrawBaskets()
{
  // check if there is only 1 neighbor and it is not the starting position; Am i the basket
  if (IsBasket())
  {
    DisplayBasket();
  }
}

bool IsBasket()
{
    //count the neighbors
  // count the neighbors
  // if it has only 1 neighbor and not clicked, it is a hoop.
  // If it is a hoop set color as basketball hoop
  int neighbor = 0;
  FOREACH_FACE(f)
  {
    if (!isValueReceivedOnFaceExpired(f))
    { 
      neighbor++;
    }
  }
  
  if (neighbor <= 1)
  {
    return true;
  }
}

void SendData(int n)
{
  setValueSentOnAllFaces(n);
}

void DisplayBasket()
{
  setColorOnFace(RED,0);
  setColorOnFace(WHITE,1);
  setColorOnFace(RED,2);
  setColorOnFace(WHITE,3);  
  setColorOnFace(RED,4);
  setColorOnFace(WHITE,5);
}

void CheckValueReceived()
{   
  dataReceived = 0;
  
  FOREACH_FACE(f)
  {
    if (getLastValueReceivedOnFace(f) != 0 && didValueOnFaceChange(f))
    {
        dataReceived = getLastValueReceivedOnFace(f);
    }    
  }
}

