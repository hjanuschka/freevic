#include "freevic.h"



int freevic_cdb(int device, uint8_t  * cdb, uint8_t cdb_len, int cdb_type, uint8_t * return_buffer, int return_buffer_len) {
		//SG_DXFER_FROM_DEV
		//SG_DXFER_TO_DEV
		int fd,rc;
		struct sg_io_hdr sg_io;
		uint8_t sensebuf[SENSEBUF_LEN] = {0};


		debug_printf("------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
		debug_printf("COMMAND:\n");
		freevic_hexdump(cdb, cdb_len);
		
		


		memset(&sg_io, 0, sizeof(sg_io));
		sg_io.interface_id    = 'S';
		sg_io.cmdp            = cdb;
		sg_io.cmd_len         = cdb_len;
		sg_io.dxferp          = return_buffer;
		sg_io.dxfer_len       = return_buffer_len;
		sg_io.dxfer_direction = cdb_type;
		sg_io.sbp             = sensebuf;
		sg_io.mx_sb_len       = sizeof(sensebuf);
		sg_io.timeout         = 5000;   // 5 seconds

		
		
		rc = ioctl (device, SG_IO, &sg_io);
		
		if(return_buffer_len>0 && cdb_type == SG_DXFER_FROM_DEV) {
			debug_printf("RETURN BLOCK\n");
			freevic_hexdump(return_buffer, return_buffer_len);
			debug_printf("------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
			
		}
		if(return_buffer_len>0 && cdb_type == SG_DXFER_TO_DEV) {
			debug_printf("SENT BLOCK\n");
			freevic_hexdump(return_buffer, return_buffer_len);
			debug_printf("------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
		}
		
		if(rc == -1 || sg_io.status != 0x00) {
			debug_printf("ioctl1 seems to have failed %s %d\n", strerror(errno), fd);
			close(fd);
			exit(1);
		}

}