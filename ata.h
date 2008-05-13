#ifndef _ATA_H_
#define _ATA_H_

/* The ATA interface makes use of three I/O ports; two for the 16-bit ATA
 * data bus and one for the remaining address and CS signals (referred to
 * here as the "control" port). */
/* ATA_ADDR defined in config.h */
#define ATA_DATA_LSB	(ATA_ADDR)
#define ATA_DATA_MSB	((ATA_ADDR)+1)
#define ATA_CONTROL	((ATA_ADDR)+2)

/* ATA Regsiters (values for the control port)
 * Bit 0-2 = Addr0-2, Bit 3 = /CS0, Bit 4 = /CS1 */
#define ATA_REG_DATA		0x10	/* 0: Data port */
#define ATA_REG_ERROR		0x11	/* 1: Error code is read-only */
#define ATA_REG_SECTORS		0x12	/* 2: Sectors to transfer */
/* LBA sector address is a 28-bit value made up of the following 3 bytes
 * and bits 0-3 of the fourth. */
#define ATA_REG_SECTOR_0	0x13	/* 3: low byte */
#define ATA_REG_SECTOR_1	0x14	/* 4: lower-middle byte */
#define ATA_REG_SECTOR_2	0x15	/* 5: upper-middle byte */
#define ATA_REG_SECTOR_3	0x16	/* 6: bits 0-3 high nibble */
#define ATA_REG_CONFIG		ATA_REG_SECTOR_3
/* Following register has two purposes */
#define ATA_REG_STATUS		0x17		/* When read */
#define ATA_REG_COMMAND		ATA_REG_STATUS	/* When written */

/* ATA error codes */
#define ATA_ERROR_NONE		0x00
#define ATA_ERROR_DAM		0x01
#define ATA_ERROR_TRACK		0x02
#define ATA_ERROR_ABORT		0x04
#define ATA_ERROR_ID		0x10
#define ATA_ERROR_ECC		0x40
#define ATA_ERROR_BLOCK		0x80
/* Not an official ATA error code - used internally to signal timeout */
#define ATA_ERROR_TIMEOUT	0xff

/* Masks for remaining bits (4-8) of register 6 */
#define ATA_CONFIG_BASE		0xa0	/* "Always on" bits */
#define ATA_CONFIG_MASTER	0x00	/* Access master device */
#define ATA_CONFIG_SLAVE	0x10	/* Access slave device */
#define ATA_CONFIG_LBA		0x40	/* Enable LBA */
#define ATA_CONFIG_MASK		0x0f

/* Masks for bits in status register */
#define ATA_STATUS_ERROR	0x01	/* Last command resulted in error */
#define ATA_STATUS_DRQ		0x08	/* Data Request Ready (sector buffer ready) */
#define ATA_STATUS_DF		0x20	/* Write fault */
#define ATA_STATUS_RDY		0x40	/* Ready for command */
#define ATA_STATUS_BUSY		0x80	/* Executing command */

/* IDE commands */
#define ATA_CMD_READ		0x20	/* Read sectors with retry */
#define ATA_CMD_WRITE		0x30	/* Write sectors with retry */
#define ATA_CMD_ID		0xec	/* Identify device */


const char *ata_err_str( int err );
void ata_boot( int unit );

#endif /* _ATA_H_ */
