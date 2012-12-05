
#include "romfuns.h"


static unsigned short buf[3*1024];
#define MAGIC_WORD1         0x4f53454d  /* "" */
#define MAGIC_WORD2         0x3130304e  /* "" */
static void m1_caculate(void)
{
	int i;
	unsigned short sum=0;
	unsigned * magic;
	// Calculate sum
	for(i=0;i<0x1b0/2;i++)
	{
		sum^=buf[i];
	}

	for(i=256;i<sizeof(buf)/2;i++)
	{
		sum^=buf[i];
	}
	buf[0x1b8/2]=sum;
	magic=(unsigned *)&buf[0x1b0/2];
	magic[0]=MAGIC_WORD1;
	magic[1]=MAGIC_WORD2;
}
int m1_write(FILE * fp_spl,FILE * fp_in ,FILE * fp_out)
{
    int count;
    memset(buf,0,sizeof(buf));
	fread(buf,sizeof(buf[0]),sizeof(buf)/sizeof(buf[0]),fp_spl);
	m1_caculate();
	fwrite(buf,sizeof(buf[0]),sizeof(buf)/sizeof(buf[0]),fp_out);
	while(!feof(fp_spl))
	{
		count=fread(buf,sizeof(buf[0]),sizeof(buf)/sizeof(buf[0]),fp_spl);
		fwrite(buf,sizeof(buf[0]),count,fp_out);
	}
	while(!feof(fp_in))
	{
		count=fread(buf,sizeof(char),sizeof(buf),fp_in);
        
        fwrite(buf,sizeof(char),count,fp_out);
	}
	return 0;
}