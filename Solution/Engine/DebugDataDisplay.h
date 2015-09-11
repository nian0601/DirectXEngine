#pragma once
#define TIME_FUNCTION Debug_BlockTimer funcTimer__(__FUNCTION__);

#include "Debug_BlockTimer.h"

#include <unordered_map>

class Text;
class Camera;

class DebugDataDisplay
{
public:
	DebugDataDisplay();
	~DebugDataDisplay();

	void Init();

	void AddFunctionTime(const std::string& aFunc, const unsigned long long aStartTime, const unsigned long long aEndTime);

	void Render(Camera& aCamera);

private:
	struct FunctionData
	{
		int myHitCount;
		unsigned long long myStart;
		unsigned long long myEnd;
		bool myEnabled;
	};

	std::unordered_map<std::string, FunctionData> myFunctionTimers;
	Text* myText;
	unsigned long long myFrequency;
};

