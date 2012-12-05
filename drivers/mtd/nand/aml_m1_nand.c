/*
 * aml_m1_nand.c
 *
 *  Created on: 2010-6-9
 *      Author: yuhao
 */
#include <common.h>
#include <nand.h>
#include <asm/io.h>
#include <asm/arch/nand.h>
#include <malloc.h>
#include <linux/err.h>
#include <asm/cache.h>
#include <asm/arch/pinmux.h>

#define CONFIG_SYS_NAND_MAX_DEVICE 1
#define CONFIG_SYS_NAND_MAX_CHIPS 2
//CONFIG_AMLROM_U_BOOT CONFIG_AML_NANDBCH16  in your config file 

struct aml_platform_info{
	u32     mlc_mode;
	u32		ecc_mode;
	u32     ecc_size;
	u32		ce_sel;
	u32  *	info_buf;		
	struct nand_ecclayout * layout;
};

static struct aml_platform_info aml_chips[CONFIG_SYS_MAX_NAND_DEVICE+1]=
{

#ifdef CONFIG_AML_NANDBCH16

	[0]=
			{	.mlc_mode=1,
				.ecc_mode=NAND_ECC_BCH16,
				.ce_sel=CE0
			},
#else
	[0]=
			{	.mlc_mode=1,
				.ecc_mode=NAND_ECC_BCH8 ,
				.ce_sel=CE0
			},
	
#endif	

#ifdef CONFIG_AMLROM_NANDBOOT
		[CONFIG_SYS_MAX_NAND_DEVICE]=
			{	.mlc_mode=1,
				.ecc_mode=NAND_ECC_BCH16 ,
				.ce_sel=CE0
			},
#endif
};


#ifdef CONFIG_AMLROM_NANDBOOT
const char default_boot_name[]="nandboot";
#endif

DECLARE_GLOBAL_DATA_PTR;

int nand_curr_device = -1;
nand_info_t nand_info[CONFIG_SYS_NAND_MAX_DEVICE+1]=
{
#ifdef CONFIG_AMLROM_NANDBOOT
	[CONFIG_SYS_NAND_MAX_DEVICE]=
	{	
		.name="nandboot"
	},
#endif
};

static struct nand_chip nand_chip[CONFIG_SYS_MAX_NAND_DEVICE+1];
static const char default_nand_name[] = "nand";
static __attribute__((unused)) char dev_name[CONFIG_SYS_MAX_NAND_DEVICE+1][15];
static unsigned char inter_buf[8192];			//FIXME malloc


static struct aml_platform_info * get_platform_info(struct mtd_info * mtd)
{
	struct nand_chip * chip=(struct nand_chip *)(mtd->priv);
	return (struct aml_platform_info *)(chip->priv);
}
static u32 aml_get_ce(struct mtd_info * mtd)
{
	struct aml_platform_info * ctrl_info=get_platform_info(mtd);
	u32 ret=ctrl_info->ce_sel;
	return (u32)ret;
}
static void m1_nfc_hwcontrol(struct mtd_info *mtd, int cmd,  unsigned int ctrl)
{
	u32 ce=aml_get_ce(mtd);
    if (cmd == NAND_CMD_NONE)
        return;

    if (ctrl & NAND_CLE)
        cmd=NFC_CMD_CLE(ce,cmd);
    else
    	cmd=NFC_CMD_ALE(ce,cmd);

    NFC_SEND_CMD(cmd);
}

static int m1_nfc_devready(struct mtd_info *mtd)
{
	// return NFC_GET_RB_STATUS(aml_get_ce(mtd));
	struct nand_chip *chip = mtd->priv;
	unsigned ce=0;
	unsigned TWO_RB_IN_ONE =0;
	unsigned int ce_sel=aml_get_ce(mtd);
	if(chip->options&NAND_2RB_IN_ONE)
	{
		TWO_RB_IN_ONE =1;
	  //printk("TWO_RB_IN_ONE=%d \n",TWO_RB_IN_ONE);
	}
	if(ce_sel==CE0)
	   	ce=1;
	else 	if(ce_sel==CE1)
	{
		ce=2;
		if(TWO_RB_IN_ONE==1)
			ce=1;
	}
	else	if(ce_sel==CE2)
	{
		ce=4;
		if(TWO_RB_IN_ONE==1)
			ce=2;
	}
	else if(ce_sel==CE3)
	{
		ce=8;
		if(TWO_RB_IN_ONE==1)
			ce=2;
	}
	return (NFC_INFO_GET()>>26)&ce;
}


