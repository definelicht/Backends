#ifndef BACKENDS_STOPWATCH_H_
#define BACKENDS_STOPWATCH_H_

#include "common.h"

// OS X compatibility
#ifdef __MACH__
#include <mach/clock.h>
#include <mach/mach.h>
#endif

#include <ctime>

class Stopwatch {

private:

  long long t1;
  long long t2;

public:

  void Start() { t1 = GetTime(); }

  double Stop() {
    t2 = GetTime();
    return Elapsed();
  }

  double Elapsed() const { return InSeconds(t2 - t1); }

private:

  static double InSeconds(const long long seconds) {
    return 1e-9*static_cast<double>(seconds);
  }

  long long GetTime() const {

    struct timespec ts;

#ifdef __MACH__
    // OS X compatibility code taken from
    // http://stackoverflow.com/questions/5167269/
    clock_serv_t cclock;
    mach_timespec_t mts;
    host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock);
    clock_get_time(cclock, &mts);
    mach_port_deallocate(mach_task_self(), cclock);
    ts.tv_sec = mts.tv_sec;
    ts.tv_nsec = mts.tv_nsec;
#else
    clock_gettime(CLOCK_REALTIME, &ts);
#endif

    return static_cast<long long>(1000000000UL)
         * static_cast<long long>(ts.tv_sec)
         + static_cast<long long>(ts.tv_nsec);
  }

};

#endif // BACKENDS_STOPWATCH_H_