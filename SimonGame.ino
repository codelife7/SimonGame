
#include <LedBtnControl.h>
int speakerOut = 8;    // Set up speaker on digital pin 7
const byte nLedBtn = 4;
LedBtnControl LB[nLedBtn] = {LedBtnControl(13,7),LedBtnControl(12,6),LedBtnControl(11,5),LedBtnControl(10,4)};

byte currPair = 0;
int timer = 1000;
int level = 1;
void setup(){

 
  tone(speakerOut, 500, 5000);
  // Diagnostics testing
  for(currPair = 0; currPair < nLedBtn ; currPair++)
  {
    LB[currPair].on();
    delay(300);
    LB[currPair].off();
    
    }
    
}

void loop(){

int randNum = random(10*level);
for (i = 0; i < 32; ++i) {  // assuming a 32 bit int
    array[i] = (theValue >> i) & 1
}

for(currPair = 0; currPair < nLedBtn ; currPair++)
  {    if(LB[currPair].getBtnState())
    LB[currPair].blink(timer,1);
  }
  }
