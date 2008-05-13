#ifndef _ERRNO_H_
#define _ERRNO_H_

/* devices */
#define ENOUNIT		0x20	/* no such unit */
#define EINTERNAL	0x21	/* device-specific error */

extern const char* strerr( int err );

#endif /* _ERRNO_H_ */
