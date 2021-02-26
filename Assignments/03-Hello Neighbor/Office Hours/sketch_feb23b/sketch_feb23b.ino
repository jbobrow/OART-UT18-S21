void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

  if (!isAlone()) { // if we are not alone
    // display a signal to say we have neighbors
    setColor(BLUE); // I have neighbors
  }
  else {
    // display that we are alone
    setColor(RED); // I am lonely
  }

}
