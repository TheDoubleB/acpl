#include "Tests.h"

#include "../src/Raii.h"


static struct _local_CallReg
{
	acpl::UInt8 sFunc0;
	acpl::UInt8 sFuncR0;
	acpl::UInt8 sFunc1;
	acpl::UInt8 sFuncR1;
	acpl::UInt8 sFunc2;
	acpl::UInt8 sFuncR2;
	acpl::UInt8 sMethod0;
	acpl::UInt8 sMethodR0;
	acpl::UInt8 sMethod1;
	acpl::UInt8 sMethodR1;
	acpl::UInt8 sMethod2;
	acpl::UInt8 sMethodR2;
} lCallReg = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };


void _local_Func0()
{
	lCallReg.sFunc0++;
}

int _local_FuncR0()
{
	lCallReg.sFuncR0++;
	return 0;
}

void _local_Func1(int)
{
	lCallReg.sFunc1++;
}

int _local_FuncR1(int)
{
	lCallReg.sFuncR1++;
	return 0;
}

void _local_Func2(int, float)
{
	lCallReg.sFunc2++;
}

int _local_FuncR2(int, float)
{
	lCallReg.sFuncR2++;
	return 0;
}


class _local_MethodHolder
{
	public:
		void Method0()
		{
			lCallReg.sMethod0++;
		}

		int MethodR0()
		{
			lCallReg.sMethodR0++;
			return 0;
		}

		void Method1(int)
		{
			lCallReg.sMethod1++;
		}

		int MethodR1(int)
		{
			lCallReg.sMethodR1++;
			return 0;
		}

		void Method2(int, float)
		{
			lCallReg.sMethod2++;
		}

		int MethodR2(int, float)
		{
			lCallReg.sMethodR2++;
			return 0;
		}
} lMethodHolder;


static int TestRaiiFunc0s()
{
	PrintFn();
	
	
	// RaiiFunc0 (normal)
	{
		// WARNING: This is a deliberate block!
		acpl::RaiiFunc0<_local_Func0> oRaii;
	}
	Test(lCallReg.sFunc0 == 1);
	
	// RaiiFunc0 + Disable()
	{
		// WARNING: This is a deliberate block!
		acpl::RaiiFunc0<_local_Func0> oRaii;
		oRaii.Disable();
	}
	Test(lCallReg.sFunc0 == 1);
	
	// RaiiFunc0 + Disable() + Enable()
	{
		// WARNING: This is a deliberate block!
		acpl::RaiiFunc0<_local_Func0> oRaii;
		oRaii.Disable();
		oRaii.Enable();
	}
	Test(lCallReg.sFunc0 == 2);
	
	
	// RaiiFuncR0 (normal)
	{
		// WARNING: This is a deliberate block!
		acpl::RaiiFuncR0<int, _local_FuncR0> oRaii;
	}
	Test(lCallReg.sFuncR0 == 1);
	
	// RaiiFuncR0 + Disable()
	{
		// WARNING: This is a deliberate block!
		acpl::RaiiFuncR0<int, _local_FuncR0> oRaii;
		oRaii.Disable();
	}
	Test(lCallReg.sFuncR0 == 1);
	
	// RaiiFuncR0 + Disable() + Enable()
	{
		// WARNING: This is a deliberate block!
		acpl::RaiiFuncR0<int, _local_FuncR0> oRaii;
		oRaii.Disable();
		oRaii.Enable();
	}
	Test(lCallReg.sFuncR0 == 2);
	
	
	return 0;
}

static int TestRaiiFunc1s()
{
	PrintFn();
	
	
	int oParam = 0;
	
	
	// RaiiFunc1 (normal)
	{
		// WARNING: This is a deliberate block!
		acpl::RaiiFunc1<int, _local_Func1> oRaii(oParam);
	}
	Test(lCallReg.sFunc1 == 1);
	
	// RaiiFunc1 + Disable()
	{
		// WARNING: This is a deliberate block!
		acpl::RaiiFunc1<int, _local_Func1> oRaii(oParam);
		oRaii.Disable();
	}
	Test(lCallReg.sFunc1 == 1);
	
	// RaiiFunc1 + Disable() + Enable()
	{
		// WARNING: This is a deliberate block!
		acpl::RaiiFunc1<int, _local_Func1> oRaii(oParam);
		oRaii.Disable();
		oRaii.Enable();
	}
	Test(lCallReg.sFunc1 == 2);
	
	
	// RaiiFuncR1 (normal)
	{
		// WARNING: This is a deliberate block!
		acpl::RaiiFuncR1<int, int, _local_FuncR1> oRaii(oParam);
	}
	Test(lCallReg.sFuncR1 == 1);
	
	// RaiiFuncR1 + Disable()
	{
		// WARNING: This is a deliberate block!
		acpl::RaiiFuncR1<int, int, _local_FuncR1> oRaii(oParam);
		oRaii.Disable();
	}
	Test(lCallReg.sFuncR1 == 1);
	
	// RaiiFuncR1 + Disable() + Enable()
	{
		// WARNING: This is a deliberate block!
		acpl::RaiiFuncR1<int, int, _local_FuncR1> oRaii(oParam);
		oRaii.Disable();
		oRaii.Enable();
	}
	Test(lCallReg.sFuncR1 == 2);
	
	
	return 0;
}

