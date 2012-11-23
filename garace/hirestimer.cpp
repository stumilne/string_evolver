#include "hirestimer.h"


stu::HiResTimer::HiResTimer()
 : mSecondsPerCount(0.0), mBaseCount(0), 
	mPausedCount(0), mPreviousCount(0), mCurrentCount(0), mIsStopped(false)
{
	// Find the granularity of the performance counter
	__int64 countsPerSec;
	QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSec);

	// Check for performance counter support, not implemented as we can assume modern x86 CPU's will support this
	// TODO - fall back if not supported, ie. GetTickCount or get_time
	//if((double)countsPerSec == 0) { PerformanceCounterNotSupportedOnHardwareNotify(); }

	// Calculate the time in seconds that each step of the performance counter represents
	mSecondsPerCount = 1.0 / static_cast<double>(countsPerSec);
}

float stu::HiResTimer::GetElapsedTimeSeconds() const
{
	return static_cast<float>(GetElapsedTimeSecondsAsDouble());
}

float stu::HiResTimer::GetElapsedTimeMilliseconds() const
{
	return static_cast<float>(GetElapsedTimeMillisecondsAsDouble());
}

double stu::HiResTimer::GetElapsedTimeSecondsAsDouble() const
{
	// If we are stopped, do not count the time that has passed since we stopped.
	//
	// ----*---------------*------------------------------*------> time
	//  mBaseTime       mStopTime                      mCurrTime

	if( mIsStopped )
	{
		return (mStopCount - mBaseCount) * mSecondsPerCount;
	}

	// The distance mCurrTime - mBaseTime includes paused time,
	// which we do not want to count.  To correct this, we can subtract 
	// the paused time from mCurrTime:  
	//
	//  (mCurrTime - mPausedTime) - mBaseTime 
	//
	//                     |<-------d------->|
	// ----*---------------*-----------------*------------*------> time
	//  mBaseTime       mStopTime        startTime     mCurrTime

	else
	{
		return ((mCurrentCount-mPausedCount)-mBaseCount ) * mSecondsPerCount;
	}
}

double stu::HiResTimer::GetElapsedTimeMillisecondsAsDouble() const
{
	return GetElapsedTimeSecondsAsDouble() * 1000;
}

void stu::HiResTimer::Reset()
{
	__int64 currTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

	mBaseCount = currTime;
	mPreviousCount = currTime;
	mStopCount = 0;
	mIsStopped  = false;
}

void stu::HiResTimer::Start()
{
	// Accumulate the time elapsed between stop and start pairs.
	//
	//                     |<-------d------->|
	// ----*---------------*-----------------*------------> time
	//  mBaseTime       mStopTime        startTime     

	// Can only start a stopped timer
	if( mIsStopped )
	{
		__int64 startTime;
		QueryPerformanceCounter((LARGE_INTEGER*)&startTime);
		// Keep track of how long we've been paused, cumulative since last call to Reset()
		mPausedCount += (startTime - mStopCount);	

		mPreviousCount = startTime;	// Begin counting again
		mStopCount = 0;
		mIsStopped  = false;
	}
}

void stu::HiResTimer::Stop()
{
	if( !mIsStopped )
	{
		__int64 currTime;
		QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

		mStopCount = currTime;	// Start counting stop time
		mIsStopped  = true;
	}
}
