#ifndef TIMER_HPP_
#define TIMER_HPP_

namespace OsirisI {
	namespace Utilities {
		class Timer {
		private:
			double countsPerSecond;
			double frameTime;
			int frameCount;
			int fps;
			__int64 frameTimeOld;
			__int64 counterStart;


		public:
			Timer();
			void StartTimer();
			double GetTime();
			double GetFrameTime();
		};
	}
}

#endif /* TIMER_HPP_ */
