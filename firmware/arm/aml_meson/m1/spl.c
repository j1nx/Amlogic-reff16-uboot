#define FIRMWARE_IN_ONE_FILE
#define CONFIG_ENABLE_SPL_DEBUG_ROM 0
#if __CC_ARM
#define CONFIG_ENABLE_MEM_DEVICE_TEST
#define CONFIG_ENABLE_SPL_MORE_CMD 1
#endif
#include <uartpin.c>
#include <pinmux.c>
#include <serial.c>
#include <timer.c>

#include <timming.c>
#include <memtest.c>
#include <ddr.c>
#include <pll.c>
#include <spiwrite.c>
#include <sdio.c>
#include <debug_rom.c>
#include <relocate_init.c>
#include <lowlevel_init.c>

