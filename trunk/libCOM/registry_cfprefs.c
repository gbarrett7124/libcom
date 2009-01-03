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

#include "p_libcom.h"

#if defined(COM_USE_CF)

static com_result_t __stdcall CoCFPrefsReg_QueryInterface(ICoRegistry *intf, com_riid_t iid, void **out);
static uint32_t __stdcall CoCFPrefsReg_AddRef(ICoRegistry *intf);
static uint32_t __stdcall CoCFPrefsReg_Release(ICoRegistry *intf);
static com_result_t __stdcall CoCFPrefsReg_RegisterClass(ICoRegistry *intf, com_rco_t *rco, uint32_t *key);
static com_result_t __stdcall CoCFPrefsReg_UnregisterClassKey(ICoRegistry *intf, uint32_t key);
static com_result_t __stdcall CoCFPrefsReg_UnregisterClassID(ICoRegistry *intf, com_rclsid_t clsid);
static com_result_t __stdcall CoCFPrefsReg_GetClassObject(ICoRegistry *intf, com_rclsid_t clsid, com_context_t context, com_server_t *server, com_riid_t riid, void **out);
static com_result_t __stdcall CoCFPrefsReg_WriteString(ICoRegistry *intf, const char *key, const char *entryname, const char *entrydata, com_regflags_t flags);
static com_result_t __stdcall CoCFPrefsReg_WriteDict(ICoRegistry *intf, const char *key, const char *entryname, const char **entrydata, com_regflags_t flags);

static CFURLRef com__registry_cfprefs_path(ICoRegistry *intf, com_regflags_t flags, int create);
static com_result_t com__registry_cfprefs_write(ICoRegistry *intf, const char *keyname, const char *entryname, CFPropertyListRef value, com_regflags_t flags);
static com_result_t com__registry_cfprefs_read(ICoRegistry *intf, const char *keyname, const char *entryname, CFPropertyListRef *value, com_regflags_t flags);

# ifdef COM_USE_PTHREAD
static pthread_mutex_t reglock = PTHREAD_MUTEX_INITIALIZER;
#  define REGLOCK pthread_mutex_lock(&reglock)
#  define REGUNLOCK pthread_mutex_unlock(&reglock)
# else
#  define REGLOCK
#  define REGUNLOCK
# endif

typedef struct localreg_entry_struct localreg_entry_t;

struct localreg_entry_struct
{
	com_rco_t rco;
	uint32_t key;
	uint32_t refcount;
};

static uint32_t entrykey = 1;
static localreg_entry_t *entries;
static size_t nentries;

DECLARE_CLASS(CoCFPrefsReg)
{
	DECLARE_IMPLEMENTS(CoCFPrefsReg, ICoRegistry);
};

DEFINE_CLASS_INTERFACE(CoCFPrefsReg, ICoRegistry)
{
	CoCFPrefsReg_QueryInterface,
	CoCFPrefsReg_AddRef,
	CoCFPrefsReg_Release,
	CoCFPrefsReg_RegisterClass,
	CoCFPrefsReg_UnregisterClassKey,
	CoCFPrefsReg_UnregisterClassID,
	CoCFPrefsReg_GetClassObject
};

DEFINE_STATIC_CLASS(CoCFPrefsReg, com__registry_cf)
{
	DEFINE_STATIC_IMPLEMENTATION(CoCFPrefsReg, ICoRegistry)
};

com_result_t
com__registry_cfprefs_init(void)
{
	ICoRegistry *reg;
	com_result_t r;
	
	INITIALISE_ADDREF_INTERFACE_POINTER(CoCFPrefsReg, &com__registry_cf, ICoRegistry);
	reg = GET_INTERFACE_POINTER(&com__registry_cf, ICoRegistry);
	r = com_registry_add(reg);
	ICoRegistry_Release(reg);
	return r;
}

