#include <stdlib.h>
#include <stdio.h>
/*单个生产者和单个消费者*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>

#define BUFFER_SIZE         5       //产品库存大小
#define PRODUCT_CNT         50      //产品生产总数 

void echo()
{
	while(1){
		printf("in thread.\n");
		system("echo hello cwt >> /tmp/cwt");
		sleep(1);
	}

}
int main(int argc, char *argv[])
{
    pthread_t th_a;
	int retval;
        
    pthread_create(&th_a, NULL, echo, 0);

    pthread_join(th_a, &retval);

    return 0;
}

