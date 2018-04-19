;****usart����������****
USART1	EQU	0X40013800
USART2	EQU 0x40004400
DROS	EQU	0X04
SROS	EQU 0X00

	
;�ӿ�����
	EXPORT printChar
	EXPORT printStr
	EXPORT printNum
	EXPORT SelectUsart
;�ӿ�������END

;��������
	IMPORT Div_U32
;��������,end
	

;������
 AREA USART_ADDRESS,DATA,READWRITE
Cur_Usart	DCD		USART2		;��ǰusart��ַ�洢��Ĭ��Ϊusart2
Num_Buffer	SPACE	12			;����10byte�����ջ
;������,end

 
 AREA USART_CODE,CODE,READONLY
;SelectUsart��ѡ��Usart������2��usart��������(1=usart1,��1=usart2)
SelectUsart
	STMFD	SP!,{R1,LR}		;��ջ
	LDR		R1,=Cur_Usart	;����usart��ַ�ݴ�����ַ
	CMP		R0,#1
	BNE		U2
U1	LDR		R0,=USART1
	B		SETSR
U2	LDR		R0,=USART2
SETSR
	STR		R0,[R1]
	LDMFD	SP!,{R1,PC}		;��ջ
;SelectUsart��end


;***printChar ������1��char
printChar
	STMFD	SP!,{R1-R2,LR}		;��ջ
	LDR		R1,=Cur_Usart
	LDR		R1,[R1]			;����Usart��ַ
CHR_WAIT
	LDR		R2,[R1,#SROS]	;��״̬�Ĵ���
	TST		R2,#0X80
	BEQ		CHR_WAIT		;�ȴ��ϴδ������
	STRB	R0,[R1,#DROS]	;д��
	LDMFD	SP!,{R1-R2,PC}		;��ջ
;***printChar ��end


;***printStr ������1��str
printStr
	STMFD	SP!,{R1-R3,LR}		;��ջ
	LDR		R1,=Cur_Usart
	LDR		R1,[R1]			;����Usart��ַ
SEND_STR
	LDR		R2,[R1,#SROS]
	TST		R2,#0X80
	BEQ		SEND_STR	;�ȴ��ϴδ������
	LDRB	R3,[R0],#1	;��ָ�루R0�����ַ�����R3
	STRB	R3,[R1,#DROS];д��
	CMP		R3,#0
	BNE		SEND_STR;ѭ��ֱ��������'\0'
	LDMFD	SP!,{R1-R3,PC}		;��ջ
;***printStr ��end


;printNum ������1��NUM������2��usart��������(1=usart1,��1=usart2)
printNum
	STMFD	SP!,{R1-R5,LR}	;��ջ
	LDR		R1,=10			;����Ϊ10������������R0��
	LDR		R3,=Num_Buffer	;���뻺�����׵�ַ
	LDR		R4,=0
	LDR		R5,=0			;����ָʾ����1=����
	TST		R0,#0x80000000
	BEQ		STR_NUM			;����
	EOR		R0,#0xFFFFFFFF	;��������ת
	ADD		R0,#1
	LDR		R5,=1
STR_NUM
	BL		Div_U32
	ADD		R2,R2,#0x30		;ת��Ϊchar
	STRB	R2,[R3],#1		;�������뻺����
	ADD		R4,R4,#1		;����+1
	CMP		R0,#0
	BNE		STR_NUM			;ֱ����Ϊ0
	CMP		R5,#1
	
	LDR		R1,=Cur_Usart
	LDR		R1,[R1]			;����Usart��ַ
	BNE		SEND_NUM		;����
	LDR		R2,='-'
	STRB	R2,[R3],#1		;'-'�Ŵ��뻺����
	ADD		R4,R4,#1		;����+1
SEND_NUM
	LDRB	R0,[R3,#-1]!
SEND_WAIT
	LDR		R2,[R1,#SROS]	;��״̬�Ĵ���
	TST		R2,#0X80
	BEQ		SEND_WAIT		;�ȴ��ϴδ������
	STRB	R0,[R1,#DROS]	;д��
	SUBS	R4,R4,#1
	BNE		SEND_NUM
	
	LDMFD	SP!,{R1-R5,PC}		;��ջ
;printNum ��end


 END
 