// Stuart Milne August 2012

// This high resolution timer uses the high resolution performance counter on the CPU
// This is accessed using the QueryPerformanceCounter/Frequency windows api calls.
// Note that some hardware (VERY unlikely in x86) may not have a performance counter onboard, in this case the timer will fail
// There is no fallback in this case
// See the comments in the constructor for how to begin addressing this limitation if it is a problem

// The performance counter operates as an incremental counter, that is it increments by 1 uniformly a specific
// number of times per second. By querying this frequency we can determine the time-step for a single increment
// Timing is then a case of determining the how many counts have passed in a period and multiplying this by the time-step
// Note all internal calculations are performed with double precision, downcasting to float is only performed on client code queries
// and does not affect internal timekeeping.
#ifndef __STU_HIRES_TIMER_H_
#define __STU_HIRES_TIMER_H_

#include "itimer.h"

namespace stu
{
	class HiResTimer : public ITimer, public IHighResolutionTimer
	{
	public:
		HiResTimer();

		// GetElapsed returns the time since the last call to Reset()
		float GetElapsedTimeSeconds() const;
		float GetElapsedTimeMilliseconds() const;
		double GetElapsedTimeSecondsAsDouble() const;
		double GetElapsedTimeMillisecondsAsDouble() const;

		void Reset(); // Call before message loop.
		void Start(); // Call to unpause a paused timer.
		void Stop();  // Call to pause timer
		
	protected:
		// Double precision internally
		double mSecondsPerCount;

		// Internals for performance counter which is a 64bit integer
		__int64 mBaseCount;
		__int64 mPausedCount;
		__int64 mStopCount;
		__int64 mPreviousCount;
		__int64 mCurrentCount;

		bool mIsStopped;
	};
}//end namespace stu
#endif