static CFURLRef
com__registry_cfprefs_path(ICoRegistry *intf, com_regflags_t flags, int create)
{
	const char *appname = "com.googlecode.libcom";
	const char *home;
	struct stat s;
	char buf[PATH_MAX + 1];
	CFStringRef homedir, filename;
	CFStringRef basepath = CFSTR("Library/Application Support/COM Registry/");
	CFURLRef baseurl, homeurl, url;
	
	if(flags & COM_REG_ALLUSERS)
	{
		homedir = NULL;
		homeurl = NULL;
		baseurl = CFURLCreateWithFileSystemPath(kCFAllocatorDefault, basepath, kCFURLPOSIXPathStyle, true);
	}
	else
	{
		if(NULL == (home = getenv("HOME")))
		{
			home = "/";
		}
		homedir = CFStringCreateWithBytes(kCFAllocatorDefault, (const UInt8 *) home, strlen(home), kCFStringEncodingUTF8, false);
		homeurl = CFURLCreateWithFileSystemPath(kCFAllocatorDefault, homedir, kCFURLPOSIXPathStyle, true);
		baseurl = CFURLCreateWithFileSystemPathRelativeToBase(kCFAllocatorDefault, basepath, kCFURLPOSIXPathStyle, true, homeurl);
	}
	CFURLGetFileSystemRepresentation(baseurl, true, buf, sizeof(buf));
	if(1 == create && -1 == stat(buf, &s) && ENOENT == errno)
	{
		mkdir(buf, 0777);
	}
	filename = CFStringCreateWithFormat(kCFAllocatorDefault, NULL, CFSTR("%s.plist"), appname, NULL);
	url = CFURLCreateWithFileSystemPathRelativeToBase(kCFAllocatorDefault, filename, kCFURLPOSIXPathStyle, false, baseurl);
	CFURLGetFileSystemRepresentation(url, true, buf, sizeof(buf));

	if(NULL != filename) CFRelease(filename);
	if(NULL != baseurl) CFRelease(baseurl);
	if(NULL != homeurl) CFRelease(homeurl);
	if(NULL != homedir) CFRelease(homedir);

	return url;
}

