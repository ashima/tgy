;***********************************************************
;* AfroESC                                                 *
;* 2011.09                                                 *
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
.equ	ApFET		, 3	;o (mosi)
.equ	BpFET		, 2	;o
.equ	CpFET		, 1	;o
.equ	rcp_in		, 0	;i r/c pulse input

.equ	INIT_PB		, 0
.equ	DIR_PB		, (1<<ApFET)+(1<<BpFET)+(1<<CpFET)

.equ	ApFET_port	, PORTB
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

.MACRO ApFET_on
	sbi	PORTB, ApFET
.ENDM
.MACRO ApFET_off
	cbi	PORTB, ApFET
.ENDM
.MACRO BpFET_on
	sbi	PORTB, BpFET
.ENDM
.MACRO BpFET_off
	cbi	PORTB, BpFET
.ENDM
.MACRO CpFET_on
	sbi	PORTB, CpFET
.ENDM
.MACRO CpFET_off
	cbi	PORTB, CpFET
.ENDM

.MACRO ApFET_on_reg r0:req
	sbr	\r0, 1<<ApFET
.ENDM
.MACRO ApFET_off_reg r0:req
	cbr	\r0, 1<<ApFET
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

.MACRO all_pFETs_off r0:req
	in	\r0, PORTB
	cbr	\r0, (1<<ApFET)+(1<<BpFET)+(1<<CpFET)
	out	PORTB, \r0
.ENDM

;*********************
; PORT C definitions *
;*********************
.equ	accu_adc	, 7	; ADC7 voltage control input
;.equ			, 6	; ADC6
.equ	i2c_clk		, 5	; ADC5/SCL
.equ	i2c_data	, 4	; ADC4/SDA
.equ	temp_adc	, 3	; ADC3 temperature control input
.equ	mux_c		, 2	; ADC2 phase input
.equ	mux_b		, 1	; ADC1 phase input
.equ	mux_a		, 0	; ADC0 phase input

.equ	INIT_PC		, (1<<i2c_clk)+(1<<i2c_data)
.equ	DIR_PC		, 0

.MACRO comp_init r0:req
	in	\r0, SFIOR
	sbr	\r0, (1<<ACME)		; switch to comparator multiplexed
	out	SFIOR, \r0
	cbi	ADCSRA, ADEN		; disable ADC
.ENDM
.MACRO set_comp_phase_a r0:req
	ldi	\r0, mux_a		; set comparator multiplexer to phase A
	out	ADMUX, \r0
.ENDM
.MACRO set_comp_phase_b r0:req
	ldi	\r0, mux_b		; set comparator multiplexer to phase B
	out	ADMUX, \r0
.ENDM
.MACRO set_comp_phase_c r0:req
	ldi	\r0, mux_c		; set comparator multiplexer to phase C
	out	ADMUX, \r0
.ENDM


;*********************
; PORT D definitions *
;*********************
.equ	WarnLED		, 2
.equ	AnFET		, 3
.equ	BnFET		, 4
.equ	CnFET		, 5
.equ	StatLED		, 7

.equ	INIT_PD		, 0
.equ	DIR_PD		, (1<<AnFET)+(1<<BnFET)+(1<<CnFET)+(1<<WarnLED)+(1<<StatLED)

.equ	AnFET_port	, PORTD
.equ	BnFET_port	, PORTD
.equ	CnFET_port	, PORTD

.MACRO AnFET_on
	sbi	PORTD, AnFET
.ENDM
.MACRO AnFET_off
	cbi	PORTD, AnFET
.ENDM
.MACRO BnFET_on
	sbi	PORTD, BnFET
.ENDM
.MACRO BnFET_off
	cbi	PORTD, BnFET
.ENDM
.MACRO CnFET_on
	sbi	PORTD, CnFET
.ENDM
.MACRO CnFET_off
	cbi	PORTD, CnFET
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
	cbi	PORTD, StatLED
.ENDM
.MACRO GRN_off
	sbi	PORTD, StatLED
.ENDM
