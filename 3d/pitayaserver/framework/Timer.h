#ifndef _TIMER_H
#define _TIMER_H

#include "Defines.h"
#include "Set.h"

#ifdef WIN32
#include <windows.h>
#include <time.h>
#else
#include <sys/time.h>
#include <sys/timeb.h>
#endif

#ifdef WIN32
inline uint32 getMSTime() { return GetTickCount(); }
#else
inline time_t getMSTime()
{    
	//struct timeval tv;
	//struct timezone tz;
	//gettimeofday( &tv, &tz );
	//return (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
	struct timespec time1 = {0, 0};
	clock_gettime(CLOCK_MONOTONIC, &time1);
	return (time1.tv_sec * 1000) + (time1.tv_nsec / 1000000);
}
#endif

#define INIT_PROCESS_TIMER \
uint32_t interval = 0;\
uint32_t before = 0;                                                                                           
#define PROCESS_TIMER_START \
interval = getMSTime();                                                                     
                                                                                            
#define PROCESS_TIMER_END_COUNT(t, key)  \
before = interval;\
interval = getMSTime();\
if (interval - before >= t)\
   IME_LOG("[===%s before %u total consume timer %u ms ===]", key, before, interval - before);

#define PROCESS_TIMER_END(key) PROCESS_TIMER_END_COUNT(CSet::ExecutionTimeInterval, key);

struct IntervalTimer
{
    public:

        IntervalTimer()
            : _interval(0), _current(0)
        {
        }

        void Update(time_t diff)
        {
            _current += diff;
            if (_current < 0)
                _current = 0;
        }

        bool Passed()
        {
            return _current >= _interval;
        }

        void Reset()
        {
            if (_current >= _interval)
                _current -= _interval;
        }

        void SetCurrent(time_t current)
        {
            _current = current;
        }

        void SetInterval(time_t interval)
        {
            _interval = interval;
        }

        time_t GetInterval() const
        {
            return _interval;
        }

        time_t GetCurrent() const
        {
            return _current;
        }

    private:

        time_t _interval;
        time_t _current;
};

struct TimeTracker
{
    public:

        TimeTracker(time_t expiry)
            : i_expiryTime(expiry)
        {
        }

        void Update(time_t diff)
        {
            i_expiryTime -= diff;
        }

        bool Passed() const
        {
            return i_expiryTime <= 0;
        }

        void Reset(time_t interval)
        {
            i_expiryTime = interval;
        }

        time_t GetExpiry() const
        {
            return i_expiryTime;
        }

    private:

        time_t i_expiryTime;
};

struct TimeTrackerSmall
{
    public:

        TimeTrackerSmall(uint32 expiry)
            : i_expiryTime(expiry)
        {
        }

        void Update(int32 diff)
        {
            i_expiryTime -= diff;
        }

        bool Passed() const
        {
            return i_expiryTime <= 0;
        }

        void Reset(uint32 interval)
        {
            i_expiryTime = interval;
        }

        int32 GetExpiry() const
        {
            return i_expiryTime;
        }

    private:

        int32 i_expiryTime;
};

struct PeriodicTimer
{
    public:

        PeriodicTimer(int32 period, int32 start_time)
            : i_period(period), i_expireTime(start_time)
        {
        }

        bool Update(const uint32 &diff)
        {
            if ((i_expireTime -= diff) > 0)
                return false;

            i_expireTime += i_period > int32(diff) ? i_period : diff;
            return true;
        }

        void SetPeriodic(int32 period, int32 start_time)
        {
            i_expireTime = start_time;
            i_period = period;
        }

        // Tracker interface
        void TUpdate(int32 diff) { i_expireTime -= diff; }
        bool TPassed() const { return i_expireTime <= 0; }
        void TReset(int32 diff, int32 period)  { i_expireTime += period > diff ? period : diff; }

    private:

        int32 i_period;
        int32 i_expireTime;
};




#endif
