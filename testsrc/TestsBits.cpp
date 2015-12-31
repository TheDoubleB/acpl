#include "Tests.h"

#include "../src/Bits.h"


static int TestBits()
{
	PrintFn();
	
	
	acpl::UInt64 oVar;
	
	
	oVar = acpl::Const::UI64(0x00000000, 0x00000000);
	
	Test(acpl::Bits::Get(oVar, acpl::Num<acpl::UInt64>::Max()) == acpl::Const::UI64(0x00000000, 0x00000000));
	Test(acpl::Bits::AllSet(oVar, acpl::Num<acpl::UInt64>::Max()) == false);
	Test(acpl::Bits::AnySet(oVar, acpl::Num<acpl::UInt64>::Max()) == false);
	
	acpl::Bits::Set(oVar, acpl::Const::UI64(0x000000FF, 0x000FF000), true);
	
	Test(acpl::Bits::Get(oVar, acpl::Num<acpl::UInt64>::Max()) == acpl::Const::UI64(0x000000FF, 0x000FF000));
	Test(acpl::Bits::AllSet(oVar, acpl::Num<acpl::UInt64>::Max()) == false);
	Test(acpl::Bits::AnySet(oVar, acpl::Num<acpl::UInt64>::Max()) == true);
	
	Test(acpl::Bits::Get(oVar, acpl::Const::UI64(0x000000FF, 0x000FF000)) == acpl::Const::UI64(0x000000FF, 0x000FF000));
	Test(acpl::Bits::AllSet(oVar, acpl::Const::UI64(0x000000FF, 0x000FF000)) == true);
	Test(acpl::Bits::AnySet(oVar, acpl::Const::UI64(0x000000FF, 0x000FF000)) == true);
	
	Test(acpl::Bits::Get(oVar, acpl::Const::UI64(0x000000F0, 0x0000F000)) == acpl::Const::UI64(0x000000F0, 0x0000F000));
	Test(acpl::Bits::AllSet(oVar, acpl::Const::UI64(0x000000F0, 0x0000F000)) == true);
	Test(acpl::Bits::AnySet(oVar, acpl::Const::UI64(0x000000F0, 0x0000F000)) == true);
	
	Test(acpl::Bits::Get(oVar, acpl::Const::UI64(0x00000FF0, 0x0000FF00)) == acpl::Const::UI64(0x000000F0, 0x0000F000));
	Test(acpl::Bits::AllSet(oVar, acpl::Const::UI64(0x00000FF0, 0x0000FF00)) == false);
	Test(acpl::Bits::AnySet(oVar, acpl::Const::UI64(0x00000FF0, 0x0000FF00)) == true);
	
	
	return 0;
}


SectionFuncMain(bits)
{
	if (argc < 1)
	{
		Test(TestBits() == 0);
		
		PrintOut("All Passed!\n");
	}
	else
		return PrintUnrecParams(acplDbgSrcLoc, lSectionName, argc, argv);
	
	return 0;
}

SectionFuncParamsBegin(bits)
// SectionFuncParamsAdd("param [param [param ...]]", "description")
SectionFuncParamsEnd(bits)
