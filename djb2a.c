#include "djb2a.h"

// DJB2A is a modification of the DJB2 hash function, created by Daniel J. Bernstein.
// The implementation below was inspired by the following web page:
// http://www.cse.yorku.ca/~oz/hash.html.

u64 djb2a(char* str) {
	u64 hash = DJB2A_MAGIC;
	while (*str) {
		hash = ((hash << 5) + hash) ^ *str++;
	}

	return hash;
}