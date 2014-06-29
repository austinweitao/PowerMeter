#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define CLOCKID CLOCK_REALTIME

#define USER_SAMPLE_INTERVAL (5)
#define USER_UPLOAD_INTERVAL (30*SECSPERMIN)
#define SECSPERHOUR 3600
#define SECSPERMIN 60	

typedef struct{
	int addr;  	//meter attribute register start address
	unsigned char reg_num;	//meter attribute register numbers
	int scale;	// attribute value scale
	char *value_type;  //type of the value: float,int,long
}Meter_Attribute;

typedef struct{
	unsigned char modbus_id;	//modbus id for a specific meter 
	unsigned char num_attribute;	//num of atrributes a meter provides, MAX:255	
	Meter_Attribute *attribute;	//pointer to a specific meter's attriubtes
}Meter;


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
	time_t rawtime;
    	struct tm *info;
	printf("******sample timer expired******.\n");
    	time(&rawtime);
    	/* Get GMT time */
    	info = gmtime(&rawtime );
    	printf("UTC: %2d:%02d:%02d\n", info->tm_hour, info->tm_min,info->tm_sec);
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

	interval_init(&interval);

   	time_t rawtime;
   	struct tm *info;

	timer_t timerid_sample, timerid_upload;

	struct sigevent evp_sample,evp_upload;
	memset(&evp_sample, 0, sizeof(struct sigevent));		
	memset(&evp_upload, 0, sizeof(struct sigevent));		

	evp_sample.sigev_value.sival_int = 100;			
	evp_sample.sigev_notify = SIGEV_THREAD;		
	evp_sample.sigev_notify_function = timer_thread_sample;		

	evp_upload.sigev_value.sival_int = 200;			
	evp_upload.sigev_notify = SIGEV_THREAD;			
	evp_upload.sigev_notify_function = timer_thread_upload;	

	if (timer_create(CLOCKID, &evp_sample, &timerid_sample) == -1) {
		perror("fail to sample timer_create");
		exit(-1);
	}
	if (timer_create(CLOCKID, &evp_upload, &timerid_upload) == -1) {
		perror("fail to upload timer_create");
		exit(-1);
	}

	time(&rawtime);
	/* Get GMT time */
	info = gmtime(&rawtime );
	printf("Current world clock:\n");
	printf("UTC: %2d:%02d:%02d\n\n", info->tm_hour, info->tm_min,info->tm_sec);
	
	   
	struct itimerspec it_sample,it_upload;

	itimer_init(info,&it_sample,interval.sample_interval);
	itimer_init(info,&it_upload,interval.upload_interval);
	
	if (timer_settime(timerid_sample, 0, &it_sample, NULL) == -1) {
		perror("fail to sample timer_settime");
		exit(-1);
	}
	if (timer_settime(timerid_upload, 0, &it_upload, NULL) == -1) {
		perror("fail to upload timer_settime");
		exit(-1);
	}
	
	pause();
	
	return 0;
}
