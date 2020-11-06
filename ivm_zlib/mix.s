### Example how to package a self-extracting program

### The "inner" program here is the result of 'ivm as echo.s', see 'payload'.
### When the current file is assembled, the assembler prepends 43 bytes that can
### be safely be removed as an additional optimization.

program:
    heap_start = (+ arg_location (load8 arg_location) 8)

### Copy program and argument onto stack
    set_sp! (+ &0 decompress -heap_start) # Make room
    push!!! decompress heap_start &0
    call! copy
    set_sp! &3
    push!! program (+ arg_location -payload)

### Jump to the copy of 'decompress' on the stack. This works since the
### assembler produces that only uses relative addressing.
    jump! &2

### Decompress
decompress:
    payload_size = (load8 (+ decompress -16))
    orig_program = (load8 (+ decompress -8))
    push! (+ &2 payload -decompress)
    push! (+ $0 payload_size)
    push!!! $1 $0 orig_program
    call! copy
    set_sp! $3

### Copy back arg
    decomp_size = payload_size  # Assuming 0% compression for now
    push!!! &0 (+ &0 $0 8) (+ orig_program decomp_size)
    call! copy
    set_sp! (+ &3 $3 8)

### Jump to the decompressed program
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

### Payload
### NB: This payload must not be too short.
payload:
    ## echo.b
    data1 [ 0x05 0x08 0x67 0x20 0x06 0x13 0x06 0x13 0x13 0x20 0x08 0x08 0x20 0x05 0x08 0x14 ]
    data1 [ 0x20 0x17 0x05 0x08 0x07 0x20 0x17 0x07 0x03 0x10 0x00 0x00 0x00 0x00 0x00 0x00 ]
    data1 [ 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x00 0x05 0x08 0x10 0x2a 0x20 0x13 ]
    data1 [ 0x08 0x08 0x20 0x06 0x13 0x05 0x08 0x1b 0x2a 0x20 0x13 0x13 0x20 0x06 0x13 0x06 ]
    data1 [ 0x08 0x10 0x20 0x13 0x07 0x2a 0x21 0x20 0x03 0x17 0x06 0x08 0x08 0x20 0x13 0x10 ]
    data1 [ 0xfa 0x06 0x08 0x08 0x20 0x13 0x08 0x01 0x20 0x06 0x08 0x10 0x20 0x17 0x07 0x03 ]
    data1 [ 0xdc 0x06 0x08 0x10 0x20 0x04 0x07 0x00 ]
arg_location:
