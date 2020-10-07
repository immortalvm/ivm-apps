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

  /* Compressed a[] as bytes: */

  /*
  const Bytef ca[] = { 0x78, 0x9C, 0xCB, 0x48, 0xCD, 0xC9, 0xC9, 0xD7, 0x51, 0xC8,
                       0x80, 0x50, 0xE5, 0xF9, 0x45, 0x29, 0x95, 0x20, 0x32, 0x27, 
                       0x45, 0x51, 0x21, 0x38, 0x35, 0x55, 0xA1, 0x32, 0x31, 0x43,
                       0x8F, 0x0B, 0x00, 0xED, 0x36, 0x0C, 0x36 };
  */


  const Bytef ca[] = { 0x78, 0x9C, 0xCB, 0x48, 0xCD, 0xC9, 0xC9, 0xD7, 0x51, 0xC8,
                       0x80, 0x50, 0xE5, 0xF9, 0x45, 0x29, 0x95, 0x20, 0x32, 0x27,
                       0x45, 0x51, 0x21, 0x38, 0x35, 0x55, 0xA1, 0x32, 0x31, 0x43,
                       0x0F, 0x00, 0xE1, 0x00, 0x0C, 0x2C };
  
  /* Produced as follows:
     % echo -n "hello, hello, wordy world! See yah." | zlib-flate -compress
  */


  const uLongf caLen = sizeof(ca);
  

  int stat;

  /* printf("aLen = %ld\n", aLen); */
  /* printf("caLenBound = %ld\n", caLenBound); */


  /* stat = compress(ca_p, &caLen, a, aLen); */

  /* if (stat == Z_OK)  */
  /*   { */
  /*     puts("1 - compress: success"); */
  /*     printf("caLen = %ld\n", caLen); */
  /*   } */
  /* else  */
  /*   { */
  /*     fputs("1- compress: fail\n", stderr); */
  /*     fprintf(stderr, "return: %d\n", stat); */
  /*     exit(1); */
  /*   } */

  Bytef* ua_p = malloc (sizeof(Bytef)*aLen);
  uLongf uaLen = aLen;
  

  stat = uncompress(ua_p, &uaLen, ca, caLen);

  if (stat == Z_OK) 
    {
      puts("2 - uncompress: success");
      printf("uaLen = %ld\n", uaLen);
    }
  else 
    {
      fputs("2 - uncompress: FAIL\n", stderr);
      fprintf(stderr, "return: %d\n", stat);
      exit(1);
    }


  puts("Payload bytes - start:[");
  
  for (int i = 0; i < uaLen; i++ ) 
    printf("%#02x, ", ua_p[i]);

  puts("]:stop - Payload bytes.");

  printf("Payload  string: [%s]\n, ", ua_p);
  printf("Original string: [%s]\n, ", a);


  if (! strcmp(a, ua_p)) 
    {
      puts("2 - uncompress: identical");
    }
  else
    {
      fputs("2 - uncompress: NOT identical\n", stderr);
      exit(1);
    }
  
  return 0;
}
