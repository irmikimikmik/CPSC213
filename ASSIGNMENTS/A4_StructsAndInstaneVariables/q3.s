.pos 0x1000
code:
                # v0 = s.x[i];
                ld $s, r0                   # r0 = address of s
                ld $i, r1                   # r1 = address of i
                ld (r1), r1                 # r1 = i
                ld (r0, r1, 4), r2          # r2 = s.x[i]
                ld $v0, r3                  # r3 = address of v0
                st r2, (r3)                 # v0 = s.x[i]

                # v1 = s.y[i];
                ld $v1, r4                  # r4 = address of v1
                ld $i, r1                   # r1 = address of i
                ld 8(r0), r3                # r3 = (value of) address of s + 8 (which is the address of s.y[0])
                ld (r1), r1                 # r1 = i
                ld (r3, r1, 4), r5          # r5 = s.y[i]
                st r5, (r4)                 # v1 = s.y[i]

                # v2 = s.z->x[i];
                ld $v2, r3                  # r3 = address of v2
                ld 12(r0), r2               # r2 = value stored at adress of i + 16 (offset of s.z)
                ld $i, r1                   # r1 = address of i
                ld (r1), r1                  # r1 = i
                ld (r2, r1, 4), r4          # r4 = s.z->x[i]
                st r4, (r3)                 # v2 = s.z->x[i]

                #v3 = s.z->z->y[i];
                ld $v3, r3                  # r3 = address of v3
                ld 12(r0), r2               # r2 = value stored at adress of i + 16 (offset of s.z)
                ld 12(r2), r4               # r4 = s.z->z
                ld 8(r4), r4                # r4 = s.z->z->y[0]
                ld $i, r1                   # r1 = address of i
                ld (r1), r1                 # r1 = i
                ld (r4, r1, 4), r4          # r4 = s.z->z->y[i]
                st r4, (r3)                 # v3 = s.z->z->y[i]

                halt
.pos 0x2000
static:
i:              .long 0x00000001            # i
v0:             .long 0x00000002            # v0
v1:             .long 0x00000003            # v1
v2:             .long 0x00000004            # v2
v3:             .long 0x00000005            # v3
s:              .long 0x00000006            # s.x[0]
                .long 0x0000000b            # s.x[1]
                .long s_y                   # s.y
                .long s_z                   # s.z
.pos 0x3000
heap:
s_y:            
                .long 0x00000001            # s.y[0]
                .long 0x00000002            # s.y[1]
s_z:            
                .long 0x00000003            # s.z.x[0]
                .long 0x00000004            # s.z.x[1]
                .long 0x00000005            # s.z.y
                .long s_z_z                 # s.z->z
s_z_z:          
                .long 0x00000006            # s.z->x[0]
                .long 0x00000007            # s.z->x[1]
                .long s_z_z_y               # s.z->y
                .long 0x00000008            # s.z->z
s_z_z_y:
                .long 0x00000009            # s.z->z->y[0]
                .long 0x0000000a            # s.z->z->y[1]
