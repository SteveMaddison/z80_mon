#include "util.h"

static const char *digits = "0123456789abcdef";

void *memcpy( void *dest, const void *src, size_t n )
{
	char *d = dest;
	char *s = (char*)src;
	size_t i;

	for ( i = 0 ; i < n ; i++ )
		*d++ = *s++;

	return dest;
}

void *memset( void *dest, char byte, size_t n )
{
	size_t i;
	char* d = dest;

	for( i = 0 ; i < n ; i++ )
		*d++ = byte;

	return dest;
}

char *strcpy( char* dest, const char* src )
{
	while( (*dest++ = *src++) );

	return dest;
}

int strcmp( const char* this, const char* that ) {
	while ( *this && *that ) {
		if ( *this++ != *that++ ) {
			return 1;
		}
	}
	if ( *this || *that )
		return 1;
	else
		return 0;
}

int strlen( const char *string )
{
	char *pos = (char*)string;
	int length = 0;
	while( *pos++ != '\0' )
		length++;

	return length;
}
