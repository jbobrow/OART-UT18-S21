
//GUESSING BLINKS
int x = 0;
int color = 0;

void setup() {

}

void loop() {



  if (buttonSingleClicked())
  {
    x++;
    color = x % 6;
    if (color == 0)
    {
      setColorOnFace(makeColorRGB(162, 26, 235), 0);
    }
    else if (color == 1)
    {
      setColorOnFace(BLUE, 0);
    }
    else if (color == 2)
    {
      setColorOnFace(CYAN, 0);
    }
    else if (color == 3)
    {
      setColorOnFace(ORANGE, 0);
    }
    else if (color == 4)
    {
      setColorOnFace(YELLOW, 0);
    }
    else if (color == 5)
    {
      setColorOnFace(MAGENTA, 0);
    }
  }

  setValueSentOnFace(color, 0);


  
}
