#include "freevic.h"

/*
IT's alpha 0.1 - it is not beautyfull, just a proof of concept - once the structs have been worked out i ll do a cleaner version :)
use at your own risk!!


*/

int in_debug=1;

int evic_flash_fw(char * device, char * fw_file) {
	
	
	int evic_device_handle;
	uint8_t return_buffer[1024] = {0};
	char * fw_block_file;
	int x;
	
	FILE * fp;
	
	return 1;
	
	
	uint8_t unlock_device_cdb_1[8]= {0xcc, 0x80, 0x02, 0x00,0x80,0x00,0x00,0x04};
	uint8_t unlock_device_cdb_2[8]= {0x1e, 0x00, 0x00, 0x01,0x00,0x00,0x00,0x00};
	
	uint8_t fw_cmd1[8] = 						{0xcc, 0x80,0x02,0x00,0x80,0x00,0x00,0x04};
	uint8_t user_info_cdb[8] =      {0xc9, 0x00, 0x02, 0x00,0x80,0x00,0x00,0x04};
	
	uint8_t fw_out1[8] = 						{0xc9, 0x00, 0x01, 0x00,0x80,0x00,0x00,0x04};
	uint8_t fw_in1[8] = 						{0xc9, 0x00, 0x02, 0x00,0x80,0x00,0x00,0x04};
	uint8_t fw_state[8] = 					{0xcf, 0x00, 0x01, 0x00,0x00,0x00,0x00,0x04};
	uint8_t fw_forward[8] = 				{0xce, 0x80, 0x01, 0x00,0x20,0x00,0x00,0x04};
	uint8_t fw_forward2[8] = 				{0xce, 0x80, 0x02, 0x00,0x20,0x00,0x00,0x04};
	uint8_t fw_forward3[8] = 				{0xce, 0x80, 0x01, 0x00,0x24,0x00,0x00,0x04};
	uint8_t fw_forward4[8] = 				{0xce, 0x80, 0x02, 0x00,0x24,0x00,0x00,0x04};
	uint8_t fw_forward5[8] = 				{0xce, 0x80, 0x01, 0x00,0x28,0x00,0x00,0x04};
	uint8_t fw_forward6[8] = 				{0xce, 0x80, 0x02, 0x00,0x28,0x00,0x00,0x04};
	
	
	FILE * fw;
	//Unlock
	//Fw_cmd1
	//user_info_fetch
	//OUT FW 1k // first block
	//FW state
	
	evic_device_handle=open(device, O_RDONLY);
	
	
	//Unlock
	freevic_cdb(evic_device_handle, unlock_device_cdb_1, sizeof(unlock_device_cdb_1), SG_DXFER_FROM_DEV, return_buffer, 0);
	freevic_cdb(evic_device_handle, unlock_device_cdb_2, sizeof(unlock_device_cdb_2), SG_DXFER_FROM_DEV, return_buffer, 0);

	
	
	freevic_cdb(evic_device_handle, fw_cmd1, sizeof(fw_cmd1), SG_DXFER_FROM_DEV, return_buffer, 0);
	
	
	
	
	//Fill Buffer with 1k of firmware (the first block)
	memset(return_buffer, 0x0, 1024);
	fw=fopen(fw_file, "rb");
	fread(&return_buffer, sizeof(uint8_t), 1024, fw);

	

	freevic_cdb(evic_device_handle, fw_out1, sizeof(fw_out1), SG_DXFER_TO_DEV, return_buffer, sizeof(return_buffer));
	freevic_cdb(evic_device_handle, fw_state, sizeof(fw_state), SG_DXFER_FROM_DEV, return_buffer,  sizeof(return_buffer));
	
	
	//Next Block :
	//WireShark Filter
	//usb.data_len == 1024 and usb.endpoint_number.direction == 0
	
	uint8_t cureg=0x20;
	for(x=1; x<=56; x++) {
				
				fw_forward[4]=cureg;
				fw_forward2[4]=cureg;
			
				memset(return_buffer, 0x0, 1024);
				fread(&return_buffer, sizeof(uint8_t), 1024, fw);
				freevic_cdb(evic_device_handle, fw_out1, sizeof(fw_out1), SG_DXFER_TO_DEV, return_buffer, sizeof(return_buffer));
				freevic_cdb(evic_device_handle, fw_forward, sizeof(fw_forward), SG_DXFER_FROM_DEV, return_buffer, 0);
				freevic_cdb(evic_device_handle, fw_forward2, sizeof(fw_forward2), SG_DXFER_FROM_DEV, return_buffer, 0);
				freevic_cdb(evic_device_handle, fw_in1, sizeof(fw_in1), SG_DXFER_FROM_DEV, return_buffer, sizeof(return_buffer));
	
	
				cureg = cureg+4;
	}
	fclose(fp);
	exit(1);
	



	
		
}
int evic_get_data_from_device(char * device, uint8_t * evic_core_reply, int evic_core_reply_size, uint8_t * evic_user_reply,int evic_user_reply_size) {
	int evic_device_handle;
	uint8_t return_buffer[1024] = {0};
	
	
	uint8_t unlock_device_cdb_1[8]= {0xcc, 0x80, 0x02, 0x00,0x80,0x00,0x00,0x04};
	uint8_t unlock_device_cdb_2[8]= {0x1e, 0x00, 0x00, 0x01,0x00,0x00,0x00,0x00};
	uint8_t core_info_cdb[8] =      {0xc9, 0x00, 0x03, 0x00,0x80,0x00,0x00,0x04};
	uint8_t user_info_cdb[8] =      {0xc9, 0x00, 0x02, 0x00,0x80,0x00,0x00,0x04};
	
	//Open the device
	evic_device_handle=open(device, O_RDONLY);
	
	
	//SEND CDBS
	//Unlock
	freevic_cdb(evic_device_handle, unlock_device_cdb_1, sizeof(unlock_device_cdb_1), SG_DXFER_FROM_DEV, return_buffer, 0);
	freevic_cdb(evic_device_handle, unlock_device_cdb_2, sizeof(unlock_device_cdb_2), SG_DXFER_FROM_DEV, return_buffer, 0);

	//Retrieve Core info
	freevic_cdb(evic_device_handle, core_info_cdb, sizeof(core_info_cdb), SG_DXFER_FROM_DEV, evic_core_reply, evic_core_reply_size);
	
	//Required to reset bits
	freevic_cdb(evic_device_handle, unlock_device_cdb_1, sizeof(unlock_device_cdb_1), SG_DXFER_FROM_DEV, return_buffer, 0);
	freevic_cdb(evic_device_handle, unlock_device_cdb_2, sizeof(unlock_device_cdb_2), SG_DXFER_FROM_DEV, return_buffer, 0);
	
	//Retrieve User info
	freevic_cdb(evic_device_handle, user_info_cdb, sizeof(user_info_cdb), SG_DXFER_FROM_DEV, evic_user_reply, evic_user_reply_size);
	
	close(evic_device_handle);
	
}
 
int main(int argc, char ** argv) {
	
	/*
	argv[1] = device or file
	argv[2] = if argv0 is file - than argv1 == core argv2 == user dump
	*/
	int x;
	uint8_t evic_reply[512];
	uint8_t evic_core_reply[512];
	FILE * evic_core_dmp, * evic_user_dmp;
	struct stat file_info;
	struct evic_current_cfg * evic_status; //this is where the current live config will be represented
	struct evic_core_cfg * evic_core;
	
	
	
	stat(argv[1], &file_info);
	
	
	if(S_ISREG(file_info.st_mode)) {
		debug_printf("IS FILE\n");
		evic_core_dmp = fopen(argv[1], "rb");
		evic_user_dmp = fopen(argv[2], "rb");
		
		fread(evic_core_reply,sizeof(uint8_t),sizeof(evic_core_reply),evic_core_dmp);
		fread(evic_reply,sizeof(uint8_t),sizeof(evic_reply),evic_user_dmp);
		
		fclose(evic_core_dmp);
		fclose(evic_user_dmp);
	} else {
		debug_printf("is device\n");
		evic_get_data_from_device(argv[1], evic_core_reply,sizeof(evic_core_reply), evic_reply,sizeof(evic_reply));
		
		//exit(1);
		
	}
	
	
	

	evic_status = (struct evic_current_cfg *)&evic_reply;
	evic_core = (struct evic_core_cfg *)&evic_core_reply;
	
	//terminate product name 
	evic_core->product_name[4]=0x0;
	
	printf("###############################\n");
	printf("Core Evic Settings:\n");
	printf("Serial: %u\n", htobe32(evic_core->serial_no));
	printf("Product Name: '%s'\n", evic_core->product_name);
	printf("###############################\n");
	printf("User Evic Settings:\n");
	printf("###############################\n");
	printf("First Name: %s\n", evic_status->first_name);
	printf("Last Name: %s\n", evic_status->last_name);
	printf("Age: %u\n", evic_status->age);
	
	//0=mr 1=mrs 2=miss 3=ms
	printf("Gender: %u ", evic_status->gender);
	switch(evic_status->gender) {
		
		case 0:
			printf("Mr");
		break;
		case 1:
			printf("Mrs");
		break;
		case 2:
			printf("Miss");
		break;
		case 3:
			printf("Ms");
		break;
		default:
				printf("Unkown");
		break;
		
		
	}
	printf("\n");
	
	printf("Battery Life: %u%%\n", evic_status->battery_perc);
	printf("Evic Mode: %u - ", evic_status->mode);
	
	
	switch(evic_status->mode) {
			case 0:
				printf("Volt Mode (value: %0.2f)", (float)evic_status->volt_value/10);
			break;		
			case 1:
				printf("Watt Mode (value: %0.2f)", (float)evic_status->watt_value/10);
			break;
			default:
				printf("unkown");
			break;
	}
	printf("\n");
	
	printf("Atomizer Resistance %0.2f ohm\n", (float)evic_status->atomizer_resistance/10);	 
	printf("Puff Count Total:%u\n", htobe16(evic_status->puff_count_total));	 
	printf("Battery mAh:%u\n", htobe16(evic_status->battery_mah));	 
	
	
	
	printf("###############################\n");
			printf("RVW Block 1 Settings\n");
			printf("\t Name: %s\n", evic_status->rvwa_name);
			printf("\t\t Seconds:");
			for(x=0; x<10; x++) {
				printf("%0.2f,", (float)evic_status->rvwa_seconds[x]/10);
			}	
			printf("\n");
			printf("\t\t Watt:");
			for(x=0; x<10; x++) {
				printf("%0.2f,", (float)evic_status->rvwa_watt[x]/10);
			}	
			printf("\n");
	
			printf("RVW Block 2 Settings\n");
			printf("\t Name: %s\n", evic_status->rvwb_name);
			printf("\t\t Seconds:");
			for(x=0; x<10; x++) {
				printf("%0.2f,", (float)evic_status->rvwb_seconds[x]/10);
			}	
			printf("\n");
			printf("\t\t Watt:");
			for(x=0; x<10; x++) {
				printf("%0.2f,", (float)evic_status->rvwb_watt[x]/10);
			}	
			printf("\n");
	
			printf("RVW Block 3 Settings\n");
			printf("\t Name: %s\n", evic_status->rvwc_name);
			printf("\t\t Seconds:");
			for(x=0; x<10; x++) {
				printf("%0.2f,", (float)evic_status->rvwc_seconds[x]/10);
			}	
			printf("\n");
			printf("\t\t Watt:");
			for(x=0; x<10; x++) {
				printf("%0.2f,", (float)evic_status->rvwc_watt[x]/10);
			}	
			printf("\n");
	
	
			printf("RVV Settings\n");
			
			printf("\t\t Seconds:");
			for(x=0; x<10; x++) {
				printf("%0.2f,", (float)evic_status->rvv_seconds[x]/10);
			}	
			printf("\n");
			printf("\t\t Watt:");
			for(x=0; x<10; x++) {
				printf("%0.2f,", (float)evic_status->rvv_volts[x]/10);
			}	
			printf("\n");
	
	printf("###############################\n");


}






