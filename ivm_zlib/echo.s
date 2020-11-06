### Example program which outputs each byte in the argument file as a character,
### i.e. one UNICODE character between 0 and 255 (not UTF-8!).

EXPORT main
main:
    arg_location = (load8 (+ main -16))
    arg_length = (load8 arg_location)
    arg_start = (+ arg_location 8)

    push! arg_start
    push! (+ $0 arg_length)
loop:
    push! (+ $0 -$1)
    jump_zero! end
    put_char! (load1 $1)
    store8!! (+ $1 1) &1
    jump! loop
end:
    set_sp! &2
    exit! 0
