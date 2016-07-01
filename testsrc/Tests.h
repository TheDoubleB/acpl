#ifndef ACPL_TESTS_H
#define ACPL_TESTS_H

#include <stdio.h>
#include <string.h>

#include "../src/Types.h"
#include "../src/Devel.h"


// Defines

#define AppNameBase "testACPL"

#if   (acplCRuntime == acplCRuntimeGlibc)
#	define AppName AppNameBase
#elif (acplCRuntime == acplCRuntimeMscrt)
#	define AppName AppNameBase ".exe"
#else
#	error Add platform specific AppName.
#endif



// Types

struct TestSectionParam
{
	const char *sParams;
	const char *sDesc;
};

struct VerbosityFuncs
{
	int (*sFprintf)(FILE *stream, const char *format, ...);
	int (*sFflush)(FILE *);
};


// Externs

#ifndef _local_AppName
	extern const char *gArgv0;
	extern struct VerbosityFuncs gVerbosityFuncs;
#endif



// Declarations

int StringCompare(const char *nStr1, const char *nStr2);
int StringCompare(const wchar_t *nStr1, const wchar_t *nStr2);
int StringCompare(const wchar_t *nStr1, const char *nStr2);
int PrintUnrecParams(const char * const nSrcLoc, const char * const nSectionName, int nArgCount, const char * const * const nArgArray);
void PrintHex(const void *nBfr, acpl::SizeT nBfrSize);

#define SectionFuncMainD(N_SECTION_NAME) \
	int N_SECTION_NAME##_main(int argc, char **argv)

#define SectionFuncParamsD(N_SECTION_NAME) \
	TestSectionParam *N_SECTION_NAME##_params()

#define SectionFuncMain(N_SECTION_NAME) \
	static const char * const lSectionName = #N_SECTION_NAME; \
	SectionFuncMainD(N_SECTION_NAME)

#define SectionFuncParamsBegin(N_SECTION_NAME) \
	static TestSectionParam lTestSectParams_##N_SECTION_NAME[] =  \
	{

#define SectionFuncParamsEnd(N_SECTION_NAME) \
		{ NULL, NULL } \
	}; \
	\
	SectionFuncParamsD(N_SECTION_NAME) \
	{ \
		return lTestSectParams_##N_SECTION_NAME; \
	}

#define SectionFuncParamsAdd(N_PARAMS, N_DESCR) \
		{ N_PARAMS, N_DESCR },

#define SectionFuncDecls(N_SECTION_NAME) \
	SectionFuncMainD(N_SECTION_NAME); \
	SectionFuncParamsD(N_SECTION_NAME);

SectionFuncDecls(adhoc)
SectionFuncDecls(array)
SectionFuncDecls(bits)
SectionFuncDecls(charsets)
SectionFuncDecls(defines)
SectionFuncDecls(endian)
SectionFuncDecls(fsdir)
SectionFuncDecls(fsfile)
SectionFuncDecls(fspath)
SectionFuncDecls(io)
SectionFuncDecls(memory)
SectionFuncDecls(netaddr)
SectionFuncDecls(netheader)
SectionFuncDecls(netpoll)
SectionFuncDecls(netsock)
SectionFuncDecls(raii)
SectionFuncDecls(string)
SectionFuncDecls(threads)
SectionFuncDecls(time)
SectionFuncDecls(types)
SectionFuncDecls(uri)


// Macros

#define PrintErr(...)		{ fprintf(stderr, __VA_ARGS__); }
#define PrintOut(...)		{ gVerbosityFuncs.sFprintf(stdout, __VA_ARGS__); }
#define PrintOutFlush(...)	{ gVerbosityFuncs.sFprintf(stdout, __VA_ARGS__); gVerbosityFuncs.sFflush(stdout); }

#define PrintFn()			{ PrintOut("%s\n", __FUNCTION__); }
#define PrintFnM(N_MSG)		{ PrintOut("%s %s\n", __FUNCTION__, N_MSG); }

#define Test(N_CND)						{ if ((N_CND) == false) { PrintErr(acplDbgSrcLoc ": no error message\n"); return 1; } }
#define TestM(N_CND, N_ERR_MSG)			{ if ((N_CND) == false) { PrintErr(acplDbgSrcLoc ": error: %s\n", N_ERR_MSG); return 1; } }
#define TestFM(N_CND, N_ERR_FMT, ...)	{ if ((N_CND) == false) { PrintErr(acplDbgSrcLoc ": error: " N_ERR_FMT "\n", __VA_ARGS__); return 1; } }


#endif // ACPL_TESTS_H
