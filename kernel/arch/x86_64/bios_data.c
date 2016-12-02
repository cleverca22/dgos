#include "bios_data.h"
#include "mm.h"
#include "callout.h"

char *zero_page;

static void biosdata_remap(void *arg)
{
    (void)arg;
    zero_page = mmap(
            0, PAGESIZE, PROT_READ | PROT_WRITE,
            MAP_PHYSICAL, -1, 0);
}

REGISTER_CALLOUT(biosdata_remap, 0, 'V', "000");
