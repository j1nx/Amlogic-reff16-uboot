#ifndef _APB_REG_A3_H
#define _APB_REG_A3_H
#define APB_BASE_ADDR 0xd0040000

#define APB_ADAC_RESET                		(0x4000+0x00*4)
#define APB_ADAC_LATCH                		(0x4000+0x01*4)
#define APB_ADAC_CLOCK                		(0x4000+0x02*4)
#define APB_ADAC_I2S_CONFIG_REG1      		(0x4000+0x0c*4)
#define APB_ADAC_I2S_CONFIG_REG2      		(0x4000+0x0d*4)
#define APB_ADAC_POWER_CTRL_REG1      		(0x4000+0x10*4)
#define APB_ADAC_POWER_CTRL_REG2      		(0x4000+0x11*4)
#define APB_ADAC_POWER_CTRL_REG3      		(0x4000+0x12*4)
#define APB_ADAC_MUTE_CTRL_REG1       		(0x4000+0x18*4)
#define APB_ADAC_DAC_ADC_MIXER        		(0x4000+0x1a*4)
#define APB_ADAC_PLAYBACK_VOL_CTRL_LSB      (0x4000+0x20*4)
#define APB_ADAC_PLAYBACK_VOL_CTRL_MSB      (0x4000+0x21*4)
#define APB_ADAC_STEREO_HS_VOL_CTRL_LSB     (0x4000+0x22*4)
#define APB_ADAC_STEREO_HS_VOL_CTRL_MSB     (0x4000+0x23*4)
#endif
