    gpc $10, r0          # store address of buffer in r0
    ld $7, r1            # load size to r1
    sys $2               # system call: execute program named by buffer
    halt
    .long 0x2f62696e     # \bin
    .long 0x2f736800     # \sh
