void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

  // send the face ID on each face
  FOREACH_FACE(f) {
    setValueSentOnFace(f, f);
  }

  // listen to neighbors and display their value
  FOREACH_FACE(f) {
    //
    if (!isValueReceivedOnFaceExpired(f)) {
      // display the value
      byte neighborVal = getLastValueReceivedOnFace(f);
      byte hue = neighborVal * 42;
      setColor(makeColorHSB(hue, 255, 255));
    }
  }

}
