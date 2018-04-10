#include "Data_manage.h"

//get the average of data group
//after this progress the martix only have 300 data to use
void Data_manage(uint16_t* data)
{
	int i,j;
	uint16_t* cur=data;
	uint16_t* tem=data;
	for(i=0;i<300;i++)
	{
		for(j=0;j<5;j++)
		{
			*cur=(uint16_t)(*tem+*(tem+1)+*(tem+2)+(tem+3)+*(tem+4))/5;
		}
		tem=cur+5;
		cur++;
	}
}
