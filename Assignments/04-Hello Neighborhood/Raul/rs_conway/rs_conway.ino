int state = 0;
int aliveCount = 0;
bool aliveCell = false;

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
    aliveCell = true;
  }
  if (state == 0)
  {
    //dead cell
    setColor(OFF);
    aliveCell = false;
  }
  if (state == 1 || aliveCell == true)
  {
    //alive cell
    setColor(MAGENTA);
    setValueSentOnAllFaces(3);
  }

  //RULES

  //counting alive neighbors
  FOREACH_FACE(f)
  {
    if (getLastValueReceivedOnFace(f) == 3)
    {
      aliveCount++;
    }
  }

  //any live cell with fewer than two live neighbors dies

  if (state == 1 && aliveCount < 2)
  {
    state = 0;
    aliveCount = 0;
  }

  // any live cell with 2 or 3 neighbors lives on to the next generation

  if (state == 1 && aliveCount == 2)
  {
    aliveCount = 0;
  }
  else if (state == 1 && aliveCount == 3)
  {
    aliveCount = 0;
  }

  // any live cell with more than 2 or 3 neighbors dies

  if (state == 1 && aliveCount > 3)
  {
    state = 0;
    aliveCount = 0;
  }
  //any dead cell with exactly 3 alive neighbors becomes alive

  if (state == 0 && aliveCount == 3)
  {
    state = 0;
    aliveCount = 0;
  }

}
