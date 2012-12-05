
#include "romfuns.h"
#define READ_SIZE       32*1024     // Size for data reading
#define CHECK_SIZE      8*1024      // Size for data checking

static unsigned short buf[READ_SIZE/sizeof(short)];
typedef unsigned long long u64;


unsigned enanble_rsa=0;
#define pow_debug(a...) 
//if(tag)printf(a)
#define swap(a,b)  {a=a^b;b=a^b;a=a^b;}
static u64 mulmod(u64 a,u64 b,u64 mod)
{
    u64 min_v=a%mod;
    u64 max_v=b%mod;
    
    u64 overflow;
    if(min_v%mod==0 || max_v%mod==0)
        return 0;
    
    if(min_v>max_v)
        swap(min_v,max_v);
    
    overflow=(0-1)/max_v;
    if(min_v>overflow)//overflow
    {
        return ((mod - max_v)*(mod - min_v))%mod;
    }
    return ((min_v)*(max_v))%mod;
}
static unsigned pow_mod(unsigned input_d,unsigned pow,  u64 mod)
{
    u64 temp;
    u64 input=input_d;
    temp=1;
    while(pow > 1){
        pow_debug("1.input=%llu,pow=%u,temp=%llu mod=%llu\n",input,pow,temp,mod);
        ///out = pow(input,pow)%mod = (pow( (input1=pow(input,2)%mod),pow/2)*(input*temp%mod))%mod
        //temp deal
        if(pow&1)
        {
            temp=mulmod(temp,input,mod);
            pow&=~1;
        }
        
        input=mulmod(input,input,mod);
        pow>>=1;
        
        pow_debug("2.input=%llu,pow=%u,temp=%llu \n",input,pow,temp);
  
   }
   return mulmod(temp,input,mod);
}
/*
p=109        q=601  x= 64800
e1=7        e2=55543 mod=1
n=65509
Public key(65509,7)
Private key(65509,55543)

*/
#if CONFIG_ENABLE_SECURITY
unsigned short rsa_decrypt(unsigned short c_short)
{
        unsigned c=c_short;
        c = pow_mod(c,7,65509);
        return (unsigned short)c;

}
#else
unsigned short rsa_encrypt(unsigned short c_short)
{
        unsigned c=c_short;
        c = pow_mod(c,55543,65509);
        return (unsigned short)c;

}
#endif
#if 0
unsigned short checksum(unsigned base,short sum)
{
    unsigned short * buf=(unsigned short * )base;
    int i;
    unsigned short tmp=0;
    
    for(i=0;i<0x1b0/2;i++)
    {
        tmp^=buf[i];
    }

    for(i=256;i<(CHECK_SIZE/2);i++)
    {
        tmp^=buf[i];
    }
    tmp^=buf[0x1b8/2];
    return tmp;

}
#endif

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
/*
void main()
{
int x ;
srand((unsigned)time(NULL));

for(x=0;x<=20;x++)
printf("%i\t",rand());

}
*/
static void m3_caculate(void)
{
	int i;
	unsigned short sum=0;
	unsigned short xor;
	unsigned * magic;
	//rsa encrypt
	if(enanble_rsa)
    {
//        srand((unsigned)time(NULL));
//        xor=rand()%65509;
        xor=0x43aa%65509;
        
        // Calculate sum
        for(i=0;i<0x1b0/2;i++)
    	{
    		buf[i]^=xor;
    	}
    	buf[0x1ba/2]=rsa_encrypt(xor);
    	printf("Enter Crypt %x %x\n",xor,buf[0x1ba/2]);
        
    }
	// Calculate sum
	for(i=0;i<0x1b0/2;i++)
	{
		sum^=buf[i];
	}

	for(i=256;i<CHECK_SIZE/2;i++)
	{
		sum^=buf[i];
	}
	buf[0x1b8/2]=sum;
}
int m3_write(FILE * fp_spl,FILE * fp_in ,FILE * fp_out)
{
    int count;
    memset(buf,0,sizeof(buf));
	fread(buf,sizeof(buf[0]),sizeof(buf)/sizeof(buf[0]),fp_spl);
	m3_caculate();
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
int m3_write_ex(FILE * fp_spl,FILE * fp_in ,FILE * fp_out,unsigned addr)
{
    int count;
    memset(buf,0,sizeof(buf));
	fread(buf,sizeof(buf[0]),sizeof(buf)/sizeof(buf[0]),fp_spl);
	m3_caculate();
	fwrite(buf,sizeof(buf[0]),sizeof(buf)/sizeof(buf[0]),fp_out);
	while(!feof(fp_spl))
	{
		count=fread(buf,sizeof(char),sizeof(buf),fp_spl);
		fwrite(buf,sizeof(char),(count+3)&(~3),fp_out);
	}
	while(!feof(fp_in))
	{
		count=fread(buf,sizeof(char),sizeof(buf),fp_in);
       
        fwrite(buf,sizeof(char),count,fp_out);
	}
	return 0;
    
}
