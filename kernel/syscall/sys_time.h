#pragma once
#include "types.h"
#include "sys/sys_types.h"

__BEGIN_DECLS

struct timespec {
    time_t tv_sec;
    long   tv_nsec;
};

int sys_nanosleep(timespec const* req, timespec *rem);

int sys_clock_gettime(clockid_t id, timespec *tm);
int sys_clock_getres(clockid_t id, timespec *tm);

__END_DECLS
