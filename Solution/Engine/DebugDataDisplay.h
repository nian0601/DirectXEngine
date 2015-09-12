#pragma once
#define TIME_FUNCTION Debug_BlockTimer funcTimer__(__FUNCTION__);

#include "Debug_BlockTimer.h"

#include <unordered_map>
#include <bitset>
#include <Vector.h>

class Text;
class Camera;

class DebugDataDisplay
{
public:
	DebugDataDisplay();
	~DebugDataDisplay();

	void Init();

	void AddFunctionTime(const std::string& aFunc, const unsigned long long aStartTime, const unsigned long long aEndTime);

	void Render(Camera& aCamera, const float aDeltaTime);

	void ToggleFunctionTimers();
	void ToggleMemoryUsage();
	void ToggleCPUUsage();
	void ToggleFrameTime();

private:
	enum eDisplays
	{
		FUNCTION_TIMERS,
		MEMORY_USAGE,
		CPU_USAGE,
		FRAME_TIME,
		count
	};
	struct FunctionData
	{
		int myHitCount;
		unsigned long long myStart;
		unsigned long long myEnd;
		bool myEnabled;
	};

	void RenderFunctionTimers(Camera& aCamera);
	void RenderMemoryUsage(Camera& aCamera);
	void RenderCPUUsage(Camera& aCamera);
	void RenderFrameTime(Camera& aCamera, const float aDeltaTime);

	int GetMemoryUsageMB();

	ULONGLONG FixCPUTimings(const FILETIME &a, const FILETIME &b);
	float GetCPUUsage(FILETIME *prevSysKernel, FILETIME *prevSysUser,
		FILETIME *prevProcKernel, FILETIME *prevProcUser,
		bool firstRun = false);

	std::unordered_map<std::string, FunctionData> myFunctionTimers;
	Text* myText;
	unsigned long long myFrequency;
	FILETIME myPrevSysKernel;
	FILETIME myPrevSysUser;
	FILETIME myPrevProcKernel;
	FILETIME myPrevProcUser;

	std::bitset<eDisplays::count> myDisplayStatuses;

	CU::Vector2<float> myFunctionTimersStartPos;
	CU::Vector2<float> myMemUsageStartPos;
	CU::Vector2<float> myCPUUSageStartPos;
	CU::Vector2<float> myFrameTimeStartPos;
	float myTextScale;
};

inline void DebugDataDisplay::ToggleFunctionTimers()
{
	myDisplayStatuses[eDisplays::FUNCTION_TIMERS].flip();
}

inline void DebugDataDisplay::ToggleMemoryUsage()
{
	myDisplayStatuses[eDisplays::MEMORY_USAGE].flip();
}

inline void DebugDataDisplay::ToggleCPUUsage()
{
	myDisplayStatuses[eDisplays::CPU_USAGE].flip();
}

inline void DebugDataDisplay::ToggleFrameTime()
{
	myDisplayStatuses[eDisplays::FRAME_TIME].flip();
}

