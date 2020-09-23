/* Program to be stored as compressed code */

#include <stdio.h>

void payload() 
{
  puts("The sky above the port was the color of television, tuned to a dead channel.");
}


/*
I would like to to not include main(), after compiling with "-c" the assembler complains about missing symbols "puts"
*/


int main(int argc, char* argv[])
{
  payload();
  
  return 0;
}
