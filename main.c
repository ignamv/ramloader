// Jump to a program stored in RAM
#include <inc/hw_types.h>
#include <driverlib/debug.h>
#include <driverlib/rom.h>
#include <driverlib/gpio.h>
#include <driverlib/sysctl.h>
#include <driverlib/uart.h>
#include "../../ek-lm4f120xl.h"

#ifdef SIMULATE_SERIAL

const char *serial = 
"S30D200040001E4B1A6842F0010272\n"
"S30D200040081A6053F8102C42F057\n"
"S30D20004010010243F8102CA3F570\n"
"S30D200040187A23A3F5FC730821AD\n"
"S30D200040201A6842F003021A603F\n"
"S30D20004028D3F8FC2042F003024C\n"
"S30D20004030C3F8FC2003F5F84358\n"
"S30D2000403810331A6822F0010280\n"
"S30D200040401A600F4A11602C21C1\n"
"S30D20004048516070219160D2F84D\n"
"S30D20004050A41F41F00501C2F88E\n"
"S30D20004058A41F1A6842F440720D\n"
"S30D200040601A601A6842F0010201\n"
"S30D200040681A60064B1B68DB06FB\n"
"S30D20004070FBD4054B1A68D2B2FD\n"
"S30D20004078F6E700BF18E60F4031\n"
"S30D2000408024C0004018C00040D6\n"
"S3092000408800C000400E\n"
"S7052000400199\n";

unsigned char getchar() {
    unsigned char ret = *serial;
    serial++;
    return ret;
}

#else

#define getchar()   ROM_UARTCharGet(UART0_BASE)

#endif

// The linker will point this to the section reserved for the program.
// Right now this section is loaded from flash to SRAM by the reset ISR.

extern void _program();

#ifdef DEBUG
void __error__(char *pcFilename, unsigned long ulLine) {
    while(1);
    }
#endif

unsigned char read_hex() {
    unsigned char ret = 0;
    for (char ii = 0; ii < 2; ii++) {
        ret <<= 4;
        char c = getchar();
        if (c >= 'A' && c <= 'F') {
            ret |= c - 'A' + 0xA;
        } else if (c >= '0' && c <= '9') {
            ret |= c - '0';
        } else {
            // Error
            // TODO: handle
            ASSERT(0);
        }
    }
    return ret;
}

void configure_uart() 
{
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    ROM_GPIOPinConfigure(GPIO_PA0_U0RX);
    ROM_GPIOPinConfigure(GPIO_PA1_U0TX);
    ROM_GPIOPinTypeUART(UARTPORT_BASE, UARTPORT_RX | UARTPORT_TX);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    ROM_UARTConfigSetExpClk(UART0_BASE, 16e6, 115200, UART_CONFIG_WLEN_8 |
            UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE);
    ROM_UARTEnable(UART0_BASE);
}

unsigned char *read_address() {
    unsigned long ret = 0;
    for (char ii = 0; ii < sizeof(ret); ii++) {
        ret <<= 8;
        ret |= read_hex();
    }
    return (unsigned char*) ret;
}

void load_srec() 
{
    unsigned char size, *address, checksum;
    while (1) {
        char cc = getchar();
        switch (cc) {
            case '\n':
            case '\r':
                continue;
            case 'S':
                break;
            default:
                // TODO: handle
                ASSERT(0);
        }
        switch (getchar()) {
            case '\n':
            case '\r':
                break;
            case '3':
                size = read_hex() - 5;
                address = read_address();
                while (size--) {
                    *address = read_hex();
                    checksum += *address++;
                }
                // TODO checksum
                read_hex();
                break;
            case '7':
                // Skip size
                read_hex();
                // Jump to program start
                address =  read_address();
                ((void (*)()) address)();
                break;
            default:
                // TODO: handle
                ASSERT(0);
        }
    }
}


int main(void)
{
    configure_uart();
    load_srec();
    return 0;
}

