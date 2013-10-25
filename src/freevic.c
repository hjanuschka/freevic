#include "freevic.h"


int in_debug=1;



int main(int argc, char ** argv) {
	
	uint8_t cdb[16] = {0}, evicreply[1024] = {0};
	
	int rc;
	uint8_t * cdbs[1];

//	long long rsize;
	// 12 00 00 00 60 00

	evic_cmd(cdb, 0xc9, 0x00, 0x02, 0x00, 0x80, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00);
	cdbs[0]=cdb;
	rc=evic_send_CDB("/dev/sg2", evicreply, sizeof(evicreply), cdbs, 1, 0, -1, 1);
	

	
	
	
	
}

void evic_cmd( uint8_t * cc, uint8_t c1, uint8_t c2, uint8_t c3, uint8_t c4, uint8_t c5,
		uint8_t c6, uint8_t c7, uint8_t c8, uint8_t c9, uint8_t c10,
		uint8_t c11, uint8_t c12, uint8_t c13, uint8_t c14, uint8_t c15) {
	cc[0]=c1;
	cc[1]=c2;
	cc[2]=c3;
	cc[3]=c4;
	cc[4]=c5;
	cc[5]=c6;
	cc[6]=c7;
	cc[7]=c8;
	cc[8]=c9;
	cc[9]=c10;
	cc[10]=c11;
	cc[11]=c12;
/*
	cc[12]=c13;
	cc[13]=c14;
	cc[14]=c15;
*/

	
}

void debug_printf(char *fmt, ...) {
	va_list args;

	if(in_debug == 0)
		return;

	va_start(args, fmt);
	printf("[debug] - ");
	vprintf(fmt, args);
	printf("\n");
	va_end(args);
}


int evic_send_CDB(char * device, uint8_t * rbuf, size_t rbuf_len, uint8_t ** cdbs, size_t cdb_count, int nonblock, int already_open_device, int exit_or_return) {
	int x;
	uint8_t * cdb;
	int fd,rc;
	struct sg_io_hdr sg_io;
	uint8_t sensebuf[SENSEBUF_LEN] = {0};

	if(already_open_device < 0) {
		debug_printf("opening device %s", device);
		if(nonblock == 1) {
			fd = open(device, O_RDONLY | O_NONBLOCK);
		} else {
			fd = open(device, O_RDONLY);
		}
		if(fd == -1) {
			if(exit_or_return == 1)  {
				return -1;
			} else {
				debug_printf( "could not open device\n");
				exit(1);
			}
		}
	} else {
		//debug_printf("USE already open DEVICE: %d", already_open_device);
		fd = already_open_device;
	}


	for(x=0; x<cdb_count; x++) {
		cdb=cdbs[x];
		
		debugPrintcmd(cdb);

		memset(&sg_io, 0, sizeof(sg_io));
		sg_io.interface_id    = 'S';
		sg_io.cmdp            = cdb;
		sg_io.cmd_len         = 12*sizeof(uint8_t);
		sg_io.dxferp          = rbuf;
		sg_io.dxfer_len       = rbuf_len;
		sg_io.dxfer_direction = SG_DXFER_FROM_DEV;
		sg_io.sbp             = sensebuf;
		sg_io.mx_sb_len       = sizeof(sensebuf);
		sg_io.timeout         = 5000;   // 5 seconds

		
		
		rc = ioctl (fd, SG_IO, &sg_io);
		//debugPrintf("status: %02X", sg_io.status);
		if(rc == -1 || sg_io.status != 0x00) {
			if(exit_or_return == 1)  {
				close(fd);
				if(sg_io.status != 0x00) return -1;
				return rc;
			}
			debug_printf("ioctl1 seems to have failed %s %d\n", strerror(errno), fd);
			close(fd);
			exit(1);
		}



	}
	//only close if we opened it ;)
	if(already_open_device < 0) {
		debug_printf("closing device");
		close(fd);
	}
	return 1;
}
void debugPrintcmd(uint8_t *cc) {
	if(in_debug == 1)
		debug_printf("CMD: %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x",
				cc[0], cc[1], cc[2], cc[3], cc[4], cc[5], cc[6], cc[7],
				cc[8], cc[9], cc[10], cc[11]);
}