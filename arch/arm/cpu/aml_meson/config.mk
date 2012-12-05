
CROSS_COMPILE=arm-none-eabi-
ARM_CPU=cortex-a9
PLATFORM_CPPFLAGS += -mcpu=$(ARM_CPU) -mthumb-interwork -O -ffixed-r8 -mno-long-calls  -Wall 
TEXT_BASE=0x80000000