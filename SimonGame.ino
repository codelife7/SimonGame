
#include <LedBtnControl.h>
int speakerOut = 8;    // Set up speaker on digital pin 7
const byte nLedBtn = 4;
LedBtnControl LB[nLedBtn] = {LedBtnControl(13, 7), LedBtnControl(12, 6), LedBtnControl(11, 5), LedBtnControl(10, 4)};

byte currPair = 0;
int timer = 1000;
int level = 1;

void setup() {
  Serial.begin(9600);

  tone(speakerOut, 500, 5000);
  // Diagnostics testing
  for (currPair = 0; currPair < nLedBtn ; currPair++)
  {
    LB[currPair].on();
    delay(300);
    LB[currPair].off();
  }

}

bool randomBool()
{
  int i = random (10 * level) % 2;
  if (i == 1)
    return true;
  if (i == 0)
    return false;
}

void loop() {

  int array[level] = 0;

  for (currPair = 0; currPair <= level ; currPair++)
  { if (randomBool())
    {
      LB[currPair].blink(timer, 1);
      array[currPair] = 1;
    }
    else
    {
      array[currPair] = 0;
    }
  }


  Serial.println(randomBool());
  for (currPair = 0; currPair < nLedBtn ; currPair++)
  { if (LB[currPair].getBtnState())
      LB[currPair].blink(timer, 1);
  }
}
