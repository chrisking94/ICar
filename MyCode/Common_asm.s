;���ú�����

;�ӿ�����
	EXPORT Bcd2Dec
	EXPORT Dec2Bcd
	EXPORT Div_U32
 
 
;������
 AREA COMMON_DATE,DATA,READWRITE

;������
 AREA COMMON_CODE,CODE,READONLY
 
Bcd2Dec;BCD��תʮ���ƣ�����1��ʮ������
	STMFD	SP!,{R1-R2,LR}
	AND		R1,R0,#0xF	;��4λ
	LSR		R0,#4
	LDR		R2,=10
	MUL		R0,R0,R2
	ADD		R0,R0,R1
	LDMFD	SP!,{R1-R2,PC}
	
	
Dec2Bcd;ʮ����תBCD�룬����1��BCD��
	STMFD	SP!,{R1-R2,LR}
	LDR		R1,=10
	BL		Div_U32
	LSL		R0,#4
	ORR		R0,R2
	LDMFD	SP!,{R1-R2,PC}
	
	
;Div_U32��32λ�޷�����������������1��������������2���������̴���R0����������R2
Div_U32
	STMFD	SP!,{R3-R4,LR}	;��ջ
	LDR		R3,=0			;r3���㣬���������Ƶĸ�λ�洢��
	LDR		R4,=32			;��32��
SUB_AGAIN
	LSL		R3,#1			;R3���ƣ�����һ����λ
	TST		R0,#0X80000000	;��λ���
	BEQ		HBIT_EQU0		;��λΪ0��ֱ������
	ORR		R3,#1			;��λΪ1������R3
HBIT_EQU0
	LSL		R0,#1
	SUBS	R2,R3,R1
	BCC		SUB_END			;����������
	ORR		R0,#1			;��1
	MOV		R3,R2			;���������
SUB_END
	SUBS	R4,R4,#1		;������1��������cpsr���λ��s�Ǳ���ģ�
	BNE		SUB_AGAIN
	
	MOV		R2,R3			;����д��r2
	LDMFD	SP!,{R3-R4,PC}	;��ջ
;Div_U32��end
