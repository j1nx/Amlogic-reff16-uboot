#include <common.h>
#include <command.h>
#include <image.h>
#include <u-boot/zlib.h>
#include <asm/byteorder.h>
#include <asm/setup.h>
#include <version.h>
#include <u-boot/minilzo.h>
#include <malloc.h>

#define SLOT0MAGIC (0x0)
#define SLOT1MAGIC (0x1)
#define SLOT2MAGIC (0x2)
static char version_info[] =
	U_BOOT_VERSION"(" __DATE__ ")";

typedef enum{
	BOOT_NORMAL,
	SD_SLOT_1,
	SD_SLOT_2,
	USB_HOST_SLOT,
	NAND_SLOT_1,
	NAND_SLOT_2,
	NOR_SLOT_1,
	NOR_SLOT_2,
}AMEBootDevice;

typedef struct out_ident{
	unsigned slot;
	unsigned platform;
	unsigned customer_id;
	unsigned software_ver;
	unsigned create_date;
	unsigned char info[44];
}OutIdent;

typedef struct _ame_boot_parameter{
	AMEBootDevice boot_device;
	unsigned char boot_path[124];
	unsigned pad[31];
	OutIdent * boot_loader;
	unsigned res_offset;
}AMEBootPara;

typedef struct {
	unsigned magic;
	unsigned short head_size;
	unsigned short head_crc;
}AMHeaderMagic;

typedef struct _ame_head{
	AMHeaderMagic magic;
	unsigned head_ver;
	unsigned text_offset;
	unsigned text_head_size;
	unsigned text_entry;
	unsigned res_offset;
	unsigned res_head_count;
	unsigned boot_parameter;
	OutIdent ident;
}AMEHead;

typedef struct _ame_text{
	unsigned in_offset;
	unsigned in_len;
	unsigned out_addr;
	unsigned out_len;
	unsigned check_sum;
}AMEText;

typedef struct _ame_res_item{
	unsigned offset;
	unsigned res_size;
	unsigned check_sum;
}AMEResource;

#define EXE_LEN_MASK (0xfffffff)
#define BSSTYPE_SEGMENT (0x10000000)
#define DIRECT_COPY_SEGMENT (0x20000000)
#define DECOMPRESS_SEGMENT  (0x0)
#define HEAD_MAGIC 0x414d4c20

OutIdent software_ident_struct={
		0, 	// slot ident
		0,	//platform_id
		0,	//customer_id
		0,	//software_id
		0,	//create date
		""	// info
	};


DECLARE_GLOBAL_DATA_PTR;

void do_bootm_avos(int flag, int argc, char *argv[],
		     bootm_headers_t *images)
{
	ulong	ep = 0;
	void	(*theKernel)();
	ulong image_start, image_len;
	AMEHead * head;
	ulong res_offset;
	unsigned char* src = NULL;
	unsigned char *dest = NULL;
	/* find kernel entry point */
	if (images->legacy_hdr_valid) {
		ep = image_get_ep (&images->legacy_hdr_os_copy);
	} else {
		puts ("Could not find kernel entry point!\n");
		goto error;
	}
	image_start = images->os.image_start;
	image_len = images->os.image_len;

	head=(AMEHead*)image_start;

	if(head->magic.magic!=HEAD_MAGIC)
	{
		res_offset=0;
		printf("Wrong head magic...!\n");
		return;
	}

	AMEText * text=(AMEText*)&head[1];
	AMEResource * resource=(AMEResource*)&text[head->text_head_size];
	if (lzo_init() != LZO_E_OK)
	{
		printf("LZO init fail...!\n");
		return;
	}
	int i,j;
	lzo_uint out_len ;
	ulong * temp_seq=(unsigned *)malloc(head->text_head_size*sizeof(unsigned*));
	if(temp_seq == NULL){
		printf("Malloc fail...!\n");
		return;
	}
	ulong count=0;
	ulong temp_int;
	for(i=0;i<head->text_head_size;i++)
	{
		if((text[i].in_len&(~EXE_LEN_MASK))==DECOMPRESS_SEGMENT)
			temp_seq[count++]=i;
	}
	for(i=count-1;i>0;i--)
	{
		for(j=0;j<i;j++)
		{
			if(text[temp_seq[j]].out_len<text[temp_seq[j+1]].out_len)
			{
				temp_int=temp_seq[j];
				temp_seq[j]=temp_seq[j+1];
				temp_seq[j+1]=temp_int;
			}
		}
	}

	for(i=0;i<count;i++){
		
		src = (unsigned char*)(image_start + text[temp_seq[i]].in_offset);
		int r=lzo1x_decompress((lzo_byte *)src, text[temp_seq[i]].in_len&EXE_LEN_MASK,
				(lzo_byte *)text[temp_seq[i]].out_addr, &out_len,
				(lzo_byte *)0);

		if (r != LZO_E_OK || text[temp_seq[i]].out_len!=out_len) {
			printf("LZO uncompress fail...!\n");
			free(temp_seq);
			return;
		}
	}
	free(temp_seq);

	for(i=0;i<head->text_head_size;i++)	{
		
		switch(text[i].in_len&(~EXE_LEN_MASK)) {
			case BSSTYPE_SEGMENT:
				memset((void*)text[i].out_addr,0,text[i].out_len);
				break;
			case DIRECT_COPY_SEGMENT:
				src = (unsigned char*)(image_start + text[i].in_offset);
				dest = (unsigned char*)text[i].out_addr;
				memcpy(dest, src, text[i].in_len&EXE_LEN_MASK);
				break;
			case DECOMPRESS_SEGMENT:				
				break;
		}
	}

	theKernel = (void (*)())head->text_entry;
	debug ("## Transferring control to AVOS (at address %08lx) ...\n",
	       (ulong) theKernel);
	/* parameter passing */
	AMEBootPara * boot=(AMEBootPara *)head->boot_parameter;
	boot->res_offset=resource->offset;
	memcpy(software_ident_struct.info,version_info,sizeof(version_info));
	boot->boot_loader = &software_ident_struct;
	/* we assume that the kernel is in place */
	printf ("\nStarting kernel ...\n\n");
	dcache_flush();
	icache_invalid();
	__asm__ __volatile (" nop");
    __asm__ __volatile (" nop");
    __asm__ __volatile (" nop");
    __asm__ __volatile (" nop");
	theKernel ();
	__asm__ __volatile (" nop");
    __asm__ __volatile (" nop");
    __asm__ __volatile (" nop");
    __asm__ __volatile (" nop");
	/* does not return */
	return;

error:
//	if (images->autostart)
//		do_reset (cmdtp, flag, argc, argv);
	return;
}
