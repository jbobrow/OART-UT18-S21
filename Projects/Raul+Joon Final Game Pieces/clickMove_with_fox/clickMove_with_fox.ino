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

//RESET STUFF
int myState = 0;
uint32_t timeOfSwitch = 0;
#define BROADCAST_DURATION 100
#define DEAF_DURATION 200

void setup() {

}

void loop() {

  //double click to cycle through piece choices
  if (buttonDoubleClicked())
  {
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
    //If I single click a non-animal element, then tell it to transition
    if (isGrass == true || isBunnyTrail == true)
    {
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
      //if I am a bunny and i recieve a fox signal, turn to fox and trigger a win state
      if (getLastValueReceivedOnFace(f) == 4 && isBunny == true && !isValueReceivedOnFaceExpired(f))
      {
        isFox = true;
        pieceCycle = 2;
        //TrIGGER THE WIN STATE
        //sendAll(1);
      }
      //if I was a fox and I am told to transition, turn off
      if (getLastValueReceivedOnFace(f) == 5 && isFox == true && !isValueReceivedOnFaceExpired(f))
      {
        isGrass = true;
        pieceCycle = 0;
      }
      //if i am a fox and I recieve a bunny signal, turn off
      if (getLastValueReceivedOnFace(f) == 3 && isFox == true && !isValueReceivedOnFaceExpired(f))
      {
        isGrass = true;
        pieceCycle = 0;
      }
    }
  }

  //__HIDING RABBIT STATE

  //If the button is triple clicked, change the bunny to match the surroundings
  if (buttonMultiClicked() && isBunny == true)
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
    //set my state to reset
    myState = 2;
    sendAll(myState);
    reset = false;
  }



}

//COORDINATE SEND FUNCTION

void sendAll(byte state)
{
   //set my state
   setState(state);
  
  //should we listen to neighbors?
  if (timeOfSwitch < millis() - (BROADCAST_DURATION + DEAF_DURATION) )
  {

    // we have broadcasted and ignored feedback, so we are good to listen again
    FOREACH_FACE(f)
    {
      //is there a blink here?
      if (!isValueReceivedOnFaceExpired(f))
      {

        //get their value
        byte neighbor = getLastValueReceivedOnFace(f);


        //is our neighbor different
        if (neighbor != 5 && neighbor != myState)
        {
          setState (neighbor);
        }
      }
    }
  }

  if (timeOfSwitch <  millis() - BROADCAST_DURATION )
  {
    // return to sending 0
    setValueSentOnAllFaces(5);  // stop sending state - I used to have it as zero but 5 is the default for a non-animal being
  }


  // display

  if (state == 1)
  {
    winAnim();

  }

  if (state == 2)
  {
    resetAnim();
  }

}

void setState(byte _state)
{
  //set the value we are passing in equal to my own state
  myState = _state;

  timeOfSwitch = millis();

  setValueSentOnAllFaces(myState);// start sending state
}



void winAnim ()
{
  //turn all the blinks to orange and back to normal
  setColor(dim(ORANGE, 125));
  setColorOnFace(ORANGE, ((millis() / 90) % 6));
  wipeTimer.set(3000);
  if (wipeTimer.isExpired())
  {
    isGrass = true;
    pieceCycle = 0;
  }

  myState = 0;
}


void resetAnim ()
{
  //Turn off all the blinks for one second and then change everything back to normal
  //setColor(OFF);
  //wipeTimer.set(1000);
  //if (wipeTimer.isExpired())
  //{
    isGrass = true;
    pieceCycle = 0;
  //}

  //myState = 0;
}
