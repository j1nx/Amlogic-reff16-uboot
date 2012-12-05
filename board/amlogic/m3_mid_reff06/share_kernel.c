#include <share_kernel.h>

kernel_cmd_t get_kernel_cmd(void)
{
    printf("kernel_args = 0x%08x\n", kernel_args);
    if(kernel_args == 0x05050505)
    {
        kernel_args = 0;
        return RECOVERY_BOOT;
    }
    else if(kernel_args == 0x06060606)
    {
        kernel_args = 0;
        return SYSTEM_REBOOT;
    }
    return NORMAL_BOOT;
}
