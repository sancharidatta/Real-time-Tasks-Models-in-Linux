#define _GNU_SOURCE 
#include<stdio.h>
#include <sched.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdbool.h>
#include <signal.h>
#include <sys/time.h>
#include <linux/input.h>
#include <math.h>
#include <semaphore.h>
#define MOUSEFILE "/dev/input/mice"					//Mouse Event FILE
pthread_mutex_t lock[9];						 
static pthread_mutex_t left_click_mutex , right_click_mutex ;
static pthread_cond_t left_click_cond , right_click_cond ;
long globalTimer;
pthread_mutex_t M[10],startm;
pthread_cond_t start;
int cret;
struct task{		//Structure to store the input files
char type;
int pri[4];

}task;

struct periodValues{   //structure for storing the input data
int thread_num;
int period;
int iterations1;
int iterations2;
}periodValues;

FILE *fp;


/*taskbody execution*/
void taskbody(int x)
{

int i=0, j=0;
	
	for (i = 0; i < x; i++)
	{
		j = j + i;
	}
	

}


/*PERIODIC FUNCTION*/ 
void *periodic(void *arg)
{
  struct periodValues* per=(struct periodValues*) arg;
  printf("T_num %d Period %d \n",per->thread_num,per->period);
  struct timespec now, after, diff,last;
  int loop_itr;
  while(1)
{
clock_gettime(CLOCK_MONOTONIC, &now);
loop_itr = rand()% per->iterations1 + (per->iterations2-per->iterations1);  //Iterations uniformly distributed between the given inputs. 


    taskbody(loop_itr);					//Taskbody execution

	fprintf(fp,"Periodic thread number %d \n",per->thread_num);
clock_gettime(CLOCK_MONOTONIC, &after);
diff.tv_sec = after.tv_sec - now.tv_sec;
diff.tv_nsec = after.tv_nsec - now.tv_nsec;

clock_nanosleep(CLOCK_MONOTONIC,TIMER_ABSTIME,&diff,0);
clock_gettime(CLOCK_MONOTONIC,&last);
if(last.tv_sec - globalTimer >=3 ){
exit(1);}


}
  
}

/* Mouse function*/
void *mouseclick(void *arg)
{
     int fd;
     unsigned char data[3];
    
    
    if((fd=open(MOUSEFILE,O_RDONLY))== -1)
    {
        perror("opening device");
        exit(EXIT_FAILURE);
        
    }
    while(1)
    {
        if((read(fd,data,sizeof(data)))>0)
        {
	    if(data[0]& 0x1)//(ie.value == 0 && ie.code == 272)// value =0 means key release
            {
                fprintf(fp,"left mouse click\n");
                pthread_cond_broadcast(&left_click_cond);
                
            }
	    else if(data[0]& 0x2)//(ie.value == 0 && ie.code == 273)
            {
                fprintf(fp,"right mouse click\n");
                pthread_cond_broadcast(&right_click_cond);
               
		
            }
            
        }
    }
}

/*Aperiodic Fuction*/
void *aperiodic(void *arg)
{
  int loop_itr;
  
  struct periodValues* a_per=(struct periodValues*) arg;
  printf("A T_num %d APeriod %d \n",a_per->thread_num,a_per->period);
  loop_itr = rand()% a_per->iterations1 + (a_per->iterations2-a_per->iterations1);
  pthread_mutex_lock(&startm);
    cret = pthread_cond_wait(&start,&startm);            
    if(cret!=0)
        printf("Periodic condition variable error %d\n",cret);        
pthread_mutex_unlock(&startm);

printf("Inside aperiodic thread\n");


while(1){
		if(a_per->period == 0)

			{
				pthread_mutex_lock(&left_click_mutex);
				
				pthread_cond_wait(&left_click_cond, &left_click_mutex);
				fprintf(fp,"inside cond wait");
				taskbody(loop_itr);
				fprintf(fp,"APeriodic thread number %d \n",a_per->thread_num);
				pthread_mutex_unlock(&left_click_mutex);
			}
		
else if(a_per->period == 1)
		{
			
				pthread_mutex_lock(&right_click_mutex);		
	
				pthread_cond_wait(&right_click_cond, &right_click_mutex);
				
				
				taskbody(loop_itr);
				fprintf(fp,"APeriodic thread number %d \n",a_per->thread_num);	
				pthread_mutex_unlock(&right_click_mutex);
			}
		
		
	}
  pthread_exit(0);

}

/*Main Function */

int main()
{
int num_tasks;
int total_period;


FILE *ptr_file; 
ptr_file =fopen("input", "r");              //opening input file
if(ptr_file==NULL){
printf("No file Found");
return -1;
}
fscanf(ptr_file,"%d %d",&num_tasks,&total_period); 

printf("%d\t%d\n",num_tasks,total_period);
fp = fopen("result.txt","w");			//printing the output in result file

struct task allTasks[num_tasks];

int i=0;
while(fscanf(ptr_file," %c %d %d %d %d",&allTasks[i].type,&allTasks[i].pri[0],&allTasks[i].pri[1],&allTasks[i].pri[2],&allTasks[i].pri[3])!=EOF){ 

i++;
}

for(int i=0;i<num_tasks;i++){

printf("%c\t %d\t%d\t%d\t%d \n",allTasks[i].type,allTasks[i].pri[0],allTasks[i].pri[1],allTasks[i].pri[2],allTasks[i].pri[3]);
}


fclose(ptr_file);


pthread_attr_t attr[num_tasks];
    pthread_t thread[num_tasks];
    pthread_t mouse;
    

struct sched_param param,m_param;
 
struct periodValues arg[num_tasks];
    cpu_set_t cpus;
    CPU_ZERO(&cpus);
    CPU_SET(0,&cpus);

m_param.sched_priority = 99;			//Priority of mouse thread
pthread_create(&mouse,NULL,&mouseclick,NULL);    //Creating mouse thread
pthread_setschedparam(mouse, SCHED_FIFO, &m_param);
pthread_tryjoin_np(mouse,NULL);

struct timespec global;
clock_gettime(CLOCK_MONOTONIC,&global) ;
globalTimer = global.tv_sec;


for (int i = 0 ; i < num_tasks;i++)		
    {
         pthread_attr_init(&attr[i]); 
         pthread_attr_setschedpolicy(&attr[i],SCHED_FIFO);
        pthread_attr_setaffinity_np(&attr[i],sizeof(cpu_set_t),&cpus);
        param.sched_priority = allTasks[i].pri[0] ;
        pthread_attr_setschedparam(&attr[i],&param);

        arg[i].thread_num   = i;
        arg[i].iterations1  = allTasks[i].pri[2];
        arg[i].iterations2  = allTasks[i].pri[3];
        arg[i].period       = allTasks[i].pri[1];
        


//Creation of Periodic and Aperiodic threads

 if(allTasks[i].type=='P')					//checking periodic or aperiodic
        pthread_create(&thread[i],&attr[i],periodic,&arg[i]);//creating thread
        else
        {
            printf("creating");
        pthread_create(&thread[i],&attr[i],aperiodic,&arg[i]);   
        }
    }




  for (int i = 0 ; i < num_tasks;i++)
    {
      pthread_join(thread[i],NULL);
    }

return 0;
}

