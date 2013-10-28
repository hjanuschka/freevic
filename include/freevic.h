
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


int evic_send_CDB(char * device, uint8_t * rbuf, size_t rbuf_len, uint8_t ** cdbs, size_t cdb_count, int nonblock, int already_open_device, int exit_or_return);
void debugPrintcmd(uint8_t *cc);
void evic_cmd( uint8_t * cc, uint8_t c1, uint8_t c2, uint8_t c3, uint8_t c4, uint8_t c5, 		uint8_t c6, uint8_t c7, uint8_t c8, uint8_t c9, uint8_t c10, 		uint8_t c11, uint8_t c12, uint8_t c13, uint8_t c14, uint8_t c15);
void debug_printf(char *fmt, ...);



//Evic Live Config
struct evic_current_cfg {
	uint8_t volt_value;
	uint8_t atomizer_resistance;
	uint8_t padd5[11];
	//14-15
	uint16_t puff_count_total;
	
	uint8_t padder[7];
	uint8_t battery_perc;
	uint8_t padder1[46];
	char last_name[17];
	char first_name[22];
	//86
	uint8_t padder3[6];
	uint8_t watt_value;
	uint8_t padder2;
	uint8_t mode;	
	
};
