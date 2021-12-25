.pos 0x100

    # tmp = 0;
    ld $tmp, r0         # r0 = address of tmp
    ld $0, r1           # r1 = 0
    st r1, (r0)         # tmp = 0

    # tos = 0;
    ld $tos, r2         # r2 = address of tos
    st r1, (r2)         # tos = 0

    # s[tos] = a[0];
    ld $s, r3           # r3 = address of s
    ld $a, r4           # r4 = address of a
    ld (r2), r2         # r2 = (value of) tos
    ld (r4, r1, 4), r5  # r5 = a[0]
    st r5, (r3, r2, 4)  # s[tos] = a[0]

    # tos++;;
    ld $1, r6           # r6 = 1
    add r6, r2          # r2 = tos + 1
    ld $tos, r1         # r1 = address of tos
    st r2, (r1)         # tos = tos + 1

    # s[tos] = a[1];
    ld (r4, r6, 4), r5  # r5 = a[1]
    st r5, (r3, r2, 4)  # s[tos] = a[1]

    # tos++;;
    add r6, r2          # r2 = tos + 1
    st r2, (r1)         # tos = tos + 1

    # s[tos] = a[2];
    ld 8(r4), r5        # r5 = a[2]
    st r5, (r3, r2, 4)  # s[tos] = a[1]

    # tos++;
    add r6, r2          # r2 = tos + 1
    st r2, (r1)         # tos = tos + 1

    # tos--;
    dec r2              # r2 = tos - 1
    st r2, (r1)         # tos = tos - 1

    # tmp = s[tos];
    ld (r3, r2, 4), r5  # r5 = s[tos]
    st r5, (r0)         # tmp = s[tos]

    # tos--;
    dec r2              # r2 = tos - 1
    st r2, (r1)         # tos = tos - 1

    # tmp = tmp + s[tos];
    ld (r3, r2, 4), r5  # r5 = s[tos]
    ld (r0), r3         # r3 = (value of) tmp
    add r5, r3          # r3 = tmp + s[tos]
    st r3, (r0)         # tmp = tmp + s[tos]

    # tos--;
    dec r2              # r2 = tos - 1
    st r2, (r1)         # tos = tos - 1

    # tmp = tmp + s[tos];
    ld $s, r3           # r3 = address of s
    ld (r3, r2, 4), r5  # r5 = s[tos]
    ld (r0), r6         # r6 = (value of) tmp
    add r5, r6          # r6 = tmp + s[tos]
    st r6, (r0)         # tmp = tmp + s[tos]

    halt

.pos 0x500
# Data area

a:  .long 3             # a[0]
    .long 2             # a[1]
    .long 3             # a[2]
s:  .long 3             # s[0]
    .long 4             # s[1]
    .long 5             # s[2]
    .long 6             # s[3]
    .long 7             # s[4]
tos:.long 2             # tos
tmp:.long 2             # tmp