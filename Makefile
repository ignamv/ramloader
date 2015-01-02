DEBUG=y

STELLA=/home/ignamv/programacion/stellaris
PREFIX=arm-none-eabi
INCLUDES=-I${STELLA}

CPU=-mcpu=cortex-m4
FPU=-mfpu=fpv4-sp-d16 -mfloat-abi=softfp
PART=LM4F120H5QR

AFLAGS=-mthumb \
       ${CPU}  \
       ${FPU}

CFLAGS=-mthumb                  \
       ${CPU}                   \
       ${FPU}                   \
       -O0                      \
       -ffunction-sections      \
       -fdata-sections          \
       -fno-stack-protector     \
       -std=c99                 \
       -Wall                    \
       -Werror                  \
       -DPART_${PART}           \
	   -DTARGET_IS_BLIZZARD_RA2 \
	   -DDEBUG					\
	   -g				        \
	   ${INCLUDES}
LDFLAGS2=--gc-sections -T linkerscript.ld
LDFLAGS=--gc-sections -T linkprogram.ld

AR=${PREFIX}-ar
LD=${PREFIX}-ld
CC=${PREFIX}-gcc
OBJCOPY=${PREFIX}-objcopy
OBJDUMP=${PREFIX}-objdump
GDB=${PREFIX}-gdb

%.S: %.c
	$(CC) $(AFLAGS) $(CFLAGS) -S -o $@ $<
%.o: %.c
	$(CC) $(AFLAGS) $(CFLAGS) -c -o $@ $<
%.o: %.S
	$(CC) $(AFLAGS) $(CFLAGS) -c -o $@ $<
%.srec: %.axf
	$(OBJCOPY) -S -O srec --srec-len=8 $< $@
	# Strip header
	sed -i /^S0/d $@

all: main.txt program.txt program.srec

main.axf: main.o
main.axf: startup.o

main.axf: main.o
	$(LD) $(LDFLAGS2) -o $@ $^

%.axf: %.o
	$(LD) $(LDFLAGS) -o $@ $^

%.bin: %.axf
	$(OBJCOPY) -O binary $< $@

%.txt: %.axf
	$(OBJDUMP) -xdS $< > $@

.PHONY: download tags clean debug

download: main.bin
	lm4flash -v -s 0E103AE6 $<

tags: *.c
	ctags *

clean:
	rm -f *.o *.bin *.axf main.txt program.txt *.srec

debug: main.axf
	$(GDB) -x debug $<

