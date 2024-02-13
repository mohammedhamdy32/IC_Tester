#include "stdio.h"

long long arr=0x123456;
int set_num(int num,int bit)
{
	return (num) | (1<<bit);
}

int main()
{
long long temp;
temp=(unsigned char)(arr&0x0000FF);
temp<<=8;
printf("%x\n",temp);

arr>>=8;
temp|=(unsigned char)(arr&0x0000FF);
printf("%x\n",temp);



}
