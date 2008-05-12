#include "config.h"
#include "errno.h"
#include "types.h"
#include "uart.h"
#include "util.h"

#define CLI_BUFFER_LENGTH 32

#define MODE_NONE	0
#define MODE_ADDR	1
#define MODE_REG	2
#define MODE_PORT	3

#define REG_AF		0
#define REG_BC		1
#define REG_DE		2
#define REG_HL		3
#define REG_IX		4
#define REG_IY		5
#define REG_IR		6
#define REG_SP		7
#define NO_REGS		8
int reg[NO_REGS];

/* Maximum number of units supported by bootstraps */
#define MAX_UNIT	8

/* Cheap intetface to RAM - must be declared first! */
unsigned char *mem = 0;

/* Constants */
const char *program = "Cosam 3z Monitor";
const char *version = "0.1";
const char digits[] = "0123456789abcdef";

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

int number( const char *string, int base ) {
	/* Simple string to number converter */
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

void boot_ata( int unit ) {
	UNUSED(unit);
	return;
}

int reg_lookup( char* name ) {
	if( !strcmp( name, "af" ) ) {
		return REG_AF;
	}
	else if( !strcmp( name, "bc" ) ) {
		return REG_BC;
	}
	else if( !strcmp( name, "de" ) ) {
		return REG_DE;
	}
	else if( !strcmp( name, "hl" ) ) {
		return REG_HL;
	}
	else if( !strcmp( name, "ix" ) ) {
		return REG_IX;
	}
	else if( !strcmp( name, "iy" ) ) {
		return REG_IY;
	}
	else if( !strcmp( name, "ir" ) ) {
		return REG_IR;
	}
	else if( !strcmp( name, "sp" ) ) {
		return REG_SP;
	}
	else {
		return NO_REGS;
	}
}

void mem_test( void ) {
	return;
}

void run_cmd( const char* cmd, const char *param ) {
	if( !strcmp( cmd, "ba" ) ) {
		/* boot ATA, param is unit number */
		int unit = 0;
		if( param ) {
			unit = number( param, 10 );
		}
		if( unit < MAX_UNIT ) {
			boot_ata( unit );
		}
		else {
			con_puts("Bad unit\n");
		}
	}
	else if( !strcmp( cmd, "he" ) ) {
		/* display help */
		con_puts("Editor\n");
		con_puts(" [a]/[c] : Examine memory address [a] / set contents to [c]\n");
		con_puts(" [r]$[c] : Examine register [r] / set contents to [c]\n");
		con_puts(" [p]@[c] : Examine I/O port [p] / set contents to [c]\n");
		con_puts(" <cr>    : Opens next address\n");
		con_puts(" <bs>    : Opens previous address\n");
		con_puts(" <esc>   : Exits edit mode\n");
		con_puts("Commands\n");
		con_puts(" he      : Display this help\n");
		con_puts(" go [a]  : Run from address [a]\n");
		con_puts(" ba [u]  : Boot ATA unit [u]\n");
		con_puts(" mt      : Memory test\n");
	}
	else if( !strcmp( cmd, "go" ) ) {
		/* run, param is address */
		if( param ) {
			int addr = number( param, 16 );
			/* jmp $addr */
		}
		else {
			con_puts("Bad address\n");
		}
	}
	else if( !strcmp( cmd, "mt" ) ) {
		mem_test();
	}
	else {
		/* invalid command */
		con_puts("Bad command: type 'he' for help\n");
	}
}

int main() {
	char cli_buffer[CLI_BUFFER_LENGTH];
	int addr = 0;
	int reg_id = 0;
	word_t value = 0;
		
	con_crlf();
	con_puts(program);
	con_puts(" v");
	con_puts(version);
	con_crlf();
	con_crlf();
	
	for(;;) {
		char *pos = &cli_buffer[0];
		char *param = NULL;
		char mode = MODE_NONE;
		char done = 0;
		
		con_puts("# ");
		*pos = 0;
		
		while( done == 0 ) {
			char c = con_getc();
			switch( c ) {
				case 0x1b: /* escape */
					con_crlf();
					done = 1;
					break;
				case '\b':
					if( mode == MODE_ADDR || mode == MODE_PORT ) {
						addr--;
						con_crlf();
						con_putaddr( addr );
						if( mode == MODE_ADDR ) {
							con_putc('/');
							con_putword( mem[addr] );
						}
						else {
							con_putc('@');
							con_putword( inb(addr) );
						}
						con_putc(' ');
					}
					else {
						if( pos > &cli_buffer[0] ) {
							*pos--;
							con_putc(c);
							con_putc(' ');
							con_putc(c);
						}
					}
					break;
				case '\n':
					*pos++ = 0;
					con_crlf();
					if( mode == MODE_ADDR || mode == MODE_PORT ) {
						if( *param ) {
							int value = number( param, 16 );
							if( mode == MODE_ADDR ) {
								mem[addr] = value;
							}
							else {
								outb(addr, (char)value);
							}
						}
						addr++;
						con_putaddr( addr );
						if( mode == MODE_ADDR ) {
							con_putc('/');
							con_putword( mem[addr] );
						}
						else {
							con_putc('@');
							con_putword( inb(addr) );
						}
						con_putc(' ');
					}
					else if( mode == MODE_REG ) {
						if( *param ) {
							int value = number( param, 16 );
							reg[reg_id] = value;
						}
						done = 1;
					}
					else {
						if( cli_buffer[0] ) {
							run_cmd( cli_buffer, param );
						}
						done = 1;
					}
					break;
				case ' ':
					*pos++ = 0;
					param = pos;
					*pos = 0;
					con_putc(c);
					break;
				case '/':
				case '@':
					*pos++ = 0;
					param = pos;
					*pos = 0;
					addr = number( cli_buffer, 16 );
					con_putc(c);
					if( c == '/' ) {
						con_putword( mem[addr] );
						mode = MODE_ADDR;
					}
					else {
						con_putword( inb(addr) );
						mode = MODE_PORT;
					}
					con_putc(' ');
					break;
				case '$':
					*pos++ = 0;
					param = pos;
					*pos = 0;
					con_putc(c);
					reg_id = reg_lookup(cli_buffer);
					if( reg_id == NO_REGS ) {
						con_crlf();
						done = 1;
					}
					else {
						con_putaddr( reg[reg_id] );
						con_putc(' ');
						mode = MODE_REG;
					}
					break;
				default:
					if( c >= 'A' && c <= 'Z' ) {
						/* upper -> lower case */
						c += 0x20;
					}
					if( (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') ) {
						if( pos - &cli_buffer[0] >= CLI_BUFFER_LENGTH ) {
							/* full! */
						}
						else {
							con_putc(c);
							*pos++ = c;
						}
					}
					break;
			}
			
		}
	}
}
