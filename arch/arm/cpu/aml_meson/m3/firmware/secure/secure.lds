OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(secure_entry_point)

SECTIONS 
{
    . = 0x9FE00000;
    . = ALIGN(4); 	 
    .SECURE_BOOT_ENTRY :
    {
    		*(SECURE_BOOT_ENTRY)
    }
    
 	. = 0x9FE01000; 
 	. = ALIGN(4); 	 
 	.text   :
     {
     	*entry.o (.text)          
          *(.text .text* )                            
     }
    	
    . = ALIGN(4);
    .rodata : { *(.rodata* .rodata)}     	
        
    . = ALIGN(4);        
    __data_start = .;
    .data : { *(.data*)} 
   __data_end = .;
    
    	_end = .;
    . = ALIGN(4);      
   __bss_start = .;
   .bss : { *(.bss) } 
   . = ALIGN(32);
   __bss_end = .;     
   
}
