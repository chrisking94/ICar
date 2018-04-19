;****usart驱动，汇编版****
USART1	EQU	0X40013800
USART2	EQU 0x40004400
DROS	EQU	0X04
SROS	EQU 0X00

	
;接口声明
	EXPORT printChar
	EXPORT printStr
	EXPORT printNum
	EXPORT SelectUsart
;接口声明，END

;引用声明
	IMPORT Div_U32
;引用声明,end
	

;数据区
 AREA USART_ADDRESS,DATA,READWRITE
Cur_Usart	DCD		USART2		;当前usart地址存储，默认为usart2
Num_Buffer	SPACE	12			;定义10byte缓冲堆栈
;数据区,end

 
 AREA USART_CODE,CODE,READONLY
;SelectUsart，选择Usart，参数2：usart器件代号(1=usart1,非1=usart2)
SelectUsart
	STMFD	SP!,{R1,LR}		;入栈
	LDR		R1,=Cur_Usart	;读入usart地址暂存器地址
	CMP		R0,#1
	BNE		U2
U1	LDR		R0,=USART1
	B		SETSR
U2	LDR		R0,=USART2
SETSR
	STR		R0,[R1]
	LDMFD	SP!,{R1,PC}		;出栈
;SelectUsart，end


;***printChar ，参数1：char
printChar
	STMFD	SP!,{R1-R2,LR}		;入栈
	LDR		R1,=Cur_Usart
	LDR		R1,[R1]			;载入Usart地址
CHR_WAIT
	LDR		R2,[R1,#SROS]	;读状态寄存器
	TST		R2,#0X80
	BEQ		CHR_WAIT		;等待上次传输完成
	STRB	R0,[R1,#DROS]	;写入
	LDMFD	SP!,{R1-R2,PC}		;出栈
;***printChar ，end


;***printStr ，参数1：str
printStr
	STMFD	SP!,{R1-R3,LR}		;入栈
	LDR		R1,=Cur_Usart
	LDR		R1,[R1]			;载入Usart地址
SEND_STR
	LDR		R2,[R1,#SROS]
	TST		R2,#0X80
	BEQ		SEND_STR	;等待上次传输完成
	LDRB	R3,[R0],#1	;将指针（R0）处字符读入R3
	STRB	R3,[R1,#DROS];写入
	CMP		R3,#0
	BNE		SEND_STR;循环直到结束符'\0'
	LDMFD	SP!,{R1-R3,PC}		;出栈
;***printStr ，end


;printNum ，参数1：NUM，参数2：usart器件代号(1=usart1,非1=usart2)
printNum
	STMFD	SP!,{R1-R5,LR}	;入栈
	LDR		R1,=10			;除数为10，被除数已在R0中
	LDR		R3,=Num_Buffer	;载入缓冲区首地址
	LDR		R4,=0
	LDR		R5,=0			;负数指示器，1=负数
	TST		R0,#0x80000000
	BEQ		STR_NUM			;正数
	EOR		R0,#0xFFFFFFFF	;负数，反转
	ADD		R0,#1
	LDR		R5,=1
STR_NUM
	BL		Div_U32
	ADD		R2,R2,#0x30		;转换为char
	STRB	R2,[R3],#1		;余数存入缓冲区
	ADD		R4,R4,#1		;计数+1
	CMP		R0,#0
	BNE		STR_NUM			;直到商为0
	CMP		R5,#1
	
	LDR		R1,=Cur_Usart
	LDR		R1,[R1]			;载入Usart地址
	BNE		SEND_NUM		;正数
	LDR		R2,='-'
	STRB	R2,[R3],#1		;'-'号存入缓冲区
	ADD		R4,R4,#1		;计数+1
SEND_NUM
	LDRB	R0,[R3,#-1]!
SEND_WAIT
	LDR		R2,[R1,#SROS]	;读状态寄存器
	TST		R2,#0X80
	BEQ		SEND_WAIT		;等待上次传输完成
	STRB	R0,[R1,#DROS]	;写入
	SUBS	R4,R4,#1
	BNE		SEND_NUM
	
	LDMFD	SP!,{R1-R5,PC}		;出栈
;printNum ，end


 END
 