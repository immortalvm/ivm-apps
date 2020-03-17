CCOPTS = -Xlinker -mdce 
CC = ivm64-gcc $(CCOPTS)


run_ex: ex_image_output.out
	ivm as-run --out out -m 100000000 ex_image_output.out

ex_image_output.out: ex_image_output.c ivm_io.o cleanout
	$(CC) -o $@ $< ivm_io.o

run_oldex: ex_image_output_standalone.out cleanout
	ivm as-run --out out -m 1000000000 ./ex_image_output_standalone.out

ivm_io.o: ivm_io.c ivm_io.h
	$(CC) -c $<

%.out: %.c
	$(CC) -o $@ $<

view:
	eog out/*.png

clean: cleanout
	rm -f *.b
	rm -f *.sym
	rm -f *.out

cleanout:
	mkdir -p out
	rm -f ./out/*
