
#ifndef NAND_H_INCLUDED
#define NAND_H_INCLUDED
#include <linux/mtd/mtd.h>
#include <linux/mtd/nand.h>
#include <linux/mtd/partitions.h>
#include <common.h>
#include "regs.h"
#include "reg_addr.h"

/** Register defination **/

#define NAND_SYS_CLK_NAME	  "clk81"
#define NAND_CYCLE_DELAY	  65
#define NAND_BOOT_NAME		  "nandboot"
#define NAND_NORMAL_NAME	  "nandnormal"
#define NAND_MULTI_NAME		  "nandmulti"

#define NAND_CONVERST_ADDR	  0xa0000000
#define M2_BOOT_WRITE_SIZE	  		0x600
#define M2_BOOT_COPY_NUM	  		4
#define M2_BOOT_PAGES_PER_COPY	 	256

#define NFC_BASE			  CBUS_REG_ADDR(NAND_CMD)
#define NFC_OFF_CMD           ((NAND_CMD -NAND_CMD)<<2)
#define NFC_OFF_CFG           ((NAND_CFG -NAND_CMD)<<2)
#define NFC_OFF_DADR          ((NAND_DADR-NAND_CMD)<<2)
#define NFC_OFF_IADR          ((NAND_IADR-NAND_CMD)<<2)
#define NFC_OFF_BUF           ((NAND_BUF -NAND_CMD)<<2)
#define NFC_OFF_INFO          ((NAND_INFO-NAND_CMD)<<2)
#define NFC_OFF_DC            ((NAND_DC  -NAND_CMD)<<2)
#define NFC_OFF_ADR           ((NAND_ADR -NAND_CMD)<<2)
#define NFC_OFF_DL            ((NAND_DL  -NAND_CMD)<<2)
#define NFC_OFF_DH            ((NAND_DH  -NAND_CMD)<<2)

#define PER_INFO_BYTE		  sizeof(int)
/*
   Common Nand Read Flow
*/
#define CE0         (0xe<<10)
#define CE1         (0xd<<10)
#define CE2         (0xb<<10)
#define CE3         (0x7<<10)
#define CE_NOT_SEL  (0xf<<10)

#define CLE         (0x5<<14)
#define ALE         (0x6<<14)
#define DWR         (0x4<<14)
#define DRD         (0x8<<14)
#define IDLE        (0xc<<14)
#define RB          (0x10<<14)
#define STANDBY     (0xf<<10)

#define ADL  (0xc<<16)
#define ADH  (0xd<<16)
#define AIL  (0xe<<16)
#define AIH  (0xf<<16)
#define M2N  (0x4<<17)
#define N2M  (0x5<<17)
/**
    Nand Flash Controller (M1)
    Global Macros
*/
/**
   Config Group
*/
#define NFC_SET_TIMING(mode,cycles,adjust)    WRITE_CBUS_REG_BITS(NAND_CFG,((cycles)|((adjust&0xf)<<10)|((mode&7)<<5)),0,14)
#define NFC_SET_DMA_MODE(is_apb,spare_only)   WRITE_CBUS_REG_BITS(NAND_CFG,((spare_only<<1)|(is_apb)),14,2)

/**
    CMD relative Macros
    Shortage word . NFCC
*/
#define NFC_CMD_IDLE(ce,time)          ((ce)|IDLE|(time&0x3ff))
#define NFC_CMD_CLE(ce,cmd  )          ((ce)|CLE |(cmd &0x0ff))
#define NFC_CMD_ALE(ce,addr )          ((ce)|ALE |(addr&0x0ff))
#define NFC_CMD_RB(ce,time  )          ((ce)|RB  |(time&0x3ff))
#define NFC_CMD_STANDBY(time)          (STANDBY  |(time&0x3ff))
#define NFC_CMD_ADL(addr)              (ADL     |(addr&0xffff))
#define NFC_CMD_ADH(addr)              (ADH|((addr>>16)&0xffff))
#define NFC_CMD_AIL(addr)              (AIL     |(addr&0xffff))
#define NFC_CMD_AIH(addr)              (AIH|((addr>>16)&0xffff))
#define NFC_CMD_M2N(size,ecc)          (M2N |ecc|(size&0x3fff))
#define NFC_CMD_N2M(size,ecc)          (N2M |ecc|(size&0x3fff))
#define NFC_CMD_DWR(data)              (DWR     |(data&0xff  ))
#define NFC_CMD_DRD(    )              (DRD                   )

