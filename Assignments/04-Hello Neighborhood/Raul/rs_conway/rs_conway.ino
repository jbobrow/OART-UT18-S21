int state = 0;
int aliveCount = 0;
int deadCount = 0;
bool aliveCell = false;
Timer delayTimer;

void setup() {
  // put your setup code here, to run once:
  randomize();
  state = random(2);
}

void loop() {
  // put your main code here, to run repeatedly:

  //if I single click, cell becomes alive
  if (buttonSingleClicked())
  {
    aliveCell = !aliveCell;
  }

  if (delayTimer.isExpired())
  {
    delayTimer.set(1000);

    if (state == 0 || aliveCell == false)
    {
      //dead cell
      setColor(OFF);
      aliveCell = false;
      setValueSentOnAllFaces(2);
    }
    if (state == 1 || aliveCell == true)
    {
      //alive cell
      setColor(MAGENTA);
      aliveCell = true;
      setValueSentOnAllFaces(3);
    }

    //RULES

    //counting alive neighbors
    FOREACH_FACE(f)
    {
      if (!isValueReceivedOnFaceExpired(f))
      {
        if (getLastValueReceivedOnFace(f) == 3)
        {
          aliveCount++;
        }
        if (getLastValueReceivedOnFace(f) == 2)
        {
          deadCount++;
        }
      }
    }

    aliveCheck();
    deadCheck();



  }

}

void aliveCheck()
{

  //any live cell with fewer than two live neighbors dies

  if (state == 1 && aliveCount < 2)
  {
    state = 0;
    aliveCount = 0;
    deadCount = 0;
  }

  // any live cell with 2 or 3 neighbors lives on to the next generation

  if (state == 1 && aliveCount == 2)
  {
    state = 1;
    aliveCount = 0;
    deadCount = 0;
  }
  else if (state == 1 && aliveCount == 3)
  {
    state = 1;
    deadCount = 0;
    aliveCount = 0;
  }

  // any live cell with more than 2 or 3 neighbors dies

  if (state == 1 && aliveCount > 3)
  {
    state = 0;
    deadCount = 0;
    aliveCount = 0;
  }
}

void deadCheck ()
{
  //any dead cell with exactly 3 alive neighbors becomes alive
  if (state == 0 && aliveCount == 3)
  {
    state = 1;
    deadCount = 0;
    aliveCount = 0;
  }
}
