/*******************************************************************
 *
 *  Copyright C 2006 by Amlogic, Inc. All Rights Reserved.
 *
 *  Description:  soft_i2c.c
 *
 *  Author: Amlogic Software
 *  Created: 11/1/2007 
 *
 *******************************************************************/

#include <aml_a3_hard_i2c.h>

int max_rw_byte = 8 ;

hard_i2c_para_t hard_i2c_para = {
	0x2c,   //unsigned char       device_id;
	0xff,   //unsigned char       id_mask;
	0,      //unsigned char       i2c_flag    
	0x2000,    //unsigned            max_addr;
	100,  //10,   //unsigned            delay;   
	100          //unsigned            requency

    };	
    

extern int get_clk81(void);

static void hard_i2c_delay(unsigned int delay_count)
{
        if(delay_count)                 
        while(delay_count--);
}


/*return 0, sucessful;
  return -1, error, 
  */
static int hard_i2c_detect_ack(void)    //This read only bit is set if the I2C device generates a NACK during writing
{
	return( ((READ_PERI_REG(PREG_I2C_MS_CTRL)) & 0x8) >> 3 );
}

static void hard_i2c_stop(void)
{
	int count_ = 1000; 

  WRITE_PERI_REG(PREG_I2C_MS_TOKEN_LIST_REG0, ((I2C_END << 28)   | 
                                               (I2C_END << 24)   | 
                                               (I2C_END << 20)   |
                                               (I2C_END << 16)   |
                                               (I2C_END << 12)   |
                                               (I2C_END << 8)    |
                                               (I2C_END << 4)    |
                                               (I2C_STOP << 0)         ));       
        // Toggle start low then high 
  	WRITE_PERI_REG(PREG_I2C_MS_CTRL, ((READ_PERI_REG(PREG_I2C_MS_CTRL)) & 0xfffffffe ));         
   WRITE_PERI_REG(PREG_I2C_MS_CTRL, ((READ_PERI_REG(PREG_I2C_MS_CTRL)) | 0x1 )); 	

   while( ((READ_PERI_REG(PREG_I2C_MS_CTRL)) & (1 << I2C_M_STATUS)) && (count_ -- ));  // Wait for the transfer to complete  


}