static int TestRaiiFunc2s()
{
	PrintFn();
	
	
	int oParam = 0;
	float oAddParam = 0.0;
	
	
	// RaiiFunc2 (normal)
	{
		// WARNING: This is a deliberate block!
		acpl::RaiiFunc2<int, float, _local_Func2> oRaii(oParam, oAddParam);
	}
	Test(lCallReg.sFunc2 == 1);
	
	// RaiiFunc2 + Disable()
	{
		// WARNING: This is a deliberate block!
		acpl::RaiiFunc2<int, float, _local_Func2> oRaii(oParam, oAddParam);
		oRaii.Disable();
	}
	Test(lCallReg.sFunc2 == 1);
	
	// RaiiFunc2 + Disable() + Enable()
	{
		// WARNING: This is a deliberate block!
		acpl::RaiiFunc2<int, float, _local_Func2> oRaii(oParam, oAddParam);
		oRaii.Disable();
		oRaii.Enable();
	}
	Test(lCallReg.sFunc2 == 2);
	
	
	// RaiiFuncR2 (normal)
	{
		// WARNING: This is a deliberate block!
		acpl::RaiiFuncR2<int, int, float, _local_FuncR2> oRaii(oParam, oAddParam);
	}
	Test(lCallReg.sFuncR2 == 1);
	
	// RaiiFuncR2 + Disable()
	{
		// WARNING: This is a deliberate block!
		acpl::RaiiFuncR2<int, int, float, _local_FuncR2> oRaii(oParam, oAddParam);
		oRaii.Disable();
	}
	Test(lCallReg.sFuncR2 == 1);
	
	// RaiiFuncR2 + Disable() + Enable()
	{
		// WARNING: This is a deliberate block!
		acpl::RaiiFuncR2<int, int, float, _local_FuncR2> oRaii(oParam, oAddParam);
		oRaii.Disable();
		oRaii.Enable();
	}
	Test(lCallReg.sFuncR2 == 2);
	
	
	return 0;
}

static int TestRaiiMethod0s()
{
	PrintFn();
	
	
	// RaiiMethod0 (normal)
	{
		// WARNING: This is a deliberate block!
		acpl::RaiiMethod0<_local_MethodHolder, &_local_MethodHolder::Method0> oRaii(lMethodHolder);
	}
	Test(lCallReg.sMethod0 == 1);
	
	// RaiiMethod0 + Disable()
	{
		// WARNING: This is a deliberate block!
		acpl::RaiiMethod0<_local_MethodHolder, &_local_MethodHolder::Method0> oRaii(lMethodHolder);
		oRaii.Disable();
	}
	Test(lCallReg.sMethod0 == 1);
	
	// RaiiMethod0 + Disable() + Enable()
	{
		// WARNING: This is a deliberate block!
		acpl::RaiiMethod0<_local_MethodHolder, &_local_MethodHolder::Method0> oRaii(lMethodHolder);
		oRaii.Disable();
		oRaii.Enable();
	}
	Test(lCallReg.sMethod0 == 2);
	
	
	// RaiiMethodR0 (normal)
	{
		// WARNING: This is a deliberate block!
		acpl::RaiiMethodR0<_local_MethodHolder, int, &_local_MethodHolder::MethodR0> oRaii(lMethodHolder);
	}
	Test(lCallReg.sMethodR0 == 1);
	
	// RaiiMethodR0 + Disable()
	{
		// WARNING: This is a deliberate block!
		acpl::RaiiMethodR0<_local_MethodHolder, int, &_local_MethodHolder::MethodR0> oRaii(lMethodHolder);
		oRaii.Disable();
	}
	Test(lCallReg.sMethodR0 == 1);
	
	// RaiiMethodR0 + Disable() + Enable()
	{
		// WARNING: This is a deliberate block!
		acpl::RaiiMethodR0<_local_MethodHolder, int, &_local_MethodHolder::MethodR0> oRaii(lMethodHolder);
		oRaii.Disable();
		oRaii.Enable();
	}
	Test(lCallReg.sMethodR0 == 2);
	
	
	return 0;
}

