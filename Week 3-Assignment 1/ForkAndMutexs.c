#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include<sys/mman.h>
#include <errno.h>

int MAX = 100;
int WAKEUP = SIGUSR1;
int SLEEP = SIGUSR2;
pid_t otherPid;
sigset_t sigSet;
struct CIRCULAR_BUFFER
{
int count;
int lower;
int upper;
int buffer[100];
};//End Of The Shared Structure
struct CIRCULAR_BUFFER *buffer = NULL;


void sleepAndWait()
{
int nSig;
printf("Now Feeling Sleepy\n");
sigwait(&sigSet, &nSig);
printf("Have Been Called And Time To Work\n");
}//Sleep untill the WAKEUP int is called


void wakeupOther()
{
kill (otherPid, WAKEUP);
}//calls wakeup int for anything that is under the sleep command


int getValue()
{
int value = buffer->buffer[buffer->lower];
printf(" Consumer read value of %c\n", value);
++buffer->lower;
if(buffer->lower == MAX)
 buffer->lower = 0;
--buffer->count;
return value;
}//Gets Value From Shared Buffer


void putValue(int value)
{
buffer->buffer[buffer->upper] = value;
printf("Producer Storing The Value Of: %c\n", buffer->buffer[buffer->upper]);
++buffer->upper;
if(buffer->upper == MAX)
buffer->upper = 0;
++buffer->count;
}//Adds To The Shared Buffer


void consumer()
{
sigemptyset(&sigSet);
sigaddset(&sigSet, WAKEUP);
sigprocmask(SIG_BLOCK, &sigSet, NULL);
printf("Beginning The Proccess Of Consumerism...\n");
int charicter = 0;
do 
{
sleepAndWait();
charicter = getValue();
}//End Of The Enforced Do Statement
while(charicter != 0);
printf("Buffer Fool Consumerism Starting\n");
_exit(1);
}//Consumer Proccess


void producer()
{
int value = 0;
printf("Beggining The Means Of Production\n");
char messege[14] = "Matthew Foley";
for(int i =0; i < strlen(messege); ++i)
{
putValue(messege[i]);
wakeupOther();
sleep(1);
}
putValue(0);
wakeupOther();
printf("Production Is Done All Ends Now\n");
_exit(1);
}//Producer Logic


int main(int argc, char* argv[])
{
pid_t pid;
buffer = (struct CIRCULAR_BUFFER*)mmap(0,sizeof(buffer), PROT_READ|PROT_WRITE,MAP_SHARED|MAP_ANONYMOUS,-1,0);
buffer->count = 0;
buffer->lower =0;
buffer->upper = 0;

pid = fork();
if(pid == 1)
{
printf("Can't Fork Because Of Error: %d\n", errno);
exit(EXIT_FAILURE);
}//End Of The Error In The Forks IF ANY
if(pid == 0)
{
otherPid = getppid();
producer();
}//End Of Producer Fork
else
{
otherPid = pid;
consumer();
}//End Of The Consumer Fork
return 0;
}//End Of The Main Function
