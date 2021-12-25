
.pos 0x100

    # a = 3;
    ld $3, r0           # r0 = 3
    ld $a, r1           # r1 = address of a
    st r0, (r1)         # a = 3

    # p = &a;
    ld $p, r2           # r2 = address of p
    st r1, (r2)         # p = address of a = &a

    # *p = *p - 1;
    ld (r2), r3         # r3 = address of a = &a
    ld (r3), r4         # r4 = (value of) a
    dec r4              # r4 = (value of) a - 1
    st r4, (r3)         # *p = *p - 1

    # p = &b[0];
    ld $b, r5           # r5 = address of b = address of b[0]
    st r5, (r2)         # p = address of b[0] = &b[0]
    
    # p++;
    ld (r2), r3         # r3 = address of b[0] = &b[0]
    inca r3             # r3 = address of b[0] + 4 = p++
    st r3, (r2)         # p++;

    # p[a] = b[a];
    ld (r1), r1         # r1 = (value of) a
    ld (r5, r1, 4), r6  # r6 = b[a]
    st r6, (r3, r1, 4)  # p[a] = b[a]

    # *(p+3) = b[0];  !!! *(p+3) = p[3]
    ld (r2), r2         # r2 = (value of) p
    ld 0(r5), r6        # r6 = b[0]
    ld $12, r3          # r3 = 12
    add r3, r2          # r3 = (value of) p + 12
    st r6, (r2)         # *(p+3) = b[0]

    halt

.pos 0x500
# Data area

a:  .long 1             # a
p:  .long 2             # p
b:  .long 1             # b[0]
    .long 2             # b[1]
    .long 3             # b[2]
    .long 4             # b[3]
    .long 5             # b[4]