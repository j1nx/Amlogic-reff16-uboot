#
# Copyright (C) 2008-2010 ARM Limited                           
#
# This software is provided 'as-is', without any express or implied
# warranties including the implied warranties of satisfactory quality, 
# fitness for purpose or non infringement.  In no event will  ARM be 
# liable for any damages arising from the use of this software.
#
# Permission is granted to anyone to use, copy and modify this software for 
# any purpose, and to redistribute the software, subject to the following 
# restrictions: 
#
# 1. The origin of this software must not be misrepresented; you must not
#    claim that you wrote the original software. If you use this software
#    in a product, an acknowledgment in the product documentation would be
#    appreciated but is not required.                                       
# 2. Altered source versions must be plainly marked as such, and must not be
#    misrepresented as being the original software.
# 3. This notice may not be removed or altered from any source distribution.
#
include $(TOPDIR)/config.mk
OUTDIR = $(SRCTREE)/build/secure
EXTINCDIR=$(SRCTREE)/build/include2
LISTFILE=$(OUTDIR)/symbols.txt

TFMOBJS=src/addsub/fp_add.o src/addsub/fp_add_d.o src/addsub/fp_addmod.o src/addsub/fp_cmp.o \
src/addsub/fp_cmp_d.o src/addsub/fp_cmp_mag.o src/addsub/fp_sub.o src/addsub/fp_sub_d.o \
src/addsub/fp_submod.o src/addsub/s_fp_add.o src/addsub/s_fp_sub.o src/bin/fp_radix_size.o \
src/bin/fp_read_radix.o src/bin/fp_read_signed_bin.o src/bin/fp_read_unsigned_bin.o \
src/bin/fp_reverse.o src/bin/fp_s_rmap.o src/bin/fp_signed_bin_size.o src/bin/fp_to_signed_bin.o \
src/bin/fp_to_unsigned_bin.o src/bin/fp_toradix.o src/bin/fp_unsigned_bin_size.o src/bit/fp_cnt_lsb.o \
src/bit/fp_count_bits.o src/bit/fp_div_2.o src/bit/fp_div_2d.o src/bit/fp_lshd.o src/bit/fp_mod_2d.o \
src/bit/fp_rshd.o src/divide/fp_div.o src/divide/fp_div_d.o src/divide/fp_mod.o src/divide/fp_mod_d.o \
src/exptmod/fp_2expt.o src/exptmod/fp_exptmod.o src/misc/fp_ident.o src/misc/fp_set.o \
src/mont/fp_montgomery_calc_normalization.o src/mont/fp_montgomery_reduce.o \
src/mont/fp_montgomery_setup.o src/mul/fp_mul.o src/mul/fp_mul_2.o src/mul/fp_mul_2d.o \
src/mul/fp_mul_comba.o src/mul/fp_mul_comba_12.o src/mul/fp_mul_comba_17.o src/mul/fp_mul_comba_20.o \
src/mul/fp_mul_comba_24.o src/mul/fp_mul_comba_28.o src/mul/fp_mul_comba_3.o src/mul/fp_mul_comba_32.o \
src/mul/fp_mul_comba_4.o src/mul/fp_mul_comba_48.o src/mul/fp_mul_comba_6.o src/mul/fp_mul_comba_64.o \
src/mul/fp_mul_comba_7.o src/mul/fp_mul_comba_8.o src/mul/fp_mul_comba_9.o \
src/mul/fp_mul_comba_small_set.o src/mul/fp_mul_d.o src/mul/fp_mulmod.o src/numtheory/fp_gcd.o \
src/numtheory/fp_invmod.o src/numtheory/fp_isprime.o src/numtheory/fp_lcm.o \
src/numtheory/fp_prime_miller_rabin.o src/numtheory/fp_prime_random_ex.o src/sqr/fp_sqr.o \
src/sqr/fp_sqr_comba.o src/sqr/fp_sqr_comba_12.o src/sqr/fp_sqr_comba_17.o src/sqr/fp_sqr_comba_20.o \
src/sqr/fp_sqr_comba_24.o src/sqr/fp_sqr_comba_28.o src/sqr/fp_sqr_comba_3.o src/sqr/fp_sqr_comba_32.o \
src/sqr/fp_sqr_comba_4.o src/sqr/fp_sqr_comba_48.o src/sqr/fp_sqr_comba_6.o src/sqr/fp_sqr_comba_64.o \
src/sqr/fp_sqr_comba_7.o src/sqr/fp_sqr_comba_8.o src/sqr/fp_sqr_comba_9.o \
src/sqr/fp_sqr_comba_generic.o src/sqr/fp_sqr_comba_small_set.o src/sqr/fp_sqrmod.o \
src/sha256/sha256calc.o src/sha256/base.o src/sha256/sha256.o
					
OBJECTS = $(addprefix $(OUTDIR)/tfm/, $(TFMOBJS))
APIS =  $(OUTDIR)/entry.o \
			$(OUTDIR)/secure_runtime_apis.o  \
			$(OUTDIR)/secure_boottime_apis.o  \
			$(OUTDIR)/sha256_apis.o \
			$(OUTDIR)/rsa1024.o \
			$(OUTDIR)/efuse.o \
			$(OUTDIR)/secure_info_apis.o \
			$(OUTDIR)/bch_8.o 

			
CPU=cortex-a9
ENTRY_POINT=secure_entry_point
CC=arm-none-eabi-gcc
AS=arm-none-eabi-gcc
LD=arm-none-eabi-ld
AR=arm-none-eabi-ar
fromelf=arm-none-eabi-objcopy			
MAPFILE=secure.lds
LD_LIB_PATH_ARM=$(dir $(shell $(CC) -print-libgcc-file-name)) \
									$(dir $(shell $(CC) -print-file-name=libc.a))
ASFLAGS= -g -mcpu=$(CPU) -mfpu=vfpv3 -c -I. 
CFLAGS= -I. -I./include/ -I$(EXTINCDIR) -gdwarf-2 -mcpu=$(CPU) -g -O2 -fpie -fpic  -mno-long-calls -Wall  -mfpu=vfpv3  
LDFLAGS= -Bstatic -g  -pie -fpic -T $(MAPFILE) $(LD_LIB_PATH_ARM:%=-L%)	--verbose -lm -lc  -lgcc -lgcov -Map $(OUTDIR)/test.map

$(OUTDIR)/%.o : %.S Makefile $(MAPFILE) 
		$(AS) $(ASFLAGS) $< -o $@

all: clean $(OUTDIR)/secure.out $(OUTDIR)/secure.bin $(OUTDIR)/secure_srec.txt

clean:
	rm -rf $(OUTDIR)
	rm -f $(EXTINCDIR)/secure_firmware.dat
	mkdir -p $(OUTDIR)/tfm/src/addsub	
	mkdir -p $(OUTDIR)/tfm/src/bin
	mkdir -p $(OUTDIR)/tfm/src/bit
	mkdir -p $(OUTDIR)/tfm/src/divide
	mkdir -p $(OUTDIR)/tfm/src/exptmod
	mkdir -p $(OUTDIR)/tfm/src/misc
	mkdir -p $(OUTDIR)/tfm/src/mont
	mkdir -p $(OUTDIR)/tfm/src/mul
	mkdir -p $(OUTDIR)/tfm/src/numtheory
	mkdir -p $(OUTDIR)/tfm/src/sha256
	mkdir -p $(OUTDIR)/tfm/src/sqr	
	
$(OUTDIR)/%.o : %.c Makefile $(MAPFILE) 
		$(CC) $(CFLAGS) -c $< -o $@

$(OUTDIR)/secure.out:  $(MAPFILE) Makefile $(OUTDIR)/libtfm.a $(APIS)
	$(LD) $(APIS) $(OUTDIR)/libtfm.a -o $@ $(LDFLAGS)  > $(LISTFILE)	

$(OUTDIR)/libtfm.a: $(OBJECTS) Makefile
	$(AR) r $@ $(filter %.o,$^)		

$(OUTDIR)/secure.bin: $(OUTDIR)/secure.out
	$(fromelf) -O binary $<  $@
	./bin2code secure_firmware_code $@ $(OUTDIR)/secure_firmware.dat
	cp -f $(OUTDIR)/secure_firmware.dat $(SRCTREE)/arch/arm/cpu/aml_meson/m3/secure_firmware.dat
	
$(OUTDIR)/secure_srec.txt: $(OUTDIR)/secure.out
	$(fromelf) --gap-fill 0xff -O srec $< $@	