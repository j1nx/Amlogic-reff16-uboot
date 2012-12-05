aml_tv_m2_cst01 socket 厚板：

V59C1G02168QBP25A ddr 运行在150M,其它都不改变

uboot:版本是1857

修改单个文件版本如下：
1 aml_tv_m2_cst01.h 
  位置：/include/configs/amlogic/
  版本号：2207
2 board.c 
  位置：arch/arm/lib/
  版本号：2205
3 timing.c
  位置：/board/amlogic/aml_tv/m2_cst01/firmware/
  版本号：2209
4 ddr.c
  位置：/board/amlogic/aml_tv_m2_cst01/firmware/
  版本号：2208
5 ddr_types.h
  位置：include/
  版本号：2206





