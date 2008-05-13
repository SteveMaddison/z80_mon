#ifndef _CONSOLE_H_
#define _CONSOLE_H_

#include "types.h"

unsigned char con_getc( void );
void con_putc( unsigned char c );
void con_putaddr( addr_t a );
void con_putword( unsigned char c );
void con_puts( const char *s );
void con_crlf( void );
int number( const char *string, int base );

#endif /* _CONSOLE_H_ */