static void prepare_info_buf_before_read(struct mtd_info *mtd,int len)
{
	struct aml_platform_info * aml_info=get_platform_info(mtd);
	memset(aml_info->info_buf,0,len<<2);
}
static void transfer_info_buf_after_read(struct mtd_info *mtd,int len)
{
	struct aml_platform_info * aml_info=get_platform_info(mtd);
	struct nand_chip *chip = mtd->priv;
	uint16_t *p=(uint16_t *)chip->oob_poi;							//FIXME
	int i;

	for(i=0;i<len;i++)
	{
		p[i]=aml_info->info_buf[i]&0xffff;
	}
	return ;
}

static void prepare_info_buf_before_write(struct mtd_info *mtd,int len)
{
	struct aml_platform_info * aml_info=get_platform_info(mtd);
	struct nand_chip *chip = mtd->priv;
	uint16_t *p=(uint16_t *)chip->oob_poi;
	int i;
	for(i=0;i<len;i++)
	{
		aml_info->info_buf[i]=p[i];
	}
}

static uint8_t m1_nfc_read_byte(struct mtd_info *mtd)
{
	uint8_t val;
	unsigned int ce_sel=aml_get_ce(mtd);
	NFC_SEND_CMD(ce_sel|DRD | 0);
   	NFC_SEND_CMD(ce_sel|IDLE | 5); // 100 ns
	while(NFC_CMDFIFO_SIZE()>0);
	val=NFC_GET_BUF()&0xff;	
    return val;
}


static void m1_nfc_read_buf_ecc(struct mtd_info *mtd, uint8_t *buf, int len,int bch_mode,int ecc_on)
{
	volatile unsigned  int i=0,j=0;
	struct aml_platform_info * aml_info=get_platform_info(mtd);
	volatile 	unsigned int * pbuf=NULL;		
	volatile 	unsigned int * pbuf1=NULL;
	unsigned count=(len+511)>>9;
	pbuf=(aml_info->info_buf);	
	pbuf1=(unsigned int *)((((unsigned)(pbuf)<<1)>>1)+(unsigned)0xa0000000);

	memset(pbuf1,0,count*sizeof(u32));
	memset(aml_info->info_buf,0,count*sizeof(u32));
	dcache_invalid_range((unsigned )pbuf1,count*sizeof(u32));
	dcache_invalid_range((unsigned)buf,len);
	NFC_SET_DADDR(buf);
	NFC_SET_IADDR(pbuf1);	
	NFC_SEND_CMD_N2M(len,bch_mode,ecc_on);
	while(NFC_CMDFIFO_SIZE()>0);

	
	 do{
		pbuf=&(aml_info->info_buf[count-1]);	
		pbuf1=(unsigned int *)((((unsigned)(pbuf)<<1)>>1)+(unsigned)0xa0000000);
		j=*pbuf1;	
	}while(!j);
	pbuf=&(aml_info->info_buf[0]);	
	pbuf1=(unsigned int *)((((unsigned)(pbuf)<<1)>>1)+(unsigned)0xa0000000);	
	memcpy(aml_info->info_buf,pbuf1,count*sizeof(u32));
	if(ecc_on)
		transfer_info_buf_after_read(mtd,(len+511)>>9);
}
/*
static void m1_nfc_read_buf_ecc(struct mtd_info *mtd, uint8_t *buf, int len,int bch_mode,int ecc_on)
{
	volatile unsigned  int i=0,j=0;
	struct aml_platform_info * aml_info=get_platform_info(mtd);
	volatile 	unsigned int * pbuf=NULL;		
	volatile 	unsigned int * pbuf1=NULL;
	unsigned count=(len+511)>>9;

	memset(aml_info->info_buf,0x00,count*sizeof(u32));
	dcache_invalid_range((unsigned)buf,len);
	dcache_invalid_range((unsigned )(aml_info->info_buf),count*sizeof(u32));

	NFC_SEND_CMD_ADL((int)(buf));      
	NFC_SEND_CMD_ADH((int)(buf));         
	NFC_SEND_CMD_AIL((int)(aml_info->info_buf));          
	NFC_SEND_CMD_AIH((int)(aml_info->info_buf));
	NFC_SEND_CMD_N2M(len,bch_mode,ecc_on);
	NFC_SEND_CMD_IDLE(aml_get_ce(mtd),3);
	while(NFC_CMDFIFO_SIZE()>0);

	do{
		dcache_invalid_range((unsigned )(aml_info->info_buf),count*sizeof(u32));
		pbuf=&(aml_info->info_buf[count-1]);	
		j=*pbuf;	
	}while(!j);

	if(ecc_on)
		transfer_info_buf_after_read(mtd,(len+511)>>9);
}*/


