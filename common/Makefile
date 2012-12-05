#
# (C) Copyright 2004-2006
# Wolfgang Denk, DENX Software Engineering, wd@denx.de.
#
# See file CREDITS for list of people who contributed to this
# project.
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License as
# published by the Free Software Foundation; either version 2 of
# the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston,
# MA 02111-1307 USA
#

include $(TOPDIR)/config.mk

LIB	= $(obj)libcommon.a

AOBJS	=

# core
COBJS-y += main.o
COBJS-y += console.o
COBJS-y += command.o
COBJS-y += dlmalloc.o
COBJS-y += exports.o
COBJS-$(CONFIG_SYS_HUSH_PARSER) += hush.o
COBJS-y += image.o
COBJS-y += memsize.o
COBJS-y += s_record.o
COBJS-$(CONFIG_SERIAL_MULTI) += serial.o
COBJS-y += stdio.o
COBJS-y += xyzModem.o

# core command
COBJS-y += cmd_boot.o
COBJS-y += cmd_bootm.o
COBJS-y += cmd_help.o
COBJS-y += cmd_nvedit.o
COBJS-y += cmd_version.o


COBJS-$(CONFIG_ENABLE_ARC) += cmd_enblearc.o
#gui
COBJS-$(CONFIG_AML_FONT) += devfont.o

# environment
COBJS-y += env_common.o
COBJS-$(CONFIG_ENV_IS_IN_DATAFLASH) += env_dataflash.o
COBJS-$(CONFIG_ENV_IS_IN_EEPROM) += env_eeprom.o
COBJS-$(CONFIG_ENV_IS_EMBEDDED) += env_embedded.o
COBJS-$(CONFIG_ENV_IS_IN_EEPROM) += env_embedded.o
COBJS-$(CONFIG_ENV_IS_IN_FLASH) += env_embedded.o
COBJS-$(CONFIG_ENV_IS_IN_NVRAM) += env_embedded.o
COBJS-$(CONFIG_ENV_IS_IN_FLASH) += env_flash.o
COBJS-$(CONFIG_ENV_IS_IN_MG_DISK) += env_mgdisk.o
COBJS-$(CONFIG_ENV_IS_IN_NAND) += env_nand.o
COBJS-$(CONFIG_ENV_IS_IN_AML_NAND) += env_amlnand.o
COBJS-$(CONFIG_ENV_IS_IN_NVRAM) += env_nvram.o
COBJS-$(CONFIG_ENV_IS_IN_ONENAND) += env_onenand.o
COBJS-$(CONFIG_ENV_IS_IN_SPI_FLASH) += env_sf.o
COBJS-$(CONFIG_ENV_IS_IN_EMMC) += env_emmc.o
COBJS-$(CONFIG_ENV_IS_NOWHERE) += env_nowhere.o

