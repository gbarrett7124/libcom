libcom aims to implement the Component Object Model on both POSIX and Win32/Win64 systems, interoperating both with Mozilla XPCOM (if available) and with Windows’ own COM (and DCOM) subsystem.

libcom provides facilities for in-process servers, as hooks for inter-process (or inter-host) communication schemes.

As libcom aims for portability, and because COM is defined by its ABI, libcom interface pointers can be used interchangeably with XPCOM and Win32/64 COM interface pointers. Compatibility macros are provided to assist in porting of IDL-generated header files to libcom applications.

libcom includes the beginnings of an IDL compiler—`comidl`—which can be used to generate interface definition headers (for C/C++) from DCE RPC or Microsoft COM (`midl`) IDL sources.
