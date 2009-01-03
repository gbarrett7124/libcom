## @(#) $Id$

BUILT_SOURCES = nbase.h lbase.h uuid.h rpc.h

dcerpcinclude_HEADERS = DCE-RPC.h idlbase.h $(BUILT_SOURCES) nbase.idl lbase.idl uuid.idl

%.h: $(srcdir)/%.idl
	$(COMIDL) -H$@ -nostdinc -I${srcdir} $<

