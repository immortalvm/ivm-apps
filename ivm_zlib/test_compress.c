/* Test zlib */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include "zlib.h"


int main(int argc, char* argv[])
{
  const Bytef a[] = "hello, hello, wordy world! See yah."; // size = 36 bytes on x86
  const uLongf aLen = sizeof(a);

  uLongf caLenBound = compressBound(aLen), caLen;
  Bytef* ca_p = malloc (sizeof(Bytef)*caLenBound);

  int stat;

  printf("aLen = %ld\n", aLen);
  printf("caLenBound = %ld\n", caLenBound);


  stat = compress(ca_p, &caLen, a, aLen);

  if (stat == Z_OK) 
    {
      puts("1 - compress: success");
      printf("caLen = %ld\n", caLen);
    }
  else 
    {
      fputs("1- compress: fail\n", stderr);
      fprintf(stderr, "return: %d\n", stat);
      exit(1);
    }

  uLongf uaLen = aLen;
  Bytef* ua_p = malloc (sizeof(Bytef)*uaLen);
  

  stat = uncompress(ua_p, &uaLen, ca_p, caLen);

  if (stat == Z_OK) 
    {
      puts("2 - uncompress: success");
      printf("uaLen = %ld\n", uaLen);
    }
  else 
    {
      fputs("2- uncompress: fail\n", stderr);
      fprintf(stderr, "return: %d\n", stat);
      exit(1);
    }

  if (! strcmp(a, ua_p)) 
    {
      puts("2 - compress, then uncompress: identical");
    }
  else
    {
      fputs("2- compress, then uncompress: NOT identical\n", stderr);
      exit(1);
    }
  
  return 0;
}
