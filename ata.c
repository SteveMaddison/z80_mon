#include "ata.h"
#include "console.h"
#include "errno.h"
#include "util.h"

unsigned char ata_status( void ) {
	outb( ATA_CONTROL, ATA_REG_STATUS );
	return inb( ATA_DATA_LSB );
}

int ata_status_wait( unsigned char status ) {
	unsigned int retry = 0xffff;
	while( ata_status() != status && retry-- );
	return (retry == 0);
}

int ata_check_error( void ) {
	if( ata_status() & ATA_STATUS_ERROR ) {
		outb( ATA_CONTROL, ATA_REG_ERROR );
		return inb( ATA_DATA_LSB );
	}
	return 0;
}

unsigned char ata_config( unsigned char value ) {
	unsigned char config = (ATA_CONFIG_BASE | ATA_CONFIG_LBA | value) & ATA_CONFIG_MASK;
	outb( ATA_CONTROL, config );
	return config;
}

void ata_sector_select( ssize_t addr ) {
	outb( ATA_CONTROL, ATA_REG_SECTOR_0 );
	outb( ATA_DATA_LSB, addr & 0x000000ff );

	outb( ATA_CONTROL, ATA_REG_SECTOR_1 );
	outb( ATA_DATA_LSB, addr & 0x0000ff00 >> 8 );

	outb( ATA_CONTROL, ATA_REG_SECTOR_2 );
	outb( ATA_DATA_LSB, addr & 0x00ff0000 >> 16 );

	outb( ATA_CONTROL, ATA_REG_SECTOR_3 );
	/* watch that high nibble... */
	outb( ATA_DATA_LSB, addr & 0x0f000000 >> 24 );
}

int ata_sector_read( const char *buffer, ssize_t addr ) {
	int i, error = 0;
	char *pos = buffer;
	/* wait until unit is ready */
	if( ata_status_wait(ATA_STATUS_BUSY & ~ATA_STATUS_RDY) ) {
		return ATA_ERROR_TIMEOUT;
	}
	/* select sector address */
	ata_sector_select( addr );
	/* set number of sectors = 1 */
	outb( ATA_CONTROL, ATA_REG_SECTORS );
	outb( ATA_DATA_LSB, 1 );
	/* send read command */
	outb( ATA_CONTROL, ATA_REG_COMMAND );
	outb( ATA_DATA_LSB, ATA_CMD_READ );
	/* wait for data to arrive */
	if( ata_status_wait(ATA_STATUS_BUSY & ~ATA_STATUS_DRQ) ) {
		return ATA_ERROR_TIMEOUT;
	}
	/* check for errors */
	if( error = ata_check_error() ) {
		return error;
	}
	/* read in the block (256 words) */
	outb( ATA_CONTROL, ATA_REG_DATA );
	for( i = 256 ; i > 0 ; i-- ) {
		/* watch byte order... */
		*pos++ = inb( ATA_DATA_MSB );
		*pos++ = inb( ATA_DATA_LSB );
	}
	return 0;
}

void ata_boot( int unit ) {
	int err = 0;
	switch( unit ) {
		case 0:
			ata_config( ATA_CONFIG_MASTER );
			break;
		case 1:
			ata_config( ATA_CONFIG_SLAVE );
			break;
		default:
			errno = ENOUNIT;
			return;
	}
	if( err = ata_sector_read( RAM_START, 0 ) ) {
		con_crlf();
		con_puts( ata_err_str(err) );
		con_crlf();
		con_crlf();
		errno = EINTERNAL;
	}
	else {
		jump( RAM_START );
	}
}

const char *ata_err_str( int err ) {
	switch( err ) {
		case ATA_ERROR_NONE:
			return "No error";
		case ATA_ERROR_DAM:
			return "DAM not found";
		case ATA_ERROR_TRACK:
			return "Track 000 not found";
		case ATA_ERROR_ABORT:
			return "Command aborted";
		case ATA_ERROR_ID:
			return "ID not found";
		case ATA_ERROR_ECC:
			return "Unrecoverable ECC error";
		case ATA_ERROR_BLOCK:
			return "Bad block detected";
		case ATA_ERROR_TIMEOUT:
			return "Timeout";
	}
	return "Unknown error";
}
