/* platform header */
/*
 * (C) Copyright 2010 Amlogic, Inc
 *
 * Victor Wan, victor.wan@amlogic.com, 
 * 2010-03-24 @ Shanghai
 *
 */

 #ifndef __PLATFORM_H__
 #define __PLATFORM_H__
#include "romboot.h"
//#pragma Offwarn(88)  /* disable "Expression has no side-effects" print debug info*/
	
/* ISO C Standard Definitions */
#if 0
typedef char               int8_t;
typedef	 short int          int16_t;
typedef	 int                int32_t;
typedef	 long               int64_t;
typedef	 unsigned char      uint8_t;
typedef	 unsigned short int uint16_t;
typedef	 unsigned int       uint32_t;
typedef	 unsigned long      uint64_t;
#endif
typedef unsigned   char    u8_t;
typedef signed     char    s8_t;
typedef unsigned   short   u16_t;
typedef signed     short   s16_t;
typedef unsigned   int     u32_t;
typedef signed     int     s32_t;
typedef unsigned   long    u64_t;
typedef signed     long    s64_t;

/* Linux definitions */
typedef u8_t     __u8;
typedef s8_t     __s8;
typedef u16_t    __u16;
typedef s16_t    __s16;
typedef u32_t    __u32;
typedef s32_t    __s32;
typedef u64_t    __u64;
typedef s64_t    __s64;

typedef u8_t     u_int8_t;
typedef u16_t    u_int16_t;
typedef u32_t    u_int32_t;
typedef u64_t    u_int64_t;

typedef s8_t     s8;
typedef u8_t     u8;
typedef s16_t    s16;
typedef u16_t    u16;
typedef s32_t    s32;
typedef u32_t    u32;
typedef u64_t    u64;

typedef __u16    __le16;
typedef __u16    __be16;
typedef __u32    __le32;
typedef __u32    __be32;
typedef __u64    __le64;
typedef __u64    __be64;


/* A3,CS2,M3,M6 chip, PORT_A is OTG, work as ROM Boot port */
#ifdef __USE_PORT_B
#define PORT_REG_OFFSET   0x80000
#else
#define PORT_REG_OFFSET   0
#endif


/*M6*/
#define DWC_REG_BASE  			(0xC9040000 + PORT_REG_OFFSET)
#define PERI_BASE_ADDR               0xC1100000
#define ISABASE                      0x01000000

#define PREI_USB_PHY_A_REG_BASE       0xC1108400  //0x2100
#define PREI_USB_PHY_B_REG_BASE       0xC1108420	//0X2108

#ifdef __USE_PORT_B
#define PREI_USB_PHY_REG_BASE   PREI_USB_PHY_B_REG_BASE
#else
#define PREI_USB_PHY_REG_BASE   PREI_USB_PHY_A_REG_BASE
#endif

typedef struct usb_aml_regs {
    volatile uint32_t config; 
    volatile uint32_t ctrl;      
    volatile uint32_t endp_intr; 
    volatile uint32_t adp_bc;    
    volatile uint32_t dbg_uart;  
    volatile uint32_t test;
    volatile uint32_t tune; 
} usb_aml_regs_t;

// ------------------------------------------------------------
// M6 CLK_SEL: These bits select the source for the 12Mhz: 
// 0 = Oscillator pad input (when a 24Mhz XTAL is used), 
// 1 = Oscillator pad input divided by 2 (when a 24mhz XTAL is used),
// 2 = ddr_pll_clk 
// 3 = mpll_clk_out0
// 4 = mpll_clk_out1
// 5 = mpll_clk_out2
// 6 = fclk_div2
// 7 = fclk_div3
#define USB_CLK_SEL_XTAL				0
#define USB_CLK_SEL_XTAL_DIV_2	1
#define USB_CLK_SEL_DDR_PLL			2
#define USB_CLK_SEL_MPLL_OUT0		3
#define USB_CLK_SEL_MPLL_OUT1		4
#define USB_CLK_SEL_MPLL_OUT2		5
#define USB_CLK_SEL_FCLK_DIV2		6
#define USB_CLK_SEL_FCLK_DIV3		7
typedef union usb_config_data {
    /** raw register data */
    uint32_t d32;
    /** register bits */
    struct {
        unsigned clk_en:1;
        unsigned clk_sel:3;
        unsigned clk_div:7;
        unsigned reserved:20;
        unsigned test_trig:1;
    } b;
} usb_config_data_t;

