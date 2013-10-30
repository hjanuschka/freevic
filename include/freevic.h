
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <scsi/sg.h>
#include <inttypes.h>
#include <sys/types.h>
#include <time.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <malloc.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <getopt.h>
#include <stdarg.h>
#include <dlfcn.h>
#include <endian.h>
 

#define INQ_REPLY_LEN 96
#define INQ_CMD_CODE 0x12
#define INQ_CMD_LEN 6


#define VERSION "0.1"
#define SENSEBUF_LEN 32


int freevic_cdb(int device, uint8_t  * cdb, uint8_t cdb_len, int cdb_type, uint8_t * return_buffer, int return_buffer_len);
void freevic_hexdump(void *mem, unsigned int len);

void debug_printf(char *fmt, ...);
void debug_printf(char *fmt, ...);

struct evic_core_cfg {
	uint8_t padder[6];
	char product_name[4];
	uint8_t padder2[6];
	uint32_t serial_no;
		
};



//Evic Live Config
struct evic_current_cfg {
	uint8_t volt_value;
	uint8_t atomizer_resistance;
	uint8_t padd5[11];
	//14-15
	uint16_t puff_count_total;
	
	uint8_t padder[7];
	uint8_t battery_perc; //22
	
	uint16_t battery_mah;
	uint8_t padder1[44];
	char first_name[17];
	char last_name[20];
	//86
	uint8_t gender;
	uint8_t age;
	
	uint8_t padder3[6];
	uint8_t watt_value;
	uint8_t padder2;
	uint8_t mode;	
	
	uint8_t padder4[12];
	//PAD FROM 116-130 RVWA 130
	uint8_t rvwa_seconds[10];
	uint8_t rvwa_watt[10];
	uint8_t rvwa_padder[2];
	char 		rvwa_name[10];
	
	
	uint8_t rvwb_seconds[10];
	uint8_t rvwb_watt[10];
	uint8_t rvwb_padder[2];
	char 		rvwb_name[10];
	
	uint8_t rvwc_seconds[10];
	uint8_t rvwc_watt[10];
	uint8_t rvwc_padder[2];
	char 		rvwc_name[10];
	
	uint8_t rvv_seconds[10];
	uint8_t rvv_volts[10];
	
	
};
