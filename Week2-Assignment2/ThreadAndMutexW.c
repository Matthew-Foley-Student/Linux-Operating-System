#include <stdio.h>
#include <pthread.h>

#define NUM_LOOPS 1000000
long long sum = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

//------------------------------------------------------------------------------------
void* add_to_bank_p(void *arg)
{
int offset = *(int *) arg;
for (int i =0; i< NUM_LOOPS; i++){
pthread_mutex_lock(&mutex);
sum += offset;
pthread_mutex_unlock(&mutex);
}//End Of The For Loop
pthread_exit(NULL);
}//End Of Adding To Bank Method

//----------------------------------------------------------------------------------------
void* add_to_bank_n(void *arg)
{
int offset = *(int *) arg;
for (int i =0; i< NUM_LOOPS; i++){
pthread_mutex_lock(&mutex);
sum -= offset;
pthread_mutex_unlock(&mutex);
}//End Of The For Loop
pthread_exit(NULL);
}//End Of Adding To Bank Method

//--------------------------------------------------------------------------------------
int main(void)
{
printf("The Starting Amount In The Bank Is $%lld\n", sum);
pthread_t id1;
int offset1 = 1;
pthread_t id2;
int offset2 = 2;
pthread_t id3;
int offset3 = 1;
pthread_t id4;
int offset4 = 1;

pthread_create(&id1, NULL, add_to_bank_p, &offset1);
pthread_create(&id2, NULL, add_to_bank_p, &offset2);
pthread_create(&id3, NULL, add_to_bank_n, &offset3);
pthread_create(&id4, NULL, add_to_bank_n, &offset4);
pthread_join(id1, NULL);
pthread_join(id2, NULL);
pthread_join(id3, NULL);
pthread_join(id4, NULL);

printf("The Amount In The Bank Is Now $%lld\n", sum);
return 0;
}//End Of The Main Method