int hard_i2c_read_0(unsigned char *rw_buff, unsigned int rw_bytes, unsigned int rw_addr)
{
    int tmp_count2, count1 , temp_data0 = 0;
    unsigned char err; 
    
    if (rw_addr > hard_i2c_para.max_addr) 
    	{
        return -1;
      } 

           
    WRITE_PERI_REG(PREG_I2C_MS_SL_ADDR, hard_i2c_para.device_id);	
    WRITE_PERI_REG(PREG_I2C_MS_TOKEN_LIST_REG0, 0);
    WRITE_PERI_REG(PREG_I2C_MS_TOKEN_LIST_REG1, 0);	                      

   if(rw_addr !=0xff)
   	{ 
      //send start
      WRITE_PERI_REG(PREG_I2C_MS_TOKEN_LIST_REG0, ((I2C_END  << 28)              | 
                                                 (I2C_END  << 24)              |   // don't stop the bus, leave it in a read state
                                                 (I2C_END << 20)   |
                                                 (I2C_END << 16)             |      // send Start Repeat for read
                                                 (I2C_END  << 12)             | 
                                                 (I2C_END << 8)               |
                                                 (I2C_END << 4)   |
                                                 (I2C_START << 0)    ));

        // Toggle start "low then high"
     WRITE_PERI_REG(PREG_I2C_MS_CTRL, ((READ_PERI_REG(PREG_I2C_MS_CTRL)) & 0xfffffffe )); 	        
     WRITE_PERI_REG(PREG_I2C_MS_CTRL, ((READ_PERI_REG(PREG_I2C_MS_CTRL)) | 0x1 )); 

     hard_i2c_delay(hard_i2c_para.delay << 6);    /*there is enough delay in order to make hardware I2C to work normal*/
     count1 = 1000; 
     while( ((READ_PERI_REG(PREG_I2C_MS_CTRL)) & (1 << I2C_M_STATUS)) && (count1 -- ));  // Wait for the transfer to complete  

      //send the device id(write)
      WRITE_PERI_REG(PREG_I2C_MS_TOKEN_LIST_REG0, ((I2C_END  << 28)              | 
                                                 (I2C_END  << 24)              |   // don't stop the bus, leave it in a read state
                                                 (I2C_END << 20)   |
                                                 (I2C_END << 16)             |      // send Start Repeat for read
                                                 (I2C_END  << 12)             | 
                                                 (I2C_END << 8)               |
                                                 (I2C_END << 4)   |
                                                 (I2C_SLAVE_ADDR_WRITE << 0)    ));

        // Toggle start "low then high"
     WRITE_PERI_REG(PREG_I2C_MS_CTRL, ((READ_PERI_REG(PREG_I2C_MS_CTRL)) & 0xfffffffe )); 	        
     WRITE_PERI_REG(PREG_I2C_MS_CTRL, ((READ_PERI_REG(PREG_I2C_MS_CTRL)) | 0x1 )); 

     hard_i2c_delay(hard_i2c_para.delay << 4);    /*there is enough delay in order to make hardware I2C to work normal*/
     count1 = 1000; 
     while( ((READ_PERI_REG(PREG_I2C_MS_CTRL)) & (1 << I2C_M_STATUS)) && (count1 -- ));  // Wait for the transfer to complete  
 
  
     if( hard_i2c_detect_ack())    //error 
    	 {
    	  	hard_i2c_stop();
    	    return -1;
    	 }  
              
     if(hard_i2c_para.id_mask == 0x00)    //send high byte of address
        {
        	WRITE_PERI_REG(PREG_I2C_MS_TOKEN_LIST_REG0, ((I2C_END  << 28)              | 
                                                 (I2C_END  << 24)              |   // don't stop the bus, leave it in a read state
                                                 (I2C_END << 20)   |
                                                 (I2C_END << 16)             |      // send Start Repeat for read
                                                 (I2C_END  << 12)             | 
                                                 (I2C_END << 8)               |
                                                 (I2C_END << 4)   |
                                                 (I2C_DATA << 0)    ));
          WRITE_PERI_REG(PREG_I2C_MS_TOKEN_WDATA_REG0, (rw_addr>>8) );   // write the register number
        // Toggle start "low then high"
        WRITE_PERI_REG(PREG_I2C_MS_CTRL, ((READ_PERI_REG(PREG_I2C_MS_CTRL)) & 0xfffffffe )); 	        
        WRITE_PERI_REG(PREG_I2C_MS_CTRL, ((READ_PERI_REG(PREG_I2C_MS_CTRL)) | 0x1 )); 

        hard_i2c_delay(hard_i2c_para.delay << 4);    /*there is enough delay in order to make hardware I2C to work normal*/
        count1 = 1000; 
        while( ((READ_PERI_REG(PREG_I2C_MS_CTRL)) & (1 << I2C_M_STATUS)) && (count1 -- ));  // Wait for the transfer to complete  
 
        if( hard_i2c_detect_ack())    //error 
    	  {
    	  	 hard_i2c_stop();
    	     return -1;
    	   }  
         
        }       

       WRITE_PERI_REG(PREG_I2C_MS_TOKEN_LIST_REG0, ( (I2C_END  << 28)              | 
                                                   (I2C_END  << 24)              | 
                                                   (I2C_END  << 20)              |   // don't stop the bus, leave it in a read state
                                                   (I2C_END << 16)   |
                                                   (I2C_END << 12)             |      // send Start Repeat for read
                                                   (I2C_END << 8)               |
                                                   (I2C_END << 4)   |
                                                   (I2C_DATA << 0)     ));

     WRITE_PERI_REG(PREG_I2C_MS_TOKEN_WDATA_REG0, rw_addr);                           // write the register number

        // Toggle start "low then high"
     WRITE_PERI_REG(PREG_I2C_MS_CTRL, ((READ_PERI_REG(PREG_I2C_MS_CTRL)) & 0xfffffffe )); 	        
     WRITE_PERI_REG(PREG_I2C_MS_CTRL, ((READ_PERI_REG(PREG_I2C_MS_CTRL)) | 0x1 )); 

     hard_i2c_delay(hard_i2c_para.delay << 4);    /*there is enough delay in order to make hardware I2C to work normal*/
     count1 = 1000; 
     while( ((READ_PERI_REG(PREG_I2C_MS_CTRL)) & (1 << I2C_M_STATUS)) && (count1 -- ));  // Wait for the transfer to complete  
 
     if( hard_i2c_detect_ack())    //error 
    	 {
    	  	hard_i2c_stop();
    	    return -1;
    	 }      	       
     }

     hard_i2c_delay(hard_i2c_para.delay << 6);    /*there is enough delay in order to make hardware I2C to work normal*/
     
    WRITE_PERI_REG(PREG_I2C_MS_TOKEN_LIST_REG0, ( (I2C_END  << 28)              | 
                                                   (I2C_END  << 24)              | 
                                                   (I2C_END  << 20)              |   
                                                   (I2C_END << 16)   |
                                                   (I2C_END << 12)             |      
                                                   (I2C_END << 8)               |
                                                   (I2C_END << 4)   |    // send Start Repeat for read
                                                   (I2C_START << 0)     ));     		
 
        // Toggle start "low then high"
     WRITE_PERI_REG(PREG_I2C_MS_CTRL, ((READ_PERI_REG(PREG_I2C_MS_CTRL)) & 0xfffffffe )); 	        
     WRITE_PERI_REG(PREG_I2C_MS_CTRL, ((READ_PERI_REG(PREG_I2C_MS_CTRL)) | 0x1 )); 

     hard_i2c_delay(hard_i2c_para.delay << 5);    /*there is enough delay in order to make hardware I2C to work normal*/
     count1 = 1000; 
     while( ((READ_PERI_REG(PREG_I2C_MS_CTRL)) & (1 << I2C_M_STATUS)) && (count1 -- ));  // Wait for the transfer to complete  
     
    //send the device id(read) 
    WRITE_PERI_REG(PREG_I2C_MS_TOKEN_LIST_REG0, ( (I2C_END  << 28)              | 
                                                   (I2C_END  << 24)              | 
                                                   (I2C_END  << 20)              |   
                                                   (I2C_END << 16)   |
                                                   (I2C_END << 12)             |      
                                                   (I2C_END << 8)               |
                                                   (I2C_END << 4)   |    // send Start Repeat for read
                                                   (I2C_SLAVE_ADDR_READ << 0)     ));     		
 
        // Toggle start "low then high"
     WRITE_PERI_REG(PREG_I2C_MS_CTRL, ((READ_PERI_REG(PREG_I2C_MS_CTRL)) & 0xfffffffe )); 	        
     WRITE_PERI_REG(PREG_I2C_MS_CTRL, ((READ_PERI_REG(PREG_I2C_MS_CTRL)) | 0x1 )); 

     hard_i2c_delay(hard_i2c_para.delay << 4);    /*there is enough delay in order to make hardware I2C to work normal*/
     count1 = 1000; 
     while( ((READ_PERI_REG(PREG_I2C_MS_CTRL)) & (1 << I2C_M_STATUS)) && (count1 -- ));  // Wait for the transfer to complete  
 
    if( hard_i2c_detect_ack())    //error 
    	{
    		hard_i2c_stop();
    	  return -1;
    	}      
    
    while(rw_bytes)
	  { 
             // Pre-Fill the token lists with TOKEN_END
        WRITE_PERI_REG(PREG_I2C_MS_TOKEN_LIST_REG0, 0);
//        WRITE_PERI_REG(PREG_I2C_MS_TOKEN_LIST_REG1, 0);
 

        if(rw_bytes == 1)
            		 WRITE_PERI_REG(PREG_I2C_MS_TOKEN_LIST_REG0,   (I2C_DATA_LAST ) ); 
         else
              	 WRITE_PERI_REG(PREG_I2C_MS_TOKEN_LIST_REG0,  (I2C_DATA ) );  
         rw_bytes--;


        // Toggle start "low then high"
       WRITE_PERI_REG(PREG_I2C_MS_CTRL, ((READ_PERI_REG(PREG_I2C_MS_CTRL)) & 0xfffffffe )); 	        
       WRITE_PERI_REG(PREG_I2C_MS_CTRL, ((READ_PERI_REG(PREG_I2C_MS_CTRL)) | 0x1 )); 
       hard_i2c_delay(hard_i2c_para.delay << 4); 

       count1 = 1000; 
       while( ((READ_PERI_REG(PREG_I2C_MS_CTRL)) & (1 << I2C_M_STATUS)) && (count1 -- )) ; // Wait for the transfer to complete  
          	  	   
            // Read the bytes just read
      tmp_count2 = ((READ_PERI_REG(PREG_I2C_MS_CTRL)) >> 8) & 0xf;

      if(tmp_count2 == 1)      //read data success
          {
             hard_i2c_delay(hard_i2c_para.delay << 5);    /*there is enough delay in order to read data correctly, about 100us*/           	
          	 temp_data0 = READ_PERI_REG(PREG_I2C_MS_TOKEN_RDATA_REG0); 
             (*rw_buff++) = temp_data0 & 0xff;
  
           }
      else
          	{
    		       hard_i2c_stop();
    	         return -1;
          	}  	
        }   
    hard_i2c_stop();        
    return 0;   
}