static void m1_nfc_write_buf_ecc(struct mtd_info *mtd, const uint8_t *buf, int len,int bch_mode,int ecc_on)
{
	struct aml_platform_info * aml_info=get_platform_info(mtd);

	if(ecc_on)
		prepare_info_buf_before_write(mtd,(len+511)>>9);
	
	dcache_flush_range(buf,len);
	dcache_flush_range(aml_info->info_buf,32*sizeof(u32));	
	NFC_SET_DADDR(buf);
	NFC_SET_IADDR(aml_info->info_buf);
	NFC_SEND_CMD_M2N(len,bch_mode,ecc_on);
	NFC_SEND_CMD_IDLE(aml_get_ce(mtd),0);
	NFC_SEND_CMD_IDLE(aml_get_ce(mtd),0);
	while(NFC_CMDFIFO_SIZE()>0);
}

static void m1_nfc_read_buf_raw(struct mtd_info *mtd, uint8_t *buf, int len)
{
    struct aml_platform_info * ctrl_info=get_platform_info(mtd);
	unsigned int size=((mtd->writesize+mtd->oobsize));
	
	m1_nfc_read_buf_ecc(mtd,buf,size,ctrl_info->ecc_mode,0);
//    m1_nfc_read_buf_ecc(mtd,buf,size,ctrl_info->ecc_mode,1);
//	BUG();
//	m1_nfc_read_buf_ecc(mtd,buf,len,0,0);
}

static void m1_nfc_write_buf_raw(struct mtd_info *mtd, const uint8_t *buf, int len)
{
	BUG();
//	m1_nfc_write_buf_ecc(mtd,buf,len,0,0);
}






/**
 * ECC functions
 */
//ecc.read_page
static int m1_nfc_read_page(struct mtd_info *mtd, struct nand_chip *chip,
				   uint8_t *buf, int page)
{
	struct aml_platform_info * ctrl_info=get_platform_info(mtd);
	//unsigned int size=((mtd->writesize+mtd->oobsize)/(ctrl_info->ecc_size))<<9;
	unsigned int size=mtd->writesize;
	if(!strcmp(mtd->name,"nandboot"))
	{
		size=3*512;	
		ctrl_info->ecc_mode = 3;
	}	
	m1_nfc_read_buf_ecc(mtd,buf,size,ctrl_info->ecc_mode,1);
	return 0;
}
static void m1_nfc_write_page(struct mtd_info *mtd, struct nand_chip *chip,
				   const uint8_t *buf)
{
	struct aml_platform_info * ctrl_info=get_platform_info(mtd);
//	unsigned int size=((mtd->writesize+mtd->oobsize)/(ctrl_info->ecc_size))<<9;
	unsigned int size=mtd->writesize;
	if(!strcmp(mtd->name,"nandboot"))
	{
		size=3*512;	
		ctrl_info->ecc_mode = 3;
	}	
	m1_nfc_write_buf_ecc(mtd,buf,size,ctrl_info->ecc_mode,1);

}
static int m1_nfc_read_page_raw(struct mtd_info *mtd, struct nand_chip *chip,
				   uint8_t *buf, int page)
{
     struct aml_platform_info * ctrl_info=get_platform_info(mtd);
	unsigned int size=((mtd->writesize+mtd->oobsize));
	m1_nfc_read_buf_ecc(mtd,buf,size,0,0);
	if(buf!=chip->buffers->databuf)
	    memcpy(chip->oob_poi,buf + mtd->writesize,mtd->oobsize);
	return 0;
//	BUG();
}
static void m1_nfc_write_page_raw(struct mtd_info *mtd, struct nand_chip *chip,
				   const uint8_t *buf)
{
	BUG();
}
static int m1_nfc_read_oob(struct mtd_info *mtd, struct nand_chip *chip,
				  int page, int sndcmd)
{

	chip->cmdfunc(mtd, NAND_CMD_READ0, 0, page);
	struct aml_platform_info * ctrl_info=get_platform_info(mtd);
	NFC_SET_SPARE_ONLY();
	m1_nfc_read_buf_ecc(mtd,NULL,mtd->writesize,ctrl_info->ecc_mode,1);
	NFC_CLEAR_SPARE_ONLY();
	return 1;
}
static int m1_nfc_write_oob(struct mtd_info *mtd,
				   struct nand_chip *chip, int page)
{

	printk("Amlogic controller does not support write OOB only \n");
	printk("That's SLC feature , please change your software \n");
	BUG();
	return  -EIO ;
}

static int m1_nfc_calculate_ecc(struct mtd_info *mtd, const u_char *dat, u_char *ecc_code)
{
    return 0;
}

static int m1_nfc_correct_data(struct mtd_info *mtd, u_char *dat, u_char *read_ecc, u_char *calc_ecc)
{
    return 0;
}
static void m1_nfc_enable_hwecc(struct mtd_info *mtd, int mode)
{
    return;
}
static int m1_nfc_block_bad(struct mtd_info *mtd, loff_t ofs, int getchip)
{
	int page, chipnr, res = 0;
	int i=0;
	struct nand_chip *chip = mtd->priv;
	struct aml_platform_info * ctrl_info=(struct aml_platform_info * )chip->priv;
	unsigned int size=((mtd->writesize+mtd->oobsize)/(ctrl_info->ecc_size))<<9;

	page = (int)(ofs >> chip->page_shift) & chip->pagemask;

	if (getchip) {
		chipnr = (int)(ofs >> chip->chip_shift);
		nand_get_device(chip, mtd, FL_READING);
		chip->select_chip(mtd, chipnr);
	}
	NFC_SET_SPARE_ONLY();
	chip->cmdfunc(mtd, NAND_CMD_READ0, 0, page);
	m1_nfc_read_buf_ecc(mtd,inter_buf,size,ctrl_info->ecc_mode,1);   //FIXME 3*512
	NFC_CLEAR_SPARE_ONLY();

	for(i=0;i<size>>9;i++)
	{
	    
		if((ctrl_info->info_buf[i])&(1<<29))
		{
		   printf("Bad block info %d =%x\n",i,ctrl_info->info_buf[i]);
			res=1;
			break;
		}
	}

	if (getchip)
		nand_release_device(mtd);

	return res;
}



