.pos 0x100
                 ld $array, r0            # r0 = address of array
                 ld $0x1, r1              # r1 = 1
                 ld $0x3, r2              # r2 = 3
                 ld (r0, r1, 4), r3       # r3 = array[1]
                 ld (r0, r2, 4), r4       # r4 = array[3]
                 st r3, (r0, r2, 4)       # address of array[3] = array[1]
                 st r4, (r0, r1, 4)       # address of array[1] = array[3]
                 halt                     # halt
.pos 0x2000
array:           .long 0xffffffff         # array[0]
                 .long 0x00000001         # array[1]
                 .long 0xffffffff         # array[2]
                 .long 0x00000003         # array[3]
                 .long 0xffffffff         # array[4]
                 .long 0xffffffff         # array[5]