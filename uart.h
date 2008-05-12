#ifndef _UART_H_
#define _UART_H_

/* UART_ADDR is defined in config.h */
#define UART_RBR (UART_ADDR)		/* RX Buffer/TX Holding Register */
#define UART_IER ((UART_ADDR) + 0x01)	/* Interrupt Enable Register */
#define UART_IIR ((UART_ADDR) + 0x02)	/* Interrupt Identifier Register */
#define UART_FCR ((UART_ADDR) + 0x02)	/* FIFO Control Register */
#define UART_LCR ((UART_ADDR) + 0x03)	/* Line Control Register */
#define UART_MCR ((UART_ADDR) + 0x04)	/* Modem Control Register */
#define UART_LSR ((UART_ADDR) + 0x05)	/* Line Status Register */
#define UART_MSR ((UART_ADDR) + 0x06)	/* Modem Status Register */
#define UART_SCR ((UART_ADDR) + 0x07)	/* Scratch Register */
#define UART_DLL (UART_ADDR)		/* Divisor Latch LSB (when DLAB=1 in LCR) */
#define UART_DLM ((UART_ADDR) + 0x01)	/* Divisor Latch MSB (when DLAB=1 in LCR) */

void uart_init( void );
unsigned char uart_getc( void );
void uart_putc( unsigned char c );

#endif /* _UART_H_ */
