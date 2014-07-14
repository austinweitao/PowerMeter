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
	int addr;	// meter attribute register start address
	unsigned char reg_num;	//meter attribute register number
	int scale;	//attribute value scale
	char*  value_type;	//typde of the value: float,int,long{0,1,2}	
	char*  value_unit;  //type of the value unit: KWh,KVarh,Volt
}Meter_Attribute;

typedef struct{
	unsigned char modbus_id;
	unsigned char num_attribute;
	Meter_Attribute *attribute;
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
    addr->num_attribute = 1;
    addr->attribute = (Meter_Attribute *) malloc(sizeof(Meter_Attribute));
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
	addr->attribute->addr = 1000;
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
    ** append node after the last one
    */
    addr=(Meter*) malloc(sizeof(Meter));
    if (addr == NULL) {
        (void) fprintf(stderr," malloc failed\n");
    	destroyNodes(&head,freeData);
        exit(-1);
    }
    new=allocateNode((void *) addr);
    appendNode(&head,&new);

    (void) fprintf(stderr,"\n---------------[ appending ]----------\n");
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

    for(i = 0; i < addr->num_attribute;i++)
    {
	addr->attribute->addr = 1002;
	addr->attribute->reg_num = 2;
	addr->attribute->scale = 2;
	addr->attribute->value_type = strdup("float");
	addr->attribute->value_unit= strdup("KVarh");
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
    ** append node after the last one
    */
    addr=(Meter*) malloc(sizeof(Meter));
    if (addr == NULL)
    {
        (void) fprintf(stderr," malloc failed\n");
        exit(-1);
    }
    new=allocateNode((void *) addr);
    appendNode(&head,&new);

    (void) fprintf(stderr,"\n---------------[ appending ]----------\n");
    /*
    ** it will be the last node
    */
    addr->modbus_id = 3;
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

    for(i = 0; i < addr->num_attribute;i++)
    {
	addr->attribute->addr = 1004;
	addr->attribute->reg_num = 2;
	addr->attribute->scale = 3;
	addr->attribute->value_type = strdup("float");
	addr->attribute->value_unit= strdup("KVAh");
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
}

