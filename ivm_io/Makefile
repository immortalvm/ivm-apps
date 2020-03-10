run_ex: ex_image_output.out
	ivm as-run --out out -m 100000000 ex_image_output.out

ex_image_output.out: ex_image_output.c ivm_io.o cleanout
	ivm64-gcc -o $@ $< ivm_io.o

run_oldex: ex_image_output_standalone.out cleanout
	ivm as-run --out out -m 1000000000 ./ex_image_output_standalone.out

ivm_io.o: ivm_io.c ivm_io.h
	ivm64-gcc -c $<

%.out: %.c
	ivm64-gcc -o $@ $<

view:
	eog out/*.png

cleanout:
	mkdir -p out
	rm -f ./out/*
