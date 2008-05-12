#include "config.h"
#include "uart.h"
#include "util.h"

void uart_init( void ) {
	outb(UART_IER, 0);	/* disable all interrupts */
	outb(UART_LCR, 0x80);	/* set DLAB */
	outb(UART_DLL, 12);	/* set baud rate 9600 */
	outb(UART_DLM, 0);
	outb(UART_LCR, 0x03);	/* 81N and clear DLAB */
}

unsigned char uart_getc( void ) {
	/* wait until UART has a character */
	while( !(inb(UART_LSR) && 0x01) );
	/* read character and return */
	return inb(UART_RBR);
}

void uart_putc( unsigned char c ) {
	/* wait until UART is ready to send */
	while( !(inb(UART_LSR) && 0x20) );
	/* send the character */
	outb(UART_RBR, c);
}
