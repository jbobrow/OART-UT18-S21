/*  --------------------
    09-Sparkle
    by Jonathan Bobrow
    Move38, Inc. 2021
    --------------------
*/

byte sparkleOffset[6] = {0, 3, 5, 1, 4, 2};
uint32_t timeOfSend = 0;
#define SPARK_DURATION  800

Timer sparkleTimer;
#define SPARKLE_RATE 3000

void setup() { 
}

void loop() {
  
  if(buttonPressed()) { // start the sparkle over when pressed
    timeOfSend = millis();
  }

  if(sparkleTimer.isExpired()) {

    timeOfSend = millis();
    sparkleTimer.set(SPARKLE_RATE);
  }

  displaySparkle();
}


void   displaySparkle() {
  uint32_t delta = millis() - timeOfSend;
  byte offset = 50;

  FOREACH_FACE(f) {

    // if the face has started it's glow
    uint16_t sparkleStart = sparkleOffset[f] * offset;
    uint16_t sparkleEnd = sparkleStart + SPARK_DURATION - (6 * offset);

    if ( delta > sparkleStart ) {
      // minimum of 125, maximum of 255
      byte phaseShift = 60 * f;
      byte amplitude = 55;
      byte midline = 185;
      byte rate = 4;
      byte lowBri = midline + (amplitude * sin8_C( (phaseShift + millis() / rate) % 255)) / 255;
      byte brightness;
      byte saturation;

      if ( delta < sparkleEnd ) {
        brightness = lowBri + 255 - map(delta, sparkleStart, sparkleStart + SPARK_DURATION - (6 * offset), lowBri, 255);
        saturation = map(delta, sparkleStart, sparkleStart + SPARK_DURATION - (6 * offset), 0, 255);
      }
      else {
        brightness = lowBri;
        saturation = 255;
      }

      Color faceColor = makeColorHSB(20, saturation, brightness);
      setColorOnFace(faceColor, f);
    }
  }
}
