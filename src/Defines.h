#ifndef ACPL_DEFINES_H
#define ACPL_DEFINES_H


#include "acpl.h"


// 
// Compiler definitions
// 

#define acplCompilerUnknown		0
#define acplCompilerGcc			1
#define acplCompilerMsvc		2


// 
// C standard library definitions
// 

#define acplCRuntimeUnknown		0
#define acplCRuntimeGlibc		1
#define acplCRuntimeMscrt		2


// 
// Platform definitions
// 

#define acplPlatformUnknown		0
#define acplPlatformLinux		1
#define acplPlatformMsWin		2


// 
// Target architecture definitions
// 

#define acplArchUnknown			0
#define acplArchX8632			1
#define acplArchX8664			2
#define acplArchIA32			acplArchX8632
#define acplArchIA64			3
#define acplArchAMD64			acplArchX8664
#define acplArchARM				4
#define acplArchHPPA			5


// 
// Environment detection
// 

#if defined(acplPlatform)
#	error Platform already defined.
#endif


#if defined(__GNUC__)

	// 
	// GNU Compiler Collection (GCC)
	// 

#	define acplPredefVersion(N_MAJOR, N_MINOR, N_FRACTION) \
		((N_MAJOR * 1000000) + (N_MINOR * 1000) + N_FRACTION)

#	include <features.h>

	// Compiler type
#	define acplCompiler			acplCompilerGcc

	// Compiler version
#	if defined(__GNUC_PATCHLEVEL__)
#		define acplCompilerVer	acplPredefVersion(__GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__)
#	else
#		define acplCompilerVer	acplPredefVersion(__GNUC__, __GNUC_MINOR__, 0)
#	endif

	// C standard library
#	if defined(__GNU_LIBRARY__) && __GNU_LIBRARY__ < 6
#		define acplCRuntime		acplCRuntimeGlibc
#		define acplCRuntimeVer	acplPredefVersion(__GNU_LIBRARY__, __GNU_LIBRARY_MINOR__, 0)
#	elif defined(__GLIBC__) // __GNU_LIBRARY__ >= 6
#		define acplCRuntime		acplCRuntimeGlibc
#		define acplCRuntimeVer	acplPredefVersion(__GLIBC__, __GLIBC_MINOR__, 0)
#	else
#		define acplCRuntime		acplCRuntimeUnknown
#		define acplCRuntimeVer	0
#	endif

	// Platform type
#	if defined(__linux__)
#		define acplPlatform		acplPlatformLinux
#	else
#		define acplPlatform		acplPlatformUnknown
#	endif

	// Target architecture type
#	if defined(__i386__)
#		define acplArch			acplArchX8632
#	elif defined(__x86_64__)
#		define acplArch			acplArchX8664
#	elif defined(__ia64__)
#		define acplArch			acplArchIA64
#	elif defined(__arm__)
#		define acplArch			acplArchARM
#	elif defined(__hppa__)
#		define acplArch			acplArchHPPA
#	else
#		define acplArch			acplArchUnknown
#	endif

	// DLL export defs
#	if defined(_acplBuiltAsShared)
#		define acplDllExport   __attribute__((visibility("default")))
#		define acplDllNoExport __attribute__((visibility("hidden")))
#	else
#		define acplDllExport
#		define acplDllNoExport
#	endif


#elif defined(_MSC_VER)

	// 
	// Microsoft Visual C/C++
	// 

#	define acplPredefVersion(N_MAJOR, N_MINOR, N_FRACTION) \
		((N_MAJOR * 10000000) + (N_MINOR * 100000) + N_FRACTION)

#	include <sdkddkver.h>

	// Compiler type
#	define acplCompiler			acplCompilerMsvc

	// Compiler version
#	if defined(_MSC_FULL_VER)
#		define acplCompilerVer	_MSC_FULL_VER // _MSC_FULL_VER is already in the correct all-in-one version number format
#	else
#		define acplCompilerVer	(_MSC_VER * 100000) // _MSC_VER is missing the (5-digit) build component, so we multiply it to fit the all-in-one version number format
#	endif

	// C standard library
#	define acplCRuntime			acplCRuntimeMscrt
#	define acplCRuntimeVer		acplPredefVersion((_WIN32_WINNT >> 8), (_WIN32_WINNT & 0xFF), 0)

	// Platform type
#	define acplPlatform			acplPlatformMsWin

	// Target architecture
#	if defined(_M_IX86)
#		define acplArch			acplArchX8632
#	elif defined(_M_X64)
#		define acplArch			acplArchX8664
#	elif defined(_M_IA64)
#		define acplArch			acplArchIA64
#	else
#		define acplArch			acplArchUnknown
#	endif

	// DLL export defs
#	if defined(_acplBuiltAsShared)
#		if defined(_acplBuildProcess)
#			define acplDllExport __declspec(dllexport)
#		else
#			define acplDllExport __declspec(dllimport)
#		endif
#		define acplDllNoExport
#	else
#		define acplDllExport
#		define acplDllNoExport
#	endif

#else

	// 
	// Unrecognized compiler
	// 

#	define acplPredefVersion(N_MAJOR, N_MINOR, N_FRACTION)

#	define acplCompiler			acplCompilerUnknown
#	define acplCompilerVer		0
#	define acplCRuntime			acplCRuntimeUnknown
#	define acplCRuntimeVer		0
#	define acplPlatform			acplPlatformUnknown
#	define acplArch				acplArchUnknown

#endif


// 
// Definitions check
// 

#if !defined(acplCompiler) || acplCompiler == acplCompilerUnknown
#	error Unrecognized compiler.
#endif

#if !defined(acplCompilerVer) || acplCompilerVer == 0
#	error Invalid compiler version number.
#endif

#if !defined(acplCRuntime) || acplCRuntime == acplCRuntimeUnknown
#	error Unrecognized C standard library.
#endif

#if !defined(acplCRuntimeVer) || acplCRuntimeVer == 0
#	error Invalid C standard library version number.
#endif

#if !defined(acplPlatform) || acplPlatform == acplPlatformUnknown
#	error Unrecognized platform.
#endif

#if !defined(acplArch) || acplArch == acplArchUnknown
#	error Unrecognized processor architecture.
#endif


#endif // ACPL_DEFINES_H
