#ifndef _TYPES_H_
#define _TYPES_H_

#ifndef _CONFIG_H_
#include "config.h"
#endif

typedef unsigned int size_t;
typedef unsigned long ssize_t;

#ifndef NULL
#define NULL ((void*)0)
#endif

typedef enum {
	MODE_NONE,
	MODE_ADDR,
	MODE_REG,
	MODE_PORT
} mode_t;

typedef enum {
	REG_AF,
	REG_BC,
	REG_DE,
	REG_HL,
	REG_IX,
	REG_IY,
	NO_REGS
} reg_id_t;

#endif /* _TYPES_H_ */
