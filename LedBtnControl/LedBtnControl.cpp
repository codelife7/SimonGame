#include "LedBtnControl.h"

LedBtnControl::LedBtnControl(uint8_t ledPin, uint8_t btnPin){
	this->LedPin=ledPin;
	this->BtnPin=btnPin;
	this->status=LOW;
	pinMode(this->LedPin,OUTPUT);
	pinMode(this->BtnPin,INPUT);
}

bool LedBtnControl::getLedState(){ return status; }

void LedBtnControl::on(void){
	digitalWrite(LedPin,HIGH);
	this->status=true;
}

void LedBtnControl::off(void){
	digitalWrite(LedPin,LOW);
	this->status=false;
}
	
void LedBtnControl::toggle(void){
	status ? off() : on();
}

void LedBtnControl::blink(unsigned int time, byte times){
	for (byte i=0; i<times; i++){
		toggle();
		delay(time/2);
		toggle();
		delay(time/2);
	}
}

bool LedBtnControl::getBtnState(){
	return digitalRead(BtnPin) ? true : false;
}

