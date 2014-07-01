#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include "sll.h"
#include <errno.h>
#include <modbus.h>

#include "unit-test.h"
enum {
    TCP,
    TCP_PI,
    RTU
};


#define CLOCKID CLOCK_REALTIME

#define USER_SAMPLE_INTERVAL (5)
#define USER_UPLOAD_INTERVAL (15)
#define SECSPERHOUR 3600
#define SECSPERMIN 60	

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
    addr->num_attribute = 1;
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
    
    (void) fprintf(stderr,"Node: %d\n", ++n);
    (void) fprintf(stderr,"  %d\n",addr->modbus_id);
    (void) fprintf(stderr,"  %d\n",addr->num_attribute);
    (void) fprintf(stderr,"  %d\n",addr->attribute->addr);
    (void) fprintf(stderr,"  %d\n",addr->attribute->reg_num);
    (void) fprintf(stderr,"  %d\n",addr->attribute->scale);
    (void) fprintf(stderr,"  %s\n",addr->attribute->value_type);
    (void) fprintf(stderr,"  %s\n",addr->attribute->value_unit);

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

void second_trans(unsigned int seconds)
{
	

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
	printf("inteval timer secs left:%d\n",it_spec->it_value.tv_sec);

}

void timer_thread_sample(union sigval v)
{
	static unsigned int counter = 0;
	counter++;

	time_t rawtime;
    	struct tm *info;
	printf("******sample timer expired******.\n");
    time(&rawtime);
    /* Get GMT time */
    info = gmtime(&rawtime );
    printf("UTC: %2d:%02d:%02d\n", info->tm_hour, info->tm_min,info->tm_sec);

//*****************************sampling data**********************//
	Sll *l;
	Meter *meter;

    int n=0;
    for (l=head; l; l=l->next)
    {
        meter = (Meter*) l->data;
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

    	ctx = modbus_new_rtu("/dev/ttyUSB0", 19200, 'N', 8, 1);
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
		for(i = 0; i < meter->num_attribute,attribute; i++,attribute++){
    		/** HOLDING REGISTERS **/

    		/* Single register */

    		rc = modbus_read_registers(ctx, attribute->addr,
                               attribute->reg_num, tab_rp_registers);
    		if (rc != attribute->reg_num) {
        		printf("FAILED (nb points %d)\n", rc);
        		goto close;
    		}

			if(!strcmp(attribute->value_type,"float")){
    			tmp_value = tab_rp_registers[0];
    			tab_rp_registers[0] = tab_rp_registers[1];
    			tab_rp_registers[1] = tmp_value;
			}
   
    		float_value = modbus_get_float(tab_rp_registers);
    		printf("float value is %f %s\n",float_value,attribute->value_unit);  

    		printf("OK\n");

			

		}
	

close:
    /* Free the memory */
    free(tab_rp_registers);

    /* Close the connection */
    modbus_close(ctx);
    modbus_free(ctx);
    }


//******************************end of sampling data******************//


	printf("counter is %d.\n",counter);
	if(counter ==  get_upload_interval() / get_sample_interval() )
	{
		printf("######upload time######.\n");
		counter = 0;
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
    	(void) fprintf(stderr,"  %d\n",addr->num_attribute);
    	(void) fprintf(stderr,"  %d\n",addr->attribute->addr);
    	(void) fprintf(stderr,"  %d\n",addr->attribute->reg_num);
    	(void) fprintf(stderr,"  %d\n",addr->attribute->scale);
    	(void) fprintf(stderr,"  %s\n",addr->attribute->value_type);
    	(void) fprintf(stderr,"  %s\n",addr->attribute->value_unit);
    }

	interval_init(&interval);


   	time_t rawtime;
   	struct tm *info;

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
	printf("Current world clock:\n");
	printf("UTC: %2d:%02d:%02d\n\n", info->tm_hour, info->tm_min,info->tm_sec);
	
	   
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
