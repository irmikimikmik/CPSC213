.pos 0x100
                 ld $0x1, r0              # r0 = 1
                 ld $u, r1                # r1 = address of u
                 ld $b, r2                # r2 = address of b
                 ld $p, r3                # r3 = address of p
                 ld $y, r4                # r4 = address of y
                 ld (r2), r2              # r2 = value of b
                 add r2, r0               # r0 = b + 1
                 ld (r1, r2, 4), r5       # r5 = u[b]
                 ld (r1, r0, 4), r6       # r6 = u[b+1]
                 add r5, r6               # r6 = u[b] + u[b + 1]
                 st r6, 0x0(r4)           # y = u[b] + u[b + 1]
                 ld $0xf0f0f0f, r7       # r7 = 0xf0f0f0f
                 and r7, r6               # r4 = y & 0xf0f0f0f
                 st r6, 0x0(r3)           # p = y & 0xf0f0f0f
                 halt                     # halt
.pos 0x1000
b:               .long 0x00000001         # b
.pos 0x2000
p:               .long 0x00000008         # p
.pos 0x3000
y:               .long 0x00000009         # y
.pos 0x4000
u:               .long 0x00000001         # u[0]
                 .long 0x00000009         # u[1]
                 .long 0x00000003         # u[2]
                 .long 0x00000004         # u[3]
                 .long 0x00000005         # u[4]