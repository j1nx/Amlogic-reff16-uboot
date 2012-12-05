#ifndef __BL_AML_H__
#define __BL_AML_H__

typedef struct blinfo {
	int	  max_bl_level;
	void  (*bl_on)(void);
	void  (*bl_off)(void);
    void  (*panel_on)(void);
    void  (*panel_off)(void);
	void  (*set_bl_level)(unsigned level);
} blinfo_t;

#endif //__BL_AML_H__

