#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/mman.h>

sem_t* semaphore;
pid_t otherPid;
sigset_t sigSet;


void signalHandler1(int signum)
{
printf("Caught The Sight Of The Signal %d\n", signum);
printf("Exiting The Proccess Now\n");
sem_post(semaphore);
_exit(0);
}//End Of The Singal Handler


void signalHandler2(int signum)
{ 
printf("I Am Alive\n");
}//End Of Yhe Quick Evenet Handler


void childProcess()
{
signal(SIGUSR1, signalHandler1);
signal(SIGUSR2, signalHandler2);
int value;
sem_getvalue(semaphore, &value);

printf("Child Proccess The Semaphore's Count Is At: %d.\n", value);
printf("Now Grabbing The Semaphore\n");
sem_wait(semaphore);
sem_getvalue(semaphore,&value);
printf("Child Proccess The Semaphore's Count Is At: %d.\n", value);
printf("--------------------------Starting The Proccess, Please Stand By-------------------------\\n");

for(int i = 0; i < 60; ++i)
{
printf("./n");
sleep(1);
}
sem_post(semaphore);
printf("Ending The Child Proccess\n");
_exit(0);
}//End Of The Child Proccess


void *checkHungChild(void *a)
{
int* status = a;
printf("Checking For Hung Child Proccess\n");
sleep(10);
if (sem_trywait(semaphore) != 0)
{
printf("Child Is Currently Hung\n");
*status = 1;
}
else
{
printf("Child Is Running As Normal\n");
*status = 0;
}
return NULL;
}//End Of The Child Checker


void parentProcess()
{
sleep(2);
if(getpgid(otherPid) >= 0)
{
printf("Child Proccess Is Currently Running\n");
}

int value;
sem_getvalue(semaphore, &value);
printf("Currently Running Parent Proccess At Semaphore Count: %d\n",value);
if(sem_trywait(semaphore) != 0)
{
pthread_t tid1;
int status =0;
printf("Detected The Child Proccess Is Running For To Long\n");
if(pthread_create(&tid1, NULL, checkHungChild, &status))
{
printf("ERROR... Creating Timer Thread/n");
_exit(1);
}
if(pthread_join(tid1, NULL))
{
printf("\n ERROR Joining Thread Timer \n");
exit(1);
}
if(status ==1)
{
printf("Going To Kill The Child Proccess With The ID Of %d\n", otherPid);
kill(otherPid, SIGTERM);
printf("Child Proccess Has Been Killed");
sleep(5);
kill(otherPid,SIGUSR2);
sleep(1);

sem_getvalue(semaphore,&value);
printf("In The Parent Proccess With A Semaphore Count Of: %d\n",value);
if(sem_trywait(semaphore) != 0)
{
if(value == 0)
sem_post(semaphore);
printf("Cleaned Up And Semaphore Available\n");
sem_getvalue(semaphore,&value);
printf("In Parent Proccess With The Semaphore Count Of %d\n",value);
}
else
{
printf("Finally Have The Semaphore\n");
}
}
}

printf("Exiting The Parent Proccess\n");
_exit(0);
}//End Of The Parent Proccess


int main(int argc, char* argv[])
{
pid_t pid;
semaphore = (sem_t*)mmap(0,sizeof(sem_t), PROT_READ|PROT_WRITE,MAP_SHARED|MAP_ANONYMOUS, -1, 0);

if(sem_init(semaphore, 1, 1) != 0)
{
printf("Semaphore Creations Has Failed \n");
exit(EXIT_FAILURE);
}

pid = fork();
if(pid == 1)
{
printf("Fork Creation Has Failed\n");
exit(EXIT_FAILURE);
}
if(pid == 0)
{
printf("Starting Child Proccess That Has The Pid Of: %d\n", getpid());
otherPid = getpid();
childProcess();
}
else
{
printf("Getting Parent Proccess With ID Of: %d\n", getpid());
otherPid = pid;
parentProcess();
}
sem_destroy(semaphore);
return 0;
}//End Of The Main Function