#include "Tests.h"

#include "../src/Types.h"


#if   (acplCRuntime == acplCRuntimeGlibc)
#	define _local_SizeOfWcharT 4
#	define _local_SizeOfLargestFloat 16
#	define _local_HasSigNaN true
#elif (acplCRuntime == acplCRuntimeMscrt)
#	define _local_SizeOfWcharT 2
#	define _local_SizeOfLargestFloat 8
#	define _local_HasSigNaN false
#else
#	error _local_SizeOfWcharT define missing
#	error _local_SizeOfLargestFloat define missing
#	error _local_HasSigNaN define missing
#endif


enum _local_TestEnum
{
	_local_teVal0,
	_local_teVal1,
	_local_teVal2
};


static int TestTypeSizes()
{
	PrintFn();
	
	
	Test(sizeof(acpl::SInt8)  ==  1);
	Test(sizeof(acpl::SInt16) ==  2);
	Test(sizeof(acpl::SInt32) ==  4);
	Test(sizeof(acpl::SInt64) ==  8);

	Test(sizeof(acpl::UInt8)  ==  1);
	Test(sizeof(acpl::UInt16) ==  2);
	Test(sizeof(acpl::UInt32) ==  4);
	Test(sizeof(acpl::UInt64) ==  8);
	
	Test(sizeof(acpl::Float32) == 4);
	Test(sizeof(acpl::Float64) == 8);
	Test(sizeof(acpl::Float::Largest) == _local_SizeOfLargestFloat);
	
	Test(sizeof(acpl::SizeT)  == sizeof(void *));
	Test(sizeof(acpl::SSizeT) == sizeof(void *));
	Test(sizeof(acpl::OffT)   == 8);
	
	Test(sizeof(acpl::TimeT) == 8);
	
	Test(sizeof(acpl::Unichar) == 4);
	
	Test(sizeof(wchar_t) == _local_SizeOfWcharT);
	
	
	return 0;
}

