// this definition needs to be before inclusion of "Tests.h"
#define _local_AppName AppName

#include "Tests.h"


const char *gArgv0 = NULL;
struct VerbosityFuncs gVerbosityFuncs;


#define TestSectionAdd(N_SECT_NAME, N_SECT_DESCR) \
	{ #N_SECT_NAME, N_SECT_DESCR, N_SECT_NAME##_main, N_SECT_NAME##_params },

static struct TestSection
{
	const char *sName;
	const char *sDesc;
	int (*sFuncMain)(int argc, char **argv);
	TestSectionParam *(*sFuncParams)();
} lTestSections[] =
{
	TestSectionAdd(defines, "Defines tests (environment print-out)")
	TestSectionAdd(types, "Basic types tests")
	TestSectionAdd(bits, "Bit tests")
	TestSectionAdd(raii, "RAII tests")
	TestSectionAdd(endian, "endianness conversions tests")
	TestSectionAdd(io, "Basic I/O framework tests")
	TestSectionAdd(time, "Time-related functionality")
	TestSectionAdd(threads, "Threads, Mutex, and Condition tests")
	TestSectionAdd(memory, "Memory tests")
	TestSectionAdd(array, "acpl::Array and corresponding functionality tests")
	TestSectionAdd(charsets, "character sets tests")
	TestSectionAdd(string, "acpl::String tests")
	
	TestSectionAdd(fspath, "acpl::Path tests")
	TestSectionAdd(fsdir, "acpl::Dir tests")
	TestSectionAdd(fsfile, "acpl::File tests")
	
	TestSectionAdd(netaddr, "network address tests")
	TestSectionAdd(netheader, "internet protocol (IP) headers tests")
	TestSectionAdd(netsock, "network socket/connection tests")
	TestSectionAdd(netpoll, "network socket polling tests")
	
	TestSectionAdd(uri, "acpl::Uri tests")
	
	TestSectionAdd(adhoc, "Ad hoc (temporary) tests")
	{ NULL, NULL, NULL, NULL }
};

static TestSection *GetTestSection(const char *nName)
{
	for (TestSection *oCurTs = lTestSections; oCurTs->sName != NULL; oCurTs++)
		if (StringCompare(nName, oCurTs->sName) == 0)
			return oCurTs;
	
	return NULL;
}

static int PrintUsage(bool nPrintSections = true)
{
	printf("\nUsage:\n");
	printf("    " _local_AppName " section [section parameter(s)] -- Run test section\n");
	printf("    " _local_AppName " \"v\" section [section parameter(s)] -- Run test section in verbose mode\n");
	printf("    " _local_AppName " \"help\" -- Print this help message\n");
	printf("    " _local_AppName " \"help\" section -- Print parameter list for a specified section\n");
	printf("    " _local_AppName " \"all\" -- Run essential tests of all sections\n");
	printf("    " _local_AppName " \"v\" \"all\" -- Run essential tests of all sections in verbose mode\n");
	
	if (nPrintSections == true)
	{
		printf("\nsection(s):\n");
		for (TestSection *oCurTs = lTestSections; oCurTs->sName != NULL; oCurTs++)
			printf("    \"%s\"%s%s\n", oCurTs->sName,
					((oCurTs->sDesc != NULL) ? " -- " : ""),
					((oCurTs->sDesc != NULL) ? oCurTs->sDesc : "")
				);
	}
	
	printf("\n");
	
	return 1;
}

static int PrintUnrecSection(const char *nSectionName)
{
	printf("\nError:\n    Unrecognized section '%s'\n", nSectionName);
	PrintUsage();
	return 1;
}

static int PrintUsageSection(const char *nSectionName)
{
	TestSection *oTs = GetTestSection(nSectionName);
	
	if (oTs != NULL)
	{
		PrintUsage(false);
		printf("'%s' section parameter(s):\n", nSectionName);
		printf("    (no params) -- essential tests\n");
		
		for (TestSectionParam *oSectParams = oTs->sFuncParams(); oSectParams != NULL && oSectParams->sParams != NULL; oSectParams++)
			printf("    %s -- %s\n", oSectParams->sParams, oSectParams->sDesc);
		
		printf("\n");
		return 1;
	}
	else
		return PrintUnrecSection(nSectionName);
}

static int run_all_sections_tests()
{
	for (TestSection *oCurTs = lTestSections; oCurTs->sName != NULL; oCurTs++)
	{
		PrintOut("SECTION: '%s'\n", oCurTs->sName);
		TestFM(oCurTs->sFuncMain(0, NULL) == 0, "Section '%s' essential tests\n", oCurTs->sName)
	}
	
	return 0;
}


int NonVerboseFprintf(FILE *, const char *, ...)
{
	return 0;
}

int NonVerboseFflush(FILE *)
{
	return 0;
}


// 
// MAIN
// 

int main(int argc, char **argv)
{
	if (argc < 2)
		return PrintUsage();
	
	gArgv0 = argv[0];
	gVerbosityFuncs.sFprintf = NonVerboseFprintf;
	gVerbosityFuncs.sFflush = NonVerboseFflush;
	
	if (StringCompare(argv[1], "v") == 0)
	{
		gVerbosityFuncs.sFprintf = fprintf;
		gVerbosityFuncs.sFflush = fflush;
		
		if (argc < 3)
			return PrintUsage();
		else
		if (StringCompare(argv[2], "all") == 0)
			return run_all_sections_tests();
	}
	else
	if (StringCompare(argv[1], "help") == 0)
	{
		if (argc < 3)
			return PrintUsage();
		else
			return PrintUsageSection(argv[2]);
	}
	else
	if (StringCompare(argv[1], "all") == 0)
		return run_all_sections_tests();
	
	TestSection *oTs = GetTestSection(argv[ ((gVerbosityFuncs.sFprintf == NonVerboseFprintf) ? 1 : 2) ]);
	
	if (oTs == NULL)
		return PrintUnrecSection(argv[ ((gVerbosityFuncs.sFprintf == NonVerboseFprintf) ? 1 : 2) ]);
	
	Test(oTs->sFuncMain(argc - ((gVerbosityFuncs.sFprintf == NonVerboseFprintf) ? 2 : 3), &argv[ ((gVerbosityFuncs.sFprintf == NonVerboseFprintf) ? 2 : 3) ]) == 0);
	
	return 0;
}


// 
// Implementations
// 

int StringCompare(const char *nStr1, const char *nStr2)
{
	for (; *nStr1 == *nStr2 && *nStr1 != '\0'; nStr1++, nStr2++);
	
	return ((*nStr1 == '\0' && *nStr2 == '\0') ? 0 : 1);
}

int StringCompare(const wchar_t *nStr1, const wchar_t *nStr2)
{
	for (; *nStr1 == *nStr2 && *nStr1 != L'\0'; nStr1++, nStr2++);
	
	return ((*nStr1 == L'\0' && *nStr2 == L'\0') ? 0 : 1);
}

int StringCompare(const wchar_t *nStr1, const char *nStr2)
{
	for (; *nStr1 == *nStr2 && *nStr1 != '\0'; nStr1++, nStr2++);
	
	return ((*nStr1 == L'\0' && *nStr2 == '\0') ? 0 : 1);
}

int PrintUnrecParams(const char * const nSrcLoc, const char * const nSectionName, int nArgCount, const char * const * const nArgArray)
{
	PrintErr("%s: error: in section \"%s\": unrecognized parameter sequence: `", nSrcLoc, nSectionName);
	
	for (int i = 0; i < nArgCount; i++)
	{
		if (i > 0)
			PrintErr(" ");
		PrintErr("%s", nArgArray[i]);
	}
	
	PrintErr("`\n");
	
	return 1;
}

void PrintHex(const void *nBfr, acpl::SizeT nBfrSize)
{
	const acpl::UInt8 *oBfr = static_cast<const acpl::UInt8 *>(nBfr);
	while (nBfrSize-- != 0)
		PrintOut("0x%02X ", *oBfr++)
}
