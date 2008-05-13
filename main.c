#include "ata.h"
#include "config.h"
#include "console.h"
#include "types.h"
#include "util.h"

/* Constants */
static const char program[] = "Cosam 3z Monitor";
static const char version[] = "0.1";

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
	else {
		return NO_REGS;
	}
}

void set_reg( reg_id_t reg_id, addr_t value ) {
	UNUSED(reg_id);
	UNUSED(value);	
}

regpair_t get_reg( reg_id_t reg_id ) {
	UNUSED(reg_id);
	return 0;
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
			ata_boot( unit );
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
			addr_t addr = number( param, 16 );
			jump( addr );
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
	addr_t addr = 0;
	reg_id_t reg_id = 0;
	word_t value = 0;

	con_init();
	con_crlf();
	con_puts(program);
	con_puts(" v");
	con_puts(version);
	con_crlf();
	con_crlf();
	
	for(;;) {
		char *pos = &cli_buffer[0];
		char *param = NULL;
		mode_t mode = MODE_NONE;
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
				case 0x7f:
					if( mode == MODE_ADDR || mode == MODE_PORT ) {
						addr--;
						con_crlf();
						con_putaddr( addr );
						if( mode == MODE_ADDR ) {
							con_putc('/');
							con_putword( *(word_t*)addr );
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
							con_putc('\b');
							con_putc(' ');
							con_putc('\b');
						}
					}
					break;
				case '\n':
				case '\r':
					*pos++ = 0;
					con_crlf();
					if( mode == MODE_ADDR || mode == MODE_PORT ) {
						if( *param ) {
							addr_t value = number( param, 16 );
							if( mode == MODE_ADDR ) {
								*(word_t*)addr = value;
							}
							else {
								value &= 0x00ff;
								outb(addr, (char)value);
							}
						}
						addr++;
						con_putaddr( addr );
						if( mode == MODE_ADDR ) {
							con_putc('/');
							con_putword( *(word_t*)addr );
						}
						else {
							con_putc('@');
							con_putword( inb(addr) );
						}
						con_putc(' ');
					}
					else if( mode == MODE_REG ) {
						if( *param ) {
							reg_t value = number( param, 16 );
							set_reg(reg_id, value);
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
						con_putword( *(word_t*)addr );
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
						con_putaddr( get_reg(reg_id) );
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
