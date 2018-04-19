;公用函数库

;接口声明
	EXPORT Bcd2Dec
	EXPORT Dec2Bcd
	EXPORT Div_U32
 
 
;数据区
 AREA COMMON_DATE,DATA,READWRITE

;代码区
 AREA COMMON_CODE,CODE,READONLY
 
Bcd2Dec;BCD码转十进制，参数1：十进制数
	STMFD	SP!,{R1-R2,LR}
	AND		R1,R0,#0xF	;低4位
	LSR		R0,#4
	LDR		R2,=10
	MUL		R0,R0,R2
	ADD		R0,R0,R1
	LDMFD	SP!,{R1-R2,PC}
	
	
Dec2Bcd;十进制转BCD码，参数1：BCD码
	STMFD	SP!,{R1-R2,LR}
	LDR		R1,=10
	BL		Div_U32
	LSL		R0,#4
	ORR		R0,R2
	LDMFD	SP!,{R1-R2,PC}
	
	
;Div_U32，32位无符号整数除法，参数1：被除数，参数2：除数，商存入R0，余数存入R2
Div_U32
	STMFD	SP!,{R3-R4,LR}	;入栈
	LDR		R3,=0			;r3清零，被除数左移的高位存储器
	LDR		R4,=32			;减32次
SUB_AGAIN
	LSL		R3,#1			;R3左移，留出一个空位
	TST		R0,#0X80000000	;高位检测
	BEQ		HBIT_EQU0		;高位为0，直接跳过
	ORR		R3,#1			;高位为1，存入R3
HBIT_EQU0
	LSL		R0,#1
	SUBS	R2,R3,R1
	BCC		SUB_END			;不够减跳过
	ORR		R0,#1			;商1
	MOV		R3,R2			;够减存入差
SUB_END
	SUBS	R4,R4,#1		;计数减1，并设置cpsr相关位（s是必须的）
	BNE		SUB_AGAIN
	
	MOV		R2,R3			;余数写入r2
	LDMFD	SP!,{R3-R4,PC}	;出栈
;Div_U32，end
