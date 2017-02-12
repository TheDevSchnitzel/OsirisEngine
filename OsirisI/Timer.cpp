#include "stdafx.h"

namespace OsirisI {
	namespace Utilities {
		Timer::Timer() {
			countsPerSecond = 0.0;
			counterStart = 0;
			frameCount = 0;
			fps = 0;
			frameTimeOld = 0;
			frameTime = 0.0;
		}

		void Timer::StartTimer() {
			LARGE_INTEGER frequencyCount;
			QueryPerformanceFrequency(&frequencyCount);

			countsPerSecond = double(frequencyCount.QuadPart);

			QueryPerformanceCounter(&frequencyCount);
			counterStart = frequencyCount.QuadPart;
		}

		double Timer::GetTime() {
			LARGE_INTEGER currentTime;
			QueryPerformanceCounter(&currentTime);
			return double(currentTime.QuadPart - counterStart) / countsPerSecond;
		}

		double Timer::GetFrameTime() {
			LARGE_INTEGER currentTime;
			__int64 tickCount;
			QueryPerformanceCounter(&currentTime);

			tickCount = currentTime.QuadPart - frameTimeOld;
			frameTimeOld = currentTime.QuadPart;

			if (tickCount < 0.0f)
				tickCount = 0.0f;

			return float(tickCount) / countsPerSecond;
		}
	}
}