#ifndef __NFIO_H_
#define __NFIO_H_


#define P_NAND_CMD  (volatile unsigned long *)0xc1108600
#define P_NAND_CFG  (volatile unsigned long *)0xc1108604
#define P_NAND_DADR (volatile unsigned long *)0xc1108608
#define P_NAND_IADR (volatile unsigned long *)0xc110860c
#define P_NAND_BUF  (volatile unsigned long *)0xc1108610
#define P_NAND_INFO (volatile unsigned long *)0xc1108614
#define P_NAND_DC   (volatile unsigned long *)0xc1108618
#define P_NAND_ADR  (volatile unsigned long *)0xc110861c
#define P_NAND_DL   (volatile unsigned long *)0xc1108620
#define P_NAND_DH   (volatile unsigned long *)0xc1108624
#define P_NAND_CADR (volatile unsigned long *)0xc1108628
#define P_NAND_SADR (volatile unsigned long *)0xc110862c

#define CEF (0xf<<10) 
#define CE0 (0xe<<10) 
#define CE1 (0xd<<10) 
#define CE2 (0xb<<10) 
#define CE3 (0x7<<10) 

#define IO4 ((0xe<<10)|(1<<18)) 
#define IO5 ((0xd<<10)|(1<<18)) 
#define IO6 ((0xb<<10)|(1<<18)) 

#define CLE  (0x5<<14)
#define ALE  (0x6<<14)
#define DWR  (0x4<<14)
#define DRD  (0x8<<14)
#define IDLE (0xc<<14)
#define RB   (1<<20) 

#define M2N  ((0<<17) | (2<<20) | (1<<19))
#define N2M  ((1<<17) | (2<<20) | (1<<19))
#define STS  ((3<<17) | (2<<20))
#define ADL  ((0<<16) | (3<<20))
#define ADH  ((1<<16) | (3<<20))
#define AIL  ((2<<16) | (3<<20))
#define AIH  ((3<<16) | (3<<20))
#define ASL  ((4<<16) | (3<<20))
#define ASH  ((5<<16) | (3<<20))
#define SEED ((8<<16) | (3<<20))




#endif
