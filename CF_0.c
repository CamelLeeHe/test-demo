//ver 0.01
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#ifndef WIN32
#include <sys/resource.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/mman.h>
#include <rtdm/rtdm.h>
#include <native/task.h>
#include <native/sem.h>
#include <native/mutex.h>
#include <native/timer.h>
#include <rtdk.h>
#include <pthread.h>
#include "ecrt.h"
#include "OD_0_0.h"

extern UNS16 gUIBufs[1000];
extern UNS16 gUOBufs[1000];
UNS8 send_consise_dcf(CO_Data* d, UNS8 nodeId,ec_slave_config_t *sc);
RT_TASK my_task;
static int run = 1;
/****************************************************************************/
/* Master 0, Slave 2, "HIO-1111 Dig. In"
 * Vendor ID:       0x00000851
 * Product code:    0x08511111
 * Revision number: 0x0191008e
 */

ec_pdo_entry_info_t slave_2_pdo_entries[] = {
    {0x3001, 0x01, 8}, /* Input */
    {0x3001, 0x02, 8}, /* Input */
};

ec_pdo_info_t slave_2_pdos[] = {
    {0x1600, 1, slave_2_pdo_entries + 0}, /* Byte 0 */
    {0x1601, 1, slave_2_pdo_entries + 1}, /* Byte 1 */
};

ec_sync_info_t slave_2_syncs[] = {
    {0, EC_DIR_INPUT, 2, slave_2_pdos + 0, EC_WD_DISABLE},
    {0xff}
};

/* Master 0, Slave 3, "HIO-1111 Dig. In"
 * Vendor ID:       0x00000851
 * Product code:    0x08511111
 * Revision number: 0x0191008e
 */

ec_pdo_entry_info_t slave_3_pdo_entries[] = {
    {0x3001, 0x01, 8}, /* Input */
    {0x3001, 0x02, 8}, /* Input */
};

ec_pdo_info_t slave_3_pdos[] = {
    {0x1600, 1, slave_3_pdo_entries + 0}, /* Byte 0 */
    {0x1601, 1, slave_3_pdo_entries + 1}, /* Byte 1 */
};

ec_sync_info_t slave_3_syncs[] = {
    {0, EC_DIR_INPUT, 2, slave_3_pdos + 0, EC_WD_DISABLE},
    {0xff}
};

/* Master 0, Slave 4, "HIO-1121 Dig. Out"
 * Vendor ID:       0x00000851
 * Product code:    0x08511121
 * Revision number: 0x01f5008e
 */

ec_pdo_entry_info_t slave_4_pdo_entries[] = {
    {0x3101, 0x01, 8}, /* Output */
    {0x3101, 0x02, 8}, /* Output */
};

ec_pdo_info_t slave_4_pdos[] = {
    {0x1a00, 1, slave_4_pdo_entries + 0}, /* Byte 0 */
    {0x1a01, 1, slave_4_pdo_entries + 1}, /* Byte 1 */
};

ec_sync_info_t slave_4_syncs[] = {
    {0, EC_DIR_OUTPUT, 1, slave_4_pdos + 0, EC_WD_ENABLE},
    {1, EC_DIR_OUTPUT, 1, slave_4_pdos + 1, EC_WD_ENABLE},
    {0xff}
};

/* Master 0, Slave 5, "HIO-1121 Dig. Out"
 * Vendor ID:       0x00000851
 * Product code:    0x08511121
 * Revision number: 0x01f5008e
 */

ec_pdo_entry_info_t slave_5_pdo_entries[] = {
    {0x3101, 0x01, 8}, /* Output */
    {0x3101, 0x02, 8}, /* Output */
};

ec_pdo_info_t slave_5_pdos[] = {
    {0x1a00, 1, slave_5_pdo_entries + 0}, /* Byte 0 */
    {0x1a01, 1, slave_5_pdo_entries + 1}, /* Byte 1 */
};

ec_sync_info_t slave_5_syncs[] = {
    {0, EC_DIR_OUTPUT, 1, slave_5_pdos + 0, EC_WD_ENABLE},
    {1, EC_DIR_OUTPUT, 1, slave_5_pdos + 1, EC_WD_ENABLE},
    {0xff}
};

//tl[0] ccode]
//0

static unsigned int cdhd_603f_0_0 = 0;
static unsigned int cdhd_6040_0_0 = 0;
static unsigned int cdhd_6041_0_0 = 0;
static unsigned int cdhd_6060_0_0 = 0;
static unsigned int cdhd_6061_0_0 = 0;
static unsigned int cdhd_6064_0_0 = 0;
static unsigned int cdhd_606c_0_0 = 0;
static unsigned int cdhd_6071_0_0 = 0;
static unsigned int cdhd_6077_0_0 = 0;
static unsigned int cdhd_6078_0_0 = 0;
static unsigned int cdhd_607a_0_0 = 0;
static unsigned int cdhd_60b1_0_0 = 0;
static unsigned int cdhd_60b2_0_0 = 0;
static unsigned int cdhd_60fd_0_0 = 0;
static unsigned int cdhd_60fe_1_0 = 0;
static unsigned int cdhd_60ff_0_0 = 0;


ec_pdo_entry_info_t slave_0_pdo_entries[] = {
    {0x6040, 0x00, 16},
    {0x6060, 0x00, 8},
    {0x607a, 0x00, 32},
    {0x60ff, 0x00, 32},
    {0x6071, 0x00, 16},
    {0x60fe, 0x01, 32},
    {0x603f, 0x00, 16},
    {0x6041, 0x00, 16},
    {0x6061, 0x00, 8},
    {0x6064, 0x00, 32},
    {0x606c, 0x00, 32},
    {0x6077, 0x00, 16},
    {0x6078, 0x00, 16},
    {0x60fd, 0x00, 32},
};

ec_pdo_info_t slave_0_pdos[] = {
    {0x1600, 2, slave_0_pdo_entries + 0},
    {0x1601, 1, slave_0_pdo_entries + 2},
    {0x1602, 1, slave_0_pdo_entries + 3},
    {0x1603, 2, slave_0_pdo_entries + 4},
    {0x1a00, 3, slave_0_pdo_entries + 6},
    {0x1a01, 1, slave_0_pdo_entries + 9},
    {0x1a02, 1, slave_0_pdo_entries + 10},
    {0x1a03, 3, slave_0_pdo_entries + 11},
};

ec_sync_info_t slave_0_syncs[] = {
    {0, EC_DIR_OUTPUT, 0, NULL, EC_WD_DISABLE},
    {1, EC_DIR_INPUT, 0, NULL, EC_WD_DISABLE},
    {2, EC_DIR_OUTPUT, 4, slave_0_pdos + 0, EC_WD_DISABLE},
    {3, EC_DIR_INPUT, 4, slave_0_pdos + 4, EC_WD_DISABLE},
    {0xff}
};

// process data
#define domain_regs_0 \
{0,0,0x000002e1, 0x00000000,  0x603f, 0, &cdhd_603f_0_0, NULL}, \
{0,0,0x000002e1, 0x00000000,  0x6040, 0, &cdhd_6040_0_0, NULL}, \
{0,0,0x000002e1, 0x00000000,  0x6060, 0, &cdhd_6060_0_0, NULL}, \
{0,0,0x000002e1, 0x00000000,  0x607a, 0, &cdhd_607a_0_0, NULL}, \
{0,0,0x000002e1, 0x00000000,  0x60ff, 0, &cdhd_60ff_0_0, NULL}, \
{0,0,0x000002e1, 0x00000000,  0x6071, 0, &cdhd_6071_0_0, NULL}, \
{0,0,0x000002e1, 0x00000000,  0x60fe, 1, &cdhd_60fe_1_0, NULL}, \
{0,0,0x000002e1, 0x00000000,  0x6041, 0, &cdhd_6041_0_0, NULL}, \
{0,0,0x000002e1, 0x00000000,  0x6061, 0, &cdhd_6061_0_0, NULL}, \
{0,0,0x000002e1, 0x00000000,  0x6064, 0, &cdhd_6064_0_0, NULL}, \
{0,0,0x000002e1, 0x00000000,  0x606c, 0, &cdhd_606c_0_0, NULL}, \
{0,0,0x000002e1, 0x00000000,  0x6077, 0, &cdhd_6077_0_0, NULL}, \
{0,0,0x000002e1, 0x00000000,  0x6078, 0, &cdhd_6078_0_0, NULL}, \
{0,0,0x000002e1, 0x00000000,  0x60fd, 0, &cdhd_60fd_0_0, NULL},
//tl[0] ccode
//1
#define ESLV_ALIAS_POS_1  0, 1//HIO-1111
#define HIO1111_VID_PCODE 0x00000851, 0x08511111//HIO-1111


static uint16_t hio1111_3001_01_1_byte0;//8bits
static uint16_t hio1111_3001_02_1_byte1;//8bits


ec_pdo_entry_info_t slave_1_pdo_entries[] = {
    {0x3001, 0x01, 8}, /* Input */
    {0x3001, 0x02, 8}, /* Input */
};

ec_pdo_info_t slave_1_pdos[] = {
    {0x1600, 1, slave_1_pdo_entries + 0}, /* Byte 0 */
    {0x1601, 1, slave_1_pdo_entries + 1}, /* Byte 1 */
};

ec_sync_info_t slave_1_syncs[] = {
    {0, EC_DIR_INPUT, 2, slave_1_pdos + 0, EC_WD_DISABLE},
    {0xff}
};

// process data
#define domain_regs_1 \
{ESLV_ALIAS_POS_1, HIO1111_VID_PCODE, 0x3001, 0x01, &hio1111_3001_01_1_byte0, NULL},\
{ESLV_ALIAS_POS_1, HIO1111_VID_PCODE, 0x3001, 0x02, &hio1111_3001_02_1_byte1, NULL},


//tl[0] ccode
//2
#define ESLV_ALIAS_POS_2  0, 2//HIO-1111
#define HIO1111_VID_PCODE 0x00000851, 0x08511111//HIO-1111


static uint16_t hio1111_3001_01_2_byte0;//8bits
static uint16_t hio1111_3001_02_2_byte1;//8bits


ec_pdo_entry_info_t slave_2_pdo_entries[] = {
    {0x3001, 0x01, 8}, /* Input */
    {0x3001, 0x02, 8}, /* Input */
};

ec_pdo_info_t slave_2_pdos[] = {
    {0x1600, 1, slave_2_pdo_entries + 0}, /* Byte 0 */
    {0x1601, 1, slave_2_pdo_entries + 1}, /* Byte 1 */
};

ec_sync_info_t slave_2_syncs[] = {
    {0, EC_DIR_INPUT, 2, slave_2_pdos + 0, EC_WD_DISABLE},
    {0xff}
};

// process data
#define domain_regs_2 \
{ESLV_ALIAS_POS_2, HIO1111_VID_PCODE, 0x3001, 0x01, &hio1111_3001_01_2_byte0, NULL},\
{ESLV_ALIAS_POS_2, HIO1111_VID_PCODE, 0x3001, 0x02, &hio1111_3001_02_2_byte1, NULL},


//tl[0] ccode
//3
#define ESLV_ALIAS_POS_3  0, 3//HIO-1121
#define HIO1121_VID_PCODE 0x00000851, 0x08511121//HIO-1121


static uint16_t hio1121_3101_01_3_byte0;//8bits
static uint16_t hio1121_3101_02_3_byte1;//8bits


ec_pdo_entry_info_t slave_3_pdo_entries[] = {
    {0x3101, 0x01, 8}, /* Output */
    {0x3101, 0x02, 8}, /* Output */
};

ec_pdo_info_t slave_3_pdos[] = {
    {0x1a00, 1, slave_3_pdo_entries + 0}, /* Byte 0 */
    {0x1a01, 1, slave_3_pdo_entries + 1}, /* Byte 1 */
};

ec_sync_info_t slave_3_syncs[] = {
    {0, EC_DIR_OUTPUT, 1, slave_3_pdos + 0, EC_WD_ENABLE},
	{1, EC_DIR_OUTPUT, 1, slave_3_pdos + 1, EC_WD_ENABLE},
    {0xff}
};

// process data
#define domain_regs_3 \
{ESLV_ALIAS_POS_3, HIO1121_VID_PCODE, 0x3101, 0x01, &hio1121_3101_01_3_byte0, NULL},\
{ESLV_ALIAS_POS_3, HIO1121_VID_PCODE, 0x3101, 0x02, &hio1121_3101_02_3_byte1, NULL},


//tl[0] ccode
//4
#define ESLV_ALIAS_POS_4  0, 4//HIO-1121
#define HIO1121_VID_PCODE 0x00000851, 0x08511121//HIO-1121


static uint16_t hio1121_3101_01_4_byte0;//8bits
static uint16_t hio1121_3101_02_4_byte1;//8bits


ec_pdo_entry_info_t slave_4_pdo_entries[] = {
    {0x3101, 0x01, 8}, /* Output */
    {0x3101, 0x02, 8}, /* Output */
};

ec_pdo_info_t slave_4_pdos[] = {
    {0x1a00, 1, slave_4_pdo_entries + 0}, /* Byte 0 */
    {0x1a01, 1, slave_4_pdo_entries + 1}, /* Byte 1 */
};

ec_sync_info_t slave_4_syncs[] = {
    {0, EC_DIR_OUTPUT, 1, slave_4_pdos + 0, EC_WD_ENABLE},
	{1, EC_DIR_OUTPUT, 1, slave_4_pdos + 1, EC_WD_ENABLE},
    {0xff}
};

// process data
#define domain_regs_4 \
{ESLV_ALIAS_POS_4, HIO1121_VID_PCODE, 0x3101, 0x01, &hio1121_3101_01_4_byte0, NULL},\
{ESLV_ALIAS_POS_4, HIO1121_VID_PCODE, 0x3101, 0x02, &hio1121_3101_02_4_byte1, NULL},



/****************************************************************************/
ec_master_t *master = NULL;
static ec_master_state_t master_state = {};
static ec_domain_state_t domain0_state = {};
static uint8_t *domain0_pd = NULL;
/****************************************************************************/
// process data
ec_domain_t *domain0 = NULL;
static ec_slave_config_t *cdhd_0_config = NULL;
static ec_slave_config_t *hio1161diginout_1_config = NULL;
static ec_slave_config_t *hio1111digin_1_config = NULL;
static ec_slave_config_t *hio1111digin_2_config = NULL;
static ec_slave_config_t *hio1121digout_3_config = NULL;
static ec_slave_config_t *hio1121digout_4_config = NULL;

// process data
const static ec_pdo_entry_reg_t domain0_regs[] = {
domain_regs_0
domain_regs_1
domain_regs_2
domain_regs_3
domain_regs_4
};
/****************************************************************************/
#define U16_8(s,dh,dl) dl=s&0xFF; dh=s>>8
#define U32_16(s,dh,dl) dl=s&0xFFFF; dh=s>>16
#define U16_32(dh,dl) dh<<16|dl
/*****************************************************************************
 * Realtime task
 ****************************************************************************/
#endif

void __retrieve_0()
{
#ifdef WIN32	
return;
#else
	ecrt_master_receive(master);
	ecrt_domain_process(domain0);
//rcode for CDHD
 
//tl[2] rcode
/* Retrieve PDO */
    ActualModes[0 ] = EC_READ_U8(domain0_pd + cdhd_6061_0_0);
	StatusWords[0 ] = EC_READ_U16(domain0_pd + cdhd_6041_0_0);
	U32_16(EC_READ_U32(domain0_pd + cdhd_6064_0_0),Position_Actual_High[0],Position_Actual_Low[0]);
    ActualTorques[0 ] = EC_READ_U16(domain0_pd + cdhd_6077_0_0);
    ActualCurrents[0 ] = EC_READ_U16(domain0_pd + cdhd_6078_0_0);
    U32_16(EC_READ_U32(domain0_pd + cdhd_60fd_0_0),HIO_Input_Bools[0],HIO_Input_Bools[0+1]);
    
    HIO_Input_Bools[0+2]   = EC_READ_U16(domain0_pd + cdhd_603f_0_0);
    

//rcode for  HIO1111DIGIN
 
//tl[2] rcode
/* Retrieve PDO */

#define U8_16(dh,dl) dh<<8|dl

HIO_Input_Bools[3+0] = U8_16(EC_READ_U8(domain0_pd + hio1111_3001_02_1_byte1), EC_READ_U8(domain0_pd + hio1111_3001_01_1_byte0));


//rcode for  HIO1111DIGIN
 
//tl[2] rcode
/* Retrieve PDO */

#define U8_16(dh,dl) dh<<8|dl

HIO_Input_Bools[4+0] = U8_16(EC_READ_U8(domain0_pd + hio1111_3001_02_2_byte1), EC_READ_U8(domain0_pd + hio1111_3001_01_2_byte0));


//rcode for  HIO1121DIGOUT
 
//tl[2] rcode
/* Retrieve PDO */



//rcode for  HIO1121DIGOUT
 
//tl[2] rcode
/* Retrieve PDO */




#endif
}
void __publish_0()
{
#ifdef WIN32	
return;
#else
//tcode for CDHD
 
//tl[3] pcode
/* TPDO */
    EC_WRITE_U8(domain0_pd + cdhd_6060_0_0,TargetModes[0]);
#if 0
    //if ((StatusWords[0] & 0x006f) == 0x0028)
    if ((StatusWords[0] & 0x0008) == 0x0008)
        EC_WRITE_U16(domain0_pd + cdhd_6040_0_0,128);//clear the fault in slave and change to SWITCH ON DISABLE state 
    else
        EC_WRITE_U16(domain0_pd + cdhd_6040_0_0,ControlWords[0]);
#endif
    EC_WRITE_U16(domain0_pd + cdhd_6040_0_0,ControlWords[0]);
      
	EC_WRITE_U32(domain0_pd + cdhd_607a_0_0,U16_32(Interpolation_data_records_high[0],Interpolation_data_records_low[0]));
    EC_WRITE_U16(domain0_pd + cdhd_6071_0_0, TargetTorques[0]);
    
    EC_WRITE_U32(domain0_pd + cdhd_60fe_1_0, U16_32(HIO_Output_Bools[0],HIO_Output_Bools[0+1]));
    
    

//tcode for  HIO1111DIGIN
 
//tl[3] pcode



//tcode for  HIO1111DIGIN
 
//tl[3] pcode



//tcode for  HIO1121DIGOUT
 
//tl[3] pcode

EC_WRITE_U8(domain0_pd + hio1121_3101_01_3_byte0, HIO_Output_Bools[2+0]&0xFF);
EC_WRITE_U8(domain0_pd + hio1121_3101_02_3_byte1, HIO_Output_Bools[2+0]>>8);



//tcode for  HIO1121DIGOUT
 
//tl[3] pcode

EC_WRITE_U8(domain0_pd + hio1121_3101_01_4_byte0, HIO_Output_Bools[3+0]&0xFF);
EC_WRITE_U8(domain0_pd + hio1121_3101_02_4_byte1, HIO_Output_Bools[3+0]>>8);




#ifdef DC
	    ecrt_master_application_time(master, rt_timer_read());
	    ecrt_master_sync_reference_clock(master);
	    ecrt_master_sync_slave_clocks(master);	
#endif
	ecrt_domain_queue(domain0);
	ecrt_master_send(master);
#endif
}
extern unsigned long long common_ticktime__;
int __init_0()
{
#ifdef WIN32	
return 0;
#else
    master = ecrt_request_master(0);

    if (!master) {
        return -1;
    }

    domain0 = ecrt_master_create_domain(master);
    if (!domain0) {        return -1;    }

//icode for CDHD
 
//tl[1] icode
	cdhd_0_config = ecrt_master_slave_config(master, 0,0, 0x000002e1, 0x00000000);
    if (!cdhd_0_config) {fprintf(stderr, "Failed to get slave configuration.\n");return -1;}
    if (ecrt_slave_config_pdos(cdhd_0_config, EC_END, slave_0_syncs)) {fprintf(stderr, "Failed to configure PDOs.\n");return -1;}	
#ifdef DC
   ecrt_slave_config_dc(cdhd_0_config, 0x300, common_ticktime__, common_ticktime__/2, 0, 0);
#endif
	ecrt_slave_config_sdo8(cdhd_0_config,0x6060,0,8);
	
	ecrt_slave_config_sdo8(cdhd_0_config,0x60c2,1,common_ticktime__/1000000);
	ecrt_slave_config_sdo8(cdhd_0_config,0x60c2,2,0xfd);
	

//icode for HIO1111DIGIN
 
//tl[1] icode
    hio1111digin_1_config = ecrt_master_slave_config(master, ESLV_ALIAS_POS_1, HIO1111_VID_PCODE);
    if (!hio1111digin_1_config) {
		fprintf(stderr, "[HZICP] Failed to get slave configuration.\n");
		return -1;
	}
    if (ecrt_slave_config_pdos(hio1111digin_1_config, EC_END, slave_1_syncs)) {
		fprintf(stderr, "[HZICP] Failed to configure PDOs.\n");
		return -1;
	}	




//icode for HIO1111DIGIN
 
//tl[1] icode
    hio1111digin_2_config = ecrt_master_slave_config(master, ESLV_ALIAS_POS_2, HIO1111_VID_PCODE);
    if (!hio1111digin_2_config) {
		fprintf(stderr, "[HZICP] Failed to get slave configuration.\n");
		return -1;
	}
    if (ecrt_slave_config_pdos(hio1111digin_2_config, EC_END, slave_2_syncs)) {
		fprintf(stderr, "[HZICP] Failed to configure PDOs.\n");
		return -1;
	}	




//icode for HIO1121DIGOUT
 
//tl[1] icode
    hio1121digout_3_config = ecrt_master_slave_config(master, ESLV_ALIAS_POS_3, HIO1121_VID_PCODE);
    if (!hio1121digout_3_config) {
		fprintf(stderr, "[HZICP] Failed to get slave configuration.\n");
		return -1;
	}
    if (ecrt_slave_config_pdos(hio1121digout_3_config, EC_END, slave_3_syncs)) {
		fprintf(stderr, "[HZICP] Failed to configure PDOs.\n");
		return -1;
	}	




//icode for HIO1121DIGOUT
 
//tl[1] icode
    hio1121digout_4_config = ecrt_master_slave_config(master, ESLV_ALIAS_POS_4, HIO1121_VID_PCODE);
    if (!hio1121digout_4_config) {
		fprintf(stderr, "[HZICP] Failed to get slave configuration.\n");
		return -1;
	}
    if (ecrt_slave_config_pdos(hio1121digout_4_config, EC_END, slave_4_syncs)) {
		fprintf(stderr, "[HZICP] Failed to configure PDOs.\n");
		return -1;
	}	




ecrt_domain_reg_pdo_entry_list(domain0, domain0_regs);


    //printf("Activating master...\n");
    if (ecrt_master_activate(master)) {
        return -1;
    }
    if (!(domain0_pd = ecrt_domain_data(domain0))) {
        fprintf(stderr, "Failed to get domain data pointer.\n");
        return -1;
    }
	return 0;
#endif
}

void __cleanup_0()
{
#ifdef WIN32	
return;
#else
    printf("End of Program\n");
    ecrt_release_master(master);
#endif
}
