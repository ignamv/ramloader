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
       -Os                      \
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
LDFLAGS=--gc-sections -T linkerscript.ld
LDFLAGS2=--gc-sections -T linkprogram.ld

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

all: main.txt program.txt

main.axf: main.o
main.axf: program.o
main.axf: startup.o

program.axf: program.o
	$(LD) $(LDFLAGS2) -o $@ $^

%.axf:
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
	rm -f *.o *.bin *.axf main.txt program.txt

debug: main.axf
	$(GDB) -x debug $<