static int TestFloatClass()
{
	PrintFn();
	
	acpl::Float::Parts oFpp;
	
	
	// Create() method
	
	Test(acpl::Float::Create<acpl::Float32>(0, 0x7F, acpl::Const::UI64(0x00000000, 0x00550000)) == 1.6640625);
	Test(acpl::Float::Create<acpl::Float64>(0, 0x3FF, acpl::Const::UI64(0x000AA000, 0x00000000)) == 1.6640625);
	Test(acpl::Float::Create<acpl::Float::Largest>(0, 0x3FFF, acpl::Const::UI64(0xD5000000, 0x00000000)) == 1.6640625 || acpl::Float::HasExtPrec() == false);
	
	
	// Min() method
	
	acpl::Float::GetParts(acpl::Float::Min<acpl::Float32>(), oFpp);
	Test(oFpp.sSign == 1);
	Test(oFpp.sExp  == 0xFE);
	Test(oFpp.sMan  == acpl::Const::UI64(0x00000000, 0x007FFFFF));
	
	acpl::Float::GetParts(acpl::Float::Min<acpl::Float64>(), oFpp);
	Test(oFpp.sSign == 1);
	Test(oFpp.sExp  == 0x7FE);
	Test(oFpp.sMan  == acpl::Const::UI64(0x000FFFFF, 0xFFFFFFFF));
	
	acpl::Float::GetParts(acpl::Float::Min<acpl::Float::Largest>(), oFpp);
	Test(oFpp.sSign == 1);
	Test(oFpp.sExp  == 0x7FFE || acpl::Float::HasExtPrec() == false);
	Test(oFpp.sMan  == acpl::Const::UI64(0xFFFFFFFF, 0xFFFFFFFF) || acpl::Float::HasExtPrec() == false);
	
	
	// Max() method
	
	acpl::Float::GetParts(acpl::Float::Max<acpl::Float32>(), oFpp);
	Test(oFpp.sSign == 0);
	Test(oFpp.sExp  == 0xFE);
	Test(oFpp.sMan  == acpl::Const::UI64(0x00000000, 0x007FFFFF));
	
	acpl::Float::GetParts(acpl::Float::Max<acpl::Float64>(), oFpp);
	Test(oFpp.sSign == 0);
	Test(oFpp.sExp  == 0x7FE);
	Test(oFpp.sMan  == acpl::Const::UI64(0x000FFFFF, 0xFFFFFFFF));
	
	acpl::Float::GetParts(acpl::Float::Max<acpl::Float::Largest>(), oFpp);
	Test(oFpp.sSign == 0);
	Test(oFpp.sExp  == 0x7FFE || acpl::Float::HasExtPrec() == false);
	Test(oFpp.sMan  == acpl::Const::UI64(0xFFFFFFFF, 0xFFFFFFFF) || acpl::Float::HasExtPrec() == false);
	
	
	// Inf() method
	
	acpl::Float::GetParts(acpl::Float::Inf<acpl::Float32>(), oFpp);
	Test(oFpp.sSign == 0);
	Test(oFpp.sExp  == 0xFF);
	Test(oFpp.sMan  == acpl::Const::UI64(0x00000000, 0x00000000));
	
	acpl::Float::GetParts(acpl::Float::Inf<acpl::Float64>(), oFpp);
	Test(oFpp.sSign == 0);
	Test(oFpp.sExp  == 0x7FF);
	Test(oFpp.sMan  == acpl::Const::UI64(0x00000000, 0x00000000));
	
	acpl::Float::GetParts(acpl::Float::Inf<acpl::Float::Largest>(), oFpp);
	Test(oFpp.sSign == 0);
	Test(oFpp.sExp  == 0x7FFF || acpl::Float::HasExtPrec() == false);
	Test(oFpp.sMan  == acpl::Const::UI64(0x80000000, 0x00000000) || acpl::Float::HasExtPrec() == false); // NOTE: the integer part (bit 63) is 1 !!!
	
	
	// *NaN() methods
	
	acpl::Float::GetParts(acpl::Float::NaN<acpl::Float32>(), oFpp);
	Test(oFpp.sSign == 0);
	Test(oFpp.sExp  == 0xFF);
	Test(oFpp.sMan  == acpl::Const::UI64(0x00000000, 0x00400000));
	
	acpl::Float::GetParts(acpl::Float::QNaN<acpl::Float32>(), oFpp);
	Test(oFpp.sSign == 0);
	Test(oFpp.sExp  == 0xFF);
	Test(oFpp.sMan  == acpl::Const::UI64(0x00000000, 0x00400000));
	
	acpl::Float::GetParts(acpl::Float::SNaN<acpl::Float32>(), oFpp);
	Test(oFpp.sSign == 0);
	Test(oFpp.sExp  == 0xFF);
	Test(oFpp.sMan  == acpl::Const::UI64(0x00000000, 0x00200000) || (_local_HasSigNaN == false && oFpp.sMan  == acpl::Const::UI64(0x00000000, 0x00400000)));
	
	acpl::Float::GetParts(acpl::Float::NaN<acpl::Float64>(), oFpp);
	Test(oFpp.sSign == 0);
	Test(oFpp.sExp  == 0x7FF);
	Test(oFpp.sMan  == acpl::Const::UI64(0x00080000, 0x00000000));
	
	acpl::Float::GetParts(acpl::Float::QNaN<acpl::Float64>(), oFpp);
	Test(oFpp.sSign == 0);
	Test(oFpp.sExp  == 0x7FF);
	Test(oFpp.sMan  == acpl::Const::UI64(0x00080000, 0x00000000));
	
	acpl::Float::GetParts(acpl::Float::SNaN<acpl::Float64>(), oFpp);
	Test(oFpp.sSign == 0);
	Test(oFpp.sExp  == 0x7FF);
	Test(oFpp.sMan  == acpl::Const::UI64(0x00040000, 0x00000000) || (_local_HasSigNaN == false && oFpp.sMan  == acpl::Const::UI64(0x00080000, 0x00000000)));
	
	acpl::Float::GetParts(acpl::Float::NaN<acpl::Float::Largest>(), oFpp);
	Test(oFpp.sSign == 0);
	Test(oFpp.sExp  == 0x7FFF || acpl::Float::HasExtPrec() == false);
	Test(oFpp.sMan  == acpl::Const::UI64(0xC0000000, 0x00000000) || acpl::Float::HasExtPrec() == false);
	
	acpl::Float::GetParts(acpl::Float::QNaN<acpl::Float::Largest>(), oFpp);
	Test(oFpp.sSign == 0);
	Test(oFpp.sExp  == 0x7FFF || acpl::Float::HasExtPrec() == false);
	Test(oFpp.sMan  == acpl::Const::UI64(0xC0000000, 0x00000000) || acpl::Float::HasExtPrec() == false);
	
	acpl::Float::GetParts(acpl::Float::SNaN<acpl::Float::Largest>(), oFpp);
	Test(oFpp.sSign == 0);
	Test(oFpp.sExp  == 0x7FFF || acpl::Float::HasExtPrec() == false);
	Test(oFpp.sMan  == acpl::Const::UI64(0xA0000000, 0x00000000) || acpl::Float::HasExtPrec() == false);
	
	
	// IsNaN() method
	
	Test(acpl::Float::IsNaN(acpl::Float::Inf<acpl::Float32>()) == false);
	Test(acpl::Float::IsNaN(acpl::Float::Inf<acpl::Float64>()) == false);
	Test(acpl::Float::IsNaN(acpl::Float::Inf<acpl::Float::Largest>()) == false);
	
	Test(acpl::Float::IsNaN(acpl::Float::NaN<acpl::Float32>()) == true);
	Test(acpl::Float::IsNaN(acpl::Float::NaN<acpl::Float64>()) == true);
	Test(acpl::Float::IsNaN(acpl::Float::NaN<acpl::Float::Largest>()) == true);
	
	Test(acpl::Float::IsNaN(acpl::Float::QNaN<acpl::Float32>()) == true);
	Test(acpl::Float::IsNaN(acpl::Float::QNaN<acpl::Float64>()) == true);
	Test(acpl::Float::IsNaN(acpl::Float::QNaN<acpl::Float::Largest>()) == true);
	
	Test(acpl::Float::IsNaN(acpl::Float::SNaN<acpl::Float32>()) == true);
	Test(acpl::Float::IsNaN(acpl::Float::SNaN<acpl::Float64>()) == true);
	Test(acpl::Float::IsNaN(acpl::Float::SNaN<acpl::Float::Largest>()) == true);
	
	
	// IsNeg() method
	
	Test(acpl::Float::IsNeg(0.0) == false);
	Test(acpl::Float::IsNeg(0.0 * -1) == true);
	Test(acpl::Float::IsNeg(1.0) == false);
	Test(acpl::Float::IsNeg(1.0 * -1) == true);
	
	Test(acpl::Float::IsNeg(acpl::Float::Max<acpl::Float32>()) == false);
	Test(acpl::Float::IsNeg(acpl::Float::Max<acpl::Float32>() * -1) == true);
	Test(acpl::Float::IsNeg(acpl::Float::Min<acpl::Float32>()) == true);
	Test(acpl::Float::IsNeg(acpl::Float::Min<acpl::Float32>() * -1) == false);
	
	Test(acpl::Float::IsNeg(acpl::Float::Max<acpl::Float64>()) == false);
	Test(acpl::Float::IsNeg(acpl::Float::Max<acpl::Float64>() * -1) == true);
	Test(acpl::Float::IsNeg(acpl::Float::Min<acpl::Float64>()) == true);
	Test(acpl::Float::IsNeg(acpl::Float::Min<acpl::Float64>() * -1) == false);
	
	Test(acpl::Float::IsNeg(acpl::Float::Max<acpl::Float::Largest>()) == false);
	Test(acpl::Float::IsNeg(acpl::Float::Max<acpl::Float::Largest>() * -1) == true);
	Test(acpl::Float::IsNeg(acpl::Float::Min<acpl::Float::Largest>()) == true);
	Test(acpl::Float::IsNeg(acpl::Float::Min<acpl::Float::Largest>() * -1) == false);
	
	Test(acpl::Float::IsNeg(acpl::Float::Inf<acpl::Float32>()) == false);
	Test(acpl::Float::IsNeg(acpl::Float::Inf<acpl::Float32>() * -1) == true);
	Test(acpl::Float::IsNeg(acpl::Float::Inf<acpl::Float64>()) == false);
	Test(acpl::Float::IsNeg(acpl::Float::Inf<acpl::Float64>() * -1) == true);
	Test(acpl::Float::IsNeg(acpl::Float::Inf<acpl::Float::Largest>()) == false);
	Test(acpl::Float::IsNeg(acpl::Float::Inf<acpl::Float::Largest>() * -1) == true);
	
	
	return 0;
}

