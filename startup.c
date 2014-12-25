#include "inc/hw_nvic.h"

void main();

extern unsigned long _etext, _data, _edata, _program, _eprogram, _bss, _ebss;

void resetISR() {
    // An axplanation of the tasks resetISR must carry out is available in
    // http://www.embecosm.com/appnotes/ean9/html/ch05s02.html
    // .data is the section for initialized variables
    // Copy the initial values from Flash to SRAM
    unsigned long *src=&_etext, *dest=&_data;
    while(dest < &_edata)
        *dest++ = *src++;
    dest = &_program;
    while(dest < &_eprogram) {
        *dest++ = *src++;
    }
    // .bss is the section for zero-initialized variables
    dest=&_bss;
    while(dest < &_ebss)
        *dest++ = 0;

    main();
}

volatile unsigned long active[4];
void missingISR() {
    // TODO: use ACTIVE0..ACTIVE3 to indicate the interrupt source
    volatile unsigned long *activeRegs;
    activeRegs = (unsigned long*)NVIC_ACTIVE0;
    for(int i=0; i<4; i++)
        active[i] = activeRegs[i];
    while(1);
}

// Reserve RAM for the stack
// TODO: may need static
static unsigned long stack[256];

__attribute__((section(".vector_table")))
void (*vectortable[])(void) = {
    (void (*) (void))((unsigned long)stack+sizeof(stack)*sizeof(unsigned long)),
    resetISR,             // Reset
    missingISR,         // NMI
    missingISR,         // Hard Fault
    missingISR,         // Memory Management Fault
    missingISR,         // Bus Fault
    missingISR,         // Usage Fault
    0,0,0,0,
    missingISR,         // SVCall
    missingISR,         // DebugMonitor
    0,
    missingISR,         // PendSV
    missingISR,         // SysTick
    missingISR,         // IRQ 16   GPIO Port A
    missingISR,         // IRQ 17   GPIO Port B
    missingISR,         // IRQ 19   GPIO Port C
    missingISR,         // IRQ 20   GPIO Port D
    missingISR,         // IRQ 21   GPIO Port E
    missingISR,         // IRQ 22   UART0
    missingISR,         // IRQ 23   UART1
    missingISR,         // IRQ 24   SSI0
    missingISR,         // IRQ 25   I2C0
    missingISR,         // IRQ 26   Reserved
    missingISR,         // IRQ 27   Reserved
    missingISR,         // IRQ 28   Reserved
    missingISR,         // IRQ 29   Reserved
    missingISR,         // IRQ 30   Reserved
    missingISR,         // IRQ 31   ADC0 Sequence 0
    missingISR,         // IRQ 32   ADC0 Sequence 1
    missingISR,         // IRQ 33   ADC0 Sequence 2
    missingISR,         // IRQ 34   ADC0 Sequence 3
    missingISR,         // IRQ 35   Watchdog
    missingISR,         // IRQ 36   16/32-Bit Timer 0A
    missingISR,         // IRQ 37   16/32-Bit Timer 0B
    missingISR,         // IRQ 38   16/32-Bit Timer 1A
    missingISR,         // IRQ 39   16/32-Bit Timer 1B
    missingISR,         // IRQ 40   16/32-Bit Timer 2A
    missingISR,         // IRQ 41   16/32-Bit Timer 2B
    missingISR,         // IRQ 42   Analog Comparator 0
    missingISR,         // IRQ 43   Analog Comparator 1
    missingISR,         // IRQ 44   Reserved
    missingISR,         // IRQ 45   System Control
    missingISR,         // IRQ 46   Flash Memory Control and EEPROM Control
    missingISR,         // IRQ 47   GPIO Port F
    missingISR,         // IRQ 48   Reserved
    missingISR,         // IRQ 49
    missingISR,         // IRQ 50
    missingISR,         // IRQ 51
    missingISR,         // IRQ 52
    missingISR,         // IRQ 53
    missingISR,         // IRQ 54
    missingISR,         // IRQ 55
    missingISR,         // IRQ 56
    missingISR,         // IRQ 57
    missingISR,         // IRQ 58
    missingISR,         // IRQ 59
    missingISR,         // IRQ 60
    missingISR,         // IRQ 61
    missingISR,          // IRQ 62
    missingISR,         // IRQ 63
    missingISR,         // IRQ 64
    missingISR,         // IRQ 65
    missingISR,         // IRQ 66
    missingISR,         // IRQ 67
    missingISR,         // IRQ 68
    missingISR,         // IRQ 69
    missingISR,         // IRQ 70
    missingISR,         // IRQ 71
    missingISR,         // IRQ 72
    missingISR,         // IRQ 73
    missingISR,         // IRQ 74
    missingISR,         // IRQ 75
    missingISR,         // IRQ 76
    missingISR,         // IRQ 77
    missingISR,         // IRQ 78
    missingISR,         // IRQ 79
    missingISR,         // IRQ 80
    missingISR,         // IRQ 81
    missingISR,         // IRQ 82
    missingISR,         // IRQ 83
    missingISR,         // IRQ 84
    missingISR,         // IRQ 85
    missingISR,         // IRQ 86
    missingISR,         // IRQ 87
    missingISR,         // IRQ 88
    missingISR,         // IRQ 89
    missingISR,         // IRQ 90
    missingISR,         // IRQ 91
    missingISR,         // IRQ 92
    missingISR,         // IRQ 93
    missingISR,         // IRQ 94
    missingISR,         // IRQ 95
    missingISR,         // IRQ 96
    missingISR,         // IRQ 97
    missingISR,         // IRQ 98
    missingISR,         // IRQ 99
    missingISR,         // IRQ 100
    missingISR,         // IRQ 101
    missingISR,         // IRQ 102
    missingISR,         // IRQ 103
    missingISR,         // IRQ 104
    missingISR,         // IRQ 105
    missingISR,         // IRQ 106
    missingISR,         // IRQ 107
    missingISR,         // IRQ 108
    missingISR,         // IRQ 109
    missingISR,         // IRQ 110
    missingISR,         // IRQ 111
    missingISR,         // IRQ 112
    missingISR,         // IRQ 113
    missingISR,         // IRQ 114
    missingISR,         // IRQ 115
    missingISR,         // IRQ 116
    missingISR,         // IRQ 117
    missingISR,         // IRQ 118
    missingISR,         // IRQ 119
    missingISR,         // IRQ 120
    missingISR,         // IRQ 121
    missingISR,         // IRQ 122
    0,                  // IRQ 123
    0,                  // IRQ 124
    0,                  // IRQ 125
    0,                  // IRQ 126
    0,                  // IRQ 127
    0,                  // IRQ 128
    0,                  // IRQ 129
    0,                  // IRQ 130
    0,                  // IRQ 131
    0,                  // IRQ 132
    0,                  // IRQ 133
    0,                  // IRQ 134
    0,                  // IRQ 135
    0,                  // IRQ 136
    0,                  // IRQ 137
    0,                  // IRQ 138
    0,                  // IRQ 139
    0,                  // IRQ 140
    0,                  // IRQ 141
    0,                  // IRQ 142
    0,                  // IRQ 143
    0,                  // IRQ 144
    0,                  // IRQ 145
    0,                  // IRQ 146
    0,                  // IRQ 147
    0,                  // IRQ 148
    0,                  // IRQ 149
    0,                  // IRQ 150
    0,                  // IRQ 151
    0,                  // IRQ 152
    0,                  // IRQ 153
    0,                  // IRQ 154
};
