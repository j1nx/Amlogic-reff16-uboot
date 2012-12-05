#include <common.h>
#include <mmc.h>
#include <asm/arch/am_reg_addr.h>
#include <asm/arch/aml_card_sd.h>

static struct aml_card_sd_info m1_sdio_ports[]={
    { .sdio_port=SDIO_PORT_A,.name="SDIO Port A"},
    { .sdio_port=SDIO_PORT_B,.name="SDIO Port B"},
    { .sdio_port=SDIO_PORT_C,.name="SDIO Port C"},
    
};
struct aml_card_sd_info * cpu_sdio_get(unsigned port)
{
    if(port<SDIO_PORT_C+1)
        return &m1_sdio_ports[port];
    return NULL;
}

void  cpu_sdio_pwr_prepare(unsigned port)
{
    switch(port)
    {
        case SDIO_PORT_A:
            
            clrbits_le32(P_PERIPHS_PIN_MUX_0,0x3f<<10);
            clrbits_le32(P_PREG_EGPIO_EN_N,(0x1<<31));
            clrbits_le32(P_PREG_EGPIO_O,(0x1<<31));
            clrbits_le32(P_PREG_FGPIO_EN_N,0x1f);
            clrbits_le32(P_PREG_FGPIO_O,0x1f);
            
            break;
        case SDIO_PORT_B://SDIOB,GPIOA_0~GPIOA_5
            //diable SDIO_B1
            clrbits_le32(P_PERIPHS_PIN_MUX_0,0x3f<<22);
            clrbits_le32(P_PREG_FGPIO_EN_N,(0x3f<<5));
            clrbits_le32(P_PREG_FGPIO_O,(0x3f<<5));
            ///@todo pull down these bits
            break;
        case SDIO_PORT_C://SDIOC GPIOB_2~GPIOB_7
            clrbits_le32(P_PERIPHS_PIN_MUX_2,0x3f);
            clrbits_le32(P_PREG_EGPIO_EN_N,(0xff<<15));
            clrsetbits_le32(P_PREG_EGPIO_O,(0xff<<15),(0x3<<15));
            ///@todo pull down these bits
            break;
    }
    
    /**
        do nothing here
    */
}
int cpu_sdio_init(unsigned port)
{
    switch(port)
    {
        case SDIO_PORT_A://SDIOA,GPIOA_9~GPIOA_14
            setbits_le32(P_PERIPHS_PIN_MUX_0,0x3f<<10);
            break;
        case SDIO_PORT_B://SDIOB,GPIOA_0~GPIOA_5
            setbits_le32(P_PERIPHS_PIN_MUX_0,0x3f<<22);
            break;
        case SDIO_PORT_C://SDIOC GPIOB_2~GPIOB_7
            setbits_le32(P_PERIPHS_PIN_MUX_2,0x3f);
            break;
        default:
            return -1;
    }
    return 0;
}
int cpu_sdio_power_on(unsigned port)
{
    if (port)
            {
                clrbits_le32(P_PREG_EGPIO_EN_N,(0x1<<8));
                clrbits_le32(P_PREG_EGPIO_O,(0x1<<8));
            
                
            }
            else
            {
                clrbits_le32(P_PREG_FGPIO_EN_N,(0x1<<25));
                clrbits_le32(P_PREG_FGPIO_O,(0x1<<25));
            }
    return 0;
}
int cpu_sdio_power_off(unsigned port)
{
    if (port)
    {
        clrbits_le32(P_PREG_EGPIO_EN_N,(0x1<<8));
        setbits_le32(P_PREG_EGPIO_O,(0x1<<8));
    
        
    }
    else
    {
        clrbits_le32(P_PREG_FGPIO_EN_N,(0x1<<25));
        setbits_le32(P_PREG_FGPIO_O,(0x1<<25));
    }
    return 0;
}
