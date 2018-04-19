;R0作为函数调用的返回值
;函数参数<=4个，参数传递使用R0~R3，>4个时借助栈
;详见ATPCS（ARM Thumb Procedure Call Standard）

 AREA asmfile,CODE,READONLY
 EXPORT RTC_init
RTC_init
	LDRB R3,[R0],#1
	LDRB R4,[R1],#1
	CMP R3,R4
	BNE DIF
	CMP R3,#0
	BNE RTC_init
SAME
	MOV R0,#1
	B ENDCMP
DIF
	MOV R0,#0
ENDCMP
	MOV PC,LR
 END
 