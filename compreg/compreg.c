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
#include <unistd.h>
#include "com/com.h"

#ifndef EXIT_FAILURE
# define EXIT_FAILURE                  1
#endif

static void
usage(const char *progname)
{
	fprintf(stderr, 
		"Usage:\n"
		"  %s FILE             Register the classes in FILE\n"
		"  %s -u[ ]FILE        Unregister the classes in FILE\n",
		progname, progname);
}

int
main(int argc, char **argv)
{
	const char *regfile, *arg;
	int unreg, c, prev;
	com_result_t r;
	com_rco_t rco;
	uint32_t key;
	
	regfile = NULL;
	arg = NULL;
	prev = 0;
	/* We don't use getopt so that we can be portable to braindead systems */
	for(c = 1; c < argc; c++)
	{
		if(prev)
		{
			if(argv[c][0] == '-')
			{
				fprintf(stderr, "%s: Option '-%c' requires an argument\n", argv[0], prev);
				exit(EXIT_FAILURE);
			}
		}
		else if(argv[c][0] == '-')
		{
			prev = argv[c][1];
			arg = &(argv[c][2]);
			if(prev == '-')
			{
				if(!argv[c][2])
				{
					break;
				}
				fprintf(stderr, "%s: Unrecognised long option '%s'\n", argv[0], argv[c]);
				exit(EXIT_FAILURE);
			}
			if(!prev)
			{
				fprintf(stderr, "%s: Expected option character after '-'\n", argv[0]);
				exit(EXIT_FAILURE);
			}
			if(!*arg)
			{
				arg = NULL;
				continue;
			}
		}
		else
		{
			if(regfile)
			{
				fprintf(stderr, "%s: Only one file may be specified\n", argv[0]);
				exit(EXIT_FAILURE);
			}
			regfile = argv[c];
		}
		switch(prev)
		{
			case 'u':
				unreg = 1;
				if(regfile)
				{
					fprintf(stderr, "%s: Only one file may be specified\n", argv[0]);
					exit(EXIT_FAILURE);
				}
				regfile = arg;
				break;
			case 'h':
				usage(argv[0]);
				return 0;
			default:
				fprintf(stderr, "%s: Unrecognised option '-%c'\n", argv[0], prev);
				exit(EXIT_FAILURE);
		}
		prev = 0;
		arg = NULL;
	}
	if(prev)
	{
		fprintf(stderr, "%s: Option '-%c' requires an argument\n", argv[0], prev);
	}
	if(!regfile)
	{
		usage(argv[0]);
		exit(EXIT_FAILURE);
	}
	com_init(NULL);
	memset(&rco, 0, sizeof(rco));
	rco.modulepath = regfile;
	rco.ctx = COM_CTX_INPROC_SERVER|COM_CTX_INPROC_HANDLER;
	rco.flags = COM_REG_PERSISTENT;
	if(COM_S_OK != (r = com_register(&rco, &key)))
	{
		fprintf(stderr, "%s: An error occurred while processing the component registration (0x%08x)\n", argv[0], r);
		exit(EXIT_FAILURE);
	}
	com_shutdown();
	return 0;
}
