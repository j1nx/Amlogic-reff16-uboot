#define C_ASSIST_HW_REV                            (volatile unsigned long *)0xc1107d4c

// --------------------------------------------------------
//                     sdio boot code from tim
// --------------------------------------------------------
// for CMD_SEND
#define     repeat_package_times_bit  24
#define     use_int_window_bit        21
#define     use_int_window_bit        21
#define     cmd_send_data_bit         20
#define     check_busy_on_dat0_bit    19
#define     response_crc7_from_8_bit  18
#define     response_have_data_bit 17
#define     response_do_not_have_crc7_bit 16
#define     cmd_response_bits_bit 8
#define     cmd_command_bit       0
// for SDIO_CONFIG
#define     sdio_write_CRC_ok_status_bit 29
#define     sdio_write_Nwr_bit        23
#define     m_endian_bit              21
#define     bus_width_bit             20   // 0 1-bit, 1-4bits
#define     data_latch_at_negedge_bit 19
#define     response_latch_at_negedge_bit 18
#define     cmd_argument_bits_bit 12
#define     cmd_out_at_posedge_bit 11
#define     cmd_disable_CRC_bit   10
#define     cmd_clk_divide_bit    0

// SDIO_STATUS_IRQ
#define     sdio_timing_out_count_bit   19
#define     arc_timing_out_int_en_bit   18
#define     amrisc_timing_out_int_en_bit 17
#define     sdio_timing_out_int_bit      16
#define     sdio_status_info_bit        12
#define     sdio_set_soft_int_bit       11
#define     sdio_soft_int_bit           10
#define     sdio_cmd_int_bit             9
#define     sdio_if_int_bit              8
#define     sdio_data_write_crc16_ok_bit 7
#define     sdio_data_read_crc16_ok_bit  6
#define     sdio_cmd_crc7_ok_bit  5
#define     sdio_cmd_busy_bit     4
#define     sdio_status_bit       0

// SDIO_IRQ_CONFIG
#define     force_halt_bit           30
#define     sdio_force_read_back_bit 24
#define     disable_mem_halt_bit     22
#define     sdio_force_output_en_bit 16
#define     soft_reset_bit           15
#define     sdio_force_enable_bit    14
#define     sdio_force_read_data_bit  8
#define     sdio_if_int_config_bit 6
#define     arc_soft_int_en_bit    5
#define     arc_cmd_int_en_bit     4
#define     arc_if_int_en_bit      3
#define     amrisc_soft_int_en_bit 2
#define     amrisc_cmd_int_en_bit  1
#define     amrisc_if_int_en_bit   0


// for SDIO_MULT_CONFIG
#define     data_catch_finish_point_bit 16
#define     response_read_index_bit     12
#define     data_catch_readout_en_bit    9
#define     write_read_out_index_bit     8
#define     data_catch_level_bit   6
#define     stream_8_bits_mode_bit 5
#define     stream_enable_bit      4
#define     ms_sclk_always_bit     3
#define     ms_enable_bit_bit      2
#define     SDIO_port_sel_bit      0

//---- Encoder I/F register
#define CMD_ARGUMENT                             (volatile unsigned long *)0xc1108c20
#define CMD_SEND                                 (volatile unsigned long *)0xc1108c24
#define SDIO_CONFIG                              (volatile unsigned long *)0xc1108c28
#define SDIO_STATUS_IRQ                          (volatile unsigned long *)0xc1108c2c
#define SDIO_IRQ_CONFIG                          (volatile unsigned long *)0xc1108c30
#define SDIO_MULT_CONFIG                         (volatile unsigned long *)0xc1108c34
#define SDIO_M_ADDR                              (volatile unsigned long *)0xc1108c38
#define SDIO_EXTENSION                           (volatile unsigned long *)0xc1108c3c

#define ERROR_NONE                  0
#define ERROR_READ_BLOCK            -13
#define ERROR_STOP_TRANSMISSION     -14

//#define CLK_DIV 3
#define NO_DELAY_DATA 0
//#define CLK_DIV 0
#define CLK_DIV_27 2
#define CLK_DIV_13 1

#define VOLTAGE_VALIDATION_RETRY    0x8000
#define APP55_RETRY                 3

/* 250ms of timeout */
#define TIMEOUT_SHORT_27    ((27L*0x100000/((CLK_DIV_27+1)*2))*250/1000)
#define TIMEOUT_DATA_27     (TIMEOUT_SHORT_27 + READ_SIZE * 8)
#define TIMEOUT_SHORT_13    ((135L*0x100000/((CLK_DIV_13+1)*2))/10*250/1000)
#define TIMEOUT_DATA_13     (TIMEOUT_SHORT_13 + READ_SIZE * 8)
#define TIMEOUT_SD           0x1fff /* maximum SD hardware counter */

#define MAGIC_WORD1         0x454b494e  /* "NIKE" */
#define MAGIC_WORD2         0x31305442  /* "BT01" */
#define READ_VOLATILE_UINT32_(a)   (*(a))

#define     READ_SIZE  0x20000 
//_Asm unsigned READ_VOLATILE_UINT32_(volatile unsigned long *addr)
//{
//    % reg addr
//    ld.di %r0, [addr]
//    add  %r0, %r0, 0
//}

#define WAIT_BUSY(x, err) \
    if (wait_busy(x)!=0) { \
        return err; \
    }


static int wait_busy(unsigned timeout)
{
    unsigned this_timeout = 0;
    unsigned r;

    *SDIO_STATUS_IRQ = TIMEOUT_SD << sdio_timing_out_count_bit;


    while (1) {
        r = READ_VOLATILE_UINT32_(SDIO_STATUS_IRQ);
        
        if ((r >> sdio_timing_out_count_bit) == 0) {
            this_timeout += TIMEOUT_SD;
            *SDIO_STATUS_IRQ = TIMEOUT_SD << sdio_timing_out_count_bit;
        }

        if (this_timeout >= timeout) {
            return -1;
        }

        if(((r >> sdio_cmd_busy_bit) & 0x1) == 0) {
            return 0;
        }
    }            
}

const unsigned clock_data[][3]={
    {
        CLK_DIV_13,
        TIMEOUT_SHORT_13,
        TIMEOUT_DATA_13,
    },
    {
        CLK_DIV_27,
        TIMEOUT_SHORT_27,
        TIMEOUT_DATA_27,
    },
};



int sd_read_i(unsigned offset,unsigned count,unsigned target)
{
	 	int CLK_DIV;
	 	unsigned read_count, addr_offset = 0;
		unsigned TIMEOUT_SHORT,TIMEOUT_DATA;
		register int clock_27   = ((*C_ASSIST_HW_REV) & (1 << 12)) ? 0 : 1;
		CLK_DIV = clock_data[clock_27][0];
		TIMEOUT_SHORT = clock_data[clock_27][1];
		TIMEOUT_DATA=clock_data[clock_27][2];
//--------------------------------------------------------------------------------
    // send CMD17 -- READ_BLOCK
    while(count)
    {
    	if(count >= (512*256))
    		read_count = 512*256;
    	else
    		read_count = count;
   		(*CMD_ARGUMENT) = (offset + addr_offset);
   		(*SDIO_M_ADDR) = (target + addr_offset); // data read data
   		(*SDIO_EXTENSION) = ((1 << (9 + 3)) + (16 - 1)) << 16; // data_rw_number 4096(data)+16(crc)-1
   		(*CMD_SEND) =  (
        	((((read_count + 511) >> 9) - 1) << repeat_package_times_bit) | // packet number
        	//(1 << repeat_package_times_bit) | // 2 blks
        	(0 << cmd_send_data_bit) | // RESPONSE has CRC7
        	(0 << response_do_not_have_crc7_bit) | // RESPONSE has CRC7
        	(1 << use_int_window_bit) | // will disable interrupt
        	(1 << response_have_data_bit) | // RESPONSE has data receive
        	(45 << cmd_response_bits_bit) | // RESPONSE have 7(cmd)+32(respnse)+7(crc)-1 data
        	((0x40+18) << cmd_command_bit)
       	);

    	WAIT_BUSY(TIMEOUT_SHORT*256, ERROR_READ_BLOCK);

    	//--------------------------------------------------------------------------------
    	// send CMD12 -- STOP_TRANSMISSION
   		(*CMD_ARGUMENT) = 0;
   		(*CMD_SEND) =  (
        		(1 << check_busy_on_dat0_bit) | // RESPONSE is R1b
        		(0 << response_crc7_from_8_bit) | // RESPONSE CRC7 is normal
        		(0 << response_do_not_have_crc7_bit) | // RESPONSE has CRC7
        		(45 << cmd_response_bits_bit) | // RESPONSE have 7(cmd)+32(respnse)+7(crc)-1 data
        		((0x40+12) << cmd_command_bit)
       		);
   		WAIT_BUSY(TIMEOUT_DATA, ERROR_STOP_TRANSMISSION);
   		
		count -= read_count;
    	addr_offset += read_count;
   	}

ERROR:   
    return ERROR_NONE;
}

void _sd_read(unsigned dummy,unsigned target,unsigned size)
{
 
	sd_read_i(0xc00,size*4,target);
}
