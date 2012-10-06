#ifndef TIMER_H
#define TIMER_H
#include "config.h"

class CTimer
{
public: 
	CTimer(); 
	~CTimer(); 
	CTimer(string timerName); 

private: 
	string m_name; 
	clock_t start; 
	clock_t end; 
};
#endif //TIMER_H 