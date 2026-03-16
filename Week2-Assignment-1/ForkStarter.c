#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int i =0;
int main(int argc, char *argv[])
{
printf("Starting The Fork Proccess, Making Parent And Child");
pid_t pid = fork();
if(pid < 0)
{perror("Fork Has Failed");}
if (pid == 0)
{workMethod("child");
exit(0);}
workMethod"parent");
wait(NULL);
}//End Of Main Method

void workMethod(char  *name)
{
const int repeat = 10;
for ( ; i < repeat; i++)
{
if (name == "child")
{
sleep(1);
printf("Passing Child Proccess Currently At # %s\n", name) ;
}else
{
sleep(2);
printf("Passing Child Proccess Currently At # %s\n", name);
}//End Of Child Parent If
}//End Of For Loop
}//End Of Work Method




