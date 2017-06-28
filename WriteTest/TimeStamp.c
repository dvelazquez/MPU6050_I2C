/**
 * Get time difference in microseconds
 * */
  
#include <stdio.h>
#include <sys/time.h>
  
double time_diff(struct timeval x , struct timeval y);
  
int main()
{  
    int i;
     
    struct timeval before , after;
    gettimeofday(&before , NULL);
   
    //Time taking task
    for (i=1 ; i <= 100 ; i++)
    {
        printf("%d %d %d n",i, i*i, i*i*i);
    }
   
    gettimeofday(&after , NULL);
 
    printf("Total time elapsed : %.0lf us" , time_diff(before , after) ); 
   
    return 0;
}
 
double time_diff(struct timeval x , struct timeval y)
{
    double x_ms , y_ms , diff;
     
    x_ms = (double)x.tv_sec*1000000 + (double)x.tv_usec;
    y_ms = (double)y.tv_sec*1000000 + (double)y.tv_usec;
     
    diff = (double)y_ms - (double)x_ms;
     
    return diff;
}
