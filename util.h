#ifndef _UTIL_H_
#define _UTIL_H_

#include "types.h"

/* used to prevent compiler warnings */
#define UNUSED(x) (x)=(x)

extern void outb( const char port, const char byte );
extern unsigned char inb( const char port );
extern size_t* get_sp( void );

extern void *memcpy( void* dest, const void* src, size_t n );
extern void *memset( void *dest, char byte, size_t n );

extern char *strcpy( char* dest, const char* src );
extern int strcmp( const char* this, const char* that );
extern int strlen( const char* string );

#endif /* _UTIL_H_ */