static void aml_select_chip(struct nand_chip * chip,int chip_num)
{
	struct aml_platform_info * ctrl_info=(struct aml_platform_info * )chip->priv;


#ifdef CONFIG_AML_MESION2
   	CLEAR_CBUS_REG_MASK(PERIPHS_PIN_MUX_2,( (1<<7) | (1<<8) | (1<<9) | (1<<10)|(1<<11)|(1<<12)));
    SET_CBUS_REG_MASK(PERIPHS_PIN_MUX_5,0x7ff);
#else
	CLEAR_CBUS_REG_MASK(PERIPHS_PIN_MUX_1,(( (1<<29) | (1<<27) | (1<<25) | (1<<23))));
	SET_CBUS_REG_MASK(PERIPHS_PIN_MUX_1,(1<<30) | (1<<28) | (1<<26) | (1<<24));
	CLEAR_CBUS_REG_MASK(PERIPHS_PIN_MUX_6,0x7fff);
	SET_CBUS_REG_MASK(PERIPHS_PIN_MUX_6,0x7ff);		
	CLEAR_CBUS_REG_MASK(PERIPHS_PIN_MUX_7,((1<<29) |(1<<28)|(1<<27)|(1<<26)|(1<<25)|(1<<24)));  
#endif

	switch (chip_num) {
		case -1:
			//chip->cmd_ctrl(mtd, NAND_CMD_NONE, 0 | NAND_CTRL_CHANGE);
#ifdef CONFIG_AML_MESION2
			CLEAR_CBUS_REG_MASK(PERIPHS_PIN_MUX_5,0x7ff);
#else
			CLEAR_CBUS_REG_MASK(PERIPHS_PIN_MUX_6,0x7fff);			
#endif
			break;
		case 0:
			ctrl_info->ce_sel=CE0;
			break;
		case 1:
			ctrl_info->ce_sel=CE1;
			break;
		case 2:
			ctrl_info->ce_sel=CE2;
			SET_CBUS_REG_MASK(PERIPHS_PIN_MUX_6,0x67ff);	
			break;
		case 3:	
			ctrl_info->ce_sel=CE3;
			SET_CBUS_REG_MASK(PERIPHS_PIN_MUX_6,0x67ff);	
			break;
		default:
			BUG();
	}

}
static void m1_nfc_select_chip(struct mtd_info * mtd,int chip_num)
{
	aml_select_chip(((struct nand_chip *)mtd->priv),chip_num);
}

static  void board_nf_reset(void)
{
#ifdef CONFIG_AML_MESION2
   	CLEAR_CBUS_REG_MASK(PERIPHS_PIN_MUX_2,( (1<<7) | (1<<8) | (1<<9) | (1<<10)|(1<<11)|(1<<12)));
    SET_CBUS_REG_MASK(PERIPHS_PIN_MUX_5,0x7ff);
#else
	CLEAR_CBUS_REG_MASK(PERIPHS_PIN_MUX_1,(( (1<<29) | (1<<27) | (1<<25) | (1<<23))));
	SET_CBUS_REG_MASK(PERIPHS_PIN_MUX_1,(1<<30) | (1<<28) | (1<<26) | (1<<24));
	CLEAR_CBUS_REG_MASK(PERIPHS_PIN_MUX_6,0x7fff);
	SET_CBUS_REG_MASK(PERIPHS_PIN_MUX_6,0x7ff);								
	CLEAR_CBUS_REG_MASK(PERIPHS_PIN_MUX_7,((1<<29) |(1<<28)|(1<<27)|(1<<26)|(1<<25)|(1<<24)));  
#endif


	NFC_SET_CFG(0);
	WRITE_CBUS_REG(NAND_CFG, (0<<10) | (0<<9) | (0<<5) | 19);	
	NFC_SEND_CMD(1<<31);

	NFC_SEND_CMD_IDLE(CE0,10);    
	NFC_SEND_CMD_CLE(CE0,0xff);   //Send reset command
	NFC_SEND_CMD_IDLE(CE0,10);    
	while(NFC_CMDFIFO_SIZE()>0);
	while(!(NFC_INFO_GET()>>26));
	
	NFC_SEND_CMD_IDLE(CE1,10);    
	NFC_SEND_CMD_CLE(CE1,0xff);   //Send reset command
	NFC_SEND_CMD_IDLE(CE1,10);    
	while(NFC_CMDFIFO_SIZE()>0);
	while(!(NFC_INFO_GET()>>26));

}


