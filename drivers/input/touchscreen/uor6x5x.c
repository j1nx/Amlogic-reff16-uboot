#include <asm/arch/i2c.h>
#include <aml_i2c.h>
#include <ts.h>
#include <uor6x5x.h>

#define msleep(a) udelay(a * 1000)
#define abs(x) ({				\
		long __x = (x);			\
		(__x < 0) ? -__x : __x;		\
	})

#define TRUE	1
#define FALSE 	0

#define InitX		0x00
#define InitY		0x10
#define MSRX_2T         0x40
#define MSRY_2T         0x50
#define MSRX_1T         0xC0
#define MSRY_1T         0xD0
#define MSRZ1_1T        0xE0
#define MSRZ2_1T        0xF0

#define R_Threshold 	       7000       //ting
#define R_Threshold2 	1200	//ting //	600

#define DX_T			64	//	72
#define DY_T			64	//	72

#define NumberFilter		6
#define NumberDrop			4	//This value must not bigger than (NumberFilter-1)

#define FIRSTTOUCHCOUNT		1    //ting
#define ONETOUCHCountAfter2 	20
#define JITTER_THRESHOLD   	       800 //ting
#define MAX_READ_PERIOD		6  //ting
#define FIRST_TWO_TOUCH_FILTER	1
#define JITTER_THRESHOLD_DXDY	32
#define PERIOD_PER_FILTER	1

#define FILTER_FUNC
#define NFilt NumberFilter
#define NDrop NumberDrop

struct uor6x5x_event {
	long xp;
	long yp;
	long pX;
	long pY;
	int count;
};

int UOR_IICRead(u32 addr, u8 cmd, u8 *buff, u32 count)
{
    struct i2c_msg msgs1[] = {
    	{
    	    .addr	 	= addr,
    	    .flags  	        = 0,
    	    .len		= 1,
    	    .buf		= &cmd,
    	},
    	{
    	    .addr	 	= addr,
    	    .flags  	        = I2C_M_RD,
    	    .len		= count,
    	    .buf		= buff,
    	}
    };
    
    if (aml_i2c_xfer(msgs1, 2) != 2)//tranfer two messages
    {
    	printf("[IIC_Read]: i2c_transfer() return error !!!\n");
    	return -1;
    }
    return 0; 
}

int xFilter[NFilt], yFilter[NFilt],DxFilter[NFilt], DyFilter[NFilt];
unsigned int XYIndex = 0;

static int XYFilter(int *xFilter, int *yFilter, int Num,int Drop){
    unsigned int i,SumTempX=0,SumTempY=0;
    int Dp,checkSmx,checkSmy,checkBgx,checkBgy;
    int SmX =0, SmY = 0;
    int LaX = 0, LaY = 0;
    int SmInX = 0, SmInY = 0;
    int LaInX = 0, LaInY =0;
    
    if( (Num <=2) && (Drop > (Num-1)) )
        return FALSE; // not enough to sample
    
    for(i=0;i<Num;i++){
    	SumTempX += xFilter[i];
    	SumTempY += yFilter[i];
    }
    Dp = Drop;
    
    checkSmx = 0;
    checkSmy = 0;
    checkBgx = 0;
    checkBgy = 0;
    while(Dp>0){
    	SmX = 0x0FFF;SmY = 0x0FFF;
    	LaX = 0x0;LaY = 0x0;
    	SmInX = 0;SmInY = 0;
    	LaInX = 0;LaInY =0;
    	for(i =  0; i < Num; i++){
    	    if(checkSmx&(1<<i)){
    	    }
    	    else if(SmX > xFilter[i]){
    	    	SmX = xFilter[i];
    	    	SmInX= i;
    	    }
    	    
    	    if(checkSmy&(1<<i)){
    	    }
    	    else if(SmY > yFilter[i]){
    	    	SmY = yFilter[i];
    	    	SmInY = i;
    	    }
    	    
    	    if(checkBgx&(1<<i)){
    	    }
    	    else if(LaX < xFilter[i]){
    	    	LaX = xFilter[i];
    	    	LaInX = i;
    	    }
    	    
    	    if(checkBgy&(1<<i)){
    	    }
    	    else if(LaY < yFilter[i]){
    	    	LaY = yFilter[i];
    	    	LaInY = i;
    	    }
    	}
    	if(Dp){
    	    SumTempX-= xFilter[SmInX];
    	    SumTempX-= xFilter[LaInX];
    	    SumTempY-= yFilter[SmInY];
    	    SumTempY-= yFilter[LaInY];
    	    Dp -= 2;
    	    //printf("in filter :SmInX %d,LaInX %d, SmInY %d , LaInY %d!!!\n", SmInX,LaInX, SmInY, LaInY);
    	}
    	checkSmx |= 1<<SmInX;
    	checkSmy |= 1<<SmInY;
    	checkBgx |= 1<<LaInX;
    	checkBgy |= 1<<LaInY;
    }
    xFilter[0] = SumTempX/(Num-Drop);
    yFilter[0] = SumTempY/(Num-Drop);
    return TRUE;
}


static int Init_UOR_HW(struct uor6x5x *ts)
{
    u8 i,icdata[2];
    u32 Dx_REF,Dy_REF,Dx_Check,Dy_Check;
    int TempDx[NumberFilter],TempDy[NumberFilter];
    
    for(i=0;i<NumberFilter;i++){
    	UOR_IICRead(ts->addr, InitX,icdata,2);
    	TempDx[i] = (icdata[0]<<4 | icdata[1]>>4);
        UOR_IICRead(ts->addr, InitY,icdata,2);
        TempDy[i] = (icdata[0]<<4 | icdata[1]>>4);
        //printf("filter test:#%d (x,y)=(%d,%d) !!!\n", i,TempDx[i], TempDy[i]);
    }
    XYFilter(TempDx,TempDy,NumberFilter,2);
    Dx_REF = TempDx[0];
    Dy_REF = TempDy[0];
    //printf("filter result:(x,y)=(%d,%d) !!!\n", Dx_REF, Dy_REF);
    
    i = 0;
    do{
        UOR_IICRead(ts->addr, InitX,icdata,2);
        Dx_Check = abs((icdata[0]<<4 | icdata[1]>>4) - Dx_REF);
        UOR_IICRead(ts->addr, InitY,icdata,2);
        Dy_Check = abs((icdata[0]<<4 | icdata[1]>>4) - Dy_REF);
        i++;
        if(i>NumberFilter)
            return -1;
    }while(Dx_Check > 4 || Dy_Check > 4);
    return 0;
}

static int FirstTC = 0,OneTCountAfter2 = 0,TWOTouchFlag = 0;
static int two_touch_count = 0, pre_dx = 0, pre_dy = 0;

static void uor_read_data(u32 addr, unsigned short *X, unsigned short *Y, unsigned short *DX, unsigned short *DY)
{
    u8 EpBuf[16];
    unsigned short	x, y;
    unsigned short	Dx, Dy;
    
    memset(EpBuf, 0, sizeof(EpBuf));
    UOR_IICRead(addr, MSRX_1T, EpBuf, 2);
    x= EpBuf[0]; 
    x <<=4;
    x |= (EpBuf[1]>>4);
    
    UOR_IICRead(addr, MSRY_1T,  EpBuf, 2);
    y = EpBuf[0]; 
    y <<=4;
    y |= (EpBuf[1]>>4);
    
    UOR_IICRead(addr, MSRX_2T,  (EpBuf), 3);
    Dx = EpBuf[2];
    
    UOR_IICRead(addr, MSRY_2T,  (EpBuf), 3);
    Dy = EpBuf[2];	
    
    *X = x;
    *Y = y;
    *DX = Dx;
    *DY = Dy;
}

