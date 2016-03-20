#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "strlib.h"

//This function relocate memory and concatenate two dynamic string
//The string needs to be freed after calling.
char* concat(char *s1, char *s2) {
	char *result = malloc(strlen(s1) + strlen(s2) + 1); //+1 for the zero-terminator
	if (result == NULL) {
		fprintf(stderr, "failed to allocate memory.\n");
	}
	strcpy(result, s1);
	strcat(result, s2);
	return result;
}

/// Note: This function returns a pointer to a substring of the original string.
/// If the given string was allocated dynamically, the caller must not overwrite
/// that pointer with the returned value, since the original pointer must be
/// deallocated using the same allocator with which it was allocated.  The return
/// value must NOT be deallocated using free() etc.
char *trimSpace(char *str) {
	char *end;
	// Trim leading space
	while (isspace(*str))
		str++;
	if (*str == 0)  // All spaces?
		return str;
	// Trim trailing space
	end = str + strlen(str) - 1;
	while (end > str && isspace(*end))
		end--;
	// Write new null terminator
	*(end + 1) = 0;

	return str;
}

///This function removes certain char in a string.
///returns the processed string.
char* removeChar(char *str) {
	char *SourcePtr = str;
	char *TargetPtr = SourcePtr;
	while (*SourcePtr != 0) {
		if (*SourcePtr != '\r' && *SourcePtr != '\n' && *SourcePtr != ' ') {
			*TargetPtr = *SourcePtr;
			TargetPtr += 1;
		}
		SourcePtr += 1;
	}
	*TargetPtr = 0;
	return str;
}