static int board_nand_init(struct nand_chip *chip,struct aml_platform_info * aml_cntl_info)
{
   	unsigned char * ptmp=0;
	chip->priv=aml_cntl_info;

	board_nf_reset();
	NFC_SET_TIMING(0,19,0) ;
    aml_select_chip(chip,0);

		
   	chip->write_buf  = m1_nfc_write_buf_raw;
   	chip->read_buf   = m1_nfc_read_buf_raw;
	chip->read_byte  = m1_nfc_read_byte;                    
	chip->cmd_ctrl     = m1_nfc_hwcontrol;
	chip->dev_ready    = m1_nfc_devready;
	chip->chip_delay   = 20;
	chip->select_chip  =m1_nfc_select_chip;
	if(aml_cntl_info->mlc_mode==0)
	{
		chip->ecc.mode = NAND_ECC_SOFT;//Support Software mode only
		BUG();
	}
	else
	{
		chip->ecc.mode =NAND_ECC_HW;	
	}
	chip->options|=NAND_SKIP_BBTSCAN;
	chip->options &= ~NAND_BUSWIDTH_16	;
	
	aml_cntl_info->info_buf=(unsigned int *)malloc(128*sizeof(u32));

	if(!aml_cntl_info->info_buf)
	{
		printk("info malloc err\n");
		BUG();
	}
	else
	{
		if(((unsigned )aml_cntl_info->info_buf%32)!=0)
		{
			ptmp=(unsigned char * )	aml_cntl_info->info_buf;
			ptmp+=32-(((unsigned )aml_cntl_info->info_buf%32));
			aml_cntl_info->info_buf=(unsigned int *)ptmp;
		}
	}
	return 0;
}
int m1_nfc_scan_tail(struct mtd_info *mtd)
{
	struct nand_chip *chip = mtd->priv;
	struct aml_platform_info * ctrl_info=(struct aml_platform_info * )chip->priv;
	struct nand_ecc_ctrl * ecc;
	int i,j;
	if(chip->ecc.mode==NAND_ECC_SOFT)
		return 0;//We choose the software ECC mode for the traditional SLC

	if(ctrl_info->ecc_mode == NAND_ECC_BCH8);
	else if(!strcmp(mtd->name, "nandboot"))
	{
		printk("Uboot partition\n");    	
	}
	else if((mtd->oobsize==64)||(mtd->oobsize==128))
	{
		ctrl_info->ecc_mode=NAND_ECC_BCH8;
		printk("\n oob size is %d bytes ,so use BCH8!\n", mtd->oobsize);
	}
	else if((mtd->oobsize!=448)&&(mtd->oobsize!=224) \
		&&(ctrl_info->ecc_mode == NAND_ECC_BCH16))
	{
		ctrl_info->ecc_mode=NAND_ECC_BCH12;
		printk("oob size not enough for BCH16 ,so use BCH12!\n");
	}

	if(ctrl_info->ecc_mode==NAND_ECC_BCH9)
		ctrl_info->ecc_size=528;
	if(ctrl_info->ecc_mode==NAND_ECC_BCH8)
	{
		printk("BCH8 device\n");
		ctrl_info->ecc_size=528;
	}	
	if(ctrl_info->ecc_mode==NAND_ECC_BCH12)
	{
		printk("BCH12 device\n");
		ctrl_info->ecc_size=534;
	}
	if(ctrl_info->ecc_mode==NAND_ECC_BCH16)
	{
		printk("BCH16 device\n");
		ctrl_info->ecc_size=540;
	}
	ecc=&(chip->ecc);
	chip->ecc.size=512;
	if(ctrl_info->ecc_mode==NAND_ECC_BCH9)
		chip->ecc.prepad=1;
	else
		chip->ecc.prepad=2;
	
	chip->ecc.steps=mtd->writesize/chip->ecc.size;
	switch(ctrl_info->ecc_mode)
	{
		case NAND_ECC_BCH9:
			chip->ecc.bytes=14;
			chip->ecc.postpad=1;
			break;
		case NAND_ECC_BCH8:
			chip->ecc.bytes=13;
			chip->ecc.postpad=1;
			break;
		case  NAND_ECC_BCH12:
			chip->ecc.bytes=20;
			chip->ecc.postpad=0;
			break;
		case NAND_ECC_BCH16:
			chip->ecc.bytes=26;
			chip->ecc.postpad=0;
			break;
	}
	int chunk=chip->ecc.bytes+chip->ecc.postpad+chip->ecc.prepad;
	if(chip->ecc.steps*chunk>mtd->oobsize)
	{
	//	printk("Error ECC select");
		printk(" ecc step %d chunk %d oob %d \n ",chip->ecc.steps,chunk,mtd->oobsize);
	//	BUG();
	}
	
	ctrl_info->layout=(struct nand_ecclayout *)malloc(sizeof(struct nand_ecclayout));
	ctrl_info->layout->eccbytes=chip->ecc.bytes*chip->ecc.steps;
	j=chip->ecc.steps*chip->ecc.prepad;
	ctrl_info->layout->oobfree[0].length=j;
	ctrl_info->layout->oobfree[0].offset=0;

	for(i=0;i<ctrl_info->layout->eccbytes;i++,j++)
	{
		ctrl_info->layout->eccpos[i]=j;
	}

	chip->ecc.layout=ctrl_info->layout;
	chip->ecc.layout->oobavail=j;

	chip->ecc.read_page  	= m1_nfc_read_page;
	chip->ecc.write_page 	= m1_nfc_write_page;
	chip->ecc.read_page_raw = m1_nfc_read_page_raw;
	chip->ecc.write_page_raw= m1_nfc_write_page_raw;
	chip->ecc.read_oob 		= m1_nfc_read_oob;
	chip->ecc.write_oob 	= m1_nfc_write_oob;
	chip->ecc.calculate 	= m1_nfc_calculate_ecc;
	chip->ecc.correct   	= m1_nfc_correct_data;
	chip->ecc.hwctl			= m1_nfc_enable_hwecc;
    chip->block_bad     	= m1_nfc_block_bad;
	
	return 0;
}
int m1_nfc_scan_ident(struct mtd_info *mtd, int maxchips)
{
	return 1;
}