typedef union usb_ctrl_data {
    /** raw register data */
    uint32_t d32;
    /** register bits */
    struct {
        unsigned soft_prst:1;
        unsigned soft_hreset:1;
        unsigned ss_scaledown_mode:2;
        unsigned clk_det_rst:1;
        unsigned intr_sel:1;
        unsigned reserved:2;
        unsigned clk_detected:1;
        unsigned sof_sent_rcvd_tgl:1; 
        unsigned sof_toggle_out:1; 
        unsigned not_used:4;
        unsigned por:1;
        unsigned sleepm:1;
        unsigned txbitstuffennh:1;
        unsigned txbitstuffenn:1;
        unsigned commononn:1; 
        unsigned refclksel:2; 
        unsigned fsel:3;
        unsigned portreset:1;
        unsigned thread_id:6;
    } b;
} usb_ctrl_data_t;

typedef union usb_endp_intr_data {
    /** raw register data */
    uint32_t d32;
    /** register bits */
    struct {
        unsigned int0:1;
        unsigned int1:1;
        unsigned int2:1;
        unsigned int3:1;
        unsigned int4:1;
        unsigned int5:1;
        unsigned int6:1;
        unsigned int7:1;
        unsigned int8:1;
        unsigned int9:1;
        unsigned int10:1;
        unsigned int11:1;
        unsigned int12:1;
        unsigned int13:1;
        unsigned int14:1;
        unsigned int15:1;
        unsigned int16:1;
        unsigned int17:1;
        unsigned int18:1;
        unsigned int19:1;
        unsigned int20:1;
        unsigned int21:1;
        unsigned int22:1;
        unsigned int23:1;
        unsigned int24:1;
        unsigned int25:1;
        unsigned int26:1;
        unsigned int27:1;
        unsigned int28:1;
        unsigned int29:1;
        unsigned int30:1;
        unsigned int31:1;
    } b;
} usb_endp_intr_data_t;

typedef union usb_adp_bc_data {
    /** raw register data */
    uint32_t d32;
    /** register bits */
    struct {
	unsigned vbusvldextsel:1;
	unsigned vbusvldext:1; 
	unsigned otgdisable:1; 
	unsigned idpullup:1; 
	unsigned drvvbus:1; 
	unsigned adp_prb_en:1; 
	unsigned adp_dischrg:1; 
	unsigned adp_chrg:1; 
	unsigned sessend:1;
	unsigned device_sess_vld:1;
	unsigned bvalid:1;  
	unsigned avalid:1; 
	unsigned iddig:1; 
	unsigned vbusvalid:1; 
	unsigned adp_probe:1;
	unsigned adp_sense:1;  
	unsigned aca_enable:1;
	unsigned dcd_enable:1; 
	unsigned vdatdetenb:1; 
	unsigned vdatsrcenb:1; 
	unsigned chrgsel:1; 
	unsigned chg_det:1;
	unsigned aca_pin_range_c:1; 
	unsigned aca_pin_range_b:1; 
	unsigned aca_pin_range_a:1; 
	unsigned aca_pin_gnd:1; 
	unsigned aca_pin_float:1; 
	unsigned not_used:5;
    } b;
} usb_adp_bc_data_t;

typedef union usb_dbg_uart_data {
    /** raw register data */
    uint32_t d32;
    /** register bits */
    struct {
	unsigned bypass_sel:1;
	unsigned bypass_dm_en:1; 
	unsigned bypass_dp_en:1; 
	unsigned bypass_dm_data:1; 
	unsigned bypass_dp_data:1; 
	unsigned fsv_minus:1;
	unsigned fsv_plus:1;
	unsigned burn_in_test:1; 
	unsigned loopbackenb:1; 
	unsigned set_iddq:1; 
	unsigned ate_reset:1; 
	unsigned reserved:4;
	unsigned not_used:17; 
    } b;
} usb_dbg_uart_data_t;

typedef union phy_test_data {
    /** raw register data */
    uint32_t d32;
    /** register bits */
    struct {
	unsigned data_in:4;
	unsigned data_in_en:4;
	unsigned addr:4;
	unsigned data_out_sel:1; 
	unsigned clk:1;
	unsigned vatestenb:2;
	unsigned data_out:4;
	unsigned not_used:12;
    } b;
} phy_test_data_t;

