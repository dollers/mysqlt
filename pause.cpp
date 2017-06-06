#include "pause.h"
#include <time.h>

Pause::Pause(float time) {
	Pause::time = time;
}

Pause::~Pause() {

}


bool Pause::pause() {
	long zt1=clock();  
	long zt2;  
	while(1)  
	{
		zt2=clock();  
		if(((float)(zt2-zt1)/CLOCKS_PER_SEC)>this->time)
			return 1;  
		else {
			//gotoxy(23,1);
			//printf("%-9.2f",((float)(zt2-zt1)/CLOCKS_PER_SEC));
		}  
	}
	return 0;
}


void Pause::setTime(float time) {
	this->time = time;
}


bool Pause::pause(float time) {
	this->time = time;
	this->pause();
}