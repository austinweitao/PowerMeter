/*
**  test appending a node at the end of the list. print the result
**  and then free the memory. a user defined function is called to free
**  the data.
**
**  Development History:
**      who                  when           why
**      ma_muquit@fccc.edu   Aug-09-1998    first cut
*/


#include <sll.h>


typedef struct{
	int modbus_id;
	int number;
}Meter;

static void freeData(void **data);

int main (int argc,char **argv) 
{
    Sll
        *l,
        *head=NULL,
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
    addr->modbus_id = 1;
    addr->number = 1; 
    /*
    ** free nodes
    */
    (void) fprintf(stderr,"\n---------------[ freeing ]----------\n");
    destroyNodes(&head,freeData);

    exit(0);
}


/*
** routine to free the user data
*/

static void freeData(void **data)
{
    (void) fprintf(stderr,"in free data function.\n");
    
    Meter
        **addr=(Meter **) data;

    static int
        n=0;

    n++;
		(void) fprintf(stderr,"Freeing Meter.\n");
        (void) free((Meter *) (*addr));
        (*addr)=NULL;
    }
}

