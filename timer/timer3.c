#include <stdio.h>
#include <time.h>

#define BST (+1)
#define CCT (+8)

int main ()
{

   time_t rawtime;
   struct tm *info;

   time(&rawtime);
   /* Get GMT time */
   info = gmtime(&rawtime );
   
   printf("Current world clock:\n");
   printf("London : %2d:%02d\n", (info->tm_hour+BST)%24, info->tm_min);
   printf("China  : %2d:%02d\n", (info->tm_hour+CCT)%24, info->tm_min);

   return(0);
}
