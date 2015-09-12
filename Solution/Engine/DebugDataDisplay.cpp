#include "stdafx.h"
#include "DebugDataDisplay.h"

#include "Text.h"
#include "Font.h"
#include <sstream>
#include <Psapi.h>


DebugDataDisplay::DebugDataDisplay()
	: myFunctionTimersStartPos(0.f, 0.f)
	, myMemUsageStartPos(1000.f, 0.f)
	, myCPUUSageStartPos(1000.f, -30.f)
	, myFrameTimeStartPos(1000.f, -60.f)
	, myTextScale(0.75f)
{
}


DebugDataDisplay::~DebugDataDisplay()
{
}

void DebugDataDisplay::Init()
{
	Font* font = new Font();
	font->Init("Data/resources/font/font.dds");

	myText = new Text();
	myText->Init(font);

	LARGE_INTEGER largeInteger;
	QueryPerformanceFrequency(&largeInteger);
	myFrequency = largeInteger.QuadPart;

	GetCPUUsage(&myPrevSysKernel, &myPrevSysUser, &myPrevProcKernel, &myPrevProcUser, true);
}

void DebugDataDisplay::AddFunctionTime(const std::string& aFunc, const unsigned long long aStartTime, const unsigned long long aEndTime)
{
	FunctionData& it = myFunctionTimers[aFunc];

	it.myStart = aStartTime;
	it.myEnd = aEndTime;
	++it.myHitCount;
	it.myEnabled = true;
}

void DebugDataDisplay::Render(Camera& aCamera, const float aDeltaTime)
{
	if (myDisplayStatuses.at(eDisplays::FUNCTION_TIMERS))
	{
		RenderFunctionTimers(aCamera);
	}

	if (myDisplayStatuses.at(eDisplays::MEMORY_USAGE))
	{
		RenderMemoryUsage(aCamera);
	}

	if (myDisplayStatuses.at(eDisplays::CPU_USAGE))
	{
		RenderCPUUsage(aCamera);
	}

	if (myDisplayStatuses.at(eDisplays::FRAME_TIME))
	{
		RenderFrameTime(aCamera, aDeltaTime);
	}
}

void DebugDataDisplay::RenderFunctionTimers(Camera& aCamera)
{
	TIME_FUNCTION

	CU::Vector2<float> drawPos = myFunctionTimersStartPos;

	for (auto it = myFunctionTimers.begin(); it != myFunctionTimers.end(); ++it)
	{
		if (it->second.myEnabled == true)
		{
			float ms = static_cast<float>(((it->second.myEnd * 1000000 / myFrequency) - (it->second.myStart * 1000000 / myFrequency)) / 1000.f);
			std::stringstream ss;
			ss << it->first << ": " << ms << " ms";
			if (it->second.myHitCount > 1)
				ss << " (" << it->second.myHitCount << " calls)";

			ss << std::endl;

			myText->UpdateSentence(ss.str().c_str(), drawPos.x, drawPos.y, myTextScale);
			myText->Render(aCamera);
			drawPos.y -= 30.f;

			FUNCTION_TIMER_LOG("%s", ss.str().c_str());

			ss.clear();

			it->second.myEnabled = false;
			it->second.myHitCount = 0;
		}
	}
}

void DebugDataDisplay::RenderMemoryUsage(Camera& aCamera)
{
	TIME_FUNCTION

	std::stringstream ss;
	ss << "Memory: " << GetMemoryUsageMB() << "mb" << std::endl;

	myText->UpdateSentence(ss.str().c_str(), myMemUsageStartPos.x, myMemUsageStartPos.y, myTextScale);
	myText->Render(aCamera);
}

void DebugDataDisplay::RenderCPUUsage(Camera& aCamera)
{
	TIME_FUNCTION

	std::stringstream ss;
	ss << "CPU: " << GetCPUUsage(&myPrevSysKernel, &myPrevSysUser, &myPrevProcKernel, &myPrevProcUser) << "%" << std::endl;

	myText->UpdateSentence(ss.str().c_str(), myCPUUSageStartPos.x, myCPUUSageStartPos.y, myTextScale);
	myText->Render(aCamera);
}

