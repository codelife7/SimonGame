
#include <LedBtnControl.h>
int speakerOut = 8;    // Set up speaker on digital pin 7
const byte nLedBtn = 4;  //number of LEDs and corrsponding buttons used
LedBtnControl LB[nLedBtn] = {LedBtnControl(13, 7), LedBtnControl(12, 6), LedBtnControl(11, 5), LedBtnControl(10, 4)}; // array to perform actions on LEDs and buttons

byte currPair = 0;    // used to traverse array declared above
int timer = 1000;   
int level = 1;      // level has a relation with number of LEDs blinking per round

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

//generates a random number and performs % to get a random bool
bool randomBool()
{
  int i = random (10 * level) % 2; 
  if (i == 1)
    return true;
  if (i == 0)
    return false;
}

void loop() {

  int array[level]; //used to store LED pattern created by randomBool()

  for (currPair = 0; currPair <= level ; currPair++)
  { if (randomBool())   //if random bool is 1 blink LED and store it in pattern array
    {
      LB[currPair].blink(timer, 1);
      array[currPair] = 1;
    }
    else                 //else just store 0 in the pattern array
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
