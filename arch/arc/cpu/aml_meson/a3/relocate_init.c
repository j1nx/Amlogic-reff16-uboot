//#include "c_arc_pointer_reg.h"
#include <config.h>
#include <common.h>
#include <asm/arch/nand.h>
#include <asm/arch/romboot.h>
#include <asm/arch/am_regs.h>

static void ipl_memcpy(unsigned * dest, unsigned * src, unsigned count/*in long*/)
{
   int i;
	for(i=0;i<count>>2;i++)
	  dest[i]=src[i];
}

static void spi_init(void);
static void nf_cntl_init(const T_ROM_BOOT_RETURN_INFO* bt_info);
static int nf_lp_read( unsigned target, unsigned size);
static void  nf_sp_read(unsigned target,unsigned size);
static void sdio_read(unsigned target,unsigned size,unsigned por_sel);

/**
   SPI_A FLASH INIT
*/
static void spi_init(void){
//#ifndef CONFIG_AML_SPI_SETTING
//   WRITE_CBUS_REG_BITS(SPI_FLASH_CTRL,__hw_setting.spi_setting,0,12);
//#else
//   Wr(P_SPI_FLASH_CTRL,CONFIG_AML_SPI_SETTING);
//#endif
}
//extern unsigned* _TEXT_BASE;
//extern unsigned* _TEXT_SIZE;
//extern unsigned* __TEXT_BASE;
//extern unsigned* __TEXT_SIZE;
void relocate_init(unsigned __TEXT_BASE,unsigned __TEXT_SIZE)
{
	unsigned por_cfg=READ_CBUS_REG(ASSIST_POR_CONFIG);
	unsigned boot_id=romboot_info->boot_id;
	unsigned size;
	unsigned * mem;
	int rc=0;	
	
//	serial_puts("ATTCH REALVIEW\n");
//	asm("wfi");


..
	/*size=(_TEXT_SIZE + 3)>>2;
	  if(size==0)
	  while(1);
	  */
	size=0x80000-READ_SIZE;

	if(((por_cfg&POR_ROM_BOOT_ENABLE)==0) || (((por_cfg&POR_INTL_CFG_MASK)==POR_INTL_SPI)&&boot_id==0))
	{
		serial_puts("spi boot");
		spi_init();
		mem=(unsigned *)(SPI_MEM_BASE+READ_SIZE);
		ipl_memcpy((unsigned*)__TEXT_BASE,mem,size);
		
	}else if(boot_id)
	{
		serial_puts("sdio boot\n");
		sdio_read(__TEXT_BASE,size,por_cfg&POR_SDIO_CFG_MASK);
	}
	else{
		
		switch(por_cfg&POR_INTL_CFG_MASK)
		{
			case  POR_INTL_NAND_LP:
				serial_puts("nand boot");
re_nand:
				rc=nf_lp_read(__TEXT_BASE,size);
				if(rc)
					goto re_sd;
				serial_puts("lpok\n");
				break;
			case  POR_INTL_NAND_SP:
				nf_sp_read(__TEXT_BASE,size);
				break;
			case  POR_INTL_SDIO_B1:
				sdio_read(__TEXT_BASE,size,0);
				break;
			default:
				serial_puts("NO PROCESS force nand \n");
				goto re_nand;
			//	goto re_sd;
		}
		
	}
//	writel(0,P_WATCHDOG_TC);

	serial_puts("JUMP U\n");
	
	return ;

re_sd:
	serial_puts("FAIL\n");
	while(1);
	return;
}

static  void nf_reset(t_nf_ce ce)
{
	NFC_SEND_CMD(1<<31);

	NFC_SEND_CMD_IDLE(ce,0);     // FIFO cleanup
	NFC_SEND_CMD_CLE(ce,0xff);   //Send reset command
	NFC_SEND_CMD_IDLE(ce,10);    //Wait tWB
	NFC_SEND_CMD_RB(ce,15);      //Wait Busy Signal
	NFC_SEND_CMD_IDLE(ce,0);     // FIFO cleanup
	NFC_SEND_CMD_IDLE(ce,0);     // FIFO cleanup
	while(NFC_CMDFIFO_SIZE()>0)   // wait until all command is finished
	{
		if(NFC_CHECEK_RB_TIMEOUT())  // RB time out
			break;
	}

	while(!((NFC_INFO_GET()>>26)&1));	
}
static void nf_cntl_init(const T_ROM_BOOT_RETURN_INFO* bt_info)
{
	// disable SPI
	CLEAR_CBUS_REG_MASK(PERIPHS_PIN_MUX_1,(( (1<<29) | (1<<27) | (1<<25) | (1<<23))));
	SET_CBUS_REG_MASK(PERIPHS_PIN_MUX_1,(1<<30) | (1<<28) | (1<<26) | (1<<24));
	// Enable NAND
	SET_CBUS_REG_MASK(PERIPHS_PIN_MUX_6,0x7ff);	
//	SET_CBUS_REG_MASK(PERIPHS_PIN_MUX_6,0x7fff);
	//card
	CLEAR_CBUS_REG_MASK(PERIPHS_PIN_MUX_7,((1<<29) |(1<<28)|(1<<27)|(1<<26)|(1<<25)|(1<<24)));  
	
	//Set NAND to DDR mode , time mode 0 , no adjust
//	 WRITE_CBUS_REG(NAND_CFG, __hw_setting.nfc_cfg);

	WRITE_CBUS_REG(NAND_CFG, (0<<10) | (0<<9) | (0<<5) | 19);		

	if(POR_GET_INTL_CFG(bt_info->por_cfg)==0x111)
	{
		serial_puts("reset nf agin\n");
		nf_reset(CE0);
	}
}
/**
   page_mode : 0, small page ; 1 large page
*/
static  int nf_send_read_cmd(unsigned page_off,unsigned page_mode)
{
	while (NFC_CMDFIFO_SIZE() > 15);

	NFC_SEND_CMD_CLE(CE0,0);                //Send NAND read start command

	NFC_SEND_CMD_ALE(CE0,0);                // Send Address
	if(page_mode)
		NFC_SEND_CMD_ALE(CE0,0);
	NFC_SEND_CMD_ALE(CE0,page_off&0xff);
	NFC_SEND_CMD_ALE(CE0,(page_off&0xff00)>>8);
	NFC_SEND_CMD_ALE(CE0,(page_off&0xff0000)>>16);

	if(page_mode)
		NFC_SEND_CMD_CLE(CE0,0x30);

	/** waiting for tWB **/
	NFC_SEND_CMD_IDLE(CE0,5); // tWB
	NFC_SEND_CMD_RB(CE0,14);
	if(((NFC_INFO_GET()>>26)&1))
	{
		serial_puts("BUG RB CMD\n");
		while(!((NFC_INFO_GET()>>26)&1));	
	}	
	
	NFC_SEND_CMD_IDLE(CE0,0);
	while (NFC_CMDFIFO_SIZE() > 0);      // all cmd is finished
	return 0;
}
static int nf_read_dma(unsigned * dest, volatile unsigned * info,unsigned size,t_ecc_mode ecc_mode)
//static int nf_read_dma(unsigned * dest,  unsigned * info,unsigned size,t_ecc_mode ecc_mode)
{
   unsigned cnt,i;
  
   if(ecc_mode!=NAND_ECC_BCH16)
	   return -1;
   if(size&0x1ff)
      return -1; // the size must be aligned with 512
  cnt=size>>9;
  for(i=0;i<cnt;i++)
  {
      info[i]=0;
  }
  NFC_SET_DADDR(dest);
  NFC_SET_IADDR(&info[0]);
  NFC_SEND_CMD_N2M(size,ecc_mode,1);
  NFC_SEND_CMD_IDLE(CE0,5);
  NFC_SEND_CMD_IDLE(CE0,0);
  while (NFC_CMDFIFO_SIZE() > 0);      // all cmd is finished


  for(i=0;i<cnt;i++)
  {
 	  while((NAND_INFO_DONE(info[i]))==0);

	  if(NAND_ECC_ENABLE(info[i])&&(NAND_ECC_FAIL(info[i])))
         return -1;
	  
	  if(NAND_INFO_DATA_2INFO(info[i])!=0x55aa)
         return -2;
  }
 
  return 0;	
}

/*
   Large Page NAND flash Read flow
*/
#define CONFIG_NAND_INFO_DMA_ADDR 0x80000000
static int  nf_lp_read(volatile unsigned  dest, volatile unsigned size)
{
	volatile  	unsigned page_base,cnt,cur;
	volatile   	int ret=0;
	unsigned char lp=0;

	//cnt=0;
	//	cur=READ_SIZE/3*512;
	nf_cntl_init(romboot_info);

	cnt=0;
	cur=6*1024/(3*512);
	page_base= romboot_info->nand_addr<<8;

	for(;cnt<size&&cur<256;cnt+=3*512,cur++)
	{

		do{

			nf_send_read_cmd(page_base+cur,1);//Large Page mode

			ret=nf_read_dma((unsigned*)(dest+cnt),( volatile unsigned*)CONFIG_NAND_INFO_DMA_ADDR,3*512, NAND_ECC_BCH16);	
		//	ret=nf_read_dma((unsigned*)(dest+cnt),(unsigned*)CONFIG_NAND_INFO_DMA_ADDR,3*512, NAND_ECC_BCH16);

			if(ret==0)
				break;
			else
			{

				if(ret==-1) serial_puts(" ecc err\n");

				if(ret==-2)	serial_puts("not 55aa\n");

				page_base+=256;
				if(page_base==1024)
				{
					serial_puts("lp 1024\n");
					goto bad;
					//	while(1);//dead , booting fail
				}
			}
		} while(ret!=0);
	}
	return 0;
bad:
	return 1;
}
static void nf_sp_read(unsigned dest,unsigned size)
{
   unsigned page_base,cnt,cur;
   int ret;
   nf_cntl_init(romboot_info);
   page_base= 0;

   for(cnt=0,cur=READ_SIZE/512;cnt<size&&(page_base+cur)<1024;cnt+=512,cur++)
   {
	   do{
		   nf_send_read_cmd(page_base+cur,1);//small Page mode
		   ret=nf_read_dma((unsigned*)(dest+cnt),(unsigned*)CONFIG_NAND_INFO_DMA_ADDR,
				   512, NAND_ECC_BCH8);

		   if(ret!=-1)
			   break;
		   else{
			   page_base+=CONFIG_NAND_SP_BLOCK_SIZE;
			   if((page_base+cur)>=CONFIG_NAND_PAGES)
				   while(1);//dead , booting fail
		   }
	   } while(ret==-1);
   }
	
}
/**
**/
// for CMD_SEND
#define     repeat_package_times_bit  24
#define     use_int_window_bit        21
#define     use_int_window_bit        21
#define     cmd_send_data_bit         20
#define     check_busy_on_dat0_bit    19
#define     response_crc7_from_8_bit  18
#define     response_have_data_bit 17
#define     response_do_not_have_crc7_bit 16
#define     cmd_response_bits_bit 8
#define     cmd_command_bit       0
// for SDIO_CONFIG
#define     sdio_write_CRC_ok_status_bit 29
#define     sdio_write_Nwr_bit        23
#define     m_endian_bit              21
#define     bus_width_bit             20   // 0 1-bit, 1-4bits
#define     data_latch_at_negedge_bit 19
#define     response_latch_at_negedge_bit 18
#define     cmd_argument_bits_bit 12
#define     cmd_out_at_posedge_bit 11
#define     cmd_disable_CRC_bit   10
#define     cmd_clk_divide_bit    0

// SDIO_STATUS_IRQ
#define     sdio_timing_out_count_bit   19
#define     arc_timing_out_int_en_bit   18
#define     amrisc_timing_out_int_en_bit 17
#define     sdio_timing_out_int_bit      16
#define     sdio_status_info_bit        12
#define     sdio_set_soft_int_bit       11
#define     sdio_soft_int_bit           10
#define     sdio_cmd_int_bit             9
#define     sdio_if_int_bit              8
#define     sdio_data_write_crc16_ok_bit 7
#define     sdio_data_read_crc16_ok_bit  6
#define     sdio_cmd_crc7_ok_bit  5
#define     sdio_cmd_busy_bit     4
#define     sdio_status_bit       0

// SDIO_IRQ_CONFIG
#define     force_halt_bit           30
#define     sdio_force_read_back_bit 24
#define     disable_mem_halt_bit     22
#define     sdio_force_output_en_bit 16
#define     soft_reset_bit           15
#define     sdio_force_enable_bit    14
#define     sdio_force_read_data_bit  8
#define     sdio_if_int_config_bit 6
#define     arc_soft_int_en_bit    5
#define     arc_cmd_int_en_bit     4
#define     arc_if_int_en_bit      3
#define     amrisc_soft_int_en_bit 2
#define     amrisc_cmd_int_en_bit  1
#define     amrisc_if_int_en_bit   0


