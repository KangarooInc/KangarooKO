#include <stdio.h>
#include <time.h>

int main ()
{
  time_t seconds;

  while (1 == 1)
  {
  seconds = time(NULL);
      if(seconds%2 == 0)
      {
  printf("Hours since January 1, 1970 = %ld\n", seconds);
      }

  }

  return(0);
}
