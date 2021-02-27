bool isFirst = false;
bool isLast = false;
int nextBlink;
int prevBlink;

Timer countdown;

void setup() {
    setColor(WHITE);
}

void loop() {
  if (isAlone()){
      if (buttonDoubleClicked()) {
          isFirst = true;
            setValueSentOnAllFaces(1);
        }
    }
    if (!isFirst) {
        FOREACH_FACE(f) {
            if (!isValueReceivedOnFaceExpired(f)){
                if (getLastValueReceivedOnFace(f) == 1) {
                    prevBlink = f;
                    if (f + 3 > 5) {
                        nextBlink = (f + 3) - 6;
                        setValueSentOnFace(nextBlink, 1);
                    }
                    else {
                        nextBlink = f + 3;
                        setValueSentOnFace(nextBlink, 1);
                    }
                }
            }
        }
        if (isValueReceivedOnFaceExpired(nextBlink)){
            isLast = true;
        }
        if (isAlone()){
            isFirst = false;
            isLast = false;
        }
        if (!isValueReceivedOnFaceExpired(nextBlink) && !isValueReceivedOnFaceExpired(prevBlink)){
            isFirst = false;
            isLast = false;
        }  
    }
  
    if (isFirst){
        if (buttonLongPressed()) {
            setColor(GREEN);
            countdown.set(2000);
            dim(GREEN, millis()/2);
            if (buttonPressed() && !countdown.isExpired()) {
                setValueSentOnAllFaces(2);
                setColor(WHITE);
            }
            else {
                setValueSentOnAllFaces(3);
                setColor(RED);
            }
        }
    }
    else {
        if (getLastValueReceivedOnFace(prevBlink) == 2) {
            setColor(GREEN);
            countdown.set(750);
            dim(GREEN, millis()/2);
            if (buttonPressed() && !countdown.isExpired()) {
                setValueSentOnAllFaces(2);
                setColor(WHITE);
            }
            else {
                setValueSentOnAllFaces(3);
                setColor(RED);
            }
        }
        else if (getLastValueReceivedOnFace(prevBlink) == 3) {
          setColor(RED);
            setValueSentOnAllFaces(3);
        }
    }
//    if (isLast) {
//        setColor(BLUE);
//    }
//    else if (isFirst) {
//        setColor(GREEN);
//    }
//    else {
//        setColor(WHITE);
//    }
}
