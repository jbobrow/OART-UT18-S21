Timer pressure_time;
byte face_position;
int game_state = 0;
Timer zzTimer;
bool clicked = false;
byte faceColor[6];
int dataReceived = 7;
int keyvalue;
int myvalue;
bool answer = false;
int counter = 1;

void setup() {
  // put your setup code here, to run once:
  randomize();


}

// value received 1 = good, 2 = bad


void loop() {
  // put your main code here, to run repeatedly:

  // SPINNY IDLE
  if (game_state == 0)
  {
    setColor(makeColorRGB(255, 255, 10));
    DrawBaskets();
    if (!IsBasket())
    {
      CheckValueReceived();
      if (dataReceived != 7)
      {
        game_state = 2;
      }


    }
    if (buttonDoubleClicked() && IsBasket())
    {
      //assign random values for each face
      FOREACH_FACE(f) {
        faceColor[f] = random(5) + 1;
      }
      game_state = 1;



    }

  }



  if (game_state == 1)
  {
    if (IsBasket())
    {
      setValueSentOnFace(faceColor[0], 0);
      setValueSentOnFace(faceColor[1], 1);
      setValueSentOnFace(faceColor[2], 2);
      setValueSentOnFace(faceColor[3], 3);
      setValueSentOnFace(faceColor[4], 4);
      setValueSentOnFace(faceColor[5], 5);
      game_state = 2;
    }


  }





  if (game_state == 2)
  {
    if (!IsBasket())
    { myvalue = dataReceived;
      SetBlinkColor(myvalue);


    }
    zzTimer.set(1000);
    game_state = 3;

  }

  if (game_state == 3)
  {
    if (zzTimer.isExpired())
    {
      if (!IsBasket())
      {
        //setColor(OFF);

        game_state = 4;

      }
      if (IsBasket())
      {
        keyvalue = random(5) + 1;
        SetBlinkColor(keyvalue);
        SendData(keyvalue + 7);
        game_state = 4;
      }
    }
  }
  if (game_state == 4)
  {
    CheckValueReceived();
    if (!IsBasket())
    {
      keyvalue = dataReceived - 7;

      if (buttonSingleClicked())
      {
        if (myvalue == keyvalue)
        {
          setColor(GREEN);
          game_state = 0;
          SendData(0);
          CheckValueReceived();


        }
        if (myvalue != keyvalue)
        {
          setColor(RED);
        }
      }


    }


  }







}

void SetBlinkColor(int _n)
{
  switch (_n)
  {
    case 1:
      {
        setColor(RED);
        break;
      }
    case 2:
      {
        setColor(BLUE);
        break;
      }
    case 3:
      {
        setColor(GREEN);
        break;
      }
    case 4:
      {
        setColor(ORANGE);
        break;
      }
    case 5:
      {
        setColor(MAGENTA);
        break;
      }
    case 6:
      {
        setColor(CYAN);
        break;
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

  if (neighbor == 6)
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
  setColorOnFace(RED, 0);
  setColorOnFace(WHITE, 1);
  setColorOnFace(RED, 2);
  setColorOnFace(WHITE, 3);
  setColorOnFace(RED, 4);
  setColorOnFace(WHITE, 5);
}

void CheckValueReceived()
{


  FOREACH_FACE(f)
  {
    if (getLastValueReceivedOnFace(f) != 0 && didValueOnFaceChange(f))
    {
      dataReceived = getLastValueReceivedOnFace(f);
    }
  }
}
