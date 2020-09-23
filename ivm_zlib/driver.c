/* Drive code that calls assembly code */

#include <stdio.h>


/*
 
This works when calling (jump!-ing) to the result of assembling both the following two items:
a) the result of compiling this file (i.e., driver.c) to assembly
b) an hand-written assembly file that looks like this:

	EXPORT printfoo
printfoo:
	<code to be executed>


TODO:

The next step is to decompress the binary code at the call site before calling it.
How to use zlib to decompress form memory is shown in test_compress.c.
However, decompression happens at run-time, so how do we connect the decompressed code
to the currently executing code? The decompressed code resides in a piece of memory
returned by malloc, that is, it does not have a label at link-time.

Also, we do not want the decompressed code to self-contained by including lots of c-lib 
references. Instead such references in the decompressed code should be "linked" to the
non-decompressed c-lib instance (which is part of the decompressing code).

(And of course, the assembler is not there at run-time or in the future, so we need a
way to combine binary files for this to ever work...)

(Do we need a new VM instruction that sets part a chunk of memory and then jumps there
and starts executing it? )

NEED A WHITEBOARD TO FIGURE THIS OUT! In the future there is just driver.b so this has
to set apart space for the payload (compressed code on medium). 


Also, how do we avoid duplicating library (libc) stuff that is needed both by the
driver and by the decompressed code? Is this even possible without having a run-time
linking facility (which is not doable)?

Note that one can make the decompressed code self-extractable, thus avoiding the need
to dealing with code binary size in the driver, but the linking issue remains...

(Command line usage of zlib on Linux is possible via command  zlib-flate, it seems.)

*/

void doit() 
{
#ifdef __ivm64__
  asm volatile("jump! printbar");
#else
  #error "ERROR: This code only makes sense for ivm64"
#endif
}


int main(int argc, char* argv[])
{
  puts("Driver starting.");

  doit();

  puts("Driver stopping. (But this never gets printed.)");
}
