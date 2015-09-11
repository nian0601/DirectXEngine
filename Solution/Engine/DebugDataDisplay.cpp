#include "stdafx.h"
#include "DebugDataDisplay.h"

#include "Text.h"
#include "Font.h"
#include <sstream>


DebugDataDisplay::DebugDataDisplay()
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
}

void DebugDataDisplay::AddFunctionTime(const std::string& aFunc, const unsigned long long aStartTime, const unsigned long long aEndTime)
{
	FunctionData& it = myFunctionTimers[aFunc];

	it.myStart = aStartTime;
	it.myEnd = aEndTime;
	++it.myHitCount;
	it.myEnabled = true;
}

void DebugDataDisplay::Render(Camera& aCamera)
{
	int drawY = 0;

	for (auto it = myFunctionTimers.begin(); it != myFunctionTimers.end(); ++it)
	{
		if (it->second.myEnabled == true)
		{
			int ms = static_cast<int>(((it->second.myEnd * 1000000 / myFrequency) - (it->second.myStart * 1000000 / myFrequency)) / 1000);

			std::stringstream ss;
			ss << it->first << ": " << ms << " ms";
			if (it->second.myHitCount > 1)
				ss << " (" << it->second.myHitCount << " calls)";

			ss << std::endl;

			myText->UpdateSentence(ss.str().c_str(), 0.f, drawY);
			myText->Render(aCamera);
			drawY -= 30.f;

			FUNCTION_TIMER_LOG("%s", ss.str().c_str());

			ss.clear();

			it->second.myEnabled = false;
			it->second.myHitCount = 0;
		}
	}

	//myFunctionTimers.clear();
}