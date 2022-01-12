void pulse_one_color_all(int brigthSpeed, int hue) {
  /*Serial.println();
  Serial.println("bouncedirection " + (String)bouncedirection);
  Serial.println("brigthSpeed " + (String)brigthSpeed);
  Serial.println("hue " + (String)hue);
  Serial.println("thissat " + (String)thissat);
  */
  if (bouncedirection == 0) {
    ibright += brigthSpeed;
    if (ibright >= 254) {
      bouncedirection = 1;
    }
  }
  if (bouncedirection == 1) {
    ibright -= brigthSpeed;
    if (ibright <= 1) {
      bouncedirection = 0;
    }
  }
  for (int idex = 0 ; idex < LED_COUNT; idex++ ) {
    leds[idex] = CHSV(hue, thissat, ibright);
  }
  LEDS.show();
  if (safeDelay(thisdelay)) return;
}

boolean safeDelay(int delTime) {
  uint32_t thisTime = millis();
  while (millis() - thisTime <= delTime) {
    if (changeFlag) {
      changeFlag = false;
      return true;
    }
  }
  return false;
}

void fireEffect(int brithness) {
  strip.setBrightness(brithness);
  mGradient<4> myGrad;
  myGrad.colors[0] = mBlack;
  myGrad.colors[1] = mRed;
  myGrad.colors[2] = mYellow;
  myGrad.colors[3] = mOrange;

  static uint32_t prevTime;

  // двигаем пламя
  if (millis() - prevTime > 40) {
    prevTime = millis();

    for (int i = 0; i < LED_COUNT; i++) {
      strip.leds[i] = myGrad.get(inoise8(i * 50, count), 255);
    }
    count += 20;
    strip.show();
  }
}

void fireEffectBlue(int brithness) {
  strip.setBrightness(brithness);
  mGradient<4> myGrad;
  myGrad.colors[0] = mBlack;
  myGrad.colors[1] = mPurple;
  myGrad.colors[2] = mNavy;
  myGrad.colors[3] = mWhite;

  static uint32_t prevTime;

  // двигаем пламя
  if (millis() - prevTime > 40) {
    prevTime = millis();

    for (int i = 0; i < LED_COUNT; i++) {
      strip.leds[i] = myGrad.get(inoise8(i * 50, count), 255);
    }
    count += 20;
    strip.show();
  }
}
