;R0作为函数调用的返回值
;函数参数<=4个，参数传递使用R0~R3，>4个时借助栈
;详见ATPCS（ARM Thumb Procedure Call Standard）
RTCCON	EQU	0X57000040
BCDSEC	EQU	0X57000070
BCDMIN	EQU BCDSEC+4
BCDHOU	EQU	BCDMIN+4
BCDDAT	EQU BCDHOU+4
BCDMON	EQU BCDDAT+8
BCDYEA	EQU BCDMON+4

;接口声明
 
 EXPORT RTC_read
 EXPORT RTC_write
 
 IMPORT	Div_U32		;usart_asm.s
 IMPORT Bcd2Dec
 IMPORT Dec2Bcd
 
 AREA RTC_CODE,CODE,READONLY

RTC_read;读数据，参数1：数据地址
	STMFD	SP!,{LR}
	LDRB	R0,[R0]
	BL		Bcd2Dec
	LDMFD	SP!,{PC}
	
	
RTC_write;写数据，参数1：要写入的数据，参数2：数据地址
	STMFD	SP!,{LR}
	BL		Dec2Bcd
	STRB	R0,[R1]
	LDMFD	SP!,{PC}
	
	
 END
 