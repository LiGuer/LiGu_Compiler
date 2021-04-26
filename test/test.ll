#1 int ALLOC 32 ALIGN 0
#2 int ALLOC 64 ALIGN 0
#3 float ALLOC 64 ALIGN 0
#4 int ALLOC 8 ALIGN 0
#5 float ALLOC 32 ALIGN 0
#6 int ALLOC 32 ALIGN 0

label7: 
#13 = < #4 #2
JMPFalse #13 label8
#14 = + #3 #2
STORE #14 *#4
JMP label7

label8:
STORE 0 *#6

label9: 
#15 = < #6 5
JMPFalse #15 label10
#16 = / #2 27.710000
#17 = * #3 0.120000
#18 = + #16 #17
#19 = - #18 2
#20 = << #19 7
STORE #20 *#2
#21 = > #2 #5
#22 = <= #2 #4
#23 = && #21 #22
JMPFalse #23 label11
JMP label10

label11: 
#24 = + #4 #2
STORE #24 *#4
#25 = > #4 #3
JMPFalse #25 label12
JMP label9

label12: 
#26 = + #3 #6
STORE #26 *#3
#27 = + #6 1
STORE #27 *#6
JMP label9

label10:
