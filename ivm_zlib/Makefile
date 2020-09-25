go_bar: driver.s print_bar.s
	ivm as-run driver.s print_bar.s 

go_foo: driver.s print_foo.s
	ivm as-run driver.s print_foo.s 

driver.s: driver.c
	ivm64-gcc -o driver.s driver.c

print_bar.s: print_bar.c
	ivm64-gcc -c print_bar.c -o print_bar.s



default: payload.b

payload.s: payload.c
	ivm64-gcc -o payload.s payload.c

payload.b: payload.s
	ivm as payload.s

# Make a one-line header file that defines payload_size as the size in bytes of file payload.bz
payload_size.h: payload.b
	rm -f payload_size.h
	set -e ;\
	PSIZE=$$(stat --format '%s' payload.b) ;\
	printf 'int payload_size=' > payload_size.h ;\
	printf $$PSIZE >> payload_size.h ;\
	printf '\n' >> payload_size.h

test_zlib: test_compress
	./test_compress

test_compress: test_compress.c
	ivm64-gcc -I ./zlib -o test_compress test_compress.c ./zlib/libz.so

size: test_uncompress.b
	ls -lh test_uncompress.b

test_uncompress.b: test_uncompress.o
	ivm as test_uncompress.o

test_uncompress.o: test_uncompress.c Makefile
	ivm64-gcc -I ./zlib -o test_uncompress.o test_uncompress.c ./zlib/libz.so

# Build zlib (in sub-directory)
./zlib/zlib.so:
	bash build-zlib.sh

# Run manually without dependencies
get_zlib:
	wget http://www.zlib.net/zlib-1.2.11.tar.gz
	tar zxvf zlib-1.2.11.gz
	ln -s zlib-1.2.11 zlib

clean:
	rm -f test_compress test_compress.b test_compress.sym
	rm -f payload.s payload.b payload.sym
	rm -f payload_size.h
	rm -f driver.s # but don't remove print_foo.s since it's hand-written assembly
	rm -f print_bar.s print_bar.b print_bar.s

distclean:
	@echo NOTE: Remove zlib files manually
