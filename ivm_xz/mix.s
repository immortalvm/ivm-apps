### Prefix assembly program for boot_test self-extractor

program:

### Copy the pointers to the heap and the argument
    argument_location = (load8 (+ program -16))
    store8!! argument_location (+ __start -16)
    store8!! (load8 (+ program -8)) (+ __start -8)

### Copy argument to the stack
    argument_size = (+ 8 (load8 argument_location))
    set_sp! (+ &0 -argument_size)
    push!!! argument_location (+ argument_location argument_size) &0
    call! copy

### Make room for the decompressed code
    set_sp! (+ &3 -(load4 decompressed_size)) # Also clean up after copy
    store8!! &0 output

### Decompress (taken from __start, thus contents of __start can be deleted)
    push!!!! 0 0 0 0
	call! _start

### Run copy of 'replacer' on the stack
    replacer_size = (+ decompressed -replacer)
	set_sp! (+ &4 -replacer_size) # Also clean up after _start
    push!!! replacer decompressed &0
    call! copy
    set_sp! &3
    push! (+ (load4 decompressed_size) argument_size) # size2
    push! program                                     # program2
    jump! &2                    # Jump to 'replacer' on stack

replacer:
    size2 = (load8 (+ replacer -8))
    program2 = (load8 (+ replacer -16))

### Copy decompressed code and argument
    push!!! decompressed (+ decompressed size2) program2
    call! copy

### Clean up and run decompressed code
    set_sp! (+ &5 replacer_size size2)

    jump! program2              # Executed from deallocated stack memory

### Helper
copy:
    ## Stack: <ret> i=dest j=source_stop k=source ...
    i = 1
    j = 2
    k = 3
    store8!! -$j &j             # Minor optimization
copy_loop:
    jump_zero!! (+ $j $k) copy_done
    store1!! (load1 $k) $i
    store8!! (+ $i 1) &i
    store8!! (+ $k 1) &k
    jump! copy_loop
copy_done:
    return

decompressed:
    ## On the stack the uncompressed code starts here.
    ## The comes the argument.

### Pointer copies
    data8 [0 0]
