#ifndef _CONFIG_H_
#define _CONFIG_H_

/*
 * 16Kb ROM at 0x0000
 */
#define ROM_START		0x0000
#define ROM_SIZE		0x4000
#define ROM_END			((ROM_START)+(ROM_SIZE)-1)

/*
 * RAM starts at 0x4000
 */
#define RAM_SIZE		0xc000	/* 48Kb */
#define RAM_START		0x4000
#define RAM_END			((RAM_START)+(RAM_SIZE)-1)

typedef unsigned int addr_t;
typedef unsigned int regpair_t;
typedef unsigned char word_t;
typedef unsigned char reg_t;

/* Base addresses of devices */
#define UART_ADDR 0x20
#define ATA_ADDR 0x40

#endif /* _CONFIG_H_ */
