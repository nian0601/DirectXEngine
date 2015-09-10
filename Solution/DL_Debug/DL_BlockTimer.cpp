#include "DL_BlockTimer.h"

#include <Windows.h>
#include "DL_Debug.h"

#include <sstream>

namespace DL_Debug
{
	DL_BlockTimer::DL_BlockTimer(const std::string& aName)
		: myName(aName)
	{
		LARGE_INTEGER largeInteger;
		QueryPerformanceFrequency(&largeInteger);
		myFrequency = largeInteger.QuadPart;

		LARGE_INTEGER current;
		QueryPerformanceCounter(&current);

		myStartTime = current.QuadPart * 1000000 / myFrequency;
	}


	DL_BlockTimer::~DL_BlockTimer()
	{
		LARGE_INTEGER current;
		QueryPerformanceCounter(&current);

		unsigned long long final = (current.QuadPart * 1000000 / myFrequency) - myStartTime;

		int ms = static_cast<int>(final / 1000);

		std::stringstream output;
		output << myName << " took " << ms << " ms." << std::endl;

		OutputDebugStringA(output.str().c_str());
		//DL_DEBUG("%s took %d ms", myName.c_str(), ms);
	}
}
