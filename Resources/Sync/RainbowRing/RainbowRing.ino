/*
   Ring of Blinks
*/

byte id;
uint16_t period;


#define DELAY 500

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

  if (buttonDoubleClicked()) {
    // check for two neighbors
    if (countNeighbors() == 2) {
      // start cycle
      // assign myself
      id = 0;
      // look to first neighbor to send
      // share my id
      sendDatagramOnFace(f)
    }
  }

  // listen for neighbors with datagrams
  FOREACH_FACE(f) {
    if(isDatagramReadyOnFace(f)) {
      byte length = getDatagramLengthOnFace(f);
      char* data[] = getDatagramOnFace(f);

      // considere this datagram read
      markDatagramReadOnFace(f);
    }
  }
  // initialized
  // if id == 0...
  // else...
}

byte countNeighbors() {
  byte count = 0;
  FOREACH_FACE(f) {
    if (!isValueReceivedOnFaceExpired(f)) {
      count++;
    }
  }
  return count;
}