/**
    Alias for CMD
*/
#define NFC_CMD_D_ADR(addr)         NFC_CMD_ADL(addr),NFC_CMD_ADH(addr)   
#define NFC_CMD_I_ADR(addr)         NFC_CMD_ADI(addr),NFC_CMD_ADI(addr)   


/**
    Register Operation and Controller Status 
*/
#define NFC_SEND_CMD(cmd)           (WRITE_CBUS_REG(NAND_CMD,cmd))
#define NFC_READ_INFO()             (READ_CBUS_REG(NAND_CMD))
/** ECC defination(M1) */
#define NAND_ECC_NONE             (0x0<<14)
#define NAND_ECC_REV0             (0x1<<14)
#define NAND_ECC_REV1             (0x2<<14)
#define NAND_ECC_REV2             (0x3<<14)
#define NAND_ECC_BCH9             (0x4<<14)
#define NAND_ECC_BCH8             (0x5<<14)
#define NAND_ECC_BCH12            (0x6<<14)
#define NAND_ECC_BCH16            (0x7<<14)
#define NAND_ECC_BCH24            (0x4<<14)

#define NAND_ECC_BCH8_1K          (0x2)
#define NAND_ECC_BCH16_1K         (0x3)
#define NAND_ECC_BCH24_1K         (0x4)
#define NAND_ECC_BCH30_1K 		  (0x5)
#define NAND_ECC_BCH40_1K 		  (0x6)
#define NAND_ECC_BCH60_1K 		  (0x7)
#define NAND_ECC_BCH_SHORT		  (0x8)
/**
    Cmd FIFO control
*/
#define NFC_CMD_FIFO_GO()               (WRITE_CBUS_REG(NAND_CMD,(1<<30)))
#define NFC_CMD_FIFO_RESET()            (WRITE_CBUS_REG(NAND_CMD,(1<<31)))
/**
    ADDR operations
*/
#define NFC_SET_DADDR(a)         (WRITE_CBUS_REG(NAND_DADR,(unsigned)a))
#define NFC_SET_IADDR(a)         (WRITE_CBUS_REG(NAND_IADR,(unsigned)a))

/**
    Send command directly
*/
#define NFC_SEND_CMD_IDLE(ce,time)          NFC_SEND_CMD((ce)|IDLE|(time&0x3ff))
#define NFC_SEND_CMD_CLE(ce,cmd  )          NFC_SEND_CMD((ce)|CLE |(cmd &0x0ff))
#define NFC_SEND_CMD_ALE(ce,addr )          NFC_SEND_CMD((ce)|ALE |(addr&0x0ff))
#define NFC_SEND_CMD_RB(ce,time  )          NFC_SEND_CMD((ce)|RB  |(time&0x3ff))
#define NFC_SEND_CMD_STANDBY(time)          NFC_SEND_CMD(STANDBY  |(time&0x3ff))
#define NFC_SEND_CMD_ADL(addr)              NFC_SEND_CMD(ADL     |(addr&0xffff))
#define NFC_SEND_CMD_ADH(addr)              NFC_SEND_CMD(ADH|((addr>>16)&0xffff))
#define NFC_SEND_CMD_AIL(addr)              NFC_SEND_CMD(AIL     |(addr&0xffff))
#define NFC_SEND_CMD_AIH(addr)              NFC_SEND_CMD(AIH|((addr>>16)&0xffff))
//#define NFC_SEND_CMD_M2N(size,ecc)          NFC_SEND_CMD(M2N |ecc|(size&0x3fff))
//#define NFC_SEND_CMD_N2M(size,ecc)          NFC_SEND_CMD(N2M |ecc|(size&0x3fff))
#define NFC_SEND_CMD_DWR(data)              NFC_SEND_CMD(DWR     |(data&0xff  ))
#define NFC_SEND_CMD_DRD(    )              NFC_SEND_CMD(DRD                   )

static inline void  NFC_SEND_CMD_M2N(unsigned len, unsigned bch_mode) 
{
	if (bch_mode == NAND_ECC_NONE)
	{
		NFC_SEND_CMD(M2N | bch_mode | len);
	}
	else if (bch_mode == NAND_ECC_BCH24)
	{
		NFC_SEND_CMD(M2N | bch_mode | ((len / 1024) & 0x3f));
	}
	else
	{
		NFC_SEND_CMD(M2N | bch_mode | ((len / 512) & 0x3f));
	}
}

