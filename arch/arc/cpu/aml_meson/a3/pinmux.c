/*
Linux PINMUX.C
#include <asm/arch
*/
#include <asm/arch/am_regs.h>
#include <asm/arch/am_eth_reg.h>

#include <asm/arch/pinmux.h>

int  clear_mio_mux(unsigned mux_index, unsigned mux_mask)
{
    unsigned mux_reg[] = {PERIPHS_PIN_MUX_0, PERIPHS_PIN_MUX_1, PERIPHS_PIN_MUX_2,PERIPHS_PIN_MUX_3,
		PERIPHS_PIN_MUX_4,PERIPHS_PIN_MUX_5,PERIPHS_PIN_MUX_6,PERIPHS_PIN_MUX_7,PERIPHS_PIN_MUX_8,
		PERIPHS_PIN_MUX_9,PERIPHS_PIN_MUX_10,PERIPHS_PIN_MUX_11,PERIPHS_PIN_MUX_12};
    if (mux_index < 13) {
        CLEAR_CBUS_REG_MASK(mux_reg[mux_index], mux_mask);
		return 0;
    }
	return -1;
}

int  set_mio_mux(unsigned mux_index, unsigned mux_mask)
{
    unsigned mux_reg[] = {PERIPHS_PIN_MUX_0, PERIPHS_PIN_MUX_1, PERIPHS_PIN_MUX_2,PERIPHS_PIN_MUX_3,
		PERIPHS_PIN_MUX_4,PERIPHS_PIN_MUX_5,PERIPHS_PIN_MUX_6,PERIPHS_PIN_MUX_7,PERIPHS_PIN_MUX_8,
		PERIPHS_PIN_MUX_9,PERIPHS_PIN_MUX_10,PERIPHS_PIN_MUX_11,PERIPHS_PIN_MUX_12};
    if (mux_index < 13) {
        SET_CBUS_REG_MASK(mux_reg[mux_index], mux_mask);
		return 0;
    }
	return -1;
}



/*
call it before pinmux init;
call it before soft reset;
*/
void   clearall_pinmux(void)
{
	int i;
	for(i=0;i<13;i++)
		clear_mio_mux(i,0xffffffff);
	return;
}
int eth_clearall_pinmux(void)
{
	clear_mio_mux(ETH_BANK0_REG1,ETH_BANK0_REG1_VAL);
	clear_mio_mux(ETH_BANK1_REG1,ETH_BANK1_REG1_VAL);
	return 0;
}

/*ETH PINMUX SETTING
More details can get from am_eth_pinmux.h
*/
int eth_set_pinmux(int bank_id,int clk_in_out_id,unsigned long ext_msk)
{
	int ret=0;
	switch(bank_id)
	{
		case	ETH_BANK0_GPIOC3_C11:
				if(ext_msk>0)
					set_mio_mux(ETH_BANK0_REG1,ext_msk);
				else
					set_mio_mux(ETH_BANK0_REG1,ETH_BANK0_REG1_VAL);
				break;
		case 	ETH_BANK1_GPIOD0_D8:
				if(ext_msk>0)
					set_mio_mux(ETH_BANK1_REG1,ext_msk);
				else
					set_mio_mux(ETH_BANK1_REG1,ETH_BANK1_REG1_VAL);
				break;
		default:
				printf("UNknow pinmux setting of ethernet!error bankid=%d,must be 0-2\n",bank_id);
				ret=-1;
				
	}
	switch(clk_in_out_id)
	{
 		case  ETH_CLK_IN_GPIOC2_REG2_18:
				set_mio_mux(2,1<<18);	
				break;
		case  ETH_CLK_IN_GPIOD9_REG4_0:
				set_mio_mux(4,1<<0);	
				break;
		case  ETH_CLK_OUT_GPIOC2_REG2_17:
				set_mio_mux(2,1<<17);	
				break;
 		case  ETH_CLK_OUT_GPIOD9_REG4_1:
				set_mio_mux(4,1<<1);	
				break;
		default:
				printf("UNknow clk_in_out_id setting of ethernet!error clk_in_out_id=%d,must be 0-3\n",clk_in_out_id);
				ret=-1;
	}
	return ret;
}




