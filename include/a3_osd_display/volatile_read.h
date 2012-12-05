/*******************************************************************
 * 
 *  Copyright C 2006 by Amlogic, Inc. All Rights Reserved.
 *
 *  Description: ASM macro to access memory bypass ARC's data cache
 *
 * Created: 8/8/2006 10:27AM
 *
 *******************************************************************/

#ifndef VOLATILE_READ_H
#define VOLATILE_READ_H

#ifdef __HIGHC__


//#define IO_WRITE32(val,addr)  ({__asm__ __volatile__ ("st.di %0,[%1]"::"r"((val)),"r"((addr))); })
//#define IO_READ32(addr) 		({unsigned int val=0;__asm__ __volatile__ ("ld.di %0,[%1]":"=&r"((val)):"r"((addr)));val;})
//
////#define IO_WRITE8(val,addr) ({__asm__ __volatile__ ("stb.di %0,[%1]"::"r"((val)),"r"((addr))); })
//#define IO_READ8(addr)	({unsigned int val=0;__asm__ __volatile__ ("ldb.di %0,[%1]":"=&r"((val)):"r"((addr)));val;})
_Asm unsigned char READ_VOLATILE_UINT8_(unsigned addr)
{
    % reg addr
    ldb.di %r0, [addr]
    add  %r0, %r0, 0
}

_Asm char READ_VOLATILE_INT8_(unsigned addr)
{
    % reg addr
    ldb.x.di %r0, [addr]
    add  %r0, %r0, 0
}

_Asm unsigned short READ_VOLATILE_UINT16_(unsigned addr)
{
    % reg addr
    ldw.di %r0, [addr]
    add  %r0, %r0, 0
}

_Asm short READ_VOLATILE_INT16_(unsigned addr)
{
    % reg addr
    ldw.x.di %r0, [addr]
    add  %r0, %r0, 0
}

_Asm unsigned READ_VOLATILE_UINT32_(unsigned addr)
{
    % reg addr
    ld.di %r0, [addr]
    add  %r0, %r0, 0
}

_Asm int READ_VOLATILE_INT32_(unsigned addr)
{
    % reg addr
    ld.di %r0, [addr]
    add  %r0, %r0, 0
}

#define READ_VOLATILE_UINT8(addr)   READ_VOLATILE_UINT8_((unsigned)(addr))
#define READ_VOLATILE_INT8(addr)    READ_VOLATILE_INT8_((unsigned)(addr))
#define READ_VOLATILE_UINT16(addr)  READ_VOLATILE_UINT16_((unsigned)(addr))
#define READ_VOLATILE_INT16(addr)   READ_VOLATILE_INT16_((unsigned)(addr))
#define READ_VOLATILE_UINT32(addr)  READ_VOLATILE_UINT32_((unsigned)(addr))
#define READ_VOLATILE_INT32(addr)   READ_VOLATILE_INT32_((unsigned)(addr))

#else

#define READ_VOLATILE_UINT8(addr)   *(volatile unsigned char *)(addr)
#define READ_VOLATILE_INT8(addr)    *(volatile char *)(addr)
#define READ_VOLATILE_UINT16(addr)  *(volatile unsigned short *)(addr)
#define READ_VOLATILE_INT16(addr)   *(volatile short *)(addr)
#define READ_VOLATILE_UINT32(addr)  *(volatile unsigned *)(addr)
#define READ_VOLATILE_INT32(addr)   *(volatile int *)(addr)

#endif

#endif /* VOLATILE_READ_H */
