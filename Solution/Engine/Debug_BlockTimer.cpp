#include "stdafx.h"
#include "Debug_BlockTimer.h"


Debug_BlockTimer::Debug_BlockTimer(const char* aName)
	: myName(aName)
{
	LARGE_INTEGER current;
	QueryPerformanceCounter(&current);

	myStartTime = current.QuadPart;
}


Debug_BlockTimer::~Debug_BlockTimer()
{
	LARGE_INTEGER current;
	QueryPerformanceCounter(&current);

	Engine::GetInstance()->GetDebugDisplay().AddFunctionTime(myName, myStartTime, current.QuadPart);
}
