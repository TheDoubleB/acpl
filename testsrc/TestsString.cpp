#include "Tests.h"

#include "../src/String.h"
#include "../src/net/Address.h"
#include "../src/Time.h"

// Multi-byte sequences
// 2bs: 0x000007FF = "\xDF\xBF"
// 3bs: 0x0000FFFF = "\xEF\xBF\xBF"
// 4bs: 0x001FFFFF = "\xF7\xBF\xBF\xBF"
// 5bs: 0x03FFFFFF = "\xFB\xBF\xBF\xBF\xBF"
// 6bs: 0x7FFFFFFF = "\xFD\xBF\xBF\xBF\xBF\xBF"


struct _local_TestRefMeasureConvert_ExpResult
{
	acpl::SizeT sStrMaxLen;
	acpl::SizeT sStrOff;
	acpl::SizeT sRefOff;
	acpl::SizeT sEncByteSize;
	const char *sEncStr;
};


static inline void _local_SetByte(void *nPtr, acpl::SizeT nOff, acpl::UInt8 nByte)
{
	static_cast<acpl::UInt8 *>(nPtr)[nOff] = nByte;
}

static bool _local_Memcmp(const void *nPtr1, const void *nPtr2, acpl::SizeT nNumBytes)
{
	while (nNumBytes--)
		if (static_cast<const acpl::UInt8 *>(nPtr1)[nNumBytes] != static_cast<const acpl::UInt8 *>(nPtr2)[nNumBytes])
			return false;
	
	return true;
}

static int _local_TestRefMeasureConvert(const acpl::String::Ref &nRef, const _local_TestRefMeasureConvert_ExpResult *nExpResults, acpl::SizeT nExpResultsCount)
{
	acpl::SizeT oRefOff;
	acpl::SizeT oEncByteSize;
	char oBfr[10];
	
	for (acpl::SizeT i = 0; i < nExpResultsCount; i++)
	{
		const _local_TestRefMeasureConvert_ExpResult *oCurExpRes = nExpResults + i;
		
		nRef.Measure(oCurExpRes->sStrMaxLen, oCurExpRes->sStrOff, oRefOff, oEncByteSize);
		TestFM(oRefOff == oCurExpRes->sRefOff, "i=%lu (%lu != %lu)", i, oRefOff, oCurExpRes->sRefOff);
		TestFM(oEncByteSize == oCurExpRes->sEncByteSize, "i=%lu (%lu != %lu)", i, oEncByteSize, oCurExpRes->sEncByteSize);
		
		TestFM(oEncByteSize < sizeof(oBfr), "i=%lu", i);
		
		oBfr[oEncByteSize] = '\0';
		nRef.Convert(oRefOff, oEncByteSize, oBfr);
		TestFM(StringCompare(oBfr, oCurExpRes->sEncStr) == 0, "i=%lu ('%s' != '%s')", i, oBfr, oCurExpRes->sEncStr);
	}
	
	return 0;
}

template <class tStrType>
static int _local_TestStringData(const typename acpl::Mem::Block<tStrType>::Type &nStrData, const tStrType *nStrCmp)
{
	const tStrType *oCurPos = nStrData.Data();
	
	while (*oCurPos != '\0' && *oCurPos++ == *nStrCmp++) ;
	
	return (*oCurPos != '\0');
}

template <class tType>
static inline bool _local_CmpFloat(tType nVal1, tType nVal2, acpl::UInt64 nManTol) // `nManTol` is used for comparing mantissa of both values with a +/- tolerance
{
	acpl::Float::Parts oParts1, oParts2;
	acpl::Float::GetParts(nVal1, oParts1);
	acpl::Float::GetParts(nVal2, oParts2);
	return (oParts1.sMan >= (oParts2.sMan - nManTol) && oParts1.sMan <= (oParts2.sMan + nManTol));
}

static inline bool _local_SkipFloatExtPrec()
{
	return (acpl::Float::HasExtPrec() == false || acpl::Float::Max<acpl::Float::Largest>() == acpl::Float::Inf<acpl::Float::Largest>());
}

static inline acpl::UInt64 _local_FloatTolSwitch(acpl::UInt64 nMaxTol)
{
	return ((_local_SkipFloatExtPrec() == true) ? nMaxTol : 0);
}


static int TestUtf8Funcs()
{
	PrintFn();
	
	
	
	
	
	return 0;
}

static int TestRef()
{
	PrintFn();
	
	
	acpl::String oEmptyStr;
	acpl::String oNonEmptyStr("abc");
	
	
	// Empty
	{
		acpl::String::Ref oRef(reinterpret_cast<const char *>(NULL));
		Test(oRef.IsNull()  == true);
		Test(oRef.GetPtr()  == NULL);
		Test(oRef.GetType() == acpl::String::Ref::rtUtf8);
	}
	
	{
		acpl::String::Ref oRef(static_cast<const char *>(NULL), false);
		Test(oRef.IsNull()  == true);
		Test(oRef.GetPtr()  == NULL);
		Test(oRef.GetType() == acpl::String::Ref::rtUtf8);
	}
	
	{
		acpl::String::Ref oRef(static_cast<const char *>(NULL), true);
		Test(oRef.IsNull()  == true);
		Test(oRef.GetPtr()  == NULL);
		Test(oRef.GetType() == acpl::String::Ref::rtMbs);
	}
	
	{
		acpl::String::Ref oRef(static_cast<const wchar_t *>(NULL));
		Test(oRef.IsNull()  == true);
		Test(oRef.GetPtr()  == NULL);
		Test(oRef.GetType() == acpl::String::Ref::rtWcs);
	}
	
	{
		acpl::String::Ref oRef(static_cast<const acpl::Unichar *>(NULL));
		Test(oRef.IsNull()  == true);
		Test(oRef.GetPtr()  == NULL);
		Test(oRef.GetType() == acpl::String::Ref::rtUcs);
	}
	
	{
		acpl::String::Ref oRef(static_cast<const acpl::String &>(acpl::String()));
		Test(oRef.IsNull()  == true);
		Test(oRef.GetPtr()  == NULL);
		Test(oRef.GetType() == acpl::String::Ref::rtSelf);
	}
	
	{
		acpl::String::Ref oRef(oEmptyStr);
		Test(oRef.IsNull()  == true);
		Test(oRef.GetPtr()  == NULL);
		Test(oRef.GetType() == acpl::String::Ref::rtSelf);
	}
	
	{
		acpl::String::Ref oRef(static_cast<const acpl::String *>(NULL));
		Test(oRef.IsNull()  == true);
		Test(oRef.GetPtr()  == NULL);
		Test(oRef.GetType() == acpl::String::Ref::rtSelf);
	}
	
	{
		acpl::String::Ref oRef(&oEmptyStr);
		Test(oRef.IsNull()  == true);
		Test(oRef.GetPtr()  == NULL);
		Test(oRef.GetType() == acpl::String::Ref::rtSelf);
	}
	
	
	// Non-empty
	{
		acpl::String::Ref oRef("abc");
		Test(oRef.IsNull()  == false);
		Test(oRef.GetPtr()  != NULL);
		Test(oRef.GetType() == acpl::String::Ref::rtUtf8);
		Test(_local_Memcmp(oRef.GetPtr(), "abc", 4) == true);
	}
	
	{
		acpl::String::Ref oRef("abc", false);
		Test(oRef.IsNull()  == false);
		Test(oRef.GetPtr()  != NULL);
		Test(oRef.GetType() == acpl::String::Ref::rtUtf8);
		Test(_local_Memcmp(oRef.GetPtr(), "abc", 4) == true);
	}
	
	{
		acpl::String::Ref oRef("abc", true);
		Test(oRef.IsNull()  == false);
		Test(oRef.GetPtr()  != NULL);
		Test(oRef.GetType() == acpl::String::Ref::rtMbs);
		Test(_local_Memcmp(oRef.GetPtr(), "abc", 4) == true);
	}
	
	{
		acpl::String::Ref oRef(L"abc");
		Test(oRef.IsNull()  == false);
		Test(oRef.GetPtr()  != NULL);
		Test(oRef.GetType() == acpl::String::Ref::rtWcs);
		Test(_local_Memcmp(oRef.GetPtr(), L"abc", 4 * sizeof(wchar_t)) == true);
	}
	
	{
		const acpl::Unichar oUcs[] = { 'a', 'b', 'c', '\0' };
		acpl::String::Ref oRef(oUcs); // "abc" and a null terminator in unicode (Unichar type)
		Test(oRef.IsNull()  == false);
		Test(oRef.GetPtr()  != NULL);
		Test(oRef.GetType() == acpl::String::Ref::rtUcs);
		Test(_local_Memcmp(oRef.GetPtr(), oUcs, 4 * sizeof(acpl::Unichar)) == true);
	}
	
	{
		acpl::String::Ref oRef(static_cast<const acpl::String &>(acpl::String("abc")));
		Test(oRef.IsNull()  == false);
		Test(oRef.GetPtr()  != NULL);
		Test(oRef.GetType() == acpl::String::Ref::rtSelf);
	}
	
	{
		acpl::String::Ref oRef(oNonEmptyStr);
		Test(oRef.IsNull()  == false);
		Test(oRef.GetPtr()  != NULL);
		Test(oRef.GetType() == acpl::String::Ref::rtSelf);
	}
	
	{
		acpl::String::Ref oRef(&oNonEmptyStr);
		Test(oRef.IsNull()  == false);
		Test(oRef.GetPtr()  != NULL);
		Test(oRef.GetType() == acpl::String::Ref::rtSelf);
	}
	
	
	return 0;
}

static int TestIntRefType()
{
	PrintFn();
	
	
	// SInt8
	{
		acpl::String::IntRef::Type oIrt(static_cast<acpl::SInt8 >(acpl::Const::UI64(0x00000000, 0x00000000)));
		Test(oIrt.GetValue() == acpl::Const::UI64(0x00000000, 0x00000000));
		Test(oIrt.GetMeta()  == 0x01);
	}
	{
		acpl::String::IntRef::Type oIrt(static_cast<acpl::SInt8 >(acpl::Const::UI64(0x00000000, 0x0000007F)));
		Test(oIrt.GetValue() == acpl::Const::UI64(0x00000000, 0x0000007F));
		Test(oIrt.GetMeta()  == 0x01);
	}
	{
		acpl::String::IntRef::Type oIrt(static_cast<acpl::SInt8 >(acpl::Const::UI64(0x00000000, 0x00000080)));
		Test(oIrt.GetValue() == acpl::Const::UI64(0xFFFFFFFF, 0xFFFFFF80));
		Test(oIrt.GetMeta()  == 0x81);
	}
	{
		acpl::String::IntRef::Type oIrt(static_cast<acpl::SInt8 >(acpl::Const::UI64(0x00000000, 0x000000FF)));
		Test(oIrt.GetValue() == acpl::Const::UI64(0xFFFFFFFF, 0xFFFFFFFF));
		Test(oIrt.GetMeta()  == 0x81);
	}
	
	// SInt16
	{
		acpl::String::IntRef::Type oIrt(static_cast<acpl::SInt16>(acpl::Const::UI64(0x00000000, 0x00000000)));
		Test(oIrt.GetValue() == acpl::Const::UI64(0x00000000, 0x00000000));
		Test(oIrt.GetMeta()  == 0x02);
	}
	{
		acpl::String::IntRef::Type oIrt(static_cast<acpl::SInt16>(acpl::Const::UI64(0x00000000, 0x00007FFF)));
		Test(oIrt.GetValue() == acpl::Const::UI64(0x00000000, 0x00007FFF));
		Test(oIrt.GetMeta()  == 0x02);
	}
	{
		acpl::String::IntRef::Type oIrt(static_cast<acpl::SInt16>(acpl::Const::UI64(0x00000000, 0x00008000)));
		Test(oIrt.GetValue() == acpl::Const::UI64(0xFFFFFFFF, 0xFFFF8000));
		Test(oIrt.GetMeta()  == 0x82);
	}
	{
		acpl::String::IntRef::Type oIrt(static_cast<acpl::SInt16>(acpl::Const::UI64(0x00000000, 0x0000FFFF)));
		Test(oIrt.GetValue() == acpl::Const::UI64(0xFFFFFFFF, 0xFFFFFFFF));
		Test(oIrt.GetMeta()  == 0x82);
	}
	
	// SInt32
	{
		acpl::String::IntRef::Type oIrt(static_cast<acpl::SInt32>(acpl::Const::UI64(0x00000000, 0x00000000)));
		Test(oIrt.GetValue() == acpl::Const::UI64(0x00000000, 0x00000000));
		Test(oIrt.GetMeta()  == 0x04);
	}
	{
		acpl::String::IntRef::Type oIrt(static_cast<acpl::SInt32>(acpl::Const::UI64(0x00000000, 0x7FFFFFFF)));
		Test(oIrt.GetValue() == acpl::Const::UI64(0x00000000, 0x7FFFFFFF));
		Test(oIrt.GetMeta()  == 0x04);
	}
	{
		acpl::String::IntRef::Type oIrt(static_cast<acpl::SInt32>(acpl::Const::UI64(0x00000000, 0x80000000)));
		Test(oIrt.GetValue() == acpl::Const::UI64(0xFFFFFFFF, 0x80000000));
		Test(oIrt.GetMeta()  == 0x84);
	}
	{
		acpl::String::IntRef::Type oIrt(static_cast<acpl::SInt32>(acpl::Const::UI64(0x00000000, 0xFFFFFFFF)));
		Test(oIrt.GetValue() == acpl::Const::UI64(0xFFFFFFFF, 0xFFFFFFFF));
		Test(oIrt.GetMeta()  == 0x84);
	}
	
	// SInt64
	{
		acpl::String::IntRef::Type oIrt(static_cast<acpl::SInt64>(acpl::Const::UI64(0x00000000, 0x00000000)));
		Test(oIrt.GetValue() == acpl::Const::UI64(0x00000000, 0x00000000));
		Test(oIrt.GetMeta()  == 0x08);
	}
	{
		acpl::String::IntRef::Type oIrt(static_cast<acpl::SInt64>(acpl::Const::UI64(0x7FFFFFFF, 0xFFFFFFFF)));
		Test(oIrt.GetValue() == acpl::Const::UI64(0x7FFFFFFF, 0xFFFFFFFF));
		Test(oIrt.GetMeta()  == 0x08);
	}
	{
		acpl::String::IntRef::Type oIrt(static_cast<acpl::SInt64>(acpl::Const::UI64(0x80000000, 0x00000000)));
		Test(oIrt.GetValue() == acpl::Const::UI64(0x80000000, 0x00000000));
		Test(oIrt.GetMeta()  == 0x88);
	}
	{
		acpl::String::IntRef::Type oIrt(static_cast<acpl::SInt64>(acpl::Const::UI64(0xFFFFFFFF, 0xFFFFFFFF)));
		Test(oIrt.GetValue() == acpl::Const::UI64(0xFFFFFFFF, 0xFFFFFFFF));
		Test(oIrt.GetMeta()  == 0x88);
	}
	
	// UInt8
	{
		acpl::String::IntRef::Type oIrt(static_cast<acpl::UInt8 >(acpl::Const::UI64(0x00000000, 0x00000000)));
		Test(oIrt.GetValue() == acpl::Const::UI64(0x00000000, 0x00000000));
		Test(oIrt.GetMeta()  == 0x01);
	}
	{
		acpl::String::IntRef::Type oIrt(static_cast<acpl::UInt8 >(acpl::Const::UI64(0x00000000, 0x0000007F)));
		Test(oIrt.GetValue() == acpl::Const::UI64(0x00000000, 0x0000007F));
		Test(oIrt.GetMeta()  == 0x01);
	}
	{
		acpl::String::IntRef::Type oIrt(static_cast<acpl::UInt8 >(acpl::Const::UI64(0x00000000, 0x00000080)));
		Test(oIrt.GetValue() == acpl::Const::UI64(0x00000000, 0x00000080));
		Test(oIrt.GetMeta()  == 0x01);
	}
	{
		acpl::String::IntRef::Type oIrt(static_cast<acpl::UInt8 >(acpl::Const::UI64(0x00000000, 0x000000FF)));
		Test(oIrt.GetValue() == acpl::Const::UI64(0x00000000, 0x000000FF));
		Test(oIrt.GetMeta()  == 0x01);
	}
	
	// UInt16
	{
		acpl::String::IntRef::Type oIrt(static_cast<acpl::UInt16>(acpl::Const::UI64(0x00000000, 0x00000000)));
		Test(oIrt.GetValue() == acpl::Const::UI64(0x00000000, 0x00000000));
		Test(oIrt.GetMeta()  == 0x02);
	}
	{
		acpl::String::IntRef::Type oIrt(static_cast<acpl::UInt16>(acpl::Const::UI64(0x00000000, 0x00007FFF)));
		Test(oIrt.GetValue() == acpl::Const::UI64(0x00000000, 0x00007FFF));
		Test(oIrt.GetMeta()  == 0x02);
	}
	{
		acpl::String::IntRef::Type oIrt(static_cast<acpl::UInt16>(acpl::Const::UI64(0x00000000, 0x00008000)));
		Test(oIrt.GetValue() == acpl::Const::UI64(0x00000000, 0x00008000));
		Test(oIrt.GetMeta()  == 0x02);
	}
	{
		acpl::String::IntRef::Type oIrt(static_cast<acpl::UInt16>(acpl::Const::UI64(0x00000000, 0x0000FFFF)));
		Test(oIrt.GetValue() == acpl::Const::UI64(0x00000000, 0x0000FFFF));
		Test(oIrt.GetMeta()  == 0x02);
	}
	
	// UInt32
	{
		acpl::String::IntRef::Type oIrt(static_cast<acpl::UInt32>(acpl::Const::UI64(0x00000000, 0x00000000)));
		Test(oIrt.GetValue() == acpl::Const::UI64(0x00000000, 0x00000000));
		Test(oIrt.GetMeta()  == 0x04);
	}
	{
		acpl::String::IntRef::Type oIrt(static_cast<acpl::UInt32>(acpl::Const::UI64(0x00000000, 0x7FFFFFFF)));
		Test(oIrt.GetValue() == acpl::Const::UI64(0x00000000, 0x7FFFFFFF));
		Test(oIrt.GetMeta()  == 0x04);
	}
	{
		acpl::String::IntRef::Type oIrt(static_cast<acpl::UInt32>(acpl::Const::UI64(0x00000000, 0x80000000)));
		Test(oIrt.GetValue() == acpl::Const::UI64(0x00000000, 0x80000000));
		Test(oIrt.GetMeta()  == 0x04);
	}
	{
		acpl::String::IntRef::Type oIrt(static_cast<acpl::UInt32>(acpl::Const::UI64(0x00000000, 0xFFFFFFFF)));
		Test(oIrt.GetValue() == acpl::Const::UI64(0x00000000, 0xFFFFFFFF));
		Test(oIrt.GetMeta()  == 0x04);
	}
	
	// UInt64
	{
		acpl::String::IntRef::Type oIrt(static_cast<acpl::UInt64>(acpl::Const::UI64(0x00000000, 0x00000000)));
		Test(oIrt.GetValue() == acpl::Const::UI64(0x00000000, 0x00000000));
		Test(oIrt.GetMeta()  == 0x08);
	}
	{
		acpl::String::IntRef::Type oIrt(static_cast<acpl::UInt64>(acpl::Const::UI64(0x7FFFFFFF, 0xFFFFFFFF)));
		Test(oIrt.GetValue() == acpl::Const::UI64(0x7FFFFFFF, 0xFFFFFFFF));
		Test(oIrt.GetMeta()  == 0x08);
	}
	{
		acpl::String::IntRef::Type oIrt(static_cast<acpl::UInt64>(acpl::Const::UI64(0x80000000, 0x00000000)));
		Test(oIrt.GetValue() == acpl::Const::UI64(0x80000000, 0x00000000));
		Test(oIrt.GetMeta()  == 0x08);
	}
	{
		acpl::String::IntRef::Type oIrt(static_cast<acpl::UInt64>(acpl::Const::UI64(0xFFFFFFFF, 0xFFFFFFFF)));
		Test(oIrt.GetValue() == acpl::Const::UI64(0xFFFFFFFF, 0xFFFFFFFF));
		Test(oIrt.GetMeta()  == 0x08);
	}
	
	// SizeT
	{
		acpl::String::IntRef::Type oIrt(static_cast<acpl::SizeT >(0));
		Test(oIrt.GetValue() == static_cast<acpl::SizeT >(0));
		
		     if (sizeof(acpl::SizeT ) == 8) Test(oIrt.GetMeta()  == 0x08)
		else if (sizeof(acpl::SizeT ) == 4) Test(oIrt.GetMeta()  == 0x04)
		else                                Test(false)
	}
	{
		acpl::String::IntRef::Type oIrt(static_cast<acpl::SizeT >(-1));
		Test(oIrt.GetValue() == static_cast<acpl::SizeT >(-1));
		
		     if (sizeof(acpl::SizeT ) == 8) Test(oIrt.GetMeta()  == 0x08)
		else if (sizeof(acpl::SizeT ) == 4) Test(oIrt.GetMeta()  == 0x04)
		else                                Test(false)
	}
	
	// SSizeT
	{
		acpl::String::IntRef::Type oIrt(static_cast<acpl::SSizeT>(0));
		Test(oIrt.GetValue() == static_cast<acpl::UInt64>(static_cast<acpl::SSizeT>(0)));
		
		     if (sizeof(acpl::SSizeT) == 8) Test(oIrt.GetMeta()  == 0x08)
		else if (sizeof(acpl::SSizeT) == 4) Test(oIrt.GetMeta()  == 0x04)
		else                                Test(false)
	}
	{
		acpl::String::IntRef::Type oIrt(static_cast<acpl::SSizeT>(-1));
		Test(oIrt.GetValue() == static_cast<acpl::UInt64>(static_cast<acpl::SSizeT>(-1)));
		
		     if (sizeof(acpl::SSizeT) == 8) Test(oIrt.GetMeta()  == 0x88)
		else if (sizeof(acpl::SSizeT) == 4) Test(oIrt.GetMeta()  == 0x84)
		else                                Test(false)
	}
	
	// OffT
	{
		acpl::String::IntRef::Type oIrt(static_cast<acpl::OffT  >(0));
		Test(oIrt.GetValue() == static_cast<acpl::UInt64>(static_cast<acpl::OffT  >(0)));
		
		     if (sizeof(acpl::OffT  ) == 8) Test(oIrt.GetMeta()  == 0x08)
		else                                Test(false)
	}
	{
		acpl::String::IntRef::Type oIrt(static_cast<acpl::OffT  >(-1));
		Test(oIrt.GetValue() == static_cast<acpl::UInt64>(static_cast<acpl::OffT  >(-1)));
		
		     if (sizeof(acpl::OffT  ) == 8) Test(oIrt.GetMeta()  == 0x88)
		else                                Test(false)
	}
	
	// Float32
	{
		acpl::String::IntRef::Type oIrt(static_cast<acpl::Float32>(0.0));
		Test(oIrt.GetValue() == acpl::Const::UI64(0x00000000, 0x00000000));
		Test(oIrt.GetMeta()  == 0x04);
	}
	{
		acpl::String::IntRef::Type oIrt(static_cast<acpl::Float32>(1.0));
		Test(oIrt.GetValue() == acpl::Const::UI64(0x00000000, 0x00000001));
		Test(oIrt.GetMeta()  == 0x04);
	}
	{
		acpl::String::IntRef::Type oIrt(static_cast<acpl::Float32>(-1.0));
		Test(oIrt.GetValue() == acpl::Const::UI64(0xFFFFFFFF, 0xFFFFFFFF)); // because of the cast (the processed value will still be only lower 4 bytes)
		Test(oIrt.GetMeta()  == 0x84);
	}
	
	// Float64
	{
		acpl::String::IntRef::Type oIrt(static_cast<acpl::Float64>(0.0));
		Test(oIrt.GetValue() == acpl::Const::UI64(0x00000000, 0x00000000));
		Test(oIrt.GetMeta()  == 0x08);
	}
	{
		acpl::String::IntRef::Type oIrt(static_cast<acpl::Float64>(1.0));
		Test(oIrt.GetValue() == acpl::Const::UI64(0x00000000, 0x00000001));
		Test(oIrt.GetMeta()  == 0x08);
	}
	{
		acpl::String::IntRef::Type oIrt(static_cast<acpl::Float64>(-1.0));
		Test(oIrt.GetValue() == acpl::Const::UI64(0xFFFFFFFF, 0xFFFFFFFF));
		Test(oIrt.GetMeta()  == 0x88);
	}
	
	
	return 0;
}

static int TestIntRef()
{
	PrintFn();
	
	
	acpl::UInt64 oHexVal = acpl::Const::UI64(0x01234567, 0x89ABCDEF);
	
	
	// 
	// Basic (same number recasted)
	// 
	
	{
		acpl::String::IntRef oRef(static_cast<acpl::SInt8 >(oHexVal), 16, true, 16, '0');
		Test(oRef.IsNull()  == false);
		Test(oRef.GetPtr()  != NULL);
		Test(oRef.GetType() == acpl::String::Ref::rtSelf);
		Test(_local_Memcmp(oRef.GetPtr(), "00000000000000EF", 17) == true);
	}
	
	{
		acpl::String::IntRef oRef(static_cast<acpl::SInt16>(oHexVal), 16, true, 16, '0');
		Test(oRef.IsNull()  == false);
		Test(oRef.GetPtr()  != NULL);
		Test(oRef.GetType() == acpl::String::Ref::rtSelf);
		Test(_local_Memcmp(oRef.GetPtr(), "000000000000CDEF", 17) == true);
	}
	
	{
		acpl::String::IntRef oRef(static_cast<acpl::SInt32>(oHexVal), 16, true, 16, '0');
		Test(oRef.IsNull()  == false);
		Test(oRef.GetPtr()  != NULL);
		Test(oRef.GetType() == acpl::String::Ref::rtSelf);
		Test(_local_Memcmp(oRef.GetPtr(), "0000000089ABCDEF", 17) == true);
	}
	
	{
		acpl::String::IntRef oRef(static_cast<acpl::SInt64>(oHexVal), 16, true, 16, '0');
		Test(oRef.IsNull()  == false);
		Test(oRef.GetPtr()  != NULL);
		Test(oRef.GetType() == acpl::String::Ref::rtSelf);
		Test(_local_Memcmp(oRef.GetPtr(), "0123456789ABCDEF", 17) == true);
	}
	
	{
		acpl::String::IntRef oRef(static_cast<acpl::UInt8 >(oHexVal), 16, true, 16, '0');
		Test(oRef.IsNull()  == false);
		Test(oRef.GetPtr()  != NULL);
		Test(oRef.GetType() == acpl::String::Ref::rtSelf);
		Test(_local_Memcmp(oRef.GetPtr(), "00000000000000EF", 17) == true);
	}
	
	{
		acpl::String::IntRef oRef(static_cast<acpl::UInt16>(oHexVal), 16, true, 16, '0');
		Test(oRef.IsNull()  == false);
		Test(oRef.GetPtr()  != NULL);
		Test(oRef.GetType() == acpl::String::Ref::rtSelf);
		Test(_local_Memcmp(oRef.GetPtr(), "000000000000CDEF", 17) == true);
	}
	
	{
		acpl::String::IntRef oRef(static_cast<acpl::UInt32>(oHexVal), 16, true, 16, '0');
		Test(oRef.IsNull()  == false);
		Test(oRef.GetPtr()  != NULL);
		Test(oRef.GetType() == acpl::String::Ref::rtSelf);
		Test(_local_Memcmp(oRef.GetPtr(), "0000000089ABCDEF", 17) == true);
	}
	
	{
		acpl::String::IntRef oRef(static_cast<acpl::UInt64>(oHexVal), 16, true, 16, '0');
		Test(oRef.IsNull()  == false);
		Test(oRef.GetPtr()  != NULL);
		Test(oRef.GetType() == acpl::String::Ref::rtSelf);
		Test(_local_Memcmp(oRef.GetPtr(), "0123456789ABCDEF", 17) == true);
	}
	
	{
		acpl::String::IntRef oRef(static_cast<acpl::SizeT >(oHexVal), 16, true, 16, '0');
		Test(oRef.IsNull()  == false);
		Test(oRef.GetPtr()  != NULL);
		Test(oRef.GetType() == acpl::String::Ref::rtSelf);
		
		
		     if (sizeof(acpl::SizeT ) == 8) Test(_local_Memcmp(oRef.GetPtr(), "0123456789ABCDEF", 17) == true)
		else if (sizeof(acpl::SizeT ) == 4) Test(_local_Memcmp(oRef.GetPtr(), "0000000089ABCDEF", 17) == true)
		else                                Test(false)
	}
	
	{
		acpl::String::IntRef oRef(static_cast<acpl::SSizeT>(oHexVal), 16, true, 16, '0');
		Test(oRef.IsNull()  == false);
		Test(oRef.GetPtr()  != NULL);
		Test(oRef.GetType() == acpl::String::Ref::rtSelf);
		
		
		     if (sizeof(acpl::SSizeT) == 8) Test(_local_Memcmp(oRef.GetPtr(), "0123456789ABCDEF", 17) == true)
		else if (sizeof(acpl::SSizeT) == 4) Test(_local_Memcmp(oRef.GetPtr(), "0000000089ABCDEF", 17) == true)
		else                                Test(false)
	}
	
	{
		acpl::String::IntRef oRef(static_cast<acpl::OffT  >(oHexVal), 16, true, 16, '0');
		Test(oRef.IsNull()  == false);
		Test(oRef.GetPtr()  != NULL);
		Test(oRef.GetType() == acpl::String::Ref::rtSelf);
		
		
		     if (sizeof(acpl::OffT  ) == 8) Test(_local_Memcmp(oRef.GetPtr(), "0123456789ABCDEF", 17) == true)
		else                                Test(false)
	}
	
	
	// 
	// Limits (decimal)
	// 
	
	// acpl::SInt8
	{
		acpl::String::IntRef oRef(static_cast<acpl::SInt8 >(acpl::Const::UI64(0x00000000, 0x00000000)), 10, true);
		Test(oRef.GetPtr()  != NULL);
		Test(_local_Memcmp(oRef.GetPtr(), "0", 2) == true);
	}
	{
		acpl::String::IntRef oRef(static_cast<acpl::SInt8 >(acpl::Const::UI64(0x00000000, 0x0000007F)), 10, true);
		Test(oRef.GetPtr()  != NULL);
		Test(_local_Memcmp(oRef.GetPtr(), "127", 4) == true);
	}
	{
		acpl::String::IntRef oRef(static_cast<acpl::SInt8 >(acpl::Const::UI64(0x00000000, 0x00000080)), 10, true);
		Test(oRef.GetPtr()  != NULL);
		Test(_local_Memcmp(oRef.GetPtr(), "-128", 5) == true);
	}
	{
		acpl::String::IntRef oRef(static_cast<acpl::SInt8 >(acpl::Const::UI64(0x00000000, 0x000000FF)), 10, true);
		Test(oRef.GetPtr()  != NULL);
		Test(_local_Memcmp(oRef.GetPtr(), "-1", 3) == true);
	}
	
	// acpl::SInt16
	{
		acpl::String::IntRef oRef(static_cast<acpl::SInt16>(acpl::Const::UI64(0x00000000, 0x00000000)), 10, true);
		Test(oRef.GetPtr()  != NULL);
		Test(_local_Memcmp(oRef.GetPtr(), "0", 2) == true);
	}
	{
		acpl::String::IntRef oRef(static_cast<acpl::SInt16>(acpl::Const::UI64(0x00000000, 0x00007FFF)), 10, true);
		Test(oRef.GetPtr()  != NULL);
		Test(_local_Memcmp(oRef.GetPtr(), "32767", 6) == true);
	}
	{
		acpl::String::IntRef oRef(static_cast<acpl::SInt16>(acpl::Const::UI64(0x00000000, 0x00008000)), 10, true);
		Test(oRef.GetPtr()  != NULL);
		Test(_local_Memcmp(oRef.GetPtr(), "-32768", 7) == true);
	}
	{
		acpl::String::IntRef oRef(static_cast<acpl::SInt16>(acpl::Const::UI64(0x00000000, 0x0000FFFF)), 10, true);
		Test(oRef.GetPtr()  != NULL);
		Test(_local_Memcmp(oRef.GetPtr(), "-1", 3) == true);
	}
	
	// acpl::SInt32
	{
		acpl::String::IntRef oRef(static_cast<acpl::SInt32>(acpl::Const::UI64(0x00000000, 0x00000000)), 10, true);
		Test(oRef.GetPtr()  != NULL);
		Test(_local_Memcmp(oRef.GetPtr(), "0", 2) == true);
	}
	{
		acpl::String::IntRef oRef(static_cast<acpl::SInt32>(acpl::Const::UI64(0x00000000, 0x7FFFFFFF)), 10, true);
		Test(oRef.GetPtr()  != NULL);
		Test(_local_Memcmp(oRef.GetPtr(), "2147483647", 11) == true);
	}
	{
		acpl::String::IntRef oRef(static_cast<acpl::SInt32>(acpl::Const::UI64(0x00000000, 0x80000000)), 10, true);
		Test(oRef.GetPtr()  != NULL);
		Test(_local_Memcmp(oRef.GetPtr(), "-2147483648", 12) == true);
	}
	{
		acpl::String::IntRef oRef(static_cast<acpl::SInt32>(acpl::Const::UI64(0x00000000, 0xFFFFFFFF)), 10, true);
		Test(oRef.GetPtr()  != NULL);
		Test(_local_Memcmp(oRef.GetPtr(), "-1", 3) == true);
	}
	
	// acpl::SInt64
	{
		acpl::String::IntRef oRef(static_cast<acpl::SInt64>(acpl::Const::UI64(0x00000000, 0x00000000)), 10, true);
		Test(oRef.GetPtr()  != NULL);
		Test(_local_Memcmp(oRef.GetPtr(), "0", 2) == true);
	}
	{
		acpl::String::IntRef oRef(static_cast<acpl::SInt64>(acpl::Const::UI64(0x7FFFFFFF, 0xFFFFFFFF)), 10, true);
		Test(oRef.GetPtr()  != NULL);
		Test(_local_Memcmp(oRef.GetPtr(), "9223372036854775807", 20) == true);
	}
	{
		acpl::String::IntRef oRef(static_cast<acpl::SInt64>(acpl::Const::UI64(0x80000000, 0x00000000)), 10, true);
		Test(oRef.GetPtr()  != NULL);
		Test(_local_Memcmp(oRef.GetPtr(), "-9223372036854775808", 21) == true);
	}
	{
		acpl::String::IntRef oRef(static_cast<acpl::SInt64>(acpl::Const::UI64(0xFFFFFFFF, 0xFFFFFFFF)), 10, true);
		Test(oRef.GetPtr()  != NULL);
		Test(_local_Memcmp(oRef.GetPtr(), "-1", 3) == true);
	}
	
	// acpl::UInt8
	{
		acpl::String::IntRef oRef(static_cast<acpl::UInt8 >(acpl::Const::UI64(0x00000000, 0x00000000)), 10, true);
		Test(oRef.GetPtr()  != NULL);
		Test(_local_Memcmp(oRef.GetPtr(), "0", 2) == true);
	}
	{
		acpl::String::IntRef oRef(static_cast<acpl::UInt8 >(acpl::Const::UI64(0x00000000, 0x0000007F)), 10, true);
		Test(oRef.GetPtr()  != NULL);
		Test(_local_Memcmp(oRef.GetPtr(), "127", 4) == true);
	}
	{
		acpl::String::IntRef oRef(static_cast<acpl::UInt8 >(acpl::Const::UI64(0x00000000, 0x00000080)), 10, true);
		Test(oRef.GetPtr()  != NULL);
		Test(_local_Memcmp(oRef.GetPtr(), "128", 4) == true);
	}
	{
		acpl::String::IntRef oRef(static_cast<acpl::UInt8 >(acpl::Const::UI64(0x00000000, 0x000000FF)), 10, true);
		Test(oRef.GetPtr()  != NULL);
		Test(_local_Memcmp(oRef.GetPtr(), "255", 4) == true);
	}
	
	// acpl::UInt16
	{
		acpl::String::IntRef oRef(static_cast<acpl::UInt16>(acpl::Const::UI64(0x00000000, 0x00000000)), 10, true);
		Test(oRef.GetPtr()  != NULL);
		Test(_local_Memcmp(oRef.GetPtr(), "0", 2) == true);
	}
	{
		acpl::String::IntRef oRef(static_cast<acpl::UInt16>(acpl::Const::UI64(0x00000000, 0x00007FFF)), 10, true);
		Test(oRef.GetPtr()  != NULL);
		Test(_local_Memcmp(oRef.GetPtr(), "32767", 6) == true);
	}
	{
		acpl::String::IntRef oRef(static_cast<acpl::UInt16>(acpl::Const::UI64(0x00000000, 0x00008000)), 10, true);
		Test(oRef.GetPtr()  != NULL);
		Test(_local_Memcmp(oRef.GetPtr(), "32768", 6) == true);
	}
	{
		acpl::String::IntRef oRef(static_cast<acpl::UInt16>(acpl::Const::UI64(0x00000000, 0x0000FFFF)), 10, true);
		Test(oRef.GetPtr()  != NULL);
		Test(_local_Memcmp(oRef.GetPtr(), "65535", 6) == true);
	}
	
	// acpl::UInt32
	{
		acpl::String::IntRef oRef(static_cast<acpl::UInt32>(acpl::Const::UI64(0x00000000, 0x00000000)), 10, true);
		Test(oRef.GetPtr()  != NULL);
		Test(_local_Memcmp(oRef.GetPtr(), "0", 2) == true);
	}
	{
		acpl::String::IntRef oRef(static_cast<acpl::UInt32>(acpl::Const::UI64(0x00000000, 0x7FFFFFFF)), 10, true);
		Test(oRef.GetPtr()  != NULL);
		Test(_local_Memcmp(oRef.GetPtr(), "2147483647", 11) == true);
	}
	{
		acpl::String::IntRef oRef(static_cast<acpl::UInt32>(acpl::Const::UI64(0x00000000, 0x80000000)), 10, true);
		Test(oRef.GetPtr()  != NULL);
		Test(_local_Memcmp(oRef.GetPtr(), "2147483648", 11) == true);
	}
	{
		acpl::String::IntRef oRef(static_cast<acpl::UInt32>(acpl::Const::UI64(0x00000000, 0xFFFFFFFF)), 10, true);
		Test(oRef.GetPtr()  != NULL);
		Test(_local_Memcmp(oRef.GetPtr(), "4294967295", 11) == true);
	}
	
	// acpl::UInt64
	{
		acpl::String::IntRef oRef(static_cast<acpl::UInt64>(acpl::Const::UI64(0x00000000, 0x00000000)), 10, true);
		Test(oRef.GetPtr()  != NULL);
		Test(_local_Memcmp(oRef.GetPtr(), "0", 2) == true);
	}
	{
		acpl::String::IntRef oRef(static_cast<acpl::UInt64>(acpl::Const::UI64(0x7FFFFFFF, 0xFFFFFFFF)), 10, true);
		Test(oRef.GetPtr()  != NULL);
		Test(_local_Memcmp(oRef.GetPtr(), "9223372036854775807", 20) == true);
	}
	{
		acpl::String::IntRef oRef(static_cast<acpl::UInt64>(acpl::Const::UI64(0x80000000, 0x00000000)), 10, true);
		Test(oRef.GetPtr()  != NULL);
		Test(_local_Memcmp(oRef.GetPtr(), "9223372036854775808", 20) == true);
	}
	{
		acpl::String::IntRef oRef(static_cast<acpl::UInt64>(acpl::Const::UI64(0xFFFFFFFF, 0xFFFFFFFF)), 10, true);
		Test(oRef.GetPtr()  != NULL);
		Test(_local_Memcmp(oRef.GetPtr(), "18446744073709551615", 21) == true);
	}
	
	
	// 
	// Upper/Lower case
	// 
	
	{
		acpl::String::IntRef oRef(static_cast<acpl::UInt64>(oHexVal), 16, true);
		Test(oRef.GetPtr()  != NULL);
		Test(_local_Memcmp(oRef.GetPtr(), "123456789ABCDEF", 16) == true);
	}
	{
		acpl::String::IntRef oRef(static_cast<acpl::UInt64>(oHexVal), 16, false);
		Test(oRef.GetPtr()  != NULL);
		Test(_local_Memcmp(oRef.GetPtr(), "123456789abcdef", 16) == true);
	}
	
	
	// 
	// Different bases
	// 
	
	{
		acpl::String::IntRef oRef(static_cast<acpl::UInt64>(oHexVal), 2, true);
		Test(oRef.GetPtr()  != NULL);
		Test(_local_Memcmp(oRef.GetPtr(), "100100011010001010110011110001001101010111100110111101111", 58) == true);
	}
	{
		acpl::String::IntRef oRef(static_cast<acpl::UInt64>(oHexVal), 4, true);
		Test(oRef.GetPtr()  != NULL);
		Test(_local_Memcmp(oRef.GetPtr(), "10203101112132021222330313233", 30) == true);
	}
	{
		acpl::String::IntRef oRef(static_cast<acpl::UInt64>(oHexVal), 8, true);
		Test(oRef.GetPtr()  != NULL);
		Test(_local_Memcmp(oRef.GetPtr(), "4432126361152746757", 20) == true);
	}
	{
		acpl::String::IntRef oRef(static_cast<acpl::UInt64>(oHexVal), 16, true);
		Test(oRef.GetPtr()  != NULL);
		Test(_local_Memcmp(oRef.GetPtr(), "123456789ABCDEF", 16) == true);
	}
	{
		acpl::String::IntRef oRef(static_cast<acpl::UInt64>(oHexVal), 32, true);
		Test(oRef.GetPtr()  != NULL);
		Test(_local_Memcmp(oRef.GetPtr(), "28Q5CU4QNJFF", 13) == true);
	}
	{
		acpl::String::IntRef oRef(static_cast<acpl::UInt64>(oHexVal), 36, true);
		Test(oRef.GetPtr()  != NULL);
		Test(_local_Memcmp(oRef.GetPtr(), "MF9G063V08F", 12) == true);
	}
	
	// Invalid bases
	{
		acpl::String::IntRef oRef(static_cast<acpl::UInt64>(oHexVal), 0, true);
		Test(oRef.GetPtr()  != NULL);
		Test(_local_Memcmp(oRef.GetPtr(), "(inv-base)", 11) == true);
	}
	{
		acpl::String::IntRef oRef(static_cast<acpl::UInt64>(oHexVal), 1, true);
		Test(oRef.GetPtr()  != NULL);
		Test(_local_Memcmp(oRef.GetPtr(), "(inv-base)", 11) == true);
	}
	{
		acpl::String::IntRef oRef(static_cast<acpl::UInt64>(oHexVal), 37, true);
		Test(oRef.GetPtr()  != NULL);
		Test(_local_Memcmp(oRef.GetPtr(), "(inv-base)", 11) == true);
	}
	
	
	// 
	// Padding
	// 
	
	{
		acpl::String::IntRef oRef(static_cast<acpl::SInt8>(-1), 2, true, 16, '0');
		Test(oRef.GetPtr()  != NULL);
		Test(_local_Memcmp(oRef.GetPtr(), "0000000011111111", 17) == true);
	}
	{
		acpl::String::IntRef oRef(static_cast<acpl::SInt8>(-1), 2, true, 16, ' ');
		Test(oRef.GetPtr()  != NULL);
		Test(_local_Memcmp(oRef.GetPtr(), "        11111111", 17) == true);
	}
	{
		acpl::String::IntRef oRef(static_cast<acpl::SInt8>(-1), 2, true, 64, ' '); // padding on limit
		Test(oRef.GetPtr()  != NULL);
		Test(_local_Memcmp(oRef.GetPtr(), "                                                        11111111", 65) == true);
	}
	{
		acpl::String::IntRef oRef(static_cast<acpl::SInt8>(-1), 2, true, 65, ' '); // padding over limit
		Test(oRef.GetPtr()  != NULL);
		Test(_local_Memcmp(oRef.GetPtr(), "                                                        11111111", 65) == true);
	}
	{
		acpl::String::IntRef oRef(static_cast<acpl::SInt8>(-1), 10, true, 5, ' ');
		Test(oRef.GetPtr()  != NULL);
		Test(_local_Memcmp(oRef.GetPtr(), "   -1", 6) == true);
	}
	{
		acpl::String::IntRef oRef(static_cast<acpl::SInt8>(-1), 10, true, 5, '0');
		Test(oRef.GetPtr()  != NULL);
		Test(_local_Memcmp(oRef.GetPtr(), "-0001", 6) == true);
	}
	{
		acpl::String::IntRef oRef(static_cast<acpl::SInt8>(1), 10, true, 5, ' ');
		Test(oRef.GetPtr()  != NULL);
		Test(_local_Memcmp(oRef.GetPtr(), "    1", 6) == true);
	}
	{
		acpl::String::IntRef oRef(static_cast<acpl::SInt8>(1), 10, true, 5, '0');
		Test(oRef.GetPtr()  != NULL);
		Test(_local_Memcmp(oRef.GetPtr(), "00001", 6) == true);
	}
	{
		acpl::String::IntRef oRef(static_cast<acpl::SInt8>(1), 10, true, 5, 0x7F);
		Test(oRef.GetPtr()  != NULL);
		Test(_local_Memcmp(oRef.GetPtr(), "    1", 6) == true);
	}
	{
		acpl::String::IntRef oRef(static_cast<acpl::SInt8>(1), 10, true, 5, 0x1F);
		Test(oRef.GetPtr()  != NULL);
		Test(_local_Memcmp(oRef.GetPtr(), "    1", 6) == true);
	}
	
	
	// 
	// Floating-point types
	// NOTE: Due to the value approximation of floating-point types, the limits
	//       cannot be tested
	// 
	
	// acpl::Float32
	{
		acpl::String::IntRef oRef(static_cast<acpl::Float32>(0.6789), 10);
		Test(oRef.GetPtr()  != NULL);
		Test(_local_Memcmp(oRef.GetPtr(), "0", 2) == true);
	}
	{
		acpl::String::IntRef oRef(static_cast<acpl::Float32>(12345.6789), 10);
		Test(oRef.GetPtr()  != NULL);
		Test(_local_Memcmp(oRef.GetPtr(), "12345", 6) == true);
	}
	{
		acpl::String::IntRef oRef(static_cast<acpl::Float32>(-12345.6789), 10);
		Test(oRef.GetPtr()  != NULL);
		Test(_local_Memcmp(oRef.GetPtr(), "-12345", 7) == true);
	}
	{
		acpl::String::IntRef oRef(static_cast<acpl::Float32>(12345.6789), 16);
		Test(oRef.GetPtr()  != NULL);
		Test(_local_Memcmp(oRef.GetPtr(), "3039", 5) == true);
	}
	{
		acpl::String::IntRef oRef(static_cast<acpl::Float32>(-1.6789), 16);
		Test(oRef.GetPtr()  != NULL);
		Test(_local_Memcmp(oRef.GetPtr(), "ffffffff", 9) == true);
	}
	{
		acpl::String::IntRef oRef(static_cast<acpl::Float32>(-12345.6789), 16);
		Test(oRef.GetPtr()  != NULL);
		Test(_local_Memcmp(oRef.GetPtr(), "ffffcfc7", 9) == true);
	}
	
	// acpl::Float64
	{
		acpl::String::IntRef oRef(static_cast<acpl::Float64>(0.6789), 10);
		Test(oRef.GetPtr()  != NULL);
		Test(_local_Memcmp(oRef.GetPtr(), "0", 2) == true);
	}
	{
		acpl::String::IntRef oRef(static_cast<acpl::Float64>(12345.6789), 10);
		Test(oRef.GetPtr()  != NULL);
		Test(_local_Memcmp(oRef.GetPtr(), "12345", 6) == true);
	}
	{
		acpl::String::IntRef oRef(static_cast<acpl::Float64>(-12345.6789), 10);
		Test(oRef.GetPtr()  != NULL);
		Test(_local_Memcmp(oRef.GetPtr(), "-12345", 7) == true);
	}
	{
		acpl::String::IntRef oRef(static_cast<acpl::Float64>(12345.6789), 16);
		Test(oRef.GetPtr()  != NULL);
		Test(_local_Memcmp(oRef.GetPtr(), "3039", 5) == true);
	}
	{
		acpl::String::IntRef oRef(static_cast<acpl::Float64>(-1.6789), 16);
		Test(oRef.GetPtr()  != NULL);
		Test(_local_Memcmp(oRef.GetPtr(), "ffffffffffffffff", 17) == true);
	}
	{
		acpl::String::IntRef oRef(static_cast<acpl::Float64>(-12345.6789), 16);
		Test(oRef.GetPtr()  != NULL);
		Test(_local_Memcmp(oRef.GetPtr(), "ffffffffffffcfc7", 17) == true);
	}
	
	
	return 0;
}

static int TestUnicharRef()
{
	PrintFn();
	
	
	// 1-byte
	
	{
		acpl::String::UnicharRef oRef(0x0);
		Test(oRef.IsNull()  == false);
		Test(oRef.GetPtr()  != NULL);
		Test(oRef.GetType() == acpl::String::Ref::rtSelf);
		Test(_local_Memcmp(oRef.GetPtr(), "U+0000", 7) == true);
	}
	{
		acpl::String::UnicharRef oRef(0x0, 3);
		Test(oRef.IsNull()  == false);
		Test(oRef.GetPtr()  != NULL);
		Test(oRef.GetType() == acpl::String::Ref::rtSelf);
		Test(_local_Memcmp(oRef.GetPtr(), "U+0000", 7) == true);
	}
	{
		acpl::String::UnicharRef oRef(0x0, 5);
		Test(oRef.IsNull()  == false);
		Test(oRef.GetPtr()  != NULL);
		Test(oRef.GetType() == acpl::String::Ref::rtSelf);
		Test(_local_Memcmp(oRef.GetPtr(), "U+00000", 8) == true);
	}
	{
		acpl::String::UnicharRef oRef(0x0, 8);
		Test(oRef.IsNull()  == false);
		Test(oRef.GetPtr()  != NULL);
		Test(oRef.GetType() == acpl::String::Ref::rtSelf);
		Test(_local_Memcmp(oRef.GetPtr(), "U+00000000", 11) == true);
	}
	{
		acpl::String::UnicharRef oRef(0x0, 9);
		Test(oRef.IsNull()  == false);
		Test(oRef.GetPtr()  != NULL);
		Test(oRef.GetType() == acpl::String::Ref::rtSelf);
		Test(_local_Memcmp(oRef.GetPtr(), "U+00000000", 11) == true);
	}
	
	{
		acpl::String::UnicharRef oRef(0xAB);
		Test(oRef.IsNull()  == false);
		Test(oRef.GetPtr()  != NULL);
		Test(oRef.GetType() == acpl::String::Ref::rtSelf);
		Test(_local_Memcmp(oRef.GetPtr(), "U+00AB", 7) == true);
	}
	{
		acpl::String::UnicharRef oRef(0xAB, 3);
		Test(oRef.IsNull()  == false);
		Test(oRef.GetPtr()  != NULL);
		Test(oRef.GetType() == acpl::String::Ref::rtSelf);
		Test(_local_Memcmp(oRef.GetPtr(), "U+00AB", 7) == true);
	}
	{
		acpl::String::UnicharRef oRef(0xAB, 5);
		Test(oRef.IsNull()  == false);
		Test(oRef.GetPtr()  != NULL);
		Test(oRef.GetType() == acpl::String::Ref::rtSelf);
		Test(_local_Memcmp(oRef.GetPtr(), "U+000AB", 8) == true);
	}
	{
		acpl::String::UnicharRef oRef(0xAB, 8);
		Test(oRef.IsNull()  == false);
		Test(oRef.GetPtr()  != NULL);
		Test(oRef.GetType() == acpl::String::Ref::rtSelf);
		Test(_local_Memcmp(oRef.GetPtr(), "U+000000AB", 11) == true);
	}
	{
		acpl::String::UnicharRef oRef(0xAB, 9);
		Test(oRef.IsNull()  == false);
		Test(oRef.GetPtr()  != NULL);
		Test(oRef.GetType() == acpl::String::Ref::rtSelf);
		Test(_local_Memcmp(oRef.GetPtr(), "U+000000AB", 11) == true);
	}
	
	
	// 2-byte
	
	{
		acpl::String::UnicharRef oRef(0xABCD);
		Test(oRef.IsNull()  == false);
		Test(oRef.GetPtr()  != NULL);
		Test(oRef.GetType() == acpl::String::Ref::rtSelf);
		Test(_local_Memcmp(oRef.GetPtr(), "U+ABCD", 7) == true);
	}
	{
		acpl::String::UnicharRef oRef(0xABCD, 3);
		Test(oRef.IsNull()  == false);
		Test(oRef.GetPtr()  != NULL);
		Test(oRef.GetType() == acpl::String::Ref::rtSelf);
		Test(_local_Memcmp(oRef.GetPtr(), "U+ABCD", 7) == true);
	}
	{
		acpl::String::UnicharRef oRef(0xABCD, 5);
		Test(oRef.IsNull()  == false);
		Test(oRef.GetPtr()  != NULL);
		Test(oRef.GetType() == acpl::String::Ref::rtSelf);
		Test(_local_Memcmp(oRef.GetPtr(), "U+0ABCD", 8) == true);
	}
	{
		acpl::String::UnicharRef oRef(0xABCD, 8);
		Test(oRef.IsNull()  == false);
		Test(oRef.GetPtr()  != NULL);
		Test(oRef.GetType() == acpl::String::Ref::rtSelf);
		Test(_local_Memcmp(oRef.GetPtr(), "U+0000ABCD", 11) == true);
	}
	{
		acpl::String::UnicharRef oRef(0xABCD, 9);
		Test(oRef.IsNull()  == false);
		Test(oRef.GetPtr()  != NULL);
		Test(oRef.GetType() == acpl::String::Ref::rtSelf);
		Test(_local_Memcmp(oRef.GetPtr(), "U+0000ABCD", 11) == true);
	}
	
	
	// 3-byte
	
	{
		acpl::String::UnicharRef oRef(0xABCDE);
		Test(oRef.IsNull()  == false);
		Test(oRef.GetPtr()  != NULL);
		Test(oRef.GetType() == acpl::String::Ref::rtSelf);
		Test(_local_Memcmp(oRef.GetPtr(), "U+ABCDE", 8) == true);
	}
	{
		acpl::String::UnicharRef oRef(0xABCDE, 3);
		Test(oRef.IsNull()  == false);
		Test(oRef.GetPtr()  != NULL);
		Test(oRef.GetType() == acpl::String::Ref::rtSelf);
		Test(_local_Memcmp(oRef.GetPtr(), "U+ABCDE", 8) == true);
	}
	{
		acpl::String::UnicharRef oRef(0xABCDE, 5);
		Test(oRef.IsNull()  == false);
		Test(oRef.GetPtr()  != NULL);
		Test(oRef.GetType() == acpl::String::Ref::rtSelf);
		Test(_local_Memcmp(oRef.GetPtr(), "U+ABCDE", 8) == true);
	}
	{
		acpl::String::UnicharRef oRef(0xABCDE, 8);
		Test(oRef.IsNull()  == false);
		Test(oRef.GetPtr()  != NULL);
		Test(oRef.GetType() == acpl::String::Ref::rtSelf);
		Test(_local_Memcmp(oRef.GetPtr(), "U+000ABCDE", 11) == true);
	}
	{
		acpl::String::UnicharRef oRef(0xABCDE, 9);
		Test(oRef.IsNull()  == false);
		Test(oRef.GetPtr()  != NULL);
		Test(oRef.GetType() == acpl::String::Ref::rtSelf);
		Test(_local_Memcmp(oRef.GetPtr(), "U+000ABCDE", 11) == true);
	}
	
	{
		acpl::String::UnicharRef oRef(0xABCDEF);
		Test(oRef.IsNull()  == false);
		Test(oRef.GetPtr()  != NULL);
		Test(oRef.GetType() == acpl::String::Ref::rtSelf);
		Test(_local_Memcmp(oRef.GetPtr(), "U+ABCDEF", 9) == true);
	}
	{
		acpl::String::UnicharRef oRef(0xABCDEF, 3);
		Test(oRef.IsNull()  == false);
		Test(oRef.GetPtr()  != NULL);
		Test(oRef.GetType() == acpl::String::Ref::rtSelf);
		Test(_local_Memcmp(oRef.GetPtr(), "U+ABCDEF", 9) == true);
	}
	{
		acpl::String::UnicharRef oRef(0xABCDEF, 5);
		Test(oRef.IsNull()  == false);
		Test(oRef.GetPtr()  != NULL);
		Test(oRef.GetType() == acpl::String::Ref::rtSelf);
		Test(_local_Memcmp(oRef.GetPtr(), "U+ABCDEF", 9) == true);
	}
	{
		acpl::String::UnicharRef oRef(0xABCDEF, 8);
		Test(oRef.IsNull()  == false);
		Test(oRef.GetPtr()  != NULL);
		Test(oRef.GetType() == acpl::String::Ref::rtSelf);
		Test(_local_Memcmp(oRef.GetPtr(), "U+00ABCDEF", 11) == true);
	}
	{
		acpl::String::UnicharRef oRef(0xABCDEF, 9);
		Test(oRef.IsNull()  == false);
		Test(oRef.GetPtr()  != NULL);
		Test(oRef.GetType() == acpl::String::Ref::rtSelf);
		Test(_local_Memcmp(oRef.GetPtr(), "U+00ABCDEF", 11) == true);
	}
	
	
	// 4-byte
	
	{
		acpl::String::UnicharRef oRef(0x9ABCDEF);
		Test(oRef.IsNull()  == false);
		Test(oRef.GetPtr()  != NULL);
		Test(oRef.GetType() == acpl::String::Ref::rtSelf);
		Test(_local_Memcmp(oRef.GetPtr(), "U+9ABCDEF", 9) == true);
	}
	{
		acpl::String::UnicharRef oRef(0x9ABCDEF, 3);
		Test(oRef.IsNull()  == false);
		Test(oRef.GetPtr()  != NULL);
		Test(oRef.GetType() == acpl::String::Ref::rtSelf);
		Test(_local_Memcmp(oRef.GetPtr(), "U+9ABCDEF", 9) == true);
	}
	{
		acpl::String::UnicharRef oRef(0x9ABCDEF, 5);
		Test(oRef.IsNull()  == false);
		Test(oRef.GetPtr()  != NULL);
		Test(oRef.GetType() == acpl::String::Ref::rtSelf);
		Test(_local_Memcmp(oRef.GetPtr(), "U+9ABCDEF", 9) == true);
	}
	{
		acpl::String::UnicharRef oRef(0x9ABCDEF, 8);
		Test(oRef.IsNull()  == false);
		Test(oRef.GetPtr()  != NULL);
		Test(oRef.GetType() == acpl::String::Ref::rtSelf);
		Test(_local_Memcmp(oRef.GetPtr(), "U+09ABCDEF", 11) == true);
	}
	{
		acpl::String::UnicharRef oRef(0x9ABCDEF, 9);
		Test(oRef.IsNull()  == false);
		Test(oRef.GetPtr()  != NULL);
		Test(oRef.GetType() == acpl::String::Ref::rtSelf);
		Test(_local_Memcmp(oRef.GetPtr(), "U+09ABCDEF", 11) == true);
	}
	
	{
		acpl::String::UnicharRef oRef(0x89ABCDEF);
		Test(oRef.IsNull()  == false);
		Test(oRef.GetPtr()  != NULL);
		Test(oRef.GetType() == acpl::String::Ref::rtSelf);
		Test(_local_Memcmp(oRef.GetPtr(), "U+89ABCDEF", 9) == true);
	}
	{
		acpl::String::UnicharRef oRef(0x89ABCDEF, 3);
		Test(oRef.IsNull()  == false);
		Test(oRef.GetPtr()  != NULL);
		Test(oRef.GetType() == acpl::String::Ref::rtSelf);
		Test(_local_Memcmp(oRef.GetPtr(), "U+89ABCDEF", 9) == true);
	}
	{
		acpl::String::UnicharRef oRef(0x89ABCDEF, 5);
		Test(oRef.IsNull()  == false);
		Test(oRef.GetPtr()  != NULL);
		Test(oRef.GetType() == acpl::String::Ref::rtSelf);
		Test(_local_Memcmp(oRef.GetPtr(), "U+89ABCDEF", 9) == true);
	}
	{
		acpl::String::UnicharRef oRef(0x89ABCDEF, 8);
		Test(oRef.IsNull()  == false);
		Test(oRef.GetPtr()  != NULL);
		Test(oRef.GetType() == acpl::String::Ref::rtSelf);
		Test(_local_Memcmp(oRef.GetPtr(), "U+89ABCDEF", 11) == true);
	}
	{
		acpl::String::UnicharRef oRef(0x89ABCDEF, 9);
		Test(oRef.IsNull()  == false);
		Test(oRef.GetPtr()  != NULL);
		Test(oRef.GetType() == acpl::String::Ref::rtSelf);
		Test(_local_Memcmp(oRef.GetPtr(), "U+89ABCDEF", 11) == true);
	}
	
	
	return 0;
}

static int TestPtrRef()
{
	PrintFn();
	
	
	void *oFakePtr;
	
	
	oFakePtr = reinterpret_cast<void *>(0x3A29B3F9);
	
	{
		acpl::String::PtrRef oRef(oFakePtr, true, 8);
		Test(oRef.IsNull()  == false);
		Test(oRef.GetPtr()  != NULL);
		Test(oRef.GetType() == acpl::String::Ref::rtSelf);
		Test(_local_Memcmp(oRef.GetPtr(), "0x3A29B3F9", 11) == true);
	}
	{
		acpl::String::PtrRef oRef(oFakePtr, true, 16);
		Test(oRef.IsNull()  == false);
		Test(oRef.GetPtr()  != NULL);
		Test(oRef.GetType() == acpl::String::Ref::rtSelf);
		Test(_local_Memcmp(oRef.GetPtr(), "0x000000003A29B3F9", 19) == true);
	}
	{
		acpl::String::PtrRef oRef(oFakePtr, true, acpl::String::PtrRef::NativePadding());
		Test(oRef.IsNull()  == false);
		Test(oRef.GetPtr()  != NULL);
		Test(oRef.GetType() == acpl::String::Ref::rtSelf);
		
		     if (sizeof(void *) == 8) Test(_local_Memcmp(oRef.GetPtr(), "0x000000003A29B3F9", 19) == true)
		else if (sizeof(void *) == 4) Test(_local_Memcmp(oRef.GetPtr(), "0x3A29B3F9", 11) == true)
		else                          Test(false)
	}
	
	
	return 0;
}

static int TestIpAddrRef()
{
	PrintFn();
	
	
	acpl::Ipv4Addr oIpv4;
	acpl::Ipv6Addr oIpv6;
	
	
	// Ipv4Addr
	
	{
		// `oIpv4` is reset
		acpl::String::IpAddrRef oRef(oIpv4);
		Test(oRef.IsNull()  == false);
		Test(oRef.GetPtr()  != NULL);
		Test(oRef.GetType() == acpl::String::Ref::rtSelf);
		Test(_local_Memcmp(oRef.GetPtr(), "0.0.0.0", 8) == true);
	}
	{
		oIpv4.Set(1, 10, 20, 200);
		acpl::String::IpAddrRef oRef(oIpv4);
		Test(oRef.IsNull()  == false);
		Test(oRef.GetPtr()  != NULL);
		Test(oRef.GetType() == acpl::String::Ref::rtSelf);
		Test(_local_Memcmp(oRef.GetPtr(), "1.10.20.200", 12) == true);
	}
	{
		oIpv4.Set(255, 255, 255, 255);
		acpl::String::IpAddrRef oRef(oIpv4);
		Test(oRef.IsNull()  == false);
		Test(oRef.GetPtr()  != NULL);
		Test(oRef.GetType() == acpl::String::Ref::rtSelf);
		Test(_local_Memcmp(oRef.GetPtr(), "255.255.255.255", 16) == true);
	}
	
	
	// Ipv6Addr (natural)
	
	{
		// `oIpv6` is reset
		acpl::String::IpAddrRef oRef(oIpv6, false, false);
		Test(oRef.IsNull()  == false);
		Test(oRef.GetPtr()  != NULL);
		Test(oRef.GetType() == acpl::String::Ref::rtSelf);
		Test(_local_Memcmp(oRef.GetPtr(), "0:0:0:0:0:0:0:0", 16) == true);
	}
	{
		oIpv6.Set(0xFE00, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000);
		acpl::String::IpAddrRef oRef(oIpv6, false, false);
		Test(oRef.IsNull()  == false);
		Test(oRef.GetPtr()  != NULL);
		Test(oRef.GetType() == acpl::String::Ref::rtSelf);
		Test(_local_Memcmp(oRef.GetPtr(), "FE00:0:0:0:0:0:0:0", 19) == true);
	}
	{
		oIpv6.Set(0xFEDC, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000);
		acpl::String::IpAddrRef oRef(oIpv6, false, false);
		Test(oRef.IsNull()  == false);
		Test(oRef.GetPtr()  != NULL);
		Test(oRef.GetType() == acpl::String::Ref::rtSelf);
		Test(_local_Memcmp(oRef.GetPtr(), "FEDC:0:0:0:0:0:0:0", 19) == true);
	}
	{
		oIpv6.Set(0xFEDC, 0xBA00, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000);
		acpl::String::IpAddrRef oRef(oIpv6, false, false);
		Test(oRef.IsNull()  == false);
		Test(oRef.GetPtr()  != NULL);
		Test(oRef.GetType() == acpl::String::Ref::rtSelf);
		Test(_local_Memcmp(oRef.GetPtr(), "FEDC:BA00:0:0:0:0:0:0", 22) == true);
	}
	{
		oIpv6.Set(0xFEDC, 0xBA98, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000);
		acpl::String::IpAddrRef oRef(oIpv6, false, false);
		Test(oRef.IsNull()  == false);
		Test(oRef.GetPtr()  != NULL);
		Test(oRef.GetType() == acpl::String::Ref::rtSelf);
		Test(_local_Memcmp(oRef.GetPtr(), "FEDC:BA98:0:0:0:0:0:0", 22) == true);
	}
	{
		oIpv6.Set(0xFEDC, 0xBA98, 0x7600, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000);
		acpl::String::IpAddrRef oRef(oIpv6, false, false);
		Test(oRef.IsNull()  == false);
		Test(oRef.GetPtr()  != NULL);
		Test(oRef.GetType() == acpl::String::Ref::rtSelf);
		Test(_local_Memcmp(oRef.GetPtr(), "FEDC:BA98:7600:0:0:0:0:0", 25) == true);
	}
	{
		oIpv6.Set(0xFEDC, 0xBA98, 0x7654, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000);
		acpl::String::IpAddrRef oRef(oIpv6, false, false);
		Test(oRef.IsNull()  == false);
		Test(oRef.GetPtr()  != NULL);
		Test(oRef.GetType() == acpl::String::Ref::rtSelf);
		Test(_local_Memcmp(oRef.GetPtr(), "FEDC:BA98:7654:0:0:0:0:0", 25) == true);
	}
	{
		oIpv6.Set(0xFEDC, 0xBA98, 0x7654, 0x3200, 0x0000, 0x0000, 0x0000, 0x0000);
		acpl::String::IpAddrRef oRef(oIpv6, false, false);
		Test(oRef.IsNull()  == false);
		Test(oRef.GetPtr()  != NULL);
		Test(oRef.GetType() == acpl::String::Ref::rtSelf);
		Test(_local_Memcmp(oRef.GetPtr(), "FEDC:BA98:7654:3200:0:0:0:0", 28) == true);
	}
	{
		oIpv6.Set(0xFEDC, 0xBA98, 0x7654, 0x3210, 0x0000, 0x0000, 0x0000, 0x0000);
		acpl::String::IpAddrRef oRef(oIpv6, false, false);
		Test(oRef.IsNull()  == false);
		Test(oRef.GetPtr()  != NULL);
		Test(oRef.GetType() == acpl::String::Ref::rtSelf);
		Test(_local_Memcmp(oRef.GetPtr(), "FEDC:BA98:7654:3210:0:0:0:0", 28) == true);
	}
	{
		oIpv6.Set(0xFEDC, 0xBA98, 0x7654, 0x3210, 0xFE00, 0x0000, 0x0000, 0x0000);
		acpl::String::IpAddrRef oRef(oIpv6, false, false);
		Test(oRef.IsNull()  == false);
		Test(oRef.GetPtr()  != NULL);
		Test(oRef.GetType() == acpl::String::Ref::rtSelf);
		Test(_local_Memcmp(oRef.GetPtr(), "FEDC:BA98:7654:3210:FE00:0:0:0", 31) == true);
	}
	{
		oIpv6.Set(0xFEDC, 0xBA98, 0x7654, 0x3210, 0xFEDC, 0x0000, 0x0000, 0x0000);
		acpl::String::IpAddrRef oRef(oIpv6, false, false);
		Test(oRef.IsNull()  == false);
		Test(oRef.GetPtr()  != NULL);
		Test(oRef.GetType() == acpl::String::Ref::rtSelf);
		Test(_local_Memcmp(oRef.GetPtr(), "FEDC:BA98:7654:3210:FEDC:0:0:0", 31) == true);
	}
	{
		oIpv6.Set(0xFEDC, 0xBA98, 0x7654, 0x3210, 0xFEDC, 0xBA00, 0x0000, 0x0000);
		acpl::String::IpAddrRef oRef(oIpv6, false, false);
		Test(oRef.IsNull()  == false);
		Test(oRef.GetPtr()  != NULL);
		Test(oRef.GetType() == acpl::String::Ref::rtSelf);
		Test(_local_Memcmp(oRef.GetPtr(), "FEDC:BA98:7654:3210:FEDC:BA00:0:0", 34) == true);
	}
	{
		oIpv6.Set(0xFEDC, 0xBA98, 0x7654, 0x3210, 0xFEDC, 0xBA98, 0x0000, 0x0000);
		acpl::String::IpAddrRef oRef(oIpv6, false, false);
		Test(oRef.IsNull()  == false);
		Test(oRef.GetPtr()  != NULL);
		Test(oRef.GetType() == acpl::String::Ref::rtSelf);
		Test(_local_Memcmp(oRef.GetPtr(), "FEDC:BA98:7654:3210:FEDC:BA98:0:0", 34) == true);
	}
	{
		oIpv6.Set(0xFEDC, 0xBA98, 0x7654, 0x3210, 0xFEDC, 0xBA98, 0x7600, 0x0000);
		acpl::String::IpAddrRef oRef(oIpv6, false, false);
		Test(oRef.IsNull()  == false);
		Test(oRef.GetPtr()  != NULL);
		Test(oRef.GetType() == acpl::String::Ref::rtSelf);
		Test(_local_Memcmp(oRef.GetPtr(), "FEDC:BA98:7654:3210:FEDC:BA98:7600:0", 37) == true);
	}
	{
		oIpv6.Set(0xFEDC, 0xBA98, 0x7654, 0x3210, 0xFEDC, 0xBA98, 0x7654, 0x0000);
		acpl::String::IpAddrRef oRef(oIpv6, false, false);
		Test(oRef.IsNull()  == false);
		Test(oRef.GetPtr()  != NULL);
		Test(oRef.GetType() == acpl::String::Ref::rtSelf);
		Test(_local_Memcmp(oRef.GetPtr(), "FEDC:BA98:7654:3210:FEDC:BA98:7654:0", 37) == true);
	}
	{
		oIpv6.Set(0xFEDC, 0xBA98, 0x7654, 0x3210, 0xFEDC, 0xBA98, 0x7654, 0x3200);
		acpl::String::IpAddrRef oRef(oIpv6, false, false);
		Test(oRef.IsNull()  == false);
		Test(oRef.GetPtr()  != NULL);
		Test(oRef.GetType() == acpl::String::Ref::rtSelf);
		Test(_local_Memcmp(oRef.GetPtr(), "FEDC:BA98:7654:3210:FEDC:BA98:7654:3200", 40) == true);
	}
	{
		oIpv6.Set(0xFEDC, 0xBA98, 0x7654, 0x3210, 0xFEDC, 0xBA98, 0x7654, 0x3210);
		acpl::String::IpAddrRef oRef(oIpv6, false, false);
		Test(oRef.IsNull()  == false);
		Test(oRef.GetPtr()  != NULL);
		Test(oRef.GetType() == acpl::String::Ref::rtSelf);
		Test(_local_Memcmp(oRef.GetPtr(), "FEDC:BA98:7654:3210:FEDC:BA98:7654:3210", 40) == true);
	}
	{
		oIpv6.Set(0x00DC, 0xBA98, 0x7654, 0x3210, 0xFEDC, 0xBA98, 0x7654, 0x3210);
		acpl::String::IpAddrRef oRef(oIpv6, false, false);
		Test(oRef.IsNull()  == false);
		Test(oRef.GetPtr()  != NULL);
		Test(oRef.GetType() == acpl::String::Ref::rtSelf);
		Test(_local_Memcmp(oRef.GetPtr(), "DC:BA98:7654:3210:FEDC:BA98:7654:3210", 38) == true);
	}
	{
		oIpv6.Set(0x0000, 0xBA98, 0x7654, 0x3210, 0xFEDC, 0xBA98, 0x7654, 0x3210);
		acpl::String::IpAddrRef oRef(oIpv6, false, false);
		Test(oRef.IsNull()  == false);
		Test(oRef.GetPtr()  != NULL);
		Test(oRef.GetType() == acpl::String::Ref::rtSelf);
		Test(_local_Memcmp(oRef.GetPtr(), "0:BA98:7654:3210:FEDC:BA98:7654:3210", 37) == true);
	}
	{
		oIpv6.Set(0x0000, 0x0098, 0x7654, 0x3210, 0xFEDC, 0xBA98, 0x7654, 0x3210);
		acpl::String::IpAddrRef oRef(oIpv6, false, false);
		Test(oRef.IsNull()  == false);
		Test(oRef.GetPtr()  != NULL);
		Test(oRef.GetType() == acpl::String::Ref::rtSelf);
		Test(_local_Memcmp(oRef.GetPtr(), "0:98:7654:3210:FEDC:BA98:7654:3210", 35) == true);
	}
	{
		oIpv6.Set(0x0000, 0x0000, 0x7654, 0x3210, 0xFEDC, 0xBA98, 0x7654, 0x3210);
		acpl::String::IpAddrRef oRef(oIpv6, false, false);
		Test(oRef.IsNull()  == false);
		Test(oRef.GetPtr()  != NULL);
		Test(oRef.GetType() == acpl::String::Ref::rtSelf);
		Test(_local_Memcmp(oRef.GetPtr(), "0:0:7654:3210:FEDC:BA98:7654:3210", 34) == true);
	}
	{
		oIpv6.Set(0x0000, 0x0000, 0x0054, 0x3210, 0xFEDC, 0xBA98, 0x7654, 0x3210);
		acpl::String::IpAddrRef oRef(oIpv6, false, false);
		Test(oRef.IsNull()  == false);
		Test(oRef.GetPtr()  != NULL);
		Test(oRef.GetType() == acpl::String::Ref::rtSelf);
		Test(_local_Memcmp(oRef.GetPtr(), "0:0:54:3210:FEDC:BA98:7654:3210", 32) == true);
	}
	{
		oIpv6.Set(0x0000, 0x0000, 0x0000, 0x3210, 0xFEDC, 0xBA98, 0x7654, 0x3210);
		acpl::String::IpAddrRef oRef(oIpv6, false, false);
		Test(oRef.IsNull()  == false);
		Test(oRef.GetPtr()  != NULL);
		Test(oRef.GetType() == acpl::String::Ref::rtSelf);
		Test(_local_Memcmp(oRef.GetPtr(), "0:0:0:3210:FEDC:BA98:7654:3210", 31) == true);
	}
	{
		oIpv6.Set(0x0000, 0x0000, 0x0000, 0x0010, 0xFEDC, 0xBA98, 0x7654, 0x3210);
		acpl::String::IpAddrRef oRef(oIpv6, false, false);
		Test(oRef.IsNull()  == false);
		Test(oRef.GetPtr()  != NULL);
		Test(oRef.GetType() == acpl::String::Ref::rtSelf);
		Test(_local_Memcmp(oRef.GetPtr(), "0:0:0:10:FEDC:BA98:7654:3210", 29) == true);
	}
	{
		oIpv6.Set(0x0000, 0x0000, 0x0000, 0x0000, 0xFEDC, 0xBA98, 0x7654, 0x3210);
		acpl::String::IpAddrRef oRef(oIpv6, false, false);
		Test(oRef.IsNull()  == false);
		Test(oRef.GetPtr()  != NULL);
		Test(oRef.GetType() == acpl::String::Ref::rtSelf);
		Test(_local_Memcmp(oRef.GetPtr(), "0:0:0:0:FEDC:BA98:7654:3210", 28) == true);
	}
	{
		oIpv6.Set(0x0000, 0x0000, 0x0000, 0x0000, 0x00DC, 0xBA98, 0x7654, 0x3210);
		acpl::String::IpAddrRef oRef(oIpv6, false, false);
		Test(oRef.IsNull()  == false);
		Test(oRef.GetPtr()  != NULL);
		Test(oRef.GetType() == acpl::String::Ref::rtSelf);
		Test(_local_Memcmp(oRef.GetPtr(), "0:0:0:0:DC:BA98:7654:3210", 26) == true);
	}
	{
		oIpv6.Set(0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xBA98, 0x7654, 0x3210);
		acpl::String::IpAddrRef oRef(oIpv6, false, false);
		Test(oRef.IsNull()  == false);
		Test(oRef.GetPtr()  != NULL);
		Test(oRef.GetType() == acpl::String::Ref::rtSelf);
		Test(_local_Memcmp(oRef.GetPtr(), "0:0:0:0:0:BA98:7654:3210", 25) == true);
	}
	{
		oIpv6.Set(0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0098, 0x7654, 0x3210);
		acpl::String::IpAddrRef oRef(oIpv6, false, false);
		Test(oRef.IsNull()  == false);
		Test(oRef.GetPtr()  != NULL);
		Test(oRef.GetType() == acpl::String::Ref::rtSelf);
		Test(_local_Memcmp(oRef.GetPtr(), "0:0:0:0:0:98:7654:3210", 23) == true);
	}
	{
		oIpv6.Set(0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x7654, 0x3210);
		acpl::String::IpAddrRef oRef(oIpv6, false, false);
		Test(oRef.IsNull()  == false);
		Test(oRef.GetPtr()  != NULL);
		Test(oRef.GetType() == acpl::String::Ref::rtSelf);
		Test(_local_Memcmp(oRef.GetPtr(), "0:0:0:0:0:0:7654:3210", 22) == true);
	}
	{
		oIpv6.Set(0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0054, 0x3210);
		acpl::String::IpAddrRef oRef(oIpv6, false, false);
		Test(oRef.IsNull()  == false);
		Test(oRef.GetPtr()  != NULL);
		Test(oRef.GetType() == acpl::String::Ref::rtSelf);
		Test(_local_Memcmp(oRef.GetPtr(), "0:0:0:0:0:0:54:3210", 20) == true);
	}
	{
		oIpv6.Set(0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x3210);
		acpl::String::IpAddrRef oRef(oIpv6, false, false);
		Test(oRef.IsNull()  == false);
		Test(oRef.GetPtr()  != NULL);
		Test(oRef.GetType() == acpl::String::Ref::rtSelf);
		Test(_local_Memcmp(oRef.GetPtr(), "0:0:0:0:0:0:0:3210", 19) == true);
	}
	{
		oIpv6.Set(0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0010);
		acpl::String::IpAddrRef oRef(oIpv6, false, false);
		Test(oRef.IsNull()  == false);
		Test(oRef.GetPtr()  != NULL);
		Test(oRef.GetType() == acpl::String::Ref::rtSelf);
		Test(_local_Memcmp(oRef.GetPtr(), "0:0:0:0:0:0:0:10", 17) == true);
	}
	
	
	// TODO: Ipv6Addr (optimized)
	
	
	// Ipv6Addr (Ipv4 mode)
	
	{
		oIpv6.Set(0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0, 0, 0, 0);
		acpl::String::IpAddrRef oRef(oIpv6, false, true);
		Test(oRef.IsNull()  == false);
		Test(oRef.GetPtr()  != NULL);
		Test(oRef.GetType() == acpl::String::Ref::rtSelf);
		Test(_local_Memcmp(oRef.GetPtr(), "0:0:0:0:0:0:0.0.0.0", 20) == true);
	}
	{
		oIpv6.Set(0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 1, 10, 20, 200);
		acpl::String::IpAddrRef oRef(oIpv6, false, true);
		Test(oRef.IsNull()  == false);
		Test(oRef.GetPtr()  != NULL);
		Test(oRef.GetType() == acpl::String::Ref::rtSelf);
		Test(_local_Memcmp(oRef.GetPtr(), "0:0:0:0:0:0:1.10.20.200", 24) == true);
	}
	{
		oIpv6.Set(0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 255, 255, 255, 255);
		acpl::String::IpAddrRef oRef(oIpv6, false, true);
		Test(oRef.IsNull()  == false);
		Test(oRef.GetPtr()  != NULL);
		Test(oRef.GetType() == acpl::String::Ref::rtSelf);
		Test(_local_Memcmp(oRef.GetPtr(), "0:0:0:0:0:0:255.255.255.255", 28) == true);
	}
	{
		oIpv6.Set(0xFEDC, 0xBA98, 0x7654, 0x3210, 0xFEDC, 0xBA98, 0, 0, 0, 0);
		acpl::String::IpAddrRef oRef(oIpv6, false, true);
		Test(oRef.IsNull()  == false);
		Test(oRef.GetPtr()  != NULL);
		Test(oRef.GetType() == acpl::String::Ref::rtSelf);
		Test(_local_Memcmp(oRef.GetPtr(), "FEDC:BA98:7654:3210:FEDC:BA98:0.0.0.0", 38) == true);
	}
	{
		oIpv6.Set(0xFEDC, 0xBA98, 0x7654, 0x3210, 0xFEDC, 0xBA98, 1, 10, 20, 200);
		acpl::String::IpAddrRef oRef(oIpv6, false, true);
		Test(oRef.IsNull()  == false);
		Test(oRef.GetPtr()  != NULL);
		Test(oRef.GetType() == acpl::String::Ref::rtSelf);
		Test(_local_Memcmp(oRef.GetPtr(), "FEDC:BA98:7654:3210:FEDC:BA98:1.10.20.200", 42) == true);
	}
	{
		oIpv6.Set(0xFEDC, 0xBA98, 0x7654, 0x3210, 0xFEDC, 0xBA98, 255, 255, 255, 255);
		acpl::String::IpAddrRef oRef(oIpv6, false, true);
		Test(oRef.IsNull()  == false);
		Test(oRef.GetPtr()  != NULL);
		Test(oRef.GetType() == acpl::String::Ref::rtSelf);
		Test(_local_Memcmp(oRef.GetPtr(), "FEDC:BA98:7654:3210:FEDC:BA98:255.255.255.255", 46) == true);
	}
	
	
	return 0;
}

static int TestNetAddrRef()
{
	PrintFn();
	
	
	acpl::DomainName oDn;
	acpl::Ipv4Addr oIpv4;
	acpl::Ipv6Addr oIpv6;
	
	
	
	{
		Test(oDn.Parse("www.example.com") == true);
		acpl::String::NetAddrRef oRef(oDn);
		Test(oRef.IsNull()  == false);
		Test(oRef.GetPtr()  != NULL);
		Test(oRef.GetType() == acpl::String::Ref::rtSelf);
		Test(_local_Memcmp(oRef.GetPtr(), "www.example.com", 16) == true);
	}
	{
		oIpv4.Set(255, 255, 255, 255);
		acpl::String::NetAddrRef oRef(oIpv4);
		Test(oRef.IsNull()  == false);
		Test(oRef.GetPtr()  != NULL);
		Test(oRef.GetType() == acpl::String::Ref::rtSelf);
		Test(_local_Memcmp(oRef.GetPtr(), "255.255.255.255", 16) == true);
	}
	{
		oIpv6.Set(0xFEDC, 0xBA98, 0x7654, 0x3210, 0xFEDC, 0xBA98, 0x7654, 0x3210);
		acpl::String::NetAddrRef oRef(oIpv6, false, false);
		Test(oRef.IsNull()  == false);
		Test(oRef.GetPtr()  != NULL);
		Test(oRef.GetType() == acpl::String::Ref::rtSelf);
		Test(_local_Memcmp(oRef.GetPtr(), "FEDC:BA98:7654:3210:FEDC:BA98:7654:3210", 40) == true);
	}
	{
		oIpv6.Set(0xFEDC, 0xBA98, 0x7654, 0x3210, 0xFEDC, 0xBA98, 255, 255, 255, 255);
		acpl::String::NetAddrRef oRef(oIpv6, false, true);
		Test(oRef.IsNull()  == false);
		Test(oRef.GetPtr()  != NULL);
		Test(oRef.GetType() == acpl::String::Ref::rtSelf);
		Test(_local_Memcmp(oRef.GetPtr(), "FEDC:BA98:7654:3210:FEDC:BA98:255.255.255.255", 46) == true);
	}
	
	
	
	return 0;
}

static int TestRefMeasureConvertSelf_1ByteSeq()
{
	PrintFn();
	
	
	acpl::String oStr("abc");
	
	
	{
		acpl::String::Ref oRef(oStr);
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 1, "a"}, {2, 0, 0, 2, "ab"}, {3, 0, 0, 3, "abc"}, {4, 0, 0, 3, "abc"},
				{0, 1, 1, 0, ""}, {1, 1, 1, 1, "b"}, {2, 1, 1, 2, "bc"}, {3, 1, 1, 2, "bc" }, {4, 1, 1, 2, "bc" },
				{0, 2, 2, 0, ""}, {1, 2, 2, 1, "c"}, {2, 2, 2, 1, "c" }, {3, 2, 2, 1, "c"  }, {4, 2, 2, 1, "c"  },
				{0, 3, 3, 0, ""}, {1, 3, 3, 0, "" }, {2, 3, 3, 0, ""  }, {3, 3, 3, 0, ""   }, {4, 3, 3, 0, ""   },
				{0, 4, 3, 0, ""}, {1, 4, 3, 0, "" }, {2, 4, 3, 0, ""  }, {3, 4, 3, 0, ""   }, {4, 4, 3, 0, ""   }
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
	
	return 0;
}

static int TestRefMeasureConvertSelf_2ByteSeq()
{
	PrintFn();
	
	
	acpl::String oStr;
	
	
	{
		oStr.Set("\xDF\xBF""bc"); // \xDF\xBF == U+7FF
		acpl::String::Ref oRef(oStr);
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 2, "\xDF\xBF"}, {2, 0, 0, 3, "\xDF\xBF""b"}, {3, 0, 0, 4, "\xDF\xBF""bc"}, {4, 0, 0, 4, "\xDF\xBF""bc"},
				{0, 1, 2, 0, ""}, {1, 1, 2, 1, "b"       }, {2, 1, 2, 2, "bc"         }, {3, 1, 2, 2, "bc"          }, {4, 1, 2, 2, "bc"          },
				{0, 2, 3, 0, ""}, {1, 2, 3, 1, "c"       }, {2, 2, 3, 1, "c"          }, {3, 2, 3, 1, "c"           }, {4, 2, 3, 1, "c"           },
				{0, 3, 4, 0, ""}, {1, 3, 4, 0, ""        }, {2, 3, 4, 0, ""           }, {3, 3, 4, 0, ""            }, {4, 3, 4, 0, ""            },
				{0, 4, 4, 0, ""}, {1, 4, 4, 0, ""        }, {2, 4, 4, 0, ""           }, {3, 4, 4, 0, ""            }, {4, 4, 4, 0, ""            }
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
	{
		oStr.Set("a\xDF\xBF""c"); // \xDF\xBF == U+7FF
		acpl::String::Ref oRef(oStr);
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 1, "a"       }, {2, 0, 0, 3, "a\xDF\xBF"  }, {3, 0, 0, 4, "a\xDF\xBF""c"}, {4, 0, 0, 4, "a\xDF\xBF""c"},
				{0, 1, 1, 0, ""}, {1, 1, 1, 2, "\xDF\xBF"}, {2, 1, 1, 3, "\xDF\xBF""c"}, {3, 1, 1, 3, "\xDF\xBF""c" }, {4, 1, 1, 3, "\xDF\xBF""c" },
				{0, 2, 3, 0, ""}, {1, 2, 3, 1, "c"       }, {2, 2, 3, 1, "c"          }, {3, 2, 3, 1, "c"           }, {4, 2, 3, 1, "c"           },
				{0, 3, 4, 0, ""}, {1, 3, 4, 0, ""        }, {2, 3, 4, 0, ""           }, {3, 3, 4, 0, ""            }, {4, 3, 4, 0, ""            },
				{0, 4, 4, 0, ""}, {1, 4, 4, 0, ""        }, {2, 4, 4, 0, ""           }, {3, 4, 4, 0, ""            }, {4, 4, 4, 0, ""            }
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
	{
		oStr.Set("ab\xDF\xBF"); // \xDF\xBF == U+7FF
		acpl::String::Ref oRef(oStr);
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 1, "a"       }, {2, 0, 0, 2, "ab"       }, {3, 0, 0, 4, "ab\xDF\xBF"}, {4, 0, 0, 4, "ab\xDF\xBF"},
				{0, 1, 1, 0, ""}, {1, 1, 1, 1, "b"       }, {2, 1, 1, 3, "b\xDF\xBF"}, {3, 1, 1, 3, "b\xDF\xBF" }, {4, 1, 1, 3, "b\xDF\xBF" },
				{0, 2, 2, 0, ""}, {1, 2, 2, 2, "\xDF\xBF"}, {2, 2, 2, 2, "\xDF\xBF" }, {3, 2, 2, 2, "\xDF\xBF"  }, {4, 2, 2, 2, "\xDF\xBF"  },
				{0, 3, 4, 0, ""}, {1, 3, 4, 0, ""        }, {2, 3, 4, 0, ""         }, {3, 3, 4, 0, ""          }, {4, 3, 4, 0, ""          },
				{0, 4, 4, 0, ""}, {1, 4, 4, 0, ""        }, {2, 4, 4, 0, ""         }, {3, 4, 4, 0, ""          }, {4, 4, 4, 0, ""          }
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
	
	return 0;
}

static int TestRefMeasureConvertSelf_3ByteSeq()
{
	PrintFn();
	
	
	acpl::String oStr;
	
	
	{
		oStr.Set("\xEF\xBF\xBF""bc"); // \xEF\xBF\xBF == U+FFFF
		acpl::String::Ref oRef(oStr);
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 3, "\xEF\xBF\xBF"}, {2, 0, 0, 4, "\xEF\xBF\xBF""b"}, {3, 0, 0, 5, "\xEF\xBF\xBF""bc"}, {4, 0, 0, 5, "\xEF\xBF\xBF""bc"},
				{0, 1, 3, 0, ""}, {1, 1, 3, 1, "b"           }, {2, 1, 3, 2, "bc"             }, {3, 1, 3, 2, "bc"              }, {4, 1, 3, 2, "bc"              },
				{0, 2, 4, 0, ""}, {1, 2, 4, 1, "c"           }, {2, 2, 4, 1, "c"              }, {3, 2, 4, 1, "c"               }, {4, 2, 4, 1, "c"               },
				{0, 3, 5, 0, ""}, {1, 3, 5, 0, ""            }, {2, 3, 5, 0, ""               }, {3, 3, 5, 0, ""                }, {4, 3, 5, 0, ""                },
				{0, 4, 5, 0, ""}, {1, 4, 5, 0, ""            }, {2, 4, 5, 0, ""               }, {3, 4, 5, 0, ""                }, {4, 4, 5, 0, ""                }
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
	{
		oStr.Set("a\xEF\xBF\xBF""c"); // \xEF\xBF\xBF == U+FFFF
		acpl::String::Ref oRef(oStr);
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 1, "a"           }, {2, 0, 0, 4, "a\xEF\xBF\xBF"  }, {3, 0, 0, 5, "a\xEF\xBF\xBF""c"}, {4, 0, 0, 5, "a\xEF\xBF\xBF""c"},
				{0, 1, 1, 0, ""}, {1, 1, 1, 3, "\xEF\xBF\xBF"}, {2, 1, 1, 4, "\xEF\xBF\xBF""c"}, {3, 1, 1, 4, "\xEF\xBF\xBF""c" }, {4, 1, 1, 4, "\xEF\xBF\xBF""c" },
				{0, 2, 4, 0, ""}, {1, 2, 4, 1, "c"           }, {2, 2, 4, 1, "c"              }, {3, 2, 4, 1, "c"               }, {4, 2, 4, 1, "c"               },
				{0, 3, 5, 0, ""}, {1, 3, 5, 0, ""            }, {2, 3, 5, 0, ""               }, {3, 3, 5, 0, ""                }, {4, 3, 5, 0, ""                },
				{0, 4, 5, 0, ""}, {1, 4, 5, 0, ""            }, {2, 4, 5, 0, ""               }, {3, 4, 5, 0, ""                }, {4, 4, 5, 0, ""                }
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
	{
		oStr.Set("ab\xEF\xBF\xBF"); // \xEF\xBF\xBF == U+FFFF
		acpl::String::Ref oRef(oStr);
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 1, "a"           }, {2, 0, 0, 2, "ab"           }, {3, 0, 0, 5, "ab\xEF\xBF\xBF"}, {4, 0, 0, 5, "ab\xEF\xBF\xBF"},
				{0, 1, 1, 0, ""}, {1, 1, 1, 1, "b"           }, {2, 1, 1, 4, "b\xEF\xBF\xBF"}, {3, 1, 1, 4, "b\xEF\xBF\xBF" }, {4, 1, 1, 4, "b\xEF\xBF\xBF" },
				{0, 2, 2, 0, ""}, {1, 2, 2, 3, "\xEF\xBF\xBF"}, {2, 2, 2, 3, "\xEF\xBF\xBF" }, {3, 2, 2, 3, "\xEF\xBF\xBF"  }, {4, 2, 2, 3, "\xEF\xBF\xBF"  },
				{0, 3, 5, 0, ""}, {1, 3, 5, 0, ""            }, {2, 3, 5, 0, ""             }, {3, 3, 5, 0, ""              }, {4, 3, 5, 0, ""              },
				{0, 4, 5, 0, ""}, {1, 4, 5, 0, ""            }, {2, 4, 5, 0, ""             }, {3, 4, 5, 0, ""              }, {4, 4, 5, 0, ""              }
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
	
	return 0;
}

static int TestRefMeasureConvertSelf_4ByteSeq()
{
	PrintFn();
	
	
	acpl::String oStr;
	
	
	{
		oStr.Set("\xF7\xBF\xBF\xBF""bc"); // \xF7\xBF\xBF\xBF == U+1FFFFF
		acpl::String::Ref oRef(oStr);
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 4, "\xF7\xBF\xBF\xBF"}, {2, 0, 0, 5, "\xF7\xBF\xBF\xBF""b"}, {3, 0, 0, 6, "\xF7\xBF\xBF\xBF""bc"}, {4, 0, 0, 6, "\xF7\xBF\xBF\xBF""bc"},
				{0, 1, 4, 0, ""}, {1, 1, 4, 1, "b"               }, {2, 1, 4, 2, "bc"                 }, {3, 1, 4, 2, "bc"                  }, {4, 1, 4, 2, "bc"                  },
				{0, 2, 5, 0, ""}, {1, 2, 5, 1, "c"               }, {2, 2, 5, 1, "c"                  }, {3, 2, 5, 1, "c"                   }, {4, 2, 5, 1, "c"                   },
				{0, 3, 6, 0, ""}, {1, 3, 6, 0, ""                }, {2, 3, 6, 0, ""                   }, {3, 3, 6, 0, ""                    }, {4, 3, 6, 0, ""                    },
				{0, 4, 6, 0, ""}, {1, 4, 6, 0, ""                }, {2, 4, 6, 0, ""                   }, {3, 4, 6, 0, ""                    }, {4, 4, 6, 0, ""                    }
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
	{
		oStr.Set("a\xF7\xBF\xBF\xBF""c"); // \xF7\xBF\xBF\xBF == U+1FFFFF
		acpl::String::Ref oRef(oStr);
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 1, "a"               }, {2, 0, 0, 5, "a\xF7\xBF\xBF\xBF"  }, {3, 0, 0, 6, "a\xF7\xBF\xBF\xBF""c"}, {4, 0, 0, 6, "a\xF7\xBF\xBF\xBF""c"},
				{0, 1, 1, 0, ""}, {1, 1, 1, 4, "\xF7\xBF\xBF\xBF"}, {2, 1, 1, 5, "\xF7\xBF\xBF\xBF""c"}, {3, 1, 1, 5, "\xF7\xBF\xBF\xBF""c" }, {4, 1, 1, 5, "\xF7\xBF\xBF\xBF""c" },
				{0, 2, 5, 0, ""}, {1, 2, 5, 1, "c"               }, {2, 2, 5, 1, "c"                  }, {3, 2, 5, 1, "c"                   }, {4, 2, 5, 1, "c"                   },
				{0, 3, 6, 0, ""}, {1, 3, 6, 0, ""                }, {2, 3, 6, 0, ""                   }, {3, 3, 6, 0, ""                    }, {4, 3, 6, 0, ""                    },
				{0, 4, 6, 0, ""}, {1, 4, 6, 0, ""                }, {2, 4, 6, 0, ""                   }, {3, 4, 6, 0, ""                    }, {4, 4, 6, 0, ""                    }
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
	{
		oStr.Set("ab\xF7\xBF\xBF\xBF"); // \xF7\xBF\xBF\xBF == U+1FFFFF
		acpl::String::Ref oRef(oStr);
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 1, "a"               }, {2, 0, 0, 2, "ab"               }, {3, 0, 0, 6, "ab\xF7\xBF\xBF\xBF"}, {4, 0, 0, 6, "ab\xF7\xBF\xBF\xBF"},
				{0, 1, 1, 0, ""}, {1, 1, 1, 1, "b"               }, {2, 1, 1, 5, "b\xF7\xBF\xBF\xBF"}, {3, 1, 1, 5, "b\xF7\xBF\xBF\xBF" }, {4, 1, 1, 5, "b\xF7\xBF\xBF\xBF" },
				{0, 2, 2, 0, ""}, {1, 2, 2, 4, "\xF7\xBF\xBF\xBF"}, {2, 2, 2, 4, "\xF7\xBF\xBF\xBF" }, {3, 2, 2, 4, "\xF7\xBF\xBF\xBF"  }, {4, 2, 2, 4, "\xF7\xBF\xBF\xBF"  },
				{0, 3, 6, 0, ""}, {1, 3, 6, 0, ""                }, {2, 3, 6, 0, ""                 }, {3, 3, 6, 0, ""                  }, {4, 3, 6, 0, ""                  },
				{0, 4, 6, 0, ""}, {1, 4, 6, 0, ""                }, {2, 4, 6, 0, ""                 }, {3, 4, 6, 0, ""                  }, {4, 4, 6, 0, ""                  }
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
	
	return 0;
}

static int TestRefMeasureConvertSelf_5ByteSeq()
{
	PrintFn();
	
	
	acpl::String oStr;
	
	
	{
		oStr.Set("\xFB\xBF\xBF\xBF\xBF""bc"); // \xFB\xBF\xBF\xBF\xBF == U+3FFFFFF
		acpl::String::Ref oRef(oStr);
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 5, "\xFB\xBF\xBF\xBF\xBF"}, {2, 0, 0, 6, "\xFB\xBF\xBF\xBF\xBF""b"}, {3, 0, 0, 7, "\xFB\xBF\xBF\xBF\xBF""bc"}, {4, 0, 0, 7, "\xFB\xBF\xBF\xBF\xBF""bc"},
				{0, 1, 5, 0, ""}, {1, 1, 5, 1, "b"                   }, {2, 1, 5, 2, "bc"                     }, {3, 1, 5, 2, "bc"                      }, {4, 1, 5, 2, "bc"                      },
				{0, 2, 6, 0, ""}, {1, 2, 6, 1, "c"                   }, {2, 2, 6, 1, "c"                      }, {3, 2, 6, 1, "c"                       }, {4, 2, 6, 1, "c"                       },
				{0, 3, 7, 0, ""}, {1, 3, 7, 0, ""                    }, {2, 3, 7, 0, ""                       }, {3, 3, 7, 0, ""                        }, {4, 3, 7, 0, ""                        },
				{0, 4, 7, 0, ""}, {1, 4, 7, 0, ""                    }, {2, 4, 7, 0, ""                       }, {3, 4, 7, 0, ""                        }, {4, 4, 7, 0, ""                        }
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
	{
		oStr.Set("a\xFB\xBF\xBF\xBF\xBF""c"); // \xFB\xBF\xBF\xBF\xBF == U+3FFFFFF
		acpl::String::Ref oRef(oStr);
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 1, "a"                   }, {2, 0, 0, 6, "a\xFB\xBF\xBF\xBF\xBF"  }, {3, 0, 0, 7, "a\xFB\xBF\xBF\xBF\xBF""c"}, {4, 0, 0, 7, "a\xFB\xBF\xBF\xBF\xBF""c"},
				{0, 1, 1, 0, ""}, {1, 1, 1, 5, "\xFB\xBF\xBF\xBF\xBF"}, {2, 1, 1, 6, "\xFB\xBF\xBF\xBF\xBF""c"}, {3, 1, 1, 6, "\xFB\xBF\xBF\xBF\xBF""c" }, {4, 1, 1, 6, "\xFB\xBF\xBF\xBF\xBF""c" },
				{0, 2, 6, 0, ""}, {1, 2, 6, 1, "c"                   }, {2, 2, 6, 1, "c"                      }, {3, 2, 6, 1, "c"                       }, {4, 2, 6, 1, "c"                       },
				{0, 3, 7, 0, ""}, {1, 3, 7, 0, ""                    }, {2, 3, 7, 0, ""                       }, {3, 3, 7, 0, ""                        }, {4, 3, 7, 0, ""                        },
				{0, 4, 7, 0, ""}, {1, 4, 7, 0, ""                    }, {2, 4, 7, 0, ""                       }, {3, 4, 7, 0, ""                        }, {4, 4, 7, 0, ""                        }
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
	{
		oStr.Set("ab\xFB\xBF\xBF\xBF\xBF"); // \xFB\xBF\xBF\xBF\xBF == U+3FFFFFF
		acpl::String::Ref oRef(oStr);
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 1, "a"                   }, {2, 0, 0, 2, "ab"                   }, {3, 0, 0, 7, "ab\xFB\xBF\xBF\xBF\xBF"}, {4, 0, 0, 7, "ab\xFB\xBF\xBF\xBF\xBF"},
				{0, 1, 1, 0, ""}, {1, 1, 1, 1, "b"                   }, {2, 1, 1, 6, "b\xFB\xBF\xBF\xBF\xBF"}, {3, 1, 1, 6, "b\xFB\xBF\xBF\xBF\xBF" }, {4, 1, 1, 6, "b\xFB\xBF\xBF\xBF\xBF" },
				{0, 2, 2, 0, ""}, {1, 2, 2, 5, "\xFB\xBF\xBF\xBF\xBF"}, {2, 2, 2, 5, "\xFB\xBF\xBF\xBF\xBF" }, {3, 2, 2, 5, "\xFB\xBF\xBF\xBF\xBF"  }, {4, 2, 2, 5, "\xFB\xBF\xBF\xBF\xBF"  },
				{0, 3, 7, 0, ""}, {1, 3, 7, 0, ""                    }, {2, 3, 7, 0, ""                     }, {3, 3, 7, 0, ""                      }, {4, 3, 7, 0, ""                      },
				{0, 4, 7, 0, ""}, {1, 4, 7, 0, ""                    }, {2, 4, 7, 0, ""                     }, {3, 4, 7, 0, ""                      }, {4, 4, 7, 0, ""                      }
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
	
	return 0;
}

static int TestRefMeasureConvertSelf_6ByteSeq()
{
	PrintFn();
	
	
	acpl::String oStr;
	
	
	{
		oStr.Set("\xFD\xBF\xBF\xBF\xBF\xBF""bc"); // \xFD\xBF\xBF\xBF\xBF\xBF == U+7FFFFFFF
		acpl::String::Ref oRef(oStr);
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 6, "\xFD\xBF\xBF\xBF\xBF\xBF"}, {2, 0, 0, 7, "\xFD\xBF\xBF\xBF\xBF\xBF""b"}, {3, 0, 0, 8, "\xFD\xBF\xBF\xBF\xBF\xBF""bc"}, {4, 0, 0, 8, "\xFD\xBF\xBF\xBF\xBF\xBF""bc"},
				{0, 1, 6, 0, ""}, {1, 1, 6, 1, "b"                       }, {2, 1, 6, 2, "bc"                         }, {3, 1, 6, 2, "bc"                          }, {4, 1, 6, 2, "bc"                          },
				{0, 2, 7, 0, ""}, {1, 2, 7, 1, "c"                       }, {2, 2, 7, 1, "c"                          }, {3, 2, 7, 1, "c"                           }, {4, 2, 7, 1, "c"                           },
				{0, 3, 8, 0, ""}, {1, 3, 8, 0, ""                        }, {2, 3, 8, 0, ""                           }, {3, 3, 8, 0, ""                            }, {4, 3, 8, 0, ""                            },
				{0, 4, 8, 0, ""}, {1, 4, 8, 0, ""                        }, {2, 4, 8, 0, ""                           }, {3, 4, 8, 0, ""                            }, {4, 4, 8, 0, ""                            }
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
	{
		oStr.Set("a\xFD\xBF\xBF\xBF\xBF\xBF""c"); // \xFD\xBF\xBF\xBF\xBF\xBF == U+7FFFFFFF
		acpl::String::Ref oRef(oStr);
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 1, "a"                       }, {2, 0, 0, 7, "a\xFD\xBF\xBF\xBF\xBF\xBF"  }, {3, 0, 0, 8, "a\xFD\xBF\xBF\xBF\xBF\xBF""c"}, {4, 0, 0, 8, "a\xFD\xBF\xBF\xBF\xBF\xBF""c"},
				{0, 1, 1, 0, ""}, {1, 1, 1, 6, "\xFD\xBF\xBF\xBF\xBF\xBF"}, {2, 1, 1, 7, "\xFD\xBF\xBF\xBF\xBF\xBF""c"}, {3, 1, 1, 7, "\xFD\xBF\xBF\xBF\xBF\xBF""c" }, {4, 1, 1, 7, "\xFD\xBF\xBF\xBF\xBF\xBF""c" },
				{0, 2, 7, 0, ""}, {1, 2, 7, 1, "c"                       }, {2, 2, 7, 1, "c"                          }, {3, 2, 7, 1, "c"                           }, {4, 2, 7, 1, "c"                           },
				{0, 3, 8, 0, ""}, {1, 3, 8, 0, ""                        }, {2, 3, 8, 0, ""                           }, {3, 3, 8, 0, ""                            }, {4, 3, 8, 0, ""                            },
				{0, 4, 8, 0, ""}, {1, 4, 8, 0, ""                        }, {2, 4, 8, 0, ""                           }, {3, 4, 8, 0, ""                            }, {4, 4, 8, 0, ""                            }
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
	{
		oStr.Set("ab\xFD\xBF\xBF\xBF\xBF\xBF"); // \xFD\xBF\xBF\xBF\xBF\xBF == U+7FFFFFFF
		acpl::String::Ref oRef(oStr);
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 1, "a"                       }, {2, 0, 0, 2, "ab"                       }, {3, 0, 0, 8, "ab\xFD\xBF\xBF\xBF\xBF\xBF"}, {4, 0, 0, 8, "ab\xFD\xBF\xBF\xBF\xBF\xBF"},
				{0, 1, 1, 0, ""}, {1, 1, 1, 1, "b"                       }, {2, 1, 1, 7, "b\xFD\xBF\xBF\xBF\xBF\xBF"}, {3, 1, 1, 7, "b\xFD\xBF\xBF\xBF\xBF\xBF" }, {4, 1, 1, 7, "b\xFD\xBF\xBF\xBF\xBF\xBF" },
				{0, 2, 2, 0, ""}, {1, 2, 2, 6, "\xFD\xBF\xBF\xBF\xBF\xBF"}, {2, 2, 2, 6, "\xFD\xBF\xBF\xBF\xBF\xBF" }, {3, 2, 2, 6, "\xFD\xBF\xBF\xBF\xBF\xBF"  }, {4, 2, 2, 6, "\xFD\xBF\xBF\xBF\xBF\xBF"  },
				{0, 3, 8, 0, ""}, {1, 3, 8, 0, ""                        }, {2, 3, 8, 0, ""                         }, {3, 3, 8, 0, ""                          }, {4, 3, 8, 0, ""                          },
				{0, 4, 8, 0, ""}, {1, 4, 8, 0, ""                        }, {2, 4, 8, 0, ""                         }, {3, 4, 8, 0, ""                          }, {4, 4, 8, 0, ""                          }
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
	
	return 0;
}

static int TestRefMeasureConvertSelf()
{
	PrintFn();
	
	
	acpl::String oStr;
	acpl::SizeT oRefOff;
	acpl::SizeT oEncByteSize;
	
	
	// Empty
	
	{
		acpl::String::Ref oRef(oStr);
		Test(oRef.GetType() == acpl::String::Ref::rtSelf);
		oRef.Measure(acpl::Num<acpl::SizeT>::Max(), 0, oRefOff, oEncByteSize);
		Test(oRefOff == 0);
		Test(oEncByteSize == 0);
	}
	
	
	Test(TestRefMeasureConvertSelf_1ByteSeq() == 0);
	Test(TestRefMeasureConvertSelf_2ByteSeq() == 0);
	Test(TestRefMeasureConvertSelf_3ByteSeq() == 0);
	Test(TestRefMeasureConvertSelf_4ByteSeq() == 0);
	Test(TestRefMeasureConvertSelf_5ByteSeq() == 0);
	Test(TestRefMeasureConvertSelf_6ByteSeq() == 0);
	
	
	return 0;
}

static int TestRefMeasureConvertUtf8_1ByteSeq()
{
	PrintFn();
	
	
	// Normal
	
	{
		acpl::String::Ref oRef("abc", false);
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 1, "a"}, {2, 0, 0, 2, "ab"}, {3, 0, 0, 3, "abc"}, {4, 0, 0, 3, "abc"},
				{0, 1, 1, 0, ""}, {1, 1, 1, 1, "b"}, {2, 1, 1, 2, "bc"}, {3, 1, 1, 2, "bc" }, {4, 1, 1, 2, "bc" },
				{0, 2, 2, 0, ""}, {1, 2, 2, 1, "c"}, {2, 2, 2, 1, "c" }, {3, 2, 2, 1, "c"  }, {4, 2, 2, 1, "c"  },
				{0, 3, 3, 0, ""}, {1, 3, 3, 0, "" }, {2, 3, 3, 0, ""  }, {3, 3, 3, 0, ""   }, {4, 3, 3, 0, ""   },
				{0, 4, 3, 0, ""}, {1, 4, 3, 0, "" }, {2, 4, 3, 0, ""  }, {3, 4, 3, 0, ""   }, {4, 4, 3, 0, ""   }
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
	
	// Invalid sequence
	
	{
		acpl::String::Ref oRef("\xFE""bc", false);
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 1, "?"}, {2, 0, 0, 2, "?b"}, {3, 0, 0, 3, "?bc"}, {4, 0, 0, 3, "?bc"},
				{0, 1, 1, 0, ""}, {1, 1, 1, 1, "b"}, {2, 1, 1, 2, "bc"}, {3, 1, 1, 2, "bc" }, {4, 1, 1, 2, "bc" },
				{0, 2, 2, 0, ""}, {1, 2, 2, 1, "c"}, {2, 2, 2, 1, "c" }, {3, 2, 2, 1, "c"  }, {4, 2, 2, 1, "c"  },
				{0, 3, 3, 0, ""}, {1, 3, 3, 0, "" }, {2, 3, 3, 0, ""  }, {3, 3, 3, 0, ""   }, {4, 3, 3, 0, ""   },
				{0, 4, 3, 0, ""}, {1, 4, 3, 0, "" }, {2, 4, 3, 0, ""  }, {3, 4, 3, 0, ""   }, {4, 4, 3, 0, ""   }
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
	{
		acpl::String::Ref oRef("a\xFE""c", false);
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 1, "a"}, {2, 0, 0, 2, "a?"}, {3, 0, 0, 3, "a?c"}, {4, 0, 0, 3, "a?c"},
				{0, 1, 1, 0, ""}, {1, 1, 1, 1, "?"}, {2, 1, 1, 2, "?c"}, {3, 1, 1, 2, "?c" }, {4, 1, 1, 2, "?c" },
				{0, 2, 2, 0, ""}, {1, 2, 2, 1, "c"}, {2, 2, 2, 1, "c" }, {3, 2, 2, 1, "c"  }, {4, 2, 2, 1, "c"  },
				{0, 3, 3, 0, ""}, {1, 3, 3, 0, "" }, {2, 3, 3, 0, ""  }, {3, 3, 3, 0, ""   }, {4, 3, 3, 0, ""   },
				{0, 4, 3, 0, ""}, {1, 4, 3, 0, "" }, {2, 4, 3, 0, ""  }, {3, 4, 3, 0, ""   }, {4, 4, 3, 0, ""   }
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
	{
		acpl::String::Ref oRef("ab\xFE", false);
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 1, "a"}, {2, 0, 0, 2, "ab"}, {3, 0, 0, 3, "ab?"}, {4, 0, 0, 3, "ab?"},
				{0, 1, 1, 0, ""}, {1, 1, 1, 1, "b"}, {2, 1, 1, 2, "b?"}, {3, 1, 1, 2, "b?" }, {4, 1, 1, 2, "b?" },
				{0, 2, 2, 0, ""}, {1, 2, 2, 1, "?"}, {2, 2, 2, 1, "?" }, {3, 2, 2, 1, "?"  }, {4, 2, 2, 1, "?"  },
				{0, 3, 3, 0, ""}, {1, 3, 3, 0, "" }, {2, 3, 3, 0, ""  }, {3, 3, 3, 0, ""   }, {4, 3, 3, 0, ""   },
				{0, 4, 3, 0, ""}, {1, 4, 3, 0, "" }, {2, 4, 3, 0, ""  }, {3, 4, 3, 0, ""   }, {4, 4, 3, 0, ""   }
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
	
	return 0;
}

static int TestRefMeasureConvertUtf8_2ByteSeq()
{
	PrintFn();
	
	
	// Normal
	
	{
		acpl::String::Ref oRef("\xDF\xBF""bc", false); // \xDF\xBF == U+7FF
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 2, "\xDF\xBF"}, {2, 0, 0, 3, "\xDF\xBF""b"}, {3, 0, 0, 4, "\xDF\xBF""bc"}, {4, 0, 0, 4, "\xDF\xBF""bc"},
				{0, 1, 2, 0, ""}, {1, 1, 2, 1, "b"       }, {2, 1, 2, 2, "bc"         }, {3, 1, 2, 2, "bc"          }, {4, 1, 2, 2, "bc"          },
				{0, 2, 3, 0, ""}, {1, 2, 3, 1, "c"       }, {2, 2, 3, 1, "c"          }, {3, 2, 3, 1, "c"           }, {4, 2, 3, 1, "c"           },
				{0, 3, 4, 0, ""}, {1, 3, 4, 0, ""        }, {2, 3, 4, 0, ""           }, {3, 3, 4, 0, ""            }, {4, 3, 4, 0, ""            },
				{0, 4, 4, 0, ""}, {1, 4, 4, 0, ""        }, {2, 4, 4, 0, ""           }, {3, 4, 4, 0, ""            }, {4, 4, 4, 0, ""            }
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
	{
		acpl::String::Ref oRef("a\xDF\xBF""c", false); // \xDF\xBF == U+7FF
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 1, "a"       }, {2, 0, 0, 3, "a\xDF\xBF"  }, {3, 0, 0, 4, "a\xDF\xBF""c"}, {4, 0, 0, 4, "a\xDF\xBF""c"},
				{0, 1, 1, 0, ""}, {1, 1, 1, 2, "\xDF\xBF"}, {2, 1, 1, 3, "\xDF\xBF""c"}, {3, 1, 1, 3, "\xDF\xBF""c" }, {4, 1, 1, 3, "\xDF\xBF""c" },
				{0, 2, 3, 0, ""}, {1, 2, 3, 1, "c"       }, {2, 2, 3, 1, "c"          }, {3, 2, 3, 1, "c"           }, {4, 2, 3, 1, "c"           },
				{0, 3, 4, 0, ""}, {1, 3, 4, 0, ""        }, {2, 3, 4, 0, ""           }, {3, 3, 4, 0, ""            }, {4, 3, 4, 0, ""            },
				{0, 4, 4, 0, ""}, {1, 4, 4, 0, ""        }, {2, 4, 4, 0, ""           }, {3, 4, 4, 0, ""            }, {4, 4, 4, 0, ""            }
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
	{
		acpl::String::Ref oRef("ab\xDF\xBF", false); // \xDF\xBF == U+7FF
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 1, "a"       }, {2, 0, 0, 2, "ab"       }, {3, 0, 0, 4, "ab\xDF\xBF"}, {4, 0, 0, 4, "ab\xDF\xBF"},
				{0, 1, 1, 0, ""}, {1, 1, 1, 1, "b"       }, {2, 1, 1, 3, "b\xDF\xBF"}, {3, 1, 1, 3, "b\xDF\xBF" }, {4, 1, 1, 3, "b\xDF\xBF" },
				{0, 2, 2, 0, ""}, {1, 2, 2, 2, "\xDF\xBF"}, {2, 2, 2, 2, "\xDF\xBF" }, {3, 2, 2, 2, "\xDF\xBF"  }, {4, 2, 2, 2, "\xDF\xBF"  },
				{0, 3, 4, 0, ""}, {1, 3, 4, 0, ""        }, {2, 3, 4, 0, ""         }, {3, 3, 4, 0, ""          }, {4, 3, 4, 0, ""          },
				{0, 4, 4, 0, ""}, {1, 4, 4, 0, ""        }, {2, 4, 4, 0, ""         }, {3, 4, 4, 0, ""          }, {4, 4, 4, 0, ""          }
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
	
	// Abnormal
	
	{
		acpl::String::Ref oRef("\xC1\xA1""bc", false); // \xC1\xA1 == 'a'
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 1, "a"}, {2, 0, 0, 2, "ab"}, {3, 0, 0, 3, "abc"}, {4, 0, 0, 3, "abc"},
				{0, 1, 2, 0, ""}, {1, 1, 2, 1, "b"}, {2, 1, 2, 2, "bc"}, {3, 1, 2, 2, "bc" }, {4, 1, 2, 2, "bc" },
				{0, 2, 3, 0, ""}, {1, 2, 3, 1, "c"}, {2, 2, 3, 1, "c" }, {3, 2, 3, 1, "c"  }, {4, 2, 3, 1, "c"  },
				{0, 3, 4, 0, ""}, {1, 3, 4, 0, "" }, {2, 3, 4, 0, ""  }, {3, 3, 4, 0, ""   }, {4, 3, 4, 0, ""   },
				{0, 4, 4, 0, ""}, {1, 4, 4, 0, "" }, {2, 4, 4, 0, ""  }, {3, 4, 4, 0, ""   }, {4, 4, 4, 0, ""   }
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
	{
		acpl::String::Ref oRef("a\xC1\xA2""c", false); // \xC1\xA2 == 'b'
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 1, "a"}, {2, 0, 0, 2, "ab"}, {3, 0, 0, 3, "abc"}, {4, 0, 0, 3, "abc"},
				{0, 1, 1, 0, ""}, {1, 1, 1, 1, "b"}, {2, 1, 1, 2, "bc"}, {3, 1, 1, 2, "bc" }, {4, 1, 1, 2, "bc" },
				{0, 2, 3, 0, ""}, {1, 2, 3, 1, "c"}, {2, 2, 3, 1, "c" }, {3, 2, 3, 1, "c"  }, {4, 2, 3, 1, "c"  },
				{0, 3, 4, 0, ""}, {1, 3, 4, 0, "" }, {2, 3, 4, 0, ""  }, {3, 3, 4, 0, ""   }, {4, 3, 4, 0, ""   },
				{0, 4, 4, 0, ""}, {1, 4, 4, 0, "" }, {2, 4, 4, 0, ""  }, {3, 4, 4, 0, ""   }, {4, 4, 4, 0, ""   }
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
	{
		acpl::String::Ref oRef("ab\xC1\xA3", false); // \xC1\xA3 == 'c'
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 1, "a"}, {2, 0, 0, 2, "ab"}, {3, 0, 0, 3, "abc"}, {4, 0, 0, 3, "abc"},
				{0, 1, 1, 0, ""}, {1, 1, 1, 1, "b"}, {2, 1, 1, 2, "bc"}, {3, 1, 1, 2, "bc" }, {4, 1, 1, 2, "bc" },
				{0, 2, 2, 0, ""}, {1, 2, 2, 1, "c"}, {2, 2, 2, 1, "c" }, {3, 2, 2, 1, "c"  }, {4, 2, 2, 1, "c"  },
				{0, 3, 4, 0, ""}, {1, 3, 4, 0, "" }, {2, 3, 4, 0, ""  }, {3, 3, 4, 0, ""   }, {4, 3, 4, 0, ""   },
				{0, 4, 4, 0, ""}, {1, 4, 4, 0, "" }, {2, 4, 4, 0, ""  }, {3, 4, 4, 0, ""   }, {4, 4, 4, 0, ""   }
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
	
	// Invalid sequence
	
	{
		acpl::String::Ref oRef("\xC0""bc", false);
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 1, "?"}, {2, 0, 0, 2, "?b"}, {3, 0, 0, 3, "?bc"}, {4, 0, 0, 3, "?bc"},
				{0, 1, 1, 0, ""}, {1, 1, 1, 1, "b"}, {2, 1, 1, 2, "bc"}, {3, 1, 1, 2, "bc" }, {4, 1, 1, 2, "bc" },
				{0, 2, 2, 0, ""}, {1, 2, 2, 1, "c"}, {2, 2, 2, 1, "c" }, {3, 2, 2, 1, "c"  }, {4, 2, 2, 1, "c"  },
				{0, 3, 3, 0, ""}, {1, 3, 3, 0, "" }, {2, 3, 3, 0, ""  }, {3, 3, 3, 0, ""   }, {4, 3, 3, 0, ""   },
				{0, 4, 3, 0, ""}, {1, 4, 3, 0, "" }, {2, 4, 3, 0, ""  }, {3, 4, 3, 0, ""   }, {4, 4, 3, 0, ""   }
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
	{
		acpl::String::Ref oRef("a\xC0""c", false);
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 1, "a"}, {2, 0, 0, 2, "a?"}, {3, 0, 0, 3, "a?c"}, {4, 0, 0, 3, "a?c"},
				{0, 1, 1, 0, ""}, {1, 1, 1, 1, "?"}, {2, 1, 1, 2, "?c"}, {3, 1, 1, 2, "?c" }, {4, 1, 1, 2, "?c" },
				{0, 2, 2, 0, ""}, {1, 2, 2, 1, "c"}, {2, 2, 2, 1, "c" }, {3, 2, 2, 1, "c"  }, {4, 2, 2, 1, "c"  },
				{0, 3, 3, 0, ""}, {1, 3, 3, 0, "" }, {2, 3, 3, 0, ""  }, {3, 3, 3, 0, ""   }, {4, 3, 3, 0, ""   },
				{0, 4, 3, 0, ""}, {1, 4, 3, 0, "" }, {2, 4, 3, 0, ""  }, {3, 4, 3, 0, ""   }, {4, 4, 3, 0, ""   }
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
	{
		acpl::String::Ref oRef("ab\xC0", false);
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 1, "a"}, {2, 0, 0, 2, "ab"}, {3, 0, 0, 3, "ab?"}, {4, 0, 0, 3, "ab?"},
				{0, 1, 1, 0, ""}, {1, 1, 1, 1, "b"}, {2, 1, 1, 2, "b?"}, {3, 1, 1, 2, "b?" }, {4, 1, 1, 2, "b?" },
				{0, 2, 2, 0, ""}, {1, 2, 2, 1, "?"}, {2, 2, 2, 1, "?" }, {3, 2, 2, 1, "?"  }, {4, 2, 2, 1, "?"  },
				{0, 3, 3, 0, ""}, {1, 3, 3, 0, "" }, {2, 3, 3, 0, ""  }, {3, 3, 3, 0, ""   }, {4, 3, 3, 0, ""   },
				{0, 4, 3, 0, ""}, {1, 4, 3, 0, "" }, {2, 4, 3, 0, ""  }, {3, 4, 3, 0, ""   }, {4, 4, 3, 0, ""   }
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
	
	// NULL terminator
	
	{
		acpl::String::Ref oRef("\xC0\x80""bc", false);
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 0, ""}, {2, 0, 0, 0, ""}, {3, 0, 0, 0, ""}, {4, 0, 0, 0, ""},
				{0, 1, 0, 0, ""}, {1, 1, 0, 0, ""}, {2, 1, 0, 0, ""}, {3, 1, 0, 0, ""}, {4, 1, 0, 0, ""},
				{0, 2, 0, 0, ""}, {1, 2, 0, 0, ""}, {2, 2, 0, 0, ""}, {3, 2, 0, 0, ""}, {4, 2, 0, 0, ""},
				{0, 3, 0, 0, ""}, {1, 3, 0, 0, ""}, {2, 3, 0, 0, ""}, {3, 3, 0, 0, ""}, {4, 3, 0, 0, ""},
				{0, 4, 0, 0, ""}, {1, 4, 0, 0, ""}, {2, 4, 0, 0, ""}, {3, 4, 0, 0, ""}, {4, 4, 0, 0, ""}
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
	{
		acpl::String::Ref oRef("a\xC0\x80""c", false);
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 1, "a"}, {2, 0, 0, 1, "a"}, {3, 0, 0, 1, "a"}, {4, 0, 0, 1, "a"},
				{0, 1, 1, 0, ""}, {1, 1, 1, 0, "" }, {2, 1, 1, 0, "" }, {3, 1, 1, 0, "" }, {4, 1, 1, 0, "" },
				{0, 2, 1, 0, ""}, {1, 2, 1, 0, "" }, {2, 2, 1, 0, "" }, {3, 2, 1, 0, "" }, {4, 2, 1, 0, "" },
				{0, 3, 1, 0, ""}, {1, 3, 1, 0, "" }, {2, 3, 1, 0, "" }, {3, 3, 1, 0, "" }, {4, 3, 1, 0, "" },
				{0, 4, 1, 0, ""}, {1, 4, 1, 0, "" }, {2, 4, 1, 0, "" }, {3, 4, 1, 0, "" }, {4, 4, 1, 0, "" }
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
	{
		acpl::String::Ref oRef("ab\xC0\x80", false);
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 1, "a"}, {2, 0, 0, 2, "ab"}, {3, 0, 0, 2, "ab"}, {4, 0, 0, 2, "ab"},
				{0, 1, 1, 0, ""}, {1, 1, 1, 1, "b"}, {2, 1, 1, 1, "b" }, {3, 1, 1, 1, "b" }, {4, 1, 1, 1, "b" },
				{0, 2, 2, 0, ""}, {1, 2, 2, 0, "" }, {2, 2, 2, 0, ""  }, {3, 2, 2, 0, ""  }, {4, 2, 2, 0, ""  },
				{0, 3, 2, 0, ""}, {1, 3, 2, 0, "" }, {2, 3, 2, 0, ""  }, {3, 3, 2, 0, ""  }, {4, 3, 2, 0, ""  },
				{0, 4, 2, 0, ""}, {1, 4, 2, 0, "" }, {2, 4, 2, 0, ""  }, {3, 4, 2, 0, ""  }, {4, 4, 2, 0, ""  }
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
	
	return 0;
}

static int TestRefMeasureConvertUtf8_3ByteSeq()
{
	PrintFn();
	
	
	// Normal
	
	{
		acpl::String::Ref oRef("\xEF\xBF\xBF""bc", false); // \xEF\xBF\xBF == U+FFFF
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 3, "\xEF\xBF\xBF"}, {2, 0, 0, 4, "\xEF\xBF\xBF""b"}, {3, 0, 0, 5, "\xEF\xBF\xBF""bc"}, {4, 0, 0, 5, "\xEF\xBF\xBF""bc"},
				{0, 1, 3, 0, ""}, {1, 1, 3, 1, "b"           }, {2, 1, 3, 2, "bc"             }, {3, 1, 3, 2, "bc"              }, {4, 1, 3, 2, "bc"              },
				{0, 2, 4, 0, ""}, {1, 2, 4, 1, "c"           }, {2, 2, 4, 1, "c"              }, {3, 2, 4, 1, "c"               }, {4, 2, 4, 1, "c"               },
				{0, 3, 5, 0, ""}, {1, 3, 5, 0, ""            }, {2, 3, 5, 0, ""               }, {3, 3, 5, 0, ""                }, {4, 3, 5, 0, ""                },
				{0, 4, 5, 0, ""}, {1, 4, 5, 0, ""            }, {2, 4, 5, 0, ""               }, {3, 4, 5, 0, ""                }, {4, 4, 5, 0, ""                }
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
	{
		acpl::String::Ref oRef("a\xEF\xBF\xBF""c", false); // \xEF\xBF\xBF == U+FFFF
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 1, "a"           }, {2, 0, 0, 4, "a\xEF\xBF\xBF"  }, {3, 0, 0, 5, "a\xEF\xBF\xBF""c"}, {4, 0, 0, 5, "a\xEF\xBF\xBF""c"},
				{0, 1, 1, 0, ""}, {1, 1, 1, 3, "\xEF\xBF\xBF"}, {2, 1, 1, 4, "\xEF\xBF\xBF""c"}, {3, 1, 1, 4, "\xEF\xBF\xBF""c" }, {4, 1, 1, 4, "\xEF\xBF\xBF""c" },
				{0, 2, 4, 0, ""}, {1, 2, 4, 1, "c"           }, {2, 2, 4, 1, "c"              }, {3, 2, 4, 1, "c"               }, {4, 2, 4, 1, "c"               },
				{0, 3, 5, 0, ""}, {1, 3, 5, 0, ""            }, {2, 3, 5, 0, ""               }, {3, 3, 5, 0, ""                }, {4, 3, 5, 0, ""                },
				{0, 4, 5, 0, ""}, {1, 4, 5, 0, ""            }, {2, 4, 5, 0, ""               }, {3, 4, 5, 0, ""                }, {4, 4, 5, 0, ""                }
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
	{
		acpl::String::Ref oRef("ab\xEF\xBF\xBF", false); // \xEF\xBF\xBF == U+FFFF
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 1, "a"           }, {2, 0, 0, 2, "ab"           }, {3, 0, 0, 5, "ab\xEF\xBF\xBF"}, {4, 0, 0, 5, "ab\xEF\xBF\xBF"},
				{0, 1, 1, 0, ""}, {1, 1, 1, 1, "b"           }, {2, 1, 1, 4, "b\xEF\xBF\xBF"}, {3, 1, 1, 4, "b\xEF\xBF\xBF" }, {4, 1, 1, 4, "b\xEF\xBF\xBF" },
				{0, 2, 2, 0, ""}, {1, 2, 2, 3, "\xEF\xBF\xBF"}, {2, 2, 2, 3, "\xEF\xBF\xBF" }, {3, 2, 2, 3, "\xEF\xBF\xBF"  }, {4, 2, 2, 3, "\xEF\xBF\xBF"  },
				{0, 3, 5, 0, ""}, {1, 3, 5, 0, ""            }, {2, 3, 5, 0, ""             }, {3, 3, 5, 0, ""              }, {4, 3, 5, 0, ""              },
				{0, 4, 5, 0, ""}, {1, 4, 5, 0, ""            }, {2, 4, 5, 0, ""             }, {3, 4, 5, 0, ""              }, {4, 4, 5, 0, ""              }
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
	
	// Abnormal
	
	{
		acpl::String::Ref oRef("\xE0\x81\xA1""bc", false); // \xE0\x81\xA1 == 'a'
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 1, "a"}, {2, 0, 0, 2, "ab"}, {3, 0, 0, 3, "abc"}, {4, 0, 0, 3, "abc"},
				{0, 1, 3, 0, ""}, {1, 1, 3, 1, "b"}, {2, 1, 3, 2, "bc"}, {3, 1, 3, 2, "bc" }, {4, 1, 3, 2, "bc" },
				{0, 2, 4, 0, ""}, {1, 2, 4, 1, "c"}, {2, 2, 4, 1, "c" }, {3, 2, 4, 1, "c"  }, {4, 2, 4, 1, "c"  },
				{0, 3, 5, 0, ""}, {1, 3, 5, 0, "" }, {2, 3, 5, 0, ""  }, {3, 3, 5, 0, ""   }, {4, 3, 5, 0, ""   },
				{0, 4, 5, 0, ""}, {1, 4, 5, 0, "" }, {2, 4, 5, 0, ""  }, {3, 4, 5, 0, ""   }, {4, 4, 5, 0, ""   }
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
	{
		acpl::String::Ref oRef("a\xE0\x81\xA2""c", false); // \xE0\x81\xA2 == 'b'
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 1, "a"}, {2, 0, 0, 2, "ab"}, {3, 0, 0, 3, "abc"}, {4, 0, 0, 3, "abc"},
				{0, 1, 1, 0, ""}, {1, 1, 1, 1, "b"}, {2, 1, 1, 2, "bc"}, {3, 1, 1, 2, "bc" }, {4, 1, 1, 2, "bc" },
				{0, 2, 4, 0, ""}, {1, 2, 4, 1, "c"}, {2, 2, 4, 1, "c" }, {3, 2, 4, 1, "c"  }, {4, 2, 4, 1, "c"  },
				{0, 3, 5, 0, ""}, {1, 3, 5, 0, "" }, {2, 3, 5, 0, ""  }, {3, 3, 5, 0, ""   }, {4, 3, 5, 0, ""   },
				{0, 4, 5, 0, ""}, {1, 4, 5, 0, "" }, {2, 4, 5, 0, ""  }, {3, 4, 5, 0, ""   }, {4, 4, 5, 0, ""   }
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
	{
		acpl::String::Ref oRef("ab\xE0\x81\xA3", false); // \xE0\x81\xA3 == 'c'
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 1, "a"}, {2, 0, 0, 2, "ab"}, {3, 0, 0, 3, "abc"}, {4, 0, 0, 3, "abc"},
				{0, 1, 1, 0, ""}, {1, 1, 1, 1, "b"}, {2, 1, 1, 2, "bc"}, {3, 1, 1, 2, "bc" }, {4, 1, 1, 2, "bc" },
				{0, 2, 2, 0, ""}, {1, 2, 2, 1, "c"}, {2, 2, 2, 1, "c" }, {3, 2, 2, 1, "c"  }, {4, 2, 2, 1, "c"  },
				{0, 3, 5, 0, ""}, {1, 3, 5, 0, "" }, {2, 3, 5, 0, ""  }, {3, 3, 5, 0, ""   }, {4, 3, 5, 0, ""   },
				{0, 4, 5, 0, ""}, {1, 4, 5, 0, "" }, {2, 4, 5, 0, ""  }, {3, 4, 5, 0, ""   }, {4, 4, 5, 0, ""   }
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
	
	// Invalid sequence
	
	{
		acpl::String::Ref oRef("\xE0""bc", false);
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 1, "?"}, {2, 0, 0, 2, "?b"}, {3, 0, 0, 3, "?bc"}, {4, 0, 0, 3, "?bc"},
				{0, 1, 1, 0, ""}, {1, 1, 1, 1, "b"}, {2, 1, 1, 2, "bc"}, {3, 1, 1, 2, "bc" }, {4, 1, 1, 2, "bc" },
				{0, 2, 2, 0, ""}, {1, 2, 2, 1, "c"}, {2, 2, 2, 1, "c" }, {3, 2, 2, 1, "c"  }, {4, 2, 2, 1, "c"  },
				{0, 3, 3, 0, ""}, {1, 3, 3, 0, "" }, {2, 3, 3, 0, ""  }, {3, 3, 3, 0, ""   }, {4, 3, 3, 0, ""   },
				{0, 4, 3, 0, ""}, {1, 4, 3, 0, "" }, {2, 4, 3, 0, ""  }, {3, 4, 3, 0, ""   }, {4, 4, 3, 0, ""   }
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
	{
		acpl::String::Ref oRef("a\xE0""c", false);
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 1, "a"}, {2, 0, 0, 2, "a?"}, {3, 0, 0, 3, "a?c"}, {4, 0, 0, 3, "a?c"},
				{0, 1, 1, 0, ""}, {1, 1, 1, 1, "?"}, {2, 1, 1, 2, "?c"}, {3, 1, 1, 2, "?c" }, {4, 1, 1, 2, "?c" },
				{0, 2, 2, 0, ""}, {1, 2, 2, 1, "c"}, {2, 2, 2, 1, "c" }, {3, 2, 2, 1, "c"  }, {4, 2, 2, 1, "c"  },
				{0, 3, 3, 0, ""}, {1, 3, 3, 0, "" }, {2, 3, 3, 0, ""  }, {3, 3, 3, 0, ""   }, {4, 3, 3, 0, ""   },
				{0, 4, 3, 0, ""}, {1, 4, 3, 0, "" }, {2, 4, 3, 0, ""  }, {3, 4, 3, 0, ""   }, {4, 4, 3, 0, ""   }
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
	{
		acpl::String::Ref oRef("ab\xE0", false);
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 1, "a"}, {2, 0, 0, 2, "ab"}, {3, 0, 0, 3, "ab?"}, {4, 0, 0, 3, "ab?"},
				{0, 1, 1, 0, ""}, {1, 1, 1, 1, "b"}, {2, 1, 1, 2, "b?"}, {3, 1, 1, 2, "b?" }, {4, 1, 1, 2, "b?" },
				{0, 2, 2, 0, ""}, {1, 2, 2, 1, "?"}, {2, 2, 2, 1, "?" }, {3, 2, 2, 1, "?"  }, {4, 2, 2, 1, "?"  },
				{0, 3, 3, 0, ""}, {1, 3, 3, 0, "" }, {2, 3, 3, 0, ""  }, {3, 3, 3, 0, ""   }, {4, 3, 3, 0, ""   },
				{0, 4, 3, 0, ""}, {1, 4, 3, 0, "" }, {2, 4, 3, 0, ""  }, {3, 4, 3, 0, ""   }, {4, 4, 3, 0, ""   }
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
	{
		acpl::String::Ref oRef("\xE0\x80""bc", false);
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 1, "?"}, {2, 0, 0, 2, "??"}, {3, 0, 0, 3, "??b"}, {4, 0, 0, 4, "??bc"}, {5, 0, 0, 4, "??bc"},
				{0, 1, 1, 0, ""}, {1, 1, 1, 1, "?"}, {2, 1, 1, 2, "?b"}, {3, 1, 1, 3, "?bc"}, {4, 1, 1, 3, "?bc" }, {5, 1, 1, 3, "?bc" },
				{0, 2, 2, 0, ""}, {1, 2, 2, 1, "b"}, {2, 2, 2, 2, "bc"}, {3, 2, 2, 2, "bc" }, {4, 2, 2, 2, "bc"  }, {5, 2, 2, 2, "bc"  },
				{0, 3, 3, 0, ""}, {1, 3, 3, 1, "c"}, {2, 3, 3, 1, "c" }, {3, 3, 3, 1, "c"  }, {4, 3, 3, 1, "c"   }, {5, 3, 3, 1, "c"   },
				{0, 4, 4, 0, ""}, {1, 4, 4, 0, "" }, {2, 4, 4, 0, ""  }, {3, 4, 4, 0, ""   }, {4, 4, 4, 0, ""    }, {5, 4, 4, 0, ""    }
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
	{
		acpl::String::Ref oRef("a\xE0\x80""c", false);
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 1, "a"}, {2, 0, 0, 2, "a?"}, {3, 0, 0, 3, "a??"}, {4, 0, 0, 4, "a??c"}, {5, 0, 0, 4, "a??c"},
				{0, 1, 1, 0, ""}, {1, 1, 1, 1, "?"}, {2, 1, 1, 2, "??"}, {3, 1, 1, 3, "??c"}, {4, 1, 1, 3, "??c" }, {5, 1, 1, 3, "??c" },
				{0, 2, 2, 0, ""}, {1, 2, 2, 1, "?"}, {2, 2, 2, 2, "?c"}, {3, 2, 2, 2, "?c" }, {4, 2, 2, 2, "?c"  }, {5, 2, 2, 2, "?c"  },
				{0, 3, 3, 0, ""}, {1, 3, 3, 1, "c"}, {2, 3, 3, 1, "c" }, {3, 3, 3, 1, "c"  }, {4, 3, 3, 1, "c"   }, {5, 3, 3, 1, "c"   },
				{0, 4, 4, 0, ""}, {1, 4, 4, 0, "" }, {2, 4, 4, 0, ""  }, {3, 4, 4, 0, ""   }, {4, 4, 4, 0, ""    }, {5, 4, 4, 0, ""    }
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
	{
		acpl::String::Ref oRef("ab\xE0\x80", false);
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 1, "a"}, {2, 0, 0, 2, "ab"}, {3, 0, 0, 3, "ab?"}, {4, 0, 0, 4, "ab??"}, {5, 0, 0, 4, "ab??"},
				{0, 1, 1, 0, ""}, {1, 1, 1, 1, "b"}, {2, 1, 1, 2, "b?"}, {3, 1, 1, 3, "b??"}, {4, 1, 1, 3, "b??" }, {5, 1, 1, 3, "b??" },
				{0, 2, 2, 0, ""}, {1, 2, 2, 1, "?"}, {2, 2, 2, 2, "??"}, {3, 2, 2, 2, "??" }, {4, 2, 2, 2, "??"  }, {5, 2, 2, 2, "??"  },
				{0, 3, 3, 0, ""}, {1, 3, 3, 1, "?"}, {2, 3, 3, 1, "?" }, {3, 3, 3, 1, "?"  }, {4, 3, 3, 1, "?"   }, {5, 3, 3, 1, "?"   },
				{0, 4, 4, 0, ""}, {1, 4, 4, 0, "" }, {2, 4, 4, 0, ""  }, {3, 4, 4, 0, ""   }, {4, 4, 4, 0, ""    }, {5, 4, 4, 0, ""    }
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
	
	// NULL terminator
	
	{
		acpl::String::Ref oRef("\xE0\x80\x80""bc", false);
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 0, ""}, {2, 0, 0, 0, ""}, {3, 0, 0, 0, ""}, {4, 0, 0, 0, ""},
				{0, 1, 0, 0, ""}, {1, 1, 0, 0, ""}, {2, 1, 0, 0, ""}, {3, 1, 0, 0, ""}, {4, 1, 0, 0, ""},
				{0, 2, 0, 0, ""}, {1, 2, 0, 0, ""}, {2, 2, 0, 0, ""}, {3, 2, 0, 0, ""}, {4, 2, 0, 0, ""},
				{0, 3, 0, 0, ""}, {1, 3, 0, 0, ""}, {2, 3, 0, 0, ""}, {3, 3, 0, 0, ""}, {4, 3, 0, 0, ""},
				{0, 4, 0, 0, ""}, {1, 4, 0, 0, ""}, {2, 4, 0, 0, ""}, {3, 4, 0, 0, ""}, {4, 4, 0, 0, ""}
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
	{
		acpl::String::Ref oRef("a\xE0\x80\x80""c", false);
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 1, "a"}, {2, 0, 0, 1, "a"}, {3, 0, 0, 1, "a"}, {4, 0, 0, 1, "a"},
				{0, 1, 1, 0, ""}, {1, 1, 1, 0, "" }, {2, 1, 1, 0, "" }, {3, 1, 1, 0, "" }, {4, 1, 1, 0, "" },
				{0, 2, 1, 0, ""}, {1, 2, 1, 0, "" }, {2, 2, 1, 0, "" }, {3, 2, 1, 0, "" }, {4, 2, 1, 0, "" },
				{0, 3, 1, 0, ""}, {1, 3, 1, 0, "" }, {2, 3, 1, 0, "" }, {3, 3, 1, 0, "" }, {4, 3, 1, 0, "" },
				{0, 4, 1, 0, ""}, {1, 4, 1, 0, "" }, {2, 4, 1, 0, "" }, {3, 4, 1, 0, "" }, {4, 4, 1, 0, "" }
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
	{
		acpl::String::Ref oRef("ab\xE0\x80\x80", false);
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 1, "a"}, {2, 0, 0, 2, "ab"}, {3, 0, 0, 2, "ab"}, {4, 0, 0, 2, "ab"},
				{0, 1, 1, 0, ""}, {1, 1, 1, 1, "b"}, {2, 1, 1, 1, "b" }, {3, 1, 1, 1, "b" }, {4, 1, 1, 1, "b" },
				{0, 2, 2, 0, ""}, {1, 2, 2, 0, "" }, {2, 2, 2, 0, ""  }, {3, 2, 2, 0, ""  }, {4, 2, 2, 0, ""  },
				{0, 3, 2, 0, ""}, {1, 3, 2, 0, "" }, {2, 3, 2, 0, ""  }, {3, 3, 2, 0, ""  }, {4, 3, 2, 0, ""  },
				{0, 4, 2, 0, ""}, {1, 4, 2, 0, "" }, {2, 4, 2, 0, ""  }, {3, 4, 2, 0, ""  }, {4, 4, 2, 0, ""  }
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
	
	return 0;
}

static int TestRefMeasureConvertUtf8_4ByteSeq()
{
	PrintFn();
	
	
	// Normal
	
	{
		acpl::String::Ref oRef("\xF7\xBF\xBF\xBF""bc", false); // \xF7\xBF\xBF\xBF == U+1FFFFF
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 4, "\xF7\xBF\xBF\xBF"}, {2, 0, 0, 5, "\xF7\xBF\xBF\xBF""b"}, {3, 0, 0, 6, "\xF7\xBF\xBF\xBF""bc"}, {4, 0, 0, 6, "\xF7\xBF\xBF\xBF""bc"},
				{0, 1, 4, 0, ""}, {1, 1, 4, 1, "b"               }, {2, 1, 4, 2, "bc"                 }, {3, 1, 4, 2, "bc"                  }, {4, 1, 4, 2, "bc"                  },
				{0, 2, 5, 0, ""}, {1, 2, 5, 1, "c"               }, {2, 2, 5, 1, "c"                  }, {3, 2, 5, 1, "c"                   }, {4, 2, 5, 1, "c"                   },
				{0, 3, 6, 0, ""}, {1, 3, 6, 0, ""                }, {2, 3, 6, 0, ""                   }, {3, 3, 6, 0, ""                    }, {4, 3, 6, 0, ""                    },
				{0, 4, 6, 0, ""}, {1, 4, 6, 0, ""                }, {2, 4, 6, 0, ""                   }, {3, 4, 6, 0, ""                    }, {4, 4, 6, 0, ""                    }
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
	{
		acpl::String::Ref oRef("a\xF7\xBF\xBF\xBF""c", false); // \xF7\xBF\xBF\xBF == U+1FFFFF
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 1, "a"               }, {2, 0, 0, 5, "a\xF7\xBF\xBF\xBF"  }, {3, 0, 0, 6, "a\xF7\xBF\xBF\xBF""c"}, {4, 0, 0, 6, "a\xF7\xBF\xBF\xBF""c"},
				{0, 1, 1, 0, ""}, {1, 1, 1, 4, "\xF7\xBF\xBF\xBF"}, {2, 1, 1, 5, "\xF7\xBF\xBF\xBF""c"}, {3, 1, 1, 5, "\xF7\xBF\xBF\xBF""c" }, {4, 1, 1, 5, "\xF7\xBF\xBF\xBF""c" },
				{0, 2, 5, 0, ""}, {1, 2, 5, 1, "c"               }, {2, 2, 5, 1, "c"                  }, {3, 2, 5, 1, "c"                   }, {4, 2, 5, 1, "c"                   },
				{0, 3, 6, 0, ""}, {1, 3, 6, 0, ""                }, {2, 3, 6, 0, ""                   }, {3, 3, 6, 0, ""                    }, {4, 3, 6, 0, ""                    },
				{0, 4, 6, 0, ""}, {1, 4, 6, 0, ""                }, {2, 4, 6, 0, ""                   }, {3, 4, 6, 0, ""                    }, {4, 4, 6, 0, ""                    }
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
	{
		acpl::String::Ref oRef("ab\xF7\xBF\xBF\xBF", false); // \xF7\xBF\xBF\xBF == U+1FFFFF
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 1, "a"               }, {2, 0, 0, 2, "ab"               }, {3, 0, 0, 6, "ab\xF7\xBF\xBF\xBF"}, {4, 0, 0, 6, "ab\xF7\xBF\xBF\xBF"},
				{0, 1, 1, 0, ""}, {1, 1, 1, 1, "b"               }, {2, 1, 1, 5, "b\xF7\xBF\xBF\xBF"}, {3, 1, 1, 5, "b\xF7\xBF\xBF\xBF" }, {4, 1, 1, 5, "b\xF7\xBF\xBF\xBF" },
				{0, 2, 2, 0, ""}, {1, 2, 2, 4, "\xF7\xBF\xBF\xBF"}, {2, 2, 2, 4, "\xF7\xBF\xBF\xBF" }, {3, 2, 2, 4, "\xF7\xBF\xBF\xBF"  }, {4, 2, 2, 4, "\xF7\xBF\xBF\xBF"  },
				{0, 3, 6, 0, ""}, {1, 3, 6, 0, ""                }, {2, 3, 6, 0, ""                 }, {3, 3, 6, 0, ""                  }, {4, 3, 6, 0, ""                  },
				{0, 4, 6, 0, ""}, {1, 4, 6, 0, ""                }, {2, 4, 6, 0, ""                 }, {3, 4, 6, 0, ""                  }, {4, 4, 6, 0, ""                  }
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
	
	// Abnormal
	
	{
		acpl::String::Ref oRef("\xF0\x80\x81\xA1""bc", false); // \xF0\x80\x81\xA1 == 'a'
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 1, "a"}, {2, 0, 0, 2, "ab"}, {3, 0, 0, 3, "abc"}, {4, 0, 0, 3, "abc"},
				{0, 1, 4, 0, ""}, {1, 1, 4, 1, "b"}, {2, 1, 4, 2, "bc"}, {3, 1, 4, 2, "bc" }, {4, 1, 4, 2, "bc" },
				{0, 2, 5, 0, ""}, {1, 2, 5, 1, "c"}, {2, 2, 5, 1, "c" }, {3, 2, 5, 1, "c"  }, {4, 2, 5, 1, "c"  },
				{0, 3, 6, 0, ""}, {1, 3, 6, 0, "" }, {2, 3, 6, 0, ""  }, {3, 3, 6, 0, ""   }, {4, 3, 6, 0, ""   },
				{0, 4, 6, 0, ""}, {1, 4, 6, 0, "" }, {2, 4, 6, 0, ""  }, {3, 4, 6, 0, ""   }, {4, 4, 6, 0, ""   }
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
	{
		acpl::String::Ref oRef("a\xF0\x80\x81\xA2""c", false); // \xF0\x80\x81\xA2 == 'b'
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 1, "a"}, {2, 0, 0, 2, "ab"}, {3, 0, 0, 3, "abc"}, {4, 0, 0, 3, "abc"},
				{0, 1, 1, 0, ""}, {1, 1, 1, 1, "b"}, {2, 1, 1, 2, "bc"}, {3, 1, 1, 2, "bc" }, {4, 1, 1, 2, "bc" },
				{0, 2, 5, 0, ""}, {1, 2, 5, 1, "c"}, {2, 2, 5, 1, "c" }, {3, 2, 5, 1, "c"  }, {4, 2, 5, 1, "c"  },
				{0, 3, 6, 0, ""}, {1, 3, 6, 0, "" }, {2, 3, 6, 0, ""  }, {3, 3, 6, 0, ""   }, {4, 3, 6, 0, ""   },
				{0, 4, 6, 0, ""}, {1, 4, 6, 0, "" }, {2, 4, 6, 0, ""  }, {3, 4, 6, 0, ""   }, {4, 4, 6, 0, ""   }
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
	{
		acpl::String::Ref oRef("ab\xF0\x80\x81\xA3", false); // \xF0\x80\x81\xA3 == 'c'
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 1, "a"}, {2, 0, 0, 2, "ab"}, {3, 0, 0, 3, "abc"}, {4, 0, 0, 3, "abc"},
				{0, 1, 1, 0, ""}, {1, 1, 1, 1, "b"}, {2, 1, 1, 2, "bc"}, {3, 1, 1, 2, "bc" }, {4, 1, 1, 2, "bc" },
				{0, 2, 2, 0, ""}, {1, 2, 2, 1, "c"}, {2, 2, 2, 1, "c" }, {3, 2, 2, 1, "c"  }, {4, 2, 2, 1, "c"  },
				{0, 3, 6, 0, ""}, {1, 3, 6, 0, "" }, {2, 3, 6, 0, ""  }, {3, 3, 6, 0, ""   }, {4, 3, 6, 0, ""   },
				{0, 4, 6, 0, ""}, {1, 4, 6, 0, "" }, {2, 4, 6, 0, ""  }, {3, 4, 6, 0, ""   }, {4, 4, 6, 0, ""   }
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
	
	// Invalid sequence
	
	{
		acpl::String::Ref oRef("\xF0""bc", false);
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 1, "?"}, {2, 0, 0, 2, "?b"}, {3, 0, 0, 3, "?bc"}, {4, 0, 0, 3, "?bc"},
				{0, 1, 1, 0, ""}, {1, 1, 1, 1, "b"}, {2, 1, 1, 2, "bc"}, {3, 1, 1, 2, "bc" }, {4, 1, 1, 2, "bc" },
				{0, 2, 2, 0, ""}, {1, 2, 2, 1, "c"}, {2, 2, 2, 1, "c" }, {3, 2, 2, 1, "c"  }, {4, 2, 2, 1, "c"  },
				{0, 3, 3, 0, ""}, {1, 3, 3, 0, "" }, {2, 3, 3, 0, ""  }, {3, 3, 3, 0, ""   }, {4, 3, 3, 0, ""   },
				{0, 4, 3, 0, ""}, {1, 4, 3, 0, "" }, {2, 4, 3, 0, ""  }, {3, 4, 3, 0, ""   }, {4, 4, 3, 0, ""   }
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
	{
		acpl::String::Ref oRef("a\xF0""c", false);
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 1, "a"}, {2, 0, 0, 2, "a?"}, {3, 0, 0, 3, "a?c"}, {4, 0, 0, 3, "a?c"},
				{0, 1, 1, 0, ""}, {1, 1, 1, 1, "?"}, {2, 1, 1, 2, "?c"}, {3, 1, 1, 2, "?c" }, {4, 1, 1, 2, "?c" },
				{0, 2, 2, 0, ""}, {1, 2, 2, 1, "c"}, {2, 2, 2, 1, "c" }, {3, 2, 2, 1, "c"  }, {4, 2, 2, 1, "c"  },
				{0, 3, 3, 0, ""}, {1, 3, 3, 0, "" }, {2, 3, 3, 0, ""  }, {3, 3, 3, 0, ""   }, {4, 3, 3, 0, ""   },
				{0, 4, 3, 0, ""}, {1, 4, 3, 0, "" }, {2, 4, 3, 0, ""  }, {3, 4, 3, 0, ""   }, {4, 4, 3, 0, ""   }
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
	{
		acpl::String::Ref oRef("ab\xF0", false);
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 1, "a"}, {2, 0, 0, 2, "ab"}, {3, 0, 0, 3, "ab?"}, {4, 0, 0, 3, "ab?"},
				{0, 1, 1, 0, ""}, {1, 1, 1, 1, "b"}, {2, 1, 1, 2, "b?"}, {3, 1, 1, 2, "b?" }, {4, 1, 1, 2, "b?" },
				{0, 2, 2, 0, ""}, {1, 2, 2, 1, "?"}, {2, 2, 2, 1, "?" }, {3, 2, 2, 1, "?"  }, {4, 2, 2, 1, "?"  },
				{0, 3, 3, 0, ""}, {1, 3, 3, 0, "" }, {2, 3, 3, 0, ""  }, {3, 3, 3, 0, ""   }, {4, 3, 3, 0, ""   },
				{0, 4, 3, 0, ""}, {1, 4, 3, 0, "" }, {2, 4, 3, 0, ""  }, {3, 4, 3, 0, ""   }, {4, 4, 3, 0, ""   }
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
	{
		acpl::String::Ref oRef("\xF0\x80\x80""bc", false);
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 1, "?"}, {2, 0, 0, 2, "??"}, {3, 0, 0, 3, "???"}, {4, 0, 0, 4, "???b"}, {5, 0, 0, 5, "???bc"}, {6, 0, 0, 5, "???bc"},
				{0, 1, 1, 0, ""}, {1, 1, 1, 1, "?"}, {2, 1, 1, 2, "??"}, {3, 1, 1, 3, "??b"}, {4, 1, 1, 4, "??bc"}, {5, 1, 1, 4, "??bc" }, {6, 1, 1, 4, "??bc" },
				{0, 2, 2, 0, ""}, {1, 2, 2, 1, "?"}, {2, 2, 2, 2, "?b"}, {3, 2, 2, 3, "?bc"}, {4, 2, 2, 3, "?bc" }, {5, 2, 2, 3, "?bc"  }, {6, 2, 2, 3, "?bc"  },
				{0, 3, 3, 0, ""}, {1, 3, 3, 1, "b"}, {2, 3, 3, 2, "bc"}, {3, 3, 3, 2, "bc" }, {4, 3, 3, 2, "bc"  }, {5, 3, 3, 2, "bc"   }, {6, 3, 3, 2, "bc"   },
				{0, 4, 4, 0, ""}, {1, 4, 4, 1, "c"}, {2, 4, 4, 1, "c" }, {3, 4, 4, 1, "c"  }, {4, 4, 4, 1, "c"   }, {5, 4, 4, 1, "c"    }, {6, 4, 4, 1, "c"    },
				{0, 5, 5, 0, ""}, {1, 5, 5, 0, "" }, {2, 5, 5, 0, ""  }, {3, 5, 5, 0, ""   }, {4, 5, 5, 0, ""    }, {5, 5, 5, 0, ""     }, {6, 5, 5, 0, ""     },
				{0, 6, 5, 0, ""}, {1, 6, 5, 0, "" }, {2, 6, 5, 0, ""  }, {3, 6, 5, 0, ""   }, {4, 6, 5, 0, ""    }, {6, 5, 5, 0, ""     }, {6, 6, 5, 0, ""     }
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
	{
		acpl::String::Ref oRef("a\xF0\x80\x80""c", false);
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 1, "a"}, {2, 0, 0, 2, "a?"}, {3, 0, 0, 3, "a??"}, {4, 0, 0, 4, "a???"}, {5, 0, 0, 5, "a???c"}, {6, 0, 0, 5, "a???c"},
				{0, 1, 1, 0, ""}, {1, 1, 1, 1, "?"}, {2, 1, 1, 2, "??"}, {3, 1, 1, 3, "???"}, {4, 1, 1, 4, "???c"}, {5, 1, 1, 4, "???c" }, {6, 1, 1, 4, "???c" },
				{0, 2, 2, 0, ""}, {1, 2, 2, 1, "?"}, {2, 2, 2, 2, "??"}, {3, 2, 2, 3, "??c"}, {4, 2, 2, 3, "??c" }, {5, 2, 2, 3, "??c"  }, {6, 2, 2, 3, "??c"  },
				{0, 3, 3, 0, ""}, {1, 3, 3, 1, "?"}, {2, 3, 3, 2, "?c"}, {3, 3, 3, 2, "?c" }, {4, 3, 3, 2, "?c"  }, {5, 3, 3, 2, "?c"   }, {6, 3, 3, 2, "?c"   },
				{0, 4, 4, 0, ""}, {1, 4, 4, 1, "c"}, {2, 4, 4, 1, "c" }, {3, 4, 4, 1, "c"  }, {4, 4, 4, 1, "c"   }, {5, 4, 4, 1, "c"    }, {6, 4, 4, 1, "c"    },
				{0, 5, 5, 0, ""}, {1, 5, 5, 0, "" }, {2, 5, 5, 0, ""  }, {3, 5, 5, 0, ""   }, {4, 5, 5, 0, ""    }, {5, 5, 5, 0, ""     }, {6, 5, 5, 0, ""     },
				{0, 6, 5, 0, ""}, {1, 6, 5, 0, "" }, {2, 6, 5, 0, ""  }, {3, 6, 5, 0, ""   }, {4, 6, 5, 0, ""    }, {6, 5, 5, 0, ""     }, {6, 6, 5, 0, ""     }
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
	{
		acpl::String::Ref oRef("ab\xF0\x80\x80", false);
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 1, "a"}, {2, 0, 0, 2, "ab"}, {3, 0, 0, 3, "ab?"}, {4, 0, 0, 4, "ab??"}, {5, 0, 0, 5, "ab???"}, {6, 0, 0, 5, "ab???"},
				{0, 1, 1, 0, ""}, {1, 1, 1, 1, "b"}, {2, 1, 1, 2, "b?"}, {3, 1, 1, 3, "b??"}, {4, 1, 1, 4, "b???"}, {5, 1, 1, 4, "b???" }, {6, 1, 1, 4, "b???" },
				{0, 2, 2, 0, ""}, {1, 2, 2, 1, "?"}, {2, 2, 2, 2, "??"}, {3, 2, 2, 3, "???"}, {4, 2, 2, 3, "???" }, {5, 2, 2, 3, "???"  }, {6, 2, 2, 3, "???"  },
				{0, 3, 3, 0, ""}, {1, 3, 3, 1, "?"}, {2, 3, 3, 2, "??"}, {3, 3, 3, 2, "??" }, {4, 3, 3, 2, "??"  }, {5, 3, 3, 2, "??"   }, {6, 3, 3, 2, "??"   },
				{0, 4, 4, 0, ""}, {1, 4, 4, 1, "?"}, {2, 4, 4, 1, "?" }, {3, 4, 4, 1, "?"  }, {4, 4, 4, 1, "?"   }, {5, 4, 4, 1, "?"    }, {6, 4, 4, 1, "?"    },
				{0, 5, 5, 0, ""}, {1, 5, 5, 0, "" }, {2, 5, 5, 0, ""  }, {3, 5, 5, 0, ""   }, {4, 5, 5, 0, ""    }, {5, 5, 5, 0, ""     }, {6, 5, 5, 0, ""     },
				{0, 6, 5, 0, ""}, {1, 6, 5, 0, "" }, {2, 6, 5, 0, ""  }, {3, 6, 5, 0, ""   }, {4, 6, 5, 0, ""    }, {6, 5, 5, 0, ""     }, {6, 6, 5, 0, ""     }
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
	
	// NULL terminator
	
	{
		acpl::String::Ref oRef("\xF0\x80\x80\x80""bc", false);
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 0, ""}, {2, 0, 0, 0, ""}, {3, 0, 0, 0, ""}, {4, 0, 0, 0, ""},
				{0, 1, 0, 0, ""}, {1, 1, 0, 0, ""}, {2, 1, 0, 0, ""}, {3, 1, 0, 0, ""}, {4, 1, 0, 0, ""},
				{0, 2, 0, 0, ""}, {1, 2, 0, 0, ""}, {2, 2, 0, 0, ""}, {3, 2, 0, 0, ""}, {4, 2, 0, 0, ""},
				{0, 3, 0, 0, ""}, {1, 3, 0, 0, ""}, {2, 3, 0, 0, ""}, {3, 3, 0, 0, ""}, {4, 3, 0, 0, ""},
				{0, 4, 0, 0, ""}, {1, 4, 0, 0, ""}, {2, 4, 0, 0, ""}, {3, 4, 0, 0, ""}, {4, 4, 0, 0, ""}
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
	{
		acpl::String::Ref oRef("a\xF0\x80\x80\x80""c", false);
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 1, "a"}, {2, 0, 0, 1, "a"}, {3, 0, 0, 1, "a"}, {4, 0, 0, 1, "a"},
				{0, 1, 1, 0, ""}, {1, 1, 1, 0, "" }, {2, 1, 1, 0, "" }, {3, 1, 1, 0, "" }, {4, 1, 1, 0, "" },
				{0, 2, 1, 0, ""}, {1, 2, 1, 0, "" }, {2, 2, 1, 0, "" }, {3, 2, 1, 0, "" }, {4, 2, 1, 0, "" },
				{0, 3, 1, 0, ""}, {1, 3, 1, 0, "" }, {2, 3, 1, 0, "" }, {3, 3, 1, 0, "" }, {4, 3, 1, 0, "" },
				{0, 4, 1, 0, ""}, {1, 4, 1, 0, "" }, {2, 4, 1, 0, "" }, {3, 4, 1, 0, "" }, {4, 4, 1, 0, "" }
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
	{
		acpl::String::Ref oRef("ab\xF0\x80\x80\x80", false);
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 1, "a"}, {2, 0, 0, 2, "ab"}, {3, 0, 0, 2, "ab"}, {4, 0, 0, 2, "ab"},
				{0, 1, 1, 0, ""}, {1, 1, 1, 1, "b"}, {2, 1, 1, 1, "b" }, {3, 1, 1, 1, "b" }, {4, 1, 1, 1, "b" },
				{0, 2, 2, 0, ""}, {1, 2, 2, 0, "" }, {2, 2, 2, 0, ""  }, {3, 2, 2, 0, ""  }, {4, 2, 2, 0, ""  },
				{0, 3, 2, 0, ""}, {1, 3, 2, 0, "" }, {2, 3, 2, 0, ""  }, {3, 3, 2, 0, ""  }, {4, 3, 2, 0, ""  },
				{0, 4, 2, 0, ""}, {1, 4, 2, 0, "" }, {2, 4, 2, 0, ""  }, {3, 4, 2, 0, ""  }, {4, 4, 2, 0, ""  }
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
	
	return 0;
}

static int TestRefMeasureConvertUtf8_5ByteSeq()
{
	PrintFn();
	
	
	// Normal
	
	{
		acpl::String::Ref oRef("\xFB\xBF\xBF\xBF\xBF""bc", false); // \xFB\xBF\xBF\xBF\xBF == U+3FFFFFF
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 5, "\xFB\xBF\xBF\xBF\xBF"}, {2, 0, 0, 6, "\xFB\xBF\xBF\xBF\xBF""b"}, {3, 0, 0, 7, "\xFB\xBF\xBF\xBF\xBF""bc"}, {4, 0, 0, 7, "\xFB\xBF\xBF\xBF\xBF""bc"},
				{0, 1, 5, 0, ""}, {1, 1, 5, 1, "b"                   }, {2, 1, 5, 2, "bc"                     }, {3, 1, 5, 2, "bc"                      }, {4, 1, 5, 2, "bc"                      },
				{0, 2, 6, 0, ""}, {1, 2, 6, 1, "c"                   }, {2, 2, 6, 1, "c"                      }, {3, 2, 6, 1, "c"                       }, {4, 2, 6, 1, "c"                       },
				{0, 3, 7, 0, ""}, {1, 3, 7, 0, ""                    }, {2, 3, 7, 0, ""                       }, {3, 3, 7, 0, ""                        }, {4, 3, 7, 0, ""                        },
				{0, 4, 7, 0, ""}, {1, 4, 7, 0, ""                    }, {2, 4, 7, 0, ""                       }, {3, 4, 7, 0, ""                        }, {4, 4, 7, 0, ""                        }
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
	{
		acpl::String::Ref oRef("a\xFB\xBF\xBF\xBF\xBF""c", false); // \xFB\xBF\xBF\xBF\xBF == U+3FFFFFF
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 1, "a"                   }, {2, 0, 0, 6, "a\xFB\xBF\xBF\xBF\xBF"  }, {3, 0, 0, 7, "a\xFB\xBF\xBF\xBF\xBF""c"}, {4, 0, 0, 7, "a\xFB\xBF\xBF\xBF\xBF""c"},
				{0, 1, 1, 0, ""}, {1, 1, 1, 5, "\xFB\xBF\xBF\xBF\xBF"}, {2, 1, 1, 6, "\xFB\xBF\xBF\xBF\xBF""c"}, {3, 1, 1, 6, "\xFB\xBF\xBF\xBF\xBF""c" }, {4, 1, 1, 6, "\xFB\xBF\xBF\xBF\xBF""c" },
				{0, 2, 6, 0, ""}, {1, 2, 6, 1, "c"                   }, {2, 2, 6, 1, "c"                      }, {3, 2, 6, 1, "c"                       }, {4, 2, 6, 1, "c"                       },
				{0, 3, 7, 0, ""}, {1, 3, 7, 0, ""                    }, {2, 3, 7, 0, ""                       }, {3, 3, 7, 0, ""                        }, {4, 3, 7, 0, ""                        },
				{0, 4, 7, 0, ""}, {1, 4, 7, 0, ""                    }, {2, 4, 7, 0, ""                       }, {3, 4, 7, 0, ""                        }, {4, 4, 7, 0, ""                        }
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
	{
		acpl::String::Ref oRef("ab\xFB\xBF\xBF\xBF\xBF", false); // \xFB\xBF\xBF\xBF\xBF == U+3FFFFFF
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 1, "a"                   }, {2, 0, 0, 2, "ab"                   }, {3, 0, 0, 7, "ab\xFB\xBF\xBF\xBF\xBF"}, {4, 0, 0, 7, "ab\xFB\xBF\xBF\xBF\xBF"},
				{0, 1, 1, 0, ""}, {1, 1, 1, 1, "b"                   }, {2, 1, 1, 6, "b\xFB\xBF\xBF\xBF\xBF"}, {3, 1, 1, 6, "b\xFB\xBF\xBF\xBF\xBF" }, {4, 1, 1, 6, "b\xFB\xBF\xBF\xBF\xBF" },
				{0, 2, 2, 0, ""}, {1, 2, 2, 5, "\xFB\xBF\xBF\xBF\xBF"}, {2, 2, 2, 5, "\xFB\xBF\xBF\xBF\xBF" }, {3, 2, 2, 5, "\xFB\xBF\xBF\xBF\xBF"  }, {4, 2, 2, 5, "\xFB\xBF\xBF\xBF\xBF"  },
				{0, 3, 7, 0, ""}, {1, 3, 7, 0, ""                    }, {2, 3, 7, 0, ""                     }, {3, 3, 7, 0, ""                      }, {4, 3, 7, 0, ""                      },
				{0, 4, 7, 0, ""}, {1, 4, 7, 0, ""                    }, {2, 4, 7, 0, ""                     }, {3, 4, 7, 0, ""                      }, {4, 4, 7, 0, ""                      }
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
	
	// Abnormal
	
	{
		acpl::String::Ref oRef("\xF8\x80\x80\x81\xA1""bc", false); // \xF8\x80\x80\x81\xA1 == 'a'
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 1, "a"}, {2, 0, 0, 2, "ab"}, {3, 0, 0, 3, "abc"}, {4, 0, 0, 3, "abc"},
				{0, 1, 5, 0, ""}, {1, 1, 5, 1, "b"}, {2, 1, 5, 2, "bc"}, {3, 1, 5, 2, "bc" }, {4, 1, 5, 2, "bc" },
				{0, 2, 6, 0, ""}, {1, 2, 6, 1, "c"}, {2, 2, 6, 1, "c" }, {3, 2, 6, 1, "c"  }, {4, 2, 6, 1, "c"  },
				{0, 3, 7, 0, ""}, {1, 3, 7, 0, "" }, {2, 3, 7, 0, ""  }, {3, 3, 7, 0, ""   }, {4, 3, 7, 0, ""   },
				{0, 4, 7, 0, ""}, {1, 4, 7, 0, "" }, {2, 4, 7, 0, ""  }, {3, 4, 7, 0, ""   }, {4, 4, 7, 0, ""   }
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
	{
		acpl::String::Ref oRef("a\xF8\x80\x80\x81\xA2""c", false); // \xF8\x80\x80\x81\xA2 == 'b'
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 1, "a"}, {2, 0, 0, 2, "ab"}, {3, 0, 0, 3, "abc"}, {4, 0, 0, 3, "abc"},
				{0, 1, 1, 0, ""}, {1, 1, 1, 1, "b"}, {2, 1, 1, 2, "bc"}, {3, 1, 1, 2, "bc" }, {4, 1, 1, 2, "bc" },
				{0, 2, 6, 0, ""}, {1, 2, 6, 1, "c"}, {2, 2, 6, 1, "c" }, {3, 2, 6, 1, "c"  }, {4, 2, 6, 1, "c"  },
				{0, 3, 7, 0, ""}, {1, 3, 7, 0, "" }, {2, 3, 7, 0, ""  }, {3, 3, 7, 0, ""   }, {4, 3, 7, 0, ""   },
				{0, 4, 7, 0, ""}, {1, 4, 7, 0, "" }, {2, 4, 7, 0, ""  }, {3, 4, 7, 0, ""   }, {4, 4, 7, 0, ""   }
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
	{
		acpl::String::Ref oRef("ab\xF8\x80\x80\x81\xA3", false); // \xF8\x80\x80\x81\xA3 == 'c'
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 1, "a"}, {2, 0, 0, 2, "ab"}, {3, 0, 0, 3, "abc"}, {4, 0, 0, 3, "abc"},
				{0, 1, 1, 0, ""}, {1, 1, 1, 1, "b"}, {2, 1, 1, 2, "bc"}, {3, 1, 1, 2, "bc" }, {4, 1, 1, 2, "bc" },
				{0, 2, 2, 0, ""}, {1, 2, 2, 1, "c"}, {2, 2, 2, 1, "c" }, {3, 2, 2, 1, "c"  }, {4, 2, 2, 1, "c"  },
				{0, 3, 7, 0, ""}, {1, 3, 7, 0, "" }, {2, 3, 7, 0, ""  }, {3, 3, 7, 0, ""   }, {4, 3, 7, 0, ""   },
				{0, 4, 7, 0, ""}, {1, 4, 7, 0, "" }, {2, 4, 7, 0, ""  }, {3, 4, 7, 0, ""   }, {4, 4, 7, 0, ""   }
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
	
	// Invalid sequence
	
	{
		acpl::String::Ref oRef("\xF8""bc", false);
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 1, "?"}, {2, 0, 0, 2, "?b"}, {3, 0, 0, 3, "?bc"}, {4, 0, 0, 3, "?bc"},
				{0, 1, 1, 0, ""}, {1, 1, 1, 1, "b"}, {2, 1, 1, 2, "bc"}, {3, 1, 1, 2, "bc" }, {4, 1, 1, 2, "bc" },
				{0, 2, 2, 0, ""}, {1, 2, 2, 1, "c"}, {2, 2, 2, 1, "c" }, {3, 2, 2, 1, "c"  }, {4, 2, 2, 1, "c"  },
				{0, 3, 3, 0, ""}, {1, 3, 3, 0, "" }, {2, 3, 3, 0, ""  }, {3, 3, 3, 0, ""   }, {4, 3, 3, 0, ""   },
				{0, 4, 3, 0, ""}, {1, 4, 3, 0, "" }, {2, 4, 3, 0, ""  }, {3, 4, 3, 0, ""   }, {4, 4, 3, 0, ""   }
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
	{
		acpl::String::Ref oRef("a\xF8""c", false);
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 1, "a"}, {2, 0, 0, 2, "a?"}, {3, 0, 0, 3, "a?c"}, {4, 0, 0, 3, "a?c"},
				{0, 1, 1, 0, ""}, {1, 1, 1, 1, "?"}, {2, 1, 1, 2, "?c"}, {3, 1, 1, 2, "?c" }, {4, 1, 1, 2, "?c" },
				{0, 2, 2, 0, ""}, {1, 2, 2, 1, "c"}, {2, 2, 2, 1, "c" }, {3, 2, 2, 1, "c"  }, {4, 2, 2, 1, "c"  },
				{0, 3, 3, 0, ""}, {1, 3, 3, 0, "" }, {2, 3, 3, 0, ""  }, {3, 3, 3, 0, ""   }, {4, 3, 3, 0, ""   },
				{0, 4, 3, 0, ""}, {1, 4, 3, 0, "" }, {2, 4, 3, 0, ""  }, {3, 4, 3, 0, ""   }, {4, 4, 3, 0, ""   }
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
	{
		acpl::String::Ref oRef("ab\xF8", false);
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 1, "a"}, {2, 0, 0, 2, "ab"}, {3, 0, 0, 3, "ab?"}, {4, 0, 0, 3, "ab?"},
				{0, 1, 1, 0, ""}, {1, 1, 1, 1, "b"}, {2, 1, 1, 2, "b?"}, {3, 1, 1, 2, "b?" }, {4, 1, 1, 2, "b?" },
				{0, 2, 2, 0, ""}, {1, 2, 2, 1, "?"}, {2, 2, 2, 1, "?" }, {3, 2, 2, 1, "?"  }, {4, 2, 2, 1, "?"  },
				{0, 3, 3, 0, ""}, {1, 3, 3, 0, "" }, {2, 3, 3, 0, ""  }, {3, 3, 3, 0, ""   }, {4, 3, 3, 0, ""   },
				{0, 4, 3, 0, ""}, {1, 4, 3, 0, "" }, {2, 4, 3, 0, ""  }, {3, 4, 3, 0, ""   }, {4, 4, 3, 0, ""   }
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
	{
		acpl::String::Ref oRef("\xF8\x80\x80\x80""bc", false);
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 1, "?"}, {2, 0, 0, 2, "??"}, {3, 0, 0, 3, "???"}, {4, 0, 0, 4, "????"}, {5, 0, 0, 5, "????b"}, {6, 0, 0, 6, "????bc"}, {7, 0, 0, 6, "????bc"},
				{0, 1, 1, 0, ""}, {1, 1, 1, 1, "?"}, {2, 1, 1, 2, "??"}, {3, 1, 1, 3, "???"}, {4, 1, 1, 4, "???b"}, {5, 1, 1, 5, "???bc"}, {6, 1, 1, 5, "???bc" }, {7, 1, 1, 5, "???bc" },
				{0, 2, 2, 0, ""}, {1, 2, 2, 1, "?"}, {2, 2, 2, 2, "??"}, {3, 2, 2, 3, "??b"}, {4, 2, 2, 4, "??bc"}, {5, 2, 2, 4, "??bc" }, {6, 2, 2, 4, "??bc"  }, {7, 2, 2, 4, "??bc"  },
				{0, 3, 3, 0, ""}, {1, 3, 3, 1, "?"}, {2, 3, 3, 2, "?b"}, {3, 3, 3, 3, "?bc"}, {4, 3, 3, 3, "?bc" }, {5, 3, 3, 3, "?bc"  }, {6, 3, 3, 3, "?bc"   }, {7, 3, 3, 3, "?bc"   },
				{0, 4, 4, 0, ""}, {1, 4, 4, 1, "b"}, {2, 4, 4, 2, "bc"}, {3, 4, 4, 2, "bc" }, {4, 4, 4, 2, "bc"  }, {5, 4, 4, 2, "bc"   }, {6, 4, 4, 2, "bc"    }, {7, 4, 4, 2, "bc"    },
				{0, 5, 5, 0, ""}, {1, 5, 5, 1, "c"}, {2, 5, 5, 1, "c" }, {3, 5, 5, 1, "c"  }, {4, 5, 5, 1, "c"   }, {5, 5, 5, 1, "c"    }, {6, 5, 5, 1, "c"     }, {7, 5, 5, 1, "c"     },
				{0, 6, 6, 0, ""}, {1, 6, 6, 0, "" }, {2, 6, 6, 0, ""  }, {3, 6, 6, 0, ""   }, {4, 6, 6, 0, ""    }, {5, 6, 6, 0, ""     }, {6, 6, 6, 0, ""      }, {7, 6, 6, 0, ""      },
				{0, 7, 6, 0, ""}, {1, 7, 6, 0, "" }, {2, 7, 6, 0, ""  }, {3, 7, 6, 0, ""   }, {4, 7, 6, 0, ""    }, {5, 7, 6, 0, ""     }, {6, 7, 6, 0, ""      }, {7, 7, 6, 0, ""      }
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
	{
		acpl::String::Ref oRef("a\xF8\x80\x80\x80""c", false);
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 1, "a"}, {2, 0, 0, 2, "a?"}, {3, 0, 0, 3, "a??"}, {4, 0, 0, 4, "a???"}, {5, 0, 0, 5, "a????"}, {6, 0, 0, 6, "a????c"}, {7, 0, 0, 6, "a????c"},
				{0, 1, 1, 0, ""}, {1, 1, 1, 1, "?"}, {2, 1, 1, 2, "??"}, {3, 1, 1, 3, "???"}, {4, 1, 1, 4, "????"}, {5, 1, 1, 5, "????c"}, {6, 1, 1, 5, "????c" }, {7, 1, 1, 5, "????c" },
				{0, 2, 2, 0, ""}, {1, 2, 2, 1, "?"}, {2, 2, 2, 2, "??"}, {3, 2, 2, 3, "???"}, {4, 2, 2, 4, "???c"}, {5, 2, 2, 4, "???c" }, {6, 2, 2, 4, "???c"  }, {7, 2, 2, 4, "???c"  },
				{0, 3, 3, 0, ""}, {1, 3, 3, 1, "?"}, {2, 3, 3, 2, "??"}, {3, 3, 3, 3, "??c"}, {4, 3, 3, 3, "??c" }, {5, 3, 3, 3, "??c"  }, {6, 3, 3, 3, "??c"   }, {7, 3, 3, 3, "??c"   },
				{0, 4, 4, 0, ""}, {1, 4, 4, 1, "?"}, {2, 4, 4, 2, "?c"}, {3, 4, 4, 2, "?c" }, {4, 4, 4, 2, "?c"  }, {5, 4, 4, 2, "?c"   }, {6, 4, 4, 2, "?c"    }, {7, 4, 4, 2, "?c"    },
				{0, 5, 5, 0, ""}, {1, 5, 5, 1, "c"}, {2, 5, 5, 1, "c" }, {3, 5, 5, 1, "c"  }, {4, 5, 5, 1, "c"   }, {5, 5, 5, 1, "c"    }, {6, 5, 5, 1, "c"     }, {7, 5, 5, 1, "c"     },
				{0, 6, 6, 0, ""}, {1, 6, 6, 0, "" }, {2, 6, 6, 0, ""  }, {3, 6, 6, 0, ""   }, {4, 6, 6, 0, ""    }, {5, 6, 6, 0, ""     }, {6, 6, 6, 0, ""      }, {7, 6, 6, 0, ""      },
				{0, 7, 6, 0, ""}, {1, 7, 6, 0, "" }, {2, 7, 6, 0, ""  }, {3, 7, 6, 0, ""   }, {4, 7, 6, 0, ""    }, {5, 7, 6, 0, ""     }, {6, 7, 6, 0, ""      }, {7, 7, 6, 0, ""      }
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
	{
		acpl::String::Ref oRef("ab\xF8\x80\x80\x80", false);
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 1, "a"}, {2, 0, 0, 2, "ab"}, {3, 0, 0, 3, "ab?"}, {4, 0, 0, 4, "ab??"}, {5, 0, 0, 5, "ab???"}, {6, 0, 0, 6, "ab????"}, {7, 0, 0, 6, "ab????"},
				{0, 1, 1, 0, ""}, {1, 1, 1, 1, "b"}, {2, 1, 1, 2, "b?"}, {3, 1, 1, 3, "b??"}, {4, 1, 1, 4, "b???"}, {5, 1, 1, 5, "b????"}, {6, 1, 1, 5, "b????" }, {7, 1, 1, 5, "b????" },
				{0, 2, 2, 0, ""}, {1, 2, 2, 1, "?"}, {2, 2, 2, 2, "??"}, {3, 2, 2, 3, "???"}, {4, 2, 2, 4, "????"}, {5, 2, 2, 4, "????" }, {6, 2, 2, 4, "????"  }, {7, 2, 2, 4, "????"  },
				{0, 3, 3, 0, ""}, {1, 3, 3, 1, "?"}, {2, 3, 3, 2, "??"}, {3, 3, 3, 3, "???"}, {4, 3, 3, 3, "???" }, {5, 3, 3, 3, "???"  }, {6, 3, 3, 3, "???"   }, {7, 3, 3, 3, "???"   },
				{0, 4, 4, 0, ""}, {1, 4, 4, 1, "?"}, {2, 4, 4, 2, "??"}, {3, 4, 4, 2, "??" }, {4, 4, 4, 2, "??"  }, {5, 4, 4, 2, "??"   }, {6, 4, 4, 2, "??"    }, {7, 4, 4, 2, "??"    },
				{0, 5, 5, 0, ""}, {1, 5, 5, 1, "?"}, {2, 5, 5, 1, "?" }, {3, 5, 5, 1, "?"  }, {4, 5, 5, 1, "?"   }, {5, 5, 5, 1, "?"    }, {6, 5, 5, 1, "?"     }, {7, 5, 5, 1, "?"     },
				{0, 6, 6, 0, ""}, {1, 6, 6, 0, "" }, {2, 6, 6, 0, ""  }, {3, 6, 6, 0, ""   }, {4, 6, 6, 0, ""    }, {5, 6, 6, 0, ""     }, {6, 6, 6, 0, ""      }, {7, 6, 6, 0, ""      },
				{0, 7, 6, 0, ""}, {1, 7, 6, 0, "" }, {2, 7, 6, 0, ""  }, {3, 7, 6, 0, ""   }, {4, 7, 6, 0, ""    }, {5, 7, 6, 0, ""     }, {6, 7, 6, 0, ""      }, {7, 7, 6, 0, ""      }
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
	
	// NULL terminator
	
	{
		acpl::String::Ref oRef("\xF8\x80\x80\x80\x80""bc", false);
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 0, ""}, {2, 0, 0, 0, ""}, {3, 0, 0, 0, ""}, {4, 0, 0, 0, ""},
				{0, 1, 0, 0, ""}, {1, 1, 0, 0, ""}, {2, 1, 0, 0, ""}, {3, 1, 0, 0, ""}, {4, 1, 0, 0, ""},
				{0, 2, 0, 0, ""}, {1, 2, 0, 0, ""}, {2, 2, 0, 0, ""}, {3, 2, 0, 0, ""}, {4, 2, 0, 0, ""},
				{0, 3, 0, 0, ""}, {1, 3, 0, 0, ""}, {2, 3, 0, 0, ""}, {3, 3, 0, 0, ""}, {4, 3, 0, 0, ""},
				{0, 4, 0, 0, ""}, {1, 4, 0, 0, ""}, {2, 4, 0, 0, ""}, {3, 4, 0, 0, ""}, {4, 4, 0, 0, ""}
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
	{
		acpl::String::Ref oRef("a\xF8\x80\x80\x80\x80""c", false);
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 1, "a"}, {2, 0, 0, 1, "a"}, {3, 0, 0, 1, "a"}, {4, 0, 0, 1, "a"},
				{0, 1, 1, 0, ""}, {1, 1, 1, 0, "" }, {2, 1, 1, 0, "" }, {3, 1, 1, 0, "" }, {4, 1, 1, 0, "" },
				{0, 2, 1, 0, ""}, {1, 2, 1, 0, "" }, {2, 2, 1, 0, "" }, {3, 2, 1, 0, "" }, {4, 2, 1, 0, "" },
				{0, 3, 1, 0, ""}, {1, 3, 1, 0, "" }, {2, 3, 1, 0, "" }, {3, 3, 1, 0, "" }, {4, 3, 1, 0, "" },
				{0, 4, 1, 0, ""}, {1, 4, 1, 0, "" }, {2, 4, 1, 0, "" }, {3, 4, 1, 0, "" }, {4, 4, 1, 0, "" }
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
	{
		acpl::String::Ref oRef("ab\xF8\x80\x80\x80\x80", false);
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 1, "a"}, {2, 0, 0, 2, "ab"}, {3, 0, 0, 2, "ab"}, {4, 0, 0, 2, "ab"},
				{0, 1, 1, 0, ""}, {1, 1, 1, 1, "b"}, {2, 1, 1, 1, "b" }, {3, 1, 1, 1, "b" }, {4, 1, 1, 1, "b" },
				{0, 2, 2, 0, ""}, {1, 2, 2, 0, "" }, {2, 2, 2, 0, ""  }, {3, 2, 2, 0, ""  }, {4, 2, 2, 0, ""  },
				{0, 3, 2, 0, ""}, {1, 3, 2, 0, "" }, {2, 3, 2, 0, ""  }, {3, 3, 2, 0, ""  }, {4, 3, 2, 0, ""  },
				{0, 4, 2, 0, ""}, {1, 4, 2, 0, "" }, {2, 4, 2, 0, ""  }, {3, 4, 2, 0, ""  }, {4, 4, 2, 0, ""  }
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
	
	return 0;
}

static int TestRefMeasureConvertUtf8_6ByteSeq()
{
	PrintFn();
	
	
	// Normal
	
	{
		acpl::String::Ref oRef("\xFD\xBF\xBF\xBF\xBF\xBF""bc", false); // \xFD\xBF\xBF\xBF\xBF\xBF == U+7FFFFFFF
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 6, "\xFD\xBF\xBF\xBF\xBF\xBF"}, {2, 0, 0, 7, "\xFD\xBF\xBF\xBF\xBF\xBF""b"}, {3, 0, 0, 8, "\xFD\xBF\xBF\xBF\xBF\xBF""bc"}, {4, 0, 0, 8, "\xFD\xBF\xBF\xBF\xBF\xBF""bc"},
				{0, 1, 6, 0, ""}, {1, 1, 6, 1, "b"                       }, {2, 1, 6, 2, "bc"                         }, {3, 1, 6, 2, "bc"                          }, {4, 1, 6, 2, "bc"                          },
				{0, 2, 7, 0, ""}, {1, 2, 7, 1, "c"                       }, {2, 2, 7, 1, "c"                          }, {3, 2, 7, 1, "c"                           }, {4, 2, 7, 1, "c"                           },
				{0, 3, 8, 0, ""}, {1, 3, 8, 0, ""                        }, {2, 3, 8, 0, ""                           }, {3, 3, 8, 0, ""                            }, {4, 3, 8, 0, ""                            },
				{0, 4, 8, 0, ""}, {1, 4, 8, 0, ""                        }, {2, 4, 8, 0, ""                           }, {3, 4, 8, 0, ""                            }, {4, 4, 8, 0, ""                            }
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
	{
		acpl::String::Ref oRef("a\xFD\xBF\xBF\xBF\xBF\xBF""c", false); // \xFD\xBF\xBF\xBF\xBF\xBF == U+7FFFFFFF
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 1, "a"                       }, {2, 0, 0, 7, "a\xFD\xBF\xBF\xBF\xBF\xBF"  }, {3, 0, 0, 8, "a\xFD\xBF\xBF\xBF\xBF\xBF""c"}, {4, 0, 0, 8, "a\xFD\xBF\xBF\xBF\xBF\xBF""c"},
				{0, 1, 1, 0, ""}, {1, 1, 1, 6, "\xFD\xBF\xBF\xBF\xBF\xBF"}, {2, 1, 1, 7, "\xFD\xBF\xBF\xBF\xBF\xBF""c"}, {3, 1, 1, 7, "\xFD\xBF\xBF\xBF\xBF\xBF""c" }, {4, 1, 1, 7, "\xFD\xBF\xBF\xBF\xBF\xBF""c" },
				{0, 2, 7, 0, ""}, {1, 2, 7, 1, "c"                       }, {2, 2, 7, 1, "c"                          }, {3, 2, 7, 1, "c"                           }, {4, 2, 7, 1, "c"                           },
				{0, 3, 8, 0, ""}, {1, 3, 8, 0, ""                        }, {2, 3, 8, 0, ""                           }, {3, 3, 8, 0, ""                            }, {4, 3, 8, 0, ""                            },
				{0, 4, 8, 0, ""}, {1, 4, 8, 0, ""                        }, {2, 4, 8, 0, ""                           }, {3, 4, 8, 0, ""                            }, {4, 4, 8, 0, ""                            }
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
	{
		acpl::String::Ref oRef("ab\xFD\xBF\xBF\xBF\xBF\xBF", false); // \xFD\xBF\xBF\xBF\xBF\xBF == U+7FFFFFFF
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 1, "a"                       }, {2, 0, 0, 2, "ab"                       }, {3, 0, 0, 8, "ab\xFD\xBF\xBF\xBF\xBF\xBF"}, {4, 0, 0, 8, "ab\xFD\xBF\xBF\xBF\xBF\xBF"},
				{0, 1, 1, 0, ""}, {1, 1, 1, 1, "b"                       }, {2, 1, 1, 7, "b\xFD\xBF\xBF\xBF\xBF\xBF"}, {3, 1, 1, 7, "b\xFD\xBF\xBF\xBF\xBF\xBF" }, {4, 1, 1, 7, "b\xFD\xBF\xBF\xBF\xBF\xBF" },
				{0, 2, 2, 0, ""}, {1, 2, 2, 6, "\xFD\xBF\xBF\xBF\xBF\xBF"}, {2, 2, 2, 6, "\xFD\xBF\xBF\xBF\xBF\xBF" }, {3, 2, 2, 6, "\xFD\xBF\xBF\xBF\xBF\xBF"  }, {4, 2, 2, 6, "\xFD\xBF\xBF\xBF\xBF\xBF"  },
				{0, 3, 8, 0, ""}, {1, 3, 8, 0, ""                        }, {2, 3, 8, 0, ""                         }, {3, 3, 8, 0, ""                          }, {4, 3, 8, 0, ""                          },
				{0, 4, 8, 0, ""}, {1, 4, 8, 0, ""                        }, {2, 4, 8, 0, ""                         }, {3, 4, 8, 0, ""                          }, {4, 4, 8, 0, ""                          }
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
	
	// Abnormal
	
	{
		acpl::String::Ref oRef("\xFC\x80\x80\x80\x81\xA1""bc", false); // \xFC\x80\x80\x80\x81\xA1 == 'a'
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 1, "a"}, {2, 0, 0, 2, "ab"}, {3, 0, 0, 3, "abc"}, {4, 0, 0, 3, "abc"},
				{0, 1, 6, 0, ""}, {1, 1, 6, 1, "b"}, {2, 1, 6, 2, "bc"}, {3, 1, 6, 2, "bc" }, {4, 1, 6, 2, "bc" },
				{0, 2, 7, 0, ""}, {1, 2, 7, 1, "c"}, {2, 2, 7, 1, "c" }, {3, 2, 7, 1, "c"  }, {4, 2, 7, 1, "c"  },
				{0, 3, 8, 0, ""}, {1, 3, 8, 0, "" }, {2, 3, 8, 0, ""  }, {3, 3, 8, 0, ""   }, {4, 3, 8, 0, ""   },
				{0, 4, 8, 0, ""}, {1, 4, 8, 0, "" }, {2, 4, 8, 0, ""  }, {3, 4, 8, 0, ""   }, {4, 4, 8, 0, ""   }
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
	{
		acpl::String::Ref oRef("a\xFC\x80\x80\x80\x81\xA2""c", false); // \xFC\x80\x80\x80\x81\xA2 == 'b'
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 1, "a"}, {2, 0, 0, 2, "ab"}, {3, 0, 0, 3, "abc"}, {4, 0, 0, 3, "abc"},
				{0, 1, 1, 0, ""}, {1, 1, 1, 1, "b"}, {2, 1, 1, 2, "bc"}, {3, 1, 1, 2, "bc" }, {4, 1, 1, 2, "bc" },
				{0, 2, 7, 0, ""}, {1, 2, 7, 1, "c"}, {2, 2, 7, 1, "c" }, {3, 2, 7, 1, "c"  }, {4, 2, 7, 1, "c"  },
				{0, 3, 8, 0, ""}, {1, 3, 8, 0, "" }, {2, 3, 8, 0, ""  }, {3, 3, 8, 0, ""   }, {4, 3, 8, 0, ""   },
				{0, 4, 8, 0, ""}, {1, 4, 8, 0, "" }, {2, 4, 8, 0, ""  }, {3, 4, 8, 0, ""   }, {4, 4, 8, 0, ""   }
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
	{
		acpl::String::Ref oRef("ab\xFC\x80\x80\x80\x81\xA3", false); // \xFC\x80\x80\x80\x81\xA3 == 'c'
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 1, "a"}, {2, 0, 0, 2, "ab"}, {3, 0, 0, 3, "abc"}, {4, 0, 0, 3, "abc"},
				{0, 1, 1, 0, ""}, {1, 1, 1, 1, "b"}, {2, 1, 1, 2, "bc"}, {3, 1, 1, 2, "bc" }, {4, 1, 1, 2, "bc" },
				{0, 2, 2, 0, ""}, {1, 2, 2, 1, "c"}, {2, 2, 2, 1, "c" }, {3, 2, 2, 1, "c"  }, {4, 2, 2, 1, "c"  },
				{0, 3, 8, 0, ""}, {1, 3, 8, 0, "" }, {2, 3, 8, 0, ""  }, {3, 3, 8, 0, ""   }, {4, 3, 8, 0, ""   },
				{0, 4, 8, 0, ""}, {1, 4, 8, 0, "" }, {2, 4, 8, 0, ""  }, {3, 4, 8, 0, ""   }, {4, 4, 8, 0, ""   }
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
	
	// Invalid sequence
	
	{
		acpl::String::Ref oRef("\xFC""bc", false);
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 1, "?"}, {2, 0, 0, 2, "?b"}, {3, 0, 0, 3, "?bc"}, {4, 0, 0, 3, "?bc"},
				{0, 1, 1, 0, ""}, {1, 1, 1, 1, "b"}, {2, 1, 1, 2, "bc"}, {3, 1, 1, 2, "bc" }, {4, 1, 1, 2, "bc" },
				{0, 2, 2, 0, ""}, {1, 2, 2, 1, "c"}, {2, 2, 2, 1, "c" }, {3, 2, 2, 1, "c"  }, {4, 2, 2, 1, "c"  },
				{0, 3, 3, 0, ""}, {1, 3, 3, 0, "" }, {2, 3, 3, 0, ""  }, {3, 3, 3, 0, ""   }, {4, 3, 3, 0, ""   },
				{0, 4, 3, 0, ""}, {1, 4, 3, 0, "" }, {2, 4, 3, 0, ""  }, {3, 4, 3, 0, ""   }, {4, 4, 3, 0, ""   }
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
	{
		acpl::String::Ref oRef("a\xFC""c", false);
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 1, "a"}, {2, 0, 0, 2, "a?"}, {3, 0, 0, 3, "a?c"}, {4, 0, 0, 3, "a?c"},
				{0, 1, 1, 0, ""}, {1, 1, 1, 1, "?"}, {2, 1, 1, 2, "?c"}, {3, 1, 1, 2, "?c" }, {4, 1, 1, 2, "?c" },
				{0, 2, 2, 0, ""}, {1, 2, 2, 1, "c"}, {2, 2, 2, 1, "c" }, {3, 2, 2, 1, "c"  }, {4, 2, 2, 1, "c"  },
				{0, 3, 3, 0, ""}, {1, 3, 3, 0, "" }, {2, 3, 3, 0, ""  }, {3, 3, 3, 0, ""   }, {4, 3, 3, 0, ""   },
				{0, 4, 3, 0, ""}, {1, 4, 3, 0, "" }, {2, 4, 3, 0, ""  }, {3, 4, 3, 0, ""   }, {4, 4, 3, 0, ""   }
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
	{
		acpl::String::Ref oRef("ab\xFC", false);
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 1, "a"}, {2, 0, 0, 2, "ab"}, {3, 0, 0, 3, "ab?"}, {4, 0, 0, 3, "ab?"},
				{0, 1, 1, 0, ""}, {1, 1, 1, 1, "b"}, {2, 1, 1, 2, "b?"}, {3, 1, 1, 2, "b?" }, {4, 1, 1, 2, "b?" },
				{0, 2, 2, 0, ""}, {1, 2, 2, 1, "?"}, {2, 2, 2, 1, "?" }, {3, 2, 2, 1, "?"  }, {4, 2, 2, 1, "?"  },
				{0, 3, 3, 0, ""}, {1, 3, 3, 0, "" }, {2, 3, 3, 0, ""  }, {3, 3, 3, 0, ""   }, {4, 3, 3, 0, ""   },
				{0, 4, 3, 0, ""}, {1, 4, 3, 0, "" }, {2, 4, 3, 0, ""  }, {3, 4, 3, 0, ""   }, {4, 4, 3, 0, ""   }
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
	{
		acpl::String::Ref oRef("\xFC\x80\x80\x80\x80""bc", false);
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 1, "?"}, {2, 0, 0, 2, "??"}, {3, 0, 0, 3, "???"}, {4, 0, 0, 4, "????"}, {5, 0, 0, 5, "?????"}, {6, 0, 0, 6, "?????b"}, {7, 0, 0, 7, "?????bc"}, {8, 0, 0, 7, "?????bc"},
				{0, 1, 1, 0, ""}, {1, 1, 1, 1, "?"}, {2, 1, 1, 2, "??"}, {3, 1, 1, 3, "???"}, {4, 1, 1, 4, "????"}, {5, 1, 1, 5, "????b"}, {6, 1, 1, 6, "????bc"}, {7, 1, 1, 6, "????bc" }, {8, 1, 1, 6, "????bc" },
				{0, 2, 2, 0, ""}, {1, 2, 2, 1, "?"}, {2, 2, 2, 2, "??"}, {3, 2, 2, 3, "???"}, {4, 2, 2, 4, "???b"}, {5, 2, 2, 5, "???bc"}, {6, 2, 2, 5, "???bc" }, {7, 2, 2, 5, "???bc"  }, {8, 2, 2, 5, "???bc"  },
				{0, 3, 3, 0, ""}, {1, 3, 3, 1, "?"}, {2, 3, 3, 2, "??"}, {3, 3, 3, 3, "??b"}, {4, 3, 3, 4, "??bc"}, {5, 3, 3, 4, "??bc" }, {6, 3, 3, 4, "??bc"  }, {7, 3, 3, 4, "??bc"   }, {8, 3, 3, 4, "??bc"   },
				{0, 4, 4, 0, ""}, {1, 4, 4, 1, "?"}, {2, 4, 4, 2, "?b"}, {3, 4, 4, 3, "?bc"}, {4, 4, 4, 3, "?bc" }, {5, 4, 4, 3, "?bc"  }, {6, 4, 4, 3, "?bc"   }, {7, 4, 4, 3, "?bc"    }, {8, 4, 4, 3, "?bc"    },
				{0, 5, 5, 0, ""}, {1, 5, 5, 1, "b"}, {2, 5, 5, 2, "bc"}, {3, 5, 5, 2, "bc" }, {4, 5, 5, 2, "bc"  }, {5, 5, 5, 2, "bc"   }, {6, 5, 5, 2, "bc"    }, {7, 5, 5, 2, "bc"     }, {8, 5, 5, 2, "bc"     },
				{0, 6, 6, 0, ""}, {1, 6, 6, 1, "c"}, {2, 6, 6, 1, "c" }, {3, 6, 6, 1, "c"  }, {4, 6, 6, 1, "c"   }, {5, 6, 6, 1, "c"    }, {6, 6, 6, 1, "c"     }, {7, 6, 6, 1, "c"      }, {8, 6, 6, 1, "c"      },
				{0, 7, 7, 0, ""}, {1, 7, 7, 0, "" }, {2, 7, 7, 0, ""  }, {3, 7, 7, 0, ""   }, {4, 7, 7, 0, ""    }, {5, 7, 7, 0, ""     }, {6, 7, 7, 0, ""      }, {7, 7, 7, 0, ""       }, {8, 7, 7, 0, ""       },
				{0, 8, 7, 0, ""}, {1, 8, 7, 0, "" }, {2, 8, 7, 0, ""  }, {3, 8, 7, 0, ""   }, {4, 8, 7, 0, ""    }, {5, 8, 7, 0, ""     }, {6, 8, 7, 0, ""      }, {7, 8, 7, 0, ""       }, {8, 8, 7, 0, ""       }
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
	{
		acpl::String::Ref oRef("a\xFC\x80\x80\x80\x80""c", false);
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 1, "a"}, {2, 0, 0, 2, "a?"}, {3, 0, 0, 3, "a??"}, {4, 0, 0, 4, "a???"}, {5, 0, 0, 5, "a????"}, {6, 0, 0, 6, "a?????"}, {7, 0, 0, 7, "a?????c"}, {8, 0, 0, 7, "a?????c"},
				{0, 1, 1, 0, ""}, {1, 1, 1, 1, "?"}, {2, 1, 1, 2, "??"}, {3, 1, 1, 3, "???"}, {4, 1, 1, 4, "????"}, {5, 1, 1, 5, "?????"}, {6, 1, 1, 6, "?????c"}, {7, 1, 1, 6, "?????c" }, {8, 1, 1, 6, "?????c" },
				{0, 2, 2, 0, ""}, {1, 2, 2, 1, "?"}, {2, 2, 2, 2, "??"}, {3, 2, 2, 3, "???"}, {4, 2, 2, 4, "????"}, {5, 2, 2, 5, "????c"}, {6, 2, 2, 5, "????c" }, {7, 2, 2, 5, "????c"  }, {8, 2, 2, 5, "????c"  },
				{0, 3, 3, 0, ""}, {1, 3, 3, 1, "?"}, {2, 3, 3, 2, "??"}, {3, 3, 3, 3, "???"}, {4, 3, 3, 4, "???c"}, {5, 3, 3, 4, "???c" }, {6, 3, 3, 4, "???c"  }, {7, 3, 3, 4, "???c"   }, {8, 3, 3, 4, "???c"   },
				{0, 4, 4, 0, ""}, {1, 4, 4, 1, "?"}, {2, 4, 4, 2, "??"}, {3, 4, 4, 3, "??c"}, {4, 4, 4, 3, "??c" }, {5, 4, 4, 3, "??c"  }, {6, 4, 4, 3, "??c"   }, {7, 4, 4, 3, "??c"    }, {8, 4, 4, 3, "??c"    },
				{0, 5, 5, 0, ""}, {1, 5, 5, 1, "?"}, {2, 5, 5, 2, "?c"}, {3, 5, 5, 2, "?c" }, {4, 5, 5, 2, "?c"  }, {5, 5, 5, 2, "?c"   }, {6, 5, 5, 2, "?c"    }, {7, 5, 5, 2, "?c"     }, {8, 5, 5, 2, "?c"     },
				{0, 6, 6, 0, ""}, {1, 6, 6, 1, "c"}, {2, 6, 6, 1, "c" }, {3, 6, 6, 1, "c"  }, {4, 6, 6, 1, "c"   }, {5, 6, 6, 1, "c"    }, {6, 6, 6, 1, "c"     }, {7, 6, 6, 1, "c"      }, {8, 6, 6, 1, "c"      },
				{0, 7, 7, 0, ""}, {1, 7, 7, 0, "" }, {2, 7, 7, 0, ""  }, {3, 7, 7, 0, ""   }, {4, 7, 7, 0, ""    }, {5, 7, 7, 0, ""     }, {6, 7, 7, 0, ""      }, {7, 7, 7, 0, ""       }, {8, 7, 7, 0, ""       },
				{0, 8, 7, 0, ""}, {1, 8, 7, 0, "" }, {2, 8, 7, 0, ""  }, {3, 8, 7, 0, ""   }, {4, 8, 7, 0, ""    }, {5, 8, 7, 0, ""     }, {6, 8, 7, 0, ""      }, {7, 8, 7, 0, ""       }, {8, 8, 7, 0, ""       }
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
	{
		acpl::String::Ref oRef("ab\xFC\x80\x80\x80\x80", false);
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 1, "a"}, {2, 0, 0, 2, "ab"}, {3, 0, 0, 3, "ab?"}, {4, 0, 0, 4, "ab??"}, {5, 0, 0, 5, "ab???"}, {6, 0, 0, 6, "ab????"}, {7, 0, 0, 7, "ab?????"}, {8, 0, 0, 7, "ab?????"},
				{0, 1, 1, 0, ""}, {1, 1, 1, 1, "b"}, {2, 1, 1, 2, "b?"}, {3, 1, 1, 3, "b??"}, {4, 1, 1, 4, "b???"}, {5, 1, 1, 5, "b????"}, {6, 1, 1, 6, "b?????"}, {7, 1, 1, 6, "b?????" }, {8, 1, 1, 6, "b?????" },
				{0, 2, 2, 0, ""}, {1, 2, 2, 1, "?"}, {2, 2, 2, 2, "??"}, {3, 2, 2, 3, "???"}, {4, 2, 2, 4, "????"}, {5, 2, 2, 5, "?????"}, {6, 2, 2, 5, "?????" }, {7, 2, 2, 5, "?????"  }, {8, 2, 2, 5, "?????"  },
				{0, 3, 3, 0, ""}, {1, 3, 3, 1, "?"}, {2, 3, 3, 2, "??"}, {3, 3, 3, 3, "???"}, {4, 3, 3, 4, "????"}, {5, 3, 3, 4, "????" }, {6, 3, 3, 4, "????"  }, {7, 3, 3, 4, "????"   }, {8, 3, 3, 4, "????"   },
				{0, 4, 4, 0, ""}, {1, 4, 4, 1, "?"}, {2, 4, 4, 2, "??"}, {3, 4, 4, 3, "???"}, {4, 4, 4, 3, "???" }, {5, 4, 4, 3, "???"  }, {6, 4, 4, 3, "???"   }, {7, 4, 4, 3, "???"    }, {8, 4, 4, 3, "???"    },
				{0, 5, 5, 0, ""}, {1, 5, 5, 1, "?"}, {2, 5, 5, 2, "??"}, {3, 5, 5, 2, "??" }, {4, 5, 5, 2, "??"  }, {5, 5, 5, 2, "??"   }, {6, 5, 5, 2, "??"    }, {7, 5, 5, 2, "??"     }, {8, 5, 5, 2, "??"     },
				{0, 6, 6, 0, ""}, {1, 6, 6, 1, "?"}, {2, 6, 6, 1, "?" }, {3, 6, 6, 1, "?"  }, {4, 6, 6, 1, "?"   }, {5, 6, 6, 1, "?"    }, {6, 6, 6, 1, "?"     }, {7, 6, 6, 1, "?"      }, {8, 6, 6, 1, "?"      },
				{0, 7, 7, 0, ""}, {1, 7, 7, 0, "" }, {2, 7, 7, 0, ""  }, {3, 7, 7, 0, ""   }, {4, 7, 7, 0, ""    }, {5, 7, 7, 0, ""     }, {6, 7, 7, 0, ""      }, {7, 7, 7, 0, ""       }, {8, 7, 7, 0, ""       },
				{0, 8, 7, 0, ""}, {1, 8, 7, 0, "" }, {2, 8, 7, 0, ""  }, {3, 8, 7, 0, ""   }, {4, 8, 7, 0, ""    }, {5, 8, 7, 0, ""     }, {6, 8, 7, 0, ""      }, {7, 8, 7, 0, ""       }, {8, 8, 7, 0, ""       }
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
	
	// NULL terminator
	
	{
		acpl::String::Ref oRef("\xFC\x80\x80\x80\x80\x80""bc", false);
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 0, ""}, {2, 0, 0, 0, ""}, {3, 0, 0, 0, ""}, {4, 0, 0, 0, ""},
				{0, 1, 0, 0, ""}, {1, 1, 0, 0, ""}, {2, 1, 0, 0, ""}, {3, 1, 0, 0, ""}, {4, 1, 0, 0, ""},
				{0, 2, 0, 0, ""}, {1, 2, 0, 0, ""}, {2, 2, 0, 0, ""}, {3, 2, 0, 0, ""}, {4, 2, 0, 0, ""},
				{0, 3, 0, 0, ""}, {1, 3, 0, 0, ""}, {2, 3, 0, 0, ""}, {3, 3, 0, 0, ""}, {4, 3, 0, 0, ""},
				{0, 4, 0, 0, ""}, {1, 4, 0, 0, ""}, {2, 4, 0, 0, ""}, {3, 4, 0, 0, ""}, {4, 4, 0, 0, ""}
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
	{
		acpl::String::Ref oRef("a\xFC\x80\x80\x80\x80\x80""c", false);
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 1, "a"}, {2, 0, 0, 1, "a"}, {3, 0, 0, 1, "a"}, {4, 0, 0, 1, "a"},
				{0, 1, 1, 0, ""}, {1, 1, 1, 0, "" }, {2, 1, 1, 0, "" }, {3, 1, 1, 0, "" }, {4, 1, 1, 0, "" },
				{0, 2, 1, 0, ""}, {1, 2, 1, 0, "" }, {2, 2, 1, 0, "" }, {3, 2, 1, 0, "" }, {4, 2, 1, 0, "" },
				{0, 3, 1, 0, ""}, {1, 3, 1, 0, "" }, {2, 3, 1, 0, "" }, {3, 3, 1, 0, "" }, {4, 3, 1, 0, "" },
				{0, 4, 1, 0, ""}, {1, 4, 1, 0, "" }, {2, 4, 1, 0, "" }, {3, 4, 1, 0, "" }, {4, 4, 1, 0, "" }
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
	{
		acpl::String::Ref oRef("ab\xFC\x80\x80\x80\x80\x80", false);
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 1, "a"}, {2, 0, 0, 2, "ab"}, {3, 0, 0, 2, "ab"}, {4, 0, 0, 2, "ab"},
				{0, 1, 1, 0, ""}, {1, 1, 1, 1, "b"}, {2, 1, 1, 1, "b" }, {3, 1, 1, 1, "b" }, {4, 1, 1, 1, "b" },
				{0, 2, 2, 0, ""}, {1, 2, 2, 0, "" }, {2, 2, 2, 0, ""  }, {3, 2, 2, 0, ""  }, {4, 2, 2, 0, ""  },
				{0, 3, 2, 0, ""}, {1, 3, 2, 0, "" }, {2, 3, 2, 0, ""  }, {3, 3, 2, 0, ""  }, {4, 3, 2, 0, ""  },
				{0, 4, 2, 0, ""}, {1, 4, 2, 0, "" }, {2, 4, 2, 0, ""  }, {3, 4, 2, 0, ""  }, {4, 4, 2, 0, ""  }
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
	
	return 0;
}

static int TestRefMeasureConvertUtf8()
{
	PrintFn();
	
	
	acpl::SizeT oRefOff;
	acpl::SizeT oEncByteSize;
	
	
	// Empty
	
	{
		acpl::String::Ref oRef(reinterpret_cast<const char *>(NULL), false);
		Test(oRef.GetType() == acpl::String::Ref::rtUtf8);
		oRef.Measure(acpl::Num<acpl::SizeT>::Max(), 0, oRefOff, oEncByteSize);
		Test(oRefOff == 0);
		Test(oEncByteSize == 0);
	}
	
	
	Test(TestRefMeasureConvertUtf8_1ByteSeq() == 0);
	Test(TestRefMeasureConvertUtf8_2ByteSeq() == 0);
	Test(TestRefMeasureConvertUtf8_3ByteSeq() == 0);
	Test(TestRefMeasureConvertUtf8_4ByteSeq() == 0);
	Test(TestRefMeasureConvertUtf8_5ByteSeq() == 0);
	Test(TestRefMeasureConvertUtf8_6ByteSeq() == 0);
	
	
	return 0;
}

static int TestRefMeasureConvertMbs()
{
	PrintFn();
	
	
	acpl::SizeT oRefOff;
	acpl::SizeT oEncByteSize;
	
	
	// Empty
	
	{
		acpl::String::Ref oRef(reinterpret_cast<const char *>(NULL), true);
		Test(oRef.GetType() == acpl::String::Ref::rtMbs);
		oRef.Measure(acpl::Num<acpl::SizeT>::Max(), 0, oRefOff, oEncByteSize);
		Test(oRefOff == 0);
		Test(oEncByteSize == 0);
	}
	
	
	// Non-empty
	
	{
		acpl::String::Ref oRef("abc", true);
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 1, "a"}, {2, 0, 0, 2, "ab"}, {3, 0, 0, 3, "abc"}, {4, 0, 0, 3, "abc"},
				{0, 1, 1, 0, ""}, {1, 1, 1, 1, "b"}, {2, 1, 1, 2, "bc"}, {3, 1, 1, 2, "bc" }, {4, 1, 1, 2, "bc" },
				{0, 2, 2, 0, ""}, {1, 2, 2, 1, "c"}, {2, 2, 2, 1, "c" }, {3, 2, 2, 1, "c"  }, {4, 2, 2, 1, "c"  },
				{0, 3, 3, 0, ""}, {1, 3, 3, 0, "" }, {2, 3, 3, 0, ""  }, {3, 3, 3, 0, ""   }, {4, 3, 3, 0, ""   },
				{0, 4, 3, 0, ""}, {1, 4, 3, 0, "" }, {2, 4, 3, 0, ""  }, {3, 4, 3, 0, ""   }, {4, 4, 3, 0, ""   }
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
	
	// Unfortunatelly there can't be a unified tests for upper-ascii characters as they heavily depend on current locale
	
	
	return 0;
}

static int TestRefMeasureConvertWcs_EncAs1ByteSeq()
{
	PrintFn();
	
	
	{
		acpl::String::Ref oRef(L"abc");
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 1, "a"}, {2, 0, 0, 2, "ab"}, {3, 0, 0, 3, "abc"}, {4, 0, 0, 3, "abc"},
				{0, 1, 1, 0, ""}, {1, 1, 1, 1, "b"}, {2, 1, 1, 2, "bc"}, {3, 1, 1, 2, "bc" }, {4, 1, 1, 2, "bc" },
				{0, 2, 2, 0, ""}, {1, 2, 2, 1, "c"}, {2, 2, 2, 1, "c" }, {3, 2, 2, 1, "c"  }, {4, 2, 2, 1, "c"  },
				{0, 3, 3, 0, ""}, {1, 3, 3, 0, "" }, {2, 3, 3, 0, ""  }, {3, 3, 3, 0, ""   }, {4, 3, 3, 0, ""   },
				{0, 4, 3, 0, ""}, {1, 4, 3, 0, "" }, {2, 4, 3, 0, ""  }, {3, 4, 3, 0, ""   }, {4, 4, 3, 0, ""   }
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
	
	return 0;
}

static int TestRefMeasureConvertWcs_EncAs2ByteSeq()
{
	PrintFn();
	
	
	{
		acpl::String::Ref oRef(L"\x7FF"L"bc"); // U+7FF == \xDF\xBF
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 2, "\xDF\xBF"}, {2, 0, 0, 3, "\xDF\xBF""b"}, {3, 0, 0, 4, "\xDF\xBF""bc"}, {4, 0, 0, 4, "\xDF\xBF""bc"},
				{0, 1, 1, 0, ""}, {1, 1, 1, 1, "b"       }, {2, 1, 1, 2, "bc"         }, {3, 1, 1, 2, "bc"          }, {4, 1, 1, 2, "bc"          },
				{0, 2, 2, 0, ""}, {1, 2, 2, 1, "c"       }, {2, 2, 2, 1, "c"          }, {3, 2, 2, 1, "c"           }, {4, 2, 2, 1, "c"           },
				{0, 3, 3, 0, ""}, {1, 3, 3, 0, ""        }, {2, 3, 3, 0, ""           }, {3, 3, 3, 0, ""            }, {4, 3, 3, 0, ""            },
				{0, 4, 3, 0, ""}, {1, 4, 3, 0, ""        }, {2, 4, 3, 0, ""           }, {3, 4, 3, 0, ""            }, {4, 4, 3, 0, ""            }
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
	{
		acpl::String::Ref oRef(L"a\x7FF"L"c"); // U+7FF == \xDF\xBF
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 1, "a"       }, {2, 0, 0, 3, "a\xDF\xBF"  }, {3, 0, 0, 4, "a\xDF\xBF""c"}, {4, 0, 0, 4, "a\xDF\xBF""c"},
				{0, 1, 1, 0, ""}, {1, 1, 1, 2, "\xDF\xBF"}, {2, 1, 1, 3, "\xDF\xBF""c"}, {3, 1, 1, 3, "\xDF\xBF""c" }, {4, 1, 1, 3, "\xDF\xBF""c" },
				{0, 2, 2, 0, ""}, {1, 2, 2, 1, "c"       }, {2, 2, 2, 1, "c"          }, {3, 2, 2, 1, "c"           }, {4, 2, 2, 1, "c"           },
				{0, 3, 3, 0, ""}, {1, 3, 3, 0, ""        }, {2, 3, 3, 0, ""           }, {3, 3, 3, 0, ""            }, {4, 3, 3, 0, ""            },
				{0, 4, 3, 0, ""}, {1, 4, 3, 0, ""        }, {2, 4, 3, 0, ""           }, {3, 4, 3, 0, ""            }, {4, 4, 3, 0, ""            }
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
	{
		acpl::String::Ref oRef(L"ab\x7FF"); // U+7FF == \xDF\xBF
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 1, "a"       }, {2, 0, 0, 2, "ab"       }, {3, 0, 0, 4, "ab\xDF\xBF"}, {4, 0, 0, 4, "ab\xDF\xBF"},
				{0, 1, 1, 0, ""}, {1, 1, 1, 1, "b"       }, {2, 1, 1, 3, "b\xDF\xBF"}, {3, 1, 1, 3, "b\xDF\xBF" }, {4, 1, 1, 3, "b\xDF\xBF" },
				{0, 2, 2, 0, ""}, {1, 2, 2, 2, "\xDF\xBF"}, {2, 2, 2, 2, "\xDF\xBF" }, {3, 2, 2, 2, "\xDF\xBF"  }, {4, 2, 2, 2, "\xDF\xBF"  },
				{0, 3, 3, 0, ""}, {1, 3, 3, 0, ""        }, {2, 3, 3, 0, ""         }, {3, 3, 3, 0, ""          }, {4, 3, 3, 0, ""          },
				{0, 4, 3, 0, ""}, {1, 4, 3, 0, ""        }, {2, 4, 3, 0, ""         }, {3, 4, 3, 0, ""          }, {4, 4, 3, 0, ""          }
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
	
	return 0;
}

static int TestRefMeasureConvertWcs_EncAs3ByteSeq()
{
	PrintFn();
	
	
	{
		acpl::String::Ref oRef(L"\xFFFF"L"bc"); // U+FFFF == \xEF\xBF\xBF
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 3, "\xEF\xBF\xBF"}, {2, 0, 0, 4, "\xEF\xBF\xBF""b"}, {3, 0, 0, 5, "\xEF\xBF\xBF""bc"}, {4, 0, 0, 5, "\xEF\xBF\xBF""bc"},
				{0, 1, 1, 0, ""}, {1, 1, 1, 1, "b"           }, {2, 1, 1, 2, "bc"             }, {3, 1, 1, 2, "bc"              }, {4, 1, 1, 2, "bc"              },
				{0, 2, 2, 0, ""}, {1, 2, 2, 1, "c"           }, {2, 2, 2, 1, "c"              }, {3, 2, 2, 1, "c"               }, {4, 2, 2, 1, "c"               },
				{0, 3, 3, 0, ""}, {1, 3, 3, 0, ""            }, {2, 3, 3, 0, ""               }, {3, 3, 3, 0, ""                }, {4, 3, 3, 0, ""                },
				{0, 4, 3, 0, ""}, {1, 4, 3, 0, ""            }, {2, 4, 3, 0, ""               }, {3, 4, 3, 0, ""                }, {4, 4, 3, 0, ""                }
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
	{
		acpl::String::Ref oRef(L"a\xFFFF"L"c"); // U+FFFF == \xEF\xBF\xBF
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 1, "a"           }, {2, 0, 0, 4, "a\xEF\xBF\xBF"  }, {3, 0, 0, 5, "a\xEF\xBF\xBF""c"}, {4, 0, 0, 5, "a\xEF\xBF\xBF""c"},
				{0, 1, 1, 0, ""}, {1, 1, 1, 3, "\xEF\xBF\xBF"}, {2, 1, 1, 4, "\xEF\xBF\xBF""c"}, {3, 1, 1, 4, "\xEF\xBF\xBF""c" }, {4, 1, 1, 4, "\xEF\xBF\xBF""c" },
				{0, 2, 2, 0, ""}, {1, 2, 2, 1, "c"           }, {2, 2, 2, 1, "c"              }, {3, 2, 2, 1, "c"               }, {4, 2, 2, 1, "c"               },
				{0, 3, 3, 0, ""}, {1, 3, 3, 0, ""            }, {2, 3, 3, 0, ""               }, {3, 3, 3, 0, ""                }, {4, 3, 3, 0, ""                },
				{0, 4, 3, 0, ""}, {1, 4, 3, 0, ""            }, {2, 4, 3, 0, ""               }, {3, 4, 3, 0, ""                }, {4, 4, 3, 0, ""                }
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
	{
		acpl::String::Ref oRef(L"ab\xFFFF"); // U+FFFF == \xEF\xBF\xBF
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 1, "a"           }, {2, 0, 0, 2, "ab"           }, {3, 0, 0, 5, "ab\xEF\xBF\xBF"}, {4, 0, 0, 5, "ab\xEF\xBF\xBF"},
				{0, 1, 1, 0, ""}, {1, 1, 1, 1, "b"           }, {2, 1, 1, 4, "b\xEF\xBF\xBF"}, {3, 1, 1, 4, "b\xEF\xBF\xBF" }, {4, 1, 1, 4, "b\xEF\xBF\xBF" },
				{0, 2, 2, 0, ""}, {1, 2, 2, 3, "\xEF\xBF\xBF"}, {2, 2, 2, 3, "\xEF\xBF\xBF" }, {3, 2, 2, 3, "\xEF\xBF\xBF"  }, {4, 2, 2, 3, "\xEF\xBF\xBF"  },
				{0, 3, 3, 0, ""}, {1, 3, 3, 0, ""            }, {2, 3, 3, 0, ""             }, {3, 3, 3, 0, ""              }, {4, 3, 3, 0, ""              },
				{0, 4, 3, 0, ""}, {1, 4, 3, 0, ""            }, {2, 4, 3, 0, ""             }, {3, 4, 3, 0, ""              }, {4, 4, 3, 0, ""              }
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
	
	return 0;
}

static int TestRefMeasureConvertWcs_EncAs4ByteSeq()
{
	PrintFn();
	
	
#if   (acplCRuntime == acplCRuntimeGlibc)
	
	{
		acpl::String::Ref oRef(L"\x1FFFFF"L"bc"); // U+1FFFFF == \xF7\xBF\xBF\xBF
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 4, "\xF7\xBF\xBF\xBF"}, {2, 0, 0, 5, "\xF7\xBF\xBF\xBF""b"}, {3, 0, 0, 6, "\xF7\xBF\xBF\xBF""bc"}, {4, 0, 0, 6, "\xF7\xBF\xBF\xBF""bc"},
				{0, 1, 1, 0, ""}, {1, 1, 1, 1, "b"               }, {2, 1, 1, 2, "bc"                 }, {3, 1, 1, 2, "bc"                  }, {4, 1, 1, 2, "bc"                  },
				{0, 2, 2, 0, ""}, {1, 2, 2, 1, "c"               }, {2, 2, 2, 1, "c"                  }, {3, 2, 2, 1, "c"                   }, {4, 2, 2, 1, "c"                   },
				{0, 3, 3, 0, ""}, {1, 3, 3, 0, ""                }, {2, 3, 3, 0, ""                   }, {3, 3, 3, 0, ""                    }, {4, 3, 3, 0, ""                    },
				{0, 4, 3, 0, ""}, {1, 4, 3, 0, ""                }, {2, 4, 3, 0, ""                   }, {3, 4, 3, 0, ""                    }, {4, 4, 3, 0, ""                    }
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
	{
		acpl::String::Ref oRef(L"a\x1FFFFF"L"c"); // U+1FFFFF == \xF7\xBF\xBF\xBF
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 1, "a"               }, {2, 0, 0, 5, "a\xF7\xBF\xBF\xBF"  }, {3, 0, 0, 6, "a\xF7\xBF\xBF\xBF""c"}, {4, 0, 0, 6, "a\xF7\xBF\xBF\xBF""c"},
				{0, 1, 1, 0, ""}, {1, 1, 1, 4, "\xF7\xBF\xBF\xBF"}, {2, 1, 1, 5, "\xF7\xBF\xBF\xBF""c"}, {3, 1, 1, 5, "\xF7\xBF\xBF\xBF""c" }, {4, 1, 1, 5, "\xF7\xBF\xBF\xBF""c" },
				{0, 2, 2, 0, ""}, {1, 2, 2, 1, "c"               }, {2, 2, 2, 1, "c"                  }, {3, 2, 2, 1, "c"                   }, {4, 2, 2, 1, "c"                   },
				{0, 3, 3, 0, ""}, {1, 3, 3, 0, ""                }, {2, 3, 3, 0, ""                   }, {3, 3, 3, 0, ""                    }, {4, 3, 3, 0, ""                    },
				{0, 4, 3, 0, ""}, {1, 4, 3, 0, ""                }, {2, 4, 3, 0, ""                   }, {3, 4, 3, 0, ""                    }, {4, 4, 3, 0, ""                    }
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
	{
		acpl::String::Ref oRef(L"ab\x1FFFFF"); // U+1FFFFF == \xF7\xBF\xBF\xBF
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 1, "a"               }, {2, 0, 0, 2, "ab"               }, {3, 0, 0, 6, "ab\xF7\xBF\xBF\xBF"}, {4, 0, 0, 6, "ab\xF7\xBF\xBF\xBF"},
				{0, 1, 1, 0, ""}, {1, 1, 1, 1, "b"               }, {2, 1, 1, 5, "b\xF7\xBF\xBF\xBF"}, {3, 1, 1, 5, "b\xF7\xBF\xBF\xBF" }, {4, 1, 1, 5, "b\xF7\xBF\xBF\xBF" },
				{0, 2, 2, 0, ""}, {1, 2, 2, 4, "\xF7\xBF\xBF\xBF"}, {2, 2, 2, 4, "\xF7\xBF\xBF\xBF" }, {3, 2, 2, 4, "\xF7\xBF\xBF\xBF"  }, {4, 2, 2, 4, "\xF7\xBF\xBF\xBF"  },
				{0, 3, 3, 0, ""}, {1, 3, 3, 0, ""                }, {2, 3, 3, 0, ""                 }, {3, 3, 3, 0, ""                  }, {4, 3, 3, 0, ""                  },
				{0, 4, 3, 0, ""}, {1, 4, 3, 0, ""                }, {2, 4, 3, 0, ""                 }, {3, 4, 3, 0, ""                  }, {4, 4, 3, 0, ""                  }
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
#elif (acplCRuntime == acplCRuntimeMscrt)
	
	{
		acpl::String::Ref oRef(L"\xD800\xDC00"L"bc"); // \xD800\xDC00 (U+10000, first 2-element seqence in UTF-16) == \xF0\x90\x80\x80
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 4, "\xF0\x90\x80\x80"}, {2, 0, 0, 5, "\xF0\x90\x80\x80""b"}, {3, 0, 0, 6, "\xF0\x90\x80\x80""bc"}, {4, 0, 0, 6, "\xF0\x90\x80\x80""bc"},
				{0, 1, 2, 0, ""}, {1, 1, 2, 1, "b"               }, {2, 1, 2, 2, "bc"                 }, {3, 1, 2, 2, "bc"                  }, {4, 1, 2, 2, "bc"                  },
				{0, 2, 3, 0, ""}, {1, 2, 3, 1, "c"               }, {2, 2, 3, 1, "c"                  }, {3, 2, 3, 1, "c"                   }, {4, 2, 3, 1, "c"                   },
				{0, 3, 4, 0, ""}, {1, 3, 4, 0, ""                }, {2, 3, 4, 0, ""                   }, {3, 3, 4, 0, ""                    }, {4, 3, 4, 0, ""                    },
				{0, 4, 4, 0, ""}, {1, 4, 4, 0, ""                }, {2, 4, 4, 0, ""                   }, {3, 4, 4, 0, ""                    }, {4, 4, 4, 0, ""                    }
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
	{
		acpl::String::Ref oRef(L"a\xD800\xDC00"L"c"); // \xD800\xDC00 (U+10000, first 2-element seqence in UTF-16) == \xF0\x90\x80\x80
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 1, "a"               }, {2, 0, 0, 5, "a\xF0\x90\x80\x80"  }, {3, 0, 0, 6, "a\xF0\x90\x80\x80""c"}, {4, 0, 0, 6, "a\xF0\x90\x80\x80""c"},
				{0, 1, 1, 0, ""}, {1, 1, 1, 4, "\xF0\x90\x80\x80"}, {2, 1, 1, 5, "\xF0\x90\x80\x80""c"}, {3, 1, 1, 5, "\xF0\x90\x80\x80""c" }, {4, 1, 1, 5, "\xF0\x90\x80\x80""c" },
				{0, 2, 3, 0, ""}, {1, 2, 3, 1, "c"               }, {2, 2, 3, 1, "c"                  }, {3, 2, 3, 1, "c"                   }, {4, 2, 3, 1, "c"                   },
				{0, 3, 4, 0, ""}, {1, 3, 4, 0, ""                }, {2, 3, 4, 0, ""                   }, {3, 3, 4, 0, ""                    }, {4, 3, 4, 0, ""                    },
				{0, 4, 4, 0, ""}, {1, 4, 4, 0, ""                }, {2, 4, 4, 0, ""                   }, {3, 4, 4, 0, ""                    }, {4, 4, 4, 0, ""                    }
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
	{
		acpl::String::Ref oRef(L"ab\xD800\xDC00"); // \xD800\xDC00 (U+10000, first 2-element seqence in UTF-16) == \xF0\x90\x80\x80
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 1, "a"               }, {2, 0, 0, 2, "ab"               }, {3, 0, 0, 6, "ab\xF0\x90\x80\x80"}, {4, 0, 0, 6, "ab\xF0\x90\x80\x80"},
				{0, 1, 1, 0, ""}, {1, 1, 1, 1, "b"               }, {2, 1, 1, 5, "b\xF0\x90\x80\x80"}, {3, 1, 1, 5, "b\xF0\x90\x80\x80" }, {4, 1, 1, 5, "b\xF0\x90\x80\x80" },
				{0, 2, 2, 0, ""}, {1, 2, 2, 4, "\xF0\x90\x80\x80"}, {2, 2, 2, 4, "\xF0\x90\x80\x80" }, {3, 2, 2, 4, "\xF0\x90\x80\x80"  }, {4, 2, 2, 4, "\xF0\x90\x80\x80"  },
				{0, 3, 4, 0, ""}, {1, 3, 4, 0, ""                }, {2, 3, 4, 0, ""                 }, {3, 3, 4, 0, ""                  }, {4, 3, 4, 0, ""                  },
				{0, 4, 4, 0, ""}, {1, 4, 4, 0, ""                }, {2, 4, 4, 0, ""                 }, {3, 4, 4, 0, ""                  }, {4, 4, 4, 0, ""                  }
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
	{
		acpl::String::Ref oRef(L"\xDBFF\xDFFF"L"bc"); // \xDBFF\xDFFF (U+10FFFF, last 2-element seqence in UTF-16) == \xF4\x8F\xBF\xBF
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 4, "\xF4\x8F\xBF\xBF"}, {2, 0, 0, 5, "\xF4\x8F\xBF\xBF""b"}, {3, 0, 0, 6, "\xF4\x8F\xBF\xBF""bc"}, {4, 0, 0, 6, "\xF4\x8F\xBF\xBF""bc"},
				{0, 1, 2, 0, ""}, {1, 1, 2, 1, "b"               }, {2, 1, 2, 2, "bc"                 }, {3, 1, 2, 2, "bc"                  }, {4, 1, 2, 2, "bc"                  },
				{0, 2, 3, 0, ""}, {1, 2, 3, 1, "c"               }, {2, 2, 3, 1, "c"                  }, {3, 2, 3, 1, "c"                   }, {4, 2, 3, 1, "c"                   },
				{0, 3, 4, 0, ""}, {1, 3, 4, 0, ""                }, {2, 3, 4, 0, ""                   }, {3, 3, 4, 0, ""                    }, {4, 3, 4, 0, ""                    },
				{0, 4, 4, 0, ""}, {1, 4, 4, 0, ""                }, {2, 4, 4, 0, ""                   }, {3, 4, 4, 0, ""                    }, {4, 4, 4, 0, ""                    }
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
	{
		acpl::String::Ref oRef(L"a\xDBFF\xDFFF"L"c"); // \xDBFF\xDFFF (U+10FFFF, last 2-element seqence in UTF-16) == \xF4\x8F\xBF\xBF
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 1, "a"               }, {2, 0, 0, 5, "a\xF4\x8F\xBF\xBF"  }, {3, 0, 0, 6, "a\xF4\x8F\xBF\xBF""c"}, {4, 0, 0, 6, "a\xF4\x8F\xBF\xBF""c"},
				{0, 1, 1, 0, ""}, {1, 1, 1, 4, "\xF4\x8F\xBF\xBF"}, {2, 1, 1, 5, "\xF4\x8F\xBF\xBF""c"}, {3, 1, 1, 5, "\xF4\x8F\xBF\xBF""c" }, {4, 1, 1, 5, "\xF4\x8F\xBF\xBF""c" },
				{0, 2, 3, 0, ""}, {1, 2, 3, 1, "c"               }, {2, 2, 3, 1, "c"                  }, {3, 2, 3, 1, "c"                   }, {4, 2, 3, 1, "c"                   },
				{0, 3, 4, 0, ""}, {1, 3, 4, 0, ""                }, {2, 3, 4, 0, ""                   }, {3, 3, 4, 0, ""                    }, {4, 3, 4, 0, ""                    },
				{0, 4, 4, 0, ""}, {1, 4, 4, 0, ""                }, {2, 4, 4, 0, ""                   }, {3, 4, 4, 0, ""                    }, {4, 4, 4, 0, ""                    }
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
	{
		acpl::String::Ref oRef(L"ab\xDBFF\xDFFF"); // \xDBFF\xDFFF (U+10FFFF, last 2-element seqence in UTF-16) == \xF4\x8F\xBF\xBF
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 1, "a"               }, {2, 0, 0, 2, "ab"               }, {3, 0, 0, 6, "ab\xF4\x8F\xBF\xBF"}, {4, 0, 0, 6, "ab\xF4\x8F\xBF\xBF"},
				{0, 1, 1, 0, ""}, {1, 1, 1, 1, "b"               }, {2, 1, 1, 5, "b\xF4\x8F\xBF\xBF"}, {3, 1, 1, 5, "b\xF4\x8F\xBF\xBF" }, {4, 1, 1, 5, "b\xF4\x8F\xBF\xBF" },
				{0, 2, 2, 0, ""}, {1, 2, 2, 4, "\xF4\x8F\xBF\xBF"}, {2, 2, 2, 4, "\xF4\x8F\xBF\xBF" }, {3, 2, 2, 4, "\xF4\x8F\xBF\xBF"  }, {4, 2, 2, 4, "\xF4\x8F\xBF\xBF"  },
				{0, 3, 4, 0, ""}, {1, 3, 4, 0, ""                }, {2, 3, 4, 0, ""                 }, {3, 3, 4, 0, ""                  }, {4, 3, 4, 0, ""                  },
				{0, 4, 4, 0, ""}, {1, 4, 4, 0, ""                }, {2, 4, 4, 0, ""                 }, {3, 4, 4, 0, ""                  }, {4, 4, 4, 0, ""                  }
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
	// Invalid seqence
	
	{
		acpl::String::Ref oRef(L"\xD800"L"bc"); // \xD800 (invalid 2-element seqence in UTF-16) == '?'
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 1, "?"}, {2, 0, 0, 2, "?b"}, {3, 0, 0, 3, "?bc"}, {4, 0, 0, 3, "?bc"},
				{0, 1, 1, 0, ""}, {1, 1, 1, 1, "b"}, {2, 1, 1, 2, "bc"}, {3, 1, 1, 2, "bc" }, {4, 1, 1, 2, "bc" },
				{0, 2, 2, 0, ""}, {1, 2, 2, 1, "c"}, {2, 2, 2, 1, "c" }, {3, 2, 2, 1, "c"  }, {4, 2, 2, 1, "c"  },
				{0, 3, 3, 0, ""}, {1, 3, 3, 0, "" }, {2, 3, 3, 0, ""  }, {3, 3, 3, 0, ""   }, {4, 3, 3, 0, ""   },
				{0, 4, 3, 0, ""}, {1, 4, 3, 0, "" }, {2, 4, 3, 0, ""  }, {3, 4, 3, 0, ""   }, {4, 4, 3, 0, ""   }
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
	{
		acpl::String::Ref oRef(L"a\xD800"L"c"); // \xD800 (invalid 2-element seqence in UTF-16) == '?'
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 1, "a"}, {2, 0, 0, 2, "a?"}, {3, 0, 0, 3, "a?c"}, {4, 0, 0, 3, "a?c"},
				{0, 1, 1, 0, ""}, {1, 1, 1, 1, "?"}, {2, 1, 1, 2, "?c"}, {3, 1, 1, 2, "?c" }, {4, 1, 1, 2, "?c" },
				{0, 2, 2, 0, ""}, {1, 2, 2, 1, "c"}, {2, 2, 2, 1, "c" }, {3, 2, 2, 1, "c"  }, {4, 2, 2, 1, "c"  },
				{0, 3, 3, 0, ""}, {1, 3, 3, 0, "" }, {2, 3, 3, 0, ""  }, {3, 3, 3, 0, ""   }, {4, 3, 3, 0, ""   },
				{0, 4, 3, 0, ""}, {1, 4, 3, 0, "" }, {2, 4, 3, 0, ""  }, {3, 4, 3, 0, ""   }, {4, 4, 3, 0, ""   }
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
	{
		acpl::String::Ref oRef(L"ab\xD800"); // \xD800 (invalid 2-element seqence in UTF-16) == '?'
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 1, "a"}, {2, 0, 0, 2, "ab"}, {3, 0, 0, 3, "ab?"}, {4, 0, 0, 3, "ab?"},
				{0, 1, 1, 0, ""}, {1, 1, 1, 1, "b"}, {2, 1, 1, 2, "b?"}, {3, 1, 1, 2, "b?" }, {4, 1, 1, 2, "b?" },
				{0, 2, 2, 0, ""}, {1, 2, 2, 1, "?"}, {2, 2, 2, 1, "?" }, {3, 2, 2, 1, "?"  }, {4, 2, 2, 1, "?"  },
				{0, 3, 3, 0, ""}, {1, 3, 3, 0, "" }, {2, 3, 3, 0, ""  }, {3, 3, 3, 0, ""   }, {4, 3, 3, 0, ""   },
				{0, 4, 3, 0, ""}, {1, 4, 3, 0, "" }, {2, 4, 3, 0, ""  }, {3, 4, 3, 0, ""   }, {4, 4, 3, 0, ""   }
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
#else
#	error Add tests for this C library
#endif
	
	
	return 0;
}

static int TestRefMeasureConvertWcs_EncAs5ByteSeq()
{
	PrintFn();
	
	
#if   (acplCRuntime == acplCRuntimeGlibc)
	
	{
		acpl::String::Ref oRef(L"\x3FFFFFF"L"bc"); // U+3FFFFFF == \xFB\xBF\xBF\xBF\xBF
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 5, "\xFB\xBF\xBF\xBF\xBF"}, {2, 0, 0, 6, "\xFB\xBF\xBF\xBF\xBF""b"}, {3, 0, 0, 7, "\xFB\xBF\xBF\xBF\xBF""bc"}, {4, 0, 0, 7, "\xFB\xBF\xBF\xBF\xBF""bc"},
				{0, 1, 1, 0, ""}, {1, 1, 1, 1, "b"                   }, {2, 1, 1, 2, "bc"                     }, {3, 1, 1, 2, "bc"                      }, {4, 1, 1, 2, "bc"                      },
				{0, 2, 2, 0, ""}, {1, 2, 2, 1, "c"                   }, {2, 2, 2, 1, "c"                      }, {3, 2, 2, 1, "c"                       }, {4, 2, 2, 1, "c"                       },
				{0, 3, 3, 0, ""}, {1, 3, 3, 0, ""                    }, {2, 3, 3, 0, ""                       }, {3, 3, 3, 0, ""                        }, {4, 3, 3, 0, ""                        },
				{0, 4, 3, 0, ""}, {1, 4, 3, 0, ""                    }, {2, 4, 3, 0, ""                       }, {3, 4, 3, 0, ""                        }, {4, 4, 3, 0, ""                        }
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
	{
		acpl::String::Ref oRef(L"a\x3FFFFFF"L"c"); // U+3FFFFFF == \xFB\xBF\xBF\xBF\xBF
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 1, "a"                   }, {2, 0, 0, 6, "a\xFB\xBF\xBF\xBF\xBF"  }, {3, 0, 0, 7, "a\xFB\xBF\xBF\xBF\xBF""c"}, {4, 0, 0, 7, "a\xFB\xBF\xBF\xBF\xBF""c"},
				{0, 1, 1, 0, ""}, {1, 1, 1, 5, "\xFB\xBF\xBF\xBF\xBF"}, {2, 1, 1, 6, "\xFB\xBF\xBF\xBF\xBF""c"}, {3, 1, 1, 6, "\xFB\xBF\xBF\xBF\xBF""c" }, {4, 1, 1, 6, "\xFB\xBF\xBF\xBF\xBF""c" },
				{0, 2, 2, 0, ""}, {1, 2, 2, 1, "c"                   }, {2, 2, 2, 1, "c"                      }, {3, 2, 2, 1, "c"                       }, {4, 2, 2, 1, "c"                       },
				{0, 3, 3, 0, ""}, {1, 3, 3, 0, ""                    }, {2, 3, 3, 0, ""                       }, {3, 3, 3, 0, ""                        }, {4, 3, 3, 0, ""                        },
				{0, 4, 3, 0, ""}, {1, 4, 3, 0, ""                    }, {2, 4, 3, 0, ""                       }, {3, 4, 3, 0, ""                        }, {4, 4, 3, 0, ""                        }
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
	{
		acpl::String::Ref oRef(L"ab\x3FFFFFF"); // U+3FFFFFF == \xFB\xBF\xBF\xBF\xBF
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 1, "a"                   }, {2, 0, 0, 2, "ab"                   }, {3, 0, 0, 7, "ab\xFB\xBF\xBF\xBF\xBF"}, {4, 0, 0, 7, "ab\xFB\xBF\xBF\xBF\xBF"},
				{0, 1, 1, 0, ""}, {1, 1, 1, 1, "b"                   }, {2, 1, 1, 6, "b\xFB\xBF\xBF\xBF\xBF"}, {3, 1, 1, 6, "b\xFB\xBF\xBF\xBF\xBF" }, {4, 1, 1, 6, "b\xFB\xBF\xBF\xBF\xBF" },
				{0, 2, 2, 0, ""}, {1, 2, 2, 5, "\xFB\xBF\xBF\xBF\xBF"}, {2, 2, 2, 5, "\xFB\xBF\xBF\xBF\xBF" }, {3, 2, 2, 5, "\xFB\xBF\xBF\xBF\xBF"  }, {4, 2, 2, 5, "\xFB\xBF\xBF\xBF\xBF"  },
				{0, 3, 3, 0, ""}, {1, 3, 3, 0, ""                    }, {2, 3, 3, 0, ""                     }, {3, 3, 3, 0, ""                      }, {4, 3, 3, 0, ""                      },
				{0, 4, 3, 0, ""}, {1, 4, 3, 0, ""                    }, {2, 4, 3, 0, ""                     }, {3, 4, 3, 0, ""                      }, {4, 4, 3, 0, ""                      }
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
#elif (acplCRuntime == acplCRuntimeMscrt)
	// No tests necessary as UTF-16 values do not go as high as to occupy more than 4-bytes in UTF-8
#else
#	error Add tests for this C library
#endif
	
	
	return 0;
}

static int TestRefMeasureConvertWcs_EncAs6ByteSeq()
{
	PrintFn();
	
	
#if   (acplCRuntime == acplCRuntimeGlibc)
	
	{
		acpl::String::Ref oRef(L"\x7FFFFFFF"L"bc"); // U+7FFFFFFF == \xFD\xBF\xBF\xBF\xBF\xBF
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 6, "\xFD\xBF\xBF\xBF\xBF\xBF"}, {2, 0, 0, 7, "\xFD\xBF\xBF\xBF\xBF\xBF""b"}, {3, 0, 0, 8, "\xFD\xBF\xBF\xBF\xBF\xBF""bc"}, {4, 0, 0, 8, "\xFD\xBF\xBF\xBF\xBF\xBF""bc"},
				{0, 1, 1, 0, ""}, {1, 1, 1, 1, "b"                       }, {2, 1, 1, 2, "bc"                         }, {3, 1, 1, 2, "bc"                          }, {4, 1, 1, 2, "bc"                          },
				{0, 2, 2, 0, ""}, {1, 2, 2, 1, "c"                       }, {2, 2, 2, 1, "c"                          }, {3, 2, 2, 1, "c"                           }, {4, 2, 2, 1, "c"                           },
				{0, 3, 3, 0, ""}, {1, 3, 3, 0, ""                        }, {2, 3, 3, 0, ""                           }, {3, 3, 3, 0, ""                            }, {4, 3, 3, 0, ""                            },
				{0, 4, 3, 0, ""}, {1, 4, 3, 0, ""                        }, {2, 4, 3, 0, ""                           }, {3, 4, 3, 0, ""                            }, {4, 4, 3, 0, ""                            }
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
	{
		acpl::String::Ref oRef(L"a\x7FFFFFFF"L"c"); // U+7FFFFFFF == \xFD\xBF\xBF\xBF\xBF\xBF
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 1, "a"                       }, {2, 0, 0, 7, "a\xFD\xBF\xBF\xBF\xBF\xBF"  }, {3, 0, 0, 8, "a\xFD\xBF\xBF\xBF\xBF\xBF""c"}, {4, 0, 0, 8, "a\xFD\xBF\xBF\xBF\xBF\xBF""c"},
				{0, 1, 1, 0, ""}, {1, 1, 1, 6, "\xFD\xBF\xBF\xBF\xBF\xBF"}, {2, 1, 1, 7, "\xFD\xBF\xBF\xBF\xBF\xBF""c"}, {3, 1, 1, 7, "\xFD\xBF\xBF\xBF\xBF\xBF""c" }, {4, 1, 1, 7, "\xFD\xBF\xBF\xBF\xBF\xBF""c" },
				{0, 2, 2, 0, ""}, {1, 2, 2, 1, "c"                       }, {2, 2, 2, 1, "c"                          }, {3, 2, 2, 1, "c"                           }, {4, 2, 2, 1, "c"                           },
				{0, 3, 3, 0, ""}, {1, 3, 3, 0, ""                        }, {2, 3, 3, 0, ""                           }, {3, 3, 3, 0, ""                            }, {4, 3, 3, 0, ""                            },
				{0, 4, 3, 0, ""}, {1, 4, 3, 0, ""                        }, {2, 4, 3, 0, ""                           }, {3, 4, 3, 0, ""                            }, {4, 4, 3, 0, ""                            }
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
	{
		acpl::String::Ref oRef(L"ab\x7FFFFFFF"); // U+7FFFFFFF == \xFD\xBF\xBF\xBF\xBF\xBF
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 1, "a"                       }, {2, 0, 0, 2, "ab"                       }, {3, 0, 0, 8, "ab\xFD\xBF\xBF\xBF\xBF\xBF"}, {4, 0, 0, 8, "ab\xFD\xBF\xBF\xBF\xBF\xBF"},
				{0, 1, 1, 0, ""}, {1, 1, 1, 1, "b"                       }, {2, 1, 1, 7, "b\xFD\xBF\xBF\xBF\xBF\xBF"}, {3, 1, 1, 7, "b\xFD\xBF\xBF\xBF\xBF\xBF" }, {4, 1, 1, 7, "b\xFD\xBF\xBF\xBF\xBF\xBF" },
				{0, 2, 2, 0, ""}, {1, 2, 2, 6, "\xFD\xBF\xBF\xBF\xBF\xBF"}, {2, 2, 2, 6, "\xFD\xBF\xBF\xBF\xBF\xBF" }, {3, 2, 2, 6, "\xFD\xBF\xBF\xBF\xBF\xBF"  }, {4, 2, 2, 6, "\xFD\xBF\xBF\xBF\xBF\xBF"  },
				{0, 3, 3, 0, ""}, {1, 3, 3, 0, ""                        }, {2, 3, 3, 0, ""                         }, {3, 3, 3, 0, ""                          }, {4, 3, 3, 0, ""                          },
				{0, 4, 3, 0, ""}, {1, 4, 3, 0, ""                        }, {2, 4, 3, 0, ""                         }, {3, 4, 3, 0, ""                          }, {4, 4, 3, 0, ""                          }
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
	// Over the limit of UTF-8
	
	{
		acpl::String::Ref oRef(L"\x80000000"L"bc"); // U+80000000 (too high for UTF-8) == '?'
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 1, "?"}, {2, 0, 0, 2, "?b"}, {3, 0, 0, 3, "?bc"}, {4, 0, 0, 3, "?bc"},
				{0, 1, 1, 0, ""}, {1, 1, 1, 1, "b"}, {2, 1, 1, 2, "bc"}, {3, 1, 1, 2, "bc" }, {4, 1, 1, 2, "bc" },
				{0, 2, 2, 0, ""}, {1, 2, 2, 1, "c"}, {2, 2, 2, 1, "c" }, {3, 2, 2, 1, "c"  }, {4, 2, 2, 1, "c"  },
				{0, 3, 3, 0, ""}, {1, 3, 3, 0, "" }, {2, 3, 3, 0, ""  }, {3, 3, 3, 0, ""   }, {4, 3, 3, 0, ""   },
				{0, 4, 3, 0, ""}, {1, 4, 3, 0, "" }, {2, 4, 3, 0, ""  }, {3, 4, 3, 0, ""   }, {4, 4, 3, 0, ""   }
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
	{
		acpl::String::Ref oRef(L"a\x80000000"L"c"); // U+80000000 (too high for UTF-8) == '?'
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 1, "a"}, {2, 0, 0, 2, "a?"}, {3, 0, 0, 3, "a?c"}, {4, 0, 0, 3, "a?c"},
				{0, 1, 1, 0, ""}, {1, 1, 1, 1, "?"}, {2, 1, 1, 2, "?c"}, {3, 1, 1, 2, "?c" }, {4, 1, 1, 2, "?c" },
				{0, 2, 2, 0, ""}, {1, 2, 2, 1, "c"}, {2, 2, 2, 1, "c" }, {3, 2, 2, 1, "c"  }, {4, 2, 2, 1, "c"  },
				{0, 3, 3, 0, ""}, {1, 3, 3, 0, "" }, {2, 3, 3, 0, ""  }, {3, 3, 3, 0, ""   }, {4, 3, 3, 0, ""   },
				{0, 4, 3, 0, ""}, {1, 4, 3, 0, "" }, {2, 4, 3, 0, ""  }, {3, 4, 3, 0, ""   }, {4, 4, 3, 0, ""   }
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
	{
		acpl::String::Ref oRef(L"ab\x80000000"); // U+80000000 (too high for UTF-8) == '?'
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 1, "a"}, {2, 0, 0, 2, "ab"}, {3, 0, 0, 3, "ab?"}, {4, 0, 0, 3, "ab?"},
				{0, 1, 1, 0, ""}, {1, 1, 1, 1, "b"}, {2, 1, 1, 2, "b?"}, {3, 1, 1, 2, "b?" }, {4, 1, 1, 2, "b?" },
				{0, 2, 2, 0, ""}, {1, 2, 2, 1, "?"}, {2, 2, 2, 1, "?" }, {3, 2, 2, 1, "?"  }, {4, 2, 2, 1, "?"  },
				{0, 3, 3, 0, ""}, {1, 3, 3, 0, "" }, {2, 3, 3, 0, ""  }, {3, 3, 3, 0, ""   }, {4, 3, 3, 0, ""   },
				{0, 4, 3, 0, ""}, {1, 4, 3, 0, "" }, {2, 4, 3, 0, ""  }, {3, 4, 3, 0, ""   }, {4, 4, 3, 0, ""   }
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
#elif (acplCRuntime == acplCRuntimeMscrt)
	// No tests necessary as UTF-16 values do not go as high as to occupy more than 4-bytes in UTF-8
#else
#	error Add tests for this C library
#endif
	
	
	return 0;
}

static int TestRefMeasureConvertWcs()
{
	PrintFn();
	
	
	acpl::SizeT oRefOff;
	acpl::SizeT oEncByteSize;
	
	
	// Empty
	
	{
		acpl::String::Ref oRef(reinterpret_cast<const wchar_t *>(NULL));
		Test(oRef.GetType() == acpl::String::Ref::rtWcs);
		oRef.Measure(acpl::Num<acpl::SizeT>::Max(), 0, oRefOff, oEncByteSize);
		Test(oRefOff == 0);
		Test(oEncByteSize == 0);
	}
	
	
	Test(TestRefMeasureConvertWcs_EncAs1ByteSeq() == 0);
	Test(TestRefMeasureConvertWcs_EncAs2ByteSeq() == 0);
	Test(TestRefMeasureConvertWcs_EncAs3ByteSeq() == 0);
	Test(TestRefMeasureConvertWcs_EncAs4ByteSeq() == 0);
	Test(TestRefMeasureConvertWcs_EncAs5ByteSeq() == 0);
	Test(TestRefMeasureConvertWcs_EncAs6ByteSeq() == 0);
	
	
	return 0;
}

static int TestRefMeasureConvertUcs_EncAs1ByteSeq()
{
	PrintFn();
	
	
	{
		const acpl::Unichar oUcs[] = { 'a', 'b', 'c', '\0' };
		acpl::String::Ref oRef(oUcs);
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 1, "a"}, {2, 0, 0, 2, "ab"}, {3, 0, 0, 3, "abc"}, {4, 0, 0, 3, "abc"},
				{0, 1, 1, 0, ""}, {1, 1, 1, 1, "b"}, {2, 1, 1, 2, "bc"}, {3, 1, 1, 2, "bc" }, {4, 1, 1, 2, "bc" },
				{0, 2, 2, 0, ""}, {1, 2, 2, 1, "c"}, {2, 2, 2, 1, "c" }, {3, 2, 2, 1, "c"  }, {4, 2, 2, 1, "c"  },
				{0, 3, 3, 0, ""}, {1, 3, 3, 0, "" }, {2, 3, 3, 0, ""  }, {3, 3, 3, 0, ""   }, {4, 3, 3, 0, ""   },
				{0, 4, 3, 0, ""}, {1, 4, 3, 0, "" }, {2, 4, 3, 0, ""  }, {3, 4, 3, 0, ""   }, {4, 4, 3, 0, ""   }
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
	
	return 0;
}

static int TestRefMeasureConvertUcs_EncAs2ByteSeq()
{
	PrintFn();
	
	
	{
		const acpl::Unichar oUcs[] = { 0x7FF, 'b', 'c', '\0' };  // U+7FF == \xDF\xBF
		acpl::String::Ref oRef(oUcs);
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 2, "\xDF\xBF"}, {2, 0, 0, 3, "\xDF\xBF""b"}, {3, 0, 0, 4, "\xDF\xBF""bc"}, {4, 0, 0, 4, "\xDF\xBF""bc"},
				{0, 1, 1, 0, ""}, {1, 1, 1, 1, "b"       }, {2, 1, 1, 2, "bc"         }, {3, 1, 1, 2, "bc"          }, {4, 1, 1, 2, "bc"          },
				{0, 2, 2, 0, ""}, {1, 2, 2, 1, "c"       }, {2, 2, 2, 1, "c"          }, {3, 2, 2, 1, "c"           }, {4, 2, 2, 1, "c"           },
				{0, 3, 3, 0, ""}, {1, 3, 3, 0, ""        }, {2, 3, 3, 0, ""           }, {3, 3, 3, 0, ""            }, {4, 3, 3, 0, ""            },
				{0, 4, 3, 0, ""}, {1, 4, 3, 0, ""        }, {2, 4, 3, 0, ""           }, {3, 4, 3, 0, ""            }, {4, 4, 3, 0, ""            }
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
	{
		const acpl::Unichar oUcs[] = { 'a', 0x7FF, 'c', '\0' };  // U+7FF == \xDF\xBF
		acpl::String::Ref oRef(oUcs);
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 1, "a"       }, {2, 0, 0, 3, "a\xDF\xBF"  }, {3, 0, 0, 4, "a\xDF\xBF""c"}, {4, 0, 0, 4, "a\xDF\xBF""c"},
				{0, 1, 1, 0, ""}, {1, 1, 1, 2, "\xDF\xBF"}, {2, 1, 1, 3, "\xDF\xBF""c"}, {3, 1, 1, 3, "\xDF\xBF""c" }, {4, 1, 1, 3, "\xDF\xBF""c" },
				{0, 2, 2, 0, ""}, {1, 2, 2, 1, "c"       }, {2, 2, 2, 1, "c"          }, {3, 2, 2, 1, "c"           }, {4, 2, 2, 1, "c"           },
				{0, 3, 3, 0, ""}, {1, 3, 3, 0, ""        }, {2, 3, 3, 0, ""           }, {3, 3, 3, 0, ""            }, {4, 3, 3, 0, ""            },
				{0, 4, 3, 0, ""}, {1, 4, 3, 0, ""        }, {2, 4, 3, 0, ""           }, {3, 4, 3, 0, ""            }, {4, 4, 3, 0, ""            }
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
	{
		const acpl::Unichar oUcs[] = { 'a', 'b', 0x7FF, '\0' };  // U+7FF == \xDF\xBF
		acpl::String::Ref oRef(oUcs);
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 1, "a"       }, {2, 0, 0, 2, "ab"       }, {3, 0, 0, 4, "ab\xDF\xBF"}, {4, 0, 0, 4, "ab\xDF\xBF"},
				{0, 1, 1, 0, ""}, {1, 1, 1, 1, "b"       }, {2, 1, 1, 3, "b\xDF\xBF"}, {3, 1, 1, 3, "b\xDF\xBF" }, {4, 1, 1, 3, "b\xDF\xBF" },
				{0, 2, 2, 0, ""}, {1, 2, 2, 2, "\xDF\xBF"}, {2, 2, 2, 2, "\xDF\xBF" }, {3, 2, 2, 2, "\xDF\xBF"  }, {4, 2, 2, 2, "\xDF\xBF"  },
				{0, 3, 3, 0, ""}, {1, 3, 3, 0, ""        }, {2, 3, 3, 0, ""         }, {3, 3, 3, 0, ""          }, {4, 3, 3, 0, ""          },
				{0, 4, 3, 0, ""}, {1, 4, 3, 0, ""        }, {2, 4, 3, 0, ""         }, {3, 4, 3, 0, ""          }, {4, 4, 3, 0, ""          }
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
	
	return 0;
}

static int TestRefMeasureConvertUcs_EncAs3ByteSeq()
{
	PrintFn();
	
	
	{
		const acpl::Unichar oUcs[] = { 0xFFFF, 'b', 'c', '\0' };  // U+FFFF == \xEF\xBF\xBF
		acpl::String::Ref oRef(oUcs);
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 3, "\xEF\xBF\xBF"}, {2, 0, 0, 4, "\xEF\xBF\xBF""b"}, {3, 0, 0, 5, "\xEF\xBF\xBF""bc"}, {4, 0, 0, 5, "\xEF\xBF\xBF""bc"},
				{0, 1, 1, 0, ""}, {1, 1, 1, 1, "b"           }, {2, 1, 1, 2, "bc"             }, {3, 1, 1, 2, "bc"              }, {4, 1, 1, 2, "bc"              },
				{0, 2, 2, 0, ""}, {1, 2, 2, 1, "c"           }, {2, 2, 2, 1, "c"              }, {3, 2, 2, 1, "c"               }, {4, 2, 2, 1, "c"               },
				{0, 3, 3, 0, ""}, {1, 3, 3, 0, ""            }, {2, 3, 3, 0, ""               }, {3, 3, 3, 0, ""                }, {4, 3, 3, 0, ""                },
				{0, 4, 3, 0, ""}, {1, 4, 3, 0, ""            }, {2, 4, 3, 0, ""               }, {3, 4, 3, 0, ""                }, {4, 4, 3, 0, ""                }
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
	{
		const acpl::Unichar oUcs[] = { 'a', 0xFFFF, 'c', '\0' };  // U+FFFF == \xEF\xBF\xBF
		acpl::String::Ref oRef(oUcs);
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 1, "a"           }, {2, 0, 0, 4, "a\xEF\xBF\xBF"  }, {3, 0, 0, 5, "a\xEF\xBF\xBF""c"}, {4, 0, 0, 5, "a\xEF\xBF\xBF""c"},
				{0, 1, 1, 0, ""}, {1, 1, 1, 3, "\xEF\xBF\xBF"}, {2, 1, 1, 4, "\xEF\xBF\xBF""c"}, {3, 1, 1, 4, "\xEF\xBF\xBF""c" }, {4, 1, 1, 4, "\xEF\xBF\xBF""c" },
				{0, 2, 2, 0, ""}, {1, 2, 2, 1, "c"           }, {2, 2, 2, 1, "c"              }, {3, 2, 2, 1, "c"               }, {4, 2, 2, 1, "c"               },
				{0, 3, 3, 0, ""}, {1, 3, 3, 0, ""            }, {2, 3, 3, 0, ""               }, {3, 3, 3, 0, ""                }, {4, 3, 3, 0, ""                },
				{0, 4, 3, 0, ""}, {1, 4, 3, 0, ""            }, {2, 4, 3, 0, ""               }, {3, 4, 3, 0, ""                }, {4, 4, 3, 0, ""                }
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
	{
		const acpl::Unichar oUcs[] = { 'a', 'b', 0xFFFF, '\0' };  // U+FFFF == \xEF\xBF\xBF
		acpl::String::Ref oRef(oUcs);
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 1, "a"           }, {2, 0, 0, 2, "ab"           }, {3, 0, 0, 5, "ab\xEF\xBF\xBF"}, {4, 0, 0, 5, "ab\xEF\xBF\xBF"},
				{0, 1, 1, 0, ""}, {1, 1, 1, 1, "b"           }, {2, 1, 1, 4, "b\xEF\xBF\xBF"}, {3, 1, 1, 4, "b\xEF\xBF\xBF" }, {4, 1, 1, 4, "b\xEF\xBF\xBF" },
				{0, 2, 2, 0, ""}, {1, 2, 2, 3, "\xEF\xBF\xBF"}, {2, 2, 2, 3, "\xEF\xBF\xBF" }, {3, 2, 2, 3, "\xEF\xBF\xBF"  }, {4, 2, 2, 3, "\xEF\xBF\xBF"  },
				{0, 3, 3, 0, ""}, {1, 3, 3, 0, ""            }, {2, 3, 3, 0, ""             }, {3, 3, 3, 0, ""              }, {4, 3, 3, 0, ""              },
				{0, 4, 3, 0, ""}, {1, 4, 3, 0, ""            }, {2, 4, 3, 0, ""             }, {3, 4, 3, 0, ""              }, {4, 4, 3, 0, ""              }
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
	
	return 0;
}

static int TestRefMeasureConvertUcs_EncAs4ByteSeq()
{
	PrintFn();
	
	
	{
		const acpl::Unichar oUcs[] = { 0x1FFFFF, 'b', 'c', '\0' };  // U+1FFFFF == \xF7\xBF\xBF\xBF
		acpl::String::Ref oRef(oUcs);
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 4, "\xF7\xBF\xBF\xBF"}, {2, 0, 0, 5, "\xF7\xBF\xBF\xBF""b"}, {3, 0, 0, 6, "\xF7\xBF\xBF\xBF""bc"}, {4, 0, 0, 6, "\xF7\xBF\xBF\xBF""bc"},
				{0, 1, 1, 0, ""}, {1, 1, 1, 1, "b"               }, {2, 1, 1, 2, "bc"                 }, {3, 1, 1, 2, "bc"                  }, {4, 1, 1, 2, "bc"                  },
				{0, 2, 2, 0, ""}, {1, 2, 2, 1, "c"               }, {2, 2, 2, 1, "c"                  }, {3, 2, 2, 1, "c"                   }, {4, 2, 2, 1, "c"                   },
				{0, 3, 3, 0, ""}, {1, 3, 3, 0, ""                }, {2, 3, 3, 0, ""                   }, {3, 3, 3, 0, ""                    }, {4, 3, 3, 0, ""                    },
				{0, 4, 3, 0, ""}, {1, 4, 3, 0, ""                }, {2, 4, 3, 0, ""                   }, {3, 4, 3, 0, ""                    }, {4, 4, 3, 0, ""                    }
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
	{
		const acpl::Unichar oUcs[] = { 'a', 0x1FFFFF, 'c', '\0' };  // U+1FFFFF == \xF7\xBF\xBF\xBF
		acpl::String::Ref oRef(oUcs);
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 1, "a"               }, {2, 0, 0, 5, "a\xF7\xBF\xBF\xBF"  }, {3, 0, 0, 6, "a\xF7\xBF\xBF\xBF""c"}, {4, 0, 0, 6, "a\xF7\xBF\xBF\xBF""c"},
				{0, 1, 1, 0, ""}, {1, 1, 1, 4, "\xF7\xBF\xBF\xBF"}, {2, 1, 1, 5, "\xF7\xBF\xBF\xBF""c"}, {3, 1, 1, 5, "\xF7\xBF\xBF\xBF""c" }, {4, 1, 1, 5, "\xF7\xBF\xBF\xBF""c" },
				{0, 2, 2, 0, ""}, {1, 2, 2, 1, "c"               }, {2, 2, 2, 1, "c"                  }, {3, 2, 2, 1, "c"                   }, {4, 2, 2, 1, "c"                   },
				{0, 3, 3, 0, ""}, {1, 3, 3, 0, ""                }, {2, 3, 3, 0, ""                   }, {3, 3, 3, 0, ""                    }, {4, 3, 3, 0, ""                    },
				{0, 4, 3, 0, ""}, {1, 4, 3, 0, ""                }, {2, 4, 3, 0, ""                   }, {3, 4, 3, 0, ""                    }, {4, 4, 3, 0, ""                    }
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
	{
		const acpl::Unichar oUcs[] = { 'a', 'b', 0x1FFFFF, '\0' };  // U+1FFFFF == \xF7\xBF\xBF\xBF
		acpl::String::Ref oRef(oUcs);
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 1, "a"               }, {2, 0, 0, 2, "ab"               }, {3, 0, 0, 6, "ab\xF7\xBF\xBF\xBF"}, {4, 0, 0, 6, "ab\xF7\xBF\xBF\xBF"},
				{0, 1, 1, 0, ""}, {1, 1, 1, 1, "b"               }, {2, 1, 1, 5, "b\xF7\xBF\xBF\xBF"}, {3, 1, 1, 5, "b\xF7\xBF\xBF\xBF" }, {4, 1, 1, 5, "b\xF7\xBF\xBF\xBF" },
				{0, 2, 2, 0, ""}, {1, 2, 2, 4, "\xF7\xBF\xBF\xBF"}, {2, 2, 2, 4, "\xF7\xBF\xBF\xBF" }, {3, 2, 2, 4, "\xF7\xBF\xBF\xBF"  }, {4, 2, 2, 4, "\xF7\xBF\xBF\xBF"  },
				{0, 3, 3, 0, ""}, {1, 3, 3, 0, ""                }, {2, 3, 3, 0, ""                 }, {3, 3, 3, 0, ""                  }, {4, 3, 3, 0, ""                  },
				{0, 4, 3, 0, ""}, {1, 4, 3, 0, ""                }, {2, 4, 3, 0, ""                 }, {3, 4, 3, 0, ""                  }, {4, 4, 3, 0, ""                  }
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
	
	return 0;
}

static int TestRefMeasureConvertUcs_EncAs5ByteSeq()
{
	PrintFn();
	
	
	{
		const acpl::Unichar oUcs[] = { 0x3FFFFFF, 'b', 'c', '\0' };  // U+3FFFFFF == \xFB\xBF\xBF\xBF\xBF
		acpl::String::Ref oRef(oUcs);
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 5, "\xFB\xBF\xBF\xBF\xBF"}, {2, 0, 0, 6, "\xFB\xBF\xBF\xBF\xBF""b"}, {3, 0, 0, 7, "\xFB\xBF\xBF\xBF\xBF""bc"}, {4, 0, 0, 7, "\xFB\xBF\xBF\xBF\xBF""bc"},
				{0, 1, 1, 0, ""}, {1, 1, 1, 1, "b"                   }, {2, 1, 1, 2, "bc"                     }, {3, 1, 1, 2, "bc"                      }, {4, 1, 1, 2, "bc"                      },
				{0, 2, 2, 0, ""}, {1, 2, 2, 1, "c"                   }, {2, 2, 2, 1, "c"                      }, {3, 2, 2, 1, "c"                       }, {4, 2, 2, 1, "c"                       },
				{0, 3, 3, 0, ""}, {1, 3, 3, 0, ""                    }, {2, 3, 3, 0, ""                       }, {3, 3, 3, 0, ""                        }, {4, 3, 3, 0, ""                        },
				{0, 4, 3, 0, ""}, {1, 4, 3, 0, ""                    }, {2, 4, 3, 0, ""                       }, {3, 4, 3, 0, ""                        }, {4, 4, 3, 0, ""                        }
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
	{
		const acpl::Unichar oUcs[] = { 'a', 0x3FFFFFF, 'c', '\0' };  // U+3FFFFFF == \xFB\xBF\xBF\xBF\xBF
		acpl::String::Ref oRef(oUcs);
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 1, "a"                   }, {2, 0, 0, 6, "a\xFB\xBF\xBF\xBF\xBF"  }, {3, 0, 0, 7, "a\xFB\xBF\xBF\xBF\xBF""c"}, {4, 0, 0, 7, "a\xFB\xBF\xBF\xBF\xBF""c"},
				{0, 1, 1, 0, ""}, {1, 1, 1, 5, "\xFB\xBF\xBF\xBF\xBF"}, {2, 1, 1, 6, "\xFB\xBF\xBF\xBF\xBF""c"}, {3, 1, 1, 6, "\xFB\xBF\xBF\xBF\xBF""c" }, {4, 1, 1, 6, "\xFB\xBF\xBF\xBF\xBF""c" },
				{0, 2, 2, 0, ""}, {1, 2, 2, 1, "c"                   }, {2, 2, 2, 1, "c"                      }, {3, 2, 2, 1, "c"                       }, {4, 2, 2, 1, "c"                       },
				{0, 3, 3, 0, ""}, {1, 3, 3, 0, ""                    }, {2, 3, 3, 0, ""                       }, {3, 3, 3, 0, ""                        }, {4, 3, 3, 0, ""                        },
				{0, 4, 3, 0, ""}, {1, 4, 3, 0, ""                    }, {2, 4, 3, 0, ""                       }, {3, 4, 3, 0, ""                        }, {4, 4, 3, 0, ""                        }
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
	{
		const acpl::Unichar oUcs[] = { 'a', 'b', 0x3FFFFFF, '\0' };  // U+3FFFFFF == \xFB\xBF\xBF\xBF\xBF
		acpl::String::Ref oRef(oUcs);
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 1, "a"                   }, {2, 0, 0, 2, "ab"                   }, {3, 0, 0, 7, "ab\xFB\xBF\xBF\xBF\xBF"}, {4, 0, 0, 7, "ab\xFB\xBF\xBF\xBF\xBF"},
				{0, 1, 1, 0, ""}, {1, 1, 1, 1, "b"                   }, {2, 1, 1, 6, "b\xFB\xBF\xBF\xBF\xBF"}, {3, 1, 1, 6, "b\xFB\xBF\xBF\xBF\xBF" }, {4, 1, 1, 6, "b\xFB\xBF\xBF\xBF\xBF" },
				{0, 2, 2, 0, ""}, {1, 2, 2, 5, "\xFB\xBF\xBF\xBF\xBF"}, {2, 2, 2, 5, "\xFB\xBF\xBF\xBF\xBF" }, {3, 2, 2, 5, "\xFB\xBF\xBF\xBF\xBF"  }, {4, 2, 2, 5, "\xFB\xBF\xBF\xBF\xBF"  },
				{0, 3, 3, 0, ""}, {1, 3, 3, 0, ""                    }, {2, 3, 3, 0, ""                     }, {3, 3, 3, 0, ""                      }, {4, 3, 3, 0, ""                      },
				{0, 4, 3, 0, ""}, {1, 4, 3, 0, ""                    }, {2, 4, 3, 0, ""                     }, {3, 4, 3, 0, ""                      }, {4, 4, 3, 0, ""                      }
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
	
	return 0;
}

static int TestRefMeasureConvertUcs_EncAs6ByteSeq()
{
	PrintFn();
	
	
	{
		const acpl::Unichar oUcs[] = { 0x7FFFFFFF, 'b', 'c', '\0' };  // U+7FFFFFFF == \xFD\xBF\xBF\xBF\xBF\xBF
		acpl::String::Ref oRef(oUcs);
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 6, "\xFD\xBF\xBF\xBF\xBF\xBF"}, {2, 0, 0, 7, "\xFD\xBF\xBF\xBF\xBF\xBF""b"}, {3, 0, 0, 8, "\xFD\xBF\xBF\xBF\xBF\xBF""bc"}, {4, 0, 0, 8, "\xFD\xBF\xBF\xBF\xBF\xBF""bc"},
				{0, 1, 1, 0, ""}, {1, 1, 1, 1, "b"                       }, {2, 1, 1, 2, "bc"                         }, {3, 1, 1, 2, "bc"                          }, {4, 1, 1, 2, "bc"                          },
				{0, 2, 2, 0, ""}, {1, 2, 2, 1, "c"                       }, {2, 2, 2, 1, "c"                          }, {3, 2, 2, 1, "c"                           }, {4, 2, 2, 1, "c"                           },
				{0, 3, 3, 0, ""}, {1, 3, 3, 0, ""                        }, {2, 3, 3, 0, ""                           }, {3, 3, 3, 0, ""                            }, {4, 3, 3, 0, ""                            },
				{0, 4, 3, 0, ""}, {1, 4, 3, 0, ""                        }, {2, 4, 3, 0, ""                           }, {3, 4, 3, 0, ""                            }, {4, 4, 3, 0, ""                            }
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
	{
		const acpl::Unichar oUcs[] = { 'a', 0x7FFFFFFF, 'c', '\0' };  // U+7FFFFFFF == \xFD\xBF\xBF\xBF\xBF\xBF
		acpl::String::Ref oRef(oUcs);
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 1, "a"                       }, {2, 0, 0, 7, "a\xFD\xBF\xBF\xBF\xBF\xBF"  }, {3, 0, 0, 8, "a\xFD\xBF\xBF\xBF\xBF\xBF""c"}, {4, 0, 0, 8, "a\xFD\xBF\xBF\xBF\xBF\xBF""c"},
				{0, 1, 1, 0, ""}, {1, 1, 1, 6, "\xFD\xBF\xBF\xBF\xBF\xBF"}, {2, 1, 1, 7, "\xFD\xBF\xBF\xBF\xBF\xBF""c"}, {3, 1, 1, 7, "\xFD\xBF\xBF\xBF\xBF\xBF""c" }, {4, 1, 1, 7, "\xFD\xBF\xBF\xBF\xBF\xBF""c" },
				{0, 2, 2, 0, ""}, {1, 2, 2, 1, "c"                       }, {2, 2, 2, 1, "c"                          }, {3, 2, 2, 1, "c"                           }, {4, 2, 2, 1, "c"                           },
				{0, 3, 3, 0, ""}, {1, 3, 3, 0, ""                        }, {2, 3, 3, 0, ""                           }, {3, 3, 3, 0, ""                            }, {4, 3, 3, 0, ""                            },
				{0, 4, 3, 0, ""}, {1, 4, 3, 0, ""                        }, {2, 4, 3, 0, ""                           }, {3, 4, 3, 0, ""                            }, {4, 4, 3, 0, ""                            }
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
	{
		const acpl::Unichar oUcs[] = { 'a', 'b', 0x7FFFFFFF, '\0' };  // U+7FFFFFFF == \xFD\xBF\xBF\xBF\xBF\xBF
		acpl::String::Ref oRef(oUcs);
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 1, "a"                       }, {2, 0, 0, 2, "ab"                       }, {3, 0, 0, 8, "ab\xFD\xBF\xBF\xBF\xBF\xBF"}, {4, 0, 0, 8, "ab\xFD\xBF\xBF\xBF\xBF\xBF"},
				{0, 1, 1, 0, ""}, {1, 1, 1, 1, "b"                       }, {2, 1, 1, 7, "b\xFD\xBF\xBF\xBF\xBF\xBF"}, {3, 1, 1, 7, "b\xFD\xBF\xBF\xBF\xBF\xBF" }, {4, 1, 1, 7, "b\xFD\xBF\xBF\xBF\xBF\xBF" },
				{0, 2, 2, 0, ""}, {1, 2, 2, 6, "\xFD\xBF\xBF\xBF\xBF\xBF"}, {2, 2, 2, 6, "\xFD\xBF\xBF\xBF\xBF\xBF" }, {3, 2, 2, 6, "\xFD\xBF\xBF\xBF\xBF\xBF"  }, {4, 2, 2, 6, "\xFD\xBF\xBF\xBF\xBF\xBF"  },
				{0, 3, 3, 0, ""}, {1, 3, 3, 0, ""                        }, {2, 3, 3, 0, ""                         }, {3, 3, 3, 0, ""                          }, {4, 3, 3, 0, ""                          },
				{0, 4, 3, 0, ""}, {1, 4, 3, 0, ""                        }, {2, 4, 3, 0, ""                         }, {3, 4, 3, 0, ""                          }, {4, 4, 3, 0, ""                          }
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
	// Over the limit of UTF-8
	
	{
		const acpl::Unichar oUcs[] = { 0x80000000, 'b', 'c', '\0' };  // U+80000000 (too high for UTF-8) == '?'
		acpl::String::Ref oRef(oUcs);
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 1, "?"}, {2, 0, 0, 2, "?b"}, {3, 0, 0, 3, "?bc"}, {4, 0, 0, 3, "?bc"},
				{0, 1, 1, 0, ""}, {1, 1, 1, 1, "b"}, {2, 1, 1, 2, "bc"}, {3, 1, 1, 2, "bc" }, {4, 1, 1, 2, "bc" },
				{0, 2, 2, 0, ""}, {1, 2, 2, 1, "c"}, {2, 2, 2, 1, "c" }, {3, 2, 2, 1, "c"  }, {4, 2, 2, 1, "c"  },
				{0, 3, 3, 0, ""}, {1, 3, 3, 0, "" }, {2, 3, 3, 0, ""  }, {3, 3, 3, 0, ""   }, {4, 3, 3, 0, ""   },
				{0, 4, 3, 0, ""}, {1, 4, 3, 0, "" }, {2, 4, 3, 0, ""  }, {3, 4, 3, 0, ""   }, {4, 4, 3, 0, ""   }
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
	{
		const acpl::Unichar oUcs[] = { 'a', 0x80000000, 'c', '\0' };  // U+80000000 (too high for UTF-8) == '?'
		acpl::String::Ref oRef(oUcs);
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 1, "a"}, {2, 0, 0, 2, "a?"}, {3, 0, 0, 3, "a?c"}, {4, 0, 0, 3, "a?c"},
				{0, 1, 1, 0, ""}, {1, 1, 1, 1, "?"}, {2, 1, 1, 2, "?c"}, {3, 1, 1, 2, "?c" }, {4, 1, 1, 2, "?c" },
				{0, 2, 2, 0, ""}, {1, 2, 2, 1, "c"}, {2, 2, 2, 1, "c" }, {3, 2, 2, 1, "c"  }, {4, 2, 2, 1, "c"  },
				{0, 3, 3, 0, ""}, {1, 3, 3, 0, "" }, {2, 3, 3, 0, ""  }, {3, 3, 3, 0, ""   }, {4, 3, 3, 0, ""   },
				{0, 4, 3, 0, ""}, {1, 4, 3, 0, "" }, {2, 4, 3, 0, ""  }, {3, 4, 3, 0, ""   }, {4, 4, 3, 0, ""   }
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
	{
		const acpl::Unichar oUcs[] = { 'a', 'b', 0x80000000, '\0' };  // U+80000000 (too high for UTF-8) == '?'
		acpl::String::Ref oRef(oUcs);
		
		_local_TestRefMeasureConvert_ExpResult oExpRes[] = {
				{0, 0, 0, 0, ""}, {1, 0, 0, 1, "a"}, {2, 0, 0, 2, "ab"}, {3, 0, 0, 3, "ab?"}, {4, 0, 0, 3, "ab?"},
				{0, 1, 1, 0, ""}, {1, 1, 1, 1, "b"}, {2, 1, 1, 2, "b?"}, {3, 1, 1, 2, "b?" }, {4, 1, 1, 2, "b?" },
				{0, 2, 2, 0, ""}, {1, 2, 2, 1, "?"}, {2, 2, 2, 1, "?" }, {3, 2, 2, 1, "?"  }, {4, 2, 2, 1, "?"  },
				{0, 3, 3, 0, ""}, {1, 3, 3, 0, "" }, {2, 3, 3, 0, ""  }, {3, 3, 3, 0, ""   }, {4, 3, 3, 0, ""   },
				{0, 4, 3, 0, ""}, {1, 4, 3, 0, "" }, {2, 4, 3, 0, ""  }, {3, 4, 3, 0, ""   }, {4, 4, 3, 0, ""   }
			};
		
		Test(_local_TestRefMeasureConvert(oRef, oExpRes, (sizeof(oExpRes) / sizeof(_local_TestRefMeasureConvert_ExpResult))) == 0);
	}
	
	
	return 0;
}

static int TestRefMeasureConvertUcs()
{
	PrintFn();
	
	
	acpl::SizeT oRefOff;
	acpl::SizeT oEncByteSize;
	
	
	// Empty
	
	{
		acpl::String::Ref oRef(reinterpret_cast<const acpl::Unichar *>(NULL));
		Test(oRef.GetType() == acpl::String::Ref::rtUcs);
		oRef.Measure(acpl::Num<acpl::SizeT>::Max(), 0, oRefOff, oEncByteSize);
		Test(oRefOff == 0);
		Test(oEncByteSize == 0);
	}
	
	
	Test(TestRefMeasureConvertUcs_EncAs1ByteSeq() == 0);
	Test(TestRefMeasureConvertUcs_EncAs2ByteSeq() == 0);
	Test(TestRefMeasureConvertUcs_EncAs3ByteSeq() == 0);
	Test(TestRefMeasureConvertUcs_EncAs4ByteSeq() == 0);
	Test(TestRefMeasureConvertUcs_EncAs5ByteSeq() == 0);
	Test(TestRefMeasureConvertUcs_EncAs6ByteSeq() == 0);
	
	
	return 0;
}

static int TestPosAttach()
{
	PrintFn();
	
	
	const acpl::Unichar oUcs[] = { 'a', 0xC4B2, 0x024A, 'b', 0x2166, '\0' };
	acpl::String oStr(oUcs);
	acpl::Unichar oUc;
	
	
	// Is everything ah-oh-kay?
	
	Test(oStr.ByteLength() == 10);
	Test(oStr.Length() == 5);
	
	
	// Init
	
	acpl::String::Pos oPos; // Do not initialize!
	Test(oPos.GetString() == NULL);
	Test(oPos.IsNull() == true);
	Test(oPos.GetPtr() == NULL);
	Test(oPos.Utf8()[0] == '\0');
	Test(oPos.GetChar() == '\0');
	Test(oPos.GetUnichar() == '\0');
	Test(oPos.GetUnichar(oUc) == false);
	Test(oPos.IsFirst() == false);
	Test(oPos.IsLast() == false);
	
	Test(oPos.First() == false);
	Test(oPos.Last() == false);
	Test(oPos.Next() == false);
	Test(oPos.Prev() == false);
	
	
	// Attachment
	
	oPos.Attach(oStr);
	Test(oPos.GetString() == &oStr);
	Test(oPos.IsNull() == false);
	Test(oPos.GetPtr() == oStr.Utf8());
	Test(oPos.Utf8() == oStr.Utf8());
	Test(oPos.GetChar() == 'a');
	Test(oPos.GetUnichar() == 'a');
	Test(oPos.GetUnichar(oUc) == true);
	Test(oUc == 'a');
	Test(oPos.IsFirst() == true);
	Test(oPos.IsLast() == false);
	
	
	// Attachment on other `Pos`
	
	acpl::String::Pos oAddPos(oPos);
	Test(oPos.IsSameString(oAddPos) == true);
	Test(oAddPos.IsSameString(oPos) == true);
	Test(oAddPos.GetString() == &oStr);
	Test(oAddPos.IsNull() == false);
	Test(oAddPos.GetPtr() == oStr.Utf8());
	Test(oAddPos.Utf8() == oStr.Utf8());
	Test(oAddPos.GetChar() == 'a');
	Test(oAddPos.GetUnichar() == 'a');
	Test(oAddPos.GetUnichar(oUc) == true);
	Test(oUc == 'a');
	Test(oAddPos.IsFirst() == true);
	Test(oAddPos.IsLast() == false);
	
	
	// Detachment (NULL attachment)
	
	oAddPos.Attach(NULL);
	Test(oAddPos.GetString() == NULL);
	Test(oAddPos.IsNull() == true);
	Test(oAddPos.GetPtr() == NULL);
	Test(oAddPos.Utf8()[0] == '\0');
	Test(oAddPos.GetChar() == '\0');
	Test(oAddPos.GetUnichar() == '\0');
	Test(oAddPos.GetUnichar(oUc) == false);
	Test(oAddPos.IsFirst() == false);
	Test(oAddPos.IsLast() == false);
	
	
	// Deinit
	
	Test(oAddPos.First() == false);
	Test(oAddPos.Last() == false);
	Test(oAddPos.Next() == false);
	Test(oAddPos.Prev() == false);
	
	
	return 0;
}

static int TestPosMove()
{
	PrintFn();
	
	
	const acpl::Unichar oUcs[] = { 'a', 0xC4B2, 0x024A, 'b', 0x2166, '\0' };
	acpl::String oStr(oUcs);
	acpl::String::Pos oPos(oStr);
	acpl::Unichar oUc;
	
	
	// Last
	
	Test(oPos.Last() == true);
	Test(oPos.IsFirst() == false);
	Test(oPos.IsLast() == true);
	Test(oPos.GetUnichar(oUc) == true);
	Test(oUc == 0x2166);
	Test(oPos.Next() == false);
	
	
	// Prev
	
	Test(oPos.Prev() == true);
	Test(oPos.IsFirst() == false);
	Test(oPos.IsLast() == false);
	Test(oPos.GetUnichar(oUc) == true);
	Test(oUc == 'b');
	Test(oPos.Prev() == true);
	Test(oPos.IsFirst() == false);
	Test(oPos.IsLast() == false);
	Test(oPos.GetUnichar(oUc) == true);
	Test(oUc == 0x024A);
	
	
	// First
	
	Test(oPos.First() == true);
	Test(oPos.IsFirst() == true);
	Test(oPos.IsLast() == false);
	Test(oPos.GetUnichar(oUc) == true);
	Test(oUc == 'a');
	Test(oPos.Prev() == false);
	
	
	// Next
	
	Test(oPos.Next() == true);
	Test(oPos.IsFirst() == false);
	Test(oPos.IsLast() == false);
	Test(oPos.GetUnichar(oUc) == true);
	Test(oUc == 0xC4B2);
	Test(oPos.Next() == true);
	Test(oPos.IsFirst() == false);
	Test(oPos.IsLast() == false);
	Test(oPos.GetUnichar(oUc) == true);
	Test(oUc == 0x024A);
	
	
	// Forth
	
	Test(oPos.Forth(3) == true);
	Test(oPos.IsFirst() == false);
	Test(oPos.IsLast() == true);
	Test(oPos.GetUnichar(oUc) == true);
	Test(oUc == 0x2166);
	Test(oPos.First() == true);
	Test(oPos.Forth(2) == true);
	Test(oPos.IsFirst() == false);
	Test(oPos.IsLast() == false);
	Test(oPos.GetUnichar(oUc) == true);
	Test(oUc == 0x024A);
	
	
	// Back
	
	Test(oPos.Back(3) == true);
	Test(oPos.IsFirst() == true);
	Test(oPos.IsLast() == false);
	Test(oPos.GetUnichar(oUc) == true);
	Test(oUc == 'a');
	Test(oPos.Last() == true);
	Test(oPos.Back(2) == true);
	Test(oPos.IsFirst() == false);
	Test(oPos.IsLast() == false);
	Test(oPos.GetUnichar(oUc) == true);
	Test(oUc == 0x024A);
	
	
	// SkipFirst
	
	Test(oPos.SkipFirst(0) == true);
	Test(oPos.IsFirst() == true);
	Test(oPos.IsLast() == false);
	Test(oPos.GetUnichar(oUc) == true);
	Test(oUc == 'a');
	Test(oPos.SkipFirst(3) == true);
	Test(oPos.IsFirst() == false);
	Test(oPos.IsLast() == false);
	Test(oPos.GetUnichar(oUc) == true);
	Test(oUc == 'b');
	Test(oPos.SkipFirst(6) == true);
	Test(oPos.IsFirst() == false);
	Test(oPos.IsLast() == true);
	Test(oPos.GetUnichar(oUc) == true);
	Test(oUc == 0x2166);
	
	
	// SkipLast
	
	Test(oPos.SkipLast(0) == true);
	Test(oPos.IsFirst() == false);
	Test(oPos.IsLast() == true);
	Test(oPos.GetUnichar(oUc) == true);
	Test(oUc == 0x2166);
	Test(oPos.SkipLast(3) == true);
	Test(oPos.IsFirst() == false);
	Test(oPos.IsLast() == false);
	Test(oPos.GetUnichar(oUc) == true);
	Test(oUc == 0xC4B2);
	Test(oPos.SkipLast(6) == true);
	Test(oPos.IsFirst() == true);
	Test(oPos.IsLast() == false);
	Test(oPos.GetUnichar(oUc) == true);
	Test(oUc == 'a');
	
	
	return 0;
}

static int TestPosMoveUnichar()
{
	PrintFn();
	
	
	const acpl::Unichar oUcs[] = { 'a', 0xC4B2, 0xC4B2, 'b', 0x2166, '\0' };
	acpl::String oStr(oUcs);
	acpl::String::Pos oPos(oStr);
	acpl::Unichar oUc;
	
	
	// Last
	
	Test(oPos.Last(0x2166) == true);
	Test(oPos.IsFirst() == false);
	Test(oPos.IsLast() == true);
	Test(oPos.GetUnichar(oUc) == true);
	Test(oUc == 0x2166);
	Test(oPos.Next(0x2166) == false);
	Test(oPos.Last(0xC4B2) == true);
	Test(oPos.IsFirst() == false);
	Test(oPos.IsLast() == false);
	Test(oPos.GetUnichar(oUc) == true);
	Test(oUc == 0xC4B2);
	
	
	// Prev
	
	Test(oPos.Prev(0xC4B2) == true);
	Test(oPos.IsFirst() == false);
	Test(oPos.IsLast() == false);
	Test(oPos.GetUnichar(oUc) == true);
	Test(oUc == 0xC4B2);
	Test(oPos.Prev(0xC4B2) == false);
	
	
	// First
	
	Test(oPos.First('a') == true);
	Test(oPos.IsFirst() == true);
	Test(oPos.IsLast() == false);
	Test(oPos.GetUnichar(oUc) == true);
	Test(oUc == 'a');
	Test(oPos.Prev('a') == false);
	Test(oPos.First(0xC4B2) == true);
	Test(oPos.IsFirst() == false);
	Test(oPos.IsLast() == false);
	Test(oPos.GetUnichar(oUc) == true);
	Test(oUc == 0xC4B2);
	
	
	// Next
	
	Test(oPos.Next(0xC4B2) == true);
	Test(oPos.IsFirst() == false);
	Test(oPos.IsLast() == false);
	Test(oPos.GetUnichar(oUc) == true);
	Test(oUc == 0xC4B2);
	Test(oPos.Next(0xC4B2) == false);
	
	
	return 0;
}

static int TestPosMoveSubstr()
{
	PrintFn();
	
	
	const acpl::Unichar oUcs[] = { 'a', 0xC4B2, 0xC4B2, 'b', 0x2166, '\0' };
	acpl::String oStr(oUcs);
	acpl::String::Pos oPos(oStr);
	acpl::Unichar oUc;
	acpl::String oSubStr;
	
	
	// Last
	
	oSubStr.Set(oUcs, 2, 3);
	Test(oPos.Last(oSubStr) == true);
	Test(oPos.IsFirst() == false);
	Test(oPos.IsLast() == false);
	Test(oPos.GetUnichar(oUc) == true);
	Test(oUc == 'b');
	oSubStr.Set(oUcs, 2, 1);
	Test(oPos.Last(oSubStr) == true);
	Test(oPos.IsFirst() == false);
	Test(oPos.IsLast() == false);
	Test(oPos.GetUnichar(oUc) == true);
	Test(oUc == 0xC4B2);
	
	
	// Prev
	
	oSubStr.Set(oUcs, 2, 0);
	Test(oPos.Prev(oSubStr) == true);
	Test(oPos.IsFirst() == true);
	Test(oPos.IsLast() == false);
	Test(oPos.GetUnichar(oUc) == true);
	Test(oUc == 'a');
	Test(oPos.Prev(oSubStr) == false);
	
	
	// First
	
	oSubStr.Set(oUcs, 2, 0);
	Test(oPos.First(oSubStr) == true);
	Test(oPos.IsFirst() == true);
	Test(oPos.IsLast() == false);
	Test(oPos.GetUnichar(oUc) == true);
	Test(oUc == 'a');
	Test(oPos.Prev(oSubStr) == false);
	oSubStr.Set(oUcs, 2, 1);
	Test(oPos.First(oSubStr) == true);
	Test(oPos.IsFirst() == false);
	Test(oPos.IsLast() == false);
	Test(oPos.GetUnichar(oUc) == true);
	Test(oUc == 0xC4B2);
	
	
	// Next
	oSubStr.Set(oUcs, 2, 2);
	Test(oPos.Next(oSubStr) == true);
	Test(oPos.IsFirst() == false);
	Test(oPos.IsLast() == false);
	Test(oPos.GetUnichar(oUc) == true);
	Test(oUc == 0xC4B2);
	Test(oPos.Next(oSubStr) == false);
	
	
	return 0;
}

static int TestRefList()
{
	PrintFn();
	
	
	acpl::String::RefList oRl;
	acpl::String oStr;
	
	
	// String
	
	oRl.String(static_cast<char *>(NULL));
	Test(oRl.List().GetLast()->IsNull() == false);
	Test(_local_Memcmp(oRl.List().GetLast()->GetPtr(), "(null)", 7) == true);
	
	oRl.String(oStr);
	Test(oRl.List().GetLast()->IsNull() == false);
	Test(_local_Memcmp(oRl.List().GetLast()->GetPtr(), "(null)", 7) == true);
	
	oRl.String("abc");
	Test(oRl.List().GetLast()->IsNull() == false);
	Test(_local_Memcmp(oRl.List().GetLast()->GetPtr(), "abc", 4) == true);
	
	oStr.Set("cba");
	oRl.String(oStr);
	oStr.Clear();
	Test(oRl.List().GetLast()->IsNull() == false);
	Test(_local_Memcmp(oRl.List().GetLast()->GetPtr(), "cba", 4) == true);
	
	
	// Int
	
	oRl.Int(12345);
	Test(oRl.List().GetLast()->IsNull() == false);
	Test(_local_Memcmp(oRl.List().GetLast()->GetPtr(), "12345", 6) == true);
	
	oRl.Int(acpl::Const::UI64(0x01234567, 0x89ABCDEF), 16, true, 16, '0');
	Test(oRl.List().GetLast()->IsNull() == false);
	Test(_local_Memcmp(oRl.List().GetLast()->GetPtr(), "0123456789ABCDEF", 17) == true);
	
	
	return 0;
}

static int TestFmtRef()
{
	PrintFn();
	
	
	acpl::String::FmtRef oFmt;
	acpl::String::RefList oRl;
	
	
	// Init tests
	Test(oFmt.IsNull() == true);
	Test(oFmt.GetPtr() == NULL);
	Test(oFmt.Utf8() != NULL);
	Test(oFmt.Utf8()[0] == '\0');
	Test(oFmt.FmtUtf8() != NULL);
	Test(oFmt.FmtUtf8()[0] == '\0');
	
	// `SetFmt()`
	oFmt.SetFmt("abc");
	Test(oFmt.IsNull() == true);
	Test(oFmt.GetPtr() == NULL);
	Test(oFmt.Utf8() != NULL);
	Test(oFmt.Utf8()[0] == '\0');
	Test(oFmt.FmtUtf8() != NULL);
	Test(_local_Memcmp(oFmt.FmtUtf8(), "abc", 4) == true);
	
	// `Format()` with no args in format and empty `oRl`
	oFmt.SetFmt("abc");
	oFmt.Format(oRl);
	Test(oFmt.IsNull() == false);
	Test(oFmt.GetPtr() == oFmt.Utf8());
	Test(oFmt.GetPtr() != NULL);
	Test(oFmt.Utf8() != NULL);
	Test(oFmt.FmtUtf8() != NULL);
	Test(_local_Memcmp(oFmt.FmtUtf8(), "abc", 4) == true);
	Test(_local_Memcmp(oFmt.Utf8(), "abc", 4) == true);
	
	// `Format()` with args in format but empty `oRl`
	oFmt.SetFmt("$2 abc ${12} cba");
	oFmt.Format(oRl);
	Test(oFmt.IsNull() == false);
	Test(oFmt.GetPtr() == oFmt.Utf8());
	Test(oFmt.GetPtr() != NULL);
	Test(oFmt.Utf8() != NULL);
	Test(oFmt.FmtUtf8() != NULL);
	Test(_local_Memcmp(oFmt.FmtUtf8(), "$2 abc ${12} cba", 17) == true);
	Test(_local_Memcmp(oFmt.Utf8(), "(no-arg) abc (no-arg) cba", 26) == true);
	
	oRl.String("xyz").Int(1234).String("zyx").Int(4321);
	
	// `Format()` with no args in format but full `oRl`
	oFmt.SetFmt("abc");
	oFmt.Format(oRl);
	Test(oFmt.IsNull() == false);
	Test(oFmt.GetPtr() == oFmt.Utf8());
	Test(oFmt.GetPtr() != NULL);
	Test(oFmt.Utf8() != NULL);
	Test(oFmt.FmtUtf8() != NULL);
	Test(_local_Memcmp(oFmt.FmtUtf8(), "abc", 4) == true);
	Test(_local_Memcmp(oFmt.Utf8(), "abc", 4) == true);
	
	// `Format()` with args in format and full `oRl`
	oFmt.SetFmt("abc $1 cba ${2}");
	oFmt.Format(oRl);
	Test(oFmt.IsNull() == false);
	Test(oFmt.GetPtr() == oFmt.Utf8());
	Test(oFmt.GetPtr() != NULL);
	Test(oFmt.Utf8() != NULL);
	Test(oFmt.FmtUtf8() != NULL);
	Test(_local_Memcmp(oFmt.FmtUtf8(), "abc $1 cba ${2}", 16) == true);
	Test(_local_Memcmp(oFmt.Utf8(), "abc 1234 cba zyx", 17) == true);
	
	// Quirks/escapes in fmt #1
	oFmt.SetFmt("\\${2} abc $ 1 cba ${");
	oFmt.Format(oRl);
	Test(oFmt.IsNull() == false);
	Test(oFmt.GetPtr() == oFmt.Utf8());
	Test(oFmt.GetPtr() != NULL);
	Test(oFmt.Utf8() != NULL);
	Test(oFmt.FmtUtf8() != NULL);
	Test(_local_Memcmp(oFmt.FmtUtf8(), "\\${2} abc $ 1 cba ${", 21) == true);
	Test(_local_Memcmp(oFmt.Utf8(), "${2} abc $ 1 cba ${", 20) == true);
	
	// Quirks/escapes in fmt #2
	oFmt.SetFmt("${ abc ${1a} \\\\$1\\$ cba ${a1} $");
	oFmt.Format(oRl);
	Test(oFmt.IsNull() == false);
	Test(oFmt.GetPtr() == oFmt.Utf8());
	Test(oFmt.GetPtr() != NULL);
	Test(oFmt.Utf8() != NULL);
	Test(oFmt.FmtUtf8() != NULL);
	Test(_local_Memcmp(oFmt.FmtUtf8(), "${ abc ${1a} \\\\$1\\$ cba ${a1} $", 32) == true);
	Test(_local_Memcmp(oFmt.Utf8(), "${ abc ${1a} \\1234$ cba ${a1} $", 32) == true);
	
	
	return 0;
}

static int TestFmtDtRef()
{
	PrintFn();
	
	
	acpl::String::FmtDtRef oFmtDt;
	acpl::DateTime oDt;
	oDt.SetDate(2012, 11, 28);
	oDt.SetTime(20, 1, 32);
	oDt.Normalize();
	
	
	// Init tests
	Test(oFmtDt.IsNull() == true);
	Test(oFmtDt.GetPtr() == NULL);
	Test(oFmtDt.Utf8() != NULL);
	Test(oFmtDt.Utf8()[0] == '\0');
	Test(oFmtDt.FmtUtf8() != NULL);
	Test(oFmtDt.FmtUtf8()[0] == '\0');
	
	// `SetFmt()`
	oFmtDt.SetFmt("abc");
	Test(oFmtDt.IsNull() == true);
	Test(oFmtDt.GetPtr() == NULL);
	Test(oFmtDt.Utf8() != NULL);
	Test(oFmtDt.Utf8()[0] == '\0');
	Test(oFmtDt.FmtUtf8() != NULL);
	Test(_local_Memcmp(oFmtDt.FmtUtf8(), "abc", 4) == true);
	
	// `Format()` with no args in format
	oFmtDt.SetFmt("abc");
	oFmtDt.Format(oDt);
	Test(oFmtDt.IsNull() == false);
	Test(oFmtDt.GetPtr() == oFmtDt.Utf8());
	Test(oFmtDt.GetPtr() != NULL);
	Test(oFmtDt.Utf8() != NULL);
	Test(oFmtDt.FmtUtf8() != NULL);
	Test(_local_Memcmp(oFmtDt.FmtUtf8(), "abc", 4) == true);
	Test(_local_Memcmp(oFmtDt.Utf8(), "abc", 4) == true);
	
	// `Format()` with args in format (all posibilities)
	oFmtDt.SetFmt("%C %d %F %H %m %M %S %T %y %Y");
	oFmtDt.Format(oDt);
	Test(oFmtDt.IsNull() == false);
	Test(oFmtDt.GetPtr() == oFmtDt.Utf8());
	Test(oFmtDt.GetPtr() != NULL);
	Test(oFmtDt.Utf8() != NULL);
	Test(oFmtDt.FmtUtf8() != NULL);
	Test(_local_Memcmp(oFmtDt.FmtUtf8(), "%C %d %F %H %m %M %S %T %y %Y", 30) == true);
	Test(_local_Memcmp(oFmtDt.Utf8(), "20 28 2012-11-28 20 11 01 32 20:01:32 12 2012", 46) == true);
	
	// Quirks/escapes in fmt #1
	oFmtDt.SetFmt("% %- %%T %% %");
	oFmtDt.Format(oDt);
	Test(oFmtDt.IsNull() == false);
	Test(oFmtDt.GetPtr() == oFmtDt.Utf8());
	Test(oFmtDt.GetPtr() != NULL);
	Test(oFmtDt.Utf8() != NULL);
	Test(oFmtDt.FmtUtf8() != NULL);
	Test(_local_Memcmp(oFmtDt.FmtUtf8(), "% %- %%T %% %", 14) == true);
	Test(_local_Memcmp(oFmtDt.Utf8(), "% %- %T % %", 12) == true);
	
	
	return 0;
}

static int TestInit()
{
	PrintFn();
	
	
	acpl::String oStr;
	
	
	Test(oStr.IsNull() == true);
	Test(oStr.IsEmpty() == true);
	Test(oStr.IsAllBlanks() == true);
	Test(oStr.IsAllWspaces() == true);
	Test(oStr.Length() == 0);
	Test(oStr.ByteLength() == 0);
	
	Test(_local_Memcmp(oStr.Utf8(), "", 1) == true);
	
	
	return 0;
}

static int TestIsAllSpaces()
{
	PrintFn();
	
	
	acpl::String oStr;
	
	
	// IsAllBlanks
	
	oStr.Set("   \t  \t");
	Test(oStr.IsAllBlanks() == true);
	oStr.Apnd("a");
	Test(oStr.IsAllBlanks() == false);
	
	
	// IsAllWspaces
	
	oStr.Set("   \t  \t");
	Test(oStr.IsAllWspaces() == true);
	oStr.Apnd("a");
	Test(oStr.IsAllWspaces() == false);
	
	oStr.Set("\x09\x0A\x0B\x0C\x0D\x20\xC2\x85\xC2\xA0\xE1\x9A\x80\xE1\xA0\x8E\xE2\x80\x80\xE2\x80\x81\xE2\x80\x82\xE2\x80\x83\xE2\x80\x84\xE2\x80\x85\xE2\x80\x86\xE2\x80\x87\xE2\x80\x88\xE2\x80\x89\xE2\x80\x8A\xE2\x80\xA8\xE2\x80\xA9\xE2\x80\xAF\xE2\x81\x9F\xE3\x80\x80");
	Test(oStr.IsAllWspaces() == true);
	oStr.Apnd("a");
	Test(oStr.IsAllWspaces() == false);
	
	
	return 0;
}

static int TestSetFromRef()
{
	PrintFn();
	
	
	// 1-byte seqence
	
	acpl::String oStr("abc");
	Test(oStr.ByteLength() == 3);
	Test(oStr.Length() == 3);
	Test(StringCompare(oStr.Utf8(), "abc") == 0);
	
	oStr.Clear();
	Test(oStr.ByteLength() == 0);
	Test(oStr.Length() == 0);
	
	oStr.Set(L"abc");
	Test(oStr.ByteLength() == 3);
	Test(oStr.Length() == 3);
	Test(StringCompare(oStr.Utf8(), "abc") == 0);
	
	const acpl::Unichar oUcs[] = { 'a', 'b', 'c', '\0' };
	oStr.Set(oUcs);
	Test(oStr.ByteLength() == 3);
	Test(oStr.Length() == 3);
	Test(StringCompare(oStr.Utf8(), "abc") == 0);
	
	acpl::String oStr2(oStr);
	Test(oStr2.Length() == 3);
	Test(StringCompare(oStr2.Utf8(), "abc") == 0);
	
	oStr.Clear().Set(oStr2);
	Test(oStr.ByteLength() == 3);
	Test(oStr.Length() == 3);
	Test(StringCompare(oStr.Utf8(), "abc") == 0);
	
	oStr.Clear();
	Test(oStr.ByteLength() == 0);
	Test(oStr.Length() == 0);
	Test(StringCompare(oStr.Utf8(), "") == 0);
	
	
	// 2-byte seqence
	
	oStr.Set("\xDF\xBF""bc");
	Test(oStr.ByteLength() == 4);
	Test(oStr.Length() == 3);
	Test(StringCompare(oStr.Utf8(), "\xDF\xBF""bc") == 0);
	
	oStr.Set("a\xDF\xBF""c");
	Test(oStr.ByteLength() == 4);
	Test(oStr.Length() == 3);
	Test(StringCompare(oStr.Utf8(), "a\xDF\xBF""c") == 0);
	
	oStr.Set("ab\xDF\xBF");
	Test(oStr.ByteLength() == 4);
	Test(oStr.Length() == 3);
	Test(StringCompare(oStr.Utf8(), "ab\xDF\xBF") == 0);
	
	
	// 3-byte seqence
	
	oStr.Set("\xEF\xBF\xBF""bc");
	Test(oStr.ByteLength() == 5);
	Test(oStr.Length() == 3);
	Test(StringCompare(oStr.Utf8(), "\xEF\xBF\xBF""bc") == 0);
	
	oStr.Set("a\xEF\xBF\xBF""c");
	Test(oStr.ByteLength() == 5);
	Test(oStr.Length() == 3);
	Test(StringCompare(oStr.Utf8(), "a\xEF\xBF\xBF""c") == 0);
	
	oStr.Set("ab\xEF\xBF\xBF");
	Test(oStr.ByteLength() == 5);
	Test(oStr.Length() == 3);
	Test(StringCompare(oStr.Utf8(), "ab\xEF\xBF\xBF") == 0);
	
	
	// 4-byte seqence
	
	oStr.Set("\xF7\xBF\xBF\xBF""bc");
	Test(oStr.ByteLength() == 6);
	Test(oStr.Length() == 3);
	Test(StringCompare(oStr.Utf8(), "\xF7\xBF\xBF\xBF""bc") == 0);
	
	oStr.Set("a\xF7\xBF\xBF\xBF""c");
	Test(oStr.ByteLength() == 6);
	Test(oStr.Length() == 3);
	Test(StringCompare(oStr.Utf8(), "a\xF7\xBF\xBF\xBF""c") == 0);
	
	oStr.Set("ab\xF7\xBF\xBF\xBF");
	Test(oStr.ByteLength() == 6);
	Test(oStr.Length() == 3);
	Test(StringCompare(oStr.Utf8(), "ab\xF7\xBF\xBF\xBF") == 0);
	
	
	// 5-byte seqence
	
	oStr.Set("\xFB\xBF\xBF\xBF\xBF""bc");
	Test(oStr.ByteLength() == 7);
	Test(oStr.Length() == 3);
	Test(StringCompare(oStr.Utf8(), "\xFB\xBF\xBF\xBF\xBF""bc") == 0);
	
	oStr.Set("a\xFB\xBF\xBF\xBF\xBF""c");
	Test(oStr.ByteLength() == 7);
	Test(oStr.Length() == 3);
	Test(StringCompare(oStr.Utf8(), "a\xFB\xBF\xBF\xBF\xBF""c") == 0);
	
	oStr.Set("ab\xFB\xBF\xBF\xBF\xBF");
	Test(oStr.ByteLength() == 7);
	Test(oStr.Length() == 3);
	Test(StringCompare(oStr.Utf8(), "ab\xFB\xBF\xBF\xBF\xBF") == 0);
	
	
	// 6-byte seqence
	
	oStr.Set("\xFD\xBF\xBF\xBF\xBF\xBF""bc");
	Test(oStr.ByteLength() == 8);
	Test(oStr.Length() == 3);
	Test(StringCompare(oStr.Utf8(), "\xFD\xBF\xBF\xBF\xBF\xBF""bc") == 0);
	
	oStr.Set("a\xFD\xBF\xBF\xBF\xBF\xBF""c");
	Test(oStr.ByteLength() == 8);
	Test(oStr.Length() == 3);
	Test(StringCompare(oStr.Utf8(), "a\xFD\xBF\xBF\xBF\xBF\xBF""c") == 0);
	
	oStr.Set("ab\xFD\xBF\xBF\xBF\xBF\xBF");
	Test(oStr.ByteLength() == 8);
	Test(oStr.Length() == 3);
	Test(StringCompare(oStr.Utf8(), "ab\xFD\xBF\xBF\xBF\xBF\xBF") == 0);
	
	
	return 0;
}

static int TestSetFromPos()
{
	PrintFnM("-- WARNING: NOT IMPLEMENTED!");
	
	return 0;
}

static int TestApndFromRef()
{
	PrintFn();
	
	
	acpl::String oStr;
	
	
	// 1-byte seqence
	
	oStr.Apnd("abc");
	Test(oStr.ByteLength() == 3);
	Test(oStr.Length() == 3);
	Test(StringCompare(oStr.Utf8(), "abc") == 0);
	
	oStr.Apnd(L"abc");
	Test(oStr.ByteLength() == 6);
	Test(oStr.Length() == 6);
	Test(StringCompare(oStr.Utf8(), "abcabc") == 0);
	
	const acpl::Unichar oUcs[] = { 'a', 'b', 'c', '\0' };
	oStr.Apnd(oUcs);
	Test(oStr.ByteLength() == 9);
	Test(oStr.Length() == 9);
	Test(StringCompare(oStr.Utf8(), "abcabcabc") == 0);
	
	acpl::String oStr2("abc");
	oStr.Apnd(oStr2);
	Test(oStr.ByteLength() == 12);
	Test(oStr.Length() == 12);
	Test(StringCompare(oStr.Utf8(), "abcabcabcabc") == 0);
	
	oStr.Clear();
	Test(oStr.ByteLength() == 0);
	Test(oStr.Length() == 0);
	Test(StringCompare(oStr.Utf8(), "") == 0);
	
	
	// 2-byte seqence
	
	oStr.Apnd("\xDF\xBF""bc");
	Test(oStr.ByteLength() == 4);
	Test(oStr.Length() == 3);
	Test(StringCompare(oStr.Utf8(), "\xDF\xBF""bc") == 0);
	
	oStr.Apnd("a\xDF\xBF""c");
	Test(oStr.ByteLength() == 8);
	Test(oStr.Length() == 6);
	Test(StringCompare(oStr.Utf8(), "\xDF\xBF""bca\xDF\xBF""c") == 0);
	
	oStr.Apnd("ab\xDF\xBF");
	Test(oStr.ByteLength() == 12);
	Test(oStr.Length() == 9);
	Test(StringCompare(oStr.Utf8(), "\xDF\xBF""bca\xDF\xBF""cab\xDF\xBF") == 0);
	
	oStr.Clear();
	
	
	// 3-byte seqence
	
	oStr.Apnd("\xEF\xBF\xBF""bc");
	Test(oStr.ByteLength() == 5);
	Test(oStr.Length() == 3);
	Test(StringCompare(oStr.Utf8(), "\xEF\xBF\xBF""bc") == 0);
	
	oStr.Apnd("a\xEF\xBF\xBF""c");
	Test(oStr.ByteLength() == 10);
	Test(oStr.Length() == 6);
	Test(StringCompare(oStr.Utf8(), "\xEF\xBF\xBF""bca\xEF\xBF\xBF""c") == 0);
	
	oStr.Apnd("ab\xEF\xBF\xBF");
	Test(oStr.ByteLength() == 15);
	Test(oStr.Length() == 9);
	Test(StringCompare(oStr.Utf8(), "\xEF\xBF\xBF""bca\xEF\xBF\xBF""cab\xEF\xBF\xBF") == 0);
	
	oStr.Clear();
	
	
	// 4-byte seqence
	
	oStr.Apnd("\xF7\xBF\xBF\xBF""bc");
	Test(oStr.ByteLength() == 6);
	Test(oStr.Length() == 3);
	Test(StringCompare(oStr.Utf8(), "\xF7\xBF\xBF\xBF""bc") == 0);
	
	oStr.Apnd("a\xF7\xBF\xBF\xBF""c");
	Test(oStr.ByteLength() == 12);
	Test(oStr.Length() == 6);
	Test(StringCompare(oStr.Utf8(), "\xF7\xBF\xBF\xBF""bca\xF7\xBF\xBF\xBF""c") == 0);
	
	oStr.Apnd("ab\xF7\xBF\xBF\xBF");
	Test(oStr.ByteLength() == 18);
	Test(oStr.Length() == 9);
	Test(StringCompare(oStr.Utf8(), "\xF7\xBF\xBF\xBF""bca\xF7\xBF\xBF\xBF""cab\xF7\xBF\xBF\xBF") == 0);
	
	oStr.Clear();
	
	
	// 5-byte seqence
	
	oStr.Apnd("\xFB\xBF\xBF\xBF\xBF""bc");
	Test(oStr.ByteLength() == 7);
	Test(oStr.Length() == 3);
	Test(StringCompare(oStr.Utf8(), "\xFB\xBF\xBF\xBF\xBF""bc") == 0);
	
	oStr.Apnd("a\xFB\xBF\xBF\xBF\xBF""c");
	Test(oStr.ByteLength() == 14);
	Test(oStr.Length() == 6);
	Test(StringCompare(oStr.Utf8(), "\xFB\xBF\xBF\xBF\xBF""bca\xFB\xBF\xBF\xBF\xBF""c") == 0);
	
	oStr.Apnd("ab\xFB\xBF\xBF\xBF\xBF");
	Test(oStr.ByteLength() == 21);
	Test(oStr.Length() == 9);
	Test(StringCompare(oStr.Utf8(), "\xFB\xBF\xBF\xBF\xBF""bca\xFB\xBF\xBF\xBF\xBF""cab\xFB\xBF\xBF\xBF\xBF") == 0);
	
	oStr.Clear();
	
	
	// 6-byte seqence
	
	oStr.Apnd("\xFD\xBF\xBF\xBF\xBF\xBF""bc");
	Test(oStr.ByteLength() == 8);
	Test(oStr.Length() == 3);
	Test(StringCompare(oStr.Utf8(), "\xFD\xBF\xBF\xBF\xBF\xBF""bc") == 0);
	
	oStr.Apnd("a\xFD\xBF\xBF\xBF\xBF\xBF""c");
	Test(oStr.ByteLength() == 16);
	Test(oStr.Length() == 6);
	Test(StringCompare(oStr.Utf8(), "\xFD\xBF\xBF\xBF\xBF\xBF""bca\xFD\xBF\xBF\xBF\xBF\xBF""c") == 0);
	
	oStr.Apnd("ab\xFD\xBF\xBF\xBF\xBF\xBF");
	Test(oStr.ByteLength() == 24);
	Test(oStr.Length() == 9);
	Test(StringCompare(oStr.Utf8(), "\xFD\xBF\xBF\xBF\xBF\xBF""bca\xFD\xBF\xBF\xBF\xBF\xBF""cab\xFD\xBF\xBF\xBF\xBF\xBF") == 0);
	
	
	return 0;
}

static int TestApndFromPos()
{
	PrintFnM("-- WARNING: NOT IMPLEMENTED!");
	
	return 0;
}

static int TestInsFromRef()
{
	PrintFn();
	
	
	acpl::String oStr;
	
	
	// 1-byte seqence
	
	oStr.Ins(3, "abc");
	Test(oStr.ByteLength() == 3);
	Test(oStr.Length() == 3);
	Test(StringCompare(oStr.Utf8(), "abc") == 0);
	
	oStr.Ins(1, L"abc");
	Test(oStr.ByteLength() == 6);
	Test(oStr.Length() == 6);
	Test(StringCompare(oStr.Utf8(), "aabcbc") == 0);
	
	const acpl::Unichar oUcs[] = { 'a', 'b', 'c', '\0' };
	oStr.Ins(3, oUcs);
	Test(oStr.ByteLength() == 9);
	Test(oStr.Length() == 9);
	Test(StringCompare(oStr.Utf8(), "aababccbc") == 0);
	
	acpl::String oStr2("abc");
	oStr.Ins(8, oStr2);
	Test(oStr.ByteLength() == 12);
	Test(oStr.Length() == 12);
	Test(StringCompare(oStr.Utf8(), "aababccbabcc") == 0);
	
	oStr.Clear();
	Test(oStr.ByteLength() == 0);
	Test(oStr.Length() == 0);
	Test(StringCompare(oStr.Utf8(), "") == 0);
	
	
	// 2-byte seqence
	
	oStr.Ins(0, "\xDF\xBF""bc");
	Test(oStr.ByteLength() == 4);
	Test(oStr.Length() == 3);
	Test(StringCompare(oStr.Utf8(), "\xDF\xBF""bc") == 0);
	
	oStr.Ins(1, "a\xDF\xBF""c");
	Test(oStr.ByteLength() == 8);
	Test(oStr.Length() == 6);
	Test(StringCompare(oStr.Utf8(), "\xDF\xBF""a\xDF\xBF""cbc") == 0);
	
	oStr.Ins(6, "ab\xDF\xBF");
	Test(oStr.ByteLength() == 12);
	Test(oStr.Length() == 9);
	Test(StringCompare(oStr.Utf8(), "\xDF\xBF""a\xDF\xBF""cbcab\xDF\xBF") == 0);
	
	oStr.Clear();
	
	
	// 3-byte seqence
	
	oStr.Ins(0, "\xEF\xBF\xBF""bc");
	Test(oStr.ByteLength() == 5);
	Test(oStr.Length() == 3);
	Test(StringCompare(oStr.Utf8(), "\xEF\xBF\xBF""bc") == 0);
	
	oStr.Ins(2, "a\xEF\xBF\xBF""c");
	Test(oStr.ByteLength() == 10);
	Test(oStr.Length() == 6);
	Test(StringCompare(oStr.Utf8(), "\xEF\xBF\xBF""ba\xEF\xBF\xBF""cc") == 0);
	
	oStr.Ins(4, "ab\xEF\xBF\xBF");
	Test(oStr.ByteLength() == 15);
	Test(oStr.Length() == 9);
	Test(StringCompare(oStr.Utf8(), "\xEF\xBF\xBF""ba\xEF\xBF\xBF""ab\xEF\xBF\xBF""cc") == 0);
	
	oStr.Clear();
	
	
	// 4-byte seqence
	
	oStr.Ins(0, "\xF7\xBF\xBF\xBF""bc");
	Test(oStr.ByteLength() == 6);
	Test(oStr.Length() == 3);
	Test(StringCompare(oStr.Utf8(), "\xF7\xBF\xBF\xBF""bc") == 0);
	
	oStr.Ins(3, "a\xF7\xBF\xBF\xBF""c");
	Test(oStr.ByteLength() == 12);
	Test(oStr.Length() == 6);
	Test(StringCompare(oStr.Utf8(), "\xF7\xBF\xBF\xBF""bca\xF7\xBF\xBF\xBF""c") == 0);
	
	oStr.Ins(2, "ab\xF7\xBF\xBF\xBF");
	Test(oStr.ByteLength() == 18);
	Test(oStr.Length() == 9);
	Test(StringCompare(oStr.Utf8(), "\xF7\xBF\xBF\xBF""bab\xF7\xBF\xBF\xBF""ca\xF7\xBF\xBF\xBF""c") == 0);
	
	oStr.Clear();
	
	
	// 5-byte seqence
	
	oStr.Ins(0, "\xFB\xBF\xBF\xBF\xBF""bc");
	Test(oStr.ByteLength() == 7);
	Test(oStr.Length() == 3);
	Test(StringCompare(oStr.Utf8(), "\xFB\xBF\xBF\xBF\xBF""bc") == 0);
	
	oStr.Ins(1, "a\xFB\xBF\xBF\xBF\xBF""c");
	Test(oStr.ByteLength() == 14);
	Test(oStr.Length() == 6);
	Test(StringCompare(oStr.Utf8(), "\xFB\xBF\xBF\xBF\xBF""a\xFB\xBF\xBF\xBF\xBF""cbc") == 0);
	
	oStr.Ins(5, "ab\xFB\xBF\xBF\xBF\xBF");
	Test(oStr.ByteLength() == 21);
	Test(oStr.Length() == 9);
	Test(StringCompare(oStr.Utf8(), "\xFB\xBF\xBF\xBF\xBF""a\xFB\xBF\xBF\xBF\xBF""cbab\xFB\xBF\xBF\xBF\xBF""c") == 0);
	
	oStr.Clear();
	
	
	// 6-byte seqence
	
	oStr.Ins(0, "\xFD\xBF\xBF\xBF\xBF\xBF""bc");
	Test(oStr.ByteLength() == 8);
	Test(oStr.Length() == 3);
	Test(StringCompare(oStr.Utf8(), "\xFD\xBF\xBF\xBF\xBF\xBF""bc") == 0);
	
	oStr.Ins(2, "a\xFD\xBF\xBF\xBF\xBF\xBF""c");
	Test(oStr.ByteLength() == 16);
	Test(oStr.Length() == 6);
	Test(StringCompare(oStr.Utf8(), "\xFD\xBF\xBF\xBF\xBF\xBF""ba\xFD\xBF\xBF\xBF\xBF\xBF""cc") == 0);
	
	oStr.Ins(6, "ab\xFD\xBF\xBF\xBF\xBF\xBF");
	Test(oStr.ByteLength() == 24);
	Test(oStr.Length() == 9);
	Test(StringCompare(oStr.Utf8(), "\xFD\xBF\xBF\xBF\xBF\xBF""ba\xFD\xBF\xBF\xBF\xBF\xBF""ccab\xFD\xBF\xBF\xBF\xBF\xBF") == 0);
	
	
	return 0;
}

static int TestInsFromPos()
{
	PrintFn();
	
	
	acpl::String oStr;
	acpl::String::Pos oAtPos(oStr);
	
	
	// 1-byte seqence
	
	oStr.Ins(oAtPos, false, "abc");
	Test(oStr.ByteLength() == 3);
	Test(oStr.Length() == 3);
	Test(StringCompare(oStr.Utf8(), "abc") == 0);
	
	oAtPos.Next();
	oStr.Ins(oAtPos, true, L"abc");
	Test(oStr.ByteLength() == 6);
	Test(oStr.Length() == 6);
	Test(StringCompare(oStr.Utf8(), "aabcbc") == 0);
	
	oAtPos.Next();
	const acpl::Unichar oUcs[] = { 'a', 'b', 'c', '\0' };
	oStr.Ins(oAtPos, false, oUcs);
	Test(oStr.ByteLength() == 9);
	Test(oStr.Length() == 9);
	Test(StringCompare(oStr.Utf8(), "aababccbc") == 0);
	
	oAtPos.Forth(6);
	acpl::String oStr2("abc");
	oStr.Ins(oAtPos, true, oStr2);
	Test(oStr.ByteLength() == 12);
	Test(oStr.Length() == 12);
	Test(StringCompare(oStr.Utf8(), "aababccbabcc") == 0);
	
	oStr.Clear();
	Test(oStr.ByteLength() == 0);
	Test(oStr.Length() == 0);
	Test(StringCompare(oStr.Utf8(), "") == 0);
	
	
	// 2-byte seqence
	
	oStr.Ins(oAtPos, true, "\xDF\xBF""bc");
	Test(oStr.ByteLength() == 4);
	Test(oStr.Length() == 3);
	Test(StringCompare(oStr.Utf8(), "\xDF\xBF""bc") == 0);
	
	oAtPos.First();
	oStr.Ins(oAtPos, false, "a\xDF\xBF""c");
	Test(oStr.ByteLength() == 8);
	Test(oStr.Length() == 6);
	Test(StringCompare(oStr.Utf8(), "\xDF\xBF""a\xDF\xBF""cbc") == 0);
	
	oAtPos.Last();
	oStr.Ins(oAtPos, false, "ab\xDF\xBF");
	Test(oStr.ByteLength() == 12);
	Test(oStr.Length() == 9);
	Test(StringCompare(oStr.Utf8(), "\xDF\xBF""a\xDF\xBF""cbcab\xDF\xBF") == 0);
	
	oStr.Clear();
	
	
	// 3-byte seqence
	
	oStr.Ins(oAtPos, true, "\xEF\xBF\xBF""bc");
	Test(oStr.ByteLength() == 5);
	Test(oStr.Length() == 3);
	Test(StringCompare(oStr.Utf8(), "\xEF\xBF\xBF""bc") == 0);
	
	oAtPos.SkipFirst(2);
	oStr.Ins(oAtPos, true, "a\xEF\xBF\xBF""c");
	Test(oStr.ByteLength() == 10);
	Test(oStr.Length() == 6);
	Test(StringCompare(oStr.Utf8(), "\xEF\xBF\xBF""ba\xEF\xBF\xBF""cc") == 0);
	
	oAtPos.Next();
	oStr.Ins(oAtPos, false, "ab\xEF\xBF\xBF");
	Test(oStr.ByteLength() == 15);
	Test(oStr.Length() == 9);
	Test(StringCompare(oStr.Utf8(), "\xEF\xBF\xBF""ba\xEF\xBF\xBF""ab\xEF\xBF\xBF""cc") == 0);
	
	oStr.Clear();
	
	
	// 4-byte seqence
	
	oStr.Ins(oAtPos, true, "\xF7\xBF\xBF\xBF""bc");
	Test(oStr.ByteLength() == 6);
	Test(oStr.Length() == 3);
	Test(StringCompare(oStr.Utf8(), "\xF7\xBF\xBF\xBF""bc") == 0);
	
	oAtPos.SkipFirst(3); // actually will skip just 2
	oStr.Ins(oAtPos, false, "a\xF7\xBF\xBF\xBF""c");
	Test(oStr.ByteLength() == 12);
	Test(oStr.Length() == 6);
	Test(StringCompare(oStr.Utf8(), "\xF7\xBF\xBF\xBF""bca\xF7\xBF\xBF\xBF""c") == 0);
	
	oStr.Ins(oAtPos, true, "ab\xF7\xBF\xBF\xBF");
	Test(oStr.ByteLength() == 18);
	Test(oStr.Length() == 9);
	Test(StringCompare(oStr.Utf8(), "\xF7\xBF\xBF\xBF""bab\xF7\xBF\xBF\xBF""ca\xF7\xBF\xBF\xBF""c") == 0);
	
	oStr.Clear();
	
	
	// 5-byte seqence
	
	oStr.Ins(oAtPos, false, "\xFB\xBF\xBF\xBF\xBF""bc");
	Test(oStr.ByteLength() == 7);
	Test(oStr.Length() == 3);
	Test(StringCompare(oStr.Utf8(), "\xFB\xBF\xBF\xBF\xBF""bc") == 0);
	
	oAtPos.SkipFirst(1);
	oStr.Ins(oAtPos, true, "a\xFB\xBF\xBF\xBF\xBF""c");
	Test(oStr.ByteLength() == 14);
	Test(oStr.Length() == 6);
	Test(StringCompare(oStr.Utf8(), "\xFB\xBF\xBF\xBF\xBF""a\xFB\xBF\xBF\xBF\xBF""cbc") == 0);
	
	oAtPos.Forth(3);
	oStr.Ins(oAtPos, false, "ab\xFB\xBF\xBF\xBF\xBF");
	Test(oStr.ByteLength() == 21);
	Test(oStr.Length() == 9);
	Test(StringCompare(oStr.Utf8(), "\xFB\xBF\xBF\xBF\xBF""a\xFB\xBF\xBF\xBF\xBF""cbab\xFB\xBF\xBF\xBF\xBF""c") == 0);
	
	oStr.Clear();
	
	
	// 6-byte seqence
	
	oStr.Ins(oAtPos, false, "\xFD\xBF\xBF\xBF\xBF\xBF""bc");
	Test(oStr.ByteLength() == 8);
	Test(oStr.Length() == 3);
	Test(StringCompare(oStr.Utf8(), "\xFD\xBF\xBF\xBF\xBF\xBF""bc") == 0);
	
	oAtPos.SkipFirst(2);
	oStr.Ins(oAtPos, true, "a\xFD\xBF\xBF\xBF\xBF\xBF""c");
	Test(oStr.ByteLength() == 16);
	Test(oStr.Length() == 6);
	Test(StringCompare(oStr.Utf8(), "\xFD\xBF\xBF\xBF\xBF\xBF""ba\xFD\xBF\xBF\xBF\xBF\xBF""cc") == 0);
	
	oAtPos.Last();
	oStr.Ins(oAtPos, false, "ab\xFD\xBF\xBF\xBF\xBF\xBF");
	Test(oStr.ByteLength() == 24);
	Test(oStr.Length() == 9);
	Test(StringCompare(oStr.Utf8(), "\xFD\xBF\xBF\xBF\xBF\xBF""ba\xFD\xBF\xBF\xBF\xBF\xBF""ccab\xFD\xBF\xBF\xBF\xBF\xBF") == 0);
	
	
	return 0;
}

static int TestReplFromRef()
{
	PrintFn();
	
	
	acpl::String oStr;
	
	
	// 1-byte seqence
	
	oStr.Repl(3, 4, "abc");
	Test(oStr.ByteLength() == 3);
	Test(oStr.Length() == 3);
	Test(StringCompare(oStr.Utf8(), "abc") == 0);
	
	oStr.Repl(3, 2, L"abc");
	Test(oStr.ByteLength() == 6);
	Test(oStr.Length() == 6);
	Test(StringCompare(oStr.Utf8(), "abcabc") == 0);
	
	const acpl::Unichar oUcs[] = { 'a', 'b', 'c', '\0' };
	oStr.Repl(2, 3, oUcs);
	Test(oStr.ByteLength() == 6);
	Test(oStr.Length() == 6);
	Test(StringCompare(oStr.Utf8(), "ababcc") == 0);
	
	acpl::String oStr2("abc");
	oStr.Repl(3, 2, oStr2);
	Test(oStr.ByteLength() == 7);
	Test(oStr.Length() == 7);
	Test(StringCompare(oStr.Utf8(), "abaabcc") == 0);
	
	oStr.Repl(2, 0, oStr2);
	Test(oStr.ByteLength() == 10);
	Test(oStr.Length() == 10);
	Test(StringCompare(oStr.Utf8(), "ababcaabcc") == 0);
	
	oStr.Clear();
	Test(oStr.ByteLength() == 0);
	Test(oStr.Length() == 0);
	Test(StringCompare(oStr.Utf8(), "") == 0);
	
	
	// 2-byte seqence
	
	oStr.Repl(0, 0, "\xDF\xBF""bc");
	Test(oStr.ByteLength() == 4);
	Test(oStr.Length() == 3);
	Test(StringCompare(oStr.Utf8(), "\xDF\xBF""bc") == 0);
	
	oStr.Repl(1, 0, "a\xDF\xBF""c");
	Test(oStr.ByteLength() == 8);
	Test(oStr.Length() == 6);
	Test(StringCompare(oStr.Utf8(), "\xDF\xBF""a\xDF\xBF""cbc") == 0);
	
	oStr.Repl(2, 2, "ab\xDF\xBF");
	Test(oStr.ByteLength() == 9);
	Test(oStr.Length() == 7);
	Test(StringCompare(oStr.Utf8(), "\xDF\xBF""aab\xDF\xBF""bc") == 0);
	
	
	// 3-byte seqence
	
	oStr.Repl(0, 2, "\xEF\xBF\xBF""bc");
	Test(oStr.ByteLength() == 11);
	Test(oStr.Length() == 8);
	Test(StringCompare(oStr.Utf8(), "\xEF\xBF\xBF""bcab\xDF\xBF""bc") == 0);
	
	oStr.Repl(3, 3, "a\xEF\xBF\xBF""c");
	Test(oStr.ByteLength() == 12);
	Test(oStr.Length() == 8);
	Test(StringCompare(oStr.Utf8(), "\xEF\xBF\xBF""bca\xEF\xBF\xBF""cbc") == 0);
	
	oStr.Repl(1, 4, "ab\xEF\xBF\xBF");
	Test(oStr.ByteLength() == 11);
	Test(oStr.Length() == 7);
	Test(StringCompare(oStr.Utf8(), "\xEF\xBF\xBF""ab\xEF\xBF\xBF""cbc") == 0);
	
	
	// 4-byte seqence
	
	oStr.Repl(6, 2, "\xF7\xBF\xBF\xBF""bc");
	Test(oStr.ByteLength() == 16);
	Test(oStr.Length() == 9);
	Test(StringCompare(oStr.Utf8(), "\xEF\xBF\xBF""ab\xEF\xBF\xBF""cb\xF7\xBF\xBF\xBF""bc") == 0);
	
	oStr.Repl(3, 1, "a\xF7\xBF\xBF\xBF""c");
	Test(oStr.ByteLength() == 19);
	Test(oStr.Length() == 11);
	Test(StringCompare(oStr.Utf8(), "\xEF\xBF\xBF""aba\xF7\xBF\xBF\xBF""c""cb\xF7\xBF\xBF\xBF""bc") == 0);
	
	oStr.Repl(4, 5, "ab\xF7\xBF\xBF\xBF");
	Test(oStr.ByteLength() == 14);
	Test(oStr.Length() == 9);
	Test(StringCompare(oStr.Utf8(), "\xEF\xBF\xBF""abaab\xF7\xBF\xBF\xBF""bc") == 0);
	
	
	// 5-byte seqence
	
	oStr.Repl(3, 1, "\xFB\xBF\xBF\xBF\xBF""bc");
	Test(oStr.ByteLength() == 20);
	Test(oStr.Length() == 11);
	Test(StringCompare(oStr.Utf8(), "\xEF\xBF\xBF""ab\xFB\xBF\xBF\xBF\xBF""bcab\xF7\xBF\xBF\xBF""bc") == 0);
	
	oStr.Repl(3, 6, "a\xFB\xBF\xBF\xBF\xBF""c");
	Test(oStr.ByteLength() == 14);
	Test(oStr.Length() == 8);
	Test(StringCompare(oStr.Utf8(), "\xEF\xBF\xBF""aba\xFB\xBF\xBF\xBF\xBF""cbc") == 0);
	
	oStr.Repl(2, 3, "ab\xFB\xBF\xBF\xBF\xBF");
	Test(oStr.ByteLength() == 14);
	Test(oStr.Length() == 8);
	Test(StringCompare(oStr.Utf8(), "\xEF\xBF\xBF""aab\xFB\xBF\xBF\xBF\xBF""cbc") == 0);
	
	
	// 6-byte seqence
	
	oStr.Repl(acpl::Num<acpl::SizeT>::Max(), acpl::Num<acpl::SizeT>::Max(), "\xFD\xBF\xBF\xBF\xBF\xBF""bc");
	Test(oStr.ByteLength() == 22);
	Test(oStr.Length() == 11);
	Test(StringCompare(oStr.Utf8(), "\xEF\xBF\xBF""aab\xFB\xBF\xBF\xBF\xBF""cbc\xFD\xBF\xBF\xBF\xBF\xBF""bc") == 0);
	
	oStr.Repl(0, 5, "a\xFD\xBF\xBF\xBF\xBF\xBF""c");
	Test(oStr.ByteLength() == 19);
	Test(oStr.Length() == 9);
	Test(StringCompare(oStr.Utf8(), "a\xFD\xBF\xBF\xBF\xBF\xBF""ccbc\xFD\xBF\xBF\xBF\xBF\xBF""bc") == 0);
	
	oStr.Repl(4, 2, "ab\xFD\xBF\xBF\xBF\xBF\xBF");
	Test(oStr.ByteLength() == 25);
	Test(oStr.Length() == 10);
	Test(StringCompare(oStr.Utf8(), "a\xFD\xBF\xBF\xBF\xBF\xBF""ccab\xFD\xBF\xBF\xBF\xBF\xBF\xFD\xBF\xBF\xBF\xBF\xBF""bc") == 0);
	
	
	return 0;
}

static int TestReplFromPos()
{
	PrintFn();
	
	
	acpl::String oStr;
	acpl::String::Pos oAtPos(oStr);
	
	
	// 1-byte seqence
	
	oStr.Repl(oAtPos, true, 4, "abc");
	Test(oStr.ByteLength() == 3);
	Test(oStr.Length() == 3);
	Test(StringCompare(oStr.Utf8(), "abc") == 0);
	
	oAtPos.Forth(2);
	oStr.Repl(oAtPos, false, 2, L"abc");
	Test(oStr.ByteLength() == 6);
	Test(oStr.Length() == 6);
	Test(StringCompare(oStr.Utf8(), "abcabc") == 0);
	
	const acpl::Unichar oUcs[] = { 'a', 'b', 'c', '\0' };
	oStr.Repl(oAtPos, true, 3, oUcs);
	Test(oStr.ByteLength() == 6);
	Test(oStr.Length() == 6);
	Test(StringCompare(oStr.Utf8(), "ababcc") == 0);
	
	oAtPos.Next();
	acpl::String oStr2("abc");
	oStr.Repl(oAtPos, true, 2, oStr2);
	Test(oStr.ByteLength() == 7);
	Test(oStr.Length() == 7);
	Test(StringCompare(oStr.Utf8(), "abaabcc") == 0);
	
	oAtPos.Back(2);
	oStr.Repl(oAtPos, false, 0, oStr2);
	Test(oStr.ByteLength() == 10);
	Test(oStr.Length() == 10);
	Test(StringCompare(oStr.Utf8(), "ababcaabcc") == 0);
	
	oStr.Clear();
	Test(oStr.ByteLength() == 0);
	Test(oStr.Length() == 0);
	Test(StringCompare(oStr.Utf8(), "") == 0);
	
	
	// 2-byte seqence
	
	oStr.Repl(oAtPos, true, 0, "\xDF\xBF""bc");
	Test(oStr.ByteLength() == 4);
	Test(oStr.Length() == 3);
	Test(StringCompare(oStr.Utf8(), "\xDF\xBF""bc") == 0);
	
	oAtPos.First();
	oStr.Repl(oAtPos, false, 0, "a\xDF\xBF""c");
	Test(oStr.ByteLength() == 8);
	Test(oStr.Length() == 6);
	Test(StringCompare(oStr.Utf8(), "\xDF\xBF""a\xDF\xBF""cbc") == 0);
	
	oAtPos.Forth(2);
	oStr.Repl(oAtPos, true, 2, "ab\xDF\xBF");
	Test(oStr.ByteLength() == 9);
	Test(oStr.Length() == 7);
	Test(StringCompare(oStr.Utf8(), "\xDF\xBF""aab\xDF\xBF""bc") == 0);
	
	
	// 3-byte seqence
	
	oAtPos.Back(3); // will be 2
	oStr.Repl(oAtPos, true, 2, "\xEF\xBF\xBF""bc");
	Test(oStr.ByteLength() == 11);
	Test(oStr.Length() == 8);
	Test(StringCompare(oStr.Utf8(), "\xEF\xBF\xBF""bcab\xDF\xBF""bc") == 0);
	
	oAtPos.Forth(2);
	oStr.Repl(oAtPos, false, 3, "a\xEF\xBF\xBF""c");
	Test(oStr.ByteLength() == 12);
	Test(oStr.Length() == 8);
	Test(StringCompare(oStr.Utf8(), "\xEF\xBF\xBF""bca\xEF\xBF\xBF""cbc") == 0);
	
	oAtPos.Prev();
	oStr.Repl(oAtPos, true, 4, "ab\xEF\xBF\xBF");
	Test(oStr.ByteLength() == 11);
	Test(oStr.Length() == 7);
	Test(StringCompare(oStr.Utf8(), "\xEF\xBF\xBF""ab\xEF\xBF\xBF""cbc") == 0);
	
	
	// 4-byte seqence
	
	oAtPos.SkipLast(1);
	oStr.Repl(oAtPos, false, 2, "\xF7\xBF\xBF\xBF""bc");
	Test(oStr.ByteLength() == 16);
	Test(oStr.Length() == 9);
	Test(StringCompare(oStr.Utf8(), "\xEF\xBF\xBF""ab\xEF\xBF\xBF""cb\xF7\xBF\xBF\xBF""bc") == 0);
	
	oAtPos.Back(2);
	oStr.Repl(oAtPos, true, 1, "a\xF7\xBF\xBF\xBF""c");
	Test(oStr.ByteLength() == 19);
	Test(oStr.Length() == 11);
	Test(StringCompare(oStr.Utf8(), "\xEF\xBF\xBF""aba\xF7\xBF\xBF\xBF""ccb\xF7\xBF\xBF\xBF""bc") == 0);
	
	oStr.Repl(oAtPos, false, 5, "ab\xF7\xBF\xBF\xBF");
	Test(oStr.ByteLength() == 14);
	Test(oStr.Length() == 9);
	Test(StringCompare(oStr.Utf8(), "\xEF\xBF\xBF""abaab\xF7\xBF\xBF\xBF""bc") == 0);
	
	
	// 5-byte seqence
	
	oStr.Repl(oAtPos, true, 1, "\xFB\xBF\xBF\xBF\xBF""bc");
	Test(oStr.ByteLength() == 20);
	Test(oStr.Length() == 11);
	Test(StringCompare(oStr.Utf8(), "\xEF\xBF\xBF""ab\xFB\xBF\xBF\xBF\xBF""bcab\xF7\xBF\xBF\xBF""bc") == 0);
	
	oAtPos.Prev();
	oStr.Repl(oAtPos, false, 6, "a\xFB\xBF\xBF\xBF\xBF""c");
	Test(oStr.ByteLength() == 14);
	Test(oStr.Length() == 8);
	Test(StringCompare(oStr.Utf8(), "\xEF\xBF\xBF""aba\xFB\xBF\xBF\xBF\xBF""cbc") == 0);
	
	oStr.Repl(oAtPos, true, 3, "ab\xFB\xBF\xBF\xBF\xBF");
	Test(oStr.ByteLength() == 14);
	Test(oStr.Length() == 8);
	Test(StringCompare(oStr.Utf8(), "\xEF\xBF\xBF""aab\xFB\xBF\xBF\xBF\xBF""cbc") == 0);
	
	
	// 6-byte seqence
	
	oAtPos.Last();
	oStr.Repl(oAtPos, false, acpl::Num<acpl::SizeT>::Max(), "\xFD\xBF\xBF\xBF\xBF\xBF""bc");
	Test(oStr.ByteLength() == 22);
	Test(oStr.Length() == 11);
	Test(StringCompare(oStr.Utf8(), "\xEF\xBF\xBF""aab\xFB\xBF\xBF\xBF\xBF""cbc\xFD\xBF\xBF\xBF\xBF\xBF""bc") == 0);
	
	oAtPos.First();
	oStr.Repl(oAtPos, true, 5, "a\xFD\xBF\xBF\xBF\xBF\xBF""c");
	Test(oStr.ByteLength() == 19);
	Test(oStr.Length() == 9);
	Test(StringCompare(oStr.Utf8(), "a\xFD\xBF\xBF\xBF\xBF\xBF""ccbc\xFD\xBF\xBF\xBF\xBF\xBF""bc") == 0);
	
	oAtPos.Forth(3);
	oStr.Repl(oAtPos, false, 2, "ab\xFD\xBF\xBF\xBF\xBF\xBF");
	Test(oStr.ByteLength() == 25);
	Test(oStr.Length() == 10);
	Test(StringCompare(oStr.Utf8(), "a\xFD\xBF\xBF\xBF\xBF\xBF""ccab\xFD\xBF\xBF\xBF\xBF\xBF\xFD\xBF\xBF\xBF\xBF\xBF""bc") == 0);
	
	
	return 0;
}

static int TestReplFromPosToPos()
{
	PrintFn();
	
	
	acpl::String oStr;
	acpl::String::Pos oFromPos(oStr);
	acpl::String::Pos oToPos(oStr);
	
	
	// 1-byte seqence
	
	oStr.Repl(oFromPos, true, oToPos, false, "abc");
	Test(oStr.ByteLength() == 3);
	Test(oStr.Length() == 3);
	Test(StringCompare(oStr.Utf8(), "abc") == 0);
	
	oFromPos.Forth(2);
	oToPos.Last();
	oStr.Repl(oFromPos, false, oToPos, false, L"abc");
	Test(oStr.ByteLength() == 6);
	Test(oStr.Length() == 6);
	Test(StringCompare(oStr.Utf8(), "abcabc") == 0);
	
	oToPos.SkipLast(1);
	const acpl::Unichar oUcs[] = { 'a', 'b', 'c', '\0' };
	oStr.Repl(oFromPos, true, oToPos, false, oUcs);
	Test(oStr.ByteLength() == 6);
	Test(oStr.Length() == 6);
	Test(StringCompare(oStr.Utf8(), "ababcc") == 0);
	
	oFromPos.Next();
	oToPos.Attach(oFromPos).Forth(2);
	acpl::String oStr2("abc");
	oStr.Repl(oFromPos, true, oToPos, true, oStr2);
	Test(oStr.ByteLength() == 7);
	Test(oStr.Length() == 7);
	Test(StringCompare(oStr.Utf8(), "abaabcc") == 0);
	
	oFromPos.Back(2);
	oToPos.Attach(oFromPos);
	oStr.Repl(oFromPos, false, oToPos, false, oStr2);
	Test(oStr.ByteLength() == 10);
	Test(oStr.Length() == 10);
	Test(StringCompare(oStr.Utf8(), "ababcaabcc") == 0);
	
	oStr.Clear();
	Test(oStr.ByteLength() == 0);
	Test(oStr.Length() == 0);
	Test(StringCompare(oStr.Utf8(), "") == 0);
	
	
	// 2-byte seqence
	
	oStr.Repl(oFromPos, true, oToPos, false, "\xDF\xBF""bc");
	Test(oStr.ByteLength() == 4);
	Test(oStr.Length() == 3);
	Test(StringCompare(oStr.Utf8(), "\xDF\xBF""bc") == 0);
	
	oFromPos.First();
	oToPos.SkipFirst(1);
	oStr.Repl(oFromPos, false, oToPos, true, "a\xDF\xBF""c");
	Test(oStr.ByteLength() == 8);
	Test(oStr.Length() == 6);
	Test(StringCompare(oStr.Utf8(), "\xDF\xBF""a\xDF\xBF""cbc") == 0);
	
	oFromPos.Forth(2);
	oToPos.Forth(2);
	oStr.Repl(oFromPos, true, oToPos, false, "ab\xDF\xBF");
	Test(oStr.ByteLength() == 9);
	Test(oStr.Length() == 7);
	Test(StringCompare(oStr.Utf8(), "\xDF\xBF""aab\xDF\xBF""bc") == 0);
	
	
	// 3-byte seqence
	
	oFromPos.Back(3); // will be 2
	oToPos.SkipFirst(1);
	oStr.Repl(oFromPos, true, oToPos, false, "\xEF\xBF\xBF""bc");
	Test(oStr.ByteLength() == 11);
	Test(oStr.Length() == 8);
	Test(StringCompare(oStr.Utf8(), "\xEF\xBF\xBF""bcab\xDF\xBF""bc") == 0);
	
	oFromPos.Forth(2);
	oToPos.Attach(oFromPos).Forth(4);
	oStr.Repl(oFromPos, false, oToPos, true, "a\xEF\xBF\xBF""c");
	Test(oStr.ByteLength() == 12);
	Test(oStr.Length() == 8);
	Test(StringCompare(oStr.Utf8(), "\xEF\xBF\xBF""bca\xEF\xBF\xBF""cbc") == 0);
	
	oFromPos.Prev();
	oToPos.Attach(oFromPos).Forth(3);
	oStr.Repl(oFromPos, true, oToPos, false, "ab\xEF\xBF\xBF");
	Test(oStr.ByteLength() == 11);
	Test(oStr.Length() == 7);
	Test(StringCompare(oStr.Utf8(), "\xEF\xBF\xBF""ab\xEF\xBF\xBF""cbc") == 0);
	
	
	// 4-byte seqence
	
	oFromPos.SkipLast(1);
	oToPos.Last();
	oStr.Repl(oFromPos, false, oToPos, false, "\xF7\xBF\xBF\xBF""bc");
	Test(oStr.ByteLength() == 16);
	Test(oStr.Length() == 9);
	Test(StringCompare(oStr.Utf8(), "\xEF\xBF\xBF""ab\xEF\xBF\xBF""cb\xF7\xBF\xBF\xBF""bc") == 0);
	
	oFromPos.Back(2);
	oToPos.Attach(oFromPos);
	oStr.Repl(oFromPos, true, oToPos, false, "a\xF7\xBF\xBF\xBF""c");
	Test(oStr.ByteLength() == 19);
	Test(oStr.Length() == 11);
	Test(StringCompare(oStr.Utf8(), "\xEF\xBF\xBF""aba\xF7\xBF\xBF\xBF""ccb\xF7\xBF\xBF\xBF""bc") == 0);
	
	oToPos.SkipLast(1);
	oStr.Repl(oFromPos, false, oToPos, true, "ab\xF7\xBF\xBF\xBF");
	Test(oStr.ByteLength() == 14);
	Test(oStr.Length() == 9);
	Test(StringCompare(oStr.Utf8(), "\xEF\xBF\xBF""abaab\xF7\xBF\xBF\xBF""bc") == 0);
	
	
	// 5-byte seqence
	
	oToPos.Attach(oFromPos);
	oStr.Repl(oFromPos, true, oToPos, false, "\xFB\xBF\xBF\xBF\xBF""bc");
	Test(oStr.ByteLength() == 20);
	Test(oStr.Length() == 11);
	Test(StringCompare(oStr.Utf8(), "\xEF\xBF\xBF""ab\xFB\xBF\xBF\xBF\xBF""bcab\xF7\xBF\xBF\xBF""bc") == 0);
	
	oFromPos.Prev();
	oToPos.Forth(6);
	oStr.Repl(oFromPos, false, oToPos, true, "a\xFB\xBF\xBF\xBF\xBF""c");
	Test(oStr.ByteLength() == 14);
	Test(oStr.Length() == 8);
	Test(StringCompare(oStr.Utf8(), "\xEF\xBF\xBF""aba\xFB\xBF\xBF\xBF\xBF""cbc") == 0);
	
	oToPos.Attach(oFromPos).Forth(2);
	oStr.Repl(oFromPos, true, oToPos, false, "ab\xFB\xBF\xBF\xBF\xBF");
	Test(oStr.ByteLength() == 14);
	Test(oStr.Length() == 8);
	Test(StringCompare(oStr.Utf8(), "\xEF\xBF\xBF""aab\xFB\xBF\xBF\xBF\xBF""cbc") == 0);
	
	
	// 6-byte seqence
	
	oFromPos.Last();
	oToPos.Last();
	oStr.Repl(oFromPos, false, oToPos, false, "\xFD\xBF\xBF\xBF\xBF\xBF""bc");
	Test(oStr.ByteLength() == 22);
	Test(oStr.Length() == 11);
	Test(StringCompare(oStr.Utf8(), "\xEF\xBF\xBF""aab\xFB\xBF\xBF\xBF\xBF""cbc\xFD\xBF\xBF\xBF\xBF\xBF""bc") == 0);
	
	oFromPos.First();
	oToPos.SkipFirst(4);
	oStr.Repl(oFromPos, true, oToPos, false, "a\xFD\xBF\xBF\xBF\xBF\xBF""c");
	Test(oStr.ByteLength() == 19);
	Test(oStr.Length() == 9);
	Test(StringCompare(oStr.Utf8(), "a\xFD\xBF\xBF\xBF\xBF\xBF""ccbc\xFD\xBF\xBF\xBF\xBF\xBF""bc") == 0);
	
	oFromPos.Forth(3);
	oToPos.SkipFirst(6);
	oStr.Repl(oFromPos, false, oToPos, true, "ab\xFD\xBF\xBF\xBF\xBF\xBF");
	Test(oStr.ByteLength() == 25);
	Test(oStr.Length() == 10);
	Test(StringCompare(oStr.Utf8(), "a\xFD\xBF\xBF\xBF\xBF\xBF""ccab\xFD\xBF\xBF\xBF\xBF\xBF\xFD\xBF\xBF\xBF\xBF\xBF""bc") == 0);
	
	
	return 0;
}

static int TestPadLeft()
{
	PrintFn();
	
	
	acpl::String oStr;
	
	
	// 1-byte seqence
	
	oStr.Clear().PadLeft('p', 0);
	Test(oStr.ByteLength() == 0);
	Test(oStr.Length() == 0);
	Test(StringCompare(oStr.Utf8(), "") == 0);
	
	oStr.Clear().PadLeft('p', 1);
	Test(oStr.ByteLength() == 1);
	Test(oStr.Length() == 1);
	Test(StringCompare(oStr.Utf8(), "p") == 0);
	
	oStr.Clear().PadLeft('p', 3);
	Test(oStr.ByteLength() == 3);
	Test(oStr.Length() == 3);
	Test(StringCompare(oStr.Utf8(), "ppp") == 0);
	
	oStr.Set("abc").PadLeft('p', 3);
	Test(oStr.ByteLength() == 3);
	Test(oStr.Length() == 3);
	Test(StringCompare(oStr.Utf8(), "abc") == 0);
	
	oStr.Set("abc").PadLeft('p', 4);
	Test(oStr.ByteLength() == 4);
	Test(oStr.Length() == 4);
	Test(StringCompare(oStr.Utf8(), "pabc") == 0);
	
	oStr.Set("abc").PadLeft('p', 6);
	Test(oStr.ByteLength() == 6);
	Test(oStr.Length() == 6);
	Test(StringCompare(oStr.Utf8(), "pppabc") == 0);
	
	
	// 2-byte seqence
	
	oStr.Clear().PadLeft(0x000007FF, 0);
	Test(oStr.ByteLength() == 0);
	Test(oStr.Length() == 0);
	Test(StringCompare(oStr.Utf8(), "") == 0);
	
	oStr.Clear().PadLeft(0x000007FF, 1);
	Test(oStr.ByteLength() == 2);
	Test(oStr.Length() == 1);
	Test(StringCompare(oStr.Utf8(), "\xDF\xBF") == 0);
	
	oStr.Clear().PadLeft(0x000007FF, 3);
	Test(oStr.ByteLength() == 6);
	Test(oStr.Length() == 3);
	Test(StringCompare(oStr.Utf8(), "\xDF\xBF\xDF\xBF\xDF\xBF") == 0);
	
	oStr.Set("abc").PadLeft(0x000007FF, 3);
	Test(oStr.ByteLength() == 3);
	Test(oStr.Length() == 3);
	Test(StringCompare(oStr.Utf8(), "abc") == 0);
	
	oStr.Set("abc").PadLeft(0x000007FF, 4);
	Test(oStr.ByteLength() == 5);
	Test(oStr.Length() == 4);
	Test(StringCompare(oStr.Utf8(), "\xDF\xBF""abc") == 0);
	
	oStr.Set("abc").PadLeft(0x000007FF, 6);
	Test(oStr.ByteLength() == 9);
	Test(oStr.Length() == 6);
	Test(StringCompare(oStr.Utf8(), "\xDF\xBF\xDF\xBF\xDF\xBF""abc") == 0);
	
	
	// 3-byte seqence
	
	oStr.Clear().PadLeft(0x0000FFFF, 0);
	Test(oStr.ByteLength() == 0);
	Test(oStr.Length() == 0);
	Test(StringCompare(oStr.Utf8(), "") == 0);
	
	oStr.Clear().PadLeft(0x0000FFFF, 1);
	Test(oStr.ByteLength() == 3);
	Test(oStr.Length() == 1);
	Test(StringCompare(oStr.Utf8(), "\xEF\xBF\xBF") == 0);
	
	oStr.Clear().PadLeft(0x0000FFFF, 3);
	Test(oStr.ByteLength() == 9);
	Test(oStr.Length() == 3);
	Test(StringCompare(oStr.Utf8(), "\xEF\xBF\xBF\xEF\xBF\xBF\xEF\xBF\xBF") == 0);
	
	oStr.Set("abc").PadLeft(0x0000FFFF, 3);
	Test(oStr.ByteLength() == 3);
	Test(oStr.Length() == 3);
	Test(StringCompare(oStr.Utf8(), "abc") == 0);
	
	oStr.Set("abc").PadLeft(0x0000FFFF, 4);
	Test(oStr.ByteLength() == 6);
	Test(oStr.Length() == 4);
	Test(StringCompare(oStr.Utf8(), "\xEF\xBF\xBF""abc") == 0);
	
	oStr.Set("abc").PadLeft(0x0000FFFF, 6);
	Test(oStr.ByteLength() == 12);
	Test(oStr.Length() == 6);
	Test(StringCompare(oStr.Utf8(), "\xEF\xBF\xBF\xEF\xBF\xBF\xEF\xBF\xBF""abc") == 0);
	
	
	// 4-byte seqence
	
	oStr.Clear().PadLeft(0x001FFFFF, 0);
	Test(oStr.ByteLength() == 0);
	Test(oStr.Length() == 0);
	Test(StringCompare(oStr.Utf8(), "") == 0);
	
	oStr.Clear().PadLeft(0x001FFFFF, 1);
	Test(oStr.ByteLength() == 4);
	Test(oStr.Length() == 1);
	Test(StringCompare(oStr.Utf8(), "\xF7\xBF\xBF\xBF") == 0);
	
	oStr.Clear().PadLeft(0x001FFFFF, 3);
	Test(oStr.ByteLength() == 12);
	Test(oStr.Length() == 3);
	Test(StringCompare(oStr.Utf8(), "\xF7\xBF\xBF\xBF\xF7\xBF\xBF\xBF\xF7\xBF\xBF\xBF") == 0);
	
	oStr.Set("abc").PadLeft(0x001FFFFF, 3);
	Test(oStr.ByteLength() == 3);
	Test(oStr.Length() == 3);
	Test(StringCompare(oStr.Utf8(), "abc") == 0);
	
	oStr.Set("abc").PadLeft(0x001FFFFF, 4);
	Test(oStr.ByteLength() == 7);
	Test(oStr.Length() == 4);
	Test(StringCompare(oStr.Utf8(), "\xF7\xBF\xBF\xBF""abc") == 0);
	
	oStr.Set("abc").PadLeft(0x001FFFFF, 6);
	Test(oStr.ByteLength() == 15);
	Test(oStr.Length() == 6);
	Test(StringCompare(oStr.Utf8(), "\xF7\xBF\xBF\xBF\xF7\xBF\xBF\xBF\xF7\xBF\xBF\xBF""abc") == 0);
	
	
	// 5-byte seqence
	
	oStr.Clear().PadLeft(0x03FFFFFF, 0);
	Test(oStr.ByteLength() == 0);
	Test(oStr.Length() == 0);
	Test(StringCompare(oStr.Utf8(), "") == 0);
	
	oStr.Clear().PadLeft(0x03FFFFFF, 1);
	Test(oStr.ByteLength() == 5);
	Test(oStr.Length() == 1);
	Test(StringCompare(oStr.Utf8(), "\xFB\xBF\xBF\xBF\xBF") == 0);
	
	oStr.Clear().PadLeft(0x03FFFFFF, 3);
	Test(oStr.ByteLength() == 15);
	Test(oStr.Length() == 3);
	Test(StringCompare(oStr.Utf8(), "\xFB\xBF\xBF\xBF\xBF\xFB\xBF\xBF\xBF\xBF\xFB\xBF\xBF\xBF\xBF") == 0);
	
	oStr.Set("abc").PadLeft(0x03FFFFFF, 3);
	Test(oStr.ByteLength() == 3);
	Test(oStr.Length() == 3);
	Test(StringCompare(oStr.Utf8(), "abc") == 0);
	
	oStr.Set("abc").PadLeft(0x03FFFFFF, 4);
	Test(oStr.ByteLength() == 8);
	Test(oStr.Length() == 4);
	Test(StringCompare(oStr.Utf8(), "\xFB\xBF\xBF\xBF\xBF""abc") == 0);
	
	oStr.Set("abc").PadLeft(0x03FFFFFF, 6);
	Test(oStr.ByteLength() == 18);
	Test(oStr.Length() == 6);
	Test(StringCompare(oStr.Utf8(), "\xFB\xBF\xBF\xBF\xBF\xFB\xBF\xBF\xBF\xBF\xFB\xBF\xBF\xBF\xBF""abc") == 0);
	
	
	// 6-byte seqence
	
	oStr.Clear().PadLeft(0x7FFFFFFF, 0);
	Test(oStr.ByteLength() == 0);
	Test(oStr.Length() == 0);
	Test(StringCompare(oStr.Utf8(), "") == 0);
	
	oStr.Clear().PadLeft(0x7FFFFFFF, 1);
	Test(oStr.ByteLength() == 6);
	Test(oStr.Length() == 1);
	Test(StringCompare(oStr.Utf8(), "\xFD\xBF\xBF\xBF\xBF\xBF") == 0);
	
	oStr.Clear().PadLeft(0x7FFFFFFF, 3);
	Test(oStr.ByteLength() == 18);
	Test(oStr.Length() == 3);
	Test(StringCompare(oStr.Utf8(), "\xFD\xBF\xBF\xBF\xBF\xBF\xFD\xBF\xBF\xBF\xBF\xBF\xFD\xBF\xBF\xBF\xBF\xBF") == 0);
	
	oStr.Set("abc").PadLeft(0x7FFFFFFF, 3);
	Test(oStr.ByteLength() == 3);
	Test(oStr.Length() == 3);
	Test(StringCompare(oStr.Utf8(), "abc") == 0);
	
	oStr.Set("abc").PadLeft(0x7FFFFFFF, 4);
	Test(oStr.ByteLength() == 9);
	Test(oStr.Length() == 4);
	Test(StringCompare(oStr.Utf8(), "\xFD\xBF\xBF\xBF\xBF\xBF""abc") == 0);
	
	oStr.Set("abc").PadLeft(0x7FFFFFFF, 6);
	Test(oStr.ByteLength() == 21);
	Test(oStr.Length() == 6);
	Test(StringCompare(oStr.Utf8(), "\xFD\xBF\xBF\xBF\xBF\xBF\xFD\xBF\xBF\xBF\xBF\xBF\xFD\xBF\xBF\xBF\xBF\xBF""abc") == 0);
	
	
	// Invalid seqence
	
	oStr.Clear().PadLeft(0x80000000, 0);
	Test(oStr.ByteLength() == 0);
	Test(oStr.Length() == 0);
	Test(StringCompare(oStr.Utf8(), "") == 0);
	
	oStr.Clear().PadLeft(0x80000000, 1);
	Test(oStr.ByteLength() == 0);
	Test(oStr.Length() == 0);
	Test(StringCompare(oStr.Utf8(), "") == 0);
	
	oStr.Clear().PadLeft(0x80000000, 3);
	Test(oStr.ByteLength() == 0);
	Test(oStr.Length() == 0);
	Test(StringCompare(oStr.Utf8(), "") == 0);
	
	oStr.Set("abc").PadLeft(0x80000000, 3);
	Test(oStr.ByteLength() == 3);
	Test(oStr.Length() == 3);
	Test(StringCompare(oStr.Utf8(), "abc") == 0);
	
	oStr.Set("abc").PadLeft(0x80000000, 4);
	Test(oStr.ByteLength() == 3);
	Test(oStr.Length() == 3);
	Test(StringCompare(oStr.Utf8(), "abc") == 0);
	
	oStr.Set("abc").PadLeft(0x80000000, 6);
	Test(oStr.ByteLength() == 3);
	Test(oStr.Length() == 3);
	Test(StringCompare(oStr.Utf8(), "abc") == 0);
	
	
	return 0;
}

static int TestPadRight()
{
	PrintFn();
	
	
	acpl::String oStr;
	
	
	// 1-byte seqence
	
	oStr.Clear().PadRight('p', 0);
	Test(oStr.ByteLength() == 0);
	Test(oStr.Length() == 0);
	Test(StringCompare(oStr.Utf8(), "") == 0);
	
	oStr.Clear().PadRight('p', 1);
	Test(oStr.ByteLength() == 1);
	Test(oStr.Length() == 1);
	Test(StringCompare(oStr.Utf8(), "p") == 0);
	
	oStr.Clear().PadRight('p', 3);
	Test(oStr.ByteLength() == 3);
	Test(oStr.Length() == 3);
	Test(StringCompare(oStr.Utf8(), "ppp") == 0);
	
	oStr.Set("abc").PadRight('p', 3);
	Test(oStr.ByteLength() == 3);
	Test(oStr.Length() == 3);
	Test(StringCompare(oStr.Utf8(), "abc") == 0);
	
	oStr.Set("abc").PadRight('p', 4);
	Test(oStr.ByteLength() == 4);
	Test(oStr.Length() == 4);
	Test(StringCompare(oStr.Utf8(), "abcp") == 0);
	
	oStr.Set("abc").PadRight('p', 6);
	Test(oStr.ByteLength() == 6);
	Test(oStr.Length() == 6);
	Test(StringCompare(oStr.Utf8(), "abcppp") == 0);
	
	
	// 2-byte seqence
	
	oStr.Clear().PadRight(0x000007FF, 0);
	Test(oStr.ByteLength() == 0);
	Test(oStr.Length() == 0);
	Test(StringCompare(oStr.Utf8(), "") == 0);
	
	oStr.Clear().PadRight(0x000007FF, 1);
	Test(oStr.ByteLength() == 2);
	Test(oStr.Length() == 1);
	Test(StringCompare(oStr.Utf8(), "\xDF\xBF") == 0);
	
	oStr.Clear().PadRight(0x000007FF, 3);
	Test(oStr.ByteLength() == 6);
	Test(oStr.Length() == 3);
	Test(StringCompare(oStr.Utf8(), "\xDF\xBF\xDF\xBF\xDF\xBF") == 0);
	
	oStr.Set("abc").PadRight(0x000007FF, 3);
	Test(oStr.ByteLength() == 3);
	Test(oStr.Length() == 3);
	Test(StringCompare(oStr.Utf8(), "abc") == 0);
	
	oStr.Set("abc").PadRight(0x000007FF, 4);
	Test(oStr.ByteLength() == 5);
	Test(oStr.Length() == 4);
	Test(StringCompare(oStr.Utf8(), "abc\xDF\xBF") == 0);
	
	oStr.Set("abc").PadRight(0x000007FF, 6);
	Test(oStr.ByteLength() == 9);
	Test(oStr.Length() == 6);
	Test(StringCompare(oStr.Utf8(), "abc\xDF\xBF\xDF\xBF\xDF\xBF") == 0);
	
	
	// 3-byte seqence
	
	oStr.Clear().PadRight(0x0000FFFF, 0);
	Test(oStr.ByteLength() == 0);
	Test(oStr.Length() == 0);
	Test(StringCompare(oStr.Utf8(), "") == 0);
	
	oStr.Clear().PadRight(0x0000FFFF, 1);
	Test(oStr.ByteLength() == 3);
	Test(oStr.Length() == 1);
	Test(StringCompare(oStr.Utf8(), "\xEF\xBF\xBF") == 0);
	
	oStr.Clear().PadRight(0x0000FFFF, 3);
	Test(oStr.ByteLength() == 9);
	Test(oStr.Length() == 3);
	Test(StringCompare(oStr.Utf8(), "\xEF\xBF\xBF\xEF\xBF\xBF\xEF\xBF\xBF") == 0);
	
	oStr.Set("abc").PadRight(0x0000FFFF, 3);
	Test(oStr.ByteLength() == 3);
	Test(oStr.Length() == 3);
	Test(StringCompare(oStr.Utf8(), "abc") == 0);
	
	oStr.Set("abc").PadRight(0x0000FFFF, 4);
	Test(oStr.ByteLength() == 6);
	Test(oStr.Length() == 4);
	Test(StringCompare(oStr.Utf8(), "abc\xEF\xBF\xBF") == 0);
	
	oStr.Set("abc").PadRight(0x0000FFFF, 6);
	Test(oStr.ByteLength() == 12);
	Test(oStr.Length() == 6);
	Test(StringCompare(oStr.Utf8(), "abc\xEF\xBF\xBF\xEF\xBF\xBF\xEF\xBF\xBF") == 0);
	
	
	// 4-byte seqence
	
	oStr.Clear().PadRight(0x001FFFFF, 0);
	Test(oStr.ByteLength() == 0);
	Test(oStr.Length() == 0);
	Test(StringCompare(oStr.Utf8(), "") == 0);
	
	oStr.Clear().PadRight(0x001FFFFF, 1);
	Test(oStr.ByteLength() == 4);
	Test(oStr.Length() == 1);
	Test(StringCompare(oStr.Utf8(), "\xF7\xBF\xBF\xBF") == 0);
	
	oStr.Clear().PadRight(0x001FFFFF, 3);
	Test(oStr.ByteLength() == 12);
	Test(oStr.Length() == 3);
	Test(StringCompare(oStr.Utf8(), "\xF7\xBF\xBF\xBF\xF7\xBF\xBF\xBF\xF7\xBF\xBF\xBF") == 0);
	
	oStr.Set("abc").PadRight(0x001FFFFF, 3);
	Test(oStr.ByteLength() == 3);
	Test(oStr.Length() == 3);
	Test(StringCompare(oStr.Utf8(), "abc") == 0);
	
	oStr.Set("abc").PadRight(0x001FFFFF, 4);
	Test(oStr.ByteLength() == 7);
	Test(oStr.Length() == 4);
	Test(StringCompare(oStr.Utf8(), "abc\xF7\xBF\xBF\xBF") == 0);
	
	oStr.Set("abc").PadRight(0x001FFFFF, 6);
	Test(oStr.ByteLength() == 15);
	Test(oStr.Length() == 6);
	Test(StringCompare(oStr.Utf8(), "abc\xF7\xBF\xBF\xBF\xF7\xBF\xBF\xBF\xF7\xBF\xBF\xBF") == 0);
	
	
	// 5-byte seqence
	
	oStr.Clear().PadRight(0x03FFFFFF, 0);
	Test(oStr.ByteLength() == 0);
	Test(oStr.Length() == 0);
	Test(StringCompare(oStr.Utf8(), "") == 0);
	
	oStr.Clear().PadRight(0x03FFFFFF, 1);
	Test(oStr.ByteLength() == 5);
	Test(oStr.Length() == 1);
	Test(StringCompare(oStr.Utf8(), "\xFB\xBF\xBF\xBF\xBF") == 0);
	
	oStr.Clear().PadRight(0x03FFFFFF, 3);
	Test(oStr.ByteLength() == 15);
	Test(oStr.Length() == 3);
	Test(StringCompare(oStr.Utf8(), "\xFB\xBF\xBF\xBF\xBF\xFB\xBF\xBF\xBF\xBF\xFB\xBF\xBF\xBF\xBF") == 0);
	
	oStr.Set("abc").PadRight(0x03FFFFFF, 3);
	Test(oStr.ByteLength() == 3);
	Test(oStr.Length() == 3);
	Test(StringCompare(oStr.Utf8(), "abc") == 0);
	
	oStr.Set("abc").PadRight(0x03FFFFFF, 4);
	Test(oStr.ByteLength() == 8);
	Test(oStr.Length() == 4);
	Test(StringCompare(oStr.Utf8(), "abc\xFB\xBF\xBF\xBF\xBF") == 0);
	
	oStr.Set("abc").PadRight(0x03FFFFFF, 6);
	Test(oStr.ByteLength() == 18);
	Test(oStr.Length() == 6);
	Test(StringCompare(oStr.Utf8(), "abc\xFB\xBF\xBF\xBF\xBF\xFB\xBF\xBF\xBF\xBF\xFB\xBF\xBF\xBF\xBF") == 0);
	
	
	// 6-byte seqence
	
	oStr.Clear().PadRight(0x7FFFFFFF, 0);
	Test(oStr.ByteLength() == 0);
	Test(oStr.Length() == 0);
	Test(StringCompare(oStr.Utf8(), "") == 0);
	
	oStr.Clear().PadRight(0x7FFFFFFF, 1);
	Test(oStr.ByteLength() == 6);
	Test(oStr.Length() == 1);
	Test(StringCompare(oStr.Utf8(), "\xFD\xBF\xBF\xBF\xBF\xBF") == 0);
	
	oStr.Clear().PadRight(0x7FFFFFFF, 3);
	Test(oStr.ByteLength() == 18);
	Test(oStr.Length() == 3);
	Test(StringCompare(oStr.Utf8(), "\xFD\xBF\xBF\xBF\xBF\xBF\xFD\xBF\xBF\xBF\xBF\xBF\xFD\xBF\xBF\xBF\xBF\xBF") == 0);
	
	oStr.Set("abc").PadRight(0x7FFFFFFF, 3);
	Test(oStr.ByteLength() == 3);
	Test(oStr.Length() == 3);
	Test(StringCompare(oStr.Utf8(), "abc") == 0);
	
	oStr.Set("abc").PadRight(0x7FFFFFFF, 4);
	Test(oStr.ByteLength() == 9);
	Test(oStr.Length() == 4);
	Test(StringCompare(oStr.Utf8(), "abc\xFD\xBF\xBF\xBF\xBF\xBF") == 0);
	
	oStr.Set("abc").PadRight(0x7FFFFFFF, 6);
	Test(oStr.ByteLength() == 21);
	Test(oStr.Length() == 6);
	Test(StringCompare(oStr.Utf8(), "abc\xFD\xBF\xBF\xBF\xBF\xBF\xFD\xBF\xBF\xBF\xBF\xBF\xFD\xBF\xBF\xBF\xBF\xBF") == 0);
	
	
	// Invalid seqence
	
	oStr.Clear().PadRight(0x80000000, 0);
	Test(oStr.ByteLength() == 0);
	Test(oStr.Length() == 0);
	Test(StringCompare(oStr.Utf8(), "") == 0);
	
	oStr.Clear().PadRight(0x80000000, 1);
	Test(oStr.ByteLength() == 0);
	Test(oStr.Length() == 0);
	Test(StringCompare(oStr.Utf8(), "") == 0);
	
	oStr.Clear().PadRight(0x80000000, 3);
	Test(oStr.ByteLength() == 0);
	Test(oStr.Length() == 0);
	Test(StringCompare(oStr.Utf8(), "") == 0);
	
	oStr.Set("abc").PadRight(0x80000000, 3);
	Test(oStr.ByteLength() == 3);
	Test(oStr.Length() == 3);
	Test(StringCompare(oStr.Utf8(), "abc") == 0);
	
	oStr.Set("abc").PadRight(0x80000000, 4);
	Test(oStr.ByteLength() == 3);
	Test(oStr.Length() == 3);
	Test(StringCompare(oStr.Utf8(), "abc") == 0);
	
	oStr.Set("abc").PadRight(0x80000000, 6);
	Test(oStr.ByteLength() == 3);
	Test(oStr.Length() == 3);
	Test(StringCompare(oStr.Utf8(), "abc") == 0);
	
	
	return 0;
}

static int TestCutIndexCount()
{
	PrintFn();
	
	
	acpl::String oStr;
	
	
	// 1-byte seqence
	
	oStr.Set("abcabcabc").Cut(0, 0);
	Test(oStr.ByteLength() == 9);
	Test(oStr.Length() == 9);
	Test(StringCompare(oStr.Utf8(), "abcabcabc") == 0);
	
	oStr.Set("abcabcabc").Cut(0, 3);
	Test(oStr.ByteLength() == 6);
	Test(oStr.Length() == 6);
	Test(StringCompare(oStr.Utf8(), "abcabc") == 0);
	
	oStr.Set("abcabcabc").Cut(1, 5);
	Test(oStr.ByteLength() == 4);
	Test(oStr.Length() == 4);
	Test(StringCompare(oStr.Utf8(), "aabc") == 0);
	
	oStr.Set("abcabcabc").Cut(5, 4);
	Test(oStr.ByteLength() == 5);
	Test(oStr.Length() == 5);
	Test(StringCompare(oStr.Utf8(), "abcab") == 0);
	
	oStr.Set("abcabcabc").Cut(5, 5);
	Test(oStr.ByteLength() == 5);
	Test(oStr.Length() == 5);
	Test(StringCompare(oStr.Utf8(), "abcab") == 0);
	
	oStr.Set("abcabcabc").Cut(8, 1);
	Test(oStr.ByteLength() == 8);
	Test(oStr.Length() == 8);
	Test(StringCompare(oStr.Utf8(), "abcabcab") == 0);
	
	oStr.Set("abcabcabc").Cut(9, 1);
	Test(oStr.ByteLength() == 9);
	Test(oStr.Length() == 9);
	Test(StringCompare(oStr.Utf8(), "abcabcabc") == 0);
	
	
	// 2-byte seqence
	
	oStr.Set("\xDF\xBF""bca\xDF\xBF""cab\xDF\xBF").Cut(0, 0);
	Test(oStr.ByteLength() == 12);
	Test(oStr.Length() == 9);
	Test(StringCompare(oStr.Utf8(), "\xDF\xBF""bca\xDF\xBF""cab\xDF\xBF") == 0);
	
	oStr.Set("\xDF\xBF""bca\xDF\xBF""cab\xDF\xBF").Cut(0, 3);
	Test(oStr.ByteLength() == 8);
	Test(oStr.Length() == 6);
	Test(StringCompare(oStr.Utf8(), "a\xDF\xBF""cab\xDF\xBF") == 0);
	
	oStr.Set("\xDF\xBF""bca\xDF\xBF""cab\xDF\xBF").Cut(1, 5);
	Test(oStr.ByteLength() == 6);
	Test(oStr.Length() == 4);
	Test(StringCompare(oStr.Utf8(), "\xDF\xBF""ab\xDF\xBF") == 0);
	
	oStr.Set("\xDF\xBF""bca\xDF\xBF""cab\xDF\xBF").Cut(5, 4);
	Test(oStr.ByteLength() == 7);
	Test(oStr.Length() == 5);
	Test(StringCompare(oStr.Utf8(), "\xDF\xBF""bca\xDF\xBF") == 0);
	
	oStr.Set("\xDF\xBF""bca\xDF\xBF""cab\xDF\xBF").Cut(5, 5);
	Test(oStr.ByteLength() == 7);
	Test(oStr.Length() == 5);
	Test(StringCompare(oStr.Utf8(), "\xDF\xBF""bca\xDF\xBF") == 0);
	
	oStr.Set("\xDF\xBF""bca\xDF\xBF""cab\xDF\xBF").Cut(8, 1);
	Test(oStr.ByteLength() == 10);
	Test(oStr.Length() == 8);
	Test(StringCompare(oStr.Utf8(), "\xDF\xBF""bca\xDF\xBF""cab") == 0);
	
	oStr.Set("\xDF\xBF""bca\xDF\xBF""cab\xDF\xBF").Cut(9, 1);
	Test(oStr.ByteLength() == 12);
	Test(oStr.Length() == 9);
	Test(StringCompare(oStr.Utf8(), "\xDF\xBF""bca\xDF\xBF""cab\xDF\xBF") == 0);
	
	
	// 3-byte seqence
	
	oStr.Set("\xEF\xBF\xBF""bca\xEF\xBF\xBF""cab\xEF\xBF\xBF").Cut(0, 0);
	Test(oStr.ByteLength() == 15);
	Test(oStr.Length() == 9);
	Test(StringCompare(oStr.Utf8(), "\xEF\xBF\xBF""bca\xEF\xBF\xBF""cab\xEF\xBF\xBF") == 0);
	
	oStr.Set("\xEF\xBF\xBF""bca\xEF\xBF\xBF""cab\xEF\xBF\xBF").Cut(0, 3);
	Test(oStr.ByteLength() == 10);
	Test(oStr.Length() == 6);
	Test(StringCompare(oStr.Utf8(), "a\xEF\xBF\xBF""cab\xEF\xBF\xBF") == 0);
	
	oStr.Set("\xEF\xBF\xBF""bca\xEF\xBF\xBF""cab\xEF\xBF\xBF").Cut(1, 5);
	Test(oStr.ByteLength() == 8);
	Test(oStr.Length() == 4);
	Test(StringCompare(oStr.Utf8(), "\xEF\xBF\xBF""ab\xEF\xBF\xBF") == 0);
	
	oStr.Set("\xEF\xBF\xBF""bca\xEF\xBF\xBF""cab\xEF\xBF\xBF").Cut(5, 4);
	Test(oStr.ByteLength() == 9);
	Test(oStr.Length() == 5);
	Test(StringCompare(oStr.Utf8(), "\xEF\xBF\xBF""bca\xEF\xBF\xBF") == 0);
	
	oStr.Set("\xEF\xBF\xBF""bca\xEF\xBF\xBF""cab\xEF\xBF\xBF").Cut(5, 5);
	Test(oStr.ByteLength() == 9);
	Test(oStr.Length() == 5);
	Test(StringCompare(oStr.Utf8(), "\xEF\xBF\xBF""bca\xEF\xBF\xBF") == 0);
	
	oStr.Set("\xEF\xBF\xBF""bca\xEF\xBF\xBF""cab\xEF\xBF\xBF").Cut(8, 1);
	Test(oStr.ByteLength() == 12);
	Test(oStr.Length() == 8);
	Test(StringCompare(oStr.Utf8(), "\xEF\xBF\xBF""bca\xEF\xBF\xBF""cab") == 0);
	
	oStr.Set("\xEF\xBF\xBF""bca\xEF\xBF\xBF""cab\xEF\xBF\xBF").Cut(9, 1);
	Test(oStr.ByteLength() == 15);
	Test(oStr.Length() == 9);
	Test(StringCompare(oStr.Utf8(), "\xEF\xBF\xBF""bca\xEF\xBF\xBF""cab\xEF\xBF\xBF") == 0);
	
	
	// 4-byte seqence
	
	oStr.Set("\xF7\xBF\xBF\xBF""bca\xF7\xBF\xBF\xBF""cab\xF7\xBF\xBF\xBF").Cut(0, 0);
	Test(oStr.ByteLength() == 18);
	Test(oStr.Length() == 9);
	Test(StringCompare(oStr.Utf8(), "\xF7\xBF\xBF\xBF""bca\xF7\xBF\xBF\xBF""cab\xF7\xBF\xBF\xBF") == 0);
	
	oStr.Set("\xF7\xBF\xBF\xBF""bca\xF7\xBF\xBF\xBF""cab\xF7\xBF\xBF\xBF").Cut(0, 3);
	Test(oStr.ByteLength() == 12);
	Test(oStr.Length() == 6);
	Test(StringCompare(oStr.Utf8(), "a\xF7\xBF\xBF\xBF""cab\xF7\xBF\xBF\xBF") == 0);
	
	oStr.Set("\xF7\xBF\xBF\xBF""bca\xF7\xBF\xBF\xBF""cab\xF7\xBF\xBF\xBF").Cut(1, 5);
	Test(oStr.ByteLength() == 10);
	Test(oStr.Length() == 4);
	Test(StringCompare(oStr.Utf8(), "\xF7\xBF\xBF\xBF""ab\xF7\xBF\xBF\xBF") == 0);
	
	oStr.Set("\xF7\xBF\xBF\xBF""bca\xF7\xBF\xBF\xBF""cab\xF7\xBF\xBF\xBF").Cut(5, 4);
	Test(oStr.ByteLength() == 11);
	Test(oStr.Length() == 5);
	Test(StringCompare(oStr.Utf8(), "\xF7\xBF\xBF\xBF""bca\xF7\xBF\xBF\xBF") == 0);
	
	oStr.Set("\xF7\xBF\xBF\xBF""bca\xF7\xBF\xBF\xBF""cab\xF7\xBF\xBF\xBF").Cut(5, 5);
	Test(oStr.ByteLength() == 11);
	Test(oStr.Length() == 5);
	Test(StringCompare(oStr.Utf8(), "\xF7\xBF\xBF\xBF""bca\xF7\xBF\xBF\xBF") == 0);
	
	oStr.Set("\xF7\xBF\xBF\xBF""bca\xF7\xBF\xBF\xBF""cab\xF7\xBF\xBF\xBF").Cut(8, 1);
	Test(oStr.ByteLength() == 14);
	Test(oStr.Length() == 8);
	Test(StringCompare(oStr.Utf8(), "\xF7\xBF\xBF\xBF""bca\xF7\xBF\xBF\xBF""cab") == 0);
	
	oStr.Set("\xF7\xBF\xBF\xBF""bca\xF7\xBF\xBF\xBF""cab\xF7\xBF\xBF\xBF").Cut(9, 1);
	Test(oStr.ByteLength() == 18);
	Test(oStr.Length() == 9);
	Test(StringCompare(oStr.Utf8(), "\xF7\xBF\xBF\xBF""bca\xF7\xBF\xBF\xBF""cab\xF7\xBF\xBF\xBF") == 0);
	
	
	// 5-byte seqence
	
	oStr.Set("\xFB\xBF\xBF\xBF\xBF""bca\xFB\xBF\xBF\xBF\xBF""cab\xFB\xBF\xBF\xBF\xBF").Cut(0, 0);
	Test(oStr.ByteLength() == 21);
	Test(oStr.Length() == 9);
	Test(StringCompare(oStr.Utf8(), "\xFB\xBF\xBF\xBF\xBF""bca\xFB\xBF\xBF\xBF\xBF""cab\xFB\xBF\xBF\xBF\xBF") == 0);
	
	oStr.Set("\xFB\xBF\xBF\xBF\xBF""bca\xFB\xBF\xBF\xBF\xBF""cab\xFB\xBF\xBF\xBF\xBF").Cut(0, 3);
	Test(oStr.ByteLength() == 14);
	Test(oStr.Length() == 6);
	Test(StringCompare(oStr.Utf8(), "a\xFB\xBF\xBF\xBF\xBF""cab\xFB\xBF\xBF\xBF\xBF") == 0);
	
	oStr.Set("\xFB\xBF\xBF\xBF\xBF""bca\xFB\xBF\xBF\xBF\xBF""cab\xFB\xBF\xBF\xBF\xBF").Cut(1, 5);
	Test(oStr.ByteLength() == 12);
	Test(oStr.Length() == 4);
	Test(StringCompare(oStr.Utf8(), "\xFB\xBF\xBF\xBF\xBF""ab\xFB\xBF\xBF\xBF\xBF") == 0);
	
	oStr.Set("\xFB\xBF\xBF\xBF\xBF""bca\xFB\xBF\xBF\xBF\xBF""cab\xFB\xBF\xBF\xBF\xBF").Cut(5, 4);
	Test(oStr.ByteLength() == 13);
	Test(oStr.Length() == 5);
	Test(StringCompare(oStr.Utf8(), "\xFB\xBF\xBF\xBF\xBF""bca\xFB\xBF\xBF\xBF\xBF") == 0);
	
	oStr.Set("\xFB\xBF\xBF\xBF\xBF""bca\xFB\xBF\xBF\xBF\xBF""cab\xFB\xBF\xBF\xBF\xBF").Cut(5, 5);
	Test(oStr.ByteLength() == 13);
	Test(oStr.Length() == 5);
	Test(StringCompare(oStr.Utf8(), "\xFB\xBF\xBF\xBF\xBF""bca\xFB\xBF\xBF\xBF\xBF") == 0);
	
	oStr.Set("\xFB\xBF\xBF\xBF\xBF""bca\xFB\xBF\xBF\xBF\xBF""cab\xFB\xBF\xBF\xBF\xBF").Cut(8, 1);
	Test(oStr.ByteLength() == 16);
	Test(oStr.Length() == 8);
	Test(StringCompare(oStr.Utf8(), "\xFB\xBF\xBF\xBF\xBF""bca\xFB\xBF\xBF\xBF\xBF""cab") == 0);
	
	oStr.Set("\xFB\xBF\xBF\xBF\xBF""bca\xFB\xBF\xBF\xBF\xBF""cab\xFB\xBF\xBF\xBF\xBF").Cut(9, 1);
	Test(oStr.ByteLength() == 21);
	Test(oStr.Length() == 9);
	Test(StringCompare(oStr.Utf8(), "\xFB\xBF\xBF\xBF\xBF""bca\xFB\xBF\xBF\xBF\xBF""cab\xFB\xBF\xBF\xBF\xBF") == 0);
	
	
	// 6-byte seqence
	
	oStr.Set("\xFD\xBF\xBF\xBF\xBF\xBF""bca\xFD\xBF\xBF\xBF\xBF\xBF""cab\xFD\xBF\xBF\xBF\xBF\xBF").Cut(0, 0);
	Test(oStr.ByteLength() == 24);
	Test(oStr.Length() == 9);
	Test(StringCompare(oStr.Utf8(), "\xFD\xBF\xBF\xBF\xBF\xBF""bca\xFD\xBF\xBF\xBF\xBF\xBF""cab\xFD\xBF\xBF\xBF\xBF\xBF") == 0);
	
	oStr.Set("\xFD\xBF\xBF\xBF\xBF\xBF""bca\xFD\xBF\xBF\xBF\xBF\xBF""cab\xFD\xBF\xBF\xBF\xBF\xBF").Cut(0, 3);
	Test(oStr.ByteLength() == 16);
	Test(oStr.Length() == 6);
	Test(StringCompare(oStr.Utf8(), "a\xFD\xBF\xBF\xBF\xBF\xBF""cab\xFD\xBF\xBF\xBF\xBF\xBF") == 0);
	
	oStr.Set("\xFD\xBF\xBF\xBF\xBF\xBF""bca\xFD\xBF\xBF\xBF\xBF\xBF""cab\xFD\xBF\xBF\xBF\xBF\xBF").Cut(1, 5);
	Test(oStr.ByteLength() == 14);
	Test(oStr.Length() == 4);
	Test(StringCompare(oStr.Utf8(), "\xFD\xBF\xBF\xBF\xBF\xBF""ab\xFD\xBF\xBF\xBF\xBF\xBF") == 0);
	
	oStr.Set("\xFD\xBF\xBF\xBF\xBF\xBF""bca\xFD\xBF\xBF\xBF\xBF\xBF""cab\xFD\xBF\xBF\xBF\xBF\xBF").Cut(5, 4);
	Test(oStr.ByteLength() == 15);
	Test(oStr.Length() == 5);
	Test(StringCompare(oStr.Utf8(), "\xFD\xBF\xBF\xBF\xBF\xBF""bca\xFD\xBF\xBF\xBF\xBF\xBF") == 0);
	
	oStr.Set("\xFD\xBF\xBF\xBF\xBF\xBF""bca\xFD\xBF\xBF\xBF\xBF\xBF""cab\xFD\xBF\xBF\xBF\xBF\xBF").Cut(5, 5);
	Test(oStr.ByteLength() == 15);
	Test(oStr.Length() == 5);
	Test(StringCompare(oStr.Utf8(), "\xFD\xBF\xBF\xBF\xBF\xBF""bca\xFD\xBF\xBF\xBF\xBF\xBF") == 0);
	
	oStr.Set("\xFD\xBF\xBF\xBF\xBF\xBF""bca\xFD\xBF\xBF\xBF\xBF\xBF""cab\xFD\xBF\xBF\xBF\xBF\xBF").Cut(8, 1);
	Test(oStr.ByteLength() == 18);
	Test(oStr.Length() == 8);
	Test(StringCompare(oStr.Utf8(), "\xFD\xBF\xBF\xBF\xBF\xBF""bca\xFD\xBF\xBF\xBF\xBF\xBF""cab") == 0);
	
	oStr.Set("\xFD\xBF\xBF\xBF\xBF\xBF""bca\xFD\xBF\xBF\xBF\xBF\xBF""cab\xFD\xBF\xBF\xBF\xBF\xBF").Cut(9, 1);
	Test(oStr.ByteLength() == 24);
	Test(oStr.Length() == 9);
	Test(StringCompare(oStr.Utf8(), "\xFD\xBF\xBF\xBF\xBF\xBF""bca\xFD\xBF\xBF\xBF\xBF\xBF""cab\xFD\xBF\xBF\xBF\xBF\xBF") == 0);
	
	
	return 0;
}

static int TestCutPosCount()
{
	PrintFn();
	
	
	acpl::String oStr;
	acpl::String::Pos oPos(oStr);
	
	
	// 1-byte seqence
	
	oStr.Set("abcabcabc");
	oPos.First();
	oStr.Cut(oPos, true, 0);
	Test(oStr.ByteLength() == 9);
	Test(oStr.Length() == 9);
	Test(StringCompare(oStr.Utf8(), "abcabcabc") == 0);
	
	oStr.Set("abcabcabc");
	oStr.Cut(oPos, true, 3);
	Test(oStr.ByteLength() == 6);
	Test(oStr.Length() == 6);
	Test(StringCompare(oStr.Utf8(), "abcabc") == 0);
	
	oStr.Set("abcabcabc");
	oStr.Cut(oPos, false, 5);
	Test(oStr.ByteLength() == 4);
	Test(oStr.Length() == 4);
	Test(StringCompare(oStr.Utf8(), "aabc") == 0);
	
	oStr.Set("abcabcabc");
	oPos.SkipFirst(4);
	oStr.Cut(oPos, false, 4);
	Test(oStr.ByteLength() == 5);
	Test(oStr.Length() == 5);
	Test(StringCompare(oStr.Utf8(), "abcab") == 0);
	
	oStr.Set("abcabcabc");
	oPos.Next();
	oStr.Cut(oPos, true, 5);
	Test(oStr.ByteLength() == 5);
	Test(oStr.Length() == 5);
	Test(StringCompare(oStr.Utf8(), "abcab") == 0);
	
	oStr.Set("abcabcabc");
	oPos.Last();
	oStr.Cut(oPos, true, 1);
	Test(oStr.ByteLength() == 8);
	Test(oStr.Length() == 8);
	Test(StringCompare(oStr.Utf8(), "abcabcab") == 0);
	
	oStr.Set("abcabcabc");
	oStr.Cut(oPos, false, 1);
	Test(oStr.ByteLength() == 9);
	Test(oStr.Length() == 9);
	Test(StringCompare(oStr.Utf8(), "abcabcabc") == 0);
	
	
	// 2-byte seqence
	
	oStr.Set("\xDF\xBF""bca\xDF\xBF""cab\xDF\xBF");
	oPos.First();
	oStr.Cut(oPos, true, 0);
	Test(oStr.ByteLength() == 12);
	Test(oStr.Length() == 9);
	Test(StringCompare(oStr.Utf8(), "\xDF\xBF""bca\xDF\xBF""cab\xDF\xBF") == 0);
	
	oStr.Set("\xDF\xBF""bca\xDF\xBF""cab\xDF\xBF");
	oStr.Cut(oPos, true, 3);
	Test(oStr.ByteLength() == 8);
	Test(oStr.Length() == 6);
	Test(StringCompare(oStr.Utf8(), "a\xDF\xBF""cab\xDF\xBF") == 0);
	
	oStr.Set("\xDF\xBF""bca\xDF\xBF""cab\xDF\xBF");
	oStr.Cut(oPos, false, 5);
	Test(oStr.ByteLength() == 6);
	Test(oStr.Length() == 4);
	Test(StringCompare(oStr.Utf8(), "\xDF\xBF""ab\xDF\xBF") == 0);
	
	oStr.Set("\xDF\xBF""bca\xDF\xBF""cab\xDF\xBF");
	oPos.SkipFirst(4);
	oStr.Cut(oPos, false, 4);
	Test(oStr.ByteLength() == 7);
	Test(oStr.Length() == 5);
	Test(StringCompare(oStr.Utf8(), "\xDF\xBF""bca\xDF\xBF") == 0);
	
	oStr.Set("\xDF\xBF""bca\xDF\xBF""cab\xDF\xBF");
	oPos.Next();
	oStr.Cut(oPos, true, 5);
	Test(oStr.ByteLength() == 7);
	Test(oStr.Length() == 5);
	Test(StringCompare(oStr.Utf8(), "\xDF\xBF""bca\xDF\xBF") == 0);
	
	oStr.Set("\xDF\xBF""bca\xDF\xBF""cab\xDF\xBF");
	oPos.Last();
	oStr.Cut(oPos, true, 1);
	Test(oStr.ByteLength() == 10);
	Test(oStr.Length() == 8);
	Test(StringCompare(oStr.Utf8(), "\xDF\xBF""bca\xDF\xBF""cab") == 0);
	
	oStr.Set("\xDF\xBF""bca\xDF\xBF""cab\xDF\xBF");
	oStr.Cut(oPos, false, 1);
	Test(oStr.ByteLength() == 12);
	Test(oStr.Length() == 9);
	Test(StringCompare(oStr.Utf8(), "\xDF\xBF""bca\xDF\xBF""cab\xDF\xBF") == 0);
	
	
	// 3-byte seqence
	
	oStr.Set("\xEF\xBF\xBF""bca\xEF\xBF\xBF""cab\xEF\xBF\xBF");
	oPos.First();
	oStr.Cut(oPos, true, 0);
	Test(oStr.ByteLength() == 15);
	Test(oStr.Length() == 9);
	Test(StringCompare(oStr.Utf8(), "\xEF\xBF\xBF""bca\xEF\xBF\xBF""cab\xEF\xBF\xBF") == 0);
	
	oStr.Set("\xEF\xBF\xBF""bca\xEF\xBF\xBF""cab\xEF\xBF\xBF");
	oStr.Cut(oPos, true, 3);
	Test(oStr.ByteLength() == 10);
	Test(oStr.Length() == 6);
	Test(StringCompare(oStr.Utf8(), "a\xEF\xBF\xBF""cab\xEF\xBF\xBF") == 0);
	
	oStr.Set("\xEF\xBF\xBF""bca\xEF\xBF\xBF""cab\xEF\xBF\xBF");
	oStr.Cut(oPos, false, 5);
	Test(oStr.ByteLength() == 8);
	Test(oStr.Length() == 4);
	Test(StringCompare(oStr.Utf8(), "\xEF\xBF\xBF""ab\xEF\xBF\xBF") == 0);
	
	oStr.Set("\xEF\xBF\xBF""bca\xEF\xBF\xBF""cab\xEF\xBF\xBF");
	oPos.SkipFirst(4);
	oStr.Cut(oPos, false, 4);
	Test(oStr.ByteLength() == 9);
	Test(oStr.Length() == 5);
	Test(StringCompare(oStr.Utf8(), "\xEF\xBF\xBF""bca\xEF\xBF\xBF") == 0);
	
	oStr.Set("\xEF\xBF\xBF""bca\xEF\xBF\xBF""cab\xEF\xBF\xBF");
	oPos.Next();
	oStr.Cut(oPos, true, 5);
	Test(oStr.ByteLength() == 9);
	Test(oStr.Length() == 5);
	Test(StringCompare(oStr.Utf8(), "\xEF\xBF\xBF""bca\xEF\xBF\xBF") == 0);
	
	oStr.Set("\xEF\xBF\xBF""bca\xEF\xBF\xBF""cab\xEF\xBF\xBF");
	oPos.Last();
	oStr.Cut(oPos, true, 1);
	Test(oStr.ByteLength() == 12);
	Test(oStr.Length() == 8);
	Test(StringCompare(oStr.Utf8(), "\xEF\xBF\xBF""bca\xEF\xBF\xBF""cab") == 0);
	
	oStr.Set("\xEF\xBF\xBF""bca\xEF\xBF\xBF""cab\xEF\xBF\xBF");
	oStr.Cut(oPos, false, 1);
	Test(oStr.ByteLength() == 15);
	Test(oStr.Length() == 9);
	Test(StringCompare(oStr.Utf8(), "\xEF\xBF\xBF""bca\xEF\xBF\xBF""cab\xEF\xBF\xBF") == 0);
	
	
	// 4-byte seqence
	
	oStr.Set("\xF7\xBF\xBF\xBF""bca\xF7\xBF\xBF\xBF""cab\xF7\xBF\xBF\xBF");
	oPos.First();
	oStr.Cut(oPos, true, 0);
	Test(oStr.ByteLength() == 18);
	Test(oStr.Length() == 9);
	Test(StringCompare(oStr.Utf8(), "\xF7\xBF\xBF\xBF""bca\xF7\xBF\xBF\xBF""cab\xF7\xBF\xBF\xBF") == 0);
	
	oStr.Set("\xF7\xBF\xBF\xBF""bca\xF7\xBF\xBF\xBF""cab\xF7\xBF\xBF\xBF");
	oStr.Cut(oPos, true, 3);
	Test(oStr.ByteLength() == 12);
	Test(oStr.Length() == 6);
	Test(StringCompare(oStr.Utf8(), "a\xF7\xBF\xBF\xBF""cab\xF7\xBF\xBF\xBF") == 0);
	
	oStr.Set("\xF7\xBF\xBF\xBF""bca\xF7\xBF\xBF\xBF""cab\xF7\xBF\xBF\xBF");
	oStr.Cut(oPos, false, 5);
	Test(oStr.ByteLength() == 10);
	Test(oStr.Length() == 4);
	Test(StringCompare(oStr.Utf8(), "\xF7\xBF\xBF\xBF""ab\xF7\xBF\xBF\xBF") == 0);
	
	oStr.Set("\xF7\xBF\xBF\xBF""bca\xF7\xBF\xBF\xBF""cab\xF7\xBF\xBF\xBF");
	oPos.SkipFirst(4);
	oStr.Cut(oPos, false, 4);
	Test(oStr.ByteLength() == 11);
	Test(oStr.Length() == 5);
	Test(StringCompare(oStr.Utf8(), "\xF7\xBF\xBF\xBF""bca\xF7\xBF\xBF\xBF") == 0);
	
	oStr.Set("\xF7\xBF\xBF\xBF""bca\xF7\xBF\xBF\xBF""cab\xF7\xBF\xBF\xBF");
	oPos.Next();
	oStr.Cut(oPos, true, 5);
	Test(oStr.ByteLength() == 11);
	Test(oStr.Length() == 5);
	Test(StringCompare(oStr.Utf8(), "\xF7\xBF\xBF\xBF""bca\xF7\xBF\xBF\xBF") == 0);
	
	oStr.Set("\xF7\xBF\xBF\xBF""bca\xF7\xBF\xBF\xBF""cab\xF7\xBF\xBF\xBF");
	oPos.Last();
	oStr.Cut(oPos, true, 1);
	Test(oStr.ByteLength() == 14);
	Test(oStr.Length() == 8);
	Test(StringCompare(oStr.Utf8(), "\xF7\xBF\xBF\xBF""bca\xF7\xBF\xBF\xBF""cab") == 0);
	
	oStr.Set("\xF7\xBF\xBF\xBF""bca\xF7\xBF\xBF\xBF""cab\xF7\xBF\xBF\xBF");
	oStr.Cut(oPos, false, 1);
	Test(oStr.ByteLength() == 18);
	Test(oStr.Length() == 9);
	Test(StringCompare(oStr.Utf8(), "\xF7\xBF\xBF\xBF""bca\xF7\xBF\xBF\xBF""cab\xF7\xBF\xBF\xBF") == 0);
	
	
	// 5-byte seqence
	
	oStr.Set("\xFB\xBF\xBF\xBF\xBF""bca\xFB\xBF\xBF\xBF\xBF""cab\xFB\xBF\xBF\xBF\xBF");
	oPos.First();
	oStr.Cut(oPos, true, 0);
	Test(oStr.ByteLength() == 21);
	Test(oStr.Length() == 9);
	Test(StringCompare(oStr.Utf8(), "\xFB\xBF\xBF\xBF\xBF""bca\xFB\xBF\xBF\xBF\xBF""cab\xFB\xBF\xBF\xBF\xBF") == 0);
	
	oStr.Set("\xFB\xBF\xBF\xBF\xBF""bca\xFB\xBF\xBF\xBF\xBF""cab\xFB\xBF\xBF\xBF\xBF");
	oStr.Cut(oPos, true, 3);
	Test(oStr.ByteLength() == 14);
	Test(oStr.Length() == 6);
	Test(StringCompare(oStr.Utf8(), "a\xFB\xBF\xBF\xBF\xBF""cab\xFB\xBF\xBF\xBF\xBF") == 0);
	
	oStr.Set("\xFB\xBF\xBF\xBF\xBF""bca\xFB\xBF\xBF\xBF\xBF""cab\xFB\xBF\xBF\xBF\xBF");
	oStr.Cut(oPos, false, 5);
	Test(oStr.ByteLength() == 12);
	Test(oStr.Length() == 4);
	Test(StringCompare(oStr.Utf8(), "\xFB\xBF\xBF\xBF\xBF""ab\xFB\xBF\xBF\xBF\xBF") == 0);
	
	oStr.Set("\xFB\xBF\xBF\xBF\xBF""bca\xFB\xBF\xBF\xBF\xBF""cab\xFB\xBF\xBF\xBF\xBF");
	oPos.SkipFirst(4);
	oStr.Cut(oPos, false, 4);
	Test(oStr.ByteLength() == 13);
	Test(oStr.Length() == 5);
	Test(StringCompare(oStr.Utf8(), "\xFB\xBF\xBF\xBF\xBF""bca\xFB\xBF\xBF\xBF\xBF") == 0);
	
	oStr.Set("\xFB\xBF\xBF\xBF\xBF""bca\xFB\xBF\xBF\xBF\xBF""cab\xFB\xBF\xBF\xBF\xBF");
	oPos.Next();
	oStr.Cut(oPos, true, 5);
	Test(oStr.ByteLength() == 13);
	Test(oStr.Length() == 5);
	Test(StringCompare(oStr.Utf8(), "\xFB\xBF\xBF\xBF\xBF""bca\xFB\xBF\xBF\xBF\xBF") == 0);
	
	oStr.Set("\xFB\xBF\xBF\xBF\xBF""bca\xFB\xBF\xBF\xBF\xBF""cab\xFB\xBF\xBF\xBF\xBF");
	oPos.Last();
	oStr.Cut(oPos, true, 1);
	Test(oStr.ByteLength() == 16);
	Test(oStr.Length() == 8);
	Test(StringCompare(oStr.Utf8(), "\xFB\xBF\xBF\xBF\xBF""bca\xFB\xBF\xBF\xBF\xBF""cab") == 0);
	
	oStr.Set("\xFB\xBF\xBF\xBF\xBF""bca\xFB\xBF\xBF\xBF\xBF""cab\xFB\xBF\xBF\xBF\xBF");
	oStr.Cut(oPos, false, 1);
	Test(oStr.ByteLength() == 21);
	Test(oStr.Length() == 9);
	Test(StringCompare(oStr.Utf8(), "\xFB\xBF\xBF\xBF\xBF""bca\xFB\xBF\xBF\xBF\xBF""cab\xFB\xBF\xBF\xBF\xBF") == 0);
	
	
	// 6-byte seqence
	
	oStr.Set("\xFD\xBF\xBF\xBF\xBF\xBF""bca\xFD\xBF\xBF\xBF\xBF\xBF""cab\xFD\xBF\xBF\xBF\xBF\xBF");
	oPos.First();
	oStr.Cut(oPos, true, 0);
	Test(oStr.ByteLength() == 24);
	Test(oStr.Length() == 9);
	Test(StringCompare(oStr.Utf8(), "\xFD\xBF\xBF\xBF\xBF\xBF""bca\xFD\xBF\xBF\xBF\xBF\xBF""cab\xFD\xBF\xBF\xBF\xBF\xBF") == 0);
	
	oStr.Set("\xFD\xBF\xBF\xBF\xBF\xBF""bca\xFD\xBF\xBF\xBF\xBF\xBF""cab\xFD\xBF\xBF\xBF\xBF\xBF");
	oStr.Cut(oPos, true, 3);
	Test(oStr.ByteLength() == 16);
	Test(oStr.Length() == 6);
	Test(StringCompare(oStr.Utf8(), "a\xFD\xBF\xBF\xBF\xBF\xBF""cab\xFD\xBF\xBF\xBF\xBF\xBF") == 0);
	
	oStr.Set("\xFD\xBF\xBF\xBF\xBF\xBF""bca\xFD\xBF\xBF\xBF\xBF\xBF""cab\xFD\xBF\xBF\xBF\xBF\xBF");
	oStr.Cut(oPos, false, 5);
	Test(oStr.ByteLength() == 14);
	Test(oStr.Length() == 4);
	Test(StringCompare(oStr.Utf8(), "\xFD\xBF\xBF\xBF\xBF\xBF""ab\xFD\xBF\xBF\xBF\xBF\xBF") == 0);
	
	oStr.Set("\xFD\xBF\xBF\xBF\xBF\xBF""bca\xFD\xBF\xBF\xBF\xBF\xBF""cab\xFD\xBF\xBF\xBF\xBF\xBF");
	oPos.SkipFirst(4);
	oStr.Cut(oPos, false, 4);
	Test(oStr.ByteLength() == 15);
	Test(oStr.Length() == 5);
	Test(StringCompare(oStr.Utf8(), "\xFD\xBF\xBF\xBF\xBF\xBF""bca\xFD\xBF\xBF\xBF\xBF\xBF") == 0);
	
	oStr.Set("\xFD\xBF\xBF\xBF\xBF\xBF""bca\xFD\xBF\xBF\xBF\xBF\xBF""cab\xFD\xBF\xBF\xBF\xBF\xBF");
	oPos.Next();
	oStr.Cut(oPos, true, 5);
	Test(oStr.ByteLength() == 15);
	Test(oStr.Length() == 5);
	Test(StringCompare(oStr.Utf8(), "\xFD\xBF\xBF\xBF\xBF\xBF""bca\xFD\xBF\xBF\xBF\xBF\xBF") == 0);
	
	oStr.Set("\xFD\xBF\xBF\xBF\xBF\xBF""bca\xFD\xBF\xBF\xBF\xBF\xBF""cab\xFD\xBF\xBF\xBF\xBF\xBF");
	oPos.Last();
	oStr.Cut(oPos, true, 1);
	Test(oStr.ByteLength() == 18);
	Test(oStr.Length() == 8);
	Test(StringCompare(oStr.Utf8(), "\xFD\xBF\xBF\xBF\xBF\xBF""bca\xFD\xBF\xBF\xBF\xBF\xBF""cab") == 0);
	
	oStr.Set("\xFD\xBF\xBF\xBF\xBF\xBF""bca\xFD\xBF\xBF\xBF\xBF\xBF""cab\xFD\xBF\xBF\xBF\xBF\xBF");
	oStr.Cut(oPos, false, 1);
	Test(oStr.ByteLength() == 24);
	Test(oStr.Length() == 9);
	Test(StringCompare(oStr.Utf8(), "\xFD\xBF\xBF\xBF\xBF\xBF""bca\xFD\xBF\xBF\xBF\xBF\xBF""cab\xFD\xBF\xBF\xBF\xBF\xBF") == 0);
	
	
	return 0;
}

static int TestCutPosPos()
{
	PrintFn();
	
	
	acpl::String oStr;
	acpl::String::Pos oFromPos(oStr);
	acpl::String::Pos oToPos(oStr);
	
	
	// 1-byte seqence
	
	oStr.Set("abcabcabc");
	oFromPos.First();
	oToPos.First();
	oStr.Cut(oFromPos, true, oToPos, true);
	Test(oStr.ByteLength() == 9);
	Test(oStr.Length() == 9);
	Test(StringCompare(oStr.Utf8(), "abcabcabc") == 0);
	
	oStr.Set("abcabcabc");
	oToPos.Forth(2);
	oStr.Cut(oFromPos, true, oToPos, false);
	Test(oStr.ByteLength() == 6);
	Test(oStr.Length() == 6);
	Test(StringCompare(oStr.Utf8(), "abcabc") == 0);
	
	oStr.Set("abcabcabc");
	oToPos.Forth(4);
	oStr.Cut(oFromPos, false, oToPos, true);
	Test(oStr.ByteLength() == 4);
	Test(oStr.Length() == 4);
	Test(StringCompare(oStr.Utf8(), "aabc") == 0);
	
	oStr.Set("abcabcabc");
	oFromPos.SkipFirst(4);
	oToPos.Last();
	oStr.Cut(oFromPos, false, oToPos, false);
	Test(oStr.ByteLength() == 5);
	Test(oStr.Length() == 5);
	Test(StringCompare(oStr.Utf8(), "abcab") == 0);
	
	oStr.Set("abcabcabc");
	oFromPos.Next();
	oStr.Cut(oFromPos, true, oToPos, false);
	Test(oStr.ByteLength() == 5);
	Test(oStr.Length() == 5);
	Test(StringCompare(oStr.Utf8(), "abcab") == 0);
	
	oStr.Set("abcabcabc");
	oFromPos.Last();
	oStr.Cut(oFromPos, true, oToPos, false);
	Test(oStr.ByteLength() == 8);
	Test(oStr.Length() == 8);
	Test(StringCompare(oStr.Utf8(), "abcabcab") == 0);
	
	oStr.Set("abcabcabc");
	oStr.Cut(oFromPos, false, oToPos, false);
	Test(oStr.ByteLength() == 9);
	Test(oStr.Length() == 9);
	Test(StringCompare(oStr.Utf8(), "abcabcabc") == 0);
	
	
	// 2-byte seqence
	
	oStr.Set("\xDF\xBF""bca\xDF\xBF""cab\xDF\xBF");
	oFromPos.First();
	oToPos.First();
	oStr.Cut(oFromPos, true, oToPos, true);
	Test(oStr.ByteLength() == 12);
	Test(oStr.Length() == 9);
	Test(StringCompare(oStr.Utf8(), "\xDF\xBF""bca\xDF\xBF""cab\xDF\xBF") == 0);
	
	oStr.Set("\xDF\xBF""bca\xDF\xBF""cab\xDF\xBF");
	oToPos.Forth(2);
	oStr.Cut(oFromPos, true, oToPos, false);
	Test(oStr.ByteLength() == 8);
	Test(oStr.Length() == 6);
	Test(StringCompare(oStr.Utf8(), "a\xDF\xBF""cab\xDF\xBF") == 0);
	
	oStr.Set("\xDF\xBF""bca\xDF\xBF""cab\xDF\xBF");
	oToPos.Forth(4);
	oStr.Cut(oFromPos, false, oToPos, true);
	Test(oStr.ByteLength() == 6);
	Test(oStr.Length() == 4);
	Test(StringCompare(oStr.Utf8(), "\xDF\xBF""ab\xDF\xBF") == 0);
	
	oStr.Set("\xDF\xBF""bca\xDF\xBF""cab\xDF\xBF");
	oFromPos.SkipFirst(4);
	oToPos.Last();
	oStr.Cut(oFromPos, false, oToPos, false);
	Test(oStr.ByteLength() == 7);
	Test(oStr.Length() == 5);
	Test(StringCompare(oStr.Utf8(), "\xDF\xBF""bca\xDF\xBF") == 0);
	
	oStr.Set("\xDF\xBF""bca\xDF\xBF""cab\xDF\xBF");
	oFromPos.Next();
	oStr.Cut(oFromPos, true, oToPos, false);
	Test(oStr.ByteLength() == 7);
	Test(oStr.Length() == 5);
	Test(StringCompare(oStr.Utf8(), "\xDF\xBF""bca\xDF\xBF") == 0);
	
	oStr.Set("\xDF\xBF""bca\xDF\xBF""cab\xDF\xBF");
	oFromPos.Last();
	oStr.Cut(oFromPos, true, oToPos, false);
	Test(oStr.ByteLength() == 10);
	Test(oStr.Length() == 8);
	Test(StringCompare(oStr.Utf8(), "\xDF\xBF""bca\xDF\xBF""cab") == 0);
	
	oStr.Set("\xDF\xBF""bca\xDF\xBF""cab\xDF\xBF");
	oStr.Cut(oFromPos, false, oToPos, false);
	Test(oStr.ByteLength() == 12);
	Test(oStr.Length() == 9);
	Test(StringCompare(oStr.Utf8(), "\xDF\xBF""bca\xDF\xBF""cab\xDF\xBF") == 0);
	
	
	// 3-byte seqence
	
	oStr.Set("\xEF\xBF\xBF""bca\xEF\xBF\xBF""cab\xEF\xBF\xBF");
	oFromPos.First();
	oToPos.First();
	oStr.Cut(oFromPos, true, oToPos, true);
	Test(oStr.ByteLength() == 15);
	Test(oStr.Length() == 9);
	Test(StringCompare(oStr.Utf8(), "\xEF\xBF\xBF""bca\xEF\xBF\xBF""cab\xEF\xBF\xBF") == 0);
	
	oStr.Set("\xEF\xBF\xBF""bca\xEF\xBF\xBF""cab\xEF\xBF\xBF");
	oToPos.Forth(2);
	oStr.Cut(oFromPos, true, oToPos, false);
	Test(oStr.ByteLength() == 10);
	Test(oStr.Length() == 6);
	Test(StringCompare(oStr.Utf8(), "a\xEF\xBF\xBF""cab\xEF\xBF\xBF") == 0);
	
	oStr.Set("\xEF\xBF\xBF""bca\xEF\xBF\xBF""cab\xEF\xBF\xBF");
	oToPos.Forth(4);
	oStr.Cut(oFromPos, false, oToPos, true);
	Test(oStr.ByteLength() == 8);
	Test(oStr.Length() == 4);
	Test(StringCompare(oStr.Utf8(), "\xEF\xBF\xBF""ab\xEF\xBF\xBF") == 0);
	
	oStr.Set("\xEF\xBF\xBF""bca\xEF\xBF\xBF""cab\xEF\xBF\xBF");
	oFromPos.SkipFirst(4);
	oToPos.Last();
	oStr.Cut(oFromPos, false, oToPos, false);
	Test(oStr.ByteLength() == 9);
	Test(oStr.Length() == 5);
	Test(StringCompare(oStr.Utf8(), "\xEF\xBF\xBF""bca\xEF\xBF\xBF") == 0);
	
	oStr.Set("\xEF\xBF\xBF""bca\xEF\xBF\xBF""cab\xEF\xBF\xBF");
	oFromPos.Next();
	oStr.Cut(oFromPos, true, oToPos, false);
	Test(oStr.ByteLength() == 9);
	Test(oStr.Length() == 5);
	Test(StringCompare(oStr.Utf8(), "\xEF\xBF\xBF""bca\xEF\xBF\xBF") == 0);
	
	oStr.Set("\xEF\xBF\xBF""bca\xEF\xBF\xBF""cab\xEF\xBF\xBF");
	oFromPos.Last();
	oStr.Cut(oFromPos, true, oToPos, false);
	Test(oStr.ByteLength() == 12);
	Test(oStr.Length() == 8);
	Test(StringCompare(oStr.Utf8(), "\xEF\xBF\xBF""bca\xEF\xBF\xBF""cab") == 0);
	
	oStr.Set("\xEF\xBF\xBF""bca\xEF\xBF\xBF""cab\xEF\xBF\xBF");
	oStr.Cut(oFromPos, false, oToPos, false);
	Test(oStr.ByteLength() == 15);
	Test(oStr.Length() == 9);
	Test(StringCompare(oStr.Utf8(), "\xEF\xBF\xBF""bca\xEF\xBF\xBF""cab\xEF\xBF\xBF") == 0);
	
	
	// 4-byte seqence
	
	oStr.Set("\xF7\xBF\xBF\xBF""bca\xF7\xBF\xBF\xBF""cab\xF7\xBF\xBF\xBF");
	oFromPos.First();
	oToPos.First();
	oStr.Cut(oFromPos, true, oToPos, true);
	Test(oStr.ByteLength() == 18);
	Test(oStr.Length() == 9);
	Test(StringCompare(oStr.Utf8(), "\xF7\xBF\xBF\xBF""bca\xF7\xBF\xBF\xBF""cab\xF7\xBF\xBF\xBF") == 0);
	
	oStr.Set("\xF7\xBF\xBF\xBF""bca\xF7\xBF\xBF\xBF""cab\xF7\xBF\xBF\xBF");
	oToPos.Forth(2);
	oStr.Cut(oFromPos, true, oToPos, false);
	Test(oStr.ByteLength() == 12);
	Test(oStr.Length() == 6);
	Test(StringCompare(oStr.Utf8(), "a\xF7\xBF\xBF\xBF""cab\xF7\xBF\xBF\xBF") == 0);
	
	oStr.Set("\xF7\xBF\xBF\xBF""bca\xF7\xBF\xBF\xBF""cab\xF7\xBF\xBF\xBF");
	oToPos.Forth(4);
	oStr.Cut(oFromPos, false, oToPos, true);
	Test(oStr.ByteLength() == 10);
	Test(oStr.Length() == 4);
	Test(StringCompare(oStr.Utf8(), "\xF7\xBF\xBF\xBF""ab\xF7\xBF\xBF\xBF") == 0);
	
	oStr.Set("\xF7\xBF\xBF\xBF""bca\xF7\xBF\xBF\xBF""cab\xF7\xBF\xBF\xBF");
	oFromPos.SkipFirst(4);
	oToPos.Last();
	oStr.Cut(oFromPos, false, oToPos, false);
	Test(oStr.ByteLength() == 11);
	Test(oStr.Length() == 5);
	Test(StringCompare(oStr.Utf8(), "\xF7\xBF\xBF\xBF""bca\xF7\xBF\xBF\xBF") == 0);
	
	oStr.Set("\xF7\xBF\xBF\xBF""bca\xF7\xBF\xBF\xBF""cab\xF7\xBF\xBF\xBF");
	oFromPos.Next();
	oStr.Cut(oFromPos, true, oToPos, false);
	Test(oStr.ByteLength() == 11);
	Test(oStr.Length() == 5);
	Test(StringCompare(oStr.Utf8(), "\xF7\xBF\xBF\xBF""bca\xF7\xBF\xBF\xBF") == 0);
	
	oStr.Set("\xF7\xBF\xBF\xBF""bca\xF7\xBF\xBF\xBF""cab\xF7\xBF\xBF\xBF");
	oFromPos.Last();
	oStr.Cut(oFromPos, true, oToPos, false);
	Test(oStr.ByteLength() == 14);
	Test(oStr.Length() == 8);
	Test(StringCompare(oStr.Utf8(), "\xF7\xBF\xBF\xBF""bca\xF7\xBF\xBF\xBF""cab") == 0);
	
	oStr.Set("\xF7\xBF\xBF\xBF""bca\xF7\xBF\xBF\xBF""cab\xF7\xBF\xBF\xBF");
	oStr.Cut(oFromPos, false, oToPos, false);
	Test(oStr.ByteLength() == 18);
	Test(oStr.Length() == 9);
	Test(StringCompare(oStr.Utf8(), "\xF7\xBF\xBF\xBF""bca\xF7\xBF\xBF\xBF""cab\xF7\xBF\xBF\xBF") == 0);
	
	
	// 5-byte seqence
	
	oStr.Set("\xFB\xBF\xBF\xBF\xBF""bca\xFB\xBF\xBF\xBF\xBF""cab\xFB\xBF\xBF\xBF\xBF");
	oFromPos.First();
	oToPos.First();
	oStr.Cut(oFromPos, true, oToPos, true);
	Test(oStr.ByteLength() == 21);
	Test(oStr.Length() == 9);
	Test(StringCompare(oStr.Utf8(), "\xFB\xBF\xBF\xBF\xBF""bca\xFB\xBF\xBF\xBF\xBF""cab\xFB\xBF\xBF\xBF\xBF") == 0);
	
	oStr.Set("\xFB\xBF\xBF\xBF\xBF""bca\xFB\xBF\xBF\xBF\xBF""cab\xFB\xBF\xBF\xBF\xBF");
	oToPos.Forth(2);
	oStr.Cut(oFromPos, true, oToPos, false);
	Test(oStr.ByteLength() == 14);
	Test(oStr.Length() == 6);
	Test(StringCompare(oStr.Utf8(), "a\xFB\xBF\xBF\xBF\xBF""cab\xFB\xBF\xBF\xBF\xBF") == 0);
	
	oStr.Set("\xFB\xBF\xBF\xBF\xBF""bca\xFB\xBF\xBF\xBF\xBF""cab\xFB\xBF\xBF\xBF\xBF");
	oToPos.Forth(4);
	oStr.Cut(oFromPos, false, oToPos, true);
	Test(oStr.ByteLength() == 12);
	Test(oStr.Length() == 4);
	Test(StringCompare(oStr.Utf8(), "\xFB\xBF\xBF\xBF\xBF""ab\xFB\xBF\xBF\xBF\xBF") == 0);
	
	oStr.Set("\xFB\xBF\xBF\xBF\xBF""bca\xFB\xBF\xBF\xBF\xBF""cab\xFB\xBF\xBF\xBF\xBF");
	oFromPos.SkipFirst(4);
	oToPos.Last();
	oStr.Cut(oFromPos, false, oToPos, false);
	Test(oStr.ByteLength() == 13);
	Test(oStr.Length() == 5);
	Test(StringCompare(oStr.Utf8(), "\xFB\xBF\xBF\xBF\xBF""bca\xFB\xBF\xBF\xBF\xBF") == 0);
	
	oStr.Set("\xFB\xBF\xBF\xBF\xBF""bca\xFB\xBF\xBF\xBF\xBF""cab\xFB\xBF\xBF\xBF\xBF");
	oFromPos.Next();
	oStr.Cut(oFromPos, true, oToPos, false);
	Test(oStr.ByteLength() == 13);
	Test(oStr.Length() == 5);
	Test(StringCompare(oStr.Utf8(), "\xFB\xBF\xBF\xBF\xBF""bca\xFB\xBF\xBF\xBF\xBF") == 0);
	
	oStr.Set("\xFB\xBF\xBF\xBF\xBF""bca\xFB\xBF\xBF\xBF\xBF""cab\xFB\xBF\xBF\xBF\xBF");
	oFromPos.Last();
	oStr.Cut(oFromPos, true, oToPos, false);
	Test(oStr.ByteLength() == 16);
	Test(oStr.Length() == 8);
	Test(StringCompare(oStr.Utf8(), "\xFB\xBF\xBF\xBF\xBF""bca\xFB\xBF\xBF\xBF\xBF""cab") == 0);
	
	oStr.Set("\xFB\xBF\xBF\xBF\xBF""bca\xFB\xBF\xBF\xBF\xBF""cab\xFB\xBF\xBF\xBF\xBF");
	oStr.Cut(oFromPos, false, oToPos, false);
	Test(oStr.ByteLength() == 21);
	Test(oStr.Length() == 9);
	Test(StringCompare(oStr.Utf8(), "\xFB\xBF\xBF\xBF\xBF""bca\xFB\xBF\xBF\xBF\xBF""cab\xFB\xBF\xBF\xBF\xBF") == 0);
	
	
	// 6-byte seqence
	
	oStr.Set("\xFD\xBF\xBF\xBF\xBF\xBF""bca\xFD\xBF\xBF\xBF\xBF\xBF""cab\xFD\xBF\xBF\xBF\xBF\xBF");
	oFromPos.First();
	oToPos.First();
	oStr.Cut(oFromPos, true, oToPos, true);
	Test(oStr.ByteLength() == 24);
	Test(oStr.Length() == 9);
	Test(StringCompare(oStr.Utf8(), "\xFD\xBF\xBF\xBF\xBF\xBF""bca\xFD\xBF\xBF\xBF\xBF\xBF""cab\xFD\xBF\xBF\xBF\xBF\xBF") == 0);
	
	oStr.Set("\xFD\xBF\xBF\xBF\xBF\xBF""bca\xFD\xBF\xBF\xBF\xBF\xBF""cab\xFD\xBF\xBF\xBF\xBF\xBF");
	oToPos.Forth(2);
	oStr.Cut(oFromPos, true, oToPos, false);
	Test(oStr.ByteLength() == 16);
	Test(oStr.Length() == 6);
	Test(StringCompare(oStr.Utf8(), "a\xFD\xBF\xBF\xBF\xBF\xBF""cab\xFD\xBF\xBF\xBF\xBF\xBF") == 0);
	
	oStr.Set("\xFD\xBF\xBF\xBF\xBF\xBF""bca\xFD\xBF\xBF\xBF\xBF\xBF""cab\xFD\xBF\xBF\xBF\xBF\xBF");
	oToPos.Forth(4);
	oStr.Cut(oFromPos, false, oToPos, true);
	Test(oStr.ByteLength() == 14);
	Test(oStr.Length() == 4);
	Test(StringCompare(oStr.Utf8(), "\xFD\xBF\xBF\xBF\xBF\xBF""ab\xFD\xBF\xBF\xBF\xBF\xBF") == 0);
	
	oStr.Set("\xFD\xBF\xBF\xBF\xBF\xBF""bca\xFD\xBF\xBF\xBF\xBF\xBF""cab\xFD\xBF\xBF\xBF\xBF\xBF");
	oFromPos.SkipFirst(4);
	oToPos.Last();
	oStr.Cut(oFromPos, false, oToPos, false);
	Test(oStr.ByteLength() == 15);
	Test(oStr.Length() == 5);
	Test(StringCompare(oStr.Utf8(), "\xFD\xBF\xBF\xBF\xBF\xBF""bca\xFD\xBF\xBF\xBF\xBF\xBF") == 0);
	
	oStr.Set("\xFD\xBF\xBF\xBF\xBF\xBF""bca\xFD\xBF\xBF\xBF\xBF\xBF""cab\xFD\xBF\xBF\xBF\xBF\xBF");
	oFromPos.Next();
	oStr.Cut(oFromPos, true, oToPos, false);
	Test(oStr.ByteLength() == 15);
	Test(oStr.Length() == 5);
	Test(StringCompare(oStr.Utf8(), "\xFD\xBF\xBF\xBF\xBF\xBF""bca\xFD\xBF\xBF\xBF\xBF\xBF") == 0);
	
	oStr.Set("\xFD\xBF\xBF\xBF\xBF\xBF""bca\xFD\xBF\xBF\xBF\xBF\xBF""cab\xFD\xBF\xBF\xBF\xBF\xBF");
	oFromPos.Last();
	oStr.Cut(oFromPos, true, oToPos, false);
	Test(oStr.ByteLength() == 18);
	Test(oStr.Length() == 8);
	Test(StringCompare(oStr.Utf8(), "\xFD\xBF\xBF\xBF\xBF\xBF""bca\xFD\xBF\xBF\xBF\xBF\xBF""cab") == 0);
	
	oStr.Set("\xFD\xBF\xBF\xBF\xBF\xBF""bca\xFD\xBF\xBF\xBF\xBF\xBF""cab\xFD\xBF\xBF\xBF\xBF\xBF");
	oStr.Cut(oFromPos, false, oToPos, false);
	Test(oStr.ByteLength() == 24);
	Test(oStr.Length() == 9);
	Test(StringCompare(oStr.Utf8(), "\xFD\xBF\xBF\xBF\xBF\xBF""bca\xFD\xBF\xBF\xBF\xBF\xBF""cab\xFD\xBF\xBF\xBF\xBF\xBF") == 0);
	
	
	return 0;
}

static int TestCutLeftCount()
{
	PrintFn();
	
	
	acpl::String oStr;
	
	
	// 1-byte seqence
	
	oStr.Set("abcabcabc").CutLeft(0);
	Test(oStr.ByteLength() == 9);
	Test(oStr.Length() == 9);
	Test(StringCompare(oStr.Utf8(), "abcabcabc") == 0);
	
	oStr.Set("abcabcabc").CutLeft(5);
	Test(oStr.ByteLength() == 4);
	Test(oStr.Length() == 4);
	Test(StringCompare(oStr.Utf8(), "cabc") == 0);
	
	oStr.Set("abcabcabc").CutLeft(8);
	Test(oStr.ByteLength() == 1);
	Test(oStr.Length() == 1);
	Test(StringCompare(oStr.Utf8(), "c") == 0);
	
	oStr.Set("abcabcabc").CutLeft(9);
	Test(oStr.ByteLength() == 0);
	Test(oStr.Length() == 0);
	Test(StringCompare(oStr.Utf8(), "") == 0);
	
	oStr.Set("abcabcabc").CutLeft(10);
	Test(oStr.ByteLength() == 0);
	Test(oStr.Length() == 0);
	Test(StringCompare(oStr.Utf8(), "") == 0);
	
	
	// 2-byte seqence
	
	oStr.Set("\xDF\xBF""bca\xDF\xBF""cab\xDF\xBF").CutLeft(0);
	Test(oStr.ByteLength() == 12);
	Test(oStr.Length() == 9);
	Test(StringCompare(oStr.Utf8(), "\xDF\xBF""bca\xDF\xBF""cab\xDF\xBF") == 0);
	
	oStr.Set("\xDF\xBF""bca\xDF\xBF""cab\xDF\xBF").CutLeft(5);
	Test(oStr.ByteLength() == 5);
	Test(oStr.Length() == 4);
	Test(StringCompare(oStr.Utf8(), "cab\xDF\xBF") == 0);
	
	oStr.Set("\xDF\xBF""bca\xDF\xBF""cab\xDF\xBF").CutLeft(8);
	Test(oStr.ByteLength() == 2);
	Test(oStr.Length() == 1);
	Test(StringCompare(oStr.Utf8(), "\xDF\xBF") == 0);
	
	oStr.Set("\xDF\xBF""bca\xDF\xBF""cab\xDF\xBF").CutLeft(9);
	Test(oStr.ByteLength() == 0);
	Test(oStr.Length() == 0);
	Test(StringCompare(oStr.Utf8(), "") == 0);
	
	oStr.Set("\xDF\xBF""bca\xDF\xBF""cab\xDF\xBF").CutLeft(10);
	Test(oStr.ByteLength() == 0);
	Test(oStr.Length() == 0);
	Test(StringCompare(oStr.Utf8(), "") == 0);
	
	
	// 3-byte seqence
	
	oStr.Set("\xEF\xBF\xBF""bca\xEF\xBF\xBF""cab\xEF\xBF\xBF").CutLeft(0);
	Test(oStr.ByteLength() == 15);
	Test(oStr.Length() == 9);
	Test(StringCompare(oStr.Utf8(), "\xEF\xBF\xBF""bca\xEF\xBF\xBF""cab\xEF\xBF\xBF") == 0);
	
	oStr.Set("\xEF\xBF\xBF""bca\xEF\xBF\xBF""cab\xEF\xBF\xBF").CutLeft(5);
	Test(oStr.ByteLength() == 6);
	Test(oStr.Length() == 4);
	Test(StringCompare(oStr.Utf8(), "cab\xEF\xBF\xBF") == 0);
	
	oStr.Set("\xEF\xBF\xBF""bca\xEF\xBF\xBF""cab\xEF\xBF\xBF").CutLeft(8);
	Test(oStr.ByteLength() == 3);
	Test(oStr.Length() == 1);
	Test(StringCompare(oStr.Utf8(), "\xEF\xBF\xBF") == 0);
	
	oStr.Set("\xEF\xBF\xBF""bca\xEF\xBF\xBF""cab\xEF\xBF\xBF").CutLeft(9);
	Test(oStr.ByteLength() == 0);
	Test(oStr.Length() == 0);
	Test(StringCompare(oStr.Utf8(), "") == 0);
	
	oStr.Set("\xEF\xBF\xBF""bca\xEF\xBF\xBF""cab\xEF\xBF\xBF").CutLeft(10);
	Test(oStr.ByteLength() == 0);
	Test(oStr.Length() == 0);
	Test(StringCompare(oStr.Utf8(), "") == 0);
	
	
	// 4-byte seqence
	
	oStr.Set("\xF7\xBF\xBF\xBF""bca\xF7\xBF\xBF\xBF""cab\xF7\xBF\xBF\xBF").CutLeft(0);
	Test(oStr.ByteLength() == 18);
	Test(oStr.Length() == 9);
	Test(StringCompare(oStr.Utf8(), "\xF7\xBF\xBF\xBF""bca\xF7\xBF\xBF\xBF""cab\xF7\xBF\xBF\xBF") == 0);
	
	oStr.Set("\xF7\xBF\xBF\xBF""bca\xF7\xBF\xBF\xBF""cab\xF7\xBF\xBF\xBF").CutLeft(5);
	Test(oStr.ByteLength() == 7);
	Test(oStr.Length() == 4);
	Test(StringCompare(oStr.Utf8(), "cab\xF7\xBF\xBF\xBF") == 0);
	
	oStr.Set("\xF7\xBF\xBF\xBF""bca\xF7\xBF\xBF\xBF""cab\xF7\xBF\xBF\xBF").CutLeft(8);
	Test(oStr.ByteLength() == 4);
	Test(oStr.Length() == 1);
	Test(StringCompare(oStr.Utf8(), "\xF7\xBF\xBF\xBF") == 0);
	
	oStr.Set("\xF7\xBF\xBF\xBF""bca\xF7\xBF\xBF\xBF""cab\xF7\xBF\xBF\xBF").CutLeft(9);
	Test(oStr.ByteLength() == 0);
	Test(oStr.Length() == 0);
	Test(StringCompare(oStr.Utf8(), "") == 0);
	
	oStr.Set("\xF7\xBF\xBF\xBF""bca\xF7\xBF\xBF\xBF""cab\xF7\xBF\xBF\xBF").CutLeft(10);
	Test(oStr.ByteLength() == 0);
	Test(oStr.Length() == 0);
	Test(StringCompare(oStr.Utf8(), "") == 0);
	
	
	// 5-byte seqence
	
	oStr.Set("\xFB\xBF\xBF\xBF\xBF""bca\xFB\xBF\xBF\xBF\xBF""cab\xFB\xBF\xBF\xBF\xBF").CutLeft(0);
	Test(oStr.ByteLength() == 21);
	Test(oStr.Length() == 9);
	Test(StringCompare(oStr.Utf8(), "\xFB\xBF\xBF\xBF\xBF""bca\xFB\xBF\xBF\xBF\xBF""cab\xFB\xBF\xBF\xBF\xBF") == 0);
	
	oStr.Set("\xFB\xBF\xBF\xBF\xBF""bca\xFB\xBF\xBF\xBF\xBF""cab\xFB\xBF\xBF\xBF\xBF").CutLeft(5);
	Test(oStr.ByteLength() == 8);
	Test(oStr.Length() == 4);
	Test(StringCompare(oStr.Utf8(), "cab\xFB\xBF\xBF\xBF\xBF") == 0);
	
	oStr.Set("\xFB\xBF\xBF\xBF\xBF""bca\xFB\xBF\xBF\xBF\xBF""cab\xFB\xBF\xBF\xBF\xBF").CutLeft(8);
	Test(oStr.ByteLength() == 5);
	Test(oStr.Length() == 1);
	Test(StringCompare(oStr.Utf8(), "\xFB\xBF\xBF\xBF\xBF") == 0);
	
	oStr.Set("\xFB\xBF\xBF\xBF\xBF""bca\xFB\xBF\xBF\xBF\xBF""cab\xFB\xBF\xBF\xBF\xBF").CutLeft(9);
	Test(oStr.ByteLength() == 0);
	Test(oStr.Length() == 0);
	Test(StringCompare(oStr.Utf8(), "") == 0);
	
	oStr.Set("\xFB\xBF\xBF\xBF\xBF""bca\xFB\xBF\xBF\xBF\xBF""cab\xFB\xBF\xBF\xBF\xBF").CutLeft(10);
	Test(oStr.ByteLength() == 0);
	Test(oStr.Length() == 0);
	Test(StringCompare(oStr.Utf8(), "") == 0);
	
	
	// 6-byte seqence
	
	oStr.Set("\xFD\xBF\xBF\xBF\xBF\xBF""bca\xFD\xBF\xBF\xBF\xBF\xBF""cab\xFD\xBF\xBF\xBF\xBF\xBF").CutLeft(0);
	Test(oStr.ByteLength() == 24);
	Test(oStr.Length() == 9);
	Test(StringCompare(oStr.Utf8(), "\xFD\xBF\xBF\xBF\xBF\xBF""bca\xFD\xBF\xBF\xBF\xBF\xBF""cab\xFD\xBF\xBF\xBF\xBF\xBF") == 0);
	
	oStr.Set("\xFD\xBF\xBF\xBF\xBF\xBF""bca\xFD\xBF\xBF\xBF\xBF\xBF""cab\xFD\xBF\xBF\xBF\xBF\xBF").CutLeft(5);
	Test(oStr.ByteLength() == 9);
	Test(oStr.Length() == 4);
	Test(StringCompare(oStr.Utf8(), "cab\xFD\xBF\xBF\xBF\xBF\xBF") == 0);
	
	oStr.Set("\xFD\xBF\xBF\xBF\xBF\xBF""bca\xFD\xBF\xBF\xBF\xBF\xBF""cab\xFD\xBF\xBF\xBF\xBF\xBF").CutLeft(8);
	Test(oStr.ByteLength() == 6);
	Test(oStr.Length() == 1);
	Test(StringCompare(oStr.Utf8(), "\xFD\xBF\xBF\xBF\xBF\xBF") == 0);
	
	oStr.Set("\xFD\xBF\xBF\xBF\xBF\xBF""bca\xFD\xBF\xBF\xBF\xBF\xBF""cab\xFD\xBF\xBF\xBF\xBF\xBF").CutLeft(9);
	Test(oStr.ByteLength() == 0);
	Test(oStr.Length() == 0);
	Test(StringCompare(oStr.Utf8(), "") == 0);
	
	oStr.Set("\xFD\xBF\xBF\xBF\xBF\xBF""bca\xFD\xBF\xBF\xBF\xBF\xBF""cab\xFD\xBF\xBF\xBF\xBF\xBF").CutLeft(10);
	Test(oStr.ByteLength() == 0);
	Test(oStr.Length() == 0);
	Test(StringCompare(oStr.Utf8(), "") == 0);
	
	
	return 0;
}

static int TestCutLeftPos()
{
	PrintFn();
	
	
	acpl::String oStr;
	acpl::String::Pos oPos(oStr);
	
	
	// 1-byte seqence
	
	oStr.Set("abcabcabc");
	oPos.First();
	oStr.CutLeft(oPos, true);
	Test(oStr.ByteLength() == 9);
	Test(oStr.Length() == 9);
	Test(StringCompare(oStr.Utf8(), "abcabcabc") == 0);
	
	oStr.Set("abcabcabc");
	oPos.Forth(4);
	oStr.CutLeft(oPos, false);
	Test(oStr.ByteLength() == 4);
	Test(oStr.Length() == 4);
	Test(StringCompare(oStr.Utf8(), "cabc") == 0);
	
	oStr.Set("abcabcabc");
	oPos.Last();
	oStr.CutLeft(oPos, true);
	Test(oStr.ByteLength() == 1);
	Test(oStr.Length() == 1);
	Test(StringCompare(oStr.Utf8(), "c") == 0);
	
	oStr.Set("abcabcabc");
	oStr.CutLeft(oPos, false);
	Test(oStr.ByteLength() == 0);
	Test(oStr.Length() == 0);
	Test(StringCompare(oStr.Utf8(), "") == 0);
	
	
	// 2-byte seqence
	
	oStr.Set("\xDF\xBF""bca\xDF\xBF""cab\xDF\xBF");
	oPos.First();
	oStr.CutLeft(0);
	Test(oStr.ByteLength() == 12);
	Test(oStr.Length() == 9);
	Test(StringCompare(oStr.Utf8(), "\xDF\xBF""bca\xDF\xBF""cab\xDF\xBF") == 0);
	
	oStr.Set("\xDF\xBF""bca\xDF\xBF""cab\xDF\xBF");
	oPos.Forth(4);
	oStr.CutLeft(oPos, false);
	Test(oStr.ByteLength() == 5);
	Test(oStr.Length() == 4);
	Test(StringCompare(oStr.Utf8(), "cab\xDF\xBF") == 0);
	
	oStr.Set("\xDF\xBF""bca\xDF\xBF""cab\xDF\xBF");
	oPos.Last();
	oStr.CutLeft(oPos, true);
	Test(oStr.ByteLength() == 2);
	Test(oStr.Length() == 1);
	Test(StringCompare(oStr.Utf8(), "\xDF\xBF") == 0);
	
	oStr.Set("\xDF\xBF""bca\xDF\xBF""cab\xDF\xBF");
	oStr.CutLeft(oPos, false);
	Test(oStr.ByteLength() == 0);
	Test(oStr.Length() == 0);
	Test(StringCompare(oStr.Utf8(), "") == 0);
	
	
	// 3-byte seqence
	
	oStr.Set("\xEF\xBF\xBF""bca\xEF\xBF\xBF""cab\xEF\xBF\xBF");
	oPos.First();
	oStr.CutLeft(oPos, true);
	Test(oStr.ByteLength() == 15);
	Test(oStr.Length() == 9);
	Test(StringCompare(oStr.Utf8(), "\xEF\xBF\xBF""bca\xEF\xBF\xBF""cab\xEF\xBF\xBF") == 0);
	
	oStr.Set("\xEF\xBF\xBF""bca\xEF\xBF\xBF""cab\xEF\xBF\xBF");
	oPos.Forth(4);
	oStr.CutLeft(oPos, false);
	Test(oStr.ByteLength() == 6);
	Test(oStr.Length() == 4);
	Test(StringCompare(oStr.Utf8(), "cab\xEF\xBF\xBF") == 0);
	
	oStr.Set("\xEF\xBF\xBF""bca\xEF\xBF\xBF""cab\xEF\xBF\xBF");
	oPos.Last();
	oStr.CutLeft(oPos, true);
	Test(oStr.ByteLength() == 3);
	Test(oStr.Length() == 1);
	Test(StringCompare(oStr.Utf8(), "\xEF\xBF\xBF") == 0);
	
	oStr.Set("\xEF\xBF\xBF""bca\xEF\xBF\xBF""cab\xEF\xBF\xBF");
	oStr.CutLeft(oPos, false);
	Test(oStr.ByteLength() == 0);
	Test(oStr.Length() == 0);
	Test(StringCompare(oStr.Utf8(), "") == 0);
	
	
	// 4-byte seqence
	
	oStr.Set("\xF7\xBF\xBF\xBF""bca\xF7\xBF\xBF\xBF""cab\xF7\xBF\xBF\xBF");
	oPos.First();
	oStr.CutLeft(oPos, true);
	Test(oStr.ByteLength() == 18);
	Test(oStr.Length() == 9);
	Test(StringCompare(oStr.Utf8(), "\xF7\xBF\xBF\xBF""bca\xF7\xBF\xBF\xBF""cab\xF7\xBF\xBF\xBF") == 0);
	
	oStr.Set("\xF7\xBF\xBF\xBF""bca\xF7\xBF\xBF\xBF""cab\xF7\xBF\xBF\xBF");
	oPos.Forth(4);
	oStr.CutLeft(oPos, false);
	Test(oStr.ByteLength() == 7);
	Test(oStr.Length() == 4);
	Test(StringCompare(oStr.Utf8(), "cab\xF7\xBF\xBF\xBF") == 0);
	
	oStr.Set("\xF7\xBF\xBF\xBF""bca\xF7\xBF\xBF\xBF""cab\xF7\xBF\xBF\xBF");
	oPos.Last();
	oStr.CutLeft(oPos, true);
	Test(oStr.ByteLength() == 4);
	Test(oStr.Length() == 1);
	Test(StringCompare(oStr.Utf8(), "\xF7\xBF\xBF\xBF") == 0);
	
	oStr.Set("\xF7\xBF\xBF\xBF""bca\xF7\xBF\xBF\xBF""cab\xF7\xBF\xBF\xBF");
	oStr.CutLeft(oPos, false);
	Test(oStr.ByteLength() == 0);
	Test(oStr.Length() == 0);
	Test(StringCompare(oStr.Utf8(), "") == 0);
	
	
	// 5-byte seqence
	
	oStr.Set("\xFB\xBF\xBF\xBF\xBF""bca\xFB\xBF\xBF\xBF\xBF""cab\xFB\xBF\xBF\xBF\xBF");
	oPos.First();
	oStr.CutLeft(oPos, true);
	Test(oStr.ByteLength() == 21);
	Test(oStr.Length() == 9);
	Test(StringCompare(oStr.Utf8(), "\xFB\xBF\xBF\xBF\xBF""bca\xFB\xBF\xBF\xBF\xBF""cab\xFB\xBF\xBF\xBF\xBF") == 0);
	
	oStr.Set("\xFB\xBF\xBF\xBF\xBF""bca\xFB\xBF\xBF\xBF\xBF""cab\xFB\xBF\xBF\xBF\xBF");
	oPos.Forth(4);
	oStr.CutLeft(oPos, false);
	Test(oStr.ByteLength() == 8);
	Test(oStr.Length() == 4);
	Test(StringCompare(oStr.Utf8(), "cab\xFB\xBF\xBF\xBF\xBF") == 0);
	
	oStr.Set("\xFB\xBF\xBF\xBF\xBF""bca\xFB\xBF\xBF\xBF\xBF""cab\xFB\xBF\xBF\xBF\xBF");
	oPos.Last();
	oStr.CutLeft(oPos, true);
	Test(oStr.ByteLength() == 5);
	Test(oStr.Length() == 1);
	Test(StringCompare(oStr.Utf8(), "\xFB\xBF\xBF\xBF\xBF") == 0);
	
	oStr.Set("\xFB\xBF\xBF\xBF\xBF""bca\xFB\xBF\xBF\xBF\xBF""cab\xFB\xBF\xBF\xBF\xBF");
	oStr.CutLeft(oPos, false);
	Test(oStr.ByteLength() == 0);
	Test(oStr.Length() == 0);
	Test(StringCompare(oStr.Utf8(), "") == 0);
	
	
	// 6-byte seqence
	
	oStr.Set("\xFD\xBF\xBF\xBF\xBF\xBF""bca\xFD\xBF\xBF\xBF\xBF\xBF""cab\xFD\xBF\xBF\xBF\xBF\xBF");
	oPos.First();
	oStr.CutLeft(oPos, true);
	Test(oStr.ByteLength() == 24);
	Test(oStr.Length() == 9);
	Test(StringCompare(oStr.Utf8(), "\xFD\xBF\xBF\xBF\xBF\xBF""bca\xFD\xBF\xBF\xBF\xBF\xBF""cab\xFD\xBF\xBF\xBF\xBF\xBF") == 0);
	
	oStr.Set("\xFD\xBF\xBF\xBF\xBF\xBF""bca\xFD\xBF\xBF\xBF\xBF\xBF""cab\xFD\xBF\xBF\xBF\xBF\xBF");
	oPos.Forth(4);
	oStr.CutLeft(oPos, false);
	Test(oStr.ByteLength() == 9);
	Test(oStr.Length() == 4);
	Test(StringCompare(oStr.Utf8(), "cab\xFD\xBF\xBF\xBF\xBF\xBF") == 0);
	
	oStr.Set("\xFD\xBF\xBF\xBF\xBF\xBF""bca\xFD\xBF\xBF\xBF\xBF\xBF""cab\xFD\xBF\xBF\xBF\xBF\xBF");
	oPos.Last();
	oStr.CutLeft(oPos, true);
	Test(oStr.ByteLength() == 6);
	Test(oStr.Length() == 1);
	Test(StringCompare(oStr.Utf8(), "\xFD\xBF\xBF\xBF\xBF\xBF") == 0);
	
	oStr.Set("\xFD\xBF\xBF\xBF\xBF\xBF""bca\xFD\xBF\xBF\xBF\xBF\xBF""cab\xFD\xBF\xBF\xBF\xBF\xBF");
	oStr.CutLeft(oPos, false);
	Test(oStr.ByteLength() == 0);
	Test(oStr.Length() == 0);
	Test(StringCompare(oStr.Utf8(), "") == 0);
	
	
	return 0;
}

static int TestCutRightCount()
{
	PrintFn();
	
	
	acpl::String oStr;
	
	
	// 1-byte seqence
	
	oStr.Set("abcabcabc").CutRight(0);
	Test(oStr.ByteLength() == 9);
	Test(oStr.Length() == 9);
	Test(StringCompare(oStr.Utf8(), "abcabcabc") == 0);
	
	oStr.Set("abcabcabc").CutRight(5);
	Test(oStr.ByteLength() == 4);
	Test(oStr.Length() == 4);
	Test(StringCompare(oStr.Utf8(), "abca") == 0);
	
	oStr.Set("abcabcabc").CutRight(8);
	Test(oStr.ByteLength() == 1);
	Test(oStr.Length() == 1);
	Test(StringCompare(oStr.Utf8(), "a") == 0);
	
	oStr.Set("abcabcabc").CutRight(9);
	Test(oStr.ByteLength() == 0);
	Test(oStr.Length() == 0);
	Test(StringCompare(oStr.Utf8(), "") == 0);
	
	oStr.Set("abcabcabc").CutRight(10);
	Test(oStr.ByteLength() == 0);
	Test(oStr.Length() == 0);
	Test(StringCompare(oStr.Utf8(), "") == 0);
	
	
	// 2-byte seqence
	
	oStr.Set("\xDF\xBF""bca\xDF\xBF""cab\xDF\xBF").CutRight(0);
	Test(oStr.ByteLength() == 12);
	Test(oStr.Length() == 9);
	Test(StringCompare(oStr.Utf8(), "\xDF\xBF""bca\xDF\xBF""cab\xDF\xBF") == 0);
	
	oStr.Set("\xDF\xBF""bca\xDF\xBF""cab\xDF\xBF").CutRight(5);
	Test(oStr.ByteLength() == 5);
	Test(oStr.Length() == 4);
	Test(StringCompare(oStr.Utf8(), "\xDF\xBF""bca") == 0);
	
	oStr.Set("\xDF\xBF""bca\xDF\xBF""cab\xDF\xBF").CutRight(8);
	Test(oStr.ByteLength() == 2);
	Test(oStr.Length() == 1);
	Test(StringCompare(oStr.Utf8(), "\xDF\xBF") == 0);
	
	oStr.Set("\xDF\xBF""bca\xDF\xBF""cab\xDF\xBF").CutRight(9);
	Test(oStr.ByteLength() == 0);
	Test(oStr.Length() == 0);
	Test(StringCompare(oStr.Utf8(), "") == 0);
	
	oStr.Set("\xDF\xBF""bca\xDF\xBF""cab\xDF\xBF").CutRight(10);
	Test(oStr.ByteLength() == 0);
	Test(oStr.Length() == 0);
	Test(StringCompare(oStr.Utf8(), "") == 0);
	
	
	// 3-byte seqence
	
	oStr.Set("\xEF\xBF\xBF""bca\xEF\xBF\xBF""cab\xEF\xBF\xBF").CutRight(0);
	Test(oStr.ByteLength() == 15);
	Test(oStr.Length() == 9);
	Test(StringCompare(oStr.Utf8(), "\xEF\xBF\xBF""bca\xEF\xBF\xBF""cab\xEF\xBF\xBF") == 0);
	
	oStr.Set("\xEF\xBF\xBF""bca\xEF\xBF\xBF""cab\xEF\xBF\xBF").CutRight(5);
	Test(oStr.ByteLength() == 6);
	Test(oStr.Length() == 4);
	Test(StringCompare(oStr.Utf8(), "\xEF\xBF\xBF""bca") == 0);
	
	oStr.Set("\xEF\xBF\xBF""bca\xEF\xBF\xBF""cab\xEF\xBF\xBF").CutRight(8);
	Test(oStr.ByteLength() == 3);
	Test(oStr.Length() == 1);
	Test(StringCompare(oStr.Utf8(), "\xEF\xBF\xBF") == 0);
	
	oStr.Set("\xEF\xBF\xBF""bca\xEF\xBF\xBF""cab\xEF\xBF\xBF").CutRight(9);
	Test(oStr.ByteLength() == 0);
	Test(oStr.Length() == 0);
	Test(StringCompare(oStr.Utf8(), "") == 0);
	
	oStr.Set("\xEF\xBF\xBF""bca\xEF\xBF\xBF""cab\xEF\xBF\xBF").CutRight(10);
	Test(oStr.ByteLength() == 0);
	Test(oStr.Length() == 0);
	Test(StringCompare(oStr.Utf8(), "") == 0);
	
	
	// 4-byte seqence
	
	oStr.Set("\xF7\xBF\xBF\xBF""bca\xF7\xBF\xBF\xBF""cab\xF7\xBF\xBF\xBF").CutRight(0);
	Test(oStr.ByteLength() == 18);
	Test(oStr.Length() == 9);
	Test(StringCompare(oStr.Utf8(), "\xF7\xBF\xBF\xBF""bca\xF7\xBF\xBF\xBF""cab\xF7\xBF\xBF\xBF") == 0);
	
	oStr.Set("\xF7\xBF\xBF\xBF""bca\xF7\xBF\xBF\xBF""cab\xF7\xBF\xBF\xBF").CutRight(5);
	Test(oStr.ByteLength() == 7);
	Test(oStr.Length() == 4);
	Test(StringCompare(oStr.Utf8(), "\xF7\xBF\xBF\xBF""bca") == 0);
	
	oStr.Set("\xF7\xBF\xBF\xBF""bca\xF7\xBF\xBF\xBF""cab\xF7\xBF\xBF\xBF").CutRight(8);
	Test(oStr.ByteLength() == 4);
	Test(oStr.Length() == 1);
	Test(StringCompare(oStr.Utf8(), "\xF7\xBF\xBF\xBF") == 0);
	
	oStr.Set("\xF7\xBF\xBF\xBF""bca\xF7\xBF\xBF\xBF""cab\xF7\xBF\xBF\xBF").CutRight(9);
	Test(oStr.ByteLength() == 0);
	Test(oStr.Length() == 0);
	Test(StringCompare(oStr.Utf8(), "") == 0);
	
	oStr.Set("\xF7\xBF\xBF\xBF""bca\xF7\xBF\xBF\xBF""cab\xF7\xBF\xBF\xBF").CutRight(10);
	Test(oStr.ByteLength() == 0);
	Test(oStr.Length() == 0);
	Test(StringCompare(oStr.Utf8(), "") == 0);
	
	
	// 5-byte seqence
	
	oStr.Set("\xFB\xBF\xBF\xBF\xBF""bca\xFB\xBF\xBF\xBF\xBF""cab\xFB\xBF\xBF\xBF\xBF").CutRight(0);
	Test(oStr.ByteLength() == 21);
	Test(oStr.Length() == 9);
	Test(StringCompare(oStr.Utf8(), "\xFB\xBF\xBF\xBF\xBF""bca\xFB\xBF\xBF\xBF\xBF""cab\xFB\xBF\xBF\xBF\xBF") == 0);
	
	oStr.Set("\xFB\xBF\xBF\xBF\xBF""bca\xFB\xBF\xBF\xBF\xBF""cab\xFB\xBF\xBF\xBF\xBF").CutRight(5);
	Test(oStr.ByteLength() == 8);
	Test(oStr.Length() == 4);
	Test(StringCompare(oStr.Utf8(), "\xFB\xBF\xBF\xBF\xBF""bca") == 0);
	
	oStr.Set("\xFB\xBF\xBF\xBF\xBF""bca\xFB\xBF\xBF\xBF\xBF""cab\xFB\xBF\xBF\xBF\xBF").CutRight(8);
	Test(oStr.ByteLength() == 5);
	Test(oStr.Length() == 1);
	Test(StringCompare(oStr.Utf8(), "\xFB\xBF\xBF\xBF\xBF") == 0);
	
	oStr.Set("\xFB\xBF\xBF\xBF\xBF""bca\xFB\xBF\xBF\xBF\xBF""cab\xFB\xBF\xBF\xBF\xBF").CutRight(9);
	Test(oStr.ByteLength() == 0);
	Test(oStr.Length() == 0);
	Test(StringCompare(oStr.Utf8(), "") == 0);
	
	oStr.Set("\xFB\xBF\xBF\xBF\xBF""bca\xFB\xBF\xBF\xBF\xBF""cab\xFB\xBF\xBF\xBF\xBF").CutRight(10);
	Test(oStr.ByteLength() == 0);
	Test(oStr.Length() == 0);
	Test(StringCompare(oStr.Utf8(), "") == 0);
	
	
	// 6-byte seqence
	
	oStr.Set("\xFD\xBF\xBF\xBF\xBF\xBF""bca\xFD\xBF\xBF\xBF\xBF\xBF""cab\xFD\xBF\xBF\xBF\xBF\xBF").CutRight(0);
	Test(oStr.ByteLength() == 24);
	Test(oStr.Length() == 9);
	Test(StringCompare(oStr.Utf8(), "\xFD\xBF\xBF\xBF\xBF\xBF""bca\xFD\xBF\xBF\xBF\xBF\xBF""cab\xFD\xBF\xBF\xBF\xBF\xBF") == 0);
	
	oStr.Set("\xFD\xBF\xBF\xBF\xBF\xBF""bca\xFD\xBF\xBF\xBF\xBF\xBF""cab\xFD\xBF\xBF\xBF\xBF\xBF").CutRight(5);
	Test(oStr.ByteLength() == 9);
	Test(oStr.Length() == 4);
	Test(StringCompare(oStr.Utf8(), "\xFD\xBF\xBF\xBF\xBF\xBF""bca") == 0);
	
	oStr.Set("\xFD\xBF\xBF\xBF\xBF\xBF""bca\xFD\xBF\xBF\xBF\xBF\xBF""cab\xFD\xBF\xBF\xBF\xBF\xBF").CutRight(8);
	Test(oStr.ByteLength() == 6);
	Test(oStr.Length() == 1);
	Test(StringCompare(oStr.Utf8(), "\xFD\xBF\xBF\xBF\xBF\xBF") == 0);
	
	oStr.Set("\xFD\xBF\xBF\xBF\xBF\xBF""bca\xFD\xBF\xBF\xBF\xBF\xBF""cab\xFD\xBF\xBF\xBF\xBF\xBF").CutRight(9);
	Test(oStr.ByteLength() == 0);
	Test(oStr.Length() == 0);
	Test(StringCompare(oStr.Utf8(), "") == 0);
	
	oStr.Set("\xFD\xBF\xBF\xBF\xBF\xBF""bca\xFD\xBF\xBF\xBF\xBF\xBF""cab\xFD\xBF\xBF\xBF\xBF\xBF").CutRight(10);
	Test(oStr.ByteLength() == 0);
	Test(oStr.Length() == 0);
	Test(StringCompare(oStr.Utf8(), "") == 0);
	
	
	return 0;
}

static int TestCutRightPos()
{
	PrintFn();
	
	
	acpl::String oStr;
	acpl::String::Pos oPos(oStr);
	
	
	// 1-byte seqence
	
	oStr.Set("abcabcabc");
	oPos.Last();
	oStr.CutRight(oPos, true);
	Test(oStr.ByteLength() == 9);
	Test(oStr.Length() == 9);
	Test(StringCompare(oStr.Utf8(), "abcabcabc") == 0);
	
	oStr.Set("abcabcabc");
	oPos.Back(4);
	oStr.CutRight(oPos, false);
	Test(oStr.ByteLength() == 4);
	Test(oStr.Length() == 4);
	Test(StringCompare(oStr.Utf8(), "abca") == 0);
	
	oStr.Set("abcabcabc");
	oPos.First();
	oStr.CutRight(oPos, true);
	Test(oStr.ByteLength() == 1);
	Test(oStr.Length() == 1);
	Test(StringCompare(oStr.Utf8(), "a") == 0);
	
	oStr.Set("abcabcabc");
	oStr.CutRight(oPos, false);
	Test(oStr.ByteLength() == 0);
	Test(oStr.Length() == 0);
	Test(StringCompare(oStr.Utf8(), "") == 0);
	
	
	// 2-byte seqence
	
	oStr.Set("\xDF\xBF""bca\xDF\xBF""cab\xDF\xBF");
	oPos.Last();
	oStr.CutRight(oPos, true);
	Test(oStr.ByteLength() == 12);
	Test(oStr.Length() == 9);
	Test(StringCompare(oStr.Utf8(), "\xDF\xBF""bca\xDF\xBF""cab\xDF\xBF") == 0);
	
	oStr.Set("\xDF\xBF""bca\xDF\xBF""cab\xDF\xBF");
	oPos.Back(4);
	oStr.CutRight(oPos, false);
	Test(oStr.ByteLength() == 5);
	Test(oStr.Length() == 4);
	Test(StringCompare(oStr.Utf8(), "\xDF\xBF""bca") == 0);
	
	oStr.Set("\xDF\xBF""bca\xDF\xBF""cab\xDF\xBF");
	oPos.First();
	oStr.CutRight(oPos, true);
	Test(oStr.ByteLength() == 2);
	Test(oStr.Length() == 1);
	Test(StringCompare(oStr.Utf8(), "\xDF\xBF") == 0);
	
	oStr.Set("\xDF\xBF""bca\xDF\xBF""cab\xDF\xBF");
	oStr.CutRight(oPos, false);
	Test(oStr.ByteLength() == 0);
	Test(oStr.Length() == 0);
	Test(StringCompare(oStr.Utf8(), "") == 0);
	
	
	// 3-byte seqence
	
	oStr.Set("\xEF\xBF\xBF""bca\xEF\xBF\xBF""cab\xEF\xBF\xBF");
	oPos.Last();
	oStr.CutRight(oPos, true);
	Test(oStr.ByteLength() == 15);
	Test(oStr.Length() == 9);
	Test(StringCompare(oStr.Utf8(), "\xEF\xBF\xBF""bca\xEF\xBF\xBF""cab\xEF\xBF\xBF") == 0);
	
	oStr.Set("\xEF\xBF\xBF""bca\xEF\xBF\xBF""cab\xEF\xBF\xBF");
	oPos.Back(4);
	oStr.CutRight(oPos, false);
	Test(oStr.ByteLength() == 6);
	Test(oStr.Length() == 4);
	Test(StringCompare(oStr.Utf8(), "\xEF\xBF\xBF""bca") == 0);
	
	oStr.Set("\xEF\xBF\xBF""bca\xEF\xBF\xBF""cab\xEF\xBF\xBF");
	oPos.First();
	oStr.CutRight(oPos, true);
	Test(oStr.ByteLength() == 3);
	Test(oStr.Length() == 1);
	Test(StringCompare(oStr.Utf8(), "\xEF\xBF\xBF") == 0);
	
	oStr.Set("\xEF\xBF\xBF""bca\xEF\xBF\xBF""cab\xEF\xBF\xBF");
	oStr.CutRight(oPos, false);
	Test(oStr.ByteLength() == 0);
	Test(oStr.Length() == 0);
	Test(StringCompare(oStr.Utf8(), "") == 0);
	
	
	// 4-byte seqence
	
	oStr.Set("\xF7\xBF\xBF\xBF""bca\xF7\xBF\xBF\xBF""cab\xF7\xBF\xBF\xBF");
	oPos.Last();
	oStr.CutRight(oPos, true);
	Test(oStr.ByteLength() == 18);
	Test(oStr.Length() == 9);
	Test(StringCompare(oStr.Utf8(), "\xF7\xBF\xBF\xBF""bca\xF7\xBF\xBF\xBF""cab\xF7\xBF\xBF\xBF") == 0);
	
	oStr.Set("\xF7\xBF\xBF\xBF""bca\xF7\xBF\xBF\xBF""cab\xF7\xBF\xBF\xBF");
	oPos.Back(4);
	oStr.CutRight(oPos, false);
	Test(oStr.ByteLength() == 7);
	Test(oStr.Length() == 4);
	Test(StringCompare(oStr.Utf8(), "\xF7\xBF\xBF\xBF""bca") == 0);
	
	oStr.Set("\xF7\xBF\xBF\xBF""bca\xF7\xBF\xBF\xBF""cab\xF7\xBF\xBF\xBF");
	oPos.First();
	oStr.CutRight(oPos, true);
	Test(oStr.ByteLength() == 4);
	Test(oStr.Length() == 1);
	Test(StringCompare(oStr.Utf8(), "\xF7\xBF\xBF\xBF") == 0);
	
	oStr.Set("\xF7\xBF\xBF\xBF""bca\xF7\xBF\xBF\xBF""cab\xF7\xBF\xBF\xBF");
	oStr.CutRight(oPos, false);
	Test(oStr.ByteLength() == 0);
	Test(oStr.Length() == 0);
	Test(StringCompare(oStr.Utf8(), "") == 0);
	
	// 5-byte seqence
	
	oStr.Set("\xFB\xBF\xBF\xBF\xBF""bca\xFB\xBF\xBF\xBF\xBF""cab\xFB\xBF\xBF\xBF\xBF");
	oPos.Last();
	oStr.CutRight(oPos, true);
	Test(oStr.ByteLength() == 21);
	Test(oStr.Length() == 9);
	Test(StringCompare(oStr.Utf8(), "\xFB\xBF\xBF\xBF\xBF""bca\xFB\xBF\xBF\xBF\xBF""cab\xFB\xBF\xBF\xBF\xBF") == 0);
	
	oStr.Set("\xFB\xBF\xBF\xBF\xBF""bca\xFB\xBF\xBF\xBF\xBF""cab\xFB\xBF\xBF\xBF\xBF");
	oPos.Back(4);
	oStr.CutRight(oPos, false);
	Test(oStr.ByteLength() == 8);
	Test(oStr.Length() == 4);
	Test(StringCompare(oStr.Utf8(), "\xFB\xBF\xBF\xBF\xBF""bca") == 0);
	
	oStr.Set("\xFB\xBF\xBF\xBF\xBF""bca\xFB\xBF\xBF\xBF\xBF""cab\xFB\xBF\xBF\xBF\xBF");
	oPos.First();
	oStr.CutRight(oPos, true);
	Test(oStr.ByteLength() == 5);
	Test(oStr.Length() == 1);
	Test(StringCompare(oStr.Utf8(), "\xFB\xBF\xBF\xBF\xBF") == 0);
	
	oStr.Set("\xFB\xBF\xBF\xBF\xBF""bca\xFB\xBF\xBF\xBF\xBF""cab\xFB\xBF\xBF\xBF\xBF");
	oStr.CutRight(oPos, false);
	Test(oStr.ByteLength() == 0);
	Test(oStr.Length() == 0);
	Test(StringCompare(oStr.Utf8(), "") == 0);
	
	
	// 6-byte seqence
	
	oStr.Set("\xFD\xBF\xBF\xBF\xBF\xBF""bca\xFD\xBF\xBF\xBF\xBF\xBF""cab\xFD\xBF\xBF\xBF\xBF\xBF");
	oPos.Last();
	oStr.CutRight(oPos, true);
	Test(oStr.ByteLength() == 24);
	Test(oStr.Length() == 9);
	Test(StringCompare(oStr.Utf8(), "\xFD\xBF\xBF\xBF\xBF\xBF""bca\xFD\xBF\xBF\xBF\xBF\xBF""cab\xFD\xBF\xBF\xBF\xBF\xBF") == 0);
	
	oStr.Set("\xFD\xBF\xBF\xBF\xBF\xBF""bca\xFD\xBF\xBF\xBF\xBF\xBF""cab\xFD\xBF\xBF\xBF\xBF\xBF");
	oPos.Back(4);
	oStr.CutRight(oPos, false);
	Test(oStr.ByteLength() == 9);
	Test(oStr.Length() == 4);
	Test(StringCompare(oStr.Utf8(), "\xFD\xBF\xBF\xBF\xBF\xBF""bca") == 0);
	
	oStr.Set("\xFD\xBF\xBF\xBF\xBF\xBF""bca\xFD\xBF\xBF\xBF\xBF\xBF""cab\xFD\xBF\xBF\xBF\xBF\xBF");
	oPos.First();
	oStr.CutRight(oPos, true);
	Test(oStr.ByteLength() == 6);
	Test(oStr.Length() == 1);
	Test(StringCompare(oStr.Utf8(), "\xFD\xBF\xBF\xBF\xBF\xBF") == 0);
	
	oStr.Set("\xFD\xBF\xBF\xBF\xBF\xBF""bca\xFD\xBF\xBF\xBF\xBF\xBF""cab\xFD\xBF\xBF\xBF\xBF\xBF");
	oStr.CutRight(oPos, false);
	Test(oStr.ByteLength() == 0);
	Test(oStr.Length() == 0);
	Test(StringCompare(oStr.Utf8(), "") == 0);
	
	
	return 0;
}

static int TestTrim()
{
	PrintFn();
	
	
	acpl::String oStr;
	
	
	oStr.Trim();
	Test(oStr.ByteLength() == 0);
	Test(oStr.Length() == 0);
	Test(StringCompare(oStr.Utf8(), "") == 0);
	
	oStr.Set("").Trim();
	Test(oStr.ByteLength() == 0);
	Test(oStr.Length() == 0);
	Test(StringCompare(oStr.Utf8(), "") == 0);
	
	oStr.Set("\x20\xC2\xA0\xE2\x80\x82").Trim();
	Test(oStr.ByteLength() == 0);
	Test(oStr.Length() == 0);
	Test(StringCompare(oStr.Utf8(), "") == 0);
	
	oStr.Set("\x20\xC2\xA0\xE2\x80\x82""abc").Trim();
	Test(oStr.ByteLength() == 3);
	Test(oStr.Length() == 3);
	Test(StringCompare(oStr.Utf8(), "abc") == 0);
	
	oStr.Set("\x20\xC2\xA0\xE2\x80\x82""abc\x20\xC2\xA0\xE2\x80\x82").Trim();
	Test(oStr.ByteLength() == 3);
	Test(oStr.Length() == 3);
	Test(StringCompare(oStr.Utf8(), "abc") == 0);
	
	oStr.Set("abc\x20\xC2\xA0\xE2\x80\x82").Trim();
	Test(oStr.ByteLength() == 3);
	Test(oStr.Length() == 3);
	Test(StringCompare(oStr.Utf8(), "abc") == 0);
	
	
	return 0;
}

static int TestTrimLeft()
{
	PrintFn();
	
	
	acpl::String oStr;
	
	
	oStr.TrimLeft();
	Test(oStr.ByteLength() == 0);
	Test(oStr.Length() == 0);
	Test(StringCompare(oStr.Utf8(), "") == 0);
	
	oStr.Set("").TrimLeft();
	Test(oStr.ByteLength() == 0);
	Test(oStr.Length() == 0);
	Test(StringCompare(oStr.Utf8(), "") == 0);
	
	oStr.Set("\x20\xC2\xA0\xE2\x80\x82").TrimLeft();
	Test(oStr.ByteLength() == 0);
	Test(oStr.Length() == 0);
	Test(StringCompare(oStr.Utf8(), "") == 0);
	
	oStr.Set("\x20\xC2\xA0\xE2\x80\x82""abc").TrimLeft();
	Test(oStr.ByteLength() == 3);
	Test(oStr.Length() == 3);
	Test(StringCompare(oStr.Utf8(), "abc") == 0);
	
	oStr.Set("\x20\xC2\xA0\xE2\x80\x82""abc\x20\xC2\xA0\xE2\x80\x82").TrimLeft();
	Test(oStr.ByteLength() == 9);
	Test(oStr.Length() == 6);
	Test(StringCompare(oStr.Utf8(), "abc\x20\xC2\xA0\xE2\x80\x82") == 0);
	
	oStr.Set("abc\x20\xC2\xA0\xE2\x80\x82").TrimLeft();
	Test(oStr.ByteLength() == 9);
	Test(oStr.Length() == 6);
	Test(StringCompare(oStr.Utf8(), "abc\x20\xC2\xA0\xE2\x80\x82") == 0);
	
	
	return 0;
}

static int TestTrimRight()
{
	PrintFn();
	
	
	acpl::String oStr;
	
	
	oStr.TrimRight();
	Test(oStr.ByteLength() == 0);
	Test(oStr.Length() == 0);
	Test(StringCompare(oStr.Utf8(), "") == 0);
	
	oStr.Set("").TrimRight();
	Test(oStr.ByteLength() == 0);
	Test(oStr.Length() == 0);
	Test(StringCompare(oStr.Utf8(), "") == 0);
	
	oStr.Set("\x20\xC2\xA0\xE2\x80\x82").TrimRight();
	Test(oStr.ByteLength() == 0);
	Test(oStr.Length() == 0);
	Test(StringCompare(oStr.Utf8(), "") == 0);
	
	oStr.Set("\x20\xC2\xA0\xE2\x80\x82""abc").TrimRight();
	Test(oStr.ByteLength() == 9);
	Test(oStr.Length() == 6);
	Test(StringCompare(oStr.Utf8(), "\x20\xC2\xA0\xE2\x80\x82""abc") == 0);
	
	oStr.Set("\x20\xC2\xA0\xE2\x80\x82""abc\x20\xC2\xA0\xE2\x80\x82").TrimRight();
	Test(oStr.ByteLength() == 9);
	Test(oStr.Length() == 6);
	Test(StringCompare(oStr.Utf8(), "\x20\xC2\xA0\xE2\x80\x82""abc") == 0);
	
	oStr.Set("abc\x20\xC2\xA0\xE2\x80\x82").TrimRight();
	Test(oStr.ByteLength() == 3);
	Test(oStr.Length() == 3);
	Test(StringCompare(oStr.Utf8(), "abc") == 0);
	
	
	return 0;
}

static int TestToUnicodeStrs()
{
	PrintFn();
	
	
	acpl::String oStr;
	acpl::Mem::Block<char>::Type oUtf8Data;
	acpl::Mem::Block<wchar_t>::Type oWcsData;
	acpl::Mem::Block<acpl::Unichar>::Type oUcsData;
	
	
	// Empty
	oStr.ToUtf8(oUtf8Data);
	oStr.ToWcs(oWcsData);
	oStr.ToUcs(oUcsData);
	
	Test(oUtf8Data.Length() == 1);
	Test(oUtf8Data.Data()[0] == 0);
	
	Test(oWcsData.Length() == 1);
	Test(oWcsData.Data()[0] == 0);
	
	Test(oUcsData.Length() == 1);
	Test(oUcsData.Data()[0] == 0);
	
	
	// Simple "abc"
	oStr.Set("abc");
	oStr.ToUtf8(oUtf8Data);
	oStr.ToWcs(oWcsData);
	oStr.ToUcs(oUcsData);
	
	Test(oUtf8Data.Length() == 4);
	Test(oUtf8Data.Data()[0] == 'a');
	Test(oUtf8Data.Data()[1] == 'b');
	Test(oUtf8Data.Data()[2] == 'c');
	Test(oUtf8Data.Data()[3] == '\0');
	
	Test(oWcsData.Length() == 4);
	Test(oWcsData.Data()[0] == L'a');
	Test(oWcsData.Data()[1] == L'b');
	Test(oWcsData.Data()[2] == L'c');
	Test(oWcsData.Data()[3] == L'\0');
	
	Test(oUcsData.Length() == 4);
	Test(oUcsData.Data()[0] == 'a');
	Test(oUcsData.Data()[1] == 'b');
	Test(oUcsData.Data()[2] == 'c');
	Test(oUcsData.Data()[3] == '\0');
	
	
	// Multi-byte sequences test
	static const acpl::Unichar oUcMbSeqs[] = { 
			0x0000007F, // Max for 1-byte UTF-8 seqence
			0x000007FF, // Max for 2-byte UTF-8 seqence
			
			0x0000D7FF, // Before first invalid UTF-16 seqence surrogate
			0x0000D800, // First invalid UTF-16 seqence surrogate
			0x0000DFFF, // Last invalid UTF-16 seqence surrogate
			0x0000E000, // After last invalid UTF-16 seqence surrogate
			
			0x0000FFFF, // Max for 3-byte UTF-8 seqence and last single char for UTF-16 seqence
			0x00010000, // First double char UTF-16 seqence
			
			0x0010FFFF, // Last valid UTF-16 seqence
			0x00110000, // After last valid UTF-16 seqence
			
			0x001FFFFF, // Max for 4-byte UTF-8 seqence
			0x03FFFFFF, // Max for 5-byte UTF-8 seqence
			0x7FFFFFFF, // Max for 6-byte UTF-8 seqence
			
			0x80000000  // After max UTF-8 seqence
		};
	
	oStr.Set(oUcMbSeqs);
	oStr.ToUtf8(oUtf8Data);
	oStr.ToWcs(oWcsData);
	oStr.ToUcs(oUcsData);
	
	Test(oUtf8Data.Length() == 47);
	Test(_local_TestStringData(oUtf8Data,
			"\x7F"
			"\xDF\xBF"
			
			"\xED\x9F\xBF"
			"\xED\xA0\x80"
			"\xED\xBF\xBF"
			"\xEE\x80\x80"
			
			"\xEF\xBF\xBF"
			"\xF0\x90\x80\x80"
			
			"\xF4\x8F\xBF\xBF"
			"\xF4\x90\x80\x80"
			
			"\xF7\xBF\xBF\xBF"
			"\xFB\xBF\xBF\xBF\xBF"
			"\xFD\xBF\xBF\xBF\xBF\xBF"
			
			"?" // NULL terminator is implicit
		) == 0);
	
	static const wchar_t oConfirmWc[] = {
#if   (acplCRuntime == acplCRuntimeGlibc)
			0x0000007F,
			0x000007FF,
			
			0x0000D7FF,
			0x0000D800,
			0x0000DFFF,
			0x0000E000,
			
			0x0000FFFF,
			0x00010000,
			
			0x0010FFFF,
			0x00110000,
			
			0x001FFFFF,
			0x03FFFFFF,
			0x7FFFFFFF,
			
			L'?',
			0x00 // NULL terminator
#elif (acplCRuntime == acplCRuntimeMscrt)
			0x007F,
			0x07FF,
			
			0xD7FF,
			L'?',
			L'?',
			0xE000,
			
			0xFFFF,
			0xD800, 0xDC00,
			
			0xDBFF, 0xDFFF,
			L'?',
			
			L'?',
			L'?',
			L'?',
			
			L'?',
			0x00 // NULL terminator
#else
#	error Add platform specific tests.
#endif
		};
	Test(oWcsData.Length() == (sizeof(oConfirmWc) / sizeof(wchar_t)));
	Test(_local_TestStringData(oWcsData, oConfirmWc) == 0);
	
	static const acpl::Unichar oConfirmUc[] = { 
			0x0000007F,
			0x000007FF,
			
			0x0000D7FF,
			0x0000D800,
			0x0000DFFF,
			0x0000E000,
			
			0x0000FFFF,
			0x00010000,
			
			0x0010FFFF,
			0x00110000,
			
			0x001FFFFF,
			0x03FFFFFF,
			0x7FFFFFFF,
			
			'?',
			0x00000000
		};
	Test(oUcsData.Length() == 15);
	Test(_local_TestStringData(oUcsData, oConfirmUc) == 0);
	
	
	return 0;
}

static int TestToFromMbs()
{
	PrintFn();
	
	
	acpl::String oStr;
	acpl::Mem::Block<char>::Type oInput;
	acpl::Mem::Block<char>::Type oOutput;
	
	
	// Fill input with 255-0 values
	for (char i = 0; --i != 0; )
		oInput.Apnd(&i, 1);
	oInput.Apnd("", 1);
	
	// Set to string as current locale (nLocale == true)
	oStr.Set(acpl::String::Ref(oInput.Data(), true));
	oStr.ToMbs(oOutput);
	
	// Check for differences
	for (acpl::SizeT i = 0; i < oOutput.Length(); i++)
	{
		if (oOutput.Data()[i] != '?')
			TestFM(oOutput.Data()[i] == oInput.Data()[i], "i=%lu; 0x%02X != 0x%02X", i, oOutput.Data()[i], oInput.Data()[i]);
	}
	
	
	return 0;
}

static int TestToInt()
{
	PrintFn();
	
	
	acpl::String oStr;
	
	
	// 
	// Limits
	// 
	
	// SInt8
	{
		acpl::SInt8 oInt;
		
		Test(oStr.Set("0x00").ToInt(oInt, 16) == true);
		Test(oInt == static_cast<acpl::SInt8 >(0x00));
		Test(oStr.Set("0x7F").ToInt(oInt, 16) == true);
		Test(oInt == static_cast<acpl::SInt8 >(0x7F));
		Test(oStr.Set("0x80").ToInt(oInt, 16) == true);
		Test(oInt == static_cast<acpl::SInt8 >(0x80));
		Test(oStr.Set("0xFF").ToInt(oInt, 16) == true);
		Test(oInt == static_cast<acpl::SInt8 >(0xFF));
		Test(oStr.Set("0x1FF").ToInt(oInt, 16) == false);
		Test(oStr.Set("0xFFF").ToInt(oInt, 16) == false);
		
		Test(oStr.Set("0").ToInt(oInt, 10) == true);
		Test(oInt == 0);
		Test(oStr.Set("127").ToInt(oInt, 10) == true);
		Test(oInt == 127);
		Test(oStr.Set("-128").ToInt(oInt, 10) == true);
		Test(oInt == -128);
		Test(oStr.Set("128").ToInt(oInt, 10) == false);
		Test(oStr.Set("-129").ToInt(oInt, 10) == false);
	}
	
	// SInt16
	{
		acpl::SInt16 oInt;
		
		Test(oStr.Set("0x0000").ToInt(oInt, 16) == true);
		Test(oInt == static_cast<acpl::SInt16>(0x0000));
		Test(oStr.Set("0x7FFF").ToInt(oInt, 16) == true);
		Test(oInt == static_cast<acpl::SInt16>(0x7FFF));
		Test(oStr.Set("0x8000").ToInt(oInt, 16) == true);
		Test(oInt == static_cast<acpl::SInt16>(0x8000));
		Test(oStr.Set("0xFFFF").ToInt(oInt, 16) == true);
		Test(oInt == static_cast<acpl::SInt16>(0xFFFF));
		Test(oStr.Set("0x1FFFF").ToInt(oInt, 16) == false);
		Test(oStr.Set("0xFFFFF").ToInt(oInt, 16) == false);
		
		Test(oStr.Set("0").ToInt(oInt, 10) == true);
		Test(oInt == 0);
		Test(oStr.Set("32767").ToInt(oInt, 10) == true);
		Test(oInt == 32767);
		Test(oStr.Set("-32768").ToInt(oInt, 10) == true);
		Test(oInt == -32768);
		Test(oStr.Set("32768").ToInt(oInt, 10) == false);
		Test(oStr.Set("-32769").ToInt(oInt, 10) == false);
	}
	
	// SInt32
	{
		acpl::SInt32 oInt;
		
		Test(oStr.Set("0x00000000").ToInt(oInt, 16) == true);
		Test(oInt == static_cast<acpl::SInt32>(0x00000000));
		Test(oStr.Set("0x7FFFFFFF").ToInt(oInt, 16) == true);
		Test(oInt == static_cast<acpl::SInt32>(0x7FFFFFFF));
		Test(oStr.Set("0x80000000").ToInt(oInt, 16) == true);
		Test(oInt == static_cast<acpl::SInt32>(0x80000000));
		Test(oStr.Set("0xFFFFFFFF").ToInt(oInt, 16) == true);
		Test(oInt == static_cast<acpl::SInt32>(0xFFFFFFFF));
		Test(oStr.Set("0x1FFFFFFFF").ToInt(oInt, 16) == false);
		Test(oStr.Set("0xFFFFFFFFF").ToInt(oInt, 16) == false);
		
		Test(oStr.Set("0").ToInt(oInt, 10) == true);
		Test(oInt == 0);
		Test(oStr.Set("2147483647").ToInt(oInt, 10) == true);
		Test(oInt == 2147483647);
		Test(oStr.Set("-2147483648").ToInt(oInt, 10) == true);
		Test(oInt == -2147483648);
		Test(oStr.Set("2147483648").ToInt(oInt, 10) == false);
		Test(oStr.Set("-2147483649").ToInt(oInt, 10) == false);
	}
	
	// SInt64
	{
		acpl::SInt64 oInt;
		
		Test(oStr.Set("0x0000000000000000").ToInt(oInt, 16) == true);
		Test(oInt == static_cast<acpl::SInt64>(acpl::Const::UI64(0x00000000, 0x00000000)));
		Test(oStr.Set("0x7FFFFFFFFFFFFFFF").ToInt(oInt, 16) == true);
		Test(oInt == static_cast<acpl::SInt64>(acpl::Const::UI64(0x7FFFFFFF, 0xFFFFFFFF)));
		Test(oStr.Set("0x8000000000000000").ToInt(oInt, 16) == true);
		Test(oInt == static_cast<acpl::SInt64>(acpl::Const::UI64(0x80000000, 0x00000000)));
		Test(oStr.Set("0xFFFFFFFFFFFFFFFF").ToInt(oInt, 16) == true);
		Test(oInt == static_cast<acpl::SInt64>(acpl::Const::UI64(0xFFFFFFFF, 0xFFFFFFFF)));
		Test(oStr.Set("0x1FFFFFFFFFFFFFFFF").ToInt(oInt, 16) == false);
		Test(oStr.Set("0xFFFFFFFFFFFFFFFFF").ToInt(oInt, 16) == false);
		
		Test(oStr.Set("0").ToInt(oInt, 10) == true);
		Test(oInt == static_cast<acpl::SInt64>(acpl::Const::UI64(0x00000000, 0x00000000)));
		Test(oStr.Set("9223372036854775807").ToInt(oInt, 10) == true);
		Test(oInt == static_cast<acpl::SInt64>(acpl::Const::UI64(0x7FFFFFFF, 0xFFFFFFFF)));
		Test(oStr.Set("-9223372036854775808").ToInt(oInt, 10) == true);
		Test(oInt == static_cast<acpl::SInt64>(acpl::Const::UI64(0x80000000, 0x00000000)));
		Test(oStr.Set("9223372036854775808").ToInt(oInt, 10) == false);
		Test(oStr.Set("-9223372036854775809").ToInt(oInt, 10) == false);
	}
	
	// UInt8
	{
		acpl::UInt8 oInt;
		
		Test(oStr.Set("0x00").ToInt(oInt, 16) == true);
		Test(oInt == static_cast<acpl::UInt8 >(0x00));
		Test(oStr.Set("0x7F").ToInt(oInt, 16) == true);
		Test(oInt == static_cast<acpl::UInt8 >(0x7F));
		Test(oStr.Set("0x80").ToInt(oInt, 16) == true);
		Test(oInt == static_cast<acpl::UInt8 >(0x80));
		Test(oStr.Set("0xFF").ToInt(oInt, 16) == true);
		Test(oInt == static_cast<acpl::UInt8 >(0xFF));
		Test(oStr.Set("0x1FF").ToInt(oInt, 16) == false);
		Test(oStr.Set("0xFFF").ToInt(oInt, 16) == false);
		
		Test(oStr.Set("0").ToInt(oInt, 10) == true);
		Test(oInt == 0);
		Test(oStr.Set("127").ToInt(oInt, 10) == true);
		Test(oInt == 127);
		Test(oStr.Set("128").ToInt(oInt, 10) == true);
		Test(oInt == 128);
		Test(oStr.Set("255").ToInt(oInt, 10) == true);
		Test(oInt == 255);
		Test(oStr.Set("256").ToInt(oInt, 10) == false);
		Test(oStr.Set("-1").ToInt(oInt, 10) == false);
	}
	
	// UInt16
	{
		acpl::UInt16 oInt;
		
		Test(oStr.Set("0x0000").ToInt(oInt, 16) == true);
		Test(oInt == static_cast<acpl::UInt16>(0x0000));
		Test(oStr.Set("0x7FFF").ToInt(oInt, 16) == true);
		Test(oInt == static_cast<acpl::UInt16>(0x7FFF));
		Test(oStr.Set("0x8000").ToInt(oInt, 16) == true);
		Test(oInt == static_cast<acpl::UInt16>(0x8000));
		Test(oStr.Set("0xFFFF").ToInt(oInt, 16) == true);
		Test(oInt == static_cast<acpl::UInt16>(0xFFFF));
		Test(oStr.Set("0x1FFFF").ToInt(oInt, 16) == false);
		Test(oStr.Set("0xFFFFF").ToInt(oInt, 16) == false);
		
		Test(oStr.Set("0").ToInt(oInt, 10) == true);
		Test(oInt == 0);
		Test(oStr.Set("32767").ToInt(oInt, 10) == true);
		Test(oInt == 32767);
		Test(oStr.Set("32768").ToInt(oInt, 10) == true);
		Test(oInt == 32768);
		Test(oStr.Set("65535").ToInt(oInt, 10) == true);
		Test(oInt == 65535);
		Test(oStr.Set("65536").ToInt(oInt, 10) == false);
		Test(oStr.Set("-1").ToInt(oInt, 10) == false);
	}
	
	// UInt32
	{
		acpl::UInt32 oInt;
		
		Test(oStr.Set("0x00000000").ToInt(oInt, 16) == true);
		Test(oInt == static_cast<acpl::UInt32>(0x00000000));
		Test(oStr.Set("0x7FFFFFFF").ToInt(oInt, 16) == true);
		Test(oInt == static_cast<acpl::UInt32>(0x7FFFFFFF));
		Test(oStr.Set("0x80000000").ToInt(oInt, 16) == true);
		Test(oInt == static_cast<acpl::UInt32>(0x80000000));
		Test(oStr.Set("0xFFFFFFFF").ToInt(oInt, 16) == true);
		Test(oInt == static_cast<acpl::UInt32>(0xFFFFFFFF));
		Test(oStr.Set("0x1FFFFFFFF").ToInt(oInt, 16) == false);
		Test(oStr.Set("0xFFFFFFFFF").ToInt(oInt, 16) == false);
		
		Test(oStr.Set("0").ToInt(oInt, 10) == true);
		Test(oInt == 0);
		Test(oStr.Set("2147483647").ToInt(oInt, 10) == true);
		Test(oInt == 2147483647);
		Test(oStr.Set("2147483648").ToInt(oInt, 10) == true);
		Test(oInt == 2147483648);
		Test(oStr.Set("4294967295").ToInt(oInt, 10) == true);
		Test(oInt == 4294967295);
		Test(oStr.Set("4294967296").ToInt(oInt, 10) == false);
		Test(oStr.Set("-1").ToInt(oInt, 10) == false);
	}
	
	// UInt64
	{
		acpl::UInt64 oInt;
		
		Test(oStr.Set("0x0000000000000000").ToInt(oInt, 16) == true);
		Test(oInt == static_cast<acpl::UInt64>(acpl::Const::UI64(0x00000000, 0x00000000)));
		Test(oStr.Set("0x7FFFFFFFFFFFFFFF").ToInt(oInt, 16) == true);
		Test(oInt == static_cast<acpl::UInt64>(acpl::Const::UI64(0x7FFFFFFF, 0xFFFFFFFF)));
		Test(oStr.Set("0x8000000000000000").ToInt(oInt, 16) == true);
		Test(oInt == static_cast<acpl::UInt64>(acpl::Const::UI64(0x80000000, 0x00000000)));
		Test(oStr.Set("0xFFFFFFFFFFFFFFFF").ToInt(oInt, 16) == true);
		Test(oInt == static_cast<acpl::UInt64>(acpl::Const::UI64(0xFFFFFFFF, 0xFFFFFFFF)));
		Test(oStr.Set("0x1FFFFFFFFFFFFFFFF").ToInt(oInt, 16) == false);
		Test(oStr.Set("0xFFFFFFFFFFFFFFFFF").ToInt(oInt, 16) == false);
		
		Test(oStr.Set("0").ToInt(oInt, 10) == true);
		Test(oInt == static_cast<acpl::UInt64>(acpl::Const::UI64(0x00000000, 0x00000000)));
		Test(oStr.Set("9223372036854775807").ToInt(oInt, 10) == true);
		Test(oInt == static_cast<acpl::UInt64>(acpl::Const::UI64(0x7FFFFFFF, 0xFFFFFFFF)));
		Test(oStr.Set("9223372036854775808").ToInt(oInt, 10) == true);
		Test(oInt == static_cast<acpl::UInt64>(acpl::Const::UI64(0x80000000, 0x00000000)));
		Test(oStr.Set("18446744073709551615").ToInt(oInt, 10) == true);
		Test(oInt == static_cast<acpl::UInt64>(acpl::Const::UI64(0xFFFFFFFF, 0xFFFFFFFF)));
		Test(oStr.Set("18446744073709551616").ToInt(oInt, 10) == false);
		Test(oStr.Set("-1").ToInt(oInt, 10) == false);
	}
	
	
	// 
	// Different bases
	// 
	
	{
		acpl::UInt64 oUInt;
		acpl::SInt64 oSInt;
		
		oUInt = 0;
		Test(oStr.Set("100100011010001010110011110001001101010111100110111101111").ToInt( oUInt, 2) == true);
		Test( oUInt == acpl::Const::UI64(0x01234567, 0x89ABCDEF));
		oUInt = 0;
		Test(oStr.Set("10203101112132021222330313233").ToInt( oUInt, 4) == true);
		Test( oUInt == acpl::Const::UI64(0x01234567, 0x89ABCDEF));
		oUInt = 0;
		Test(oStr.Set("4432126361152746757").ToInt( oUInt, 8) == true);
		Test( oUInt == acpl::Const::UI64(0x01234567, 0x89ABCDEF));
		oUInt = 0;
		Test(oStr.Set("123456789ABCDEF").ToInt( oUInt, 16) == true);
		Test( oUInt == acpl::Const::UI64(0x01234567, 0x89ABCDEF));
		oUInt = 0;
		Test(oStr.Set("28Q5CU4QNJFF").ToInt( oUInt, 32) == true);
		Test( oUInt == acpl::Const::UI64(0x01234567, 0x89ABCDEF));
		oUInt = 0;
		Test(oStr.Set("MF9G063V08F").ToInt( oUInt, 36) == true);
		Test( oUInt == acpl::Const::UI64(0x01234567, 0x89ABCDEF));
		
		// Special base value 0
		oUInt = 0;
		Test(oStr.Set("04432126361152746757").ToInt( oUInt, 0) == true); // leading "0" suggests octal (8) base
		Test( oUInt == acpl::Const::UI64(0x01234567, 0x89ABCDEF));
		oUInt = 0;
		Test(oStr.Set("0x123456789ABCDEF").ToInt( oUInt, 0) == true); // leading "0x" suggests hexadecimal (16) base
		Test( oUInt == acpl::Const::UI64(0x01234567, 0x89ABCDEF));
		oUInt = 0;
		Test(oStr.Set("81985529216486895").ToInt( oUInt, 0) == true); // no leading suggests decimal (10) base
		Test( oUInt == acpl::Const::UI64(0x01234567, 0x89ABCDEF));
		// Test for signed int as well (oSInt)
		oSInt = 0;
		Test(oStr.Set("+81985529216486895").ToInt(oSInt, 0) == true); // leading "+" suggests decimal (10) base
		Test(oSInt == static_cast<acpl::SInt64>(acpl::Const::UI64(0x01234567, 0x89ABCDEF)));
		oSInt = 0;
		Test(oStr.Set("-81985529216486895").ToInt(oSInt, 0) == true); // leading "-" suggests decimal (10) base
		Test(oSInt == static_cast<acpl::SInt64>(acpl::Const::UI64(0xFEDCBA98, 0x76543211)));
		oSInt = 0;
		Test(oStr.Set(",81985529216486895").ToInt(oSInt, 0) == true); // leading "," suggests decimal (10) base
		Test(oSInt == static_cast<acpl::SInt64>(acpl::Const::UI64(0x01234567, 0x89ABCDEF)));
		
		// Invalid bases
		Test(oStr.Set("12345").ToInt( oUInt, 1) == false);
		Test(oStr.Set("12345").ToInt( oUInt, 37) == false);
	}
	
	
	// 
	// Digit groupping
	// 
	
	{
		acpl::SInt64 oSInt;
		
		// Normal (',' for digit groupping)
		oSInt = 0;
		Test(oStr.Set(",,8,,1,985,529,21,6486,89,5,,").ToInt(oSInt, 10, false) == true);
		Test(oSInt == static_cast<acpl::SInt64>(acpl::Const::UI64(0x01234567, 0x89ABCDEF)));
		oSInt = 0;
		Test(oStr.Set("+,,8,,1,985,529,21,6486,89,5,,").ToInt(oSInt, 10, false) == true);
		Test(oSInt == static_cast<acpl::SInt64>(acpl::Const::UI64(0x01234567, 0x89ABCDEF)));
		oSInt = 0;
		Test(oStr.Set("-,,8,,1,985,529,21,6486,89,5,,").ToInt(oSInt, 10, false) == true);
		Test(oSInt == static_cast<acpl::SInt64>(acpl::Const::UI64(0xFEDCBA98, 0x76543211)));
		
		// Swapped ('.' for digit groupping)
		oSInt = 0;
		Test(oStr.Set("..8..1.985.529.21.6486.89.5..").ToInt(oSInt, 10, true) == true);
		Test(oSInt == static_cast<acpl::SInt64>(acpl::Const::UI64(0x01234567, 0x89ABCDEF)));
		oSInt = 0;
		Test(oStr.Set("+..8..1.985.529.21.6486.89.5..").ToInt(oSInt, 10, true) == true);
		Test(oSInt == static_cast<acpl::SInt64>(acpl::Const::UI64(0x01234567, 0x89ABCDEF)));
		oSInt = 0;
		Test(oStr.Set("-..8..1.985.529.21.6486.89.5..").ToInt(oSInt, 10, true) == true);
		Test(oSInt == static_cast<acpl::SInt64>(acpl::Const::UI64(0xFEDCBA98, 0x76543211)));
	}
	
	
	// 
	// Floating-point types
	// NOTE: Due to the value approximation of floating-point types, the limits
	//       cannot be tested
	// 
	
	// Float32
	{
		acpl::Float32 oFloat;
		
		Test(oStr.Set("0x1234").ToInt(oFloat, 16) == true);
		Test(oFloat == 4660.0);
		Test(oStr.Set("1234").ToInt(oFloat, 10) == true);
		Test(oFloat == 1234.0);
		Test(oStr.Set("-1234").ToInt(oFloat, 10) == true);
		Test(oFloat == -1234.0);
	}
	
	// Float64
	{
		acpl::Float64 oFloat;
		
		Test(oStr.Set("0x1234").ToInt(oFloat, 16) == true);
		Test(oFloat == 4660.0);
		Test(oStr.Set("1234").ToInt(oFloat, 10) == true);
		Test(oFloat == 1234.0);
		Test(oStr.Set("-1234").ToInt(oFloat, 10) == true);
		Test(oFloat == -1234.0);
	}
	
	
	return 0;
}

static int TestToFloat()
{
	PrintFn();
	
	
	acpl::String oStr;
	acpl::Float64 oFp;
	acpl::Float::Largest oFpExt;
	
	
	// Whole numbers only
	oStr.Set("123456789").ToFloat(oFp);
	Test(_local_CmpFloat(oFp,  123456789.0, 0) == true);
	oStr.Set("+123456789").ToFloat(oFp);
	Test(_local_CmpFloat(oFp,  123456789.0, 0) == true);
	oStr.Set("-123456789").ToFloat(oFp);
	Test(_local_CmpFloat(oFp, -123456789.0, 0) == true);
	
	// Whole numbers only (with a decimal point, but no decimal places)
	oStr.Set("123456789.").ToFloat(oFp);
	Test(_local_CmpFloat(oFp,  123456789.0, 0) == true);
	oStr.Set("+123456789.").ToFloat(oFp);
	Test(_local_CmpFloat(oFp,  123456789.0, 0) == true);
	oStr.Set("-123456789.").ToFloat(oFp);
	Test(_local_CmpFloat(oFp, -123456789.0, 0) == true);
	
	// Whole numbers (with a decimal point and only '0' for decimal places)
	oStr.Set("123456789.000000000").ToFloat(oFp);
	Test(_local_CmpFloat(oFp,  123456789.0, 0) == true);
	oStr.Set("+123456789.000000000").ToFloat(oFp);
	Test(_local_CmpFloat(oFp,  123456789.0, 0) == true);
	oStr.Set("-123456789.000000000").ToFloat(oFp);
	Test(_local_CmpFloat(oFp, -123456789.0, 0) == true);
	
	// Whole + fraction numbers (a tolerance of 1 is needed because of MSVC/valgrind)
	oStr.Set("123456789.987654321").ToFloat(oFp);
	Test(_local_CmpFloat(oFp,  123456789.987654321, _local_FloatTolSwitch(1)) == true);
	oStr.Set("+123456789.987654321").ToFloat(oFp);
	Test(_local_CmpFloat(oFp,  123456789.987654321, _local_FloatTolSwitch(1)) == true);
	oStr.Set("-123456789.987654321").ToFloat(oFp);
	Test(_local_CmpFloat(oFp, -123456789.987654321, _local_FloatTolSwitch(1)) == true);
	
	// Fraction numbers only (with a leading zero before decimal point)
	oStr.Set("0.987654321").ToFloat(oFp);
	Test(_local_CmpFloat(oFp,  0.987654321, 0) == true);
	oStr.Set("+0.987654321").ToFloat(oFp);
	Test(_local_CmpFloat(oFp,  0.987654321, 0) == true);
	oStr.Set("-0.987654321").ToFloat(oFp);
	Test(_local_CmpFloat(oFp, -0.987654321, 0) == true);
	
	// Fraction numbers only (without a leading zero before decimal point)
	oStr.Set(".987654321").ToFloat(oFp);
	Test(_local_CmpFloat(oFp,  0.987654321, 0) == true);
	oStr.Set("+.987654321").ToFloat(oFp);
	Test(_local_CmpFloat(oFp,  0.987654321, 0) == true);
	oStr.Set("-.987654321").ToFloat(oFp);
	Test(_local_CmpFloat(oFp, -0.987654321, 0) == true);
	
	// Fraction numbers + positive exponent
	oStr.Set(".987654321e5").ToFloat(oFp);
	Test(_local_CmpFloat(oFp,  98765.4321, 0) == true);
	oStr.Set("+.987654321e5").ToFloat(oFp);
	Test(_local_CmpFloat(oFp,  98765.4321, 0) == true);
	oStr.Set("-.987654321e5").ToFloat(oFp);
	Test(_local_CmpFloat(oFp, -98765.4321, 0) == true);
	
	// Fraction numbers + positive exponent (with a '+' sign)
	oStr.Set(".987654321e+5").ToFloat(oFp);
	Test(_local_CmpFloat(oFp,  98765.4321, 0) == true);
	oStr.Set("+.987654321e+5").ToFloat(oFp);
	Test(_local_CmpFloat(oFp,  98765.4321, 0) == true);
	oStr.Set("-.987654321e+5").ToFloat(oFp);
	Test(_local_CmpFloat(oFp, -98765.4321, 0) == true);
	
	// Fraction numbers + negative exponent
	oStr.Set(".987654321e-5").ToFloat(oFp);
	Test(_local_CmpFloat(oFp,  0.00000987654321, 0) == true);
	oStr.Set("+.987654321e-5").ToFloat(oFp);
	Test(_local_CmpFloat(oFp,  0.00000987654321, 0) == true);
	oStr.Set("-.987654321e-5").ToFloat(oFp);
	Test(_local_CmpFloat(oFp, -0.00000987654321, 0) == true);
	
	// Whole numbers + positive exponent
	oStr.Set("123456789e+5").ToFloat(oFp);
	Test(_local_CmpFloat(oFp,  12345678900000.0, 0) == true);
	oStr.Set("+123456789e+5").ToFloat(oFp);
	Test(_local_CmpFloat(oFp,  12345678900000.0, 0) == true);
	oStr.Set("-123456789e+5").ToFloat(oFp);
	Test(_local_CmpFloat(oFp, -12345678900000.0, 0) == true);
	
	// Whole numbers + negative exponent (a tolerance of 1 is needed because of MSVC/valgrind)
	oStr.Set("123456789e-5").ToFloat(oFp);
	Test(_local_CmpFloat(oFp,  1234.56789, _local_FloatTolSwitch(1)) == true);
	oStr.Set("+123456789e-5").ToFloat(oFp);
	Test(_local_CmpFloat(oFp,  1234.56789, _local_FloatTolSwitch(1)) == true);
	oStr.Set("-123456789e-5").ToFloat(oFp);
	Test(_local_CmpFloat(oFp, -1234.56789, _local_FloatTolSwitch(1)) == true);
	
	// Huge positive exponent (for Float64) (a tolerance of 3 is needed because of MSVC/valgrind)
	oStr.Set("+1e+100").ToFloat(oFp);
	Test(_local_CmpFloat(oFp,  10000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000.0, _local_FloatTolSwitch(3)) == true);
	oStr.Set("-1e+100").ToFloat(oFp);
	Test(_local_CmpFloat(oFp, -10000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000.0, _local_FloatTolSwitch(3)) == true);
	
	// Huge negative exponent (for Float64) (a tolerance of 6 is needed because of MSVC/valgrind)
	oStr.Set("+1e-100").ToFloat(oFp);
	Test(_local_CmpFloat(oFp,  0.0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001, _local_FloatTolSwitch(6)) == true);
	oStr.Set("-1e-100").ToFloat(oFp);
	Test(_local_CmpFloat(oFp, -0.0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000001, _local_FloatTolSwitch(6)) == true);
	
	// Exponent just under maximum / over minimum (for Float64)
	oStr.Set("+1e+308").ToFloat(oFp);
	Test(oFp < acpl::Float::Max<acpl::Float64>());
	oStr.Set("-1e+308").ToFloat(oFp);
	Test(oFp > acpl::Float::Min<acpl::Float64>());
	
	// Exponent over maximum / under minimum (+/- infinity; for Float64)
	oStr.Set("+1e+309").ToFloat(oFp);
	Test(oFp == acpl::Float::Inf<acpl::Float64>());
	oStr.Set("-1e+309").ToFloat(oFp);
	Test(oFp == acpl::Float::Inf<acpl::Float64>() * -1);
	
	// Exponent just in precision scope (for Float64)
	oStr.Set("+1e-323").ToFloat(oFp);
	Test(oFp > acpl::Float::FractMin<acpl::Float64>());
	oStr.Set("-1e-323").ToFloat(oFp);
	Test(oFp < acpl::Float::FractMin<acpl::Float64>() * -1);
	
	// Exponent out of precision scope (for Float64)
	oStr.Set("+1e-324").ToFloat(oFp);
	Test(oFp == 0.0);
	oStr.Set("-1e-324").ToFloat(oFp);
	Test(oFp == 0.0);
	
	
	// NOTE: The following tests do not work in environments where x86
	//       extended precision floating-point type is not available. Such
	//       environment is under MSVC or running the test program using
	//       `valgrind` tool, hence the _local_SkipFloatExtPrec() call.
	
	// Exponent just under maximum / over minimum (for x86 extended precision `long double`)
	oStr.Set("+1e+4932").ToFloat(oFpExt);
	Test(oFpExt < acpl::Float::Max<acpl::Float::Largest>() || _local_SkipFloatExtPrec() == true);
	oStr.Set("-1e+4932").ToFloat(oFpExt);
	Test(oFpExt > acpl::Float::Min<acpl::Float::Largest>() || _local_SkipFloatExtPrec() == true);
	
	// Exponent over maximum / under minimum (+/- infinity; for x86 extended precision `long double`)
	oStr.Set("+1e+4933").ToFloat(oFpExt);
	Test(oFpExt == acpl::Float::Inf<acpl::Float::Largest>() || _local_SkipFloatExtPrec() == true);
	oStr.Set("-1e+4933").ToFloat(oFpExt);
	Test(oFpExt == acpl::Float::Inf<acpl::Float::Largest>() * -1 || _local_SkipFloatExtPrec() == true);
	
	// Exponent just in precision scope (for x86 extended precision `long double`)
	oStr.Set("+1e-4950").ToFloat(oFpExt);
	Test(oFpExt > acpl::Float::FractMin<acpl::Float::Largest>() || _local_SkipFloatExtPrec() == true);
	oStr.Set("-1e-4950").ToFloat(oFpExt);
	Test(oFpExt < acpl::Float::FractMin<acpl::Float::Largest>() * -1 || _local_SkipFloatExtPrec() == true);
	
	// Exponent out of precision scope (for x86 extended precision `long double`)
	oStr.Set("+1e-4951").ToFloat(oFpExt);
	Test(oFpExt == 0.0); // No need for HasExtPrec() check
	oStr.Set("-1e-4951").ToFloat(oFpExt);
	Test(oFpExt == 0.0); // No need for HasExtPrec() check
	
	
	return 0;
}



SectionFuncMain(string)
{
	if (argc < 1)
	{
		Test(TestUtf8Funcs() == 0);
		Test(TestRef() == 0);
		Test(TestIntRefType() == 0);
		Test(TestIntRef() == 0);
		Test(TestUnicharRef() == 0);
		Test(TestPtrRef() == 0);
		Test(TestIpAddrRef() == 0);
		Test(TestNetAddrRef() == 0);
		Test(TestRefMeasureConvertSelf() == 0);
		Test(TestRefMeasureConvertUtf8() == 0);
		Test(TestRefMeasureConvertMbs() == 0);
		Test(TestRefMeasureConvertWcs() == 0);
		Test(TestRefMeasureConvertUcs() == 0);
		// Other Ref classes
		Test(TestPosAttach() == 0);
		Test(TestPosMove() == 0);
		Test(TestPosMoveUnichar() == 0);
		Test(TestPosMoveSubstr() == 0);
		Test(TestRefList() == 0);
		Test(TestFmtRef() == 0);
		Test(TestFmtDtRef() == 0);
		Test(TestInit() == 0);
		Test(TestIsAllSpaces() == 0);
		Test(TestSetFromRef() == 0);
		Test(TestSetFromPos() == 0);
		Test(TestApndFromRef() == 0);
		Test(TestApndFromPos() == 0);
		Test(TestInsFromRef() == 0);
		Test(TestInsFromPos() == 0);
		Test(TestReplFromRef() == 0);
		Test(TestReplFromPos() == 0);
		Test(TestReplFromPosToPos() == 0);
		Test(TestPadLeft() == 0);
		Test(TestPadRight() == 0);
		Test(TestCutIndexCount() == 0);
		Test(TestCutPosCount() == 0);
		Test(TestCutPosPos() == 0);
		Test(TestCutLeftCount() == 0);
		Test(TestCutLeftPos() == 0);
		Test(TestCutRightCount() == 0);
		Test(TestCutRightPos() == 0);
		Test(TestTrim() == 0);
		Test(TestTrimLeft() == 0);
		Test(TestTrimRight() == 0);
		Test(TestToUnicodeStrs() == 0);
		Test(TestToFromMbs() == 0);
		Test(TestToInt() == 0);
		Test(TestToFloat() == 0);
		
		PrintOut("All Passed!\n");
	}
	else
		return PrintUnrecParams(acplDbgSrcLoc, lSectionName, argc, argv);
	
	return 0;
}

SectionFuncParamsBegin(string)
// SectionFuncParamsAdd("param [param [param ...]]", "description")
SectionFuncParamsEnd(string)
