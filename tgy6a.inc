;********************************************************************
;* Turnigy Plush 6A boards                                          *
;* Similar to normal "type 2" boards except with all sense lines on *
;* ADC pins (2, 4, 5) and possibly a different FET ordering. Please *
;* report if the rotation changes compared to stock firmware!       *
;* based on 6a ppm-r08 from Bernhard Konze:                         *
;* http://home.versanet.de/~b-konze/blc_6a/blc_6a.htm               *
;********************************************************************

.equ	F_CPU		, 16000000
.equ	USE_INT0	, 1
.equ	USE_I2C		, 0
.equ	USE_UART	, 0
.equ	USE_ICP		, 0

;*********************
; PORT D definitions *
;*********************
;.equ	mux_c		, 7	;i <plus> comparator input (AIN0)
;.equ	c_comp		, 6	;i common comparator input (AIN0)
.equ	ApFET		, 5	;o
.equ	CpFET		, 4	;o
.equ	BpFET		, 3	;o
.equ	rcp_in		, 2	;i r/c pulse input

.equ	INIT_PD		, 0
.equ	DIR_PD		, (1<<ApFET)+(1<<BpFET)+(1<<CpFET)

.equ	ApFET_port	, PORTD
.equ	BpFET_port	, PORTD
.equ	CpFET_port	, PORTD

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
	in	\r0, PORTD
	cbr	\r0, (1<<ApFET)+(1<<BpFET)+(1<<CpFET)
	out	PORTD, \r0
.ENDM


;*********************
; PORT C definitions *
;*********************
;.equ			, 7	; ADC7
;.equ			, 6	; ADC6
.equ	mux_a		, 5	; ADC5 phase input
.equ	mux_c		, 4	; ADC4 phase input
;.equ			, 3	; ADC3
.equ	mux_b		, 2	; ADC2 phase input
.equ	temp_adc	, 1	; ADC1 temperature control input
.equ	accu_adc	, 0	; ADC0 voltage control input

.equ	INIT_PC		, 0
.equ	DIR_PC		, 0

.MACRO comp_init r0:req
	in	\r0, SFIOR
	sbr	\r0, (1<<ACME)	; set Analog Comparator Multiplexer Enable
	out	SFIOR, \r0
	cbi	ADCSRA, ADEN	; disable ADC
.ENDM
.MACRO set_comp_phase_a r0:req
	ldi	\r0, mux_a	; set comparator multiplexer to phase A
	out	ADMUX, \r0
.ENDM
.MACRO set_comp_phase_b r0:req
	ldi	\r0, mux_b	; set comparator multiplexer to phase B
	out	ADMUX, \r0
.ENDM
.MACRO set_comp_phase_c	 r0:req
	ldi	\r0, mux_c	; set comparator multiplexer to phase C
	out	ADMUX, \r0
.ENDM


;*********************
; PORT B definitions *
;*********************
;.equ			, 7
;.equ			, 6
;.equ			, 5	(sck stk200 interface)
;.equ			, 4	(miso stk200 interface)
;.equ			, 3	(mosi stk200 interface)
.equ	AnFET		, 2
.equ	CnFET		, 1
.equ	BnFET		, 0

.equ	INIT_PB		, 0
.equ	DIR_PB		, (1<<AnFET)+(1<<BnFET)+(1<<CnFET)

.equ	AnFET_port	, PORTB
.equ	BnFET_port	, PORTB
.equ	CnFET_port	, PORTB

.MACRO AnFET_on
	sbi	PORTB, AnFET
.ENDM
.MACRO AnFET_off
	cbi	PORTB, AnFET
.ENDM
.MACRO BnFET_on
	sbi	PORTB, BnFET
.ENDM
.MACRO BnFET_off
	cbi	PORTB, BnFET
.ENDM
.MACRO CnFET_on
	sbi	PORTB, CnFET
.ENDM
.MACRO CnFET_off
	cbi	PORTB, CnFET
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
	in	\r0, PORTB
	sbr	\r0, (1<<AnFET)+(1<<BnFET)+(1<<CnFET)
	out	PORTB, \r0
.ENDM
.MACRO all_nFETs_off r0:req
	in	\r0, PORTB
	cbr	\r0, (1<<AnFET)+(1<<BnFET)+(1<<CnFET)
	out	PORTB, \r0
.ENDM

.MACRO RED_on
.ENDM
.MACRO RED_off
.ENDM
.MACRO GRN_on
.ENDM
.MACRO GRN_off
.ENDM