typedef union phy_tune_data {
    /** raw register data */
    uint32_t d32;
    /** register bits */
    struct {
	unsigned tx_res_tune:2;
	unsigned tx_hsxv_tune:2; 
	unsigned tx_vref_tune:4; 
	unsigned tx_rise_tune:2;
	unsigned tx_preemp_pulse_tune:1; 
	unsigned tx_preemp_amp_tune:2; 
	unsigned tx_fsls_tune:4;
	unsigned sqrx_tune:3; 
	unsigned otg_tune:3;
	unsigned comp_dis_tune:3;  
	unsigned not_used:6;
    } b;
} phy_tune_data_t;


/***********************************************/
#define WRITE_PERI_REG(reg, val) *(volatile unsigned *)(PERI_BASE_ADDR + ((reg)<<2)) = (val)
#define READ_PERI_REG(reg) (*(volatile unsigned *)(PERI_BASE_ADDR + ((reg)<<2)))

#define CLEAR_PERIPHS_REG_BITS(reg, mask) WRITE_PERI_REG(reg, (READ_PERI_REG(reg)&(~(mask))))
#define SET_PERIPHS_REG_BITS(reg, mask)   WRITE_PERI_REG(reg, (READ_PERI_REG(reg)|(mask)))

#define WRITE_ISA_REG(reg, val) *(volatile unsigned *)(ISABASE + (reg)) = (val)
#define READ_ISA_REG(reg) (*(volatile unsigned *)(ISABASE + (reg)))

#define CLEAR_ISA_REG_MASK(reg, mask) WRITE_ISA_REG(reg, (READ_ISA_REG(reg)&(~mask)))
#define SET_ISA_REG_MASK(reg, mask)   WRITE_ISA_REG(reg, (READ_ISA_REG(reg)|(mask)))
/***********************************************/






#define flush_cpu_cache() 


#define dwc_write_reg32(x, v) 	(*(volatile uint32_t *)(x + DWC_REG_BASE))=v
#define dwc_read_reg32(x) (*(volatile uint32_t*)(x + DWC_REG_BASE))
// void dwc_modify_reg32( volatile uint32_t *_reg, const uint32_t _clear_mask, const uint32_t _set_mask) 
#define dwc_modify_reg32(x, c, s) 	(*(volatile uint32_t *)(x + DWC_REG_BASE))=( ((dwc_read_reg32(x)) & (~c)) | (s))

#define __constant_cpu_to_le16(x) (x)
#define __constant_cpu_to_le32(x) (x)
#define cpu_to_le16(x)      (x)
#define  cpu_to_le32(x)     (x)
#define le16_to_cpu(x)      (x)
#define le32_to_cpu(x)      (x)
#define get_unaligned_16(ptr)				(((__u8 *)ptr)[0] | (((__u8 *)ptr)[1]<<8))
#define get_unaligned_32(ptr)				(((__u8 *)ptr)[0] | (((__u8 *)ptr)[1]<<8) | (((__u8 *)ptr)[2]<<16) | (((__u8 *)ptr)[3]<<24))
#define get_unaligned(ptr)				(((__u8 *)ptr)[0] | (((__u8 *)ptr)[1]<<8) | (((__u8 *)ptr)[2]<<16) | (((__u8 *)ptr)[3]<<24))

#ifndef max
#define max(a, b)	(((a) > (b))? (a): (b))
#endif
#ifndef min
#define min(a, b)	(((a) < (b))? (a): (b))
#endif

#define EXT_CLOCK	1
#define INT_CLOCK	0


// 32 bit TimerE, 1us
#define USB_ROM_CONN_TIMEOUT		5*1000*1000  //us (5s timeout,)


/* Meet with spec */
/* Meet with spec */
#define USB_ROM_VER_MAJOR	0
#define USB_ROM_VER_MINOR	8				// SPEC Version
#define USB_ROM_STAGE_MAJOR	0
#define USB_ROM_STAGE_MINOR	0		// IPL = 0,	SPL = 8, TPL = 16

#if 1
#define PRINTF(x...)	do{}while(0)
#else
#define PRINTF(x...) printf(x)
#endif

#define ERR(x...) PRINTF(x)
#define DBG(x...) PRINTF(x)
#define USB_ERR(x...)	PRINTF(x)
#define USB_DBG(x...) PRINTF(x)


//static void set_usb_phy_config(int cfg);
void usb_parameter_init();

#define wait_ms(a) udelay(a*1000);
int update_utime();
int get_utime();
//int chip_watchdog(void);
#define usb_memcpy(dst,src,len) memcpy((void*)dst,(void*)src,(unsigned)len)
#define usb_memcpy_32bits(dst,src,len) memcpy((void*)dst,(void*)src,(unsigned)len)


#endif
