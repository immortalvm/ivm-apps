# The executable to embed
TARGET = ivmboot.b

C_FILES=trivlib.c decompress.c xz_crc32.c xz_dec_lzma2.c xz_dec_stream.c
H_FILES=input.h trivlib.h decompress.h xz.h xz_config.h xz_lzma2.h xz_private.h xz_stream.h

.PHONY: all clean distclean

all: mix.b

mix.b: input.h mix.s self_extract
	ivm as mix.s self_extract

input.h: $(TARGET)
	./gen_input $(TARGET)

self_extract: self_extract.c $(H_FILES) $(C_FILES)
	ivm64-gcc -Os -o self_extract self_extract.c $(C_FILES) -DXZ_DEC_SINGLE

clean:
	rm -f self_extract self_extract.b self_extract.sym input.h mix.sym

distclean: clean
	rm -f mix.b
