OPTIMIZE=-Os
MCU=atmega8
#COMPILER_DIRECTORY=/opt/local/bin/
CC=$(COMPILER_DIRECTORY)avr-gcc
OBJCOPY=$(COMPILER_DIRECTORY)avr-objcopy
OBJDUMP=$(COMPILER_DIRECTORY)avr-objdump

ASFLAGS=-D$(@:.S.o=_esc) -x assembler-with-cpp -Wa,--gstabs,--fatal-warnings,-alms=$(<:.asm=.lst)#,--defsym=SPI_DEBUG=1
# -std=gnu99 is to allow for // style comments in C files (otherwise -pedantic whines)
CFLAGS=-I/opt/local/avr/include/ -mmcu=$(MCU) -Wall -pedantic -std=gnu99 $(OPTIMIZE) -g -c
MAPFILE=$(firstword $(<:.S.o=.map))
LDFLAGS=-Wl,--gc-sections,--entry=reset,-Map=$(MAPFILE),--cref
#-d,-E,--discard-none,

#SOURCES = tgy.S
#OBJECTS = $(patsubst %.cpp,%.o, $(filter %.pde,$(SOURCES))) \
#          $(patsubst %.c,%.o, $(filter %.c, $(SOURCES))) \
#          $(patsubst %.S,%.o, $(filter %.S, $(SOURCES)))

all: tgy.hex

# I have no idea why Makefile: Makefile.o is an implicit rule.
# http://www.gnu.org/software/make/manual/make.html#Canceling-Rules
Makefile : ;
# canceling another rule...
tgy.asm : ;

%.c.o : %.c Makefile
	$(CC) $(CFLAGS) -o $@ $< 

%.S.o : tgy.asm %.inc Makefile
	$(CC) $(CFLAGS) $(ASFLAGS) -o $@ $< 

#/opt/local/avr/lib/avr4/crtm8.o 
%.elf : %.S.o embedded-atmel-twi/TWISlaveMem14.c.o
	$(CC) $(LDFLAGS) -o $@ $< embedded-atmel-twi/TWISlaveMem14.c.o
#	replaces the .if pm_hi8(pwm_off) test in tgy.asm, which must happen after link-time, of course!
#	note that the rshift($1, 1) is equivalent to ($1 >> 1) and is because IJMP is word-oriented
#	(look up the difference between hi8 and pm_hi8)
	@gawk --non-decimal-data '/0x[0-9a-fA-F]+\s+pwm_off/ { if (rshift($$1,1) > 0xFF) { \
	printf "pm_hi8(pwm_off) is non-zero (pwm_off is at 0x%04X); please move code closer to start or use 16-bit (ZH) jump registers\n", $$1;\
	exit(1)}\
	}' $(MAPFILE) >&2

%.eep : %.elf
	$(OBJCOPY) -O ihex -j .eeprom --set-section-flags=.eeprom=alloc,load --no-change-warnings --change-section-lma .eeprom=0 $< $@

%.hex : %.elf
	$(OBJCOPY) -O ihex -R .eeprom $< $@

%.elf.dis : %.elf
	$(OBJDUMP) -S $< > $@

%.hex.dis : %.hex
	$(OBJDUMP) -m avr -D $< > $@
#t=`mktemp -t wut` && expand -t 4 bs_nfet.dis > $t && gawk -F':' '/^....:/ {printf "%-60s ;; %s\n", $2, $1; next}1' $t > bs_nfet.dis

%.c.o.dis : %.c.o
	$(OBJDUMP) -m avr -S $< > $@

%.S.o.dis : %.S.o
	$(OBJDUMP) -m avr -S $< > $@

%.dis.noaddr: %.dis
	gsed "s/^\s*[0-9a-fA-F]+://" $< > $@

upload: $(HEX)
	$(AVRDUDE) -e -C$(AVRDUDE_CONF) -p$(MCU) -c$(PROGRAMMER) -P$(PORT) -b$(BAUD) -D -Uflash:w:$(HEX):i

test: all_targets ;

ALL_TARGETS = afro.hex afro2.hex bs_nfet.hex bs.hex bs40a.hex rct50a.hex tp.hex tp_nfet.hex tgy6a.hex i2c_tgy.hex tgy.hex ian.hex

all_targets: $(ALL_TARGETS) ;

led:
	avr-gcc -I/opt/local/avr/include/ -mmcu=atmega8 -Wall -pedantic -std=gnu99 -Os -g -c -x assembler-with-cpp -Wa,--gstabs,--fatal-warnings -o led.S.o led.asm
	avr-gcc -Wl,--gc-sections,--entry=reset -o led.elf led.S.o
	avr-objcopy -O ihex -R .eeprom led.elf led.hex	
	avrdude -eV -pm8 -cavrisp -P/dev/tty.usbmodem5d11 -b57600 -D -Uflash:w:led.hex:i

clean:
	-rm -f *.o *.elf *.eep *.hex *.dis *.lst *.map

binary_zip: $(ALL_TARGETS)
	TARGET="tgy_`date '+%Y-%m-%d'`_`git rev-parse --verify --short HEAD`.zip"; \
	git archive -9 -o "$$TARGET" HEAD && \
	zip -9 "$$TARGET" $(ALL_TARGETS) && ls -l "$$TARGET"

program_dragon_%: %.hex
	avrdude -c dragon_isp -p m8 -P usb -U flash:w:$<:i

program_dapa_%: %.hex
	avrdude -c dapa -u -p m8 -U flash:w:$<:i

program_uisp_%: %.hex
	uisp -dprog=dapa --erase --upload --verify -v if=$<

program: program_dragon_tgy

program_dapa: program_dapa_tgy

program_uisp: program_uisp_tgy

read:
	avrdude -c dragon_isp -p m8 -P usb -v -U flash:r:flash.hex:i -U eeprom:r:eeprom.hex:i

read_dapa:
	avrdude -c dapa -p m8 -v -U flash:r:flash.hex:i -U eeprom:r:eeprom.hex:i

read_uisp:
	uisp -dprog=dapa --download -v of=flash.hex

terminal_dapa:
	avrdude -c dapa -p m8 -t
