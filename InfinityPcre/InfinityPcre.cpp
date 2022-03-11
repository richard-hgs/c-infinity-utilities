#include "pch.h"

#define PCRE2_CODE_UNIT_WIDTH 16

#include "InfinityPcre.h"

#include "pcre2.h"

int testPcre() {
	pcre2_code* re;
	PCRE2_SPTR pattern;     /* PCRE2_SPTR is a pointer to unsigned code units of */
	PCRE2_SPTR subject;     /* the appropriate width (8, 16, or 32 bits). */
	PCRE2_SPTR name_table;

	int crlf_is_newline;
	int errornumber;
	int find_all;
	int i;
	int namecount;
	int name_entry_size;
	int rc;
	int utf8;

	uint32_t option_bits;
	uint32_t newline;

	PCRE2_SIZE erroroffset;
	PCRE2_SIZE* ovector;

	size_t subject_length;
	pcre2_match_data* match_data;

	pattern = (PCRE2_SPTR) L"[A-Z0-9]*";
	subject = (PCRE2_SPTR) L"A1\0";
	subject_length = strlen((char*) subject);

	/*************************************************************************
	* Now we are going to compile the regular expression pattern, and handle *
	* any errors that are detected.                                          *
	*************************************************************************/

	re = pcre2_compile(
		pattern,               /* the pattern */
		PCRE2_ZERO_TERMINATED, /* indicates pattern is zero-terminated */
		0,                     /* default options */
		&errornumber,          /* for error number */
		&erroroffset,          /* for error offset */
		NULL);                 /* use default compile context */

	  /* Compilation failed: print the error message and exit. */

	if (re == NULL) {
		PCRE2_UCHAR buffer[256];
		pcre2_get_error_message(errornumber, buffer, sizeof(buffer));
		printf("PCRE2 compilation failed at offset %d: %s\n", (int) erroroffset, buffer);
		return -1;
	}


	/*************************************************************************
	* If the compilation succeeded, we call PCRE again, in order to do a     *
	* pattern match against the subject string. This does just ONE match. If *
	* further matching is needed, it will be done below. Before running the  *
	* match we must set up a match_data block for holding the result.        *
	*************************************************************************/

	/* Using this function ensures that the block is exactly the right size for
	the number of capturing parentheses in the pattern. */

	match_data = pcre2_match_data_create_from_pattern(re, NULL);

	rc = pcre2_match(
		re,                   /* the compiled pattern */
		subject,              /* the subject string */
		subject_length,       /* the length of the subject */
		0,                    /* start at offset 0 in the subject */
		0,                    /* default options */
		match_data,           /* block for storing the result */
		NULL);                /* use default match context */

	  /* Matching failed: handle error cases */

	if (rc < 0) {
		switch (rc) {
			case PCRE2_ERROR_NOMATCH: printf("No match\n"); break;
				/*
				Handle other special cases if you like
				*/
			default: printf("Matching error %d\n", rc); break;
		}
		pcre2_match_data_free(match_data);   /* Release memory used for the match */
		pcre2_code_free(re);                 /* data and the compiled pattern. */
		return -1;
	}

	/* Match succeded. Get a pointer to the output vector, where string offsets are
	stored. */

	ovector = pcre2_get_ovector_pointer(match_data);
	printf("\nMatch succeeded at offset %d\n", (int)ovector[0]);


	/*************************************************************************
	* We have found the first match within the subject string. If the output *
	* vector wasn't big enough, say so. Then output any substrings that were *
	* captured.                                                              *
	*************************************************************************/

	/* The output vector wasn't big enough. This should not happen, because we used
	pcre2_match_data_create_from_pattern() above. */

	if (rc == 0) {
		printf("ovector was not big enough for all the captured substrings\n");
	}

	/* Show substrings stored in the output vector by number. Obviously, in a real
	application you might want to do things other than print them. */

	for (i = 0; i < rc; i++) {
		PCRE2_SPTR substring_start = subject + ovector[2 * i];
		size_t substring_length = ovector[2 * i + 1] - ovector[2 * i];
		printf("%2d: %.*s\n", i, (int) substring_length, (char*) substring_start);
	}


	/**************************************************************************
	* That concludes the basic part of this demonstration program. We have    *
	* compiled a pattern, and performed a single match. The code that follows *
	* shows first how to access named substrings, and then how to code for    *
	* repeated matches on the same subject.                                   *
	**************************************************************************/
}