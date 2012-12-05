#ifndef __ROMFUNS_H_
#include<stdio.h>
struct romboot_s {
    const char * cpu;
    int (* write)(FILE * spl,FILE * in ,FILE * out);
    int (* write_ex)(FILE * spl,FILE * in ,FILE * out,unsigned addr);
};
/* tools/m1_romboot.c */
int m1_write(FILE *spl, FILE *in, FILE *out);
int a3_write(FILE *spl, FILE *in, FILE *out);
int m3_write(FILE *spl, FILE *in, FILE *out);
int m3_write_ex(FILE *spl, FILE *in, FILE *out,unsigned addr);
#endif
