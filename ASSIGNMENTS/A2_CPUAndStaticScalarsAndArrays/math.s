.pos 0x100
                 ld $0x1, r0              # r0 = 1
                 ld $0x4, r1              # r1 = 4
                 ld $v, r2                # r2 = address of v
                 ld $l, r3                # r3 = address of l
                 ld 0x0(r3), r4           # r4 = l
                 add r4, r0               # r0 = l + 1
                 add r1, r0               # r0 = l + 1 + 4
                 shl $7, r0               # r0 = (l + 1 + 4) << 7
                 and r4, r0               # r0 = ((l + 1 + 4 )<< 7) & l
                 shr $2, r0               # r0 = (((l + 1 + 4 )<< 7) & l) >> 2 (= dividing by 4)
                 st r0, 0x0(r2)           # v = r0
                 halt                     # halt
.pos 0x1000
v:               .long 0x00000000         # v
.pos 0x2000
l:               .long 0x0000000a        # l