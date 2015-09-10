#pragma once
#include <string>

namespace DL_Debug
{
	class DL_BlockTimer
	{
	public:
		DL_BlockTimer(const std::string& aName);
		~DL_BlockTimer();

	private:
		std::string myName;
		unsigned long long myFrequency;
		unsigned long long myStartTime;
	};
}

