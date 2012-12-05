#ifndef _SYS_TEST_H_
#define _SYS_TEST_H_

typedef struct titeminfo {
	int open_close;
	int test_type;
	int pos;
	unsigned int title_width;
	unsigned int item_nums;
	const char *title;
	void *item_data;
	char* (*return_tint)(int item);
	int (*do_action)(int item, unsigned int number, char **buf);
} titeminfo_t;

#define MAX_TEST_ITEMS_NUM    20
#define MAX_TEST_RESULT_NUM   20
#define MAX_TEST_FAIL_NUM     30
#define MAX_TEST_RESULT_CHARS 128

#define DISPLAY_WHITE_COLOR   0xffffff
#define DISPLAY_BLACK_COLOR   0x0
#define DISPLAY_BLUE_COLOR    0x80ff80
#define DISPLAY_RED_COLOR    0xfa0c12

#define TEST_START            0
#define TEST_SUCESS           1
#define TEST_FAIL             2

extern int systest_free(char **buf);
extern int systest_malloc(char **buf);

extern void main_test(void);
#endif
