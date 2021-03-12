
Timer pressure_time;
Timer aesthetic_time;
bool is_spinning;
byte face_position;
int game_state = 0;

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

  //setups
  face_position = (millis() / 190) % 6;
  if (game_state == 0)
  {
    setColor(OFF);
    setColorOnFace(makeColorRGB(255, 255, 10), face_position);
  }
  
 
  if (buttonDoubleClicked())
  {
    //start the game
    is_spinning = true;
    pressure_time.set(15 * 1000);
    game_state += 1;
  }

  //is the game active
  if (pressure_time.isExpired() != true)
  {
    //rotate blue light
    if (is_spinning == true)
    {
      setColor(makeColorRGB(237, 162, 0));
      setColorOnFace(makeColorRGB(10, 35, 255), face_position);

    }
  }


  if (buttonSingleClicked())
  {
    //stop the blue light
    is_spinning = false;
    pressure_time.never();
    //if the light is on p0, then flash green and allow the game to be restarted, if not flash red

    if (face_position == 0)
    {

      setColor(makeColorRGB(0, 255, 1));

    }
    if (face_position != 0)
    {
      setColor(makeColorRGB(255, 5, 5));
    }

  }

  if (pressure_time.isExpired() == true && game_state == 1)
  {
    setColor(makeColorRGB(millis(), 5, 5));
  }


}
