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

static int idl_emit_cxxinc_open(idl_module_t *module);
static void idl_emit_cxxinc_close(idl_module_t *module);
static void idl_emit_cxxinc_write_header(idl_module_t *module);
static void idl_emit_cxxinc_write_footer(idl_module_t *module);
static void idl_emit_cxxinc_write_indent(idl_module_t *module);

static void idl_emit_write_type(idl_module_t *module, FILE *f, idl_typedecl_t *decl);
static void idl_emit_write_symdef(idl_module_t *module, FILE *f, idl_symdef_t *symdef);
static int idl_emit_write_sym(idl_module_t *module, FILE *f, idl_symdef_t *symdef);

static int
idl_emit_cxxinc_open(idl_module_t *module)
{
	if(NULL == module->hout)
	{
		if(NULL != module->houtname)
		{
			if(NULL == (module->hout = fopen(module->houtname, "wb")))
			{
				fprintf(stderr, "%s: %s: %s\n", progname, module->houtname, strerror(errno));
				exit(EXIT_FAILURE);
			}
			idl_emit_cxxinc_write_header(module);
			return 1;
		}
		return 0;
	}
	return 1;
}

static void
idl_emit_cxxinc_close(idl_module_t *module)
{
	if(NULL != module->hout)
	{
		idl_emit_cxxinc_write_footer(module);
		fclose(module->hout);
		module->hout = NULL;
	}
}

static void
idl_emit_cxxinc_write_header(idl_module_t *module)
{
	fprintf(module->hout, "/* Automatically generated from %s by %s */\n\n", module->shortname, progname);
	fprintf(module->hout, "#ifndef %s\n", module->hmacro);
	fprintf(module->hout, "# define %s\n", module->hmacro);
	if(0 == module->nostdinc)
	{
		fprintf(module->hout, "\n# include \"DCE-RPC/idlbase.h\"\n");
	}
	fprintf(module->hout, "\n"
		"# if defined(__cplusplus)\n"
		"extern \"C\" {\n"
		"# endif\n\n"
		);
	if(0 == module->nostdinc)
	{
		fprintf(module->hout, "# include \"DCE-RPC/nbase.h\"\n\n");
	}
}

static void
idl_emit_cxxinc_write_footer(idl_module_t *module)
{
	size_t c;
	
	fprintf(module->hout, "\n"
		"# if defined(__cplusplus)\n"
		"}\n"
		"# endif\n\n"
		);
	fprintf(module->hout, "#endif /*!%s*/\n", module->hmacro);
	for(c = 0; c < module->nguids; c++)
	{
		fprintf(module->hout, "\n/* %s = {%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x} */\n",
			module->guids[c]->name,
			module->guids[c]->data1, module->guids[c]->data2, module->guids[c]->data3,
			module->guids[c]->data4[0], module->guids[c]->data4[1],
			module->guids[c]->data4[2], module->guids[c]->data4[3], module->guids[c]->data4[4],
			module->guids[c]->data4[5], module->guids[c]->data4[6], module->guids[c]->data4[7]);
		fprintf(module->hout, "DEFINE_GUID(%s, 0x%08x, 0x%04x, 0x%04x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x);\n",
			module->guids[c]->name,
			module->guids[c]->data1, module->guids[c]->data2, module->guids[c]->data3,
			module->guids[c]->data4[0], module->guids[c]->data4[1],
			module->guids[c]->data4[2], module->guids[c]->data4[3], module->guids[c]->data4[4],
			module->guids[c]->data4[5], module->guids[c]->data4[6], module->guids[c]->data4[7]);
	}
}

static void
idl_emit_cxxinc_write_indent(idl_module_t *module)
{
	size_t c;
	
	for(c = 0; c < module->houtdepth; c++)
	{
		fputc('\t', module->hout);
	}
}

static void
idl_emit_write_type(idl_module_t *module, FILE *f, idl_typedecl_t *decl)
{
	size_t c;
	
	(void) module;
	
	if(decl->modifiers & TYPEMOD_CONST)
	{
		fprintf(f, "const ");
	}
	if(decl->builtin_type == TYPE_CHAR ||
		decl->builtin_type == TYPE_INT ||
		decl->builtin_type == TYPE_LONG ||
		decl->builtin_type == TYPE_LONGLONG)
	{
		if(decl->modifiers & TYPEMOD_UNSIGNED)
		{
			fprintf(f, "unsigned ");
		}
		else if(decl->modifiers & TYPEMOD_SIGNED)
		{
			fprintf(f, "signed ");
		}
	}
	switch(decl->builtin_type)
	{
		case TYPE_VOID:
			fprintf(f, "void ");
			break;
		case TYPE_CHAR:
			fprintf(f, "char ");
			break;
		case TYPE_INT:
			fprintf(f, "int ");
			break;
		case TYPE_LONG:
			fprintf(f, "long ");
			break;
		case TYPE_LONGLONG:
			fprintf(f, "long long ");
			break;
		case TYPE_SHORT:
			fprintf(f, "short ");
			break;
		case TYPE_FLOAT:
			fprintf(f, "float ");
			break;
		case TYPE_DOUBLE:
			fprintf(f, "double ");
			break;
		case TYPE_BOOLEAN:
			fprintf(f, "boolean ");
			break;
		case TYPE_STRUCT:
			fprintf(f, "struct %s ", decl->tag);
			break;
		case TYPE_UNION:
			fprintf(f, "union %s ", decl->tag);
			break;
		case TYPE_ENUM:
			fprintf(f, "enum %s ", decl->tag);
			break;
		case TYPE_INTERFACE:
			fprintf(f, "cominterface %s ", decl->tag);
			break;
		case TYPE_DEF:
			fprintf(f, "%s ", decl->user_type->ident);
			break;
		case TYPE_INT8:
			if(decl->modifiers & TYPEMOD_UNSIGNED)
			{
				fprintf(f, "uint8_t ");
			}
			else
			{
				fprintf(f, "int8_t ");
			}
			break;
		case TYPE_INT16:
			if(decl->modifiers & TYPEMOD_UNSIGNED)
			{
				fprintf(f, "uint16_t ");
			}
			else
			{
				fprintf(f, "int16_t ");
			}
			break;
		case TYPE_INT32:
			if(decl->modifiers & TYPEMOD_UNSIGNED)
			{
				fprintf(f, "uint32_t ");
			}
			else
			{
				fprintf(f, "int32_t ");
			}
			break;
		case TYPE_INT64:
			if(decl->modifiers & TYPEMOD_UNSIGNED)
			{
				fprintf(f, "uint64_t ");
			}
			else
			{
				fprintf(f, "int64_t ");
			}
			break;
			
	}
	if(decl->has_symlist)
	{
		fputc('\n', f);
		idl_emit_cxxinc_write_indent(module);
		fputc('{', f);
		fputc('\n', f);
		module->houtdepth++;
		for(c = 0; c < decl->symlist.ndefs; )
		{
			idl_emit_cxxinc_write_indent(module);
			c += idl_emit_write_sym(module, f, decl->symlist.defs[c]);
			fputc(';', module->hout);
			fputc('\n', module->hout);
		}
		module->houtdepth--;
		idl_emit_cxxinc_write_indent(module);
		fputc('}', f);
		fputc(' ', f);
	}
}

