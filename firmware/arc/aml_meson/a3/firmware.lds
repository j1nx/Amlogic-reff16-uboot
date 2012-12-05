#include <asm/arch/romboot.h>
OUTPUT_FORMAT("elf32-littlearc", "elf32-littlearc", "elf32-littlearc")
OUTPUT_ARCH(arc)
ENTRY(_start)
SECTIONS
{
	. = RAM_START;
	
	. = ALIGN(4);
	.start      :
	{
	  *rom_spl*.o	(.text)
	
	}
	. = ALIGN(4);
	.setting		:
	{
		* (.setting)
	}
	. =  RAM_START+0x1b0;
	.magic :{
	  *magic.o(.text)
	}
	
	. = RAM_START+0x200;
	. = ALIGN(4);
	
	
	. = ALIGN(4);
	.text			:
	{
		*  (.text)
	}
	.rodata : { *(SORT_BY_ALIGNMENT(SORT_BY_NAME(.rodata*))) }
	.data : { *(.data) }
	 .sdata          :
  {
    __SDATA_BEGIN__ = .;
    *(.sdata .sdata.* .gnu.linkonce.s.*)
  }

	_end = .;
	. = ALIGN(4);
	_bssstart = . ;
	.bss : { *(.bss) }
	. = ALIGN(4);
	_bssend = . ;
	/* Stabs debugging sections.	*/
	.stab 0 : { *(.stab) }
	.stabstr 0 : { *(.stabstr) }
	.stab.excl 0 : { *(.stab.excl) }
	.stab.exclstr 0 : { *(.stab.exclstr) }
	.stab.index 0 : { *(.stab.index) }
	.stab.indexstr 0 : { *(.stab.indexstr) }
	.comment 0 : { *(.comment) }
	.debug_ranges 0 : { *(.debug_ranges) }
	.debug_aranges 0 : { *(.debug_aranges) }
	.debug_frame  0: { *(.debug_frame) }
	.debug_pubnames 0 : { *(.debug_pubnames) }
	.debug_info 0 : { *(.debug_info) }
	.debug_abbrev 0 : { *(.debug_abbrev) }
	.debug_line 0 : { *(.debug_line) }
	.debug_str 0 : { *(.debug_str) }
	.debug_loc 0 : { *(.debug_loc) }
	.debug_macinfo 0 : { *(.debug_macinfo) }
}
ASSERT(_end <= RAM_START+READ_SIZE, "ROM BOOT bootstrap too big");
ASSERT(_bssend <= ROM_STACK_END - 256, "ROM BOOT bss  too big");
