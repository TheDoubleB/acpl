#include "Tests.h"


SectionFuncMain(adhoc)
{
	if (argc < 1)
	{
		
		
		PrintOut("All Passed!\n");
	}
	else
		return PrintUnrecParams(acplDbgSrcLoc, lSectionName, argc, argv);
	
	return 0;
}

SectionFuncParamsBegin(adhoc)
// SectionFuncParamsAdd("param [param [param ...]]", "description")
SectionFuncParamsEnd(adhoc)
