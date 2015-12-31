#include "Tests.h"

#include "../src/Defines.h"


// 
// Compiler name
// 

#if   (acplCompiler == acplCompilerGcc)
#	define _local_CompilerName "GNU Compiler Collection"
#elif (acplCompiler == acplCompilerMsvc)
#	define _local_CompilerName "Microsoft Visual C/C++"
#else
#	error Unknown compiler???
#endif


//
// C standard library name
//

#if   (acplCRuntime == acplCRuntimeGlibc)
#	define _local_CRuntimeName "GNU C Library"
#elif (acplCRuntime == acplCRuntimeMscrt)
#	define _local_CRuntimeName "Microsoft C Run-time Library"
#else
#	error Unknown C standard library???
#endif


// 
// Platform name
// 

#if   (acplPlatform == acplPlatformLinux)
#	define _local_PlatformName "GNU/Linux"
#elif (acplPlatform == acplPlatformMsWin)
#	define _local_PlatformName "Microsoft Windows"
#else
#	error Unknown platform???
#endif


// 
// Architecture name
// 

#if   (acplArch == acplArchX8632)
#	define _local_ArchName "x86-32/IA-32"
#elif (acplArch == acplArchX8664)
#	define _local_ArchName "x86-64/AMD64"
#elif (acplArch == acplArchIA32)
#	define _local_ArchName "x86-32/IA-32"
#elif (acplArch == acplArchIA64)
#	define _local_ArchName "Itanium/IA-64"
#elif (acplArch == acplArchAMD64)
#	define _local_ArchName "x86-64/AMD64"
#elif (acplArch == acplArchARM)
#	define _local_ArchName "ARM"
#elif (acplArch == acplArchHPPA)
#	define _local_ArchName "Hewlett-Packard Precision Architecture RISC"
#else
#	error Unknown processor architecture???
#endif


static int PrintEnvSetup()
{
	PrintFn();
	
	
	PrintOut("Compiler .............: " _local_CompilerName " (version %d)\n", acplCompilerVer);
	PrintOut("C standard library ...: " _local_CRuntimeName " (version %d)\n", acplCRuntimeVer);
	PrintOut("Operating system .....: " _local_PlatformName "\n");
	PrintOut("Target architecture ..: " _local_ArchName "\n");
	
	
	return 0;
}


SectionFuncMain(defines)
{
	if (argc < 1)
	{
		Test(PrintEnvSetup() == 0);
		
		PrintOut("All Passed!\n");
	}
	else
		return PrintUnrecParams(acplDbgSrcLoc, lSectionName, argc, argv);
	
	return 0;
}

SectionFuncParamsBegin(defines)
// SectionFuncParamsAdd("param [param [param ...]]", "description")
SectionFuncParamsEnd(defines)
