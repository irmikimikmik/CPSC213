.pos 0x100
                 ld   $0x0, r0            # r0 = temp_i = 0
                 ld   $0x0, r1            # r1 = temp_c = 0
                 ld   $a, r2              # r2 = address of a[0]
                 ld   $b, r3              # r3 = address of b[0]
                 ld   $n, r4              # r4 = address of n
                 ld   (r4), r4            # r4 = n
                 not  r4                  # r4 = ! n
                 inc  r4                  # r4 = - n
loop:            mov  r4, r5              # r5 = - n
                 add  r0, r5              # r5 = temp_i-n
                 beq  r5, end_loop        # if i-n=0 goto +10
                 bgt  r5, end_loop        # if i-n>0 goto +9
                 ld   (r2, r0, 4), r6     # r6 = a[i]
                 ld   (r3, r0, 4), r7     # r7 = b[i]
                 not  r7                  # r7 = ! b[i]
                 inc  r7                  # r7 = - b[i]
                 add  r6, r7              # r7 = a[i]-b[i]
                 bgt  r7, then            # if a[i]-b[i]>0 goto +1
else:            br   end_if              # goto +1
then:            inc  r1                  # temp_c = temp_c+1
end_if:          inc  r0                  # temp_i = temp_i+1
                 br   loop                # goto - 11
end_loop:        ld   $i, r2              # r2 = address of i
                 st   r0, 0x0(r2)         # i = temp_i
                 st   r1, 0x54(r3)        # i = temp_i
                 halt                     
.pos 0x1000
i:               .long 0xffffffff         # i
n:               .long 0x00000005         # n
a:               .long 0x0000000a         # a[0]
                 .long 0x00000014         # a[1]
                 .long 0x0000001e         # a[2]
                 .long 0x00000028         # a[3]
                 .long 0x00000032         # a[4]
b:               .long 0x0000000b         # b[0]
                 .long 0x00000014         # b[1]
                 .long 0x0000001c         # b[2]
                 .long 0x0000002c         # b[3]
                 .long 0x00000030         # b[4]
c:               .long 0x00000000         # c