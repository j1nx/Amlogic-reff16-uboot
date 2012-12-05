cp ./board/amlogic/m3_mid_reff11/firmware/i2c.c ./arch/arm/cpu/aml_meson/m3/firmware/arc_power/
cp ./board/amlogic/m3_mid_reff11/firmware/test.c ./arch/arm/cpu/aml_meson/m3/firmware/arc_power/
export LD_LIB_PATH=/opt/arc/MetaWare/arc/lib/a6/le
make -f AutoMake -j 32