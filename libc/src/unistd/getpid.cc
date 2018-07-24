#include <unistd.h>
#include <sys/syscall.h>
#include <sys/syscall_num.h>
#include <sys/types.h>
#include <errno.h>

pid_t getpid(void)
{
    long status = syscall0(SYS_getpid);

    if (status >= 0)
        return status;

    errno = -status;

    return -1;
}
