#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>

int i =0;
volatile sig_atomic_t keepRunning = 0;
volatile sig_atomic_t loopCheck = 0;



void handle_siguser1(int sig)
{
keepRunning = 1;
printf("Signal Recieved \\n");
}//End Of The Signal Applicator For Loop Ending

void handle_siguser2(int sig)
{
loopCheck = 1;
printf("Signal Recieved \\n");
}//End Of The Signal Applicator For Parent Child Communication


int main(int argc, char *argv[])
{
printf("Starting The Fork Proccess, Making Parent And Child");
pid_t pid = fork();
signal(SIGUSR1, handle_siguser1);
signal(SIGUSR2, handle_siguser2);

if(pid < 0)
{perror("Fork Has Failed");}
//End Of The Failure 
if (pid > 0)
{
printf("Parent getting ready\\n");
while (!loopCheck)
{
}//End Of The 5 Loop Check
if(kill(pid,SIGUSR1 == -1))
{
perror(" Signal To Kill The Child Sent \\n");
}
wait(NULL);
printf("Child Has Been Killed \\n");
}//End Of The Parent

sleep(3);
printf("Child Is Now Running Rampant \n");
while (!keepRunning)
{
for ( ; i < 10; i++)
{
printf("Child Is Making A Mess Of The Terminal \n");
if(i==5)
{
if (kill(pid, SIGUSR2) == -1)
{perror("Making To Much Mess\n");}
}//End Of The 5 Loop Check
}//End Of For Loop
}//End Of While Loop
return 1;
}//End Of Main Method


