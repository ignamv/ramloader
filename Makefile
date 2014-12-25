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

AR=${PREFIX}-ar
LD=${PREFIX}-ld
CC=${PREFIX}-gcc
OBJCOPY=${PREFIX}-objcopy
OBJDUMP=${PREFIX}-objdump


%.S: %.c
	$(CC) $(AFLAGS) $(CFLAGS) -S -o $@ $<
%.o: %.c
	$(CC) $(AFLAGS) $(CFLAGS) -c -o $@ $<
%.o: %.S
	$(CC) $(AFLAGS) $(CFLAGS) -c -o $@ $<

all: dump.txt main.axf

main.axf: main.o
main.axf: program.o
main.axf: startup.o

%.axf:
	$(LD) $(LDFLAGS) -o $@ $^
%.bin: %.axf
	$(OBJCOPY) -O binary $< $@

dump.txt: main.axf
	$(OBJDUMP) -xdS $< > $@

.PHONY: download tags

download: main.bin
	lm4flash -v -s 0E103AE6 $<

tags: *.c
	ctags *

clean:
	rm -f *.o *.bin *.axf dump.txt