static com_result_t
com__registry_cfprefs_write(ICoRegistry *intf, const char *keyname, const char *entryname, CFPropertyListRef value, com_regflags_t flags)
{
	CFPropertyListRef plist, entry;
	CFMutableDictionaryRef mut, entrydict;
	CFURLRef url;
	CFStringRef key, name, errstr;
	CFDataRef data;
	SInt32 error;
	
	errstr = NULL;
	plist = NULL;
	data = NULL;
	url = com__registry_cfprefs_path(intf, flags, 1);
	if(true == CFURLCreateDataAndPropertiesFromResource(kCFAllocatorDefault, url, &data, NULL, NULL, &error))
	{
		if(NULL == (plist = CFPropertyListCreateFromXMLData(kCFAllocatorDefault, data, kCFPropertyListMutableContainersAndLeaves, &errstr)))
		{
			if(NULL != errstr) CFRelease(errstr);
		}
		else if(CFGetTypeID(plist) != CFDictionaryGetTypeID())
		{
			CFRelease(plist);
			plist = NULL;
		}
		if(data != NULL) CFRelease(data);
	}
	if(NULL == plist)
	{
		mut = CFDictionaryCreateMutable(kCFAllocatorDefault, 0, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
	}
	else
	{
		mut = (CFMutableDictionaryRef) plist;
	}
	key = CFStringCreateWithBytes(kCFAllocatorDefault, (const UInt8 *) keyname, strlen(keyname), kCFStringEncodingUTF8, false);
	
	if(NULL == (entry = CFDictionaryGetValue(mut, key)) || CFGetTypeID(entry) != CFDictionaryGetTypeID())
	{
		entrydict = CFDictionaryCreateMutable(kCFAllocatorDefault, 0, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
		CFDictionarySetValue(mut, key, entrydict);
	}
	else
	{
		entrydict = (CFMutableDictionaryRef) entry;
		CFRetain(entrydict);
	}
	name = CFStringCreateWithBytes(kCFAllocatorDefault, (const UInt8 *) entryname, strlen(entryname), kCFStringEncodingUTF8, false);
	CFDictionarySetValue(entrydict, name, value);
	data = CFPropertyListCreateXMLData(kCFAllocatorDefault, mut);
	CFURLWriteDataAndPropertiesToResource(url, data, NULL, &error);
	CFRelease(entrydict);
	if(mut != NULL && mut != plist) CFRelease(mut);
	if(plist != NULL) CFRelease(plist);
	return COM_S_OK;
}

static com_result_t __stdcall
CoCFPrefsReg_QueryInterface(ICoRegistry *intf, com_riid_t iid, void **out)
{
	if(com_guid_equal(iid, &IID_IUnknown) || com_guid_equal(iid, &IID_ICoRegistry))
	{
		ICoRegistry_AddRef(intf);
		*out = intf;
		return COM_S_OK;
	}
	return COM_E_NOINTERFACE;
}

static uint32_t __stdcall
CoCFPrefsReg_AddRef(ICoRegistry *intf)
{
	(void) intf;

	return 2;
}

static uint32_t __stdcall
CoCFPrefsReg_Release(ICoRegistry *intf)
{
	(void) intf;

	return 1;
}

static com_result_t __stdcall
CoCFPrefsReg_RegisterClass(ICoRegistry *intf, com_rco_t *rco, uint32_t *key)
{
	localreg_entry_t *q;
	size_t c, freeslot;
	char buf[64];
	const char *regdict[5] = { "ProgID", NULL, "InprocServer", NULL, NULL};
	
	(void) intf;

	if(COM_REG_PERSISTENT != (rco->flags & COM_REG_PERSISTENT) || NULL == rco->modulepath)
	{
		return COM_E_NOT_SUPPORTED;
	}
	sprintf(buf, "{%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x}",
		rco->clsid->data1, rco->clsid->data2, rco->clsid->data3,
		rco->clsid->data4[0], rco->clsid->data4[1],
		rco->clsid->data4[2], rco->clsid->data4[3], rco->clsid->data4[4], rco->clsid->data4[5], rco->clsid->data4[6], rco->clsid->data4[7]);
	if(NULL != rco->contractid)
	{
		CoCFPrefsReg_WriteString(intf, "ProgID", rco->contractid, buf, rco->flags);
	}
	regdict[1] = rco->contractid;
	regdict[3] = rco->modulepath;
	CoCFPrefsReg_WriteDict(intf, "Classes", buf, regdict, rco->flags);
	
	REGLOCK;
	freeslot = 0;
	for(c = 0; c < nentries; c++)
	{
		if(0 == entries[c].refcount)
		{
			freeslot = c + 1;
		}
		else if(0 != entries[c].refcount && com_guid_equal(rco->clsid, entries[c].rco.clsid))
		{
			entries[c].refcount++;
			*key = entries[c].key;
			REGUNLOCK;
			return COM_S_OK;
		}
	}
	if(0 != freeslot)
	{
		q = &(entries[freeslot - 1]);
	}
	else
	{
		if(NULL == (q = (localreg_entry_t *) realloc(entries, sizeof(localreg_entry_t) * (nentries + 1))))
		{
			REGUNLOCK;
			return COM_E_OUTOFMEMORY;
		}
		entries = q;
		q = &(entries[nentries]);
	}
	memset(q, 0, sizeof(localreg_entry_t));
	if(NULL != rco->factory)
	{
		IUnknown_AddRef(rco->factory);
	}
	q->refcount = 1;
	memcpy(&(q->rco), rco, sizeof(com_rco_t));
	*key = q->key = entrykey;
	nentries++;
	entrykey++;
	REGUNLOCK;
	return COM_S_OK;
}

static com_result_t __stdcall
CoCFPrefsReg_UnregisterClassKey(ICoRegistry *intf, uint32_t key)
{
	size_t c;

	(void) intf;

	REGLOCK;
	for(c = 0; c < nentries; c++)
	{
		if(0 != entries[c].refcount && entries[c].key == key)
		{
			entries[c].refcount--;
			if(0 == entries[c].refcount)
			{
				if(NULL != entries[c].rco.factory)
				{
					IUnknown_Release(entries[c].rco.factory);
				}
				memset(&(entries[c]), 0, sizeof(localreg_entry_t));
			}
			REGUNLOCK;
			return COM_S_OK;
		}
	}
	REGUNLOCK;
	return COM_E_CLASSNOTREG;
}

static com_result_t __stdcall
CoCFPrefsReg_UnregisterClassID(ICoRegistry *intf, com_rclsid_t clsid)
{
	size_t c;

	(void) intf;

	REGLOCK;
	for(c = 0; c < nentries; c++)
	{
		if(0 != entries[c].refcount && com_guid_equal(entries[c].rco.clsid, clsid))
		{
			entries[c].refcount--;
			if(0 == entries[c].refcount)
			{
				IUnknown_Release(entries[c].rco.factory);
				memset(&(entries[c]), 0, sizeof(localreg_entry_t));
			}
			REGUNLOCK;
			return COM_S_OK;
		}
	}
	REGUNLOCK;
	return COM_E_CLASSNOTREG;
}

static com_result_t __stdcall
CoCFPrefsReg_GetClassObject(ICoRegistry *intf, com_rclsid_t clsid, com_context_t context, com_server_t *server, com_riid_t riid, void **out)
{
	size_t c;
	localreg_entry_t *p;
	com_result_t r;

	(void) intf;

	p = NULL;
	REGLOCK;
	for(c = 0; c < nentries; c++)
	{
		if(0 != entries[c].refcount && com_guid_equal(entries[c].rco.clsid, clsid))
		{
			p = &(entries[c]);
			p->refcount++;
			REGUNLOCK;
			break;
		}
	}
	REGUNLOCK;
	if(NULL == p)
	{
		return COM_E_CLASSNOTREG;
	}
	if(NULL != p->rco.factory)
	{
		r = IUnknown_QueryInterface(p->rco.factory, riid, out);
		p->refcount--;
		return r;
	}
	return COM_E_CLASSNOTREG;
}

static com_result_t __stdcall
CoCFPrefsReg_WriteString(ICoRegistry *intf, const char *keyname, const char *entryname, const char *entrydata, com_regflags_t flags)
{
	com_result_t r;
	CFStringRef value;
	
	if(NULL == entrydata)
	{
		value = NULL;
	}
	else
	{
		value = CFStringCreateWithBytes(kCFAllocatorDefault, (const UInt8 *) entrydata, strlen(entrydata), kCFStringEncodingUTF8, false);
	}
	r = com__registry_cfprefs_write(intf, keyname, entryname, value, flags);
	if(NULL != value) CFRelease(value);
	return r;
}

static com_result_t __stdcall
CoCFPrefsReg_WriteDict(ICoRegistry *intf, const char *keyname, const char *entryname, const char **entrydata, com_regflags_t flags)
{
	com_result_t r;
	CFStringRef k, v;
	CFMutableDictionaryRef dict;
	size_t c;
	
	dict = CFDictionaryCreateMutable(kCFAllocatorDefault, 0, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
	for(c = 0; NULL != entrydata[c]; c++)
	{
		k = CFStringCreateWithBytes(kCFAllocatorDefault, (const UInt8 *) entrydata[c], strlen(entrydata[c]), kCFStringEncodingUTF8, false);
		c++;
		if(NULL == entrydata[c])
		{
			v = NULL;
		}
		else
		{
			v = CFStringCreateWithBytes(kCFAllocatorDefault, (const UInt8 *) entrydata[c], strlen(entrydata[c]), kCFStringEncodingUTF8, false);
		}
		CFDictionarySetValue(dict, k, v);
		CFRelease(k);
		CFRelease(v);
	}
	r = com__registry_cfprefs_write(intf, keyname, entryname, dict, flags);
	CFRelease(dict);
	return COM_S_OK;
}

#endif /* COM_USE_CF */