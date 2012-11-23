// Stuart Milne August 2012
#ifndef __STU_ITIMER_H_
#define __STU_ITIMER_H_

#include <windows.h>
namespace stu
{
	class ITimer
	{
	public:
		virtual void Reset() = 0;
		virtual void Start() = 0;
		virtual void Stop() = 0;

		virtual float GetElapsedTimeSeconds() const = 0;
		virtual float GetElapsedTimeMilliseconds() const = 0;
	};//end class ITimer

	class IHighResolutionTimer
	{
		virtual double GetElapsedTimeSecondsAsDouble() const = 0;
		virtual double GetElapsedTimeMillisecondsAsDouble() const = 0;
	};
}// end namespace stu
#endif