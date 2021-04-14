Color cycleColors[4] = {RED, GREEN, YELLOW, BLUE};
int cycleCount = 0;
bool increment = false;


void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

  //if a blink is separated, increment 1

  if (isAlone())
  {
    if (increment == true)
    {
      cycleCount++;
      increment = false;
    }
  }
  else
  {
    increment = true;
  }

  //send it all around

  setValueSentOnAllFaces(cycleCount);
  FOREACH_FACE(f)
  {
    if (getLastValueReceivedOnFace(f) == cycleCount)
    {
      setValueSentOnAllFaces(cycleCount);
    }
  }

  //display it

  setColor(cycleColors[cycleCount]);
  FOREACH_FACE(f)
  {
    if (getLastValueReceivedOnFace(f) == cycleCount)
    {
      setColor(cycleColors[cycleCount]);
    }
  }





}
