C_FILES=trivlib.c uncompress.c xz_crc32.c xz_dec_lzma2.c xz_dec_stream.c
H_FILES=input.h trivlib.h uncompress.h xz.h xz_config.h xz_lzma2.h xz_private.h xz_stream.h

.PHONY: clean

self_extract: self_extract.c $(H_FILES) $(C_FILES)
	ivm64-gcc -Os -o self_extract self_extract.c $(C_FILES) -DXZ_DEC_SINGLE

clean:
	rm -f self_extract.b self_extract.sym

distclean: clean
	rm -f self_extract
