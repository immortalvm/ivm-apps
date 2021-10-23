# The executable to embed
# TARGET = ivmboot.b

if IVM_COMPILE

VM=vm
TARGET = $(top_builddir)/initial-program/ivmip.b
static_TARGET=$TARGET

bin_PROGRAMS = self_extract

BUILT_SOURCES = input.h

C_FILES=trivlib.c decompress.c xz_crc32.c xz_dec_lzma2.c xz_dec_stream.c
H_FILES=trivlib.h decompress.h xz.h xz_config.h xz_lzma2.h xz_private.h xz_stream.h

self_extract_SOURCES = $(C_FILES) $(H_FILES) self_extract.c
nodist_self_extract_SOURCES = input.h
self_extract_DEPENDENCIES = mix.s $(TARGET)
self_extract_CFLAGS = $(AM_CFLAGS) -Os -DXZ_DEC_SINGLE

.PHONY: distclean $(TARGET)

all-local: self_extract.b

self_extract.b: input.h mix.s self_extract
	ivm as --bin self_extract.b --sym self_extract.sym $(srcdir)/mix.s self_extract 

input.h: $(TARGET)
	$(srcdir)/gen_input $(TARGET)

clean-local:
	rm -f self_extract self_extract.b self_extract.sym input.h

distclean-local: clean
	rm -f self_extract.b

test: self_extract.b
	$(VM) self_extract.b -o out -i $(TESTDATA_DIR) -m 1073741824

endif
