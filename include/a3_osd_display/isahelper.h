/*******************************************************************
 * 
 *  Copyright C 2008 by Amlogic, Inc. All Rights Reserved.
 *
 *  Description: 
 *      ISA register helper defines
 *  Author: Amlogic Software
 *  Created: 10/17/2008 3:31:39 PM
 *
 *******************************************************************/
#ifndef ISAHELPER_H
#define ISAHELPER_H

#include "volatile_read.h"

#define ISABASE                      0xc1100000

#define WRITE_ISA_REG(reg, val) *(volatile unsigned *)(ISABASE + ((reg)<<2)) = (val)
#define READ_ISA_REG(reg) (READ_VOLATILE_UINT32(ISABASE + ((reg)<<2)))

#define CLEAR_ISA_REG_MASK(reg, mask) WRITE_ISA_REG(reg, (READ_ISA_REG(reg)&(~mask)))
#define SET_ISA_REG_MASK(reg, mask)   WRITE_ISA_REG(reg, (READ_ISA_REG(reg)|(mask)))

/* DMA Routines **********************************************/

#define ide_dma_started()                                               \
    ((READ_ISA_REG(IREG_IDE_DMA_REG0)&IDE_DMA_START) == IDE_DMA_START)

#define ide_dma_is_busy()                                               \
    ((READ_ISA_REG(IREG_IDE_DMA_REG0)&IDE_DMA_STATUS_MASK) == IDE_DMA_IN_PROGRESS)

#define set_ide_dma_address(addr)                       \
    WRITE_ISA_REG(IREG_IDE_DMA_TABLE, (unsigned) addr)

#define read_next_ide_dma_address()                       \
    READ_ISA_REG(IREG_IDE_DMA_TABLE)

#define add_ide_dma_table(num)                       \
    WRITE_ISA_REG(IREG_TABLE_ADD_REG,num)

#define read_ide_dma_table_remain()                       \
    READ_ISA_REG(IREG_TABLE_ADD_REG)

#define halt_ide_dma() \
    CLEAR_ISA_REG_MASK(IREG_IDE_DMA_REG0, IDE_DMA_START)

#define start_ide_dma()                                 \
    SET_ISA_REG_MASK(IREG_IDE_DMA_REG0, IDE_DMA_START)

/* Interrupt routines ******************************************/
#define set_irq_mask(mask)                      \
    SET_ISA_REG_MASK(IREG_ARC1_GEN_IRQ_MASK0, mask)
#define set_irq_mask1(mask)                      \
    SET_ISA_REG_MASK(IREG_ARC1_GEN_IRQ_MASK1, mask)

#define set_arc2_irq_mask(mask)                      \
    SET_ISA_REG_MASK(IREG_ARC2_GEN_IRQ_MASK0, mask)
#define set_arc2_irq_mask1(mask)                      \
    SET_ISA_REG_MASK(IREG_ARC2_GEN_IRQ_MASK1, mask)
    
#define get_irq_mask()                          \
    READ_ISA_REG(IREG_ARC1_GEN_IRQ_MASK0)
#define get_irq_mask1()                          \
    READ_ISA_REG(IREG_ARC1_GEN_IRQ_MASK1)

#define get_arc2_irq_mask()                          \
    READ_ISA_REG(IREG_ARC2_GEN_IRQ_MASK0)
#define get_arc2_irq_mask1()                          \
    READ_ISA_REG(IREG_ARC2_GEN_IRQ_MASK1)
    
#define clear_irq_mask(mask)                    \
    CLEAR_ISA_REG_MASK(IREG_ARC1_GEN_IRQ_MASK0, mask)
#define clear_irq_mask1(mask)                    \
    CLEAR_ISA_REG_MASK(IREG_ARC1_GEN_IRQ_MASK1, mask)

#define clear_arc2_irq_mask(mask)                    \
    CLEAR_ISA_REG_MASK(IREG_ARC2_GEN_IRQ_MASK0, mask)
#define clear_arc2_irq_mask1(mask)                    \
    CLEAR_ISA_REG_MASK(IREG_ARC2_GEN_IRQ_MASK1, mask)
    
#define set_firq_mask(mask)                     \
    SET_ISA_REG_MASK(IREG_ARC1_GEN_FIRQ_MASK0, mask)
#define set_firq_mask1(mask)                     \
    SET_ISA_REG_MASK(IREG_ARC1_GEN_FIRQ_MASK1, mask)

#define set_arc2_firq_mask(mask)                     \
    SET_ISA_REG_MASK(IREG_ARC2_GEN_FIRQ_MASK0, mask)
#define set_arc2_firq_mask1(mask)                     \
    SET_ISA_REG_MASK(IREG_ARC2_GEN_FIRQ_MASK1, mask)
    
#define get_firq_mask()                     \
    READ_ISA_REG(IREG_ARC1_GEN_FIRQ_MASK0)
#define get_firq_mask1()                     \
    READ_ISA_REG(IREG_ARC1_GEN_FIRQ_MASK1)

#define get_arc2_firq_mask()                     \
    READ_ISA_REG(IREG_ARC2_GEN_FIRQ_MASK0)
#define get_arc2_firq_mask1()                     \
    READ_ISA_REG(IREG_ARC2_GEN_FIRQ_MASK1)
    
#define clear_firq_mask(mask)                       \
    CLEAR_ISA_REG_MASK(IREG_ARC1_GEN_FIRQ_MASK0, mask)
#define clear_firq_mask1(mask)                       \
    CLEAR_ISA_REG_MASK(IREG_ARC1_GEN_FIRQ_MASK1, mask)

#define clear_arc2_firq_mask(mask)                       \
    CLEAR_ISA_REG_MASK(IREG_ARC2_GEN_FIRQ_MASK0, mask)
#define clear_arc2_firq_mask1(mask)                       \
    CLEAR_ISA_REG_MASK(IREG_ARC2_GEN_FIRQ_MASK1, mask)
    
#define read_irq_status()                       \
    READ_ISA_REG(IREG_ARC1_GEN_IRQ_STATUS0)
#define read_irq_status1()                       \
    READ_ISA_REG(IREG_ARC1_GEN_IRQ_STATUS1)

#define read_arc2_irq_status()                       \
    READ_ISA_REG(IREG_ARC2_GEN_IRQ_STATUS0)
#define read_arc2_irq_status1()                       \
    READ_ISA_REG(IREG_ARC2_GEN_IRQ_STATUS1)
    
#define clear_irq_status(mask)                  \
    WRITE_ISA_REG(IREG_ARC1_GEN_IRQ_CLEAR0, mask)
#define clear_irq_status1(mask)                  \
    WRITE_ISA_REG(IREG_ARC1_GEN_IRQ_CLEAR1, mask)

#define clear_arc2_irq_status(mask)                  \
    WRITE_ISA_REG(IREG_ARC2_GEN_IRQ_CLEAR0, mask)
#define clear_arc2_irq_status1(mask)                  \
    WRITE_ISA_REG(IREG_ARC2_GEN_IRQ_CLEAR1, mask)
    
/* AMRISC Interrupt routines ***********************************/
#define set_amrisc_irq_mask(mask)                   \
    SET_ISA_REG_MASK(IREG_ARC1_AMRISC_IRQ_MASK, mask)
#define get_amrisc_irq_mask()                   \
    READ_ISA_REG(IREG_ARC1_AMRISC_IRQ_MASK)
#define clear_amrisc_irq_mask(mask)                 \
    CLEAR_ISA_REG_MASK(IREG_ARC1_AMRISC_IRQ_MASK, mask)
#define set_amrisc_firq_mask(mask)                  \
    SET_ISA_REG_MASK(IREG_ARC1_AMRISC_FIRQ_MASK, mask)
#define get_amrisc_firq_mask()                  \
    READ_ISA_REG(IREG_ARC1_AMRISC_FIRQ_MASK)
#define clear_amrisc_firq_mask()                    \
    CLEAR_ISA_REG_MASK(IREG_ARC1_AMRISC_FIRQ_MASK, mask)
#define read_amrisc_irq_status()                \
    READ_ISA_REG(IREG_ARC1_AMRISC_IRQ_STATUS)
#define clear_amrisc_irq_status(mask)           \
    WRITE_ISA_REG(IREG_ARC1_AMRISC_IRQ_CLEAR, mask)

#define set_arc2_amrisc_irq_mask(mask)                   \
    SET_ISA_REG_MASK(IREG_ARC2_AMRISC_IRQ_MASK, mask)
#define get_arc2_amrisc_irq_mask()                   \
    READ_ISA_REG(IREG_ARC2_AMRISC_IRQ_MASK)
#define clear_arc2_amrisc_irq_mask(mask)                 \
    CLEAR_ISA_REG_MASK(IREG_ARC2_AMRISC_IRQ_MASK, mask)
#define set_arc2_amrisc_firq_mask(mask)                  \
    SET_ISA_REG_MASK(IREG_ARC2_AMRISC_FIRQ_MASK, mask)
#define get_arc2_amrisc_firq_mask()                  \
    READ_ISA_REG(IREG_ARC2_AMRISC_FIRQ_MASK)
#define clear_arc2_amrisc_firq_mask()                    \
    CLEAR_ISA_REG_MASK(IREG_ARC2_AMRISC_FIRQ_MASK, mask)
#define read_arc2_amrisc_irq_status()                \
    READ_ISA_REG(IREG_ARC2_AMRISC_IRQ_STATUS)
#define clear_arc2_amrisc_irq_status(mask)           \
    WRITE_ISA_REG(IREG_ARC2_AMRISC_IRQ_CLEAR, mask)
    
/* GPIO Interrupt routines ****************************************/
#define set_gpio_irq_mask(mask)                 \
    SET_ISA_REG_MASK(IREG_ARC1_GPIO_IRQ_MASK, mask)
#define get_gpio_irq_mask()                     \
    READ_ISA_REG(IREG_ARC1_GPIO_IRQ_MASK)
#define clear_gpio_irq_mask(mask)                   \
    CLEAR_ISA_REG_MASK(IREG_ARC1_GPIO_IRQ_MASK, mask)
#define set_gpio_firq_mask(mask)                \
    SET_ISA_REG_MASK(IREG_ARC1_GPIO_FIRQ_MASK, mask)
#define get_gpio_firq_mask()                    \
    READ_ISA_REG(IREG_ARC1_GPIO_FIRQ_MASK)
#define clear_gpio_firq_mask(mask)                  \
    CLEAR_ISA_REG_MASK(IREG_ARC1_GPIO_FIRQ_MASK, mask)
#define read_gpio_irq_status()                  \
    READ_ISA_REG(IREG_ARC1_GPIO_IRQ_STATUS)
#define clear_gpio_irq_status(mask)             \
    WRITE_ISA_REG(IREG_ARC1_GPIO_IRQ_CLEAR, mask)

#define set_arc2_gpio_irq_mask(mask)                 \
    SET_ISA_REG_MASK(IREG_ARC2_GPIO_IRQ_MASK, mask)
#define get_arc2_gpio_irq_mask()                     \
    READ_ISA_REG(IREG_ARC2_GPIO_IRQ_MASK)
#define clear_arc2_gpio_irq_mask(mask)                   \
    CLEAR_ISA_REG_MASK(IREG_ARC2_GPIO_IRQ_MASK, mask)
#define set_arc2_gpio_firq_mask(mask)                \
    SET_ISA_REG_MASK(IREG_ARC2_GPIO_FIRQ_MASK, mask)
#define get_arc2_gpio_firq_mask()                    \
    READ_ISA_REG(IREG_ARC2_GPIO_FIRQ_MASK)
#define clear_arc2_gpio_firq_mask(mask)                  \
    CLEAR_ISA_REG_MASK(IREG_ARC2_GPIO_FIRQ_MASK, mask)
#define read_arc2_gpio_irq_status()                  \
    READ_ISA_REG(IREG_ARC2_GPIO_IRQ_STATUS)
#define clear_arc2_gpio_irq_status(mask)             \
    WRITE_ISA_REG(IREG_ARC2_GPIO_IRQ_CLEAR, mask)
    
/* DMA Routines **********************************************/

#define ide_dma_started()                                               \
    ((READ_ISA_REG(IREG_IDE_DMA_REG0)&IDE_DMA_START) == IDE_DMA_START)

#define ide_dma_is_busy()                                               \
    ((READ_ISA_REG(IREG_IDE_DMA_REG0)&IDE_DMA_STATUS_MASK) == IDE_DMA_IN_PROGRESS)

#define set_ide_dma_address(addr)                       \
    WRITE_ISA_REG(IREG_IDE_DMA_TABLE, (unsigned) addr)

#define read_next_ide_dma_address()                       \
    READ_ISA_REG(IREG_IDE_DMA_TABLE)

#define add_ide_dma_table(num)                       \
    WRITE_ISA_REG(IREG_TABLE_ADD_REG,num)

#define read_ide_dma_table_remain()                       \
    READ_ISA_REG(IREG_TABLE_ADD_REG)

#define halt_ide_dma() \
    CLEAR_ISA_REG_MASK(IREG_IDE_DMA_REG0, IDE_DMA_START)

#define start_ide_dma()                                 \
    SET_ISA_REG_MASK(IREG_IDE_DMA_REG0, IDE_DMA_START)
    
#define enable_viu_with_dcu2()                                 \
    SET_ISA_REG_MASK(IREG_SDRAM_CTRL, SC0_FLAG_VIU_EN)
#define disable_viu_with_dcu2()                                    \
    CLEAR_ISA_REG_MASK(IREG_SDRAM_CTRL, SC0_FLAG_VIU_EN)

#define map_eeprom_to_sdram()                           \
    SET_ISA_REG_MASK(IREG_SDRAM_CTRL, SC0_FLAG_SDRAM)
#define map_eeprom_not_to_sdram()                           \
    CLEAR_ISA_REG_MASK(IREG_SDRAM_CTRL, SC0_FLAG_SDRAM)
    

/* RTC routines ***************************************************/
#define PRECISE_CUR_TIME   READ_ISA_REG(IREG_TIMER_C_COUNT)
#define set_timer_base(n) WRITE_ISA_REG(IREG_TIMER_BASE, n) 
#define read_timer_base() READ_ISA_REG(IREG_TIMER_BASE)

#endif /* ISAHELPER_H */
