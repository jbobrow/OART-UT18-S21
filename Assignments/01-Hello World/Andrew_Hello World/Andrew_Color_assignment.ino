Timer isolated;
void setup() {
  // put your setup code here, to run once:

}
//The blinks is lonely and crying alone, but once it touches another blinks it becomes happy! 
void loop() {
  if (isAlone())
  {
    isolated.set(1000);
  } 
  if (isolated.isExpired())//Happy face, if no longer alone do this!
  { 
    byte dimness = sin8_C(millis() % 256);
    setColorOnFace(RED, 0);
    setColorOnFace(RED, 1); 
    setColorOnFace(dim(GREEN, dimness), 2); //2-3 is eyes
    setColorOnFace(dim(GREEN, dimness), 3);
    setColorOnFace(RED, 4);
    setColorOnFace(RED, 5);
    } 
  else //Sad face, if alone do this. 
    {
     setColorOnFace(dim(BLUE, 30), 2); //2-3 is eyes, sad eyes cause alone
     setColorOnFace(dim(BLUE, 30), 3);
     setColorOnFace(dim(RED, 50), 0);
     setColorOnFace(dim(OFF, 50), 1);
     setColorOnFace(dim(OFF, 50), 4);
     setColorOnFace(dim(RED, 50), 5);
  }
}
 
