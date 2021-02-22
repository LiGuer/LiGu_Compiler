#1 = alloca int 32 align 0
#2 = alloca int 64 align 0
#3 = alloca float 64 align 0
#4 = alloca int 8 align 0
#5 = alloca float 32 align 0
#6 = alloca int 32 align 0

label 7: 
#13 = < #4 #2
ifFalse #13 goto label 8
#14 = + #3 #2
store #14 *#4
goto label 7

label 8:
store 0 *#6

label 9: 
#15 = < #6 5
ifFalse #15 goto label 10
#16 = / #2 27.710000
#17 = * #3 0.120000
#18 = + #16 #17
#19 = - #18 2
store #19 *#2
#20 = > #2 #5
ifFalse #20 goto label 11
goto label 10

label 11: 
#21 = + #4 #2
store #21 *#4
#22 = > #4 #3
ifFalse #22 goto label 12
goto label 9

label 12: 
#23 = + #3 #6
store #23 *#3
#24 = + #6 1
store #24 *#6
goto label 9

label 10:
