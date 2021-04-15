int neighbor_count = 0;
bool broken_hex = false;

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

  //reset variables every passthrough
  neighbor_count = 0;
  broken_hex = false;

  // find how many neighbors I have
  for (int i = 0; i < 6; i++)
  {
    //if I have a neighbor, add to tally
    if (!isValueReceivedOnFaceExpired(i))
    {
      neighbor_count++;
    }
  }

  //if I have 5 neighbors, def in a broken hex
  if (neighbor_count == 5)
  {
    broken_hex = true;
  }

  //broadcast amount of neighbors for continuous updating
  setValueSentOnAllFaces(neighbor_count);

  // check if any face received 5
  for (int i = 0; i < 6; i++)
  {
    //if somebody is connected and has received a 5 on that face, we are in a broken hex 
    if (!isValueReceivedOnFaceExpired(i) && getLastValueReceivedOnFace(i) == 5)
    {
      broken_hex = true;
    }
  }

  //if I am alone, def not in a broken hex
  if (isAlone())
  {
    broken_hex = false;
  }

  //if I'm in a broken hex, set all to green
  if (broken_hex == true)
  {
    setColor(GREEN);
  }
  else //set color to red
  {
    setColor(RED);
  }
}
