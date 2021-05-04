bool isBunny = false;
bool isBunnyTrail = false;
int pieceCycle = 0;
bool isFox = false;
bool isGrass = false;
bool transition = false;
bool hideBunny = false;
bool reset = false;
bool animalTransition = false;
Timer wipeTimer;

void setup() {

}

void loop() {


  if (buttonDoubleClicked())
  {
    //double click to cycle through the piece possibilities
    pieceCycle++;


  }
  if (((pieceCycle) % 3) == 0 || isGrass == true)
  {
    //if these conditions are true, then become grass and not anything else
    isGrass = true;
    isBunny = false;
    isFox = false;
  }

  if (((pieceCycle) % 3) == 1 || isBunny == true)
  {
    //if these conditions are true, then become bunny and not anything else
    isGrass = false;
    isBunny = true;
    isFox = false;
  }
  if (((pieceCycle) % 3) == 2 || isFox == true)
  {
    //if these conditions are true, then become a fox and not anything else
    isGrass = false;
    isBunny = false;
    isFox = true;
  }

  if (isGrass)
  {
    //grass display
    setColor(dim(GREEN, 135));
    setColorOnFace(GREEN, ((millis() / 1000) % 6));
  }

  if (isBunny)
  {
    //bunny display and messaging
    setColor(dim(WHITE, 125));
    setColorOnFace(WHITE, ((millis() / 200) % 6));
    //setValueSentOnAllFaces(3);
  }
  if (isBunnyTrail)
  {
    //dimmer white to display the bunny trail
    setColor(dim(WHITE, 120));
  }
  if (isFox)
  {
    //fox display
    setColor(dim(ORANGE, 125));
    setColorOnFace(ORANGE, ((millis() / 500) % 6));
    //setValueSentOnAllFaces(4);
  }

  //_________ CHANGING LOGIC

  if (buttonSingleClicked())
  {
    //If I single click a grass, then tell it to transition
    if (isGrass == true || isBunnyTrail == true)
    {
      //single click a green blink to transition
      transition = true;
    }
    //if I single clicked an animal, then transition in another specific way
    if (isBunny == true || isFox == true)
    {
      animalTransition = true;
    }

  }

  //wipe single clicks
  buttonSingleClicked();

  if (transition)
  {
    //send a value to distinguish transition
    setValueSentOnAllFaces(5);
    FOREACH_FACE(f)
    {
      //change tile [to move to] to bunny
      if (getLastValueReceivedOnFace(f) == 3 && isBunny == false && !isValueReceivedOnFaceExpired(f))
      {
        isBunny = true;
        pieceCycle = 1;
      }
      //change tile [to move to] to fox
      if (getLastValueReceivedOnFace(f) == 4 && isFox == false && !isValueReceivedOnFaceExpired(f))
      {
        isFox = true;
        pieceCycle = 2;
      }
    }
    //close both gates after movement has happened
    transition = false;
    animalTransition = false;
  }


  if (animalTransition)
  {
    //If i am an animal, send specific values for my type
    if (isBunny)
    {
      setValueSentOnAllFaces(3);
    }
    if (isFox)
    {
      setValueSentOnAllFaces(4);
    }

    FOREACH_FACE(f)
    {
      //if i was a a bunny blink and i am receiving a signal to transition, show trail but don't be a bunny
      if (getLastValueReceivedOnFace(f) == 5 && isBunny == true && !isValueReceivedOnFaceExpired(f))
      {
        isBunnyTrail = true;
        isBunny = false;
      }
      //if I was a fox and I am told to transition, turn off
      if (getLastValueReceivedOnFace(f) == 5 && isFox == true && !isValueReceivedOnFaceExpired(f))
      {
        isGrass = true;
        pieceCycle = 0;
      }
    }
  }

  //__HIDING RABBIT STATE

  //If the button is triple clicked, change the bunny to match the surroundings
  if (buttonMultiClicked())
  {
    hideBunny = !hideBunny;
  }
  if (hideBunny)
  {
    setColor(dim(GREEN, 135));
    setColorOnFace(GREEN, ((millis() / 1000) % 6));
  }

  //___RESET

  if (buttonLongPressed())
  {
    reset = true;
  }
  if (reset)
  {
    setValueSentOnAllFaces(30);

    FOREACH_FACE(f)
    {
      //if I recieve 30, send it
      if (getLastValueReceivedOnFace(f) == 30 && !isValueReceivedOnFaceExpired(f))
      {
        setValueSentOnAllFaces(30);
      }
    }
    FOREACH_FACE(f)
    {
      //if I recieve the signal to reset, change to grass
      if (getLastValueReceivedOnFace(f) == 30 && !isValueReceivedOnFaceExpired(f))
      {
        setColor(OFF);
        wipeTimer.set(1000);
        if (wipeTimer.isExpired())
        {
          isGrass = true;
        }
      }
    }

    reset = false;
  }
}
