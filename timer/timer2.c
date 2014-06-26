#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define CLOCKID CLOCK_REALTIME

void timer_thread_gathering(union sigval v)
{
    time_t rawtime;
    struct tm *info;
	printf("******gathering timer expired******.\n");
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
   time_t rawtime;
   struct tm *info;
	// XXX int timer_create(clockid_t clockid, struct sigevent *evp, timer_t *timerid);
	// clockid--值：CLOCK_REALTIME,CLOCK_MONOTONIC，CLOCK_PROCESS_CPUTIME_ID,CLOCK_THREAD_CPUTIME_ID
	// evp--存放环境值的地址,结构成员说明了定时器到期的通知方式和处理方式等
	// timerid--定时器标识符

	timer_t timerid_gathering, timerid_upload;

	struct sigevent evp_gathering,evp_upload;
	memset(&evp_gathering, 0, sizeof(struct sigevent));		//清零初始化
	memset(&evp_upload, 0, sizeof(struct sigevent));		//清零初始化

	evp_gathering.sigev_value.sival_int = 100;			//也是标识定时器的，这和timerid有什么区别？回调函数可以获得
	evp_gathering.sigev_notify = SIGEV_THREAD;			//线程通知的方式，派驻新线程
	evp_gathering.sigev_notify_function = timer_thread_gathering;		//线程函数地址

	evp_upload.sigev_value.sival_int = 200;			//也是标识定时器的，这和timerid有什么区别？回调函数可以获得
	evp_upload.sigev_notify = SIGEV_THREAD;			//线程通知的方式，派驻新线程
	evp_upload.sigev_notify_function = timer_thread_upload;		//线程函数地址

	if (timer_create(CLOCKID, &evp_gathering, &timerid_gathering) == -1) {
		perror("fail to gathering timer_create");
		exit(-1);
	}
	if (timer_create(CLOCKID, &evp_upload, &timerid_upload) == -1) {
		perror("fail to upload timer_create");
		exit(-1);
	}

	// XXX int timer_settime(timer_t timerid, int flags, const struct itimerspec *new_value,struct itimerspec *old_value);
	// timerid--定时器标识
	// flags--0表示相对时间，1表示绝对时间
	// new_value--定时器的新初始值和间隔，如下面的it
	// old_value--取值通常为0，即第四个参数常为NULL,若不为NULL，则返回定时器的前一个值
	
	//第一次间隔it.it_value这么长,以后每次都是it.it_interval这么长,就是说it.it_value变0的时候会装载it.it_interval的值
    time(&rawtime);
    /* Get GMT time */
    info = gmtime(&rawtime );
    printf("Current world clock:\n");
    printf("UTC: %2d:%02d:%02d\n", info->tm_hour, info->tm_min,info->tm_sec);

	int sec_lefts;
   
	struct itimerspec it_gathering,it_upload;
    if((sec_lefts = info->tm_sec % 5) == 0)
	{
	
		it_gathering.it_interval.tv_sec = 5;
		it_gathering.it_interval.tv_nsec = 0;
		it_gathering.it_value.tv_sec = 0;
		it_gathering.it_value.tv_nsec = 1;  //a workaround to start the thread immediately

	}
	else
	{
		it_gathering.it_interval.tv_sec = 5;
		it_gathering.it_interval.tv_nsec = 0;
		it_gathering.it_value.tv_sec = 5 - sec_lefts;
		it_gathering.it_value.tv_nsec = 0;


	}

    if(info->tm_sec == 0)
	{
		//need to upload file immediately
	
		it_upload.it_interval.tv_sec = 60;
		it_upload.it_interval.tv_nsec = 0;
		it_upload.it_value.tv_sec = 0;
		it_upload.it_value.tv_nsec = 1;  //a workaround to start the thread immediately

	}
	else
	{
		it_upload.it_interval.tv_sec = 60;
		it_upload.it_interval.tv_nsec = 0;
		it_upload.it_value.tv_sec = 60 - info->tm_sec;
		it_upload.it_value.tv_nsec = 0; 
	}

	if (timer_settime(timerid_gathering, 0, &it_gathering, NULL) == -1) {
		perror("fail to gathering timer_settime");
		exit(-1);
	}
	if (timer_settime(timerid_upload, 0, &it_upload, NULL) == -1) {
		perror("fail to upload timer_settime");
		exit(-1);
	}

	pause();

	return 0;
}
