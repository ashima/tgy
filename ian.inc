;***********************************************************
;* TowerPro 18A / Turnigy Plush 18A/25A "type 2" boards    *
;* with pFETs on PORTD, nFETs on PORTB                     *
;* based on tp-18a from Bernhard's 18a_tpro_ppm_r05.zip    *
;* Mai.2007                                                *
;* http://home.versanet.de/~b-konze/                       *
;* http://home.versanet.de/~b-konze/blc_18a/blc_18a.htm    *
;***********************************************************

.equ	F_CPU		, 16000000
.equ	USE_INT0	, 0
.equ	USE_I2C		, 1
.equ	USE_UART	, 0
.equ	USE_ICP		, 0

;*********************
; PORT D definitions *
;*********************
.equ	mux_c		, 7	;i <plus> comparator input (AIN0)
;.equ	c_comp		, 6	;i common comparator input (AIN0)
.equ	CpFET		, 5
.equ	pDriveGnd	, 4	;i r/c pulse input DUMMY NOT USED
.equ	BnFET		, 3	;o
.equ	BpFET		, 2	;o
.equ	AnFET		, 1	;o
.equ	ApFET		, 0	;o


.equ	INIT_PD		, 0
.equ	DIR_PD		, (1<<ApFET)+(1<<BpFET)+(1<<CpFET)+(1<<AnFET)+(1<<BnFET)+(1<<pDriveGnd)

.equ	ApFET_port	, PORTD
.equ	BpFET_port	, PORTD
.equ	CpFET_port	, PORTD
.equ	AnFET_port	, PORTD
.equ	BnFET_port	, PORTD

.MACRO rcp_int_enable r0:req
	ldi	\r0, (1<<INT0)	; enable ext0int
	out	GICR, \r0
.ENDM
.MACRO rcp_int_disable r0
	out	GICR, ZH	; disable ext0int
.ENDM
.MACRO rcp_int_rising_edge r0:req
	ldi	\r0, (1<<ISC01)+(1<<ISC00)
	out	MCUCR, \r0	; set next int0 to rising edge
.ENDM
.MACRO rcp_int_falling_edge r0:req
	ldi	\r0, (1<<ISC01)
	out	MCUCR, \r0	; set next int0 to falling edge
.ENDM

.MACRO ApFET_on
	sbi	PORTD, ApFET
.ENDM
.MACRO ApFET_off
	cbi	PORTD, ApFET
.ENDM
.MACRO BpFET_on
	sbi	PORTD, BpFET
.ENDM
.MACRO BpFET_off
	cbi	PORTD, BpFET
.ENDM
.MACRO CpFET_on
	sbi	PORTD, CpFET
.ENDM
.MACRO CpFET_off
	cbi	PORTD, CpFET
.ENDM

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

.MACRO all_pFETs_off r0:req
	in	\r0, PORTD
	cbr	\r0, (1<<ApFET)+(1<<BpFET)+(1<<CpFET)
	out	PORTD, \r0
.ENDM


;*********************
; PORT C definitions *
;*********************
;.equ			, 7	; ADC7
;.equ			, 6	; ADC6
;.equ			, 3	; ADC3
;.equ			, 2	; ADC2 (plush has something on here)
.equ	mux_b		, 1	; ADC1 phase input
.equ	mux_a		, 0	; ADC0 phase input

.equ	INIT_PC		, 0
.equ	DIR_PC		, 0

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


;*********************
; PORT B definitions *
;*********************
;.equ			, 7
;.equ			, 6
.equ	T5		, 5	; (sck stk200 interface)
.equ	T4		, 4	; (miso stk200 interface)
.equ	T3		, 3	; (mosi stk200 interface)
.equ	CnFET		, 2
.equ	LED		, 1

.equ	INIT_PB		, (1<<LED)
.equ	DIR_PB		, (1<<CnFET)+(1<<T3)+(1<<T4)+(1<<T5)+(1<<LED)
.equ	CnFET_port	, PORTB

.MACRO CnFET_on
	sbi	PORTB, CnFET
.ENDM
.MACRO CnFET_off
	cbi	PORTB, CnFET
.ENDM

.MACRO CnFET_on_reg r0:req
	sbr	\r0, 1<<CnFET
.ENDM
.MACRO CnFET_off_reg r0:req
	cbr	\r0, 1<<CnFET
.ENDM

;*********************

.MACRO nFET_brake r0:req
	sbi	AnFET_port,	AnFET
	sbi	BnFET_port, BnFET
	sbi	CnFET_port, CnFET
.ENDM
.MACRO all_nFETs_off r0:req
	cbi	AnFET_port,	AnFET
	cbi	BnFET_port, BnFET
	cbi	CnFET_port, CnFET
.ENDM

.MACRO RED_on
.ENDM
.MACRO RED_off
.ENDM
.MACRO GRN_on
.ENDM
.MACRO GRN_off
.ENDM
