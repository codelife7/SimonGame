#ifndef LedBtnControl_H
#define LedBtnControl_H

#include "Arduino.h" 

class LedBtnControl{
  public:
    LedBtnControl(uint8_t ledPin, uint8_t btnPin);
	bool getLedState();
	bool getBtnState();
    void on();
	void off();
	void toggle();
	void blink(unsigned int time, byte times=1);

  private:
	bool status;
	uint8_t LedPin;
	uint8_t BtnPin;

};



#endif
