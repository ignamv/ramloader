#include <inc/lm4f120h5qr.h>
#include "../../ek-lm4f120xl.h"

int main(void) {
    // Enable UART0 clock
    SYSCTL_RCGCUART_R |= SYSCTL_RCGCUART_R0;
    // Enable GPIOA clock
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R0;
    // GPIOA0 = RX, GPIOA1 = TX
    GPIO_PORTA_AFSEL_R |= UARTPORT_RX | UARTPORT_TX;
    GPIO_PORTA_DEN_R |= UARTPORT_RX | UARTPORT_TX;
    // Falta PCTL, DIR
    // Set baud rate generation for 115200 baud from 16MHz clock
    UART0_CTL_R &= ~UART_CTL_UARTEN;
    UART0_IBRD_R = 8;
    UART0_FBRD_R = 44;
    // 8N1, enable FIFOs
    UART0_LCRH_R = UART_LCRH_WLEN_8 | UART_LCRH_FEN;
    UART0_CC_R |= UART_CC_CS_PIOSC;
    UART0_CTL_R |= UART_CTL_RXE | UART_CTL_TXE;
    UART0_CTL_R |= UART_CTL_UARTEN;
    // Echo received characters
    while(1) {
        while(UART0_FR_R & UART_FR_RXFE);
        char c = UART0_DR_R;
        UART0_DR_R = c;
    }
    return 0;
}
    
