// Test LiGu's Compiler
/* 2020-12 */
{
	int a;
	double b;
	char c;
	float d;
	int i;
	while(c < a) c = b + a;
	for(i = 0; i < 5; i = i + 1){
		a = a / 27.71 + b * 12E-2 - 2;
		if(a > d) break;
		c = c + a;
		if(c > b) continue;
		b = b + i;
	}
}
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
%a = alloca int32, align 0
%b = alloca float64, align 0
%c = alloca int8, align 0
%d = alloca float32, align 0
%i = alloca int32, align 0
label 1: 
%1 = < i8 %c i32 %a
ifFalse %1 goto label 2
%2 = + f64 %b i32 %a
store %2 *i8 %c
goto label 1
label 2:
store 0 *i32 %i
label 3: 
%3 = < i32 %i 5
ifFalse %3 goto label 4
%4 = / i32 %a 27.710000
%5 = * f64 %b 0.120000
%6 = + %4 %5
%7 = - %6 2
store %7 *i32 %a
%8 = > i32 %a f32 %d
ifFalse %8 goto label 5
goto label 4
label 5: 
%9 = + i8 %c i32 %a
store %9 *i8 %c
%10 = > i8 %c f64 %b
ifFalse %10 goto label 6
goto label 3
label 6: 
%11 = + f64 %b i32 %i
store %11 *f64 %b
%12 = + i32 %i 1
store %12 *i32 %i
goto label 3
label 4:
