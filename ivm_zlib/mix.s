
program:
    heap_start = (load8 (+ program -8))
    arg_location = (load8 (+ program -16))
    arg_tot = (+ heap_start -arg_location)

### Copy compressed and arg onto stack
    set_sp! (+ &0 payload -heap_start) # Make room
    push!!! payload heap_start &0
    call! copy
    set_sp! &3

### Decompress
    payload_length = (+ arg_location -payload)
    push!!! &0 (+ &0 payload_length) payload
    call! copy
    set_sp! (+ &3 payload_length)

### Copy back arg
    new_arg_loc = arg_location # No decompression yet
    push!!! &0 (+ &0 arg_tot) new_arg_loc
    call! copy
    set_sp! (+ &3 arg_tot)

    jump! payload

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


### Payload

payload:
    put_char! (+ 48 9)          # Print 9
    put_char! 10                # Print <newline>
    exit! 7