static void
idl_emit_write_symdef(idl_module_t *module, FILE *f, idl_symdef_t *symdef)
{
	size_t c;

	(void) module;
	
	for(c = 0; c < symdef->ndeclarator; c++)
	{
		switch(symdef->declarator[c])
		{
			case DECL_POINTER:
				fputc('*', f);
				if(symdef->declarator[c + 1] == DECL_CONST ||
					symdef->declarator[c + 1] == DECL_RESTRICT)
				{
					fputc(' ', f);
				}
				break;
			case DECL_CONST:
				fprintf(f, "const ");
				break;
			case DECL_RESTRICT:
				fprintf(f, "restrict ");
				break;
			case DECL_LBRACKET:
				fputc('(', f);
				break;
			case DECL_RBRACKET:
				fputc(')', f);
				break;
			case DECL_IDENT:
				fprintf(f, "%s", symdef->ident);
				break;
		}
	}
	if(1 == symdef->is_fp || SYM_METHOD == symdef->type)
	{
		if(0 == symdef->fp_params.ndefs)
		{
			fprintf(f, "(void)");
		}
		else
		{
			fputc('(', f);
			for(c = 0; c < symdef->fp_params.ndefs; c++)
			{
				/* We never chain in a list of function parameters */
				symdef->fp_params.defs[c]->nextsym = NULL;
				idl_emit_write_sym(module, f, symdef->fp_params.defs[c]);
				if(c < symdef->fp_params.ndefs - 1)
				{
					fputc(',', f);
					fputc(' ', f);
				}
			}
			fputc(')', f);
		}
	}
	if(symdef->is_array)
	{
		fputc('[', f);
		if(symdef->array_len > 0)
		{
			fprintf(f, "%d", (int) symdef->array_len);
		}
		else
		{
			fputc('1', f);
		}
		fputc(']', f);
	}
}

static int
idl_emit_write_sym(idl_module_t *module, FILE *f, idl_symdef_t *symdef)
{
	idl_symdef_t *p;
	int c;
	
	c = 0;
	idl_emit_write_type(module, f, symdef->decl);
	for(p = symdef; p; p = p->nextsym)
	{
		if(p != symdef)
		{
			fputc(',', module->hout);
			fputc(' ', module->hout);
		}
		idl_emit_write_symdef(module, module->hout, p);
		c++;
	}
	return c;
}

int
idl_emit_init(idl_module_t *module)
{
	(void) module;
	return 0;
}

int
idl_emit_done(idl_module_t *module)
{
	idl_emit_cxxinc_close(module);
	return 0;
}

/* Write a raw string to the generated C/C++ header */
int
idl_emit_cppquote(idl_module_t *module, const char *quote)
{
	if(1 == idl_emit_cxxinc_open(module))
	{
		fprintf(module->hout, "%s\n", quote);
	}
	return 0;
}

/* Write a typdef statement to the generated header */
int
idl_emit_typedef(idl_module_t *module, idl_interface_t *intf, idl_symdef_t *symdef)
{
	(void) intf;
	
	if(1 == idl_emit_cxxinc_open(module))
	{
		fprintf(module->hout, "typedef ");
		idl_emit_write_sym(module, module->hout, symdef);
		fputc(';', module->hout);
		fputc('\n', module->hout);
	}
	return 0;
}

/* Write a local method declaration to the generated header */
int
idl_emit_local_method(idl_module_t *module, idl_interface_t *intf, idl_symdef_t *symdef)
{
	if(1 == intf->local && 0 == intf->object)
	{
		if(1 == idl_emit_cxxinc_open(module))
		{
			idl_emit_write_sym(module, module->hout, symdef);
			fputc(';', module->hout);
			fputc('\n', module->hout);
		}
	}
	return 0;
}

int
idl_emit_const(idl_module_t *module, idl_interface_t *intf, const char *name, long value)
{
	(void) intf;
	
	if(1 == idl_emit_cxxinc_open(module))
	{
		fprintf(module->hout, "# define %s %ld\n", name, value);
	}
	return 0;
}
