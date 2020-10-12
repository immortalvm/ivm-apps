C_FILES=uncompress.c xz_crc32.c xz_dec_lzma2.c xz_dec_stream.c
H_FILES=input.h uncompress.h xz.h xz_config.h xz_lzma2.h xz_private.h xz_stream.h

.PHONY: clean

boot_test: boot_test.c $(H_FILES) $(C_FILES)
	ivm64-gcc -Os -o boot_test boot_test.c $(C_FILES)

clean:
	rm -f boot_test.b boot_test.sym

distclean: clean
	rm -f boot_test
