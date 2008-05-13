#include "errno.h"

/* Returns an English description of an error number. */
const char* strerr( int err )
{
	switch( err ) {
		case ENOUNIT:
			return "No such unit";
		case EINTERNAL:
			return "Internal error";		
	}
	return "Unknown error";
}