static inline void  NFC_SEND_CMD_N2M(unsigned len, unsigned bch_mode)
{
	if (bch_mode == NAND_ECC_NONE)
	{
		NFC_SEND_CMD(N2M | bch_mode | len);
	}
	else if (bch_mode == NAND_ECC_BCH24)
	{
		NFC_SEND_CMD(N2M | bch_mode | ((len / 1024) & 0x3f));
	}
	else
	{
		NFC_SEND_CMD(N2M | bch_mode | ((len / 512) & 0x3f));
	}
}

/**
    Cmd Info Macros
*/
#define NFC_INFO_GET()                      (READ_CBUS_REG(NAND_CMD))
#define NFC_CMDFIFO_SIZE()                  ((NFC_INFO_GET()>>20)&0x1f)
#define NFC_CHECEK_RB_TIMEOUT()             ((NFC_INFO_GET()>>25)&0x1)
#define NFC_GET_RB_STATUS(ce)               (((NFC_INFO_GET()>>26)&((~(ce>>10))&0xf))&0xf)
typedef unsigned    t_nfc_info;

#define NAND_INFO_DONE(a)         (((a)>>31)&1)
#define NAND_ECC_ENABLE(a)        (((a)>>30)&1)
#define NAND_ECC_FAIL(a)          (((a)>>29)&1)
#define NAND_ECC_CNT(a)           (((a)>>24)&0x1f)
#define NAND_INFO_DATA_2INFO(a)         ((a)&0xffff)
#define NAND_INFO_DATA_1INFO(a)         ((a)&0xff)


#define NFC_SET_SPARE_ONLY()			(SET_CBUS_REG_MASK(NAND_CFG,1<<15))
#define NFC_CLEAR_SPARE_ONLY()			(CLEAR_CBUS_REG_MASK(NAND_CFG,1<<15))
#define NFC_GET_BUF() 					READ_CBUS_REG(NAND_BUF)
#define NFC_GET_CFG() 					READ_CBUS_REG(NAND_CFG)
#define NFC_SET_CFG(val) 			    WRITE_CBUS_REG(NAND_CFG,(unsigned)val)


struct aml_nand_chip;
typedef unsigned  t_nf_ce;

typedef unsigned  t_ecc_mode;
#define NFC_GET_CE_CODE(a)				(((~(1<<(((unsigned)a)&3)))&0xf)<<10)

#define AML_NORMAL						0
#define AML_MULTI_CHIP					1
#define AML_MULTI_CHIP_SHARE_RB			2
#define AML_CHIP_NONE_RB				4
#define AML_INTERLEAVING_MODE			8

#define AML_NAND_CE0         			0xe
#define AML_NAND_CE1         			0xd
#define AML_NAND_CE2         			0xb
#define AML_NAND_CE3         			0x7

#define AML_BADBLK_POS					0
#define NAND_ECC_UNIT_SIZE				512
#define NAND_ECC_UNIT_1KSIZE			1024
#define NAND_ECC_UNIT_SHORT			    384

#define NAND_BCH9_ECC_SIZE				15
#define NAND_BCH8_ECC_SIZE				14
#define NAND_BCH12_ECC_SIZE				20
#define NAND_BCH16_ECC_SIZE				26
#define NAND_BCH8_1K_ECC_SIZE			14
#define NAND_BCH16_1K_ECC_SIZE			28
#define NAND_BCH24_1K_ECC_SIZE			42
#define NAND_BCH30_1K_ECC_SIZE			54
#define NAND_BCH40_1K_ECC_SIZE			70
#define NAND_BCH60_1K_ECC_SIZE			106

#define NAND_ECC_OPTIONS_MASK			0x0000000f
#define NAND_PLANE_OPTIONS_MASK			0x000000f0
#define NAND_TIMING_OPTIONS_MASK		0x00000f00
#define NAND_BUSW_OPTIONS_MASK			0x0000f000
#define NAND_INTERLEAVING_OPTIONS_MASK	0x000f0000
#define NAND_ECC_SOFT_MODE				0x00000000
#define NAND_ECC_SHORT_MODE				0x00000001
#define NAND_ECC_BCH9_MODE				0x00000002
#define NAND_ECC_BCH8_MODE				0x00000003
#define NAND_ECC_BCH12_MODE				0x00000004
#define NAND_ECC_BCH16_MODE				0x00000005
#define NAND_ECC_BCH8_1K_MODE			0x00000006
#define NAND_ECC_BCH16_1K_MODE			0x00000007
#define NAND_ECC_BCH24_1K_MODE			0x00000008
#define NAND_ECC_BCH30_1K_MODE			0x00000009
#define NAND_ECC_BCH40_1K_MODE			0x0000000a
#define NAND_ECC_BCH60_1K_MODE			0x0000000b
#define NAND_TWO_PLANE_MODE				0x00000010
#define NAND_TIMING_MODE0				0x00000000
#define NAND_TIMING_MODE1				0x00000100
#define NAND_TIMING_MODE2				0x00000200
#define NAND_TIMING_MODE3				0x00000300
#define NAND_TIMING_MODE4				0x00000400
#define NAND_TIMING_MODE5				0x00000500
#define NAND_INTERLEAVING_MODE			0x00010000

#define DEFAULT_T_REA					40
#define DEFAULT_T_RHOH					0
#define NAND_DEFAULT_OPTIONS			(NAND_TIMING_MODE5 | NAND_ECC_BCH8_MODE)

#define AML_NAND_BUSY_TIMEOUT			0x40000
#define AML_DMA_BUSY_TIMEOUT			0x100000
#define MAX_ID_LEN						8

#define NAND_CMD_PLANE2_READ_START		0x06
#define NAND_CMD_TWOPLANE_PREVIOS_READ	0x60
#define NAND_CMD_TWOPLANE_READ1			0x5a
#define NAND_CMD_TWOPLANE_READ2			0xa5
#define NAND_CMD_TWOPLANE_WRITE2_MICRO	0x80
#define NAND_CMD_TWOPLANE_WRITE2		0x81
#define NAND_CMD_DUMMY_PROGRAM			0x11
#define NAND_CMD_ERASE1_END				0xd1
#define NAND_CMD_MULTI_CHIP_STATUS		0x78
#define NAND_CMD_SET_FEATURES			0xEF
#define NAND_CMD_GET_FEATURES			0xEE
#define ONFI_TIMING_ADDR				0x01

#define MAX_CHIP_NUM		4
#define USER_BYTE_NUM		4

#define NAND_STATUS_READY_MULTI			0x20

#define NAND_BLOCK_GOOD					0
#define NAND_BLOCK_BAD					1
#define NAND_MINI_PART_SIZE				0x800000
#define NAND_MINI_PART_NUM				4
#define MAX_BAD_BLK_NUM					500
#define MAX_MTD_PART_NUM				16
#define MAX_MTD_PART_NAME_LEN			24
#define ENV_NAND_MAGIC					"envx"
#define BBT_HEAD_MAGIC					"bbts"
#define BBT_TAIL_MAGIC					"bbte"
#define MTD_PART_MAGIC					"anpt"

#define NAND_SYS_PART_SIZE				0x10000000

struct aml_nand_flash_dev {
	char *name;
	u8 id[MAX_ID_LEN];
	unsigned pagesize;
	unsigned chipsize;
	unsigned erasesize;
	unsigned oobsize;
	unsigned internal_chipnr;
	unsigned T_REA;
	unsigned T_RHOH;
	u8 onfi_mode;
	unsigned options;
};

struct aml_nand_part_info {
	char mtd_part_magic[4];
	char mtd_part_name[MAX_MTD_PART_NAME_LEN];
	uint64_t size;
	uint64_t offset;
	u_int32_t mask_flags;
};

struct aml_nand_bbt_info {
	char bbt_head_magic[4];
	int16_t nand_bbt[MAX_BAD_BLK_NUM];
	struct aml_nand_part_info aml_nand_part[MAX_MTD_PART_NUM];
	char bbt_tail_magic[4];
};

struct env_valid_node_t {
	int16_t  ec;
	int16_t	phy_blk_addr;
	int16_t	phy_page_addr;
	int timestamp;
};

struct env_free_node_t {
	int16_t  ec;
	int16_t	phy_blk_addr;
	int dirty_flag;
	struct env_free_node_t *next;
};

struct env_oobinfo_t {
	char name[4];
    int16_t  ec;
    unsigned        timestamp: 15;
    unsigned       status_page: 1;
};

struct aml_nandenv_info_t {
	struct mtd_info *mtd;
	struct env_valid_node_t *env_valid_node;
	struct env_free_node_t *env_free_node;
	u_char env_valid;
	u_char env_init;
	u_char part_num_before_sys;
	struct aml_nand_bbt_info nand_bbt_info;
};

struct aml_nand_bch_desc{
    char * name;
    unsigned bch_mode;
    unsigned bch_unit_size;
    unsigned bch_bytes;
    unsigned user_byte_mode;
};

struct aml_nand_chip {
	/* mtd info */
	u8 mfr_type;
	unsigned onfi_mode;
	unsigned T_REA;
	unsigned T_RHOH;
	unsigned options;
	unsigned page_size;
	unsigned block_size;
	unsigned oob_size;
	unsigned virtual_page_size;
	unsigned virtual_block_size;
	u8 plane_num;
	u8 chip_num;
	u8 internal_chipnr;
	unsigned internal_page_nums;

	unsigned internal_chip_shift;
	unsigned bch_mode;
	u8 user_byte_mode;
	u8 ops_mode;
	u8 cached_prog_status;
	u8 max_bch_mode;
	unsigned chip_enable[MAX_CHIP_NUM];
	unsigned rb_enable[MAX_CHIP_NUM];
	unsigned chip_selected;
	unsigned rb_received;
	unsigned valid_chip[MAX_CHIP_NUM];
	unsigned page_addr;
	unsigned char *aml_nand_data_buf;
	unsigned int *user_info_buf;
	int8_t *block_status;

	struct mtd_info			mtd;
	struct nand_chip		chip;
	struct aml_nandenv_info_t *aml_nandenv_info;
	struct aml_nand_bch_desc 	*bch_desc;

	/* platform info */
	struct aml_nand_platform	*platform;

	/* device info */
	struct device			*device;

	//plateform operation function
	void	(*aml_nand_hw_init)(struct aml_nand_chip *aml_chip);
	void	(*aml_nand_adjust_timing)(struct aml_nand_chip *aml_chip);
	int		(*aml_nand_options_confirm)(struct aml_nand_chip *aml_chip);
	void 	(*aml_nand_cmd_ctrl)(struct aml_nand_chip *aml_chip, int cmd,  unsigned int ctrl);
	void	(*aml_nand_select_chip)(struct aml_nand_chip *aml_chip, int chipnr);
	void	(*aml_nand_write_byte)(struct aml_nand_chip *aml_chip, uint8_t data);
	void	(*aml_nand_get_user_byte)(struct aml_nand_chip *aml_chip, unsigned char *oob_buf, int byte_num);
	void	(*aml_nand_set_user_byte)(struct aml_nand_chip *aml_chip, unsigned char *oob_buf, int byte_num);
	void	(*aml_nand_command)(struct aml_nand_chip *aml_chip, unsigned command, int column, int page_addr, int chipnr);
	int		(*aml_nand_wait_devready)(struct aml_nand_chip *aml_chip, int chipnr);
	int		(*aml_nand_dma_read)(struct aml_nand_chip *aml_chip, unsigned char *buf, int len, unsigned bch_mode);
	int		(*aml_nand_dma_write)(struct aml_nand_chip *aml_chip, unsigned char *buf, int len, unsigned bch_mode);
	int		(*aml_nand_hwecc_correct)(struct aml_nand_chip *aml_chip, unsigned char *buf, unsigned size, unsigned char *oob_buf);
};

struct aml_nand_platform {
		struct aml_nand_flash_dev *nand_flash_dev;
		char *name;
		unsigned chip_enable_pad;
		unsigned ready_busy_pad;

         unsigned int          T_REA;
         unsigned int          T_RHOH;

		 struct aml_nand_chip  *aml_chip;
         struct platform_nand_data platform_nand_data;
};

struct aml_nand_device {
	struct aml_nand_platform *aml_nand_platform;
	u8 dev_num;
};

/*
 * Conversion functions
 */
static inline struct aml_nand_chip *mtd_to_nand_chip(struct mtd_info *mtd)
{
	return container_of(mtd, struct aml_nand_chip, mtd);
}

static void inline  nand_get_chip(void )
{
	CLEAR_CBUS_REG_MASK(PERIPHS_PIN_MUX_2,( (1<<7) | (1<<8) | (1<<9) | (1<<10) | (1<<11) | (1<<12)));
	SET_CBUS_REG_MASK(PERIPHS_PIN_MUX_5, 0x1f5);
}

static void inline nand_release_chip(void)
{	
	CLEAR_CBUS_REG_MASK(PERIPHS_PIN_MUX_5, 0x7fff);
	CLEAR_CBUS_REG_MASK(PERIPHS_PIN_MUX_2,( (1<<7) | (1<<8) | (1<<9) | (1<<10) | (1<<11) | (1<<12)));
}

extern int aml_nand_init(struct aml_nand_chip *aml_chip);

#ifdef CONFIG_PARAMETER_PAGE
struct parameter_page{
/*0~31 byte: Revision information and features block*/
	unsigned char signature[4];
	unsigned short ver;
	unsigned short feature;
	unsigned short opt_commd;
	unsigned short reserve0;
	unsigned short ex_para_page_len;
	unsigned char num_para_page;
	unsigned char reserve1[17];
/*32~79 byte: Manufacturer information block*/
	unsigned char dev_manu[12];
	unsigned char dev_model[20];
	unsigned char JEDEC_manu_ID;
	unsigned short date_code;
	unsigned char reserve2[13];
/*80~127 byte: Memory organization block*/
	unsigned int data_bytes_perpage;
	unsigned short spare_bytes_perpage;
	unsigned int data_bytes_perpartial;//obsolete
	unsigned short spare_bytes_perpartial;//obsolete
	unsigned int pages_perblk;
	unsigned int blks_perLUN;
	unsigned char num_LUN;
	unsigned char num_addr_cycle;//4-7: column addr cycles; 0-3: row addr cycles
	unsigned char bits_percell;
	unsigned short max_badblk_perLUN;
	unsigned short blk_edurce;
	unsigned char g_v_blk_begin;//Guaranteed valid blocks at beginning of target
	unsigned short blk_edurce_g_v_blk;
	unsigned char progm_perpage;
	unsigned char prt_prog_att;//obsolete
	unsigned char bits_ECC_corretable;
	unsigned char bits_intleav_addr;//0-3: number of interleaved address bits
	unsigned char intleav_op_attr;/*6-7 Reserved (0)
									5 1 = lower bit XNOR block address restriction
									4 1 = read cache supported
									3 Address restrictions for cache operations
									2 1 = program cache supported
									1 1 = no block address restrictions
									0 Overlapped / concurrent interleaving support
	                              */
	unsigned char reserve3[13];
/*128~163 byte: Electrical parameters block*/
	unsigned char max_io_pin;
	unsigned short asy_time_mode;/*6-15 Reserved (0)
									5 1 = supports timing mode 5
									4 1 = supports timing mode 4
									3 1 = supports timing mode 3
									2 1 = supports timing mode 2
									1 1 = supports timing mode 1
									0 1 = supports timing mode 0, shall be 1
	                             */
	unsigned short asy_prog_cach_time_mode;//obsolete
	unsigned short Tprog;//Maximum page program time (Ts)
	unsigned short Tbers;//Maximum block erase time (Ts)
	unsigned short Tr;//Maximum page read time (Ts)
	unsigned short Tccs;//Minimum change column setup time (ns)
	unsigned short src_syn_time_mode;/* 6-15 Reserved (0)
									 5 1 = supports timing mode 5
									 4 1 = supports timing mode 4
									 3 1 = supports timing mode 3
									 2 1 = supports timing mode 2
									 1 1 = supports timing mode 1
									 0 1 = supports timing mode 0
	                                 */
	unsigned char src_syn_feature;/*3-7 Reserved (0)
									2 1 = device supports CLK stopped for data input
									1 1 = typical capacitance values present
									0 tCAD value to use
	                              */
	unsigned short CLK_input_pin;
	unsigned short IO_pin;
	unsigned short input_pin;
	unsigned char max_input_pin;
	unsigned char dr_strgth;
	unsigned short Tir;//Maximum interleaved page read time (Ts)
	unsigned short Tadl;//Program page register clear enhancement tADL value (ns)
	unsigned char reserve4[8];
/*164~255 byte: Vendor block*/
	unsigned short vd_ver;
	unsigned char vd_spec[88];
	unsigned short int_CRC;
/*256~ byte: Redundant Parameter Pages*/

}__attribute__ ((__packed__));
#endif

#endif // NAND_H_INCLUDED

