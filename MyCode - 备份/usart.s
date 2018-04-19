;****usart����������****
USART1	EQU	0X40013800
USART2	EQU 0x40004400
DROS	EQU	0X04
SROS	EQU 0X00
	
 AREA USART,CODE,READONLY
	EXPORT printChar
	EXPORT printStr
	EXPORT printNum
	
;printChar ������1��char������2��usart��������(1=usart1,��1=usart2)
printChar
	CMP		R1,#1
	BNE		U2
U1	LDR		R1,=USART1
	B		WRITECHAR
U2	LDR		R1,=USART2
WRITECHAR
	LDR		R2,[R1,#SROS]
STANBY
	TST		R2,#0X80
	STRB	R0,[R1,#DROS]

;printStr
printStr
;printNum
printNum
	MOV PC,LR
 END