static int TestRaiiMethod1s()
{
	PrintFn();
	
	
	int oParam = 0;
	
	
	// RaiiMethod1 (normal)
	{
		// WARNING: This is a deliberate block!
		acpl::RaiiMethod1<_local_MethodHolder, int, &_local_MethodHolder::Method1> oRaii(lMethodHolder, oParam);
	}
	Test(lCallReg.sMethod1 == 1);
	
	// RaiiMethod1 + Disable()
	{
		// WARNING: This is a deliberate block!
		acpl::RaiiMethod1<_local_MethodHolder, int, &_local_MethodHolder::Method1> oRaii(lMethodHolder, oParam);
		oRaii.Disable();
	}
	Test(lCallReg.sMethod1 == 1);
	
	// RaiiMethod1 + Disable() + Enable()
	{
		// WARNING: This is a deliberate block!
		acpl::RaiiMethod1<_local_MethodHolder, int, &_local_MethodHolder::Method1> oRaii(lMethodHolder, oParam);
		oRaii.Disable();
		oRaii.Enable();
	}
	Test(lCallReg.sMethod1 == 2);
	
	
	// RaiiMethodR1 (normal)
	{
		// WARNING: This is a deliberate block!
		acpl::RaiiMethodR1<_local_MethodHolder, int, int, &_local_MethodHolder::MethodR1> oRaii(lMethodHolder, oParam);
	}
	Test(lCallReg.sMethodR1 == 1);
	
	// RaiiMethodR1 + Disable()
	{
		// WARNING: This is a deliberate block!
		acpl::RaiiMethodR1<_local_MethodHolder, int, int, &_local_MethodHolder::MethodR1> oRaii(lMethodHolder, oParam);
		oRaii.Disable();
	}
	Test(lCallReg.sMethodR1 == 1);
	
	// RaiiMethodR1 + Disable() + Enable()
	{
		// WARNING: This is a deliberate block!
		acpl::RaiiMethodR1<_local_MethodHolder, int, int, &_local_MethodHolder::MethodR1> oRaii(lMethodHolder, oParam);
		oRaii.Disable();
		oRaii.Enable();
	}
	Test(lCallReg.sMethodR1 == 2);
	
	
	return 0;
}

static int TestRaiiMethod2s()
{
	PrintFn();
	
	
	int oParam = 0;
	float oAddParam = 0.0;
	
	
	// RaiiMethod2 (normal)
	{
		// WARNING: This is a deliberate block!
		acpl::RaiiMethod2<_local_MethodHolder, int, float, &_local_MethodHolder::Method2> oRaii(lMethodHolder, oParam, oAddParam);
	}
	Test(lCallReg.sMethod2 == 1);
	
	// RaiiMethod2 + Disable()
	{
		// WARNING: This is a deliberate block!
		acpl::RaiiMethod2<_local_MethodHolder, int, float, &_local_MethodHolder::Method2> oRaii(lMethodHolder, oParam, oAddParam);
		oRaii.Disable();
	}
	Test(lCallReg.sMethod2 == 1);
	
	// RaiiMethod2 + Disable() + Enable()
	{
		// WARNING: This is a deliberate block!
		acpl::RaiiMethod2<_local_MethodHolder, int, float, &_local_MethodHolder::Method2> oRaii(lMethodHolder, oParam, oAddParam);
		oRaii.Disable();
		oRaii.Enable();
	}
	Test(lCallReg.sMethod2 == 2);
	
	
	// RaiiMethodR2 (normal)
	{
		// WARNING: This is a deliberate block!
		acpl::RaiiMethodR2<_local_MethodHolder, int, int, float, &_local_MethodHolder::MethodR2> oRaii(lMethodHolder, oParam, oAddParam);
	}
	Test(lCallReg.sMethodR2 == 1);
	
	// RaiiMethodR2 + Disable()
	{
		// WARNING: This is a deliberate block!
		acpl::RaiiMethodR2<_local_MethodHolder, int, int, float, &_local_MethodHolder::MethodR2> oRaii(lMethodHolder, oParam, oAddParam);
		oRaii.Disable();
	}
	Test(lCallReg.sMethodR2 == 1);
	
	// RaiiMethodR2 + Disable() + Enable()
	{
		// WARNING: This is a deliberate block!
		acpl::RaiiMethodR2<_local_MethodHolder, int, int, float, &_local_MethodHolder::MethodR2> oRaii(lMethodHolder, oParam, oAddParam);
		oRaii.Disable();
		oRaii.Enable();
	}
	Test(lCallReg.sMethodR2 == 2);
	
	
	return 0;
}


SectionFuncMain(raii)
{
	if (argc < 1)
	{
		Test(TestRaiiFunc0s() == 0);
		Test(TestRaiiFunc1s() == 0);
		Test(TestRaiiFunc2s() == 0);
		Test(TestRaiiMethod0s() == 0);
		Test(TestRaiiMethod1s() == 0);
		Test(TestRaiiMethod2s() == 0);
		
		PrintOut("All Passed!\n");
	}
	else
		return PrintUnrecParams(acplDbgSrcLoc, lSectionName, argc, argv);
	
	return 0;
}

SectionFuncParamsBegin(raii)
// SectionFuncParamsAdd("param [param [param ...]]", "description")
SectionFuncParamsEnd(raii)
