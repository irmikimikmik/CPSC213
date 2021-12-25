.pos 0x100

                ld  $n, r0              # r0 = &n
                ld  (r0), r0            # r0 = n
                ld  $0, r1              # r1 = 0 --> iteration count = i
                ld  $s, r2              # r2 = &s
                ld  (r2), r2            # r2 = s
                
    loop:       mov r1, r3              # r3 = i
                not r3                  # r3 = ~ i
                inc r3                  # r3 = ~ i + 1 = - i
                add r0, r3              # r3 = n - i
                beq r3, end_loop        # if ((n - i) == 0), end_loop
                mov r1, r3              # r3 = r1
                shl $4, r3              # r3 = 16 * r1
                mov r1, r4              # r4 = r1
                shl $3, r4              # r4 = 8 * r1
                add r4, r3              # r3 = 16 * r1 + 8 * r1 = 24 * r1
                add r2, r3              # r3 = s[i]
                ld  4(r3), r4           # r4 = s[0].grade[0]
                ld  8(r3), r5           # r5 = s[0].grade[1]
                add r5, r4              # r4 = r4 + r5
                ld  12(r3), r5          # r5 = s[0].grade[2]
                add r5, r4              # r4 = r4 + r5
                ld  16(r3), r5          # r5 = s[0].grade[3]
                add r5, r4              # r4 = sum of grades
                shr $2, r4              # r4 = (sum of grades)/4
                st  r4, 20(r3)          # s[0].average = r4
                inc r1                  # i = i + 1
                br loop                 # goto loop
    end_loop:   
                # All the averages are calculated and stored
                ld  $0, r1              # r1 = 0 --> step 
bubble_for_1:   mov r1, r4
                not r4
                inc r4                  # r4 = - r1
                ld  $n, r0              # r0 = &n
                ld  (r0), r0            # r0 = n
                dec r0                  # r0 = n - 1 = size - 1
                add r0, r4              # r4 = size - step - 1
                beq r4, end_bubble      # if (0 == size - step - 1), goto end_bubble
                ld  $0, r5              # r5 = 0 = i
    bubble_for_2:
                    mov r5, r6
                    not r6
                    inc r6                  # r6 = - i
                    add r4, r6              # r6 = size - step - i - 1
                    beq r6, end_bubble_for  # if (0 == size - step - i - 1), goto end_bubble_for
                    mov r5, r0              # r0 = r5
                    shl $4, r0              # r0 = 16 * r5
                    mov r5, r6              # r6 = r5
                    shl $3, r6              # r6 = 8 * r5
                    add r6, r0              # r0 = 16 * r5 + 8 * r5 = 24 * r5
                    add r2, r0              # r0 = s[r5] = s[i] 
                    ld  $24, r6             # r6 = 24
                    add r0, r6              # r6 = s[r5 + 1] = s[i + 1]
        comp_avg:   # compares averages of r0 and r6
                    ld  20(r0), r3          # r3 = avg1 
                    ld  20(r6), r7          # r7 = avg2
                    not r7                  # r7 = ~ avg2
                    inc r7                  # r7 = ~ avg2 + 1 = - avg2
                    add r3, r7              # r7 = avg1 - avg2
                    bgt r7, res_2           # if (r7 > 0), res_2
                    inc r5
                    br  bubble_for_2
            res_2:  # swaps r0 and r6
                    ld  $6, r6              # r6 = 6
            loop_2:     beq r6, end_loop_2  # if (r6 == 0), goto end_loop_2
                        ld  (r0), r3        # r3 = r0
                        ld  24(r0), r7      # r7 = same information of next student
                        st  r7, (r0)        # swap
                        st  r3, 24(r0)      # swap
                        inca r0             # go to next field
                        dec r6
                        br  loop_2
            end_loop_2: 
                        inc r5
                        br  bubble_for_2

        end_bubble_for:
                    inc r1
                    br bubble_for_1
end_bubble:
        # the students are sorted according to their average
                    ld  $n, r0              # r0 = &n
                    ld  (r0), r0            # r0 = n
                    inc r0                  # r0 = n + 1
                    shr $1, r0              # r0 = (n + 1)/2 = median
                    dec r0
                    mov r0, r1              # r1 = r0
                    shl $4, r1              # r1 = 16 * r0
                    mov r0, r3              # r6 = r0
                    shl $3, r3              # r6 = 8 * r0
                    add r3, r1              # r1 = 16 * r0 + 8 * r0 = 24 * r0
                    add r2, r1              # r1 = s[r1]
                    ld  (r1), r1            # r1 = s[r1].sid
                    ld  $m, r4              # r4 = &m
                    st  r1, (r4)            # m = r1
                    halt





.pos 0x2000
n:              .long 5       # just one student
m:              .long 0       # put the answer here
s:              .long base    # address of the array

.pos 0x3000
base:           .long 1111    # student ID
                .long 11      # grade 0
                .long 11      # grade 1
                .long 11      # grade 2
                .long 11      # grade 3
                .long 5       # computed average
                .long 2222    # student ID
                .long 60      # grade 0
                .long 70      # grade 1
                .long 80      # grade 2
                .long 40      # grade 3
                .long 6       # computed average
                .long 3333    # student ID
                .long 10      # grade 0
                .long 40      # grade 1
                .long 50      # grade 2
                .long 70      # grade 3
                .long 6       # computed average
                .long 4444    # student ID
                .long 10      # grade 0
                .long 10      # grade 1
                .long 10      # grade 2
                .long 10      # grade 3
                .long 6       # computed average
                .long 5555    # student ID
                .long 90      # grade 0
                .long 90      # grade 1
                .long 90      # grade 2
                .long 90      # grade 3
                .long 6       # computed average