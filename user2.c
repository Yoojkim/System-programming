#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define CHECK_MMAP_SUCCESS(_addr)	\
if(_addr == MAP_FAILED){		\
	perror("mmap");			\
	exit(1);			\
}

int len(char* str){
	int i=0;
	while(str[i]!=0) i++;
	return i;
}
void clear(caddr_t addr){
	int i=0;
	while(addr[i]!=0){
		addr[i]=0;
		i++;
	}
}
int main(int argc, char*argv[]){
	int turnfd, msgfd;
	caddr_t turnaddr, msgaddr;
	char msg[16];
	if((msgfd=open("msg.dat",O_RDWR|O_CREAT,0777))==-1){
		perror("open");
		exit(1);
	}	
	ftruncate(msgfd,16);
	if((turnfd=open("turn.dat",O_RDWR|O_CREAT,0777))==-1){
		perror("open");
		exit(1);
	}
	ftruncate(turnfd,2);
	int pageSize=getpagesize();

	turnaddr=mmap(NULL,pageSize,PROT_READ|PROT_WRITE,MAP_SHARED,turnfd,(off_t)0);
	CHECK_MMAP_SUCCESS(turnaddr);
	msgaddr=mmap(NULL,pageSize,PROT_READ|PROT_WRITE,MAP_SHARED,msgfd,(off_t)0);
	CHECK_MMAP_SUCCESS(msgaddr);

	close(turnfd);
	close(msgfd);

	while(1){
		if(turnaddr[0]==2){
			printf("[수신] %s\n",msgaddr);
			clear(msgaddr);
			msync(msgaddr,16,MS_SYNC);
			printf("[발신] ");
			fgets(msg,16,stdin);
			for(int i=0;i<len(msg)-1;i++){
				msgaddr[i]=msg[i];
			}
			turnaddr[0]=1;
			//동기화 
			msync(msgaddr,16,MS_SYNC);
			msync(turnaddr,1,MS_SYNC);
			exit(0);
		}
		else sleep(1);
	}
	
	return 0;
}
				

