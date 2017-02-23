/*
 * Copyright (c) 2005-2006, Doug Harple.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of author nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * $Id: parse_options_generic.c,v 1.8 2006/07/03 16:45:54 purgedhalo Exp $
 *
 */

#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include "detox.h"
#include "config.h"

#undef HAVE_LONG_OPTIONS

#ifdef HAVE_LIBPOPT
#define HAVE_LONG_OPTIONS
#endif

#ifdef HAVE_GETOPT_LONG
#define HAVE_LONG_OPTIONS
#endif

/* *INDENT-OFF* */

#ifndef INLINE_MODE

char usage_message[] = {
	"usage: detox [-hLnrvV] [-f configfile] [-s sequence]"
#ifdef HAVE_LONG_OPTIONS
	" [--dry-run] [--special]"
	"\n\t "
#endif
	" file [file ...]\n"
};

char help_message[] = {
	"	-f configfile	choose which config file to use\n"
#ifdef HAVE_LONG_OPTIONS
	"	-h --help	this message\n"
#else
	"	-h 		this message\n"
#endif
	"	-L		list available sequences and exit\n"
	"			with -v ... dump sequence contents\n"
#ifdef HAVE_LONG_OPTIONS
	"	-n --dry-run	do a dry run (don't actually do anything)\n"
#else
	"	-n 		do a dry run (don't actually do anything)\n"
#endif
	"	-r 		be recursive (descend into subdirectories)\n"
#ifdef HAVE_LONG_OPTIONS
	"	--remove-trailing (deprecated)\n"
	"			remove trailing _ and - before a period\n"
#endif
	"	-s sequence	choose which sequence to detox with\n"
#ifdef HAVE_LONG_OPTIONS
	"	--special	work on links and special files\n"
#endif
	"	-v 		be verbose\n"
	"	-V 		show the current version\n"
};

#else /* INLINE_MODE */

char usage_message[] = {
	"usage: inline-detox [-hLvV] [-f configfile] [-s sequence] [file]\n"
};

char help_message[] = {
	"	-f configfile	choose which config file to use\n"
#ifdef HAVE_LONG_OPTIONS
	"	-h --help	this message\n"
#else
	"	-h 		this message\n"
#endif
	"	-L		list available sequences and exit\n"
	"			with -v ... dump sequence contents\n"
#ifdef HAVE_LONG_OPTIONS
	"	--remove-trailing (deprecated)\n"
	"			remove trailing _ and - before a period\n"
#endif
	"	-s sequence	choose which sequence to detox with\n"
	"	-v 		be verbose\n"
	"	-V 		show the current version\n"
};

#endif

/* *INDENT-ON* */

struct detox_options *initialize_main_options(void)
{
	struct detox_options *main_options;

	main_options = malloc(sizeof(struct detox_options));
	if (main_options == NULL) {
		fprintf(stderr, "out of memory: %s\n", strerror(errno));
		return NULL;
	}

	memset(main_options, 0, sizeof(struct detox_options));

	/*
	 * XXX - handle blank strings better
	 */
	main_options->sequence_name = getenv("DETOX_SEQUENCE");

	return main_options;
}