// for SDIO_MULT_CONFIG
#define     data_catch_finish_point_bit 16
#define     response_read_index_bit     12
#define     data_catch_readout_en_bit    9
#define     write_read_out_index_bit     8
#define     data_catch_level_bit   6
#define     stream_8_bits_mode_bit 5
#define     stream_enable_bit      4
#define     ms_sclk_always_bit     3
#define     ms_enable_bit_bit      2
#define     SDIO_port_sel_bit      0

//---- Encoder I/F register
//#define CMD_ARGUMENT                (volatile unsigned long *)0xc1108c20
//#define CMD_SEND                    (volatile unsigned long *)0xc1108c24
//#define SDIO_CONFIG                 (volatile unsigned long *)0xc1108c28
//#define SDIO_STATUS_IRQ             (volatile unsigned long *)0xc1108c2c
//#define SDIO_IRQ_CONFIG             (volatile unsigned long *)0xc1108c30
//#define SDIO_MULT_CONFIG            (volatile unsigned long *)0xc1108c34
//#define SDIO_M_ADDR                 (volatile unsigned long *)0xc1108c38
//#define SDIO_EXTENSION              (volatile unsigned long *)0xc1108c3c

#define ERROR_NONE                  0
#define ERROR_GO_IDLE1              1
#define ERROR_GO_IDLE2              2
#define ERROR_APP55_1               3
#define ERROR_ACMD41                4
#define ERROR_APP55_2               5
#define ERROR_VOLTAGE_VALIDATION    6
#define ERROR_SEND_CID1             7
#define ERROR_SEND_RELATIVE_ADDR    8
#define ERROR_SEND_CID2             9
#define ERROR_SELECT_CARD           10
#define ERROR_APP55_RETRY3          11
#define ERROR_SEND_SCR              12
#define ERROR_READ_BLOCK            13
#define ERROR_STOP_TRANSMISSION     14
#define ERROR_MAGIC_WORDS           15
#define ERROR_CMD1                  16
#define ERROR_MMC_SWITCH_BUS        17
//#define CLK_DIV 3
#define NO_DELAY_DATA 0
//#define CLK_DIV 0
#define CLK_DIV_27 30
#define CLK_DIV_13 15
//#define SD_MMC_TRANFER_CLK_DIV      1

#define VOLTAGE_VALIDATION_RETRY    0x8000
#define APP55_RETRY                 3

#define CARD_TYPE_SD        0
#define CARD_TYPE_SDHC      1
#define CARD_TYPE_MMC       2
/* 250ms of timeout */
#define TIMEOUT_SHORT_27    ((27L*0x100000/((CLK_DIV_27+1)*2))*250/1000)
#define TIMEOUT_DATA_27     (TIMEOUT_SHORT_27 + READ_SIZE * 8)
#define TIMEOUT_SHORT_13    ((135L*0x100000/((CLK_DIV_13+1)*2))/10*250/1000)
#define TIMEOUT_DATA_13     (TIMEOUT_SHORT_13 + READ_SIZE * 8)
#define TIMEOUT_SD          0x1fff /* maximum SD hardware counter */
#ifndef CONFIG_SDIO_BUFFER_SIZE
#define CONFIG_SDIO_BUFFER_SIZE 64*1024
#endif
#define WAIT_BUSY(x, err) \
    if (wait_busy(x)!=0) { \
        error=err;while(1); \
    }

static int wait_busy(unsigned timeout)
{
    unsigned this_timeout = 0;
    unsigned r;

    WRITE_CBUS_REG(SDIO_STATUS_IRQ ,TIMEOUT_SD << sdio_timing_out_count_bit);

    while (1) {
        r = READ_CBUS_REG(SDIO_STATUS_IRQ);
        
        if ((r >> sdio_timing_out_count_bit) == 0) {
            this_timeout += TIMEOUT_SD;
            WRITE_CBUS_REG(SDIO_STATUS_IRQ ,TIMEOUT_SD << sdio_timing_out_count_bit);
        }

        if (this_timeout >= timeout) {
            return -1;
        }

        if(((r >> sdio_cmd_busy_bit) & 0x1) == 0) {
            return 0;
        }
    }            
}
static void sdio_read(unsigned target,unsigned size,unsigned por_sel)
{
   unsigned SD_boot_type;
   int error;
   unsigned TIMEOUT_SHORT,TIMEOUT_DATA;
   unsigned cmd_clk_divide;
   cmd_clk_divide=__hw_setting.sdio_cmd_clk_divide;
   TIMEOUT_SHORT=__hw_setting.sdio_time_short<<7;
   TIMEOUT_DATA = TIMEOUT_SHORT+(CONFIG_SDIO_BUFFER_SIZE<<3);

   switch(por_sel)
    {
        case POR_SDIO_A_ENABLE://SDIOA,GPIOA_9~GPIOA_14
            SD_boot_type=0;
            break;
        case POR_SDIO_B_ENABLE://SDIOB,GPIOA_0~GPIOA_5
            SD_boot_type=1;
            break;
        case POR_SDIO_C_ENABLE://SDIOC GPIOB_2~GPIOB_7
            SD_boot_type=2;
            break;
        default://SDIOB1 GPIOE_6~GPIOE_11
            SD_boot_type=1;
            break;
    }
   //set clk to trnsfer clk rate and bus width
   WRITE_CBUS_REG(SDIO_CONFIG,(2 << sdio_write_CRC_ok_status_bit) |
                      (2 << sdio_write_Nwr_bit) |
                      (3 << m_endian_bit) |
                      (39 << cmd_argument_bits_bit) |
                      (0 << cmd_out_at_posedge_bit) |
                      (0 << cmd_disable_CRC_bit) |
                      (NO_DELAY_DATA << response_latch_at_negedge_bit) |
                      ((cmd_clk_divide-1)  << cmd_clk_divide_bit));

    //--------------------------------------------------------------------------------
    // send CMD17 -- READ_BLOCK
   WRITE_CBUS_REG(SDIO_MULT_CONFIG,SD_boot_type);
   unsigned addr,cur_size,read_size;
   size=(size+511)&(~(511));
   for(addr=READ_SIZE,read_size=0;read_size<size;addr+=CONFIG_SDIO_BUFFER_SIZE)
   {
      if(romboot_info->card_type==CARD_TYPE_SDHC)
         WRITE_CBUS_REG(CMD_ARGUMENT,addr>>9);
      else
         WRITE_CBUS_REG(CMD_ARGUMENT,addr);
      cur_size=(size-read_size)>CONFIG_SDIO_BUFFER_SIZE?CONFIG_SDIO_BUFFER_SIZE:(size-read_size);


      WRITE_CBUS_REG(SDIO_M_ADDR,read_size+target);
      read_size+=cur_size;
      cur_size>>=9;

      WRITE_CBUS_REG(SDIO_EXTENSION,((1 << (9 + 3)) + (16 - 1)) << 16);
      WRITE_CBUS_REG(CMD_SEND,
           (((cur_size >> 9) - 1) << repeat_package_times_bit) | // packet number
           (0 << cmd_send_data_bit) | // RESPONSE has CRC7
           (0 << response_do_not_have_crc7_bit) | // RESPONSE has CRC7
           (1 << use_int_window_bit) | // will disable interrupt
           (1 << response_have_data_bit) | // RESPONSE has data receive
           (45 << cmd_response_bits_bit) | // RESPONSE have 7(cmd)+32(respnse)+7(crc)-1 data
           ((0x40+18) << cmd_command_bit)
          );




      WAIT_BUSY(TIMEOUT_SHORT, ERROR_READ_BLOCK);
      //--------------------------------------------------------------------------------
      // send CMD12 -- STOP_TRANSMISSION
//      if(C_ROM_BOOT_DEBUG->card_type==CARD_TYPE_MMC)   	
//         Wr(CMD_ARGUMENT, 1 << 16);
//      else
      WRITE_CBUS_REG(CMD_ARGUMENT, 0 << 16);//@todo , I don't know whether it has problem
      WRITE_CBUS_REG(CMD_SEND,
          (1 << check_busy_on_dat0_bit) | // RESPONSE is R1b
          (0 << response_crc7_from_8_bit) | // RESPONSE CRC7 is normal
          (0 << response_do_not_have_crc7_bit) | // RESPONSE has CRC7
          (45 << cmd_response_bits_bit) | // RESPONSE have 7(cmd)+32(respnse)+7(crc)-1 data
          ((0x40+12) << cmd_command_bit)
          );
      WAIT_BUSY(TIMEOUT_DATA, ERROR_STOP_TRANSMISSION);
    }
}
