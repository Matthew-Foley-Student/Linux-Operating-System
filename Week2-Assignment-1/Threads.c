#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#define NUM_LOOPS 10

void* messege_sender(void *arg)
{
if(arg == "Thread 1"){
for(int i =0; i <NUM_LOOPS; i++)
{
sleep(1);
printf("This Is A Messege From Thread %p\n", arg);
}//End Of For Loops
}
else
{
for(int i =0; i <NUM_LOOPS; i++)
{
sleep(2);
printf("This Is A Messege From Thread %p\n", arg);
}//End Of For Loops
}
pthread_exit(NULL);
}//End Of Messeg Sender Method

int main (void)
{
printf("Starting Threads, please Wait..");
pthread_t id1;
pthread_t id2;

pthread_create(&id1,NULL,messege_sender,"Thread 1");
pthread_create(&id2,NULL,messege_sender,"Thread 2");
pthread_join(id1,NULL);
pthread_join(id2,NULL);
printf("Threads Are Now Combined\\n");
return 0;

}//End Of The Main Method
