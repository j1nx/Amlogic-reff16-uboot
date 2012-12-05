/*
 * (C) Copyright C 2005 by Amlogic, Inc.
 *
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

/*
 * CPU specific code
 */


#include <common.h>
#include <command.h>
#include <asm/system.h>

static char * str_1st_str[]={
    "Disable","SDIO A","SDIO B","SDIO C",
    "SPI Flash","SPI Flash",
    "NAND Flash RB Mode",
    "NAND Flash Without RB"
    };

#define get_1st_str(a) str_1st_str[a]
#define get_2nd_str(a) str_1st_str[a]

void print_romboot(void)
{
    int i;
    unsigned por = C_ROM_BOOT_DEBUG_INFO->por_cfg;
    printf("ROMBOOT Mode is %s\n", (POR_ROM_BOOT_ENABLE&por)?"Enable":"Disable");
    printf("JTAG    Mode is %s\n", (POR_JTAG_ENABLE&por)?"Enable":"Disable");
    printf("1st   Device is %s\n", get_1st_str(POR_GET_1ST_CFG(por)));
    printf("2nd   Device is %s\n", get_2nd_str(POR_GET_2ND_CFG(por)));
    printf("USB     Mode is %s\n", (POR_GET_USB_CFG(por))?"Enable":"Disable");
    printf("Now we are boot from %s Device\n",C_ROM_BOOT_DEBUG_INFO->boot_id?"2nd":"1st");
    for(i=0;i<2;i++)
    {
        if(i)
            printf("2nd Device load info \n");
        else            
            printf("1st Device load info \n");		
        
		printf("area %d load result %x\n",i,C_ROM_BOOT_DEBUG_INFO->load[i]);
        printf("area %d check result %x\n",i,C_ROM_BOOT_DEBUG_INFO->dchk[i]);
            
    }    
    
}
