#define SHMCU_ADDR		0x66
#define ADAPTER_STATUS	0x12
#define BATTERY_STATUS 	0x15
#define ADAPTER_OFF		0x80
#define HALF_CAPACITY	0x1E   //battery 30%

int is_ac_online_sh(void);
int measure_capacity_battery_sh(void);