static int aml_rom_nand_do_write_ops(struct mtd_info *mtd, loff_t to,struct mtd_oob_ops *ops)
{
	int chipnr, realpage, page, blockmask, column;
	struct nand_chip *chip = mtd->priv;
	struct aml_platform_info * ctrl_info=(struct aml_platform_info * )chip->priv;
	uint32_t writelen = ops->len;
	uint8_t *oob = ops->oobbuf;
	uint8_t *buf = ops->datbuf;
	int ret;
	uint32_t i;
	int bytes = ops->len;
	int cached =0;	
	uint8_t *wbuf = buf;
	/*uint32_t size=((mtd->writesize+mtd->oobsize)/(ctrl_info->ecc_size))<<9;
	if(ops->len!=size)
	{
//		BUG();
	}*/

	if (!writelen)
		return -EINVAL;
	if ((to + writelen) > mtd->size)
		return -EINVAL;

	ops->retlen = 0;
	chipnr = (int)(to >> chip->chip_shift);
	chip->select_chip(mtd, chipnr);
	realpage = (int)(to >> chip->page_shift);
	page = realpage & chip->pagemask;
	blockmask = (1 << (chip->phys_erase_shift - chip->page_shift)) - 1;

	while(1) 
	{
	
		oob = nand_fill_oob(chip, oob, ops);
		ret = chip->write_page(mtd, chip, wbuf, page, cached,
				       (ops->mode == MTD_OOB_RAW));
		if (ret)
		{
			printk("rom_write write err \n");
			break;
		}
		writelen -= bytes;
		if (!writelen)
			break;
		column = 0;
		buf += bytes;
		realpage++;
	}
	ops->retlen = ops->len - writelen;
	
	chip->cmdfunc(mtd, NAND_CMD_READ0, 0x00, page);
	ret=chip->ecc.read_page(mtd, chip, inter_buf,page);
	if(ret)	
		printk("rom_write  read err \n");
		
	for(i=0;i<ops->len;i++){
	
		if(inter_buf[i]!=buf[i]){
			ret=1;
			printk("rom_write cmp err at %d org 0x%x get 0x%x \n",i, inter_buf[i],buf[i]);
			break;	
			//	BUG();
		}
	}
	
	return ret;
}
static int aml_rom_nand_write_oob(struct mtd_info *mtd, loff_t to, struct mtd_oob_ops *ops)
{
	struct nand_chip *chip = mtd->priv;
	int ret;

	nand_get_device(chip, mtd, FL_WRITING);

	switch(ops->mode) {
	case MTD_OOB_PLACE:
	case MTD_OOB_AUTO:
	case MTD_OOB_RAW:
		break;

	default:
		goto out;
	}
	

	ret = aml_rom_nand_do_write_ops(mtd, to, ops);

out:
	nand_release_device(mtd);

	return ret;
}
int m1_nfc_scan(struct mtd_info *mtd, int maxchips)
{
	int ret;
	struct nand_chip *chip = mtd->priv;
	struct aml_platform_info * ctrl_info=(struct aml_platform_info * )chip->priv;

	ret = nand_scan_ident(mtd, maxchips);
	if (ret)
	{
		ret=m1_nfc_scan_ident(mtd,maxchips);
	}
	if(!ret)
		ret = m1_nfc_scan_tail(mtd);
	if(!ret)
		ret = nand_scan_tail(mtd);

	return ret;
}

static void nand_init_chip(struct mtd_info *mtd, struct nand_chip *nand,
		struct aml_platform_info * base_addr)
{
	int maxchips = CONFIG_SYS_NAND_MAX_CHIPS;
	int __attribute__((unused)) i = 0;

	if (maxchips < 1)
		maxchips = 1;
	mtd->priv = nand;

	nand->IO_ADDR_R = nand->IO_ADDR_W = (void  __iomem *)base_addr;
	if (board_nand_init(nand,base_addr) == 0) {
		if (m1_nfc_scan(mtd, maxchips)==0) {
			printf("000mtd->name: %s\n", mtd->name);
#ifdef CONFIG_MTD_DEVICE
			/*
			 * Add MTD device so that we can reference it later
			 * via the mtdcore infrastructure (e.g. ubi).
			 */
			if(!mtd->name){
			sprintf(dev_name[i], "nand%d", i);
			mtd->name = dev_name[i++];
			printf("mtd->name: %s\n", mtd->name);
			}
		
			add_mtd_device(mtd);
#else
			if (!mtd->name)
				mtd->name = (char *)default_nand_name;
#endif
		} else
			mtd->name = NULL;
	} else {
		mtd->name = NULL;
		mtd->size = 0;
	}
}

void nand_init(void)
{
	int i;
	unsigned int size = 0;
//	asm("wfi");
	for (i = 0; i < CONFIG_SYS_MAX_NAND_DEVICE; i++)
	{
		nand_init_chip(&nand_info[i], &nand_chip[i], &aml_chips[i]);
		size += nand_info[i].size / 1024;
		if (nand_curr_device == -1)
			nand_curr_device = i;
	}


#ifdef CONFIG_AMLROM_NANDBOOT
	nand_init_chip(&nand_info[CONFIG_SYS_MAX_NAND_DEVICE], &nand_chip[CONFIG_SYS_MAX_NAND_DEVICE], &aml_chips[CONFIG_SYS_MAX_NAND_DEVICE]);	
	nand_info[CONFIG_SYS_MAX_NAND_DEVICE].size=nand_info[CONFIG_SYS_MAX_NAND_DEVICE].writesize<<10;
	nand_info[CONFIG_SYS_MAX_NAND_DEVICE].write_oob = aml_rom_nand_write_oob;		
#endif
	nand_curr_device = 0;


	printf("%u MiB\n", size / 1024);

}

