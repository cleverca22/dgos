#include "printk.h"

int worked(void);

//int takespace_data[16] = { 99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99 };
//int takespace_bss[12];

int volatile bss_val;
int volatile data_val = 148;

int (*text_ptr)(void) = worked;
int volatile *data_ptr = &data_val;
int volatile *bss_ptr = &bss_val;

int dummy(void)
{
    return 40;
}

int worked(void)
{
    printk("worked!\n");
}

int entry(void)
{
    bss_val = 66;

    printk("pointer to bss %s\n", (*bss_ptr != 66) ? "failed" : "worked");
    printk("pointer to data %s\n", (*data_ptr != 148) ? "failed" : "worked");
    printk("text_ptr = %p\n", (void*)text_ptr);
    printk("pointer to text: ");
    text_ptr();
}
