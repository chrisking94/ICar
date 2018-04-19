;R0��Ϊ�������õķ���ֵ
;��������<=4������������ʹ��R0~R3��>4��ʱ����ջ
;���ATPCS��ARM Thumb Procedure Call Standard��
RTCCON	EQU	0X57000040
BCDSEC	EQU	0X57000070
BCDMIN	EQU BCDSEC+4
BCDHOU	EQU	BCDMIN+4
BCDDAT	EQU BCDHOU+4
BCDMON	EQU BCDDAT+8
BCDYEA	EQU BCDMON+4

;�ӿ�����
 
 EXPORT RTC_read
 EXPORT RTC_write
 
 IMPORT	Div_U32		;usart_asm.s
 IMPORT Bcd2Dec
 IMPORT Dec2Bcd
 
 AREA RTC_CODE,CODE,READONLY

RTC_read;�����ݣ�����1�����ݵ�ַ
	STMFD	SP!,{LR}
	LDRB	R0,[R0]
	BL		Bcd2Dec
	LDMFD	SP!,{PC}
	
	
RTC_write;д���ݣ�����1��Ҫд������ݣ�����2�����ݵ�ַ
	STMFD	SP!,{LR}
	BL		Dec2Bcd
	STRB	R0,[R1]
	LDMFD	SP!,{PC}
	
	
 END
 