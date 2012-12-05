
#include <common.h>
//#include <asm/mach-types.h>
#include <asm/arch/memory.h>
#include <asm/arch/am_reg_addr.h>
#include <asm/arch/aml_emac_lan8700.h>
#include <asm/arch/aml_card_sd.h>
#include <asm/arch/pinmux.h>
#include <asm/arch/am_eth_reg.h>
#include <asm/arch/nand.h>

#define creg(a) (*(volatile unsigned*)(0xc1100000 + ((a)<<2)))
#define reg(a) (*(volatile unsigned*)(a))

#define SN7325_ADDR 0xb2

//#define SHOW_PICTURE
//#define NO_IRQ
//#define USE_IRQ
#define stimulus_event
// --------------------------------------------------------
//                     C_Entry
// --------------------------------------------------------

#define HHI_MPEG_CLK_CNTL                          0x105d
#define PERIPHS_PIN_MUX_5                          0x2031

extern const unsigned char pic_data[];

#define I2C_B

#ifdef I2C_A
#define  I2C_CONTROL_REG      (volatile unsigned long *)0xc1108500
#define  I2C_SLAVE_ADDR       (volatile unsigned long *)0xc1108504
#define  I2C_TOKEN_LIST_REG0  (volatile unsigned long *)0xc1108508
#define  I2C_TOKEN_LIST_REG1  (volatile unsigned long *)0xc110850c
#define  I2C_TOKEN_WDATA_REG0 (volatile unsigned long *)0xc1108510
#define  I2C_TOKEN_WDATA_REG1 (volatile unsigned long *)0xc1108514
#define  I2C_TOKEN_RDATA_REG0 (volatile unsigned long *)0xc1108518
#define  I2C_TOKEN_RDATA_REG1 (volatile unsigned long *)0xc110851c
#elif defined I2C_B
#define  I2C_CONTROL_REG      (volatile unsigned long *)0xc1108520
#define  I2C_SLAVE_ADDR       (volatile unsigned long *)0xc1108524
#define  I2C_TOKEN_LIST_REG0  (volatile unsigned long *)0xc1108528
#define  I2C_TOKEN_LIST_REG1  (volatile unsigned long *)0xc110852c
#define  I2C_TOKEN_WDATA_REG0 (volatile unsigned long *)0xc1108530
#define  I2C_TOKEN_WDATA_REG1 (volatile unsigned long *)0xc1108534
#define  I2C_TOKEN_RDATA_REG0 (volatile unsigned long *)0xc1108538
#define  I2C_TOKEN_RDATA_REG1 (volatile unsigned long *)0xc110853c
#endif

#define  I2C_END               0x0
#define  I2C_START             0x1
#define  I2C_SLAVE_ADDR_WRITE  0x2
#define  I2C_SLAVE_ADDR_READ   0x3
#define  I2C_DATA              0x4
#define  I2C_DATA_LAST         0x5
#define  I2C_STOP              0x6

typedef unsigned char ubyte;

void hard_i2c_init(unsigned speed)
{
    unsigned delay = 125;//400K CLK81= 200M

    //    if(speed)
    //        delay = (1800 * 25 / speed) & 0x3ff; //(1875 * 25 / speed) & 0x3ff;

    (*I2C_CONTROL_REG) = ((*I2C_CONTROL_REG) & ~(0x3FF << 12)) | (delay << 12);
}
/*
ubyte hard_i2c_read8(ubyte SlaveAddr, ubyte RegAddr)
{
    // Set the I2C Address
    (*I2C_SLAVE_ADDR) = ((*I2C_SLAVE_ADDR) & ~0xff) | SlaveAddr;
    // Fill the token registers
    (*I2C_TOKEN_LIST_REG0) = (I2C_END  << 24)             |
                             (I2C_DATA_LAST << 20)        |  // Read Data
                             (I2C_SLAVE_ADDR_READ << 16)  |
                             (I2C_START << 12)            |
                             (I2C_DATA << 8)              |  // Read RegAddr
                             (I2C_SLAVE_ADDR_WRITE << 4)  |
                             (I2C_START << 0);

    // Fill the write data registers
    (*I2C_TOKEN_WDATA_REG0) = (RegAddr << 0);
    // Start and Wait
    (*I2C_CONTROL_REG) &= ~(1 << 0);   // Clear the start bit
    (*I2C_CONTROL_REG) |= (1 << 0);   // Set the start bit
    while( (*I2C_CONTROL_REG) & (1 << 2) ) {}

    return( (ubyte)((*I2C_TOKEN_RDATA_REG0) & 0xFF) );
}
*/
void hard_i2c_write8(ubyte SlaveAddr, ubyte RegAddr, ubyte Data)
{
    // Set the I2C Address
    (*I2C_SLAVE_ADDR) = ((*I2C_SLAVE_ADDR) & ~0xff) | SlaveAddr;
    // Fill the token registers
    (*I2C_TOKEN_LIST_REG0) = (I2C_END  << 16)             |
                             (I2C_DATA << 12)             |    // Write Data
                             (I2C_DATA << 8)              |    // Write RegAddr
                             (I2C_SLAVE_ADDR_WRITE << 4)  |
                             (I2C_START << 0);

    // Fill the write data registers
    (*I2C_TOKEN_WDATA_REG0) = (Data << 8) | (RegAddr << 0);
    // Start and Wait
    (*I2C_CONTROL_REG) &= ~(1 << 0);   // Clear the start bit
    (*I2C_CONTROL_REG) |= (1 << 0);   // Set the start bit
    while ((*I2C_CONTROL_REG) & (1 << 2)) {}
}

void power_init()
{
    hard_i2c_init(0);
    reg(0xc11080c8) |= 3 << 8; //GPIOA4-5 I2C MASTER B

    //reset
    creg(0x2010) |= 1 << 27;
    creg(0x200f) &= ~(1 << 27);

    udelay(100000);

    hard_i2c_write8(SN7325_ADDR, 3, 0x7f);
    udelay(20000);
    hard_i2c_write8(SN7325_ADDR, 2, 0x2e);
}
/**
 * Routine: board_init
 * Description: Early hardware init.
 */
int board_init(void)
{
    /*
    @todo implement this function
    */
	DECLARE_GLOBAL_DATA_PTR;
//	gd->bd->bi_arch_number=MACH_TYPE_MESON_6236M;
//	gd->bd->bi_boot_params=BOOT_PARAMS_OFFSET;
//	gd->bd->bi_memstart=PHYS_MEMORY_START;
//	gd->bd->bi_memsize=PHYS_MEMORY_SIZE;
//	gd->bd->bi_flashstart=CONFIG_SYS_FLASH_BASE;
//    gd->bd->bi_flashoffset=0;

	power_init();
#ifdef CONFIG_CMD_KGDB

	kgdb_init();
#endif

	return 0;
}

/*
 * Routine: misc_init_r
 * Description: Init ethernet (done here so udelay works)
 */
int misc_init_r(void)
{
/*
    @todo implement this function
    */

	return 0;
}

/*
 * Routine: set_muxconf_regs
 * Description: Setting up the configuration Mux registers specific to the
 *		hardware. Many pins need to be moved from protect to primary
 *		mode.
 */
void set_muxconf_regs(void)
{
    /*
    @todo implement this function
    */
}

/*
 * Routine: setup_net_chip
 * Description: Setting up the configuration GPMC registers specific to the
 *		Ethernet hardware.
 */
static void setup_net_chip(void)
{
    /*
    @todo implement this function
    */
	
}


/*
  * phyid [in]:
  * reg [in]: physical layer chip id
  * val [in]:
  */
static void phy_reg_wr(int phyid, unsigned int reg, unsigned int val)
{
    unsigned long busy = 0, tmp = 0;
    unsigned int phyaddr;
    unsigned int phyreg;
    unsigned long reg4;

    phyaddr = phyid << ETH_MAC_4_GMII_Addr_PA_P;
    phyreg  = reg << ETH_MAC_4_GMII_Addr_GR_P;

    reg4 = phyaddr | phyreg | ETH_MAC_4_GMII_Addr_CR_100_150 | ETH_MAC_4_GMII_Addr_GW | ETH_MAC_4_GMII_Addr_GB;
    writel(val, ETH_MAC_5_GMII_Data);
    writel(reg4, ETH_MAC_4_GMII_Addr);
    busy = 1;
    while (busy) {
        tmp = readl(ETH_MAC_4_GMII_Addr);
        busy = tmp & 1;
    }
}

/*
  * phyid [in]:
  * reg [in]: physical layer chip id
  * val [out]:
  */
static unsigned int phy_reg_rd(int phyid, unsigned int reg)
{
    unsigned long busy = 0, tmp = 0;
    unsigned int phyaddr;
    unsigned int phyreg;
    unsigned long reg4;

    phyaddr = phyid << ETH_MAC_4_GMII_Addr_PA_P;
    phyreg  = reg << ETH_MAC_4_GMII_Addr_GR_P;
    reg4 = phyaddr | phyreg | ETH_MAC_4_GMII_Addr_CR_100_150 | ETH_MAC_4_GMII_Addr_GB;
    writel(reg4, ETH_MAC_4_GMII_Addr);

    busy = 1;
    while (busy) {
        tmp = readl(ETH_MAC_4_GMII_Addr);
        busy = tmp & 1;
    }

    tmp = readl(ETH_MAC_5_GMII_Data);
    return tmp;
}


static unsigned int detect_phyid(void)
{
    unsigned int testval = 0;
    int i;
    static int phy_id = -1;

    if (phy_id != -1) {
        return phy_id;
    }
    for (i = 0; i < 32; i++) {
        testval = phy_reg_rd(i, PHY_SR);    //read the SR register..
        if (testval != 0x0000 && testval != 0xffff) {
            phy_id = i;
            return phy_id;
        }
    }
    return 0xffff;
}

#define NET_MAX_RESET_TEST  1000
void reset_phy(void)
{
    int i, k, phyad;
    unsigned int val;
    /* reset via i2c 0x59 SN7325 */
    hard_i2c_write8(SN7325_ADDR, 2, 0x2e);
    udelay(100);
    hard_i2c_write8(SN7325_ADDR, 2, 0xae);
    udelay(100);

    /* Set up PHY from sleep to normal mode */
    for (i = 0; i < NET_MAX_RESET_TEST; i++) {
        phyad = detect_phyid();
        if (phyad > 32 || phyad < 0) {
            continue;
        } else {
            debug("detected phy id=%d\n", phyad);
            break;
        }
    }
    val = phy_reg_rd(phyad, PHY_SPMD);
    debug("phy_reg_rd(phyad, PHY_SPMD) =0x%x\n", val);
    val |= (0x7 << 5); //Set up PHY from sleep to normal mode
    phy_reg_wr(phyad, PHY_SPMD, val);
    debug("phy_reg_rd(phyad, PHY_SPMD) =0x%x\n", phy_reg_rd(phyad, PHY_SPMD));
    udelay(100);
}

extern int aml_eth_init(bd_t *bis);
int board_eth_init(bd_t *bis)
{
#define CONFIG_ETHERNET_EXTERNAL_CRSTAL	1
	/*for a3_reference with ethernet*/	
#if defined(CONFIG_ETHERNET_EXTERNAL_CRSTAL)
   	eth_set_pinmux(ETH_BANK0_GPIOC3_C11, ETH_CLK_IN_GPIOC2_REG2_18,0);
	udelay(100);
	eth_clk_set(ETH_CLKSRC_EXTERN_PAD_CLK, get_clk_ethernet_pad(), 50*CLK_1M);
#else
	eth_clk_set(ETH_CLKSRC_SYS_CLK, get_cpu_clk(), 50*CLK_1M);
	udelay(100);
   	eth_set_pinmux(ETH_BANK0_GPIOC3_C11, ETH_CLK_OUT_GPIOC2_REG2_17,0);
#endif
	CLEAR_CBUS_REG_MASK(PREG_ETHERNET_ADDR0, 1);
	SET_CBUS_REG_MASK(PREG_ETHERNET_ADDR0, (1 << 1));
	SET_CBUS_REG_MASK(PREG_ETHERNET_ADDR0, 1);
	udelay(100);
	aml_eth_init(bis);
	return 0;
}
u32 get_board_rev(void)
{
    /*
    @todo implement this function
    */
	return 0x20;
}
static int  sdio_init(unsigned port)
{
//	setbits_le32(P_PREG_CGPIO_EN_N,1<<22);

    return cpu_sdio_init(port);
}
static int  sdio_detect(unsigned port)
{
//	return (readl(P_PREG_CGPIO_I)&(1<<22))?1:0;
    return 0;
}
static void sdio_pwr_prepare(unsigned port)
{
    /// @todo NOT FINISH
	///do nothing here
}
static void sdio_pwr_on(unsigned port)
{
	clrbits_le32(P_PREG_JTAG_GPIO_ADDR,(1<<20));
	clrbits_le32(P_PREG_JTAG_GPIO_ADDR,(1<<16));//test_n
    /// @todo NOT FINISH
}
static void sdio_pwr_off(unsigned port)
{
	setbits_le32(P_PREG_JTAG_GPIO_ADDR,(1<<20));
	clrbits_le32(P_PREG_JTAG_GPIO_ADDR,(1<<16));//test_n

	/// @todo NOT FINISH
}
static void board_mmc_register(unsigned port)
{
    AML_CARD_SD_INFO_t *aml_priv=cpu_sdio_get(port);
    if(aml_priv==NULL)
        return;
    struct mmc *mmc = (struct mmc *)malloc(sizeof(struct mmc));
    strncpy(mmc->name,aml_priv->name,31);
    mmc->priv = aml_priv;
	aml_priv->removed_flag = 1;
	aml_priv->inited_flag = 0;
	aml_priv->sdio_init=sdio_init;
	aml_priv->sdio_detect=sdio_detect;
	aml_priv->sdio_pwr_off=sdio_pwr_off;
	aml_priv->sdio_pwr_on=sdio_pwr_on;
	aml_priv->sdio_pwr_prepare=sdio_pwr_prepare;
	mmc->send_cmd = aml_sd_send_cmd;
	mmc->set_ios = aml_sd_cfg_swth;
	mmc->init = aml_sd_init;
	mmc->rca = 1;
	mmc->voltages = MMC_VDD_33_34;
	mmc->host_caps = MMC_MODE_4BIT | MMC_MODE_HS;
	//mmc->host_caps = MMC_MODE_4BIT;
	mmc->bus_width = 1;
	mmc->clock = 300000;
	mmc->f_min = 200000;
	mmc->f_max = 50000000;
	mmc_register(mmc);
}
int board_mmc_init(bd_t	*bis)
{
	board_mmc_register(SDIO_PORT_A);
//	board_mmc_register(SDIO_PORT_B);
//	board_mmc_register(SDIO_PORT_C);
//	board_mmc_register(SDIO_PORT_B1);
	return 0;
}

/*
 * struct aml_platform_info{
	u32    	mlc_mode;				//small or large 
	u32		ran_mode; 				//def close, for all part
	u32	   	codepart_ecc_mode;		//first pg bch60, this is other
	u32     romcode_ecc_mode;		//this and upper may unify one 
//	u32    	short_mode;
	u32     short_pgsz;				//zero means clode short , value no div 8 , should get realpage size first
//	u32     short_pgnm;	
//	u32     ran_seed;				 // 0xa3 + (src&0xff) is used as seed,				
	u32     ecc_size;					//compat with older code structure
	u32     rbpin_mode;					//may get from romboot , 1 use rb pin 0 no use 
	u32		ce_sel;
	u32  *	info_buf;		
	struct nand_ecclayout * layout;
};
*/

void board_nand_pinmux( unsigned int en_dis )
{
	if(en_dis) {
         set_mio_mux(4, (0x1<<11) | (0xff<<14));
	} else {
		 clear_mio_mux(4, (0x1<<11) | (0xff<<14));
	}
	
	return;	
}

/* every board have one config array */
//struct aml_platform_info aml_chips[2]=
//{
//	[0]=
//	{	.mlc_mode=1,
//		.ran_mode=0,
//		.ecc_mode=NAND_ECC_BCH8_512,
//		.short_pgsz=0,
//		.rbpin_mode=1,
//		.timingset=7|(7<<5)|(0<<10),
//		.ce_sel=CE0
//	},
//	[1]=
//	{	.mlc_mode=1,
//		.ran_mode=0,
//		.ecc_mode=NAND_ECC_BCH60_1024 ,					//NOW SET BCH60 for simple , dont modify it , cause first pg romcode use BCH60
//		.short_pgsz=384,
//		.rbpin_mode=1,
//		.timingset=7|(7<<5)|(0<<10),
//		.ce_sel=CE0
//	},
//};
struct aml_nand_platform aml_nand_mid_platform[] = {
    {
        .name = NAND_BOOT_NAME,
        .chip_enable_pad = AML_NAND_CE0,
        .ready_busy_pad = AML_NAND_CE0,
        .platform_nand_data = {
            .chip =  {
                .nr_chips = 1,
                .options = (NAND_TIMING_MODE5 | NAND_ECC_SHORT_MODE),
            },
        },
        .rbpin_mode=1,
        .short_pgsz=384,
        .ran_mode=0,
        .T_REA = 20,
        .T_RHOH = 15,
    },
    {
        .name = NAND_NORMAL_NAME,
        .chip_enable_pad = (AML_NAND_CE0) ,  //| (AML_NAND_CE1 << 4) | (AML_NAND_CE2 << 8) | (AML_NAND_CE3 << 12)),
        .ready_busy_pad = (AML_NAND_CE0) ,  //| (AML_NAND_CE0 << 4) | (AML_NAND_CE1 << 8) | (AML_NAND_CE1 << 12)),
        .platform_nand_data = {
            .chip =  {
                .nr_chips = 1,
                .options = (NAND_TIMING_MODE5 | NAND_ECC_BCH30_MODE),
            },
        },
        .rbpin_mode = 1,
        .short_pgsz = 0,
        .ran_mode = 0,
        .T_REA = 20,
        .T_RHOH = 15,
    }
};

struct aml_nand_device aml_nand_mid_device = {
    .aml_nand_platform = aml_nand_mid_platform,
    .dev_num = ARRAY_SIZE(aml_nand_mid_platform),
};

#if defined(CONFIG_CMD_KGDB)

void
kgdb_serial_init(void)
{


}

void
putDebugChar (int c)
{
        serial_putc (c);
}

void
putDebugStr (const char *str)
{
        serial_puts (str);
}

int
getDebugChar (void)
{
        return serial_getc();
}

void
kgdb_interruptible (int yes)
{
        return;
}
int
kgdb_serial_tstc (void)
{
    return 0;
}
#endif

