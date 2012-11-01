;***********************************************************
;* AfroESC 2                                               *
;* 2012.02                                                 *
;***********************************************************

.equ	F_CPU		, 16000000
.equ	USE_INT0	, 0
.equ	USE_I2C		, 1
.equ	USE_UART	, 1
.equ	USE_ICP		, 1

;*********************
; PORT B definitions *
;*********************
;.equ			, 7
;.equ			, 6
;.equ			, 5	(sck)
;.equ			, 4	(miso)
;.equ			, 3	(mosi)
.equ	BpFET		, 2	;o
.equ	CpFET		, 1	;o
.equ	rcp_in		, 0	;i r/c pulse input

.equ	INIT_PB		, 0
.equ	DIR_PB		, (1<<BpFET)+(1<<CpFET)

.equ	BpFET_port	, PORTB
.equ	CpFET_port	, PORTB

.MACRO rcp_int_enable r0:req
	in	\r0, TIMSK
	sbr	\r0, (1<<TICIE1)	; enable icp1_int
	out	TIMSK, \r0
.ENDM
.MACRO rcp_int_disable r0:req
	in	\r0, TIMSK
	cbr	\r0, (1<<TICIE1)	; disable icp1_int
	out	TIMSK, \r0
.ENDM
.MACRO rcp_int_rising_edge r0:req
	ldi	\r0, T1CLK
	out	TCCR1B, \r0
.ENDM
.MACRO rcp_int_falling_edge r0:req
	ldi	\r0, T1CLK & ~(1<<ICES1)
	out	TCCR1B, \r0
.ENDM

.MACRO BpFET_on
	sbi	BpFET_port, BpFET
.ENDM
.MACRO BpFET_off
	cbi	BpFET_port, BpFET
.ENDM
.MACRO CpFET_on
	sbi	CpFET_port, CpFET
.ENDM
.MACRO CpFET_off
	cbi	CpFET_port, CpFET
.ENDM

.MACRO BpFET_on_reg r0:req
	sbr	\r0, 1<<BpFET
.ENDM
.MACRO BpFET_off_reg r0:req
	cbr	\r0, 1<<BpFET
.ENDM
.MACRO CpFET_on_reg r0:req
	sbr	\r0, 1<<CpFET
.ENDM
.MACRO CpFET_off_reg r0:req
	cbr	\r0, 1<<CpFET
.ENDM

.MACRO all_pFETs_off r0
	ApFET_off
	BpFET_off
	CpFET_off
.ENDM

;*********************
; PORT C definitions *
;*********************
.equ	accu_adc	, 7	; ADC7 voltage control input
;.equ			, 6	; ADC6
.equ	i2c_clk		, 5	; ADC5/SCL
.equ	i2c_data	, 4	; ADC4/SDA
.equ	ApFET		, 3	; o
.equ	StatLED		, 2	; o
.equ	mux_b		, 1	; ADC1 phase input
.equ	mux_a		, 0	; ADC0 phase input

.equ	INIT_PC		, (1<<i2c_clk)+(1<<i2c_data)
.equ	DIR_PC		, (1<<ApFET)+(1<<StatLED)

.equ	ApFET_port	, PORTC

.MACRO comp_init r0:req
	in	\r0, SFIOR
	sbr	\r0, (1<<ACME)	; set Analog Comparator Multiplexer Enable
	out	SFIOR, \r0
.ENDM
.MACRO set_comp_phase_a r0:req
	ldi	\r0, mux_a	; set comparator multiplexer to phase A
	out	ADMUX, \r0
	cbi	ADCSRA, ADEN	; disable ADC
.ENDM
.MACRO set_comp_phase_b r0:req
	ldi	\r0, mux_b	; set comparator multiplexer to phase B
	out	ADMUX, \r0
	cbi	ADCSRA, ADEN	; disable ADC
.ENDM
.MACRO set_comp_phase_c r0
	sbi	ADCSRA, ADEN	; enable ADC and comparator to phase C (AIN1)
.ENDM

.MACRO ApFET_on
	sbi	ApFET_port, ApFET
.ENDM
.MACRO ApFET_off
	cbi	ApFET_port, ApFET
.ENDM
.MACRO ApFET_on_reg r0:req
	sbr	\r0, 1<<ApFET
.ENDM
.MACRO ApFET_off_reg r0:req
	cbr	\r0, 1<<ApFET
.ENDM

;*********************
; PORT D definitions *
;*********************
;.equ	mux_c		, 7 (comparator AN1)
;.equ			, 6 (comparator AN0)
.equ	CnFET		, 5
.equ	BnFET		, 4
.equ	AnFET		, 3
.equ	WarnLED		, 2
;.equ			, 1
;.equ			, 0 USART_RX

.equ	INIT_PD		, 0
.equ	DIR_PD		, (1<<AnFET)+(1<<BnFET)+(1<<CnFET)+(1<<WarnLED)

.equ	AnFET_port	, PORTD
.equ	BnFET_port	, PORTD
.equ	CnFET_port	, PORTD

.MACRO AnFET_on
	sbi	AnFET_port, AnFET
.ENDM
.MACRO AnFET_off
	cbi	AnFET_port, AnFET
.ENDM
.MACRO BnFET_on
	sbi	AnFET_port, BnFET
.ENDM
.MACRO BnFET_off
	cbi	AnFET_port, BnFET
.ENDM
.MACRO CnFET_on
	sbi	AnFET_port, CnFET
.ENDM
.MACRO CnFET_off
	cbi	AnFET_port, CnFET
.ENDM

.MACRO AnFET_on_reg r0:req
	sbr	\r0, 1<<AnFET
.ENDM
.MACRO AnFET_off_reg r0:req
	cbr	\r0, 1<<AnFET
.ENDM
.MACRO BnFET_on_reg r0:req
	sbr	\r0, 1<<BnFET
.ENDM
.MACRO BnFET_off_reg r0:req
	cbr	\r0, 1<<BnFET
.ENDM
.MACRO CnFET_on_reg r0:req
	sbr	\r0, 1<<CnFET
.ENDM
.MACRO CnFET_off_reg r0:req
	cbr	\r0, 1<<CnFET
.ENDM

.MACRO nFET_brake r0:req
	in	\r0, PORTD
	sbr	\r0, (1<<AnFET)+(1<<BnFET)+(1<<CnFET)
	out	PORTD, \r0
.ENDM
.MACRO all_nFETs_off r0:req
	in	\r0, PORTD
	cbr	\r0, (1<<AnFET)+(1<<BnFET)+(1<<CnFET)
	out	PORTD, \r0
.ENDM

.MACRO RED_on
	sbi	PORTD, WarnLED
.ENDM
.MACRO RED_off
	cbi	PORTD, WarnLED
.ENDM
.MACRO GRN_on
	sbi	PORTC, StatLED
.ENDM
.MACRO GRN_off
	cbi	PORTC, StatLED
.ENDM</pre></td>
