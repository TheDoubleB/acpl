#include "Tests.h"

#include "../src/Endian.h"
#include "../src/Memory.h"


typedef acpl::UInt8 _local_Bfr[8];
const _local_Bfr lConstBfr = { 0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF };


static int _local_ModBfrTestOnly(const _local_Bfr &nModBfr, acpl::UInt8 nCount)
{
	for (acpl::UInt8 i = 0; i < sizeof(_local_Bfr); i++)
	{
		if (i < nCount)
			TestFM(nModBfr[i] == lConstBfr[i], "i=%u", i)
		else
			TestFM(nModBfr[i] == 0x00, "i=%u", i)
	}
	
	return 0;
}

static int _local_ModBfrClearOnly(_local_Bfr &nModBfr)
{
	for (acpl::UInt8 i = 0; i < sizeof(_local_Bfr); i++)
		nModBfr[i] = 0x00;
	
	return 0;
}

static int _local_ModBfrTestClear(_local_Bfr &nModBfr, acpl::UInt8 nCount)
{
	for (acpl::UInt8 i = 0; i < sizeof(_local_Bfr); i++)
	{
		if (i < nCount)
			TestFM(nModBfr[i] == lConstBfr[i], "i=%u", i)
		else
			TestFM(nModBfr[i] == 0x00, "i=%u, 0x%02X", i, nModBfr[i])
		
		nModBfr[i] = 0x00;
	}
	
	return 0;
}


static int TestEndianness()
{
	PrintFn();
	
	PrintOut("System endianness: is-big == %s; is-little == %s\n",
			((acpl::Endian::IsNativeBig() == true) ? "true" : "false"),
			((acpl::Endian::IsNativeLittle() == true) ? "true" : "false")
		);
	
	
	acpl::UInt32 oVal;
	_local_Bfr oModBfr = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	
	
	// FromBig (normal)
	
	Test(acpl::Endian::FromBig<acpl::UInt8 >(lConstBfr, 0) == 0x00);
	Test(acpl::Endian::FromBig<acpl::UInt8 >(lConstBfr, 1) == 0x01);
	Test(acpl::Endian::FromBig<acpl::UInt8 >(lConstBfr, 2) == 0x01);
	Test(acpl::Endian::FromBig<acpl::UInt16>(lConstBfr, 0) == 0x0000);
	Test(acpl::Endian::FromBig<acpl::UInt16>(lConstBfr, 1) == 0x0001);
	Test(acpl::Endian::FromBig<acpl::UInt16>(lConstBfr, 2) == 0x0123);
	Test(acpl::Endian::FromBig<acpl::UInt16>(lConstBfr, 3) == 0x0123);
	Test(acpl::Endian::FromBig<acpl::UInt32>(lConstBfr, 0) == 0x00000000);
	Test(acpl::Endian::FromBig<acpl::UInt32>(lConstBfr, 1) == 0x00000001);
	Test(acpl::Endian::FromBig<acpl::UInt32>(lConstBfr, 2) == 0x00000123);
	Test(acpl::Endian::FromBig<acpl::UInt32>(lConstBfr, 3) == 0x00012345);
	Test(acpl::Endian::FromBig<acpl::UInt32>(lConstBfr, 4) == 0x01234567);
	Test(acpl::Endian::FromBig<acpl::UInt32>(lConstBfr, 5) == 0x01234567);
	Test(acpl::Endian::FromBig<acpl::UInt64>(lConstBfr, 0) == acpl::Const::UI64(0x00000000, 0x00000000));
	Test(acpl::Endian::FromBig<acpl::UInt64>(lConstBfr, 1) == acpl::Const::UI64(0x00000000, 0x00000001));
	Test(acpl::Endian::FromBig<acpl::UInt64>(lConstBfr, 2) == acpl::Const::UI64(0x00000000, 0x00000123));
	Test(acpl::Endian::FromBig<acpl::UInt64>(lConstBfr, 3) == acpl::Const::UI64(0x00000000, 0x00012345));
	Test(acpl::Endian::FromBig<acpl::UInt64>(lConstBfr, 4) == acpl::Const::UI64(0x00000000, 0x01234567));
	Test(acpl::Endian::FromBig<acpl::UInt64>(lConstBfr, 5) == acpl::Const::UI64(0x00000001, 0x23456789));
	Test(acpl::Endian::FromBig<acpl::UInt64>(lConstBfr, 6) == acpl::Const::UI64(0x00000123, 0x456789AB));
	Test(acpl::Endian::FromBig<acpl::UInt64>(lConstBfr, 7) == acpl::Const::UI64(0x00012345, 0x6789ABCD));
	Test(acpl::Endian::FromBig<acpl::UInt64>(lConstBfr, 8) == acpl::Const::UI64(0x01234567, 0x89ABCDEF));
	Test(acpl::Endian::FromBig<acpl::UInt64>(lConstBfr, 9) == acpl::Const::UI64(0x01234567, 0x89ABCDEF));
	
	Test(acpl::Endian::FromBig<acpl::SInt8 >(lConstBfr, 0) == 0x00);
	Test(acpl::Endian::FromBig<acpl::SInt8 >(lConstBfr, 1) == 0x01);
	Test(acpl::Endian::FromBig<acpl::SInt8 >(lConstBfr, 2) == 0x01);
	Test(acpl::Endian::FromBig<acpl::SInt16>(lConstBfr, 0) == 0x0000);
	Test(acpl::Endian::FromBig<acpl::SInt16>(lConstBfr, 1) == 0x0001);
	Test(acpl::Endian::FromBig<acpl::SInt16>(lConstBfr, 2) == 0x0123);
	Test(acpl::Endian::FromBig<acpl::SInt16>(lConstBfr, 3) == 0x0123);
	Test(acpl::Endian::FromBig<acpl::SInt32>(lConstBfr, 0) == 0x00000000);
	Test(acpl::Endian::FromBig<acpl::SInt32>(lConstBfr, 1) == 0x00000001);
	Test(acpl::Endian::FromBig<acpl::SInt32>(lConstBfr, 2) == 0x00000123);
	Test(acpl::Endian::FromBig<acpl::SInt32>(lConstBfr, 3) == 0x00012345);
	Test(acpl::Endian::FromBig<acpl::SInt32>(lConstBfr, 4) == 0x01234567);
	Test(acpl::Endian::FromBig<acpl::SInt32>(lConstBfr, 5) == 0x01234567);
	Test(acpl::Endian::FromBig<acpl::SInt64>(lConstBfr, 0) == static_cast<acpl::SInt64>(acpl::Const::UI64(0x00000000, 0x00000000)));
	Test(acpl::Endian::FromBig<acpl::SInt64>(lConstBfr, 1) == static_cast<acpl::SInt64>(acpl::Const::UI64(0x00000000, 0x00000001)));
	Test(acpl::Endian::FromBig<acpl::SInt64>(lConstBfr, 2) == static_cast<acpl::SInt64>(acpl::Const::UI64(0x00000000, 0x00000123)));
	Test(acpl::Endian::FromBig<acpl::SInt64>(lConstBfr, 3) == static_cast<acpl::SInt64>(acpl::Const::UI64(0x00000000, 0x00012345)));
	Test(acpl::Endian::FromBig<acpl::SInt64>(lConstBfr, 4) == static_cast<acpl::SInt64>(acpl::Const::UI64(0x00000000, 0x01234567)));
	Test(acpl::Endian::FromBig<acpl::SInt64>(lConstBfr, 5) == static_cast<acpl::SInt64>(acpl::Const::UI64(0x00000001, 0x23456789)));
	Test(acpl::Endian::FromBig<acpl::SInt64>(lConstBfr, 6) == static_cast<acpl::SInt64>(acpl::Const::UI64(0x00000123, 0x456789AB)));
	Test(acpl::Endian::FromBig<acpl::SInt64>(lConstBfr, 7) == static_cast<acpl::SInt64>(acpl::Const::UI64(0x00012345, 0x6789ABCD)));
	Test(acpl::Endian::FromBig<acpl::SInt64>(lConstBfr, 8) == static_cast<acpl::SInt64>(acpl::Const::UI64(0x01234567, 0x89ABCDEF)));
	Test(acpl::Endian::FromBig<acpl::SInt64>(lConstBfr, 9) == static_cast<acpl::SInt64>(acpl::Const::UI64(0x01234567, 0x89ABCDEF)));
	
	oVal = 0;
	acpl::Endian::FromBig(oVal, lConstBfr, 3);
	Test(oVal == 0x00012345);
	
	
	// FromBig (with mask)
	
	Test(acpl::Endian::FromBig<acpl::UInt8 >(0x00, lConstBfr) == 0x00);
	Test(acpl::Endian::FromBig<acpl::UInt8 >(0xFF, lConstBfr) == 0x01);
	Test(acpl::Endian::FromBig<acpl::UInt16>(0x0000, lConstBfr) == 0x0000);
	Test(acpl::Endian::FromBig<acpl::UInt16>(0x00FF, lConstBfr) == 0x0023);
	Test(acpl::Endian::FromBig<acpl::UInt16>(0xFFFF, lConstBfr) == 0x0123);
	Test(acpl::Endian::FromBig<acpl::UInt32>(0x00000000, lConstBfr) == 0x00000000);
	Test(acpl::Endian::FromBig<acpl::UInt32>(0x000000FF, lConstBfr) == 0x00000067);
	Test(acpl::Endian::FromBig<acpl::UInt32>(0x0000FFFF, lConstBfr) == 0x00004567);
	Test(acpl::Endian::FromBig<acpl::UInt32>(0x00FFFFFF, lConstBfr) == 0x00234567);
	Test(acpl::Endian::FromBig<acpl::UInt32>(0xFFFFFFFF, lConstBfr) == 0x01234567);
	Test(acpl::Endian::FromBig<acpl::UInt64>(acpl::Const::UI64(0x00000000, 0x00000000), lConstBfr) == acpl::Const::UI64(0x00000000, 0x00000000));
	Test(acpl::Endian::FromBig<acpl::UInt64>(acpl::Const::UI64(0x00000000, 0x000000FF), lConstBfr) == acpl::Const::UI64(0x00000000, 0x000000EF));
	Test(acpl::Endian::FromBig<acpl::UInt64>(acpl::Const::UI64(0x00000000, 0x0000FFFF), lConstBfr) == acpl::Const::UI64(0x00000000, 0x0000CDEF));
	Test(acpl::Endian::FromBig<acpl::UInt64>(acpl::Const::UI64(0x00000000, 0x00FFFFFF), lConstBfr) == acpl::Const::UI64(0x00000000, 0x00ABCDEF));
	Test(acpl::Endian::FromBig<acpl::UInt64>(acpl::Const::UI64(0x00000000, 0xFFFFFFFF), lConstBfr) == acpl::Const::UI64(0x00000000, 0x89ABCDEF));
	Test(acpl::Endian::FromBig<acpl::UInt64>(acpl::Const::UI64(0x000000FF, 0xFFFFFFFF), lConstBfr) == acpl::Const::UI64(0x00000067, 0x89ABCDEF));
	Test(acpl::Endian::FromBig<acpl::UInt64>(acpl::Const::UI64(0x0000FFFF, 0xFFFFFFFF), lConstBfr) == acpl::Const::UI64(0x00004567, 0x89ABCDEF));
	Test(acpl::Endian::FromBig<acpl::UInt64>(acpl::Const::UI64(0x00FFFFFF, 0xFFFFFFFF), lConstBfr) == acpl::Const::UI64(0x00234567, 0x89ABCDEF));
	Test(acpl::Endian::FromBig<acpl::UInt64>(acpl::Const::UI64(0xFFFFFFFF, 0xFFFFFFFF), lConstBfr) == acpl::Const::UI64(0x01234567, 0x89ABCDEF));
	
	Test(acpl::Endian::FromBig<acpl::SInt8 >(0x00, lConstBfr) == 0x00);
	Test(acpl::Endian::FromBig<acpl::SInt8 >(0xFF, lConstBfr) == 0x01);
	Test(acpl::Endian::FromBig<acpl::SInt16>(0x0000, lConstBfr) == 0x0000);
	Test(acpl::Endian::FromBig<acpl::SInt16>(0x00FF, lConstBfr) == 0x0023);
	Test(acpl::Endian::FromBig<acpl::SInt16>(0xFFFF, lConstBfr) == 0x0123);
	Test(acpl::Endian::FromBig<acpl::SInt32>(0x00000000, lConstBfr) == 0x00000000);
	Test(acpl::Endian::FromBig<acpl::SInt32>(0x000000FF, lConstBfr) == 0x00000067);
	Test(acpl::Endian::FromBig<acpl::SInt32>(0x0000FFFF, lConstBfr) == 0x00004567);
	Test(acpl::Endian::FromBig<acpl::SInt32>(0x00FFFFFF, lConstBfr) == 0x00234567);
	Test(acpl::Endian::FromBig<acpl::SInt32>(0xFFFFFFFF, lConstBfr) == 0x01234567);
	Test(acpl::Endian::FromBig<acpl::SInt64>(acpl::Const::UI64(0x00000000, 0x00000000), lConstBfr) == static_cast<acpl::SInt64>(acpl::Const::UI64(0x00000000, 0x00000000)));
	Test(acpl::Endian::FromBig<acpl::SInt64>(acpl::Const::UI64(0x00000000, 0x000000FF), lConstBfr) == static_cast<acpl::SInt64>(acpl::Const::UI64(0x00000000, 0x000000EF)));
	Test(acpl::Endian::FromBig<acpl::SInt64>(acpl::Const::UI64(0x00000000, 0x0000FFFF), lConstBfr) == static_cast<acpl::SInt64>(acpl::Const::UI64(0x00000000, 0x0000CDEF)));
	Test(acpl::Endian::FromBig<acpl::SInt64>(acpl::Const::UI64(0x00000000, 0x00FFFFFF), lConstBfr) == static_cast<acpl::SInt64>(acpl::Const::UI64(0x00000000, 0x00ABCDEF)));
	Test(acpl::Endian::FromBig<acpl::SInt64>(acpl::Const::UI64(0x00000000, 0xFFFFFFFF), lConstBfr) == static_cast<acpl::SInt64>(acpl::Const::UI64(0x00000000, 0x89ABCDEF)));
	Test(acpl::Endian::FromBig<acpl::SInt64>(acpl::Const::UI64(0x000000FF, 0xFFFFFFFF), lConstBfr) == static_cast<acpl::SInt64>(acpl::Const::UI64(0x00000067, 0x89ABCDEF)));
	Test(acpl::Endian::FromBig<acpl::SInt64>(acpl::Const::UI64(0x0000FFFF, 0xFFFFFFFF), lConstBfr) == static_cast<acpl::SInt64>(acpl::Const::UI64(0x00004567, 0x89ABCDEF)));
	Test(acpl::Endian::FromBig<acpl::SInt64>(acpl::Const::UI64(0x00FFFFFF, 0xFFFFFFFF), lConstBfr) == static_cast<acpl::SInt64>(acpl::Const::UI64(0x00234567, 0x89ABCDEF)));
	Test(acpl::Endian::FromBig<acpl::SInt64>(acpl::Const::UI64(0xFFFFFFFF, 0xFFFFFFFF), lConstBfr) == static_cast<acpl::SInt64>(acpl::Const::UI64(0x01234567, 0x89ABCDEF)));
	
	oVal = 0;
	acpl::Endian::FromBig(oVal, 0x00FFFFFF, lConstBfr);
	Test(oVal == 0x00234567);
	
	
	// FromLittle (normal)
	
	Test(acpl::Endian::FromLittle<acpl::UInt8 >(lConstBfr, 0) == 0x00);
	Test(acpl::Endian::FromLittle<acpl::UInt8 >(lConstBfr, 1) == 0x01);
	Test(acpl::Endian::FromLittle<acpl::UInt8 >(lConstBfr, 2) == 0x01);
	Test(acpl::Endian::FromLittle<acpl::UInt16>(lConstBfr, 0) == 0x0000);
	Test(acpl::Endian::FromLittle<acpl::UInt16>(lConstBfr, 1) == 0x0001);
	Test(acpl::Endian::FromLittle<acpl::UInt16>(lConstBfr, 2) == 0x2301);
	Test(acpl::Endian::FromLittle<acpl::UInt16>(lConstBfr, 3) == 0x2301);
	Test(acpl::Endian::FromLittle<acpl::UInt32>(lConstBfr, 0) == 0x00000000);
	Test(acpl::Endian::FromLittle<acpl::UInt32>(lConstBfr, 1) == 0x00000001);
	Test(acpl::Endian::FromLittle<acpl::UInt32>(lConstBfr, 2) == 0x00002301);
	Test(acpl::Endian::FromLittle<acpl::UInt32>(lConstBfr, 3) == 0x00452301);
	Test(acpl::Endian::FromLittle<acpl::UInt32>(lConstBfr, 4) == 0x67452301);
	Test(acpl::Endian::FromLittle<acpl::UInt32>(lConstBfr, 5) == 0x67452301);
	Test(acpl::Endian::FromLittle<acpl::UInt64>(lConstBfr, 0) == acpl::Const::UI64(0x00000000, 0x00000000));
	Test(acpl::Endian::FromLittle<acpl::UInt64>(lConstBfr, 1) == acpl::Const::UI64(0x00000000, 0x00000001));
	Test(acpl::Endian::FromLittle<acpl::UInt64>(lConstBfr, 2) == acpl::Const::UI64(0x00000000, 0x00002301));
	Test(acpl::Endian::FromLittle<acpl::UInt64>(lConstBfr, 3) == acpl::Const::UI64(0x00000000, 0x00452301));
	Test(acpl::Endian::FromLittle<acpl::UInt64>(lConstBfr, 4) == acpl::Const::UI64(0x00000000, 0x67452301));
	Test(acpl::Endian::FromLittle<acpl::UInt64>(lConstBfr, 5) == acpl::Const::UI64(0x00000089, 0x67452301));
	Test(acpl::Endian::FromLittle<acpl::UInt64>(lConstBfr, 6) == acpl::Const::UI64(0x0000AB89, 0x67452301));
	Test(acpl::Endian::FromLittle<acpl::UInt64>(lConstBfr, 7) == acpl::Const::UI64(0x00CDAB89, 0x67452301));
	Test(acpl::Endian::FromLittle<acpl::UInt64>(lConstBfr, 8) == acpl::Const::UI64(0xEFCDAB89, 0x67452301));
	Test(acpl::Endian::FromLittle<acpl::UInt64>(lConstBfr, 9) == acpl::Const::UI64(0xEFCDAB89, 0x67452301));
	
	Test(acpl::Endian::FromLittle<acpl::SInt8 >(lConstBfr, 0) == 0x00);
	Test(acpl::Endian::FromLittle<acpl::SInt8 >(lConstBfr, 1) == 0x01);
	Test(acpl::Endian::FromLittle<acpl::SInt8 >(lConstBfr, 2) == 0x01);
	Test(acpl::Endian::FromLittle<acpl::SInt16>(lConstBfr, 0) == 0x0000);
	Test(acpl::Endian::FromLittle<acpl::SInt16>(lConstBfr, 1) == 0x0001);
	Test(acpl::Endian::FromLittle<acpl::SInt16>(lConstBfr, 2) == 0x2301);
	Test(acpl::Endian::FromLittle<acpl::SInt16>(lConstBfr, 3) == 0x2301);
	Test(acpl::Endian::FromLittle<acpl::SInt32>(lConstBfr, 0) == 0x00000000);
	Test(acpl::Endian::FromLittle<acpl::SInt32>(lConstBfr, 1) == 0x00000001);
	Test(acpl::Endian::FromLittle<acpl::SInt32>(lConstBfr, 2) == 0x00002301);
	Test(acpl::Endian::FromLittle<acpl::SInt32>(lConstBfr, 3) == 0x00452301);
	Test(acpl::Endian::FromLittle<acpl::SInt32>(lConstBfr, 4) == 0x67452301);
	Test(acpl::Endian::FromLittle<acpl::SInt32>(lConstBfr, 5) == 0x67452301);
	Test(acpl::Endian::FromLittle<acpl::SInt64>(lConstBfr, 0) == static_cast<acpl::SInt64>(acpl::Const::UI64(0x00000000, 0x00000000)));
	Test(acpl::Endian::FromLittle<acpl::SInt64>(lConstBfr, 1) == static_cast<acpl::SInt64>(acpl::Const::UI64(0x00000000, 0x00000001)));
	Test(acpl::Endian::FromLittle<acpl::SInt64>(lConstBfr, 2) == static_cast<acpl::SInt64>(acpl::Const::UI64(0x00000000, 0x00002301)));
	Test(acpl::Endian::FromLittle<acpl::SInt64>(lConstBfr, 3) == static_cast<acpl::SInt64>(acpl::Const::UI64(0x00000000, 0x00452301)));
	Test(acpl::Endian::FromLittle<acpl::SInt64>(lConstBfr, 4) == static_cast<acpl::SInt64>(acpl::Const::UI64(0x00000000, 0x67452301)));
	Test(acpl::Endian::FromLittle<acpl::SInt64>(lConstBfr, 5) == static_cast<acpl::SInt64>(acpl::Const::UI64(0x00000089, 0x67452301)));
	Test(acpl::Endian::FromLittle<acpl::SInt64>(lConstBfr, 6) == static_cast<acpl::SInt64>(acpl::Const::UI64(0x0000AB89, 0x67452301)));
	Test(acpl::Endian::FromLittle<acpl::SInt64>(lConstBfr, 7) == static_cast<acpl::SInt64>(acpl::Const::UI64(0x00CDAB89, 0x67452301)));
	Test(acpl::Endian::FromLittle<acpl::SInt64>(lConstBfr, 8) == static_cast<acpl::SInt64>(acpl::Const::UI64(0xEFCDAB89, 0x67452301)));
	Test(acpl::Endian::FromLittle<acpl::SInt64>(lConstBfr, 9) == static_cast<acpl::SInt64>(acpl::Const::UI64(0xEFCDAB89, 0x67452301)));
	
	oVal = 0;
	acpl::Endian::FromLittle(oVal, lConstBfr, 3);
	Test(oVal == 0x00452301);
	
	
	// FromLittle (with mask)
	
	Test(acpl::Endian::FromLittle<acpl::UInt8 >(0x00, lConstBfr) == 0x00);
	Test(acpl::Endian::FromLittle<acpl::UInt8 >(0xFF, lConstBfr) == 0x01);
	Test(acpl::Endian::FromLittle<acpl::UInt16>(0x0000, lConstBfr) == 0x0000);
	Test(acpl::Endian::FromLittle<acpl::UInt16>(0x00FF, lConstBfr) == 0x0001);
	Test(acpl::Endian::FromLittle<acpl::UInt16>(0xFFFF, lConstBfr) == 0x2301);
	Test(acpl::Endian::FromLittle<acpl::UInt32>(0x00000000, lConstBfr) == 0x00000000);
	Test(acpl::Endian::FromLittle<acpl::UInt32>(0x000000FF, lConstBfr) == 0x00000001);
	Test(acpl::Endian::FromLittle<acpl::UInt32>(0x0000FFFF, lConstBfr) == 0x00002301);
	Test(acpl::Endian::FromLittle<acpl::UInt32>(0x00FFFFFF, lConstBfr) == 0x00452301);
	Test(acpl::Endian::FromLittle<acpl::UInt32>(0xFFFFFFFF, lConstBfr) == 0x67452301);
	Test(acpl::Endian::FromLittle<acpl::UInt64>(acpl::Const::UI64(0x00000000, 0x00000000), lConstBfr) == acpl::Const::UI64(0x00000000, 0x00000000));
	Test(acpl::Endian::FromLittle<acpl::UInt64>(acpl::Const::UI64(0x00000000, 0x000000FF), lConstBfr) == acpl::Const::UI64(0x00000000, 0x00000001));
	Test(acpl::Endian::FromLittle<acpl::UInt64>(acpl::Const::UI64(0x00000000, 0x0000FFFF), lConstBfr) == acpl::Const::UI64(0x00000000, 0x00002301));
	Test(acpl::Endian::FromLittle<acpl::UInt64>(acpl::Const::UI64(0x00000000, 0x00FFFFFF), lConstBfr) == acpl::Const::UI64(0x00000000, 0x00452301));
	Test(acpl::Endian::FromLittle<acpl::UInt64>(acpl::Const::UI64(0x00000000, 0xFFFFFFFF), lConstBfr) == acpl::Const::UI64(0x00000000, 0x67452301));
	Test(acpl::Endian::FromLittle<acpl::UInt64>(acpl::Const::UI64(0x000000FF, 0xFFFFFFFF), lConstBfr) == acpl::Const::UI64(0x00000089, 0x67452301));
	Test(acpl::Endian::FromLittle<acpl::UInt64>(acpl::Const::UI64(0x0000FFFF, 0xFFFFFFFF), lConstBfr) == acpl::Const::UI64(0x0000AB89, 0x67452301));
	Test(acpl::Endian::FromLittle<acpl::UInt64>(acpl::Const::UI64(0x00FFFFFF, 0xFFFFFFFF), lConstBfr) == acpl::Const::UI64(0x00CDAB89, 0x67452301));
	Test(acpl::Endian::FromLittle<acpl::UInt64>(acpl::Const::UI64(0xFFFFFFFF, 0xFFFFFFFF), lConstBfr) == acpl::Const::UI64(0xEFCDAB89, 0x67452301));
	
	Test(acpl::Endian::FromLittle<acpl::SInt8 >(0x00, lConstBfr) == 0x00);
	Test(acpl::Endian::FromLittle<acpl::SInt8 >(0xFF, lConstBfr) == 0x01);
	Test(acpl::Endian::FromLittle<acpl::SInt16>(0x0000, lConstBfr) == 0x0000);
	Test(acpl::Endian::FromLittle<acpl::SInt16>(0x00FF, lConstBfr) == 0x0001);
	Test(acpl::Endian::FromLittle<acpl::SInt16>(0xFFFF, lConstBfr) == 0x2301);
	Test(acpl::Endian::FromLittle<acpl::SInt32>(0x00000000, lConstBfr) == 0x00000000);
	Test(acpl::Endian::FromLittle<acpl::SInt32>(0x000000FF, lConstBfr) == 0x00000001);
	Test(acpl::Endian::FromLittle<acpl::SInt32>(0x0000FFFF, lConstBfr) == 0x00002301);
	Test(acpl::Endian::FromLittle<acpl::SInt32>(0x00FFFFFF, lConstBfr) == 0x00452301);
	Test(acpl::Endian::FromLittle<acpl::SInt32>(0xFFFFFFFF, lConstBfr) == 0x67452301);
	Test(acpl::Endian::FromLittle<acpl::SInt64>(acpl::Const::UI64(0x00000000, 0x00000000), lConstBfr) == static_cast<acpl::SInt64>(acpl::Const::UI64(0x00000000, 0x00000000)));
	Test(acpl::Endian::FromLittle<acpl::SInt64>(acpl::Const::UI64(0x00000000, 0x000000FF), lConstBfr) == static_cast<acpl::SInt64>(acpl::Const::UI64(0x00000000, 0x00000001)));
	Test(acpl::Endian::FromLittle<acpl::SInt64>(acpl::Const::UI64(0x00000000, 0x0000FFFF), lConstBfr) == static_cast<acpl::SInt64>(acpl::Const::UI64(0x00000000, 0x00002301)));
	Test(acpl::Endian::FromLittle<acpl::SInt64>(acpl::Const::UI64(0x00000000, 0x00FFFFFF), lConstBfr) == static_cast<acpl::SInt64>(acpl::Const::UI64(0x00000000, 0x00452301)));
	Test(acpl::Endian::FromLittle<acpl::SInt64>(acpl::Const::UI64(0x00000000, 0xFFFFFFFF), lConstBfr) == static_cast<acpl::SInt64>(acpl::Const::UI64(0x00000000, 0x67452301)));
	Test(acpl::Endian::FromLittle<acpl::SInt64>(acpl::Const::UI64(0x000000FF, 0xFFFFFFFF), lConstBfr) == static_cast<acpl::SInt64>(acpl::Const::UI64(0x00000089, 0x67452301)));
	Test(acpl::Endian::FromLittle<acpl::SInt64>(acpl::Const::UI64(0x0000FFFF, 0xFFFFFFFF), lConstBfr) == static_cast<acpl::SInt64>(acpl::Const::UI64(0x0000AB89, 0x67452301)));
	Test(acpl::Endian::FromLittle<acpl::SInt64>(acpl::Const::UI64(0x00FFFFFF, 0xFFFFFFFF), lConstBfr) == static_cast<acpl::SInt64>(acpl::Const::UI64(0x00CDAB89, 0x67452301)));
	Test(acpl::Endian::FromLittle<acpl::SInt64>(acpl::Const::UI64(0xFFFFFFFF, 0xFFFFFFFF), lConstBfr) == static_cast<acpl::SInt64>(acpl::Const::UI64(0xEFCDAB89, 0x67452301)));
	
	oVal = 0;
	acpl::Endian::FromLittle(oVal, 0x00FFFFFF, lConstBfr);
	Test(oVal == 0x00452301);
	
	
	// ToBig (normal)
	
	_local_ModBfrClearOnly(oModBfr); // to clear the buffer
	acpl::Endian::ToBig<acpl::UInt8 >(0x00, oModBfr, 0);
	Test(_local_ModBfrTestClear(oModBfr, 0) == 0);
	acpl::Endian::ToBig<acpl::UInt8 >(0x01, oModBfr, 1);
	Test(_local_ModBfrTestClear(oModBfr, 1) == 0);
	acpl::Endian::ToBig<acpl::UInt8 >(0x01, oModBfr, 2);
	Test(_local_ModBfrTestClear(oModBfr, 1) == 0);
	acpl::Endian::ToBig<acpl::UInt16>(0x0000, oModBfr, 0);
	Test(_local_ModBfrTestClear(oModBfr, 0) == 0);
	acpl::Endian::ToBig<acpl::UInt16>(0x0001, oModBfr, 1);
	Test(_local_ModBfrTestClear(oModBfr, 1) == 0);
	acpl::Endian::ToBig<acpl::UInt16>(0x0123, oModBfr, 2);
	Test(_local_ModBfrTestClear(oModBfr, 2) == 0);
	acpl::Endian::ToBig<acpl::UInt16>(0x0123, oModBfr, 3);
	Test(_local_ModBfrTestClear(oModBfr, 2) == 0);
	acpl::Endian::ToBig<acpl::UInt32>(0x00000000, oModBfr, 0);
	Test(_local_ModBfrTestClear(oModBfr, 0) == 0);
	acpl::Endian::ToBig<acpl::UInt32>(0x00000001, oModBfr, 1);
	Test(_local_ModBfrTestClear(oModBfr, 1) == 0);
	acpl::Endian::ToBig<acpl::UInt32>(0x00000123, oModBfr, 2);
	Test(_local_ModBfrTestClear(oModBfr, 2) == 0);
	acpl::Endian::ToBig<acpl::UInt32>(0x00012345, oModBfr, 3);
	Test(_local_ModBfrTestClear(oModBfr, 3) == 0);
	acpl::Endian::ToBig<acpl::UInt32>(0x01234567, oModBfr, 4);
	Test(_local_ModBfrTestClear(oModBfr, 4) == 0);
	acpl::Endian::ToBig<acpl::UInt32>(0x01234567, oModBfr, 5);
	Test(_local_ModBfrTestClear(oModBfr, 4) == 0);
	acpl::Endian::ToBig<acpl::UInt64>(acpl::Const::UI64(0x00000000, 0x00000000), oModBfr, 0);
	Test(_local_ModBfrTestClear(oModBfr, 0) == 0);
	acpl::Endian::ToBig<acpl::UInt64>(acpl::Const::UI64(0x00000000, 0x00000001), oModBfr, 1);
	Test(_local_ModBfrTestClear(oModBfr, 1) == 0);
	acpl::Endian::ToBig<acpl::UInt64>(acpl::Const::UI64(0x00000000, 0x00000123), oModBfr, 2);
	Test(_local_ModBfrTestClear(oModBfr, 2) == 0);
	acpl::Endian::ToBig<acpl::UInt64>(acpl::Const::UI64(0x00000000, 0x00012345), oModBfr, 3);
	Test(_local_ModBfrTestClear(oModBfr, 3) == 0);
	acpl::Endian::ToBig<acpl::UInt64>(acpl::Const::UI64(0x00000000, 0x01234567), oModBfr, 4);
	Test(_local_ModBfrTestClear(oModBfr, 4) == 0);
	acpl::Endian::ToBig<acpl::UInt64>(acpl::Const::UI64(0x00000001, 0x23456789), oModBfr, 5);
	Test(_local_ModBfrTestClear(oModBfr, 5) == 0);
	acpl::Endian::ToBig<acpl::UInt64>(acpl::Const::UI64(0x00000123, 0x456789AB), oModBfr, 6);
	Test(_local_ModBfrTestClear(oModBfr, 6) == 0);
	acpl::Endian::ToBig<acpl::UInt64>(acpl::Const::UI64(0x00012345, 0x6789ABCD), oModBfr, 7);
	Test(_local_ModBfrTestClear(oModBfr, 7) == 0);
	acpl::Endian::ToBig<acpl::UInt64>(acpl::Const::UI64(0x01234567, 0x89ABCDEF), oModBfr, 8);
	Test(_local_ModBfrTestClear(oModBfr, 8) == 0);
	acpl::Endian::ToBig<acpl::UInt64>(acpl::Const::UI64(0x01234567, 0x89ABCDEF), oModBfr, 9);
	Test(_local_ModBfrTestClear(oModBfr, 8) == 0);
	
	_local_ModBfrClearOnly(oModBfr); // to clear the buffer
	acpl::Endian::ToBig<acpl::SInt8 >(0x00, oModBfr, 0);
	Test(_local_ModBfrTestClear(oModBfr, 0) == 0);
	acpl::Endian::ToBig<acpl::SInt8 >(0x01, oModBfr, 1);
	Test(_local_ModBfrTestClear(oModBfr, 1) == 0);
	acpl::Endian::ToBig<acpl::SInt8 >(0x01, oModBfr, 2);
	Test(_local_ModBfrTestClear(oModBfr, 1) == 0);
	acpl::Endian::ToBig<acpl::SInt16>(0x0000, oModBfr, 0);
	Test(_local_ModBfrTestClear(oModBfr, 0) == 0);
	acpl::Endian::ToBig<acpl::SInt16>(0x0001, oModBfr, 1);
	Test(_local_ModBfrTestClear(oModBfr, 1) == 0);
	acpl::Endian::ToBig<acpl::SInt16>(0x0123, oModBfr, 2);
	Test(_local_ModBfrTestClear(oModBfr, 2) == 0);
	acpl::Endian::ToBig<acpl::SInt16>(0x0123, oModBfr, 3);
	Test(_local_ModBfrTestClear(oModBfr, 2) == 0);
	acpl::Endian::ToBig<acpl::SInt32>(0x00000000, oModBfr, 0);
	Test(_local_ModBfrTestClear(oModBfr, 0) == 0);
	acpl::Endian::ToBig<acpl::SInt32>(0x00000001, oModBfr, 1);
	Test(_local_ModBfrTestClear(oModBfr, 1) == 0);
	acpl::Endian::ToBig<acpl::SInt32>(0x00000123, oModBfr, 2);
	Test(_local_ModBfrTestClear(oModBfr, 2) == 0);
	acpl::Endian::ToBig<acpl::SInt32>(0x00012345, oModBfr, 3);
	Test(_local_ModBfrTestClear(oModBfr, 3) == 0);
	acpl::Endian::ToBig<acpl::SInt32>(0x01234567, oModBfr, 4);
	Test(_local_ModBfrTestClear(oModBfr, 4) == 0);
	acpl::Endian::ToBig<acpl::SInt32>(0x01234567, oModBfr, 5);
	Test(_local_ModBfrTestClear(oModBfr, 4) == 0);
	acpl::Endian::ToBig<acpl::SInt64>(acpl::Const::UI64(0x00000000, 0x00000000), oModBfr, 0);
	Test(_local_ModBfrTestClear(oModBfr, 0) == 0);
	acpl::Endian::ToBig<acpl::SInt64>(acpl::Const::UI64(0x00000000, 0x00000001), oModBfr, 1);
	Test(_local_ModBfrTestClear(oModBfr, 1) == 0);
	acpl::Endian::ToBig<acpl::SInt64>(acpl::Const::UI64(0x00000000, 0x00000123), oModBfr, 2);
	Test(_local_ModBfrTestClear(oModBfr, 2) == 0);
	acpl::Endian::ToBig<acpl::SInt64>(acpl::Const::UI64(0x00000000, 0x00012345), oModBfr, 3);
	Test(_local_ModBfrTestClear(oModBfr, 3) == 0);
	acpl::Endian::ToBig<acpl::SInt64>(acpl::Const::UI64(0x00000000, 0x01234567), oModBfr, 4);
	Test(_local_ModBfrTestClear(oModBfr, 4) == 0);
	acpl::Endian::ToBig<acpl::SInt64>(acpl::Const::UI64(0x00000001, 0x23456789), oModBfr, 5);
	Test(_local_ModBfrTestClear(oModBfr, 5) == 0);
	acpl::Endian::ToBig<acpl::SInt64>(acpl::Const::UI64(0x00000123, 0x456789AB), oModBfr, 6);
	Test(_local_ModBfrTestClear(oModBfr, 6) == 0);
	acpl::Endian::ToBig<acpl::SInt64>(acpl::Const::UI64(0x00012345, 0x6789ABCD), oModBfr, 7);
	Test(_local_ModBfrTestClear(oModBfr, 7) == 0);
	acpl::Endian::ToBig<acpl::SInt64>(acpl::Const::UI64(0x01234567, 0x89ABCDEF), oModBfr, 8);
	Test(_local_ModBfrTestClear(oModBfr, 8) == 0);
	acpl::Endian::ToBig<acpl::SInt64>(acpl::Const::UI64(0x01234567, 0x89ABCDEF), oModBfr, 9);
	Test(_local_ModBfrTestClear(oModBfr, 8) == 0);
	
	
	// ToBig (with mask)
	
	_local_ModBfrClearOnly(oModBfr); // to clear the buffer
	acpl::Endian::ToBig<acpl::UInt8 >(0x01, 0x01, oModBfr);
	Test(_local_ModBfrTestOnly(oModBfr, 1) == 0);
	acpl::Endian::ToBig<acpl::UInt16>(0x0123, 0x0023, oModBfr);
	Test(_local_ModBfrTestOnly(oModBfr, 2) == 0);
	acpl::Endian::ToBig<acpl::UInt32>(0x01234567, 0x00004567, oModBfr);
	Test(_local_ModBfrTestOnly(oModBfr, 4) == 0);
	acpl::Endian::ToBig<acpl::UInt64>(acpl::Const::UI64(0x01234567, 0x89ABCDEF), acpl::Const::UI64(0x00000000, 0x89ABCDEF), oModBfr);
	Test(_local_ModBfrTestOnly(oModBfr, 8) == 0);
	acpl::Endian::ToBig<acpl::UInt64>(acpl::Const::UI64(0xFFFFFFFF, 0xFFFFFFFF), acpl::Const::UI64(0x00000000, 0x00000000), oModBfr);
	Test(_local_ModBfrTestOnly(oModBfr, 8) == 0);
	
	_local_ModBfrClearOnly(oModBfr); // to clear the buffer
	acpl::Endian::ToBig<acpl::SInt8 >(0x01, 0x01, oModBfr);
	Test(_local_ModBfrTestOnly(oModBfr, 1) == 0);
	acpl::Endian::ToBig<acpl::SInt16>(0x0123, 0x0023, oModBfr);
	Test(_local_ModBfrTestOnly(oModBfr, 2) == 0);
	acpl::Endian::ToBig<acpl::SInt32>(0x01234567, 0x00004567, oModBfr);
	Test(_local_ModBfrTestOnly(oModBfr, 4) == 0);
	acpl::Endian::ToBig<acpl::SInt64>(acpl::Const::UI64(0x01234567, 0x89ABCDEF), acpl::Const::UI64(0x00000000, 0x89ABCDEF), oModBfr);
	Test(_local_ModBfrTestOnly(oModBfr, 8) == 0);
	acpl::Endian::ToBig<acpl::SInt64>(acpl::Const::UI64(0xFFFFFFFF, 0xFFFFFFFF), acpl::Const::UI64(0x00000000, 0x00000000), oModBfr);
	Test(_local_ModBfrTestOnly(oModBfr, 8) == 0);
	
	
	// ToLittle (normal)
	
	_local_ModBfrClearOnly(oModBfr); // to clear the buffer
	acpl::Endian::ToLittle<acpl::UInt8 >(0x00, oModBfr, 0);
	Test(_local_ModBfrTestClear(oModBfr, 0) == 0);
	acpl::Endian::ToLittle<acpl::UInt8 >(0x01, oModBfr, 1);
	Test(_local_ModBfrTestClear(oModBfr, 1) == 0);
	acpl::Endian::ToLittle<acpl::UInt8 >(0x01, oModBfr, 2);
	Test(_local_ModBfrTestClear(oModBfr, 1) == 0);
	acpl::Endian::ToLittle<acpl::UInt16>(0x0000, oModBfr, 0);
	Test(_local_ModBfrTestClear(oModBfr, 0) == 0);
	acpl::Endian::ToLittle<acpl::UInt16>(0x0001, oModBfr, 1);
	Test(_local_ModBfrTestClear(oModBfr, 1) == 0);
	acpl::Endian::ToLittle<acpl::UInt16>(0x2301, oModBfr, 2);
	Test(_local_ModBfrTestClear(oModBfr, 2) == 0);
	acpl::Endian::ToLittle<acpl::UInt16>(0x2301, oModBfr, 3);
	Test(_local_ModBfrTestClear(oModBfr, 2) == 0);
	acpl::Endian::ToLittle<acpl::UInt32>(0x00000000, oModBfr, 0);
	Test(_local_ModBfrTestClear(oModBfr, 0) == 0);
	acpl::Endian::ToLittle<acpl::UInt32>(0x00000001, oModBfr, 1);
	Test(_local_ModBfrTestClear(oModBfr, 1) == 0);
	acpl::Endian::ToLittle<acpl::UInt32>(0x00002301, oModBfr, 2);
	Test(_local_ModBfrTestClear(oModBfr, 2) == 0);
	acpl::Endian::ToLittle<acpl::UInt32>(0x00452301, oModBfr, 3);
	Test(_local_ModBfrTestClear(oModBfr, 3) == 0);
	acpl::Endian::ToLittle<acpl::UInt32>(0x67452301, oModBfr, 4);
	Test(_local_ModBfrTestClear(oModBfr, 4) == 0);
	acpl::Endian::ToLittle<acpl::UInt32>(0x67452301, oModBfr, 5);
	Test(_local_ModBfrTestClear(oModBfr, 4) == 0);
	acpl::Endian::ToLittle<acpl::UInt64>(acpl::Const::UI64(0x00000000, 0x00000000), oModBfr, 0);
	Test(_local_ModBfrTestClear(oModBfr, 0) == 0);
	acpl::Endian::ToLittle<acpl::UInt64>(acpl::Const::UI64(0x00000000, 0x00000001), oModBfr, 1);
	Test(_local_ModBfrTestClear(oModBfr, 1) == 0);
	acpl::Endian::ToLittle<acpl::UInt64>(acpl::Const::UI64(0x00000000, 0x00002301), oModBfr, 2);
	Test(_local_ModBfrTestClear(oModBfr, 2) == 0);
	acpl::Endian::ToLittle<acpl::UInt64>(acpl::Const::UI64(0x00000000, 0x00452301), oModBfr, 3);
	Test(_local_ModBfrTestClear(oModBfr, 3) == 0);
	acpl::Endian::ToLittle<acpl::UInt64>(acpl::Const::UI64(0x00000000, 0x67452301), oModBfr, 4);
	Test(_local_ModBfrTestClear(oModBfr, 4) == 0);
	acpl::Endian::ToLittle<acpl::UInt64>(acpl::Const::UI64(0x00000089, 0x67452301), oModBfr, 5);
	Test(_local_ModBfrTestClear(oModBfr, 5) == 0);
	acpl::Endian::ToLittle<acpl::UInt64>(acpl::Const::UI64(0x0000AB89, 0x67452301), oModBfr, 6);
	Test(_local_ModBfrTestClear(oModBfr, 6) == 0);
	acpl::Endian::ToLittle<acpl::UInt64>(acpl::Const::UI64(0x00CDAB89, 0x67452301), oModBfr, 7);
	Test(_local_ModBfrTestClear(oModBfr, 7) == 0);
	acpl::Endian::ToLittle<acpl::UInt64>(acpl::Const::UI64(0xEFCDAB89, 0x67452301), oModBfr, 8);
	Test(_local_ModBfrTestClear(oModBfr, 8) == 0);
	acpl::Endian::ToLittle<acpl::UInt64>(acpl::Const::UI64(0xEFCDAB89, 0x67452301), oModBfr, 9);
	Test(_local_ModBfrTestClear(oModBfr, 8) == 0);
	
	_local_ModBfrClearOnly(oModBfr); // to clear the buffer
	acpl::Endian::ToLittle<acpl::SInt8 >(0x00, oModBfr, 0);
	Test(_local_ModBfrTestClear(oModBfr, 0) == 0);
	acpl::Endian::ToLittle<acpl::SInt8 >(0x01, oModBfr, 1);
	Test(_local_ModBfrTestClear(oModBfr, 1) == 0);
	acpl::Endian::ToLittle<acpl::SInt8 >(0x01, oModBfr, 2);
	Test(_local_ModBfrTestClear(oModBfr, 1) == 0);
	acpl::Endian::ToLittle<acpl::SInt16>(0x0000, oModBfr, 0);
	Test(_local_ModBfrTestClear(oModBfr, 0) == 0);
	acpl::Endian::ToLittle<acpl::SInt16>(0x0001, oModBfr, 1);
	Test(_local_ModBfrTestClear(oModBfr, 1) == 0);
	acpl::Endian::ToLittle<acpl::SInt16>(0x2301, oModBfr, 2);
	Test(_local_ModBfrTestClear(oModBfr, 2) == 0);
	acpl::Endian::ToLittle<acpl::SInt16>(0x2301, oModBfr, 3);
	Test(_local_ModBfrTestClear(oModBfr, 2) == 0);
	acpl::Endian::ToLittle<acpl::SInt32>(0x00000000, oModBfr, 0);
	Test(_local_ModBfrTestClear(oModBfr, 0) == 0);
	acpl::Endian::ToLittle<acpl::SInt32>(0x00000001, oModBfr, 1);
	Test(_local_ModBfrTestClear(oModBfr, 1) == 0);
	acpl::Endian::ToLittle<acpl::SInt32>(0x00002301, oModBfr, 2);
	Test(_local_ModBfrTestClear(oModBfr, 2) == 0);
	acpl::Endian::ToLittle<acpl::SInt32>(0x00452301, oModBfr, 3);
	Test(_local_ModBfrTestClear(oModBfr, 3) == 0);
	acpl::Endian::ToLittle<acpl::SInt32>(0x67452301, oModBfr, 4);
	Test(_local_ModBfrTestClear(oModBfr, 4) == 0);
	acpl::Endian::ToLittle<acpl::SInt32>(0x67452301, oModBfr, 5);
	Test(_local_ModBfrTestClear(oModBfr, 4) == 0);
	acpl::Endian::ToLittle<acpl::SInt64>(acpl::Const::UI64(0x00000000, 0x00000000), oModBfr, 0);
	Test(_local_ModBfrTestClear(oModBfr, 0) == 0);
	acpl::Endian::ToLittle<acpl::SInt64>(acpl::Const::UI64(0x00000000, 0x00000001), oModBfr, 1);
	Test(_local_ModBfrTestClear(oModBfr, 1) == 0);
	acpl::Endian::ToLittle<acpl::SInt64>(acpl::Const::UI64(0x00000000, 0x00002301), oModBfr, 2);
	Test(_local_ModBfrTestClear(oModBfr, 2) == 0);
	acpl::Endian::ToLittle<acpl::SInt64>(acpl::Const::UI64(0x00000000, 0x00452301), oModBfr, 3);
	Test(_local_ModBfrTestClear(oModBfr, 3) == 0);
	acpl::Endian::ToLittle<acpl::SInt64>(acpl::Const::UI64(0x00000000, 0x67452301), oModBfr, 4);
	Test(_local_ModBfrTestClear(oModBfr, 4) == 0);
	acpl::Endian::ToLittle<acpl::SInt64>(acpl::Const::UI64(0x00000089, 0x67452301), oModBfr, 5);
	Test(_local_ModBfrTestClear(oModBfr, 5) == 0);
	acpl::Endian::ToLittle<acpl::SInt64>(acpl::Const::UI64(0x0000AB89, 0x67452301), oModBfr, 6);
	Test(_local_ModBfrTestClear(oModBfr, 6) == 0);
	acpl::Endian::ToLittle<acpl::SInt64>(acpl::Const::UI64(0x00CDAB89, 0x67452301), oModBfr, 7);
	Test(_local_ModBfrTestClear(oModBfr, 7) == 0);
	acpl::Endian::ToLittle<acpl::SInt64>(acpl::Const::UI64(0xEFCDAB89, 0x67452301), oModBfr, 8);
	Test(_local_ModBfrTestClear(oModBfr, 8) == 0);
	acpl::Endian::ToLittle<acpl::SInt64>(acpl::Const::UI64(0xEFCDAB89, 0x67452301), oModBfr, 9);
	Test(_local_ModBfrTestClear(oModBfr, 8) == 0);
	
	
	// ToLittle (with mask)
	
	_local_ModBfrClearOnly(oModBfr); // to clear the buffer
	acpl::Endian::ToLittle<acpl::UInt8 >(0x01, 0x01, oModBfr);
	Test(_local_ModBfrTestOnly(oModBfr, 1) == 0);
	acpl::Endian::ToLittle<acpl::UInt16>(0x2301, 0x2300, oModBfr);
	Test(_local_ModBfrTestOnly(oModBfr, 2) == 0);
	acpl::Endian::ToLittle<acpl::UInt32>(0x67452301, 0x67450000, oModBfr);
	Test(_local_ModBfrTestOnly(oModBfr, 4) == 0);
	acpl::Endian::ToLittle<acpl::UInt64>(acpl::Const::UI64(0xEFCDAB89, 0x67452301), acpl::Const::UI64(0xEFCDAB89, 0x00000000), oModBfr);
	Test(_local_ModBfrTestOnly(oModBfr, 8) == 0);
	acpl::Endian::ToLittle<acpl::UInt64>(acpl::Const::UI64(0xFFFFFFFF, 0xFFFFFFFF), acpl::Const::UI64(0x00000000, 0x00000000), oModBfr);
	Test(_local_ModBfrTestOnly(oModBfr, 8) == 0);
	
	_local_ModBfrClearOnly(oModBfr); // to clear the buffer
	acpl::Endian::ToLittle<acpl::SInt8 >(0x01, 0x01, oModBfr);
	Test(_local_ModBfrTestOnly(oModBfr, 1) == 0);
	acpl::Endian::ToLittle<acpl::SInt16>(0x2301, 0x2300, oModBfr);
	Test(_local_ModBfrTestOnly(oModBfr, 2) == 0);
	acpl::Endian::ToLittle<acpl::SInt32>(0x67452301, 0x67450000, oModBfr);
	Test(_local_ModBfrTestOnly(oModBfr, 4) == 0);
	acpl::Endian::ToLittle<acpl::SInt64>(acpl::Const::UI64(0xEFCDAB89, 0x67452301), acpl::Const::UI64(0xEFCDAB89, 0x00000000), oModBfr);
	Test(_local_ModBfrTestOnly(oModBfr, 8) == 0);
	acpl::Endian::ToLittle<acpl::SInt64>(acpl::Const::UI64(0xFFFFFFFF, 0xFFFFFFFF), acpl::Const::UI64(0x00000000, 0x00000000), oModBfr);
	Test(_local_ModBfrTestOnly(oModBfr, 8) == 0);
	
	
	return 0;
}


SectionFuncMain(endian)
{
	if (argc < 1)
	{
		Test(TestEndianness() == 0)
	}
	else
		return PrintUnrecParams(acplDbgSrcLoc, lSectionName, argc, argv);
	
	return 0;
}

SectionFuncParamsBegin(endian)
// SectionFuncParamsAdd("param [param [param ...]]", "description")
SectionFuncParamsEnd(endian)
