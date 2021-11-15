#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

int start=-1;
int procflag=0;

void handler(int signo){
	printf("- Received signal: %s\n",strsignal(signo));
}

void usr1Handler(int signo){
	if (start==-1){
		procflag=-1;
		start=0;
		printf("Do Not Disturb mode on!\n");
	}
	else{
		printf("Do Not Disturb mode off!\n");
		printf("[During the DND mode]\n");
		procflag=1;		
		start=-1;
	}
}	
			
int main(void){
	signal(SIGUSR1,usr1Handler);
	sigset_t blocksig;
	sigemptyset(&blocksig);
	for(int i=1;i<=SIGRTMAX;i++){
		if((i!=9) && (i!=19) && (i!=10)){
			sigaddset(&blocksig,i);
		}
	}

	while(1){
		if(procflag==-1) {
			for(int i=1;i<=SIGRTMAX;i++){
				if(i!=9 && i!=19 && i!=10){
					signal(i,handler);
				}
			}
			sigprocmask(SIG_SETMASK,&blocksig,(sigset_t *)NULL);
			procflag=0;
		}
		else if(procflag==1){
			sigprocmask(SIG_UNBLOCK,&blocksig,(sigset_t *)NULL);
			for(int i=1;i<=SIGRTMAX;i++){
				if((i!=9) && (i!=19) && (i!=10)){
					signal(i,SIG_DFL);
				}
			}
			procflag=0;
		}
	}
	return 0;
}
