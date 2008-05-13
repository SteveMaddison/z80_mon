#include "console.h"
#include "uart.h"
#include "util.h"

unsigned char con_getc( void ) {
	return uart_getc();
}

void con_putc( unsigned char c ) {
	uart_putc( c );
}

void con_putaddr( addr_t a ) {
	con_putc( digits[ a & 0xf000 >> 12 ] );
	con_putc( digits[ a & 0x0f00 >> 8 ] );
	con_putc( digits[ a & 0x00f0 >> 4 ] );
	con_putc( digits[ a & 0x000f ] );
}

void con_putword( unsigned char c ) {
	con_putc( digits[ c & 0xf0 >> 4 ] );
	con_putc( digits[ c & 0x0f ] );
}

void con_puts( const char *s ) {
	char *pos = (char*)s;
	while( *pos ) {
		con_putc( *pos++ );
	}
}

void con_crlf( void ) {
	con_putc('\n');
}
