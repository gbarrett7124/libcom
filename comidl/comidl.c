/*
 * COM IDL Compiler
 * @(#) $Id$
 */

/*
 * Copyright (c) 2008 Mo McRoberts.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The names of the author(s) of this software may not be used to endorse
 *    or promote products derived from this software without specific prior
 *    written permission.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, 
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY 
 * AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL
 * AUTHORS OF THIS SOFTWARE BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 * TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS 
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "p_comidl.h"

const char *progname = "comidl";
idl_module_t *curmod;

/* Global options */

int nostdinc = 0;
int nodefimports = 0;
int nodefinc = 0;

static void
usage(void)
{
	fprintf(stderr, "Usage: %s [OPTIONS] SOURCE\n"
		"OPTIONS is one or more of:\n"
		"  -H FILE                  Write inteface definition C/C++ header to FILE\n"
		"  -I PATH                  Add PATH to the import search path\n"
		"  -F PATH                  Add PATH to the framework search path\n"
		"  -nostdinc                Reset the import and framework search paths\n"
		"  -nodefimports            Don't perform default imports\n"
		"  -nodefinc                Omit default includes from generated C/C++ headers\n"
		"  -Wp,OPTIONS              Pass OPTIONS to the C preprocessor\n"
		"  -v                       Display version information and exit\n"
		"  -h                       Display this message and exit\n",
		progname);
}

static void
version(void)
{
	fprintf(stderr, "@(#) $Id$\n\n");
	fprintf(stderr, "Copyright (c) 2008 Mo McRoberts\n");
	fprintf(stderr, "Copyright (c) 2007 Novell, Inc. All rights reserved.\n");
	fprintf(stderr, "Copyright (c) 1989 Open Software Foundation, Inc.\n");
	fprintf(stderr, "Copyright (c) 1989 Hewlett-Packard Company\n");
	fprintf(stderr, "Copyright (c) 1989 Digital Equipment Corporation\n");
	fprintf(stderr, "\nhttp://libcom.googlecode.com/\n");
}

int
idl_parse(const char *src, const char *hout, int defimp, int useinc)
{
	FILE *f;
	char fpath[PATH_MAX + 1];
	idl_module_t *lastmod;
	void *scanner;
	
	if(0 == useinc || src[0] == '/')
	{
		if(strlen(src) > PATH_MAX)
		{
			fprintf(stderr, "%s: %s: path too long\n", progname, src);
			return -1;
		}
		strcpy(fpath, src);
	}
	else
	{
		if(-1 == idl_incpath_locate(fpath, sizeof(fpath), src))
		{
			fprintf(stderr, "%s: %s: %s\n", progname, src, strerror(ENOENT));
			return -1;
		}
	}
	if(NULL == (f = fopen(fpath, "rb")))
	{
		fprintf(stderr, "%s: %s: %s\n", progname, fpath, strerror(errno));
		return -1;
	}
	if(1 == defimp && 0 == nodefimports)
	{
		if(-1 == idl_parse("nbase.idl", NULL, 0, 1))
		{
			fclose(f);
			return -1;
		}
	}
	fprintf(stderr, "scanning '%s'\n", fpath);
	if(NULL != idl_module_lookup(fpath))
	{
		fprintf(stderr, "%s has already been imported\n", fpath);
		return 0;
	}
	lastmod = curmod;
	curmod = idl_module_create(fpath, hout);
	curmod->scanner = scanner;
	yylex_init(&scanner);
	yyrestart(f, scanner);
	yyparse(scanner);
	fclose(f);
	idl_module_done(curmod);
	curmod->scanner = NULL;
	curmod = lastmod;
	return 0;
}

int
main(int argc, char **argv)
{
	const char *srcfile, *intfheader, *t;
	char *ih, *s;
	int defaults, c;
	
	if(NULL != argv[0])
	{
		progname = argv[0];
		if(NULL != (t = strrchr(progname, '/')))
		{
			t++;
			progname = t;
		}
	}
	argv[0] = (char *) progname;
	srcfile = NULL;
	intfheader = NULL;
	defaults = 1;
	while((c = getopt(argc, argv, "H:I:F:n:W:hv")) != -1)
	{
		switch(c)
		{
			case 'H':
				intfheader = optarg;
				defaults = 0;
				break;
			case 'I':
				idl_incpath_add_includedir(optarg);
				break;
			case 'F':
				idl_incpath_add_frameworkdir(optarg);
				break;
			case 'h':
				usage();
				exit(EXIT_SUCCESS);
			case 'v':
				version();
				exit(EXIT_SUCCESS);
			default:
				usage();
				exit(EXIT_FAILURE);
		}
	}
	argv += optind;
	argc -= optind;
	if(argc != 1)
	{
		usage();
		exit(EXIT_FAILURE);
	}
	srcfile = argv[0];
	ih = NULL;
	if(defaults)
	{
		if(NULL == (t = strrchr(srcfile, '/')))
		{
			t = srcfile;
		}
		else
		{
			t++;
		}
		ih = (char *) malloc(strlen(t + 8));
		strcpy(ih, t);
		if(strlen(ih) > 4)
		{
			s = strchr(ih, 0);
			s -= 4;
			if(0 == strcasecmp(s, ".idl"))
			{
				s[0] = '.';
				s[1] = 'h';
				s[2] = 0;
			}
			else
			{
				strcat(ih, ".h");
			}
		}
		else
		{
			strcat(ih, ".h");
		}
		intfheader = ih;
	}
	curmod = NULL;
	if(-1 == idl_parse(srcfile, intfheader, 1, 0))
	{
		exit(EXIT_FAILURE);
	}
	free(ih);
	return 0;
}