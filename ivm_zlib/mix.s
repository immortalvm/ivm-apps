### Example how to package a self-extracting program

### The "inner" program here is the result of 'ivm as echo.s', see 'payload'.
### When the current file is assembled, the assembler prepends 43 bytes that can
### be safely be removed as an additional optimization.

### Example: ivm as-run -a mix.s mix.s mix_payload.s

    IMPORT mix_payload/payload
    IMPORT mix_payload/arg_location

program:
    heap_start = (+ arg_location (load8 arg_location) 8)

### Copy program and argument onto stack
    set_sp! (+ &0 run_on_stack -heap_start) # Make room
    push!!! run_on_stack heap_start &0
    call! copy
    set_sp! &3
    push!! program (+ arg_location -payload)

### Jump to the copy of 'run_on_stack'. This works since the assembler produces
### binary that only uses relative addressing.
    jump! &2

run_on_stack:
    payload_size = (load8 (+ run_on_stack -16))
    orig_program = (load8 (+ run_on_stack -8))

### Decompress
    push! (+ &2 payload -run_on_stack) # source
    push! (+ $0 payload_size)          # source_stop
    push!!! $1 $0 orig_program
    call! decompress
    ## Stack: 0:<decompressed_stop> _ 2:<payload_stop> ...

### Copy back arg
    push!!! $2 (+ 8 $2 (load8 $2)) $0
    call! copy
    ## Stack: _ _ 2:<source_stop>

### Free stack and jump to the decompressed program
    set_sp! $2
    jump! orig_program

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

### ######################################################

decompress:
    push!!! $3 $2 $1
    call! copy
    store8! &4
    store8! &4
    store8! &4
    ## Stack: ret dest_stop _ source_stop ...
    return
