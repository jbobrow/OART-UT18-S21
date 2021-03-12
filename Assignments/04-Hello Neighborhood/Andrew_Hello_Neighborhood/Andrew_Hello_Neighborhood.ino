void setup() {
  setValueSentOnAllFaces(0);
  setValueSentOnFace(2, 0); // setting. the white blink to active
}

void loop() {
bool isConnectedCorrectly = false;
 
if (getLastValueReceivedOnFace(0) == 2) //checking face 0 (white face)
{
    isConnectedCorrectly = true;
  setValueSentOnFace(2, 1); //sets the red face for this blink to active
}
 
if (getLastValueReceivedOnFace(1) == 2) //checking face 1 (red face)
{
    isConnectedCorrectly = true;
  setValueSentOnFace(2, 1);
}
 
  if(isAlone())  //turns it off after connecting
  {
    setColor(OFF);
     isConnectedCorrectly = false;
  }
 
  if (isConnectedCorrectly)
  {
    setColor(MAGENTA);
  } else
  {
    setColor(OFF);
  }

  //base face colors
  setColorOnFace(WHITE, 0);
  setColorOnFace(RED, 1);
}