# command
COBJS-$(CONFIG_SECURITYKEY) += cmd_secukey.o
COBJS-$(CONFIG_CMD_AMBAPP) += cmd_ambapp.o
COBJS-$(CONFIG_CMD_AUTOSCRIPT) += cmd_autoscript.o
COBJS-$(CONFIG_SOURCE) += cmd_source.o
COBJS-$(CONFIG_CMD_SOURCE) += cmd_source.o
COBJS-$(CONFIG_CMD_BDI) += cmd_bdinfo.o
COBJS-$(CONFIG_CMD_BEDBUG) += bedbug.o cmd_bedbug.o
COBJS-$(CONFIG_CMD_BMP) += cmd_bmp.o
COBJS-$(CONFIG_CMD_BOOTLDR) += cmd_bootldr.o
COBJS-$(CONFIG_CMD_CACHE) += cmd_cache.o
COBJS-$(CONFIG_CMD_CONSOLE) += cmd_console.o
COBJS-$(CONFIG_CMD_CPLBINFO) += cmd_cplbinfo.o
COBJS-$(CONFIG_DATAFLASH_MMC_SELECT) += cmd_dataflash_mmc_mux.o
COBJS-$(CONFIG_CMD_DATE) += cmd_date.o
ifdef CONFIG_4xx
COBJS-$(CONFIG_CMD_SETGETDCR) += cmd_dcr.o
endif
ifdef CONFIG_POST
COBJS-$(CONFIG_CMD_DIAG) += cmd_diag.o
endif
COBJS-$(CONFIG_CMD_DISPLAY) += cmd_display.o
COBJS-$(CONFIG_CMD_DTT) += cmd_dtt.o
COBJS-$(CONFIG_CMD_ECHO) += cmd_echo.o
COBJS-$(CONFIG_ENV_IS_IN_EEPROM) += cmd_eeprom.o
COBJS-$(CONFIG_CMD_EEPROM) += cmd_eeprom.o
COBJS-$(CONFIG_CMD_ELF) += cmd_elf.o
COBJS-$(CONFIG_SYS_HUSH_PARSER) += cmd_exit.o
COBJS-$(CONFIG_CMD_EXT2) += cmd_ext2.o
COBJS-$(CONFIG_CMD_FAT) += cmd_fat.o
COBJS-$(CONFIG_CMD_FDC)$(CONFIG_CMD_FDOS) += cmd_fdc.o
COBJS-$(CONFIG_OF_LIBFDT) += cmd_fdt.o fdt_support.o
COBJS-$(CONFIG_CMD_FDOS) += cmd_fdos.o
COBJS-$(CONFIG_CMD_FLASH) += cmd_flash.o
ifdef CONFIG_FPGA
COBJS-$(CONFIG_CMD_FPGA) += cmd_fpga.o
endif
COBJS-$(CONFIG_CMD_I2C) += cmd_i2c.o
COBJS-$(CONFIG_CMD_IDE) += cmd_ide.o
COBJS-$(CONFIG_CMD_IMMAP) += cmd_immap.o
COBJS-$(CONFIG_CMD_IRQ) += cmd_irq.o
COBJS-$(CONFIG_CMD_ITEST) += cmd_itest.o
COBJS-$(CONFIG_CMD_JFFS2) += cmd_jffs2.o
COBJS-$(CONFIG_CMD_CRAMFS) += cmd_cramfs.o
COBJS-$(CONFIG_CMD_LICENSE) += cmd_license.o
COBJS-y += cmd_load.o
COBJS-$(CONFIG_LOGBUFFER) += cmd_log.o
COBJS-$(CONFIG_ID_EEPROM) += cmd_mac.o
COBJS-$(CONFIG_CMD_MEMORY) += cmd_mem.o
COBJS-$(CONFIG_CMD_MFSL) += cmd_mfsl.o
COBJS-$(CONFIG_CMD_MG_DISK) += cmd_mgdisk.o
COBJS-$(CONFIG_MII) += miiphyutil.o
COBJS-$(CONFIG_CMD_MII) += miiphyutil.o
COBJS-$(CONFIG_CMD_MII) += cmd_mii.o
COBJS-$(CONFIG_CMD_MISC) += cmd_misc.o
COBJS-$(CONFIG_CMD_MMC) += cmd_mmc.o
COBJS-$(CONFIG_MP) += cmd_mp.o
COBJS-$(CONFIG_CMD_MTDPARTS) += cmd_mtdparts.o
COBJS-$(CONFIG_CMD_NAND) += cmd_nand.o
COBJS-$(CONFIG_CMD_NET) += cmd_net.o
COBJS-$(CONFIG_CMD_ONENAND) += cmd_onenand.o
COBJS-$(CONFIG_CMD_OTP) += cmd_otp.o
ifdef CONFIG_PCI
COBJS-$(CONFIG_CMD_PCI) += cmd_pci.o
endif
COBJS-y += cmd_pcmcia.o
COBJS-$(CONFIG_CMD_PORTIO) += cmd_portio.o
COBJS-$(CONFIG_CMD_REGINFO) += cmd_reginfo.o
COBJS-$(CONFIG_CMD_REISER) += cmd_reiser.o
COBJS-$(CONFIG_CMD_SATA) += cmd_sata.o
COBJS-$(CONFIG_CMD_SF) += cmd_sf.o
COBJS-$(CONFIG_CMD_SCSI) += cmd_scsi.o
COBJS-$(CONFIG_CMD_SETEXPR) += cmd_setexpr.o
COBJS-$(CONFIG_CMD_SPI) += cmd_spi.o
COBJS-$(CONFIG_CMD_SPIBOOTLDR) += cmd_spibootldr.o
COBJS-$(CONFIG_CMD_STRINGS) += cmd_strings.o
COBJS-$(CONFIG_CMD_TERMINAL) += cmd_terminal.o
COBJS-$(CONFIG_SYS_HUSH_PARSER) += cmd_test.o
COBJS-$(CONFIG_CMD_TSI148) += cmd_tsi148.o
COBJS-$(CONFIG_CMD_UBI) += cmd_ubi.o
COBJS-$(CONFIG_CMD_UBIFS) += cmd_ubifs.o
COBJS-$(CONFIG_CMD_UNIVERSE) += cmd_universe.o
ifdef CONFIG_CMD_USB
COBJS-y += cmd_usb.o
COBJS-y += usb.o
COBJS-$(CONFIG_USB_STORAGE) += usb_storage.o
endif
COBJS-$(CONFIG_CMD_XIMG) += cmd_ximg.o
COBJS-$(CONFIG_YAFFS2) += cmd_yaffs2.o
COBJS-$(CONFIG_VFD) += cmd_vfd.o
ifdef CONFIG_AML_MESION_3
COBJS-$(CONFIG_EFUSE) += cmd_efuse_m3.o
else
COBJS-$(CONFIG_EFUSE) += cmd_efuse.o
COBJS-$(CONFIG_EFUSE) += cmd_efuse_b09new.o
endif
COBJS-$(CONFIG_CMD_BL) += cmd_bl.o

# others
COBJS-$(CONFIG_DDR_SPD) += ddr_spd.o
COBJS-$(CONFIG_HWCONFIG) += hwconfig.o
COBJS-$(CONFIG_CONSOLE_MUX) += iomux.o
COBJS-y += flash.o
COBJS-$(CONFIG_CMD_KGDB) += kgdb.o kgdb_stubs.o
COBJS-$(CONFIG_KALLSYMS) += kallsyms.o
COBJS-$(CONFIG_LCD) += lcd.o
COBJS-$(CONFIG_LCD_AML) += lcd_aml.o
COBJS-$(CONFIG_LYNXKDI) += lynxkdi.o
COBJS-$(CONFIG_MODEM_SUPPORT) += modem.o
COBJS-$(CONFIG_UPDATE_TFTP) += update.o
COBJS-$(CONFIG_USB_KEYBOARD) += usb_kbd.o
ifndef CONFIG_AML_MESION_3
COBJS-$(CONFIG_EFUSE) += efuse_bch_8.o
endif
COBJS-$(CONFIG_AML_AUTOSCRIPT) += aml_autoscript.o
COBJS-$(CONFIG_CMD_RUNSCRIPT) += cmd_runscript.o

#osd
COBJS-$(CONFIG_CMD_TV) += cmd_tv.o
COBJS-$(CONFIG_CMD_OSD) += cmd_osd.o

#sys test
COBJS-$(CONFIG_HAVE_SYS_TEST) += sys_test.o

COBJS-y += ./rsa/addsub/fp_add.o \
./rsa/addsub/fp_add_d.o \
./rsa/addsub/fp_addmod.o \
./rsa/addsub/fp_cmp.o \
./rsa/addsub/fp_cmp_d.o \
./rsa/addsub/fp_cmp_mag.o \
./rsa/addsub/fp_sub.o \
./rsa/addsub/fp_sub_d.o \
./rsa/addsub/fp_submod.o \
./rsa/addsub/s_fp_add.o \
./rsa/addsub/s_fp_sub.o \
./rsa/bin/fp_radix_size.o \
./rsa/bin/fp_read_radix.o \
./rsa/bin/fp_read_signed_bin.o \
./rsa/bin/fp_read_unsigned_bin.o \
./rsa/bin/fp_reverse.o \
./rsa/bin/fp_s_rmap.o \
./rsa/bin/fp_signed_bin_size.o \
./rsa/bin/fp_to_signed_bin.o \
./rsa/bin/fp_to_unsigned_bin.o \
./rsa/bin/fp_toradix.o \
./rsa/bin/fp_unsigned_bin_size.o \
./rsa/bit/fp_cnt_lsb.o \
./rsa/bit/fp_count_bits.o \
./rsa/bit/fp_div_2.o \
./rsa/bit/fp_div_2d.o \
./rsa/bit/fp_lshd.o \
./rsa/bit/fp_mod_2d.o \
./rsa/bit/fp_rshd.o \
./rsa/divide/fp_div.o \
./rsa/divide/fp_div_d.o \
./rsa/divide/fp_mod.o \
./rsa/divide/fp_mod_d.o \
./rsa/exptmod/fp_2expt.o \
./rsa/exptmod/fp_exptmod.o \
./rsa/misc/fp_ident.o \
./rsa/misc/fp_set.o \
./rsa/mont/fp_montgomery_calc_normalization.o \
./rsa/mont/fp_montgomery_reduce.o \
./rsa/mont/fp_montgomery_setup.o \
./rsa/mul/fp_mul.o \
./rsa/mul/fp_mul_2.o \
./rsa/mul/fp_mul_2d.o \
./rsa/mul/fp_mul_comba.o \
./rsa/mul/fp_mul_comba_12.o \
./rsa/mul/fp_mul_comba_17.o \
./rsa/mul/fp_mul_comba_20.o \
./rsa/mul/fp_mul_comba_24.o \
./rsa/mul/fp_mul_comba_28.o \
./rsa/mul/fp_mul_comba_3.o \
./rsa/mul/fp_mul_comba_32.o \
./rsa/mul/fp_mul_comba_4.o \
./rsa/mul/fp_mul_comba_48.o \
./rsa/mul/fp_mul_comba_6.o \
./rsa/mul/fp_mul_comba_64.o \
./rsa/mul/fp_mul_comba_7.o \
./rsa/mul/fp_mul_comba_8.o \
./rsa/mul/fp_mul_comba_9.o \
./rsa/mul/fp_mul_comba_small_set.o \
./rsa/mul/fp_mul_d.o \
./rsa/mul/fp_mulmod.o \
./rsa/numtheory/fp_gcd.o \
./rsa/numtheory/fp_invmod.o \
./rsa/numtheory/fp_isprime.o \
./rsa/numtheory/fp_lcm.o \
./rsa/numtheory/fp_prime_miller_rabin.o \
./rsa/numtheory/fp_prime_random_ex.o \
./rsa/sqr/fp_sqr.o \
./rsa/sqr/fp_sqr_comba.o \
./rsa/sqr/fp_sqr_comba_12.o \
./rsa/sqr/fp_sqr_comba_17.o \
./rsa/sqr/fp_sqr_comba_20.o \
./rsa/sqr/fp_sqr_comba_24.o \
./rsa/sqr/fp_sqr_comba_28.o \
./rsa/sqr/fp_sqr_comba_3.o \
./rsa/sqr/fp_sqr_comba_32.o \
./rsa/sqr/fp_sqr_comba_4.o \
./rsa/sqr/fp_sqr_comba_48.o \
./rsa/sqr/fp_sqr_comba_6.o \
./rsa/sqr/fp_sqr_comba_64.o \
./rsa/sqr/fp_sqr_comba_7.o \
./rsa/sqr/fp_sqr_comba_8.o \
./rsa/sqr/fp_sqr_comba_9.o \
./rsa/sqr/fp_sqr_comba_generic.o \
./rsa/sqr/fp_sqr_comba_small_set.o \
./rsa/sqr/fp_sqrmod.o \
./rsa/rsa_api.o


COBJS	:= $(sort $(COBJS-y))
SRCS	:= $(AOBJS:.o=.S) $(COBJS:.o=.c)
OBJS	:= $(addprefix $(obj),$(AOBJS) $(COBJS))

CPPFLAGS += -I.. -I./rsa -I./rsa/include
CFLAGS += -I./rsa -I./rsa/include

all:	$(LIB) $(AOBJS)

$(LIB): $(obj).depend $(OBJS)
	$(AR) $(ARFLAGS) $@ $(OBJS)

$(obj)env_embedded.o: $(src)env_embedded.c $(obj)../tools/envcrc
	$(CC) $(AFLAGS) -Wa,--no-warn \
		-DENV_CRC=$(shell $(obj)../tools/envcrc) \
		-c -o $@ $(src)env_embedded.c

$(obj)../tools/envcrc:
	$(MAKE) -C ../tools

#########################################################################

# defines $(obj).depend target
include $(SRCTREE)/rules.mk

sinclude $(obj).depend

#########################################################################
