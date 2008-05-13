#include "errno.h"

/* Global errno contains the last error encountered */
int errno;

/* Returns an English description of an error number. */
const char* strerr( int err )
{
	switch( err ) {
		case ENOUNIT:
			return "No such unit";
		case ETIMEOUT:
			return "Timeout";		
	}
	return "Unknown error";
}

