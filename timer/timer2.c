#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "sll.h"
#include <errno.h>
#include <modbus/modbus.h>

#include "unit-test.h"
#include <curl/curl.h>
#define LOCAL_FILE      "/tmp/uploadthis.txt"
#define UPLOAD_FILE_AS  "while-uploading.txt"
#define REMOTE_URL      "ftp://austin:weitao@192.168.148.137:990/"  UPLOAD_FILE_AS
#define RENAME_FILE_TO  "SBS_Meter_PM7500.txt"
enum {
    TCP,
    TCP_PI,
    RTU
};


#define CLOCKID CLOCK_REALTIME

//#define USER_SAMPLE_INTERVAL (5 * SECSPERMIN)
//#define USER_UPLOAD_INTERVAL (30 * SECSPERMIN)
#define USER_SAMPLE_INTERVAL (10)
#define USER_UPLOAD_INTERVAL (40)
#define SECSPERHOUR 3600
#define SECSPERMIN 60	
#define INTERVAL_CMEP "00000005"

typedef struct{
	int addr;  	//meter attribute register start address
	unsigned char reg_num;	//meter attribute register numbers
	int scale;	// attribute value scale
	char *value_type;  //type of the value: float,int,long
	char *value_unit;  //type of the value: float,int,long
}Meter_Attribute;

typedef struct{
	unsigned char modbus_id;	//modbus id for a specific meter 
	unsigned char num_attribute;	//num of atrributes a meter provides, MAX:255	
	Meter_Attribute *attribute;	//pointer to a specific meter's attriubtes
	char *file_name;	//file path for sampling data file
	char *file_tmp_name;
	//int fd;		//file descriptor for sampling data file
	FILE * file;		//file descriptor for sampling data file
	FILE * file_tmp;		//file descriptor for sampling data file
	char *commodity;    //type of the meter, such as electricity,gas,water,etc.
}Meter;

static void freeData(void **data)
{

    (void) fprintf(stderr,"in free data function.\n");
    
    Meter
        **addr=(Meter **) data;

    static int
        n=0;

    n++;
    if (*addr)
    {
	(void) fprintf(stderr,"Freeing Meter Attributes First.\n");
    int i;
	for(i = 0; i< (*addr)->num_attribute; i++){
		if( (*addr)->attribute)
		{
			(void)fprintf(stderr,"Freeing value_type and value unit.\n");
			if((*addr)->attribute->value_type)
			{
				(void)free((char *)((*addr)->attribute->value_type));
			}
			if((*addr)->attribute->value_unit)
			{
				(void)free((char *)((*addr)->attribute->value_unit));
			}
			(void)fprintf(stderr,"Freeing attribute itself.\n");
			(void)free((Meter_Attribute *)(*addr)->attribute);
		}
	}
		(void) fprintf(stderr,"Freeing Meter.\n");
        (void) free((Meter *) (*addr));
        (*addr)=NULL;
    }
	printf("\n");
}
Sll *head = NULL;
void meter_init(void)
{
    Sll
        *l,
        *new=NULL;


    Meter
        *addr;
    int
        n=0;

    (void) fprintf(stderr,
"=========================================================================\n");
    (void) fprintf(stderr," Testing Append a node at the beginning of a list\n");
    (void) fprintf(stderr,
"=========================================================================\n");

    addr=(Meter*) malloc(sizeof(Meter));

    if (addr == NULL) {
        (void) fprintf(stderr," malloc failed\n");
    	destroyNodes(&head,freeData);
        exit(-1);
    }

    (void) fprintf(stderr,"\n---------------[ appending ]----------\n");
    new=allocateNode((void *) addr);
	
    appendNode(&head,&new);

    /*
    ** it will be the last node
    */
    addr->modbus_id = 2;
    addr->num_attribute = 7;

	addr->commodity = strdup("E");   //the meter is a Electricity type
	if(addr->commodity == NULL)
		(void *)fprintf(stderr,"failed to allocate meter commodity.\n");
    addr->attribute = (Meter_Attribute *) malloc(addr->num_attribute * sizeof(Meter_Attribute));
    if(addr->attribute == NULL)
    {
		(void)fprintf(stderr," Attribute:malloc failed\n");
    	destroyNodes(&head,freeData);
		exit(-1);
    }
   
    (void) fprintf(stderr,
"=========================================================================\n");
    (void) fprintf(stderr," initialzing meter attributes\n");
    (void) fprintf(stderr,
"=========================================================================\n");

#if 0
    int i;
    for(i = 0; i < addr->num_attribute;i++)
    {
		addr->attribute->addr = 999;
		addr->attribute->reg_num = 2;
		addr->attribute->scale = 1;
		addr->attribute->value_type = strdup("float");
		addr->attribute->value_unit = strdup("KWh");
    	if (addr->attribute->value_type == NULL || addr->attribute->value_unit == NULL) 
    	{
       		(void) fprintf(stderr,"malloc failed\n");
       		exit(-1);
    	}
    }

#endif

    Meter_Attribute *tmp = addr->attribute;
    tmp->addr = 999;
    tmp->reg_num = 2;
    tmp->scale = 1;
    tmp->value_type = strdup("float");
    tmp->value_unit = strdup("KWh");
    if (tmp->value_type == NULL || tmp->value_unit == NULL) 
    {
   	(void) fprintf(stderr,"malloc failed\n");
        exit(-1);
    }
 
    tmp = tmp + 1;
    tmp->addr = 1001;
    tmp->reg_num = 2;
    tmp->scale = 1;
    tmp->value_type = strdup("float");
    tmp->value_unit = strdup("KVAh");
    if (tmp->value_type == NULL || tmp->value_unit == NULL) 
    {
   	(void) fprintf(stderr,"malloc failed\n");
        exit(-1);
    }
   
    tmp = tmp + 1;
    tmp->addr = 1003;
    tmp->reg_num = 2;
    tmp->scale = 1;
    tmp->value_type = strdup("float");
    tmp->value_unit = strdup("KVarh");
    if (tmp->value_type == NULL || tmp->value_unit == NULL) 
    {
   	(void) fprintf(stderr,"malloc failed\n");
        exit(-1);
    }

    tmp = tmp + 1;
    tmp->addr = 1005;
    tmp->reg_num = 2;
    tmp->scale = 1;
    tmp->value_type = strdup("float");
    tmp->value_unit = strdup("KW");
    if (tmp->value_type == NULL || tmp->value_unit == NULL) 
    {
   	(void) fprintf(stderr,"malloc failed\n");
        exit(-1);
    }

    tmp = tmp + 1;
    tmp->addr = 1007;
    tmp->reg_num = 2;
    tmp->scale = 1;
    tmp->value_type = strdup("float");
    tmp->value_unit = strdup("KVA");
    if (tmp->value_type == NULL || tmp->value_unit == NULL) 
    {
   	(void) fprintf(stderr,"malloc failed\n");
        exit(-1);
    }
    tmp = tmp + 1;
    tmp->addr = 1009;
    tmp->reg_num = 2;
    tmp->scale = 1;
    tmp->value_type = strdup("float");
    tmp->value_unit = strdup("KVar");
    if (tmp->value_type == NULL || tmp->value_unit == NULL) 
    {
   	(void) fprintf(stderr,"malloc failed\n");
        exit(-1);
    }
    tmp = tmp + 1;
    tmp->addr = 1013;
    tmp->reg_num = 2;
    tmp->scale = 1;
    tmp->value_type = strdup("float");
    tmp->value_unit = strdup("Volt");
    if (tmp->value_type == NULL || tmp->value_unit == NULL) 
    {
   	(void) fprintf(stderr,"malloc failed\n");
        exit(-1);
    }
    Meter_Attribute *attribute = addr->attribute;
    int i;
    for(i = 0; i < addr->num_attribute && attribute; i++,attribute++){
    	(void) fprintf(stderr,"  %d\n",attribute->addr);
    	(void) fprintf(stderr,"  %d\n",attribute->reg_num);
    	(void) fprintf(stderr,"  %d\n",attribute->scale);
    	(void) fprintf(stderr,"  %s\n",attribute->value_type);
    	(void) fprintf(stderr,"  %s\n",attribute->value_unit);
    	(void) fprintf(stderr,"\n");
    }

#if 0    
    (void) fprintf(stderr,"Node: %d\n", ++n);
    (void) fprintf(stderr,"  %d\n",addr->modbus_id);
    (void) fprintf(stderr,"  %d\n",addr->num_attribute);
    (void) fprintf(stderr,"  %d\n",addr->attribute->addr);
    (void) fprintf(stderr,"  %d\n",addr->attribute->reg_num);
    (void) fprintf(stderr,"  %d\n",addr->attribute->scale);
    (void) fprintf(stderr,"  %s\n",addr->attribute->value_type);
    (void) fprintf(stderr,"  %s\n",addr->attribute->value_unit);
#endif

    /*
    ** print
    */

#if 0
    (void) fprintf(stderr,"\n---------------[ printing ]----------\n");
    n=0;
    for (l=head; l; l=l->next)
    {
        addr=(Meter*) l->data;
    	(void) fprintf(stderr,"Node: %d\n", ++n);
    	(void) fprintf(stderr,"  %d\n",addr->modbus_id);
    	(void) fprintf(stderr,"  %d\n",addr->num_attribute);
    	(void) fprintf(stderr,"  %d\n",addr->attribute->addr);
    	(void) fprintf(stderr,"  %d\n",addr->attribute->reg_num);
    	(void) fprintf(stderr,"  %d\n",addr->attribute->scale);
    	(void) fprintf(stderr,"  %s\n",addr->attribute->value_type);
    	(void) fprintf(stderr,"  %s\n",addr->attribute->value_unit);
    }
    /*
    ** free nodes
    */
    (void) fprintf(stderr,"\n---------------[ freeing ]----------\n");
    destroyNodes(&head,freeData);
#endif
#if 0
    (void) fprintf(stderr,
"=========================================================================\n");
    (void) fprintf(stderr," Testing Append a node at the beginning of a list\n");
    (void) fprintf(stderr,
"=========================================================================\n");

    addr=(Meter*) malloc(sizeof(Meter));

    if (addr == NULL) {
        (void) fprintf(stderr," malloc failed\n");
    	destroyNodes(&head,freeData);
        exit(-1);
    }

    (void) fprintf(stderr,"\n---------------[ appending ]----------\n");
    new=allocateNode((void *) addr);
	
    appendNode(&head,&new);

    /*
    ** it will be the last node
    */
    addr->modbus_id = 2;
    addr->num_attribute = 3;
    addr->attribute = (Meter_Attribute *) malloc(addr->num_attribute * sizeof(Meter_Attribute));
    if(addr->attribute == NULL)
    {
		(void)fprintf(stderr," Attribute:malloc failed\n");
    	destroyNodes(&head,freeData);
		exit(-1);
    }
   
    (void) fprintf(stderr,
"=========================================================================\n");
    (void) fprintf(stderr," initialzing meter attributes\n");
    (void) fprintf(stderr,
"=========================================================================\n");
#endif 

#if 0
    int i;
    for(i = 0; i < addr->num_attribute;i++)
    {
		addr->attribute->addr = 999;
		addr->attribute->reg_num = 2;
		addr->attribute->scale = 1;
		addr->attribute->value_type = strdup("float");
		addr->attribute->value_unit = strdup("KWh");
    	if (addr->attribute->value_type == NULL || addr->attribute->value_unit == NULL) 
    	{
       		(void) fprintf(stderr,"malloc failed\n");
       		exit(-1);
    	}
    }

#endif

#if 0
    tmp = addr->attribute;
    tmp->addr = 9990;
    tmp->reg_num = 2;
    tmp->scale = 1;
    tmp->value_type = strdup("float");
    tmp->value_unit = strdup("KWh");
    if (tmp->value_type == NULL || tmp->value_unit == NULL) 
    {
   	(void) fprintf(stderr,"malloc failed\n");
        exit(-1);
    }
 
    tmp = tmp + 1;
    tmp->addr = 10010;
    tmp->reg_num = 2;
    tmp->scale = 1;
    tmp->value_type = strdup("float");
    tmp->value_unit = strdup("KVAh");
    if (tmp->value_type == NULL || tmp->value_unit == NULL) 
    {
   	(void) fprintf(stderr,"malloc failed\n");
        exit(-1);
    }
   
    tmp = tmp + 1;
    tmp->addr = 10030;
    tmp->reg_num = 2;
    tmp->scale = 1;
    tmp->value_type = strdup("float");
    tmp->value_unit = strdup("KVarh");
    if (tmp->value_type == NULL || tmp->value_unit == NULL) 
    {
   	(void) fprintf(stderr,"malloc failed\n");
        exit(-1);
    }

    tmp = tmp + 1;
    tmp->addr = 10050;
    tmp->reg_num = 2;
    tmp->scale = 1;
    tmp->value_type = strdup("float");
    tmp->value_unit = strdup("KW");
    if (tmp->value_type == NULL || tmp->value_unit == NULL) 
    {
   	(void) fprintf(stderr,"malloc failed\n");
        exit(-1);
    }
    tmp = tmp + 1;
    tmp->addr = 1007;
    tmp->reg_num = 2;
    tmp->scale = 1;
    tmp->value_type = strdup("float");
    tmp->value_unit = strdup("KVA");
    if (tmp->value_type == NULL || tmp->value_unit == NULL) 
    {
   	(void) fprintf(stderr,"malloc failed\n");
        exit(-1);
    }
    tmp = tmp + 1;
    tmp->addr = 1009;
    tmp->reg_num = 2;
    tmp->scale = 1;
    tmp->value_type = strdup("float");
    tmp->value_unit = strdup("KVar");
    if (tmp->value_type == NULL || tmp->value_unit == NULL) 
    {
   	(void) fprintf(stderr,"malloc failed\n");
        exit(-1);
    }
    tmp = tmp + 1;
    tmp->addr = 1013;
    tmp->reg_num = 2;
    tmp->scale = 1;
    tmp->value_type = strdup("float");
    tmp->value_unit = strdup("Volt");
    if (tmp->value_type == NULL || tmp->value_unit == NULL) 
    {
   	(void) fprintf(stderr,"malloc failed\n");
        exit(-1);
    }
    attribute = addr->attribute;
    for(i = 0; i < addr->num_attribute && attribute; i++,attribute++){
    	(void) fprintf(stderr,"  %d\n",attribute->addr);
    	(void) fprintf(stderr,"  %d\n",attribute->reg_num);
    	(void) fprintf(stderr,"  %d\n",attribute->scale);
    	(void) fprintf(stderr,"  %s\n",attribute->value_type);
    	(void) fprintf(stderr,"  %s\n",attribute->value_unit);
    	(void) fprintf(stderr,"\n");
    }

#endif

}


//unit:senconds
typedef struct {
	unsigned int sample_interval;
	unsigned int upload_interval;
}Interval;

Interval interval;

unsigned int get_sample_interval()
{
	
	return USER_SAMPLE_INTERVAL;
}

unsigned int get_upload_interval()
{
	return USER_UPLOAD_INTERVAL;
}

#define SECSPERHOUR 3600
#define SECSPERMIN 60	

void second_trans(unsigned int seconds,char *time)
{
	unsigned int day,hour,min;
	day = seconds / (60 * 60 * 24);
	hour = seconds / (60 * 60) % 24;
	min = seconds / 60 % 60;

	sprintf(time,"%02d%02d%02d%02d",0,day,hour,min);
	
}

void interval_init(Interval *interval)
{
	//before the create the interval timer, we need to initialize the interval first
	printf("about to initialize sample interval and upload interval\n");
	interval->sample_interval = get_sample_interval();
	interval->upload_interval = get_upload_interval();

	printf("sample interval:%d\n",interval->sample_interval);
	printf("upload interval:%d\n\n",interval->upload_interval);

}

void itimer_init(struct tm*info, struct itimerspec * it_spec,unsigned int it_interval)
{

	
	int secs_left = 0;	
	//sample interval less than 1 hour, unit in minutes
	if(it_interval < SECSPERHOUR){
		
		if((info->tm_min * SECSPERMIN + info->tm_sec) % it_interval == 0)
			secs_left = 0;
		else
			secs_left = it_interval - (info->tm_min * SECSPERMIN + info->tm_sec) % it_interval;
	}
	//sample interval equal or greater than 1 hour, unit in hours
	else{
		 secs_left = SECSPERHOUR - (info->tm_min * SECSPERMIN + info->tm_sec);
	}
		
	if(secs_left == 0)
	{

		it_spec->it_interval.tv_sec = it_interval;
		it_spec->it_interval.tv_nsec = 0;
		it_spec->it_value.tv_sec = 0;
		it_spec->it_value.tv_nsec = 1;  //a workaround to start the thread immediately
	}
	else	
	{
		it_spec->it_interval.tv_sec = it_interval;
		it_spec->it_interval.tv_nsec = 0;
		it_spec->it_value.tv_sec = secs_left;
		it_spec->it_value.tv_nsec = 0;  

	}
	
	printf("interval timer interval:%d\n",it_spec->it_interval.tv_sec);
	printf("interval timer secs left:%d\n",it_spec->it_value.tv_sec);

}
/* NOTE: if you want this example to work on Windows with libcurl as a
   DLL, you MUST also provide a read callback with CURLOPT_READFUNCTION.
   Failing to do so will give you a crash since a DLL may not use the
   variable's memory when passed in to it from an app like this. */
static size_t read_callback(void *ptr, size_t size, size_t nmemb, void *stream)
{
  curl_off_t nread;
  /* in real-world cases, this would probably get this data differently
     as this fread() stuff is exactly what the library already would do
     by default internally */
  size_t retcode = fread(ptr, size, nmemb, stream);

  nread = (curl_off_t)retcode;

  fprintf(stderr, "*** We read %" CURL_FORMAT_CURL_OFF_T
          " bytes from file\n", nread);
  return retcode;
}

void timer_thread_sample(union sigval v)
{
	static unsigned int counter = 0;
	counter++;

	time_t rawtime;
    	struct tm *info, *info_local;
	printf("******sample timer expired******.\n");
    time(&rawtime);
    /* Get GMT time */
    info = gmtime(&rawtime );
    info_local = localtime(&rawtime);
    printf("UTC: %2d:%02d:%02d\n", info->tm_hour, info->tm_min,info->tm_sec);
    printf("LOCAL: %2d:%02d:%02d\n", info_local->tm_hour, info_local->tm_min,info_local->tm_sec);

//*****************************sampling data**********************//
	Sll *l;
	Meter *meter;

    int n=0;
	char file_name[32];
	char file_name_tmp[32];
    for (l=head; l; l=l->next)
    {

        meter = (Meter*) l->data;

//*********************for every meter,open the corresponding file for saving sampling data first//
		if(counter == 1){
			sprintf(file_name,"/tmp/sbs%d_%4d%02d%02d%02d%02d%02d_001",meter->modbus_id,(info->tm_year + 1900),(info->tm_mon + 1),info->tm_mday,info->tm_hour,info->tm_min,info->tm_sec);
			sprintf(file_name_tmp,"/tmp/sbs%d_%4d%02d%02d%02d%02d%02d_001_tmp",meter->modbus_id,(info->tm_year + 1900),(info->tm_mon + 1),info->tm_mday,info->tm_hour,info->tm_min,info->tm_sec);

			meter->file_name = strdup(file_name);
			meter->file_tmp_name = strdup(file_name_tmp);
    		if (meter->file_name == NULL) 
    		{
       			(void) fprintf(stderr,"malloc failed\n");
       			exit(-1);
    		}
    		if (meter->file_tmp_name == NULL) 
    		{
       			(void) fprintf(stderr,"malloc failed\n");
       			exit(-1);
    		}
			(void *)fprintf(stderr,"the file path is %s.\n",meter->file_name);
			(void *)fprintf(stderr,"the tmp file path is %s.\n",meter->file_tmp_name);
		}
			(void *)fprintf(stderr,"opening file %s.\n",meter->file_tmp_name);

			//meter->fd = open(meter->file_name,O_WRONLY | O_CREAT);			
//			meter->file = fopen(meter->file_name,"a");			
			meter->file = fopen(meter->file_tmp_name,"a");			
			if(meter->file == NULL)
				perror("fopen failed:");
//			(void *)fprintf(stderr,"closing file %s.\n",meter->file_name);
//			fclose(meter->file);
	





//**********************************************************************************************//
    	uint8_t *tab_rp_bits;
    	uint16_t *tab_rp_registers;
    	uint16_t *tab_rp_registers_bad;
    	modbus_t *ctx;
    	int i;
    	uint8_t value;
    	int nb_points;
    	int rc;
    	float real;
    	uint32_t ireal;
    	struct timeval old_response_timeout;
    	struct timeval response_timeout;
    	int use_backend;
    	uint16_t tmp_value;
    	float float_value;
		char interval_string[16];

    	ctx = modbus_new_rtu("/dev/ttyUSB0", 19200, 'N', 8, 1);
    	//ctx = modbus_new_rtu("/dev/ttyS0", 19200, 'N', 8, 1);
    	if (ctx == NULL) {
       	 	fprintf(stderr, "Unable to allocate libmodbus context\n");
        	return -1;
    	}
    	modbus_set_debug(ctx, TRUE);
    	modbus_set_error_recovery(ctx,
                              	MODBUS_ERROR_RECOVERY_LINK |
                              	MODBUS_ERROR_RECOVERY_PROTOCOL);
    	modbus_set_slave(ctx, meter->modbus_id);

    	if (modbus_connect(ctx) == -1) {
        	fprintf(stderr, "Connection failed: %s\n", modbus_strerror(errno));
        	modbus_free(ctx);
        	return -1;
    	}


    	/* Allocate and initialize the memory to store the registers */
    	nb_points = 16; //max register number
    	tab_rp_registers = (uint16_t *) malloc(nb_points * sizeof(uint16_t));
    	memset(tab_rp_registers, 0, nb_points * sizeof(uint16_t));

		Meter_Attribute *attribute = meter->attribute;
		printf("num_attribute is %d.\n",meter->num_attribute);
		for(i = 0; i < meter->num_attribute && attribute; i++,attribute++){
    		/** HOLDING REGISTERS **/

    		/* Single register */
		printf("reading register.\n");
    		rc = modbus_read_registers(ctx, attribute->addr,
                               attribute->reg_num, tab_rp_registers);
    		if (rc == attribute->reg_num) {
				second_trans(get_sample_interval(),interval_string);
				//fprintf(meter->file,"%s%s%s,%s,%s,%s,%d,%s,%d,%4d%02d%02d%02d%2d","MEPMD01,19970819,Schneider Electric,,,","SECN\\Schneider Electric China|SBMV\\Beijing Middle Voltage Plant","201308010358,SBMV.MCSET.FHU_HVAC_Lighting1|129","OK",meter->commodity,attribute->value_unit,attribute->scale,interval_string,1,(info->tm_year + 1900),info->tm_mon,(info->tm_mday + 1),info->tm_hour,info->tm_min);
				if(counter == 1)
					fprintf(meter->file,"%s%s%s,%s,%s,%s,%d,%s,%d,%4d%02d%02d%02d%02d,,%f#","MEPMD01,19970819,Schneider Electric,,,","SECN\\Schneider Electric China|SBMV\\Beijing Middle Voltage Plant","201308010358,SBMV.MCSET.FHU_HVAC_Lighting1|129","OK",meter->commodity,attribute->value_unit,attribute->scale,interval_string,get_upload_interval() / get_sample_interval(),(info->tm_year + 1900),(info->tm_mon + 1),info->tm_mday,info->tm_hour,info->tm_min,modbus_get_float_cdab(tab_rp_registers));
				//	fprintf(meter->file,"%s,%s,%s,%s,%d,%s,%d,%4d%02d%02d%02d%02d%02d,,%f ","OK",meter->commodity,attribute->value_unit,attribute->scale,interval_string,get_upload_interval() / get_sample_interval(), (info->tm_year + 1900),(info->tm_mon+1),info->tm_mday,info->tm_hour,info->tm_min,info->tm_sec,modbus_get_float_cdab(tab_rp_registers));
				else{	
						if( i == meter->num_attribute -1)	
							fprintf(meter->file,",,,%f",modbus_get_float_cdab(tab_rp_registers));
						else
							fprintf(meter->file,",,,%f#",modbus_get_float_cdab(tab_rp_registers));
				}
		
				fflush(meter->file);
    		}
			else
        			printf("FAILED (nb points %d)\n", rc);

		}
			fprintf(meter->file,"\n");
			(void *)fprintf(stderr,"closing file %s.\n",meter->file_tmp_name);
			fclose(meter->file);

close:
    	/* Free the memory */

    	printf("closing the modbus\n");
    	free(tab_rp_registers);

    	/* Close the connection */
    	modbus_close(ctx);
    	modbus_free(ctx);
   


//******************************end of sampling data******************//


		printf("counter is %d.\n",counter);
		if(counter ==  get_upload_interval() / get_sample_interval() )
		{
			char system_arguments[128];

			//sprintf(system_arguments,"awk '{for(i=1;i<=NF;i++){a[FNR,i]=$i}}END{for(i=1;i<=NF;i++){for(j=1;j<=FNR;j++){printf a[j,i]\" \"}print \"\"}}' %s | sed s/[[:space:]]//g > %s_new",meter->file_name,meter->file_name);
			sprintf(system_arguments,"awk 'BEGIN{FS=\"#\"}{for(i=1;i<=NF;i++){a[FNR,i]=$i}}END{for(i=1;i<=NF;i++){for(j=1;j<=FNR;j++){printf a[j,i]\"#\"}print \"\"}}' %s | sed s/#//g > %s",meter->file_tmp_name,meter->file_name);
			if( system(system_arguments) != 0) 
				(void *)fprintf(stderr,"system call error.\n"); 

		}
	}
		if(counter ==  get_upload_interval() / get_sample_interval() )
		{
    		for (l=head; l; l=l->next)
    		{

        	meter = (Meter*) l->data;

		  	printf("######upload time######.\n");
			counter = 0;
  			CURL *curl;
  			CURLcode res;
  			FILE *hd_src;
			  struct stat file_info;
			  curl_off_t fsize;
			  static const char *pCACertFile="cwt.pem";
			
			  struct curl_slist *headerlist=NULL;
			  static const char buf_1 [] = "RNFR " UPLOAD_FILE_AS;
			  static char buf_2 [64];
			  sprintf(buf_2,"RNTO %s",meter->file_name);
			
			  /* get the file size of the local file */
			  if(stat(meter->file_name, &file_info)) {
			    printf("Couldnt open '%s': %s\n", LOCAL_FILE, strerror(errno));
			    return 1;
			  }
			  fsize = (curl_off_t)file_info.st_size;
			
			  printf("Local file size: %" CURL_FORMAT_CURL_OFF_T " bytes.\n", fsize);
			
			  /* get a FILE * of the same file */
			  hd_src = fopen(meter->file_name, "rb");
			
			  /* In windows, this will init the winsock stuff */
			  curl_global_init(CURL_GLOBAL_ALL);
			
			  /* get a curl handle */
			  curl = curl_easy_init();
			  if(curl) {
			    /* build a list of commands to pass to libcurl */
			    headerlist = curl_slist_append(headerlist, buf_1);
			    headerlist = curl_slist_append(headerlist, buf_2);
			
			    /* We activate SSL and we require it for both control and data */
			    curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_ALL);
			
			    /* Switch on full protocol/debug output */
			    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
			
			    curl_easy_setopt (curl, CURLOPT_SSL_VERIFYPEER, 0L); 
			    curl_easy_setopt (curl, CURLOPT_SSL_VERIFYHOST, 0L); 
			
			    /* we want to use our own read function */
			    curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_callback);
			
			    /* enable uploading */
			    curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);

    /* specify target */
		    curl_easy_setopt(curl,CURLOPT_URL, REMOTE_URL);
		
		    /* pass in that last of FTP commands to run after the transfer */
		    curl_easy_setopt(curl, CURLOPT_POSTQUOTE, headerlist);
		
		    /* now specify which file to upload */
		    curl_easy_setopt(curl, CURLOPT_READDATA, hd_src);
		
		    /* Set the size of the file to upload (optional).  If you give a *_LARGE
		       option you MUST make sure that the type of the passed-in argument is a
		       curl_off_t. If you use CURLOPT_INFILESIZE (without _LARGE) you must
		       make sure that to pass in a type 'long' argument. */
		    curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE,
		                     (curl_off_t)fsize);
		
		    /* Now run off and do what you've been told! */
		    res = curl_easy_perform(curl);
		    /* Check for errors */
		    if(res != CURLE_OK)
		      fprintf(stderr, "curl_easy_perform() failed: %s\n",
		              curl_easy_strerror(res));
		
		    /* clean up the FTP commands list */
		    curl_slist_free_all (headerlist);
		
		    /* always cleanup */
		    curl_easy_cleanup(curl);
		  }
		  fclose(hd_src); /* close the local file */
		
		  curl_global_cleanup();
		}
	}
}
void timer_thread_upload(union sigval v)
{
    	time_t rawtime;
    	struct tm *info;
	printf("######upload timer expired######.\n");
    	time(&rawtime);
    	/* Get GMT time */
    	info = gmtime(&rawtime );
    	printf("UTC: %2d:%02d:%02d\n", info->tm_hour, info->tm_min,info->tm_sec);
}
int main()
{
	char interval_string[16];
	second_trans(get_sample_interval(),interval_string);
    (void) fprintf(stderr,"sample interval string is %s\n",interval_string);
	
	Sll *l;
	Meter *addr;

	meter_init();
    (void) fprintf(stderr,"\n---------------[ printing ]----------\n");
    int n=0;
    for (l=head; l; l=l->next)
    {
        addr=(Meter*) l->data;
    	(void) fprintf(stderr,"Node: %d\n", ++n);
    	(void) fprintf(stderr,"  %d\n",addr->modbus_id);

    	Meter_Attribute *attribute = addr->attribute;
    	int i;
    	for(i = 0; i < addr->num_attribute && attribute; i++,attribute++){
    		(void) fprintf(stderr,"  %d\n",attribute->addr);
    		(void) fprintf(stderr,"  %d\n",attribute->reg_num);
    		(void) fprintf(stderr,"  %d\n",attribute->scale);
    		(void) fprintf(stderr,"  %s\n",attribute->value_type);
    		(void) fprintf(stderr,"  %s\n",attribute->value_unit);
    		(void) fprintf(stderr,"\n");
   	 }
    }

	interval_init(&interval);


   	time_t rawtime;
   	struct tm *info, *info_local;

	//timer_t timerid_sample, timerid_upload;
	timer_t timerid_sample;

	//struct sigevent evp_sample,evp_upload;
	struct sigevent evp_sample;
	memset(&evp_sample, 0, sizeof(struct sigevent));		
	//memset(&evp_upload, 0, sizeof(struct sigevent));		

	evp_sample.sigev_value.sival_int = 100;			
	evp_sample.sigev_notify = SIGEV_THREAD;		
	evp_sample.sigev_notify_function = timer_thread_sample;		
#if 0
	evp_upload.sigev_value.sival_int = 200;			
	evp_upload.sigev_notify = SIGEV_THREAD;			
	evp_upload.sigev_notify_function = timer_thread_upload;	
#endif

	if (timer_create(CLOCKID, &evp_sample, &timerid_sample) == -1) {
		perror("fail to sample timer_create");
		exit(-1);
	}

#if 0
	if (timer_create(CLOCKID, &evp_upload, &timerid_upload) == -1) {
		perror("fail to upload timer_create");
		exit(-1);
	}
#endif

	time(&rawtime);
	/* Get GMT time */
	info = gmtime(&rawtime );
	info_local = localtime(&rawtime);
	printf("Current world clock:\n");
	//printf("UTC: %2d:%02d:%02d\n\n", info->tm_hour, info->tm_min,info->tm_sec);
	printf("UTC: %4d%02d%02d%02d%02d%02d\n\n", (info->tm_year + 1900),(info->tm_mon + 1),info->tm_mday,info->tm_hour, info->tm_min,info->tm_sec);
	printf("Current local clock:\n");
	//printf("UTC: %2d:%02d:%02d\n\n", info->tm_hour, info->tm_min,info->tm_sec);
	printf("UTC: %4d%02d%02d%02d%02d%02d\n\n", (info_local->tm_year + 1900),(info_local->tm_mon + 1),info_local->tm_mday,info_local->tm_hour, info_local->tm_min,info_local->tm_sec);
	
	   
	//struct itimerspec it_sample,it_upload;
	struct itimerspec it_sample;

	itimer_init(info,&it_sample,interval.sample_interval);
	//itimer_init(info,&it_upload,interval.upload_interval);
	
	if (timer_settime(timerid_sample, 0, &it_sample, NULL) == -1) {
		perror("fail to sample timer_settime");
		exit(-1);
	}
#if 0
	if (timer_settime(timerid_upload, 0, &it_upload, NULL) == -1) {
		perror("fail to upload timer_settime");
		exit(-1);
	}
#endif
	
	pause();
	
	return 0;
}

