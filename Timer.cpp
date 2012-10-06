#include "Timer.h"

CTimer::CTimer()
{
	m_name = ""; 
	start = clock(); 
	end = 0; 
	cout << "Start Function <" << m_name << ">..." << endl; 
}

CTimer::CTimer(string timerName)
{
	assert(timerName.length() > 0); 

	m_name = timerName; 
	start = clock();
	cout << "Start Function <" << m_name << ">..." << endl; 
}

CTimer::~CTimer()
{
	end = clock(); 
	cout << "Finish Function <" << m_name << ">..." << endl; 
	cout << "Total time: "  <<  (end - start) / (double)CLOCKS_PER_SEC << endl; 
}
