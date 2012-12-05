#ifndef _SDIO_H_
#define _SDIO_H_

/* CMD_SEND */
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

/* SDIO_CONFIG */
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

/* SDIO_STATUS_IRQ */
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

/* SDIO_IRQ_CONFIG */
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


/* SDIO_MULT_CONFIG */
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

/* Encoder I/F register */
#define CMD_ARGUMENT                (volatile unsigned long *)0xc1108c20
#define CMD_SEND                    (volatile unsigned long *)0xc1108c24
#define SDIO_CONFIG                 (volatile unsigned long *)0xc1108c28
#define SDIO_STATUS_IRQ             (volatile unsigned long *)0xc1108c2c
#define SDIO_IRQ_CONFIG             (volatile unsigned long *)0xc1108c30
#define SDIO_MULT_CONFIG            (volatile unsigned long *)0xc1108c34
#define SDIO_M_ADDR                 (volatile unsigned long *)0xc1108c38
#define SDIO_EXTENSION              (volatile unsigned long *)0xc1108c3c

// bit   17    -- arc_ahb_cbus_int_en
// bit   16    -- amrisc_ahb_cbus_int_en
// bit 15:8    -- SDIO_FIFO_count
// bit  7:4    -- m_req_count
// bit    3    -- reserved
// bit    2    -- m_write_cbus
// bit    1    -- m_req_cbus
// bit    0    -- sdio_ahb_cbus_enable
#define SDIO_AHB_CBUS_CTRL          (volatile unsigned long *)0xc1108c60

// m_rd_data[31:0] when (m_write_cbus == 0)  CBUS write
// m_wr_data[31:0] when (m_write_cbus == 1)  CBUS read
#define SDIO_AHB_CBUS_M_DATA        (volatile unsigned long *)0xc1108c64

#define NO_DELAY_DATA 0

/*
 * Freq Calculating Formula: Freq(SD_CLK)=Freq(SD_HOST_MODULE_CLK_INPUT)/((CLK_DIV+1)*2)
 */
extern Timing * timing_cur;
#define CLK_DIV						((timing_cur->sdio)-1)     // should be around 375KHz
#define SD_MMC_TRANFER_CLK_DIV		((timing_cur->sdio>>4)-1)  // should be around 6MHz
#define VOLTAGE_VALIDATION_RETRY    0x8000
#define APP55_RETRY                 3

#define TIMEOUT_SHORT       (300*1000)  /* 250ms */
#define TIMEOUT_DATA        (350*1000)  /* 300ms (TIMEOUT_SHORT+ (READ_SIZE*8)/10000000) */

/* 250ms of timeout */
#define TIMEOUT_SD          0x1fff /* maximum SD hardware counter */

#define READ_VOLATILE_UINT32_(a)  (*(a))

#define WAIT_BUSY(x, err)   \
    if (wait_busy(x)!=0) {  \
        OUT_PUT(err);       \
        return err;         \
    }

#endif
