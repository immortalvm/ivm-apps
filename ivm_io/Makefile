CCOPTS = -Xlinker -mdce 
CC = ivm64-gcc $(CCOPTS)

VMOPTS = -m 100000000
VM = ivm as-run $(VMOPTS)

AS = ivm as

# C VM must be in path (.../ivm-implementations/OtherMachines/vm)
CVM = vm $(VMOPTS)


run_ex_in: ex_image_input.out
	$(VM) --inp in ex_image_input.out

crun_ex_in: ex_image_input.b
	$(CVM) -i in $<

ex_image_input.out: ex_image_input.c ivm_io.o
	$(CC) -o $@ $< ivm_io.o

run_ex_out: ex_image_output.out
	$(VM) --out out ex_image_output.out

ex_image_output.out: ex_image_output.c ivm_io.o cleanout
	$(CC) -o $@ $< ivm_io.o

%.b : %.out
	$(AS) $<

ivm_io.o: ivm_io.c ivm_io.h
	$(CC) -c $<

view:
	eog out/*.png

clean: cleanout
	rm -f *.b
	rm -f *.sym
	rm -f *.out

cleanout:
	mkdir -p out
	rm -f ./out/*
