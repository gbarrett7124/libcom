/*
 * libcom: An implementation of the Component Object Model
 * @(#) $Id$
 */

/*
 * Copyright (c) 2002, 2003, 2004, 2005, 2006, 2007, 2008 Mo McRoberts.
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

#ifndef EXIT_FAILURE
# define EXIT_FAILURE                  1
#endif

#ifndef HAVE_GETOPT
extern int opterr, optind, optopt, optreset;
extern char *optarg;
extern int getopt(int argc, char * const argv[], const char *options);
#endif

static const char *progname = "compreg";

static void
usage(void)
{
	fprintf(stderr, 
		"Usage:\n"
		"  %s [OPTIONS] FILE        Register the classes in FILE\n"
		"  %s [OPTIONS] -u[ ]FILE   Unregister the classes in FILE\n"
		"OPTIONS is one or more of:\n"
		"  -a APPID                 Register per-application classes for APPID\n"
		"  -g                       Perform registrations globally (default is per-user)\n"
		"  -v                       Display version information and exit\n"
		"  -h                       Display this message and exit\n",
		progname, progname);
}

static void
version(void)
{
	fprintf(stderr, "@(#) $Id$\n");
}

int
main(int argc, char **argv)
{
	const char *regfile, *appid, *t;
	int unreg, c;
	com_result_t r;
	com_rco_t rco;

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
	appid = NULL;
	regfile = NULL;
	memset(&rco, 0, sizeof(rco));
	rco.flags = COM_REG_PERSISTENT;
	unreg = 0;
	
	while((c = getopt(argc, argv, "a:ugvh")) != -1)
	{
		switch(c)
		{
			case 'u':
				unreg = 1;
				break;
			case 'a':
				appid = optarg;
				break;
			case 'g':
				rco.flags |= COM_REG_ALLUSERS;
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
	if(argc != 1)
	{
		usage();
		exit(EXIT_FAILURE);
	}
	regfile = argv[0];
	com_init(appid);
	rco.modulepath = regfile;
	rco.ctx = COM_CTX_INPROC_SERVER|COM_CTX_INPROC_HANDLER;
	if(COM_S_OK != (r = com_register(&rco, NULL)))
	{
		fprintf(stderr, "%s: An error occurred while processing the component registration (0x%08x)\n", argv[0], r);
		exit(EXIT_FAILURE);
	}
	com_shutdown();
	return 0;
}
