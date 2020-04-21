CCOPTS = -Xlinker -mdce 
CC = ivm64-gcc $(CCOPTS)

VMOPTS = -m 100000000
VM = ivm as-run $(VMOPTS)

run_in_ex: ex_image_input.out
	$(VM) --inp in ex_image_input.out

ex_image_input.out: ex_image_input.c ivm_io.o
	$(CC) -o $@ $< ivm_io.o

run_out_ex: ex_image_output.out
	$(VM) --out out ex_image_output.out

ex_image_output.out: ex_image_output.c ivm_io.o cleanout
	$(CC) -o $@ $< ivm_io.o

# run_old_out_ex: ex_image_output_standalone.out cleanout
# 	$(VM) --out out ./ex_image_output_standalone.out

ivm_io.o: ivm_io.c ivm_io.h
	$(CC) -c $<

# %.out: %.c
# 	$(CC) -o $@ $<

view:
	eog out/*.png

clean: cleanout
	rm -f *.b
	rm -f *.sym
	rm -f *.out

cleanout:
	mkdir -p out
	rm -f ./out/*
