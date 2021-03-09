//MYSTERY BLINKS
int x = 0;
int color = 0;
bool double_clicked_recd = false;

enum State
{
  SETUP,
  HIDDEN
};
byte state;

//function protos
void set_my_color (int color);

void setup() {

  state = SETUP;

}

void loop() {

  setValueSentOnAllFaces(7);

  if (buttonSingleClicked() && state == SETUP)
  {
    x++;
    color = x % 6;
    set_my_color(color);
  }

  if (buttonLongPressed())
  {
    state = SETUP;
    setColor(WHITE);
  }



  if (buttonDoubleClicked())
  {
    setColor(OFF);
    setColorOnFace(WHITE, 0);
    state = HIDDEN;
  }

  if (state == HIDDEN)
  {
    if (isValueReceivedOnFaceExpired(0) || getLastValueReceivedOnFace(0) == 7)
    {
      setColor(OFF);
      setColorOnFace(WHITE, 0);
    }
    else if (!isValueReceivedOnFaceExpired(0))
    {
      //compare against the color value on that face
      if (getLastValueReceivedOnFace(0) == color)
      {
        setColor(GREEN);
      }
      else
      {
        setColor(RED);
      }
    }
  }

}

void set_my_color (int color)
{
  if (color == 0)
  {
    setColor(makeColorRGB(162, 26, 235));
  }
  else if (color == 1)
  {
    setColor(BLUE);
  }
  else if (color == 2)
  {
    setColor(CYAN);
  }
  else if (color == 3)
  {
    setColor(ORANGE);
  }
  else if (color == 4)
  {
    setColor(YELLOW);
  }
  else
  {
    setColor(MAGENTA);
  }
}
