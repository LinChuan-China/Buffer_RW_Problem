#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>
#define BUFFER 10


int buffer[10];
int get_srand();

int empty1,full1;
int empty2,full2;
int empty3,full3;
int num_producer=1, num_customer=3;

int sum = 0;
int in=0;
int out1=0;
int out2=0;
int out3=0;

int shmid_buffer[10];
int *shmptr_buffer[10]; 
int shmid_mutex;
int *shmptr_mutex;
int shmid_full1;
int *shmptr_full1;
int shmid_full2;
int *shmptr_full2;
int shmid_full3;
int *shmptr_full3;
int shmid_empty1;
int *shmptr_empty1;
int shmid_empty2;
int *shmptr_empty2;
int shmid_empty3;
int *shmptr_empty3;

int get_srand(int range)

{
        int ans;
        srand((unsigned)time(NULL));
        ans = rand()%range+1;  //1~range
        return ans;
}

void*  producer()

{
    int producer_number=1;
    while(1)
    {
    	if( (*shmptr_mutex) ==0)continue;
    	else (*shmptr_mutex)-=1;

    	if( (*shmptr_empty1) == 0){
			(*shmptr_mutex)+=1;    
			continue;		
		}
		else (*shmptr_empty1)-=1;
		
    	if( (*shmptr_empty2) == 0){
			(*shmptr_mutex)+=1; 
			(*shmptr_empty1)+=1;   
			continue;		
		}
		else (*shmptr_empty2)-=1;
		
    	if( (*shmptr_empty3) == 0){
			(*shmptr_mutex)+=1; 
			(*shmptr_empty1)+=1;  
			(*shmptr_empty2)+=1;  
			continue;		
		}
		else (*shmptr_empty3)-=1;
		    	
    	



        *(shmptr_buffer[in])=producer_number;
        printf("The producer is writing %d\n",producer_number);
        producer_number++;
        in=(in+1)%BUFFER;


 		(*shmptr_full3)+=1;       
		(*shmptr_full2)+=1;
		(*shmptr_full1)+=1;
		(*shmptr_mutex)+=1;
		
		sleep(get_srand(2));
    }
}
void* customer1()

{
    while(1)
    {
    	if( (*shmptr_mutex) ==0)continue;
    	else (*shmptr_mutex)-=1;
    	
    	
    	if( (*shmptr_full1) == 0){
			(*shmptr_mutex)+=1;    
			continue;		
		}
		else (*shmptr_full1)-=1;

          

        printf("The customer1 is reading %d\n",*(shmptr_buffer[out1]));
        out1=(out1+1)%BUFFER;
        
        
        
        
        
        (*shmptr_empty1)+=1;
		(*shmptr_mutex)+=1;
		
		sleep(get_srand(4));
    }
}
void* customer2()

{

    while(1)
    {
    	if( (*shmptr_mutex) ==0)continue;
    	else (*shmptr_mutex)-=1;
    	

    	if( (*shmptr_full2) == 0){
			(*shmptr_mutex)+=1;    
			continue;		
		}
		else (*shmptr_full2)-=1;

 
        printf("The customer2 is reading %d\n",*(shmptr_buffer[out2]));
        out2=(out2+1)%BUFFER; 
		
		
		  
        (*shmptr_empty2)+=1;		      
		(*shmptr_mutex)+=1;
		
		sleep(get_srand(6));
    }
}
void* customer3()

{
    while(1)
    {

    	if( (*shmptr_mutex) ==0)continue;
    	else (*shmptr_mutex)-=1;
    	
    	if( (*shmptr_full3) == 0){
			(*shmptr_mutex)+=1;    
			continue;		
		}
		else (*shmptr_full3)-=1;    	



        printf("The customer3 is reading %d\n",*(shmptr_buffer[out3]));
        out3=(out3+1)%BUFFER;      
 
 
 
 
        (*shmptr_empty3)+=1;        
		(*shmptr_mutex)+=1;  
		
		sleep(get_srand(8));
    }
}
int main()
{   

    for(int i=0;i<10;i++){
    	    shmid_buffer[i]=shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT|0666);
	}
	for(int i=0;i<10;i++){
    	    shmptr_buffer[i]=(int *) shmat(shmid_buffer[i], 0, 0);
	}
	
	shmid_mutex=shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT|0666);
	shmptr_mutex=(int *) shmat(shmid_mutex, 0, 0);
	*shmptr_mutex=1;
	
	shmid_full1=shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT|0666);
	shmptr_full1=(int *) shmat(shmid_full1, 0, 0);
	*shmptr_full1=0;
	
	shmid_full2=shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT|0666);
	shmptr_full2=(int *) shmat(shmid_full2, 0, 0);
	*shmptr_full2=0;
	
	shmid_full3=shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT|0666);
	shmptr_full3=(int *) shmat(shmid_full3, 0, 0);
	*shmptr_full3=0;
	
	shmid_empty1=shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT|0666);
	shmptr_empty1=(int *) shmat(shmid_empty1, 0, 0);
	*shmptr_empty1=10;
	
	shmid_empty2=shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT|0666);
	shmptr_empty2=(int *) shmat(shmid_empty2, 0, 0);
	*shmptr_empty2=10;
	
	shmid_empty3=shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT|0666);
	shmptr_empty3=(int *) shmat(shmid_empty3, 0, 0);
	*shmptr_empty3=10;
	
    pid_t pid1,pid2,pid3;
    pid1 = fork();   
    if(pid1==0)  
    {
		customer1();
    }
    else          
    {
		pid2 = fork();
		if(pid2==0)  
		{
			customer2();
		}
		else          
		{
			pid3 = fork();
			if(pid3==0){
				customer3();
			}
			else{
				producer();
			}
		}
     }
    return 0;
}
