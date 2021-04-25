
#
# ghostscript notes:
#   make so - Buils Ghostscript API library
#   make sodebug - Include debug features

noinst_PROGRAMS = static_pdf

AM_CFLAGS = -DDEBUG=1 -g -O0
static_pdf_CFLAGS = \
     $(AM_CFLAGS) \
     $(CFLAGS) \
     -I../../file-format-decoders/include \
     -L../../file-format-decoders/lib \
     -L../../ivm-ghostscript/sobin \
     -I$(LIBTESTDATA_DIR)/pdf \
     -I${top_srcdir}/../ivm-ghostscript/ghostscript-9.52/psi \
     -I${top_srcdir}/../ivm-ghostscript/ghostscript-9.52/base

static_pdf_LDADD = \
     -lgs -lz -ljpeg -lm

static_pdf_SOURCES = \
     main.c

check_PROGRAMS = static_pdf

static_pdf.b: static_pdf
	ivm as static_pdf

ivmasm: static_pdf.b

ivmrun: ivmasm
	vm -m $$((1000*1000*1000)) static_pdf.b

ivmrunemu: ivmasm
	../../../../../yet_another_ivm_emulator/ivm_emu -m $$((1000*1000*1000)) static_pdf.b
