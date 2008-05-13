#include "console.h"
#include "uart.h"
#include "util.h"

static const char digits[] = "0123456789abcdef";

void con_init( void ) {
	uart_init();
}

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

void con_putword( word_t c ) {
	con_putc( digits[ c & 0xf0 >> 4 ] );
	con_putc( digits[ c & 0x0f ] );
}

void con_puts( unsigned char *s ) {
	char *pos = (char*)s;
	while( *pos ) {
		con_putc( *pos++ );
	}
}

void con_crlf( void ) {
	con_putc('\n');
}

/* Simple string to number converter */
int number( const char *string, int base ) {
	char *c = string;
	int result = 0;
	char negative = 0;
	
	if( base > 16 )
		return 0;
		
	while( *c == ' ' )
		c++;
		
	if( *c == '-' )
		negative = 1;
		
	while( *c != 0 ) {
		int digit = 0;
		if( *c >= '0' && *c <= '9' ) {
			digit = *c - '0';
		}
		else if( *c >= 'a' && *c <= 'f' ) {
			digit = 10 + *c - 'a';	
		}
		else {
			/* invalid char */
			return 0;
		}
		if( digit > base - 1 ) {
			/* char out of range for number base */
			return 0;	
		}
		result = (result * base) + digit;
		c++;
	}
	
	if( negative )
		result = -result;
		
	return result;
}
