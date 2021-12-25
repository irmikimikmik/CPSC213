.pos 0x100
start:
    ld   $sb, r5         # sp = address of last word of stack
    inca r5              # sp = address of word after stack
    gpc  $6, r6          # r6 = pc + 6
    j    main            # call main()
    halt

f:
    deca r5              # allocate the local variables of the f frame on stack
    ld   $0, r0          # r0 = 0 = c
    ld   4(r5), r1       # r1 = x[b] = a
    ld   $0x80000000, r2 # r2 = 0x80000000
f_loop:
    beq  r1, f_end       # if x[b]=0 goto +8 (f_end)
    mov  r1, r3          # r3 = r1 = a
    and  r2, r3          # r3 = a & 0x80000000
    beq  r3, f_if1       # if r3=0 goto +2 (f_if1)
    inc  r0              # c++
f_if1:
    shl  $1, r1          # a = a * 2 = a << 1
    br   f_loop          # goto -8 (f_loop)
f_end:
    inca r5              # deallocate the local variables of the f frame on stack
    j    (r6)            # return

main:
    deca r5              # allocate the return address on stack
    deca r5              # allocate the arguments of main on stack
    st   r6, 4(r5)       # store return address to stack
    ld   $8, r4          # r4 = 8 = b
main_loop:
    beq  r4, main_end    # if b=0 goto +12 (main_end)
    dec  r4              # b--
    ld   $x, r0          # r0 = &x
    ld   (r0, r4, 4), r0 # r0 = x[b]
    deca r5              # allocate the arguments of the f frame on stack
    st   r0, (r5)        # store value of x[b] to stack
    gpc  $6, r6          # set return address
    j    f               # call f();
    inca r5              # deallocate the arguments of the f frame on stack
    ld   $y, r1          # r1 = &y
    st   r0, (r1, r4, 4) # y[b] = x[b]
    br   main_loop       # goto -12 (main_loop)
main_end:
    ld   4(r5), r6       # load ra from stack
    inca r5              # deallocate the arguments of main on stack
    inca r5              # deallocate the return address on stack
    j    (r6)            # return

.pos 0x2000
x:
    .long 1
    .long 2
    .long 3
    .long -1
    .long -2
    .long 0
    .long 184
    .long 340057058

y:
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0

.pos 0x8000
# These are here so you can see (some of) the stack contents.
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
    .long 0
sb: .long 0