/*   
     return -1, the it is error in write data,
     return 0, success,
*/
int hard_i2c_write_0( unsigned char *rw_buff, unsigned int rw_bytes, unsigned int rw_addr)
{
	 unsigned char err; 
   int   count1 = 0;	 
   if (rw_addr > hard_i2c_para.max_addr) {
        return -1;
    }

         
	  WRITE_PERI_REG(PREG_I2C_MS_SL_ADDR, hard_i2c_para.device_id); // Set the I2C Address,D0=0:W  
	  WRITE_PERI_REG(PREG_I2C_MS_TOKEN_LIST_REG0, 0);
    WRITE_PERI_REG(PREG_I2C_MS_TOKEN_LIST_REG1, 0);

      //send the start
    WRITE_PERI_REG(PREG_I2C_MS_TOKEN_LIST_REG0, ((I2C_END  << 28)              | 
                                                 (I2C_END  << 24)              |   // don't stop the bus, leave it in a read state
                                                 (I2C_END << 20)   |
                                                 (I2C_END << 16)             |      // send Start Repeat for read
                                                 (I2C_END  << 12)             | 
                                                 (I2C_END << 8)               |
                                                 (I2C_END << 4)   |
                                                 (I2C_START << 0)    ));

        // Toggle start "low then high"
     WRITE_PERI_REG(PREG_I2C_MS_CTRL, ((READ_PERI_REG(PREG_I2C_MS_CTRL)) & 0xfffffffe )); 	        
     WRITE_PERI_REG(PREG_I2C_MS_CTRL, ((READ_PERI_REG(PREG_I2C_MS_CTRL)) | 0x1 )); 

     hard_i2c_delay(hard_i2c_para.delay << 6);    /*there is enough delay in order to make hardware I2C to work normal*/
     count1 = 1000; 
     while( ((READ_PERI_REG(PREG_I2C_MS_CTRL)) & (1 << I2C_M_STATUS)) && (count1 -- ));  // Wait for the transfer to complete  

     
      //send the device id(write)
    WRITE_PERI_REG(PREG_I2C_MS_TOKEN_LIST_REG0, ((I2C_END  << 28)              | 
                                                 (I2C_END  << 24)              |   // don't stop the bus, leave it in a read state
                                                 (I2C_END << 20)   |
                                                 (I2C_END << 16)             |      // send Start Repeat for read
                                                 (I2C_END  << 12)             | 
                                                 (I2C_END << 8)               |
                                                 (I2C_END << 4)   |
                                                 (I2C_SLAVE_ADDR_WRITE << 0)    ));

        // Toggle start "low then high"
     WRITE_PERI_REG(PREG_I2C_MS_CTRL, ((READ_PERI_REG(PREG_I2C_MS_CTRL)) & 0xfffffffe )); 	        
     WRITE_PERI_REG(PREG_I2C_MS_CTRL, ((READ_PERI_REG(PREG_I2C_MS_CTRL)) | 0x1 )); 

     hard_i2c_delay(hard_i2c_para.delay << 4);    /*there is enough delay in order to make hardware I2C to work normal*/
     count1 = 1000; 
     while( ((READ_PERI_REG(PREG_I2C_MS_CTRL)) & (1 << I2C_M_STATUS)) && (count1 -- ));  // Wait for the transfer to complete  
 
     if( hard_i2c_detect_ack())    //error 
    	 {
    	  	hard_i2c_stop();
    	    return -1;
    	 }
    	  
   if(rw_addr !=0xff)
   	{ 
               
     if(hard_i2c_para.id_mask == 0x00)    //send high byte of address
        {
        	WRITE_PERI_REG(PREG_I2C_MS_TOKEN_LIST_REG0, ((I2C_END  << 28)              | 
                                                 (I2C_END  << 24)              |   // don't stop the bus, leave it in a read state
                                                 (I2C_END << 20)   |
                                                 (I2C_END << 16)             |      // send Start Repeat for read
                                                 (I2C_END  << 12)             | 
                                                 (I2C_END << 8)               |
                                                 (I2C_END << 4)   |
                                                 (I2C_DATA << 0)    ));
          WRITE_PERI_REG(PREG_I2C_MS_TOKEN_WDATA_REG0, (rw_addr>>8) );   // write the register number
        // Toggle start "low then high"
        WRITE_PERI_REG(PREG_I2C_MS_CTRL, ((READ_PERI_REG(PREG_I2C_MS_CTRL)) & 0xfffffffe )); 	        
        WRITE_PERI_REG(PREG_I2C_MS_CTRL, ((READ_PERI_REG(PREG_I2C_MS_CTRL)) | 0x1 )); 

        hard_i2c_delay(hard_i2c_para.delay << 4);    /*there is enough delay in order to make hardware I2C to work normal*/
        count1 = 1000; 
        while( ((READ_PERI_REG(PREG_I2C_MS_CTRL)) & (1 << I2C_M_STATUS)) && (count1 -- ));  // Wait for the transfer to complete  
 
        if( hard_i2c_detect_ack())    //error 
    	  {
    	  	 hard_i2c_stop();
    	     return -1;
    	   }  
         
        }       

       WRITE_PERI_REG(PREG_I2C_MS_TOKEN_LIST_REG0, ( (I2C_END  << 28)              | 
                                                   (I2C_END  << 24)              | 
                                                   (I2C_END  << 20)              |   // don't stop the bus, leave it in a read state
                                                   (I2C_END << 16)   |
                                                   (I2C_END << 12)             |      // send Start Repeat for read
                                                   (I2C_END << 8)               |
                                                   (I2C_END << 4)   |
                                                   (I2C_DATA << 0)     ));

       WRITE_PERI_REG(PREG_I2C_MS_TOKEN_WDATA_REG0, rw_addr);                           // write the register number

        // Toggle start "low then high"
       WRITE_PERI_REG(PREG_I2C_MS_CTRL, ((READ_PERI_REG(PREG_I2C_MS_CTRL)) & 0xfffffffe )); 	        
       WRITE_PERI_REG(PREG_I2C_MS_CTRL, ((READ_PERI_REG(PREG_I2C_MS_CTRL)) | 0x1 )); 

       hard_i2c_delay(hard_i2c_para.delay << 4);    /*there is enough delay in order to make hardware I2C to work normal*/
       count1 = 1000; 
       while( ((READ_PERI_REG(PREG_I2C_MS_CTRL)) & (1 << I2C_M_STATUS)) && (count1 -- ));  // Wait for the transfer to complete  
 
       if( hard_i2c_detect_ack())    //error 
    	  {
    	  	hard_i2c_stop();
    	    return -1;
    	  }      	       
     }
            
    while( rw_bytes ) 
    {
           
        // -------------------------------------------------------------------
        // write data to  I2C_TOKEN_WDATA_REG0 , I2C_TOKEN_WDATA_REG1 
        // ------------------------------------------------------------------- 
        // Pre-Fill the token lists with TOKEN_END
       WRITE_PERI_REG(PREG_I2C_MS_TOKEN_LIST_REG0, 0);
//       WRITE_PERI_REG(PREG_I2C_MS_TOKEN_LIST_REG1, 0);
       WRITE_PERI_REG(PREG_I2C_MS_TOKEN_WDATA_REG0, 0);
//       WRITE_PERI_REG(PREG_I2C_MS_TOKEN_WDATA_REG1, 0);

      // Fill Data and Token Lists
       WRITE_PERI_REG(PREG_I2C_MS_TOKEN_LIST_REG0,  (I2C_DATA ) );  
       WRITE_PERI_REG(PREG_I2C_MS_TOKEN_WDATA_REG0,  (*rw_buff));
       rw_buff++;        
       rw_bytes--;  
           
        // Toggle start "low then high"
        WRITE_PERI_REG(PREG_I2C_MS_CTRL, ((READ_PERI_REG(PREG_I2C_MS_CTRL)) & 0xfffffffe )); 	        
        WRITE_PERI_REG(PREG_I2C_MS_CTRL, ((READ_PERI_REG(PREG_I2C_MS_CTRL)) | 0x1 )); 
        
        hard_i2c_delay(hard_i2c_para.delay << 4);  

        count1 = 10000; 
        while( ((READ_PERI_REG(PREG_I2C_MS_CTRL)) & (1 << I2C_M_STATUS)) && (count1 -- )) ; // Wait for the transfer to complete  
 
        if( hard_i2c_detect_ack())    //error 
    	   {
    		   hard_i2c_stop();
    	     return -1;
    	   }       

        if(rw_bytes <= 0) 
           rw_bytes = 0;
      }
        // -------------------------------------------------------------------
        // Send STOP
        // -------------------------------------------------------------------
    hard_i2c_stop();

    return 0;  

}
#define MHZ 1000000
void Init_hard_I2c(void)
{
		unsigned host_clock, i2x_clock_set;
//    host_clock = get_clk81()/MHZ;
    host_clock = 200;//get_clk81()/MHZ;	
    i2x_clock_set = (25*host_clock) / hard_i2c_para.requency;  
    WRITE_PERI_REG(PREG_I2C_MS_CTRL, (((READ_PERI_REG(PREG_I2C_MS_CTRL)) & 0xff800ffd) | (i2x_clock_set << I2C_M_DELAY_LSB)));  //set the i2c clock frequency and enable ack detecting    		
}

