#include <asm/arch/firm/regs.h>
#include <asm/arch/firm/reg_addr.h>
#include <asm/arch/firm/timing.h>
#include <memtest.h>
#include <config.h>
#include <asm/arch/firm/io.h>
#ifndef STATIC_PREFIX 
#define STATIC_PREFIX 
#endif

STATIC_PREFIX int SPI_page_program(unsigned * addr_source, unsigned spi_addr, int byte_length)
{
   unsigned temp;
   unsigned temp_addr;
   int temp_bl,i;
   
   temp_addr = spi_addr;
   temp_bl = byte_length;
   if(byte_length&0x1f)
        return -1;
   ///remove SPI nor from AHB bus)
   clrbits_le32(P_SPI_FLASH_CTRL,1<<SPI_ENABLE_AHB);
   for(temp_addr=0;temp_addr<byte_length;temp_addr+=32)
   {
        writel(((temp_addr+spi_addr) & 0xffffff)|( 32 << SPI_FLASH_BYTES_LEN ),
            P_SPI_FLASH_ADDR);
        for(i=0;i<8;i++)
        {
            writel(*addr_source++,P_SPI_FLASH_C0+(i<<2));
        }
        writel(1 << SPI_FLASH_WREN,P_SPI_FLASH_CMD);
        while(readl(P_SPI_FLASH_CMD)!=0);
        writel(1 << SPI_FLASH_PP,P_SPI_FLASH_CMD);
        while(readl(P_SPI_FLASH_CMD)!=0);
        do{
            writel(1 << SPI_FLASH_RDSR,P_SPI_FLASH_CMD);
            while(readl(P_SPI_FLASH_CMD)!=0);
        }while(readl(P_SPI_FLASH_STATUS)&1==1);
   }
   
  ///enable SPI nor from AHB bus
  setbits_le32(P_SPI_FLASH_CTRL,1<<SPI_ENABLE_AHB); 
  return 0;
}
STATIC_PREFIX int SPI_sector_erase(unsigned addr )
{
   unsigned temp;
   clrbits_le32(P_SPI_FLASH_CTRL,1<<SPI_ENABLE_AHB);
   writel(addr & 0xffffff,P_SPI_FLASH_ADDR);
   
   //Write Enable 
   writel(1 << SPI_FLASH_WREN,P_SPI_FLASH_CMD);
   while(readl(P_SPI_FLASH_CMD)!=0);
        
   // sector erase  64Kbytes erase is block erase.
   writel(1 << SPI_FLASH_BE,P_SPI_FLASH_CMD);
   while(readl(P_SPI_FLASH_CMD)!=0);
   
   // check erase is finished.
  do{
        writel(1 << SPI_FLASH_RDSR,P_SPI_FLASH_CMD);
        while(readl(P_SPI_FLASH_CMD)!=0);
  }while(readl(P_SPI_FLASH_STATUS)&1==1);
  
  ///enable SPI nor from AHB bus
  setbits_le32(P_SPI_FLASH_CTRL,1<<SPI_ENABLE_AHB); 
  
  return 0;
}

STATIC_PREFIX void spi_erase(void)
{
    spi_pinmux_init();
    writel(__plls.spi_setting,P_SPI_FLASH_CTRL);
    serial_puts("\nErase..");
    SPI_sector_erase(0);
}
STATIC_PREFIX void spi_program(unsigned addr,unsigned size)
{
	serial_puts("\nProgram..");
    SPI_page_program((unsigned *)addr,0,size);
    serial_puts("\nEnd..\n");

}
