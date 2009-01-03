/*
 * libcom: An implementation of the Component Object Model
 * @(#) $Id$
 */

/*
 * Copyright (c) 2004, 2005, 2006, 2007, 2008, 2009 Mo McRoberts.
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

#include <stdio.h>
#include <stdlib.h>
#ifdef HAVE_UNISTD_H
# include <unistd.h>
#endif

#include "COM/COM.h"


#ifndef HAVE_GETOPT
extern int opterr, optind, optopt, optreset;
extern char *optarg;
extern int getopt(int argc, char * const argv[], const char *options);
#endif

static const char *progname = "guidgen";

static void
usage(void)
{
	fprintf(stderr, "Usage: %s [OPTIONS]\n"
		"OPTIONS is one or more of:\n"
		"  -x                       Generate sequential (v1) UUIDs\n"
		"  -i                       Output an IDL interface template\n"
		"  -s                       Output as a C DEFINE_GUID() statement\n"
		"  -c                       Output in upper-case\n"
		"  -o FILE                  Write output to FILE\n"
		"  -n COUNT                 Generate COUNT UUIDs\n"
		"  -v                       Display version information and exit\n"
		"  -h                       Display this message and exit\n",
		progname);
}

static void
version(void)
{
	fprintf(stderr, "@(#) $Id$\n\n");
	fprintf(stderr, "Copyright (c) 2007, 2008 Mo McRoberts\n");
#ifndef HAVE_GETOPT
	fprintf(stderr, "Copyright (c) 1987, 1993, 1994\nThe Regents of the University of California.  All rights reserved.\n");
#endif
	fprintf(stderr, "\nhttp://libcom.googlecode.com/\n");
}

int
main(int argc, char **argv)
{
	const char *t, *outfile;
	int c, outmode, nguids, v1;
	char buf[64];
	com_guid_t guid;
	com_guidstr_t strops;
	
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
	outfile = NULL;
	strops = COM_GUIDSTR_DEFAULT;
	outmode = 0;
	nguids = 1;
	v1 = 0;
	while((c = getopt(argc, argv, "xisco:n:vh")) != -1)
	{
		switch(c)
		{
			case 'x':
				v1 = 1;
				break;
			case 'i':
				outmode = 1;
				break;
			case 's':
				outmode = 2;
				break;
			case 'o':
				outfile = optarg;
				break;
			case 'n':
				break;
			case 'c':
				strops |= COM_GUIDSTR_UPPERCASE;
				break;
			case 'v':
				version();
				exit(EXIT_SUCCESS);
			case 'h':
				usage();
				exit(EXIT_SUCCESS);
			default:
				usage();
				exit(EXIT_FAILURE);
		}
	}
	argc -= optind;
	argv += optind;
	if(argc)
	{
		usage();
		exit(EXIT_FAILURE);
	}
	while(nguids)
	{
		com_guid_generate(&guid, COM_GUIDGEN_DEFAULT);
		switch(outmode)
		{
			case 0:
				com_guid_to_string(&guid, buf, sizeof(buf), strops);
				puts(buf);
				break;
			case 1:
				com_guid_to_string(&guid, buf, sizeof(buf), COM_GUIDSTR_DEFAULT);
				printf("[uuid(%s), version(1.0)]\ninterface NAME\n{\n\n}\n", buf);
				break;
			case 2:
				com_guid_to_string(&guid, buf, sizeof(buf), COM_GUIDSTR_BRACES);
				printf("/* %s */\nDEFINE_GUID(NAME, 0x%08x, 0x%04x, 0x%04x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x);\n",
					buf, guid.data1, guid.data2, guid.data3,
					guid.data4[0], guid.data4[1], guid.data4[2], guid.data4[3],
					guid.data4[4], guid.data4[5], guid.data4[6], guid.data4[7]);
				break;
		}
		nguids--;
	}
	return 0;
}
