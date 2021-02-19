
Timer pressure_time;

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

//setups

byte face_position = (millis()/250)%6;



//if(buttonDoubleClicked())
  //{
    pressure_time.set(5*1000);
    setColor(ORANGE);
    setColorOnFace(BLUE, face_position);
    if(buttonSingleClicked())
    {
        
        
    }

    if(pressure_time.isExpired() == true)
    {
      setColor(RED);
    }
    
  //}

 
}
