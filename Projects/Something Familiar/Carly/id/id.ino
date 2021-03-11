byte ids[10] = {1, 1, 2, 2, 3, 3, 4, 4, 5, 5};

#define UNASSIGNED 10 // this is out of bound for the array

byte myIdIndex = UNASSIGNED;  // to store which one in the array is ours

Color PURPLE = makeColorRGB(180, 74, 255);

void setup() {
  // put your setup code here, to run once:
  setValueSentOnAllFaces(0);

}

void loop() {

  // listen for a button click
  if (buttonSingleClicked() ) {
    myIdIndex = 0;  // I'm first in line
    setValueSentOnAllFaces(1);  // tell my neighbors
  }

  // listen to neighbors for assignment
  if (myIdIndex == UNASSIGNED) {  // only assign if not yet assigned
    FOREACH_FACE(f) {
      if (!isValueReceivedOnFaceExpired(f)) { // If I have a neighbor here do the following
        if (getLastValueReceivedOnFace(f) == 1) {
          myIdIndex = 1;
          setValueSentOnAllFaces(2);
        }
        else if (getLastValueReceivedOnFace(f) == 2) {
          myIdIndex = 2;
          setValueSentOnAllFaces(3);
        }
        else if (getLastValueReceivedOnFace(f) == 3) {
          myIdIndex = 3;
          setValueSentOnAllFaces(4);
        }
        else if (getLastValueReceivedOnFace(f) == 4) {
          myIdIndex = 4;
          setValueSentOnAllFaces(5);
        }
        else if (getLastValueReceivedOnFace(f) == 5) {
          myIdIndex = 5;
          setValueSentOnAllFaces(6);
        }
        else if (getLastValueReceivedOnFace(f) == 6) {
          myIdIndex = 6;
          setValueSentOnAllFaces(7);
        }
        else if (getLastValueReceivedOnFace(f) == 7) {
          myIdIndex = 7;
          setValueSentOnAllFaces(8);
        }
        else if (getLastValueReceivedOnFace(f) == 8) {
          myIdIndex = 8;
          setValueSentOnAllFaces(9);
        }
        else if (getLastValueReceivedOnFace(f) == 9) {
          myIdIndex = 9;
          setValueSentOnAllFaces(10);
        }
      }
    }
  }


  //display colors

  if (myIdIndex != UNASSIGNED) {
    // display my assigned color

    if (ids[myIdIndex] == 1) {
      setColor(MAGENTA);
    } else if (ids[myIdIndex] == 2) {
      setColor(BLUE);
    } else if (ids[myIdIndex] == 3) {
      setColor(GREEN);
    } else if (ids[myIdIndex] == 4) {
      setColor(YELLOW);
    } else if (ids[myIdIndex] == 5) {
      setColor(PURPLE);
    }
  }
  else {
    // display a rainbow when not yet assigned
    setColorOnFace(RED, 0);
    setColorOnFace(ORANGE, 1);
    setColorOnFace(YELLOW, 2);
    setColorOnFace(GREEN, 3);
    setColorOnFace(BLUE, 4);
    setColorOnFace(MAGENTA, 5);
  }
}