void DebugDataDisplay::RenderFrameTime(Camera& aCamera, const float aDeltaTime)
{
	TIME_FUNCTION

	int FPS = 1.f / aDeltaTime;
	std::stringstream fpsSS;
	fpsSS << "FPS: " << FPS << std::endl;

	myText->UpdateSentence(fpsSS.str().c_str(), myFrameTimeStartPos.x, myFrameTimeStartPos.y, myTextScale);
	myText->Render(aCamera);


	float frameTimeMS = aDeltaTime * 1000.f;
	std::stringstream frameTimeSS;
	frameTimeSS << "FrameTime: " << frameTimeMS << "ms" << std::endl;

	myText->UpdateSentence(frameTimeSS.str().c_str(), myFrameTimeStartPos.x, myFrameTimeStartPos.y - 30.f, myTextScale);
	myText->Render(aCamera);
}


int DebugDataDisplay::GetMemoryUsageMB()
{
	PROCESS_MEMORY_COUNTERS memCounter;
	BOOL result = GetProcessMemoryInfo(GetCurrentProcess(),
		&memCounter,
		sizeof(memCounter));

	//int memUsed = memCounter.WorkingSetSize / 1024;
	int memUsedMb = memCounter.WorkingSetSize / 1024 / 1024;

	return memUsedMb;
}

ULONGLONG DebugDataDisplay::FixCPUTimings(const FILETIME &a, const FILETIME &b)
{
	LARGE_INTEGER la, lb;
	la.LowPart = a.dwLowDateTime;
	la.HighPart = a.dwHighDateTime;
	lb.LowPart = b.dwLowDateTime;
	lb.HighPart = b.dwHighDateTime;

	return la.QuadPart - lb.QuadPart;
}

float DebugDataDisplay::GetCPUUsage(FILETIME *prevSysKernel, FILETIME *prevSysUser,
	FILETIME *prevProcKernel, FILETIME *prevProcUser,
	bool firstRun)
{
	FILETIME sysIdle, sysKernel, sysUser;
	FILETIME procCreation, procExit, procKernel, procUser;

	if (!GetSystemTimes(&sysIdle, &sysKernel, &sysUser) ||
		!GetProcessTimes(GetCurrentProcess(), &procCreation, &procExit, &procKernel, &procUser))
	{
		// can't get time info so return
		return -1.;
	}

	// check for first call
	if (firstRun)
	{
		// save time info before return
		prevSysKernel->dwLowDateTime = sysKernel.dwLowDateTime;
		prevSysKernel->dwHighDateTime = sysKernel.dwHighDateTime;

		prevSysUser->dwLowDateTime = sysUser.dwLowDateTime;
		prevSysUser->dwHighDateTime = sysUser.dwHighDateTime;

		prevProcKernel->dwLowDateTime = procKernel.dwLowDateTime;
		prevProcKernel->dwHighDateTime = procKernel.dwHighDateTime;

		prevProcUser->dwLowDateTime = procUser.dwLowDateTime;
		prevProcUser->dwHighDateTime = procUser.dwHighDateTime;

		return -1.;
	}

	ULONGLONG sysKernelDiff = FixCPUTimings(sysKernel, *prevSysKernel);
	ULONGLONG sysUserDiff = FixCPUTimings(sysUser, *prevSysUser);

	ULONGLONG procKernelDiff = FixCPUTimings(procKernel, *prevProcKernel);
	ULONGLONG procUserDiff = FixCPUTimings(procUser, *prevProcUser);

	ULONGLONG sysTotal = sysKernelDiff + sysUserDiff;
	ULONGLONG procTotal = procKernelDiff + procUserDiff;

	return (float)((100.0 * procTotal) / sysTotal);
}