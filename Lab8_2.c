#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define CHECK_MMAP_SUCCESS(_addr) \
	if(_addr ==MAP_FAILED) { \
		perror("mmap"); \
		exit(1); 	\
	}

int count_str_len(char *str)
{
	int length=0;
	while(str[length])
	{
		length++;
	}
	return length;
}
int compare_str(char* arr1, char* arr2)
{
	int i=0;
	while(arr1[i] != '\0' || arr2[i] !='\0')
	{
		if(arr1[i]> arr2[i])
			return arr1[i]-arr2[i];
		else if (arr1[i]<arr2[i])
			return arr1[i] - arr2[i];
		i++;
	}
	return 0;
}

int main(int argc, char* argv[])
{
	caddr_t addr;

	if(argc!=3)
	{
		printf("Usage: ./exeFile .datfile .txtfile\n");
		exit(0);
	}

	int fd1;
	FILE* fd2;

	if((fd1 = open(argv[1], O_RDWR))==-1)
	{
		perror("open_first");
		exit(1);
	}

	fd2=fopen(argv[2], "r");
	if(fd2==NULL)
	{
		perror("open_second");
		exit(1);
	}
	
	int pageSize= getpagesize();

	addr= mmap(NULL, pageSize, PROT_READ| PROT_WRITE, MAP_SHARED, fd1, (off_t)0);
	CHECK_MMAP_SUCCESS(addr);

	close(fd1);

	char buffer[255]={0};
	fgets(buffer, sizeof(buffer), fd2);
	fclose(fd2);

	int pid=0;
	switch(pid=fork())
	{
		case -1:
			perror("fork");
			exit(1);
			break;
		case 0:
			for(int i=0; i<count_str_len(buffer);i++)
			{
				addr[i]=buffer[i];
				sleep(1);
			}
			break;
		default:
			while(1)
			{
				if(compare_str(addr, buffer)!=0)
				{
					printf("도르마무 mapping을 하러 왔다\n");
					printf("%s\n", addr);
					sleep(1);
				}
				else
				{
					printf("\n%s발동!\n", addr);
					printf("도르마무를 물리쳤다!\n");
					break;
				}
			}
	}
	return 0;
}