int uor6x5x_work(struct uor6x5x *ts, struct ts_event *tc)
{
    u8 EpBuf[16];
    unsigned short x, y;
    unsigned short out_x, out_y;
    unsigned short Dx, Dy, z1, z2;
    unsigned int R_touch;
    unsigned int Rt;
    unsigned int nTouch = 0;
    int xy = 0;
    struct uor6x5x_event tsc;
    //printf("uor.c: uor_read_loop() !\n");
    
    if((ts == NULL) || (tc == NULL)) {
    	//printf("uor6x5x_work: ts is null or tc is null\n");
    	return -1;
    }
    memset(tc, 0, sizeof(struct ts_event));
    if (ts->get_pendown_state == NULL) {
    	//printf("uor6x5x_work: ts get_pendown_state is null\n");
    	return -1;
    }
	if(ts->get_pendown_state()) {
		return 0;
	}

    while(1){
    	if(!ts->get_pendown_state()) {
    	    uor_read_data(ts->addr, &x, &y, &Dx, &Dy);
    	    #ifdef FILTER_FUNC
    	    //first point
    	    xFilter[tsc.count] = x;
    	    yFilter[tsc.count] = y;	
    	    DxFilter[tsc.count] = Dx;
    	    DyFilter[tsc.count] = Dy;
    	    //printf("Data before filter:#%d (x,y)=(%d,%d) (dx,dy)=(%d,%d) !!!\n",ts.count , x, y, Dx, Dy);
    	    tsc.count ++;
    	    //udelay(PERIOD_PER_FILTER);  //ting    //Per Read Point Delay
    	    
    	    while(tsc.count < NFilt)//collect other point 
    	    {
    	    	uor_read_data(ts->addr, &x, &y, &Dx, &Dy);
    	    	xFilter[tsc.count] = x;
    	    	yFilter[tsc.count] = y;	
    	    	DxFilter[tsc.count] = Dx;
    	    	DyFilter[tsc.count] = Dy;
    	    	//printf("Data before filter:#%d (x,y)=(%d,%d) (dx,dy)=(%d,%d) !!!\n",ts.count , x, y, Dx, Dy);
    	    	tsc.count ++;
    	    	udelay(PERIOD_PER_FILTER);//Per Read Point Delay
            }
            
            if(!XYFilter(xFilter, yFilter, NFilt,NDrop)){ // no correct point	
            	printf("%s: X Y filter error !!!\n",__FUNCTION__);
            }
            
            tsc.xp =xFilter[0];
            tsc.yp =yFilter[0];					
            
            if(!XYFilter(DxFilter, DyFilter, NFilt,NDrop)){ // no correct point
            	printf("%s: DX DY filter error !!!\n",__FUNCTION__);
            }
            Dx = DxFilter[0];
            Dy = DyFilter[0];
            tsc.count = 0;
            #else // no filter
            tsc.xp = x;
            tsc.yp = y;	
            #endif
            
            memset(EpBuf, 0, sizeof(EpBuf));
            UOR_IICRead(ts->addr, MSRZ1_1T,  EpBuf, 2);
            z1 = EpBuf[0]; 
            z1 <<=4;
            z1 |= (EpBuf[1]>>4);
            
            UOR_IICRead(ts->addr, MSRZ2_1T, EpBuf, 2);
            z2 = EpBuf[0]; 
            z2 <<=4;
            z2 |= (EpBuf[1]>>4);
            
            if(z1 ==0) {
            	z1 =1;//avoid divde by zero
            }
            
            R_touch =(abs(((z2*x)/z1-x)))/4; //(float)((((float) z2)/((float) z1) -1)*(float)x)/4096;
            Rt =R_touch;
            
            if( ((Dx > DX_T) || (Dy > DY_T)) && (Rt < R_Threshold2) ) {
            	nTouch =  TWO_TOUCH;
            }
            else {
            	nTouch = ONE_TOUCH;
            }
            //printf("%s:after filter (x,y)=(%d,%d) (dx,dy)=(%d,%d) n_touch %d, R_touch %d, (z1,z2)=(%d,%d) !!!\n",__FUNCTION__, x, y, Dx, Dy, nTouch, R_touch, z1, z2);
        }
        else {//ting 
            nTouch =  ZERO_TOUCH;
            //printf("there is no touch!\n");
        }
        
        if(nTouch == ONE_TOUCH || nTouch == TWO_TOUCH){	// pen down
            if(nTouch == TWO_TOUCH){
            	if(two_touch_count < FIRST_TWO_TOUCH_FILTER){
            	    //printf("%s:filter for first two touch -(x,y)=(%d,%d) (dx,dy)=(%d,%d),count = %d, FIRST_TWO_TOUCH_FILTER = %d  !!!\n",__FUNCTION__, x, y, Dx, Dy,two_touch_count, FIRST_TWO_TOUCH_FILTER);
            	    two_touch_count++;
            	    msleep(2);    //ting
            	    continue;//re-start the loop
            	}
            	else if( (pre_dx!=0) && (pre_dy!=0) && (Dx - pre_dx > JITTER_THRESHOLD_DXDY || pre_dx - Dx > JITTER_THRESHOLD_DXDY || pre_dy - Dy > JITTER_THRESHOLD_DXDY || Dy - pre_dy > JITTER_THRESHOLD_DXDY)){
            	    //printf("%s:filter for jitter(dual) --(pre_dx,pre_dy)=(%d,%d) ,(dx,dy)=(%d,%d) , JITTER_THRESHOLD_DXDY = %d !!!\n",__FUNCTION__, pre_dx, pre_dy , Dx, Dy, JITTER_THRESHOLD_DXDY);
            	    msleep(MAX_READ_PERIOD);
            	    continue;//re-start the loop
                }
                else{
                    //printf("%s:report dual touch-- (x,y)=(%d,%d) (dx,dy)=(%d,%d)  !!!\n",__FUNCTION__, x, y, Dx, Dy);
                    //report x,y,pressure,dx,dy to Linux/Android
                    if((pre_dx!=0) && (pre_dy!=0) && ((Dx - pre_dx) <8 && (pre_dx-Dx) <8 && (Dy - pre_dy )<8&& (pre_dy-Dy )<8)){
                    	Dx = pre_dx;
                    	Dy = pre_dy;
                    }
                    
                    if ( (tsc.pX!=0) && (tsc.pY!=0) && ((tsc.xp - tsc.pX) <80 && (tsc.pX-tsc.xp) <80 && (tsc.yp - tsc.pY )<80 && (tsc.pY-tsc.yp )<80)){
                    	tsc.xp = tsc.pX;
                    	tsc.yp = tsc.pY;
                    }
                    
                    int dx_coord =(Dx - DX_T< 0) ? 0 :  ((Dx - 40) & 0x00fc) * 4;
                    int dy_coord=(Dy - DY_T< 0) ? 0 :  ((Dy - 40) & 0x00fc) * 4;

                    xy = 0;
                    out_x = tsc.xp - dx_coord;
                    out_y = tsc.yp - dy_coord;
                    if(ts->convert){
                    	xy = ts->convert(out_x, out_y);
                    	out_x = xy >> 16;
                    	out_y = xy & 0xffff;
                    	tc->x = out_x;
                    	tc->y = out_y;
                    	tc->touch = TWO_TOUCH;
                    }
                    //printf("%s:TWO_TOUCH (x1,y1)=(%d,%d)\n",__FUNCTION__, out_x, out_y);
                    
                    xy = 0;
                    out_x = tsc.xp + dx_coord;
                    out_y = tsc.yp + dy_coord;
                    if(ts->convert){
                    	xy = ts->convert(out_x, out_y);
                    	out_x = xy >> 16;
                    	out_y = xy & 0xffff;
                    	tc->x1 = out_x;
                    	tc->y1 = out_y;
                    	tc->touch = TWO_TOUCH;
                    }
                    //printf("%s:TWO_TOUCH (x2,y2)=(%d,%d)\n",__FUNCTION__, out_x, out_y);
                    
                    TWOTouchFlag = 1;
                    OneTCountAfter2 = 0;
                    pre_dx = Dx;
                    pre_dy = Dy;
                    return 1;
                }
            }
            else if(nTouch == ONE_TOUCH){
                if((TWOTouchFlag == 1) && (OneTCountAfter2 < ONETOUCHCountAfter2)){
                    //printf("%s:filter after two touch -- (x,y)=(%d,%d) ,OneTCountAfter2 = %d, ONETOUCHCountAfter2 = %d !!!\n",__FUNCTION__, x, y, OneTCountAfter2, ONETOUCHCountAfter2);
                    OneTCountAfter2++;
                    msleep(MAX_READ_PERIOD);
                    continue;//re-start the loop
                }		
                else if(((TWOTouchFlag == 0) && (FirstTC < FIRSTTOUCHCOUNT)) || (Rt > R_Threshold)){  //ting
                    //printf("%s:filter before single touch -- (x,y)=(%d,%d) ,FirstTC = %d, FIRSTTOUCHCOUNT = %d !!!\n",__FUNCTION__, x, y, FirstTC, FIRSTTOUCHCOUNT);
                    FirstTC++;
                    msleep(2);
                    continue;//re-start the loop
                }
                else if( (tsc.pX!=0) && (tsc.pY!=0) && (tsc.xp - tsc.pX > JITTER_THRESHOLD || tsc.pX - tsc.xp > JITTER_THRESHOLD || tsc.pY - tsc.yp > JITTER_THRESHOLD || tsc.yp - tsc.pY > JITTER_THRESHOLD)){
                    //printf("%s:filter for jitter -- (px,py)=(%d,%d) ,(x,y)=(%d,%d) , JITTER_THRESHOLD = %d !!!\n",__FUNCTION__, tsc.pX, tsc.pY ,x, y, JITTER_THRESHOLD);
                    msleep(MAX_READ_PERIOD);
                    continue;//re-start the loop
                }
                else{
                    //printf("%s: (Pen down)report single touch-- (x,y)=(%d,%d) !!!\n",__FUNCTION__, x, y);
                    //report x,y,pressure,size to Linux/Android
                    if((tsc.pX!=0) && (tsc.pY!=0) && ((tsc.xp - tsc.pX) <80 && (tsc.pX-tsc.xp) <80 && (tsc.yp - tsc.pY )<80 && (tsc.pY-tsc.yp )<80)){
                    	tsc.xp = tsc.pX;
                    	tsc.yp = tsc.pY;
                    }

                    xy = 0;
                    out_x = tsc.xp;
                    out_y = tsc.yp;
                    if(ts->convert){
                    	xy = ts->convert(out_x, out_y);
                    	out_x = xy >> 16;
                    	out_y = xy & 0xffff;
                    	tc->x = out_x;
                    	tc->y = out_y;
                    	tc->touch = ONE_TOUCH;
                    }
                    //printf("%s:ONE_TOUCH (x1,y1)=(%d,%d)\n",__FUNCTION__, out_x, out_y);
                    
                    //save previous single touch point
                    tsc.pX = tsc.xp; 
                    tsc.pY = tsc.yp;
					return 1;                    
                }
            }
	}
	else if(nTouch == ZERO_TOUCH){	// pen release
	   if(!ts->get_pendown_state()){
	   	msleep(MAX_READ_PERIOD);
	   	continue;
	   }
	   //printf("%s: (Pen release)!!!\n",__FUNCTION__);
	   
	   //reset filter parameters
	   FirstTC = 0;
	   OneTCountAfter2 = 0;
	   TWOTouchFlag = 0;
	   two_touch_count = 0;
	   tsc.xp= 0;
	   tsc.yp = 0;
	   tsc.pX = 0;
	   tsc.pY = 0;
	   pre_dx = 0;
	   pre_dy = 0;			
	   Init_UOR_HW(ts);
	   break;
       }
       else{
           printf("uor_read_loop(): n_touch state error !!!\n");
       }
   }
	return 0;
}

int uor6x5x_init(struct uor6x5x *ts)
{
   int ret = 0;	
   
   if(ts == NULL) {
       printf("tsc2007_init: ts is null\n");
       return -1;
   }
   
   ret = Init_UOR_HW(ts);
   if(ret < 0) {
       printf("uor.c: Init_UOR_HW() fail in uor6x5x_init()!\n");
       return ret;
   }
   return ret;
}
