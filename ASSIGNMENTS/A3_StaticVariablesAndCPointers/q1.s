.pos 0x100

    # b  = h[5];
    ld $h, r0           # r0 = address of h
    ld $b, r1           # r1 = address of b
    ld 20(r0), r2       # r2 = h[5]
    st r2, (r1)         # b = h[5]

    # b  = h[b];
    ld (r1), r2         # r2 = (value of) b
    ld (r0, r2, 4), r3  # r3 = h[b]
    st r3, (r1)         # b = h[b]

    # e = &r;
    ld $e, r3           # r3 = address of e
    ld $r, r4           # r4 = address of r = &r
    st r4, (r3)         # e = &r

    # *e = 4;
    ld $4, r4           # r4 = 4
    ld (r3), r3         # r3 = address of r
    st r4, (r3)         # *e = 4

    # e = &h[h[3]];
    ld $3, r1           # r1 = 3
    ld $e, r2           # r2 = address of e
    ld (r0, r1, 4), r3  # r3 = h[3]
    shl $2, r3          # r3 = 4 * h[3]
    ld $h, r5           # r5 = address of h
    add r5, r3          # r3 = address of h + 4 * h[3]
    st r3, (r2)         # e = &h[h[3]]

    # *e = *e + h[8];
    ld (r2), r1         # r1 = (value of) e = address of h[h[3]]
    ld (r2), r6         # r6 = (value of) e = address of h[h[3]]
    ld (r1), r1         # r1 = (value of) h[h[3]] = e*
    ld $8, r2           # r2 = 8
    ld (r0, r2, 4), r3  # r3 = h[8]
    add r3, r1          # r1 = e* + h[8]
    st r1, (r6)         # *e = *e + h[8]

    halt

.pos 0x500
# Data area

r:  .long 0             # r
b:  .long 1             # b
h:  .long 0             # h[0]
    .long 0             # h[1]
    .long 3             # h[2]
    .long 4             # h[3]
    .long 1             # h[4]
    .long 2             # h[5]
    .long 0             # h[6]
    .long 0             # h[7]
    .long 7             # h[8]
    .long 0             # h[9]
e:  .long 0             # e