static int TestNumClass()
{
	PrintFn();
	
	
	// IsSigned
	
	Test(acpl::Num<acpl::SInt8 >::IsSigned() == true);
	Test(acpl::Num<acpl::SInt16>::IsSigned() == true);
	Test(acpl::Num<acpl::SInt32>::IsSigned() == true);
	Test(acpl::Num<acpl::SInt64>::IsSigned() == true);
	
	Test(acpl::Num<acpl::UInt8 >::IsSigned() == false);
	Test(acpl::Num<acpl::UInt16>::IsSigned() == false);
	Test(acpl::Num<acpl::UInt32>::IsSigned() == false);
	Test(acpl::Num<acpl::UInt64>::IsSigned() == false);
	
	Test(acpl::Num<acpl::Float32>::IsSigned() == true);
	Test(acpl::Num<acpl::Float64>::IsSigned() == true);
	
	Test(acpl::Num<acpl::SizeT >::IsSigned() == false);
	Test(acpl::Num<acpl::SSizeT>::IsSigned() == true);
	Test(acpl::Num<acpl::OffT  >::IsSigned() == true);
	
	
	// IsFloat
	
	Test(acpl::Num<acpl::SInt8 >::IsFloat() == false);
	Test(acpl::Num<acpl::SInt16>::IsFloat() == false);
	Test(acpl::Num<acpl::SInt32>::IsFloat() == false);
	Test(acpl::Num<acpl::SInt64>::IsFloat() == false);
	
	Test(acpl::Num<acpl::UInt8 >::IsFloat() == false);
	Test(acpl::Num<acpl::UInt16>::IsFloat() == false);
	Test(acpl::Num<acpl::UInt32>::IsFloat() == false);
	Test(acpl::Num<acpl::UInt64>::IsFloat() == false);
	
	Test(acpl::Num<acpl::Float32>::IsFloat() == true);
	Test(acpl::Num<acpl::Float64>::IsFloat() == true);
	Test(acpl::Num<acpl::Float::Largest>::IsFloat() == true);
	
	Test(acpl::Num<acpl::SizeT >::IsFloat() == false);
	Test(acpl::Num<acpl::SSizeT>::IsFloat() == false);
	Test(acpl::Num<acpl::OffT  >::IsFloat() == false);
	
	
	// Min
	
	Test(acpl::Num<acpl::SInt8 >::Min() == -128);
	Test(acpl::Num<acpl::SInt16>::Min() == -32768);
	Test(acpl::Num<acpl::SInt32>::Min() == static_cast<acpl::SInt32>(0x80000000));
	Test(acpl::Num<acpl::SInt64>::Min() == static_cast<acpl::SInt64>(acpl::Const::UI64(0x80000000, 0x00000000)));
	
	Test(acpl::Num<acpl::UInt8 >::Min() == 0);
	Test(acpl::Num<acpl::UInt16>::Min() == 0);
	Test(acpl::Num<acpl::UInt32>::Min() == 0);
	Test(acpl::Num<acpl::UInt64>::Min() == 0);
	
	// More comprehensive tests are performed in tests of `acpl::Float` class
	Test(acpl::Num<acpl::Float32>::Min() == acpl::Float::Min<acpl::Float32>());
	Test(acpl::Num<acpl::Float64>::Min() == acpl::Float::Min<acpl::Float64>());
	Test(acpl::Num<acpl::Float::Largest>::Min() == acpl::Float::Min<acpl::Float::Largest>());
	
	Test(acpl::Num<acpl::SizeT >::Min() == 0);
	
	if (sizeof(acpl::SSizeT) == 8)
		Test(acpl::Num<acpl::SSizeT>::Min() == static_cast<acpl::SSizeT>(acpl::Const::UI64(0x80000000, 0x00000000)))
	else
	if (sizeof(acpl::SSizeT) == 4)
		Test(acpl::Num<acpl::SSizeT>::Min() == static_cast<acpl::SSizeT>(0x80000000))
	else
		Test(false)
	
	Test(acpl::Num<acpl::OffT  >::Min() == static_cast<acpl::OffT  >(acpl::Const::UI64(0x80000000, 0x00000000)));
	
	
	// Max
	
	Test(acpl::Num<acpl::SInt8 >::Max() == 0x7F);
	Test(acpl::Num<acpl::SInt16>::Max() == 0x7FFF);
	Test(acpl::Num<acpl::SInt32>::Max() == 0x7FFFFFFF);
	Test(acpl::Num<acpl::SInt64>::Max() == static_cast<acpl::SInt64>(acpl::Const::UI64(0x7FFFFFFF, 0xFFFFFFFF)));
	
	Test(acpl::Num<acpl::UInt8 >::Max() == 0xFF);
	Test(acpl::Num<acpl::UInt16>::Max() == 0xFFFF);
	Test(acpl::Num<acpl::UInt32>::Max() == 0xFFFFFFFF);
	Test(acpl::Num<acpl::UInt64>::Max() == acpl::Const::UI64(0xFFFFFFFF, 0xFFFFFFFF));
	
	// More comprehensive tests are performed in tests of `acpl::Float` class
	Test(acpl::Num<acpl::Float32>::Max() == acpl::Float::Max<acpl::Float32>());
	Test(acpl::Num<acpl::Float64>::Max() == acpl::Float::Max<acpl::Float64>());
	Test(acpl::Num<acpl::Float::Largest>::Max() == acpl::Float::Max<acpl::Float::Largest>());
	
	if (sizeof(acpl::SizeT ) == 8)
		Test(acpl::Num<acpl::SizeT >::Max() == static_cast<acpl::SizeT >(acpl::Const::UI64(0xFFFFFFFF, 0xFFFFFFFF)))
	else
	if (sizeof(acpl::SizeT ) == 4)
		Test(acpl::Num<acpl::SizeT >::Max() == static_cast<acpl::SizeT >(0xFFFFFFFF))
	else
		Test(false)
	
	if (sizeof(acpl::SSizeT) == 8)
		Test(acpl::Num<acpl::SSizeT>::Max() == static_cast<acpl::SSizeT>(acpl::Const::UI64(0x7FFFFFFF, 0xFFFFFFFF)))
	else
	if (sizeof(acpl::SSizeT) == 4)
		Test(acpl::Num<acpl::SSizeT>::Max() == static_cast<acpl::SSizeT>(0x7FFFFFFF))
	else
		Test(false)
	
	Test(acpl::Num<acpl::OffT  >::Max() == static_cast<acpl::OffT  >(acpl::Const::UI64(0x7FFFFFFF, 0xFFFFFFFF)));
	
	
	return 0;
}

static int TestCasts()
{
	PrintFn();
	
	
	acpl::UInt64 oUint = 0;
	acpl::SInt64 oSint = 0;
	acpl::Float64 oFloat = 0.0;
	
	
	// Static casts
	
	Test(oUint == static_cast<acpl::UInt64>(oSint));
	Test(oSint == static_cast<acpl::SInt64>(oUint));
	Test(oFloat == static_cast<acpl::Float64>(oSint));
	Test(oSint == static_cast<acpl::SInt64>(oFloat));
	Test(oFloat == static_cast<acpl::Float64>(oUint));
	Test(oUint == static_cast<acpl::UInt64>(oFloat));
	oUint++;
	oSint++;
	oFloat++;
	Test(oUint == static_cast<acpl::UInt64>(oSint));
	Test(oSint == static_cast<acpl::SInt64>(oUint));
	Test(oFloat == static_cast<acpl::Float64>(oSint));
	Test(oSint == static_cast<acpl::SInt64>(oFloat));
	Test(oFloat == static_cast<acpl::Float64>(oUint));
	Test(oUint == static_cast<acpl::UInt64>(oFloat));
	oUint -= 2;
	oSint -= 2;
	oFloat -= 2;
	Test(oUint == static_cast<acpl::UInt64>(oSint));
	Test(oSint == static_cast<acpl::SInt64>(oUint));
	Test(oFloat == static_cast<acpl::Float64>(oSint));
	Test(oSint == static_cast<acpl::SInt64>(oFloat));
	Test(oFloat != static_cast<acpl::Float64>(oUint));
	// WARNING: Do not run the following test because it is an undefined
	// behaviour when trying to cast a negative floating-point value to an
	// unsigned integer value. Here is the difference between GCC and MSVC:
	//   * GCC says:  `static_cast<acpl::UInt64>(-1.0) == 0`
	//   * MSVC says: `static_cast<acpl::UInt64>(-1.0) == 4294967295` (on 32-bit
	//     system)
	// 
	// Test(oUint == static_cast<acpl::UInt64>(oFloat));
	
	
	// acpl::Const::{U,S}I64 methods
	
	oUint = static_cast<acpl::UInt64>(-1);
	Test(oUint == acpl::Const::UI64(0xFFFFFFFF, 0xFFFFFFFF));
	
	oSint = -1;
	Test(oSint == acpl::Const::SI64(0xFFFFFFFF, 0xFFFFFFFF));
	
	
	return 0;
}

static int TestEnum()
{
	PrintFn();
	
	
	acpl::Enum<_local_TestEnum, acpl::UInt8> oEnum(_local_teVal1);
	
	Test(oEnum == _local_teVal1);
	Test(oEnum != _local_teVal0);
	Test(oEnum != _local_teVal2);
	
	Test(oEnum <  _local_teVal2);
	Test(oEnum >  _local_teVal0);
	
	Test(oEnum <= _local_teVal1);
	Test(oEnum >= _local_teVal1);
	
	oEnum = _local_teVal0;
	Test(oEnum == _local_teVal0);
	
	
	return 0;
}


SectionFuncMain(types)
{
	if (argc < 1)
	{
		Test(TestTypeSizes() == 0);
		Test(TestFloatClass() == 0);
		Test(TestNumClass() == 0);
		Test(TestCasts() == 0);
		Test(TestEnum() == 0);
		
		PrintOut("All Passed!\n");
	}
	else
		return PrintUnrecParams(acplDbgSrcLoc, lSectionName, argc, argv);
	
	return 0;
}

SectionFuncParamsBegin(types)
// SectionFuncParamsAdd("param [param [param ...]]", "description")
SectionFuncParamsEnd(types)
