#include "Tests.h"

#include "../src/net/Address.h"


class _local_NetworkAddrExt : public acpl::NetworkAddr
{
	public:
		static inline acpl::NetworkAddr::Type CIdentify(const char *nBegPtr, const char *nEndPtr) { return acpl::NetworkAddr::Identify(nBegPtr, nEndPtr); }
};

class _local_Ipv4AddrExt : public acpl::Ipv4Addr
{
	public:
		static inline bool CValidate(const char *nBegPtr, const char *nEndPtr) { return acpl::Ipv4Addr::Parse(nBegPtr, nEndPtr, NULL); }
		inline bool CParse(const char *nBegPtr, const char *nEndPtr) { return acpl::Ipv4Addr::Parse(nBegPtr, nEndPtr, this); }
};

class _local_Ipv6AddrExt : public acpl::Ipv6Addr
{
	public:
		static inline bool CValidate(const char *nBegPtr, const char *nEndPtr) { return acpl::Ipv6Addr::Parse(nBegPtr, nEndPtr, NULL); }
		inline bool CParse(const char *nBegPtr, const char *nEndPtr) { return acpl::Ipv6Addr::Parse(nBegPtr, nEndPtr, this); }
};

class _local_DomainNameExt : public acpl::DomainName
{
	public:
		static inline bool CValidate(const char *nBegPtr, const char *nEndPtr) { return acpl::DomainName::Parse(nBegPtr, nEndPtr, NULL); }
		inline bool CParse(const char *nBegPtr, const char *nEndPtr) { return acpl::DomainName::Parse(nBegPtr, nEndPtr, this); }
};

template <class tType>
static inline bool _local_IsIpvReset(const tType &nIpv)
{
	acpl::SizeT nByteCount = sizeof(typename tType::Type);
	
	while (nByteCount--)
		if (nIpv.Data()[nByteCount] != 0)
			return false;
	
	return true;
}

template <class tType>
static inline void _local_PrintIpvBytes(const tType &nIpv)
{
	for (acpl::SizeT i = 0; i < sizeof(typename tType::Type); i++)
	{
		if (i != 0)
			PrintOut("-");
		
		PrintOut("%02X", nIpv.Data()[i]);
	}
	
	PrintOut("\n");
}

static inline bool _local_CmpIpv4(const acpl::Ipv4Addr &nAddr, acpl::UInt8 nByte1, acpl::UInt8 nByte2, acpl::UInt8 nByte3, acpl::UInt8 nByte4)
{
	return (
		nAddr.Data()[0] == nByte1 &&
		nAddr.Data()[1] == nByte2 &&
		nAddr.Data()[2] == nByte3 &&
		nAddr.Data()[3] == nByte4);
}

static inline bool _local_CmpIpv6(const acpl::Ipv6Addr &nAddr, acpl::UInt8 nGrpOff, acpl::UInt16 nGrpVal)
{
	return (
		nAddr.Data()[(nGrpOff * 2) + 0] == static_cast<acpl::UInt8>(nGrpVal >> 8) &&
		nAddr.Data()[(nGrpOff * 2) + 1] == static_cast<acpl::UInt8>(nGrpVal & 0xFF));
}

static inline bool _local_CmpIpv6(const acpl::Ipv6Addr &nAddr, acpl::UInt16 nGrp1, acpl::UInt16 nGrp2, acpl::UInt16 nGrp3, acpl::UInt16 nGrp4, acpl::UInt16 nGrp5, acpl::UInt16 nGrp6)
{
	return (
		_local_CmpIpv6(nAddr, 0, nGrp1) == true &&
		_local_CmpIpv6(nAddr, 1, nGrp2) == true &&
		_local_CmpIpv6(nAddr, 2, nGrp3) == true &&
		_local_CmpIpv6(nAddr, 3, nGrp4) == true &&
		_local_CmpIpv6(nAddr, 4, nGrp5) == true &&
		_local_CmpIpv6(nAddr, 5, nGrp6) == true);
}

static inline bool _local_CmpIpv6(const acpl::Ipv6Addr &nAddr, acpl::UInt16 nGrp1, acpl::UInt16 nGrp2, acpl::UInt16 nGrp3, acpl::UInt16 nGrp4, acpl::UInt16 nGrp5, acpl::UInt16 nGrp6, acpl::UInt16 nGrp7, acpl::UInt16 nGrp8)
{
	return (
		_local_CmpIpv6(nAddr, nGrp1, nGrp2, nGrp3, nGrp4, nGrp5, nGrp6) == true &&
		_local_CmpIpv6(nAddr, 6, nGrp7) == true &&
		_local_CmpIpv6(nAddr, 7, nGrp8));
}

static inline bool _local_CmpIpv6(const acpl::Ipv6Addr &nAddr, acpl::UInt16 nGrp1, acpl::UInt16 nGrp2, acpl::UInt16 nGrp3, acpl::UInt16 nGrp4, acpl::UInt16 nGrp5, acpl::UInt16 nGrp6, acpl::UInt8 nGrp7Byte1, acpl::UInt8 nGrp7Byte2, acpl::UInt8 nGrp8Byte1, acpl::UInt8 nGrp8Byte2)
{
	return (
		_local_CmpIpv6(nAddr, nGrp1, nGrp2, nGrp3, nGrp4, nGrp5, nGrp6) == true &&
		nAddr.Data()[12] == nGrp7Byte1 &&
		nAddr.Data()[13] == nGrp7Byte2 &&
		nAddr.Data()[14] == nGrp8Byte1 &&
		nAddr.Data()[15] == nGrp8Byte2);
}

static inline bool _local_CmpIpv6(const acpl::Ipv6Addr &nAddr, acpl::UInt8 nGrp7Byte1, acpl::UInt8 nGrp7Byte2, acpl::UInt8 nGrp8Byte1, acpl::UInt8 nGrp8Byte2)
{
	return _local_CmpIpv6(nAddr, 0, 0, 0, 0, 0, 0, nGrp7Byte1, nGrp7Byte2, nGrp8Byte1, nGrp8Byte2);
}


static int TestNetworkAddr()
{
	PrintFn();
	
	
	const char *oStatVal1 = "FEDC::127.0.0.1.example.org";
	const char *oValBeg;
	const char *oValEnd;
	
	
	// Identify()
	
	Test(_local_NetworkAddrExt::Identify("") == acpl::NetworkAddr::tNone);
	Test(_local_NetworkAddrExt::Identify("127.0.0.1") == acpl::NetworkAddr::tIpv4Addr);
	Test(_local_NetworkAddrExt::Identify("FEDC::127.0.0.1") == acpl::NetworkAddr::tIpv6Addr);
	Test(_local_NetworkAddrExt::Identify("a.few.sub-domains.example.org") == acpl::NetworkAddr::tDomainName);
	
	
	// CIdentify()
	
	oValBeg = oStatVal1;
	oValEnd = oValBeg;
	Test(_local_NetworkAddrExt::CIdentify(oValBeg, oValEnd++) == acpl::NetworkAddr::tNone);
	for (acpl::UInt8 i = 0; i < 4; i++)
		TestFM(_local_NetworkAddrExt::CIdentify(oValBeg, oValEnd++) == acpl::NetworkAddr::tDomainName, "i=%u", i);
	Test(_local_NetworkAddrExt::CIdentify(oValBeg, oValEnd++) == acpl::NetworkAddr::tNone);
	for (acpl::UInt8 i = 0; i < 4; i++)
		TestFM(_local_NetworkAddrExt::CIdentify(oValBeg, oValEnd++) == acpl::NetworkAddr::tIpv6Addr, "i=%u", i);
	for (acpl::UInt8 i = 0; i < 5; i++)
		TestFM(_local_NetworkAddrExt::CIdentify(oValBeg, oValEnd++) == acpl::NetworkAddr::tNone, "i=%u", i);
	Test(_local_NetworkAddrExt::CIdentify(oValBeg, oValEnd++) == acpl::NetworkAddr::tIpv6Addr);
	for (acpl::UInt8 i = 0; i < 13; i++)
		TestFM(_local_NetworkAddrExt::CIdentify(oValBeg, oValEnd++) == acpl::NetworkAddr::tNone, "i=%u", i);
	
	oValBeg += 6;
	oValEnd = oValBeg;
	Test(_local_NetworkAddrExt::CIdentify(oValBeg, oValEnd++) == acpl::NetworkAddr::tNone);
	for (acpl::UInt8 i = 0; i < 8; i++)
	{
		if (oValEnd[-1] == '.')
			TestFM(_local_NetworkAddrExt::CIdentify(oValBeg, oValEnd++) == acpl::NetworkAddr::tNone, "i=%u", i)
		else
			TestFM(_local_NetworkAddrExt::CIdentify(oValBeg, oValEnd++) == acpl::NetworkAddr::tDomainName, "i=%u", i)
	}
	Test(_local_NetworkAddrExt::CIdentify(oValBeg, oValEnd++) == acpl::NetworkAddr::tIpv4Addr);
	for (acpl::UInt8 i = 0; i < 13; i++)
	{
		if (oValEnd[-1] == '.')
			TestFM(_local_NetworkAddrExt::CIdentify(oValBeg, oValEnd++) == acpl::NetworkAddr::tNone, "i=%u", i)
		else
			TestFM(_local_NetworkAddrExt::CIdentify(oValBeg, oValEnd++) == acpl::NetworkAddr::tDomainName, "i=%u", i)
	}
	
	
	return 0;
}

static int TestIpv4Addr()
{
	PrintFn();
	
	
	_local_Ipv4AddrExt oIpv4;
	const char *oStatVal1 = ".10.20.100.200.";
	const char *oValBeg;
	const char *oValEnd;
	acpl::String oStr;
	
	
	// Init
	
	Test(_local_IsIpvReset(oIpv4) == true);
	Test(oIpv4.IsAny() == true);
	Test(oIpv4.IsLoopback() == false);
	Test(oIpv4.IsMulticast() == false);
	
	
	// Constructor(s)
	
	Test(_local_CmpIpv4(acpl::Ipv4Addr(127, 3, 2, 1),
		127, 3, 2, 1) == true);
	
	
	// Setter(s)
	
	oIpv4.Set(127, 3, 2, 1);
	Test(_local_CmpIpv4(oIpv4, 127, 3, 2, 1) == true);
	
	
	// {Is,Set}Loopback()
	
	Test(oIpv4.IsAny() == false);
	Test(oIpv4.SetAny().IsAny() == true);
	Test(_local_CmpIpv4(oIpv4, 0, 0, 0, 0) == true);
	
	
	// {Is,Set}Loopback()
	
	Test(oIpv4.IsLoopback() == false);
	Test(oIpv4.SetLoopback().IsLoopback() == true);
	Test(_local_CmpIpv4(oIpv4, 127, 0, 0, 1) == true);
	
	
	// IsMulticast()
	
	Test(oIpv4.IsMulticast() == false);
	oIpv4.Set(223, 0, 0, 0);
	Test(oIpv4.IsMulticast() == false);
	for (acpl::UInt8 i = 224; i <= 239; i++)
	{
		oIpv4.Set(i, 0, 0, 0);
		Test(oIpv4.IsMulticast() == true);
	}
	oIpv4.Set(240, 0, 0, 0);
	Test(oIpv4.IsMulticast() == false);
	
	
	// Validate()
	
	Test(_local_Ipv4AddrExt::Validate("") == false);
	Test(_local_Ipv4AddrExt::Validate("0.0.0.0") == true);
	
	Test(_local_Ipv4AddrExt::Validate("1.0.0.0") == true);
	Test(_local_Ipv4AddrExt::Validate("255.0.0.0") == true);
	Test(_local_Ipv4AddrExt::Validate("256.0.0.0") == false);
	
	Test(_local_Ipv4AddrExt::Validate("0.1.0.0") == true);
	Test(_local_Ipv4AddrExt::Validate("0.255.0.0") == true);
	Test(_local_Ipv4AddrExt::Validate("0.256.0.0") == false);
	
	Test(_local_Ipv4AddrExt::Validate("0.0.1.0") == true);
	Test(_local_Ipv4AddrExt::Validate("0.0.255.0") == true);
	Test(_local_Ipv4AddrExt::Validate("0.0.256.0") == false);
	
	Test(_local_Ipv4AddrExt::Validate("0.0.0.1") == true);
	Test(_local_Ipv4AddrExt::Validate("0.0.0.255") == true);
	Test(_local_Ipv4AddrExt::Validate("0.0.0.256") == false);
	
	Test(_local_Ipv4AddrExt::Validate(".0.0.0") == false);
	Test(_local_Ipv4AddrExt::Validate("0..0.0") == false);
	Test(_local_Ipv4AddrExt::Validate("0.0..0") == false);
	Test(_local_Ipv4AddrExt::Validate("0.0.0.") == false);
	Test(_local_Ipv4AddrExt::Validate("0.0.0.0.") == false);
	
	
	// Parse()
	
	Test(oIpv4.Parse("") == false);
	Test(_local_IsIpvReset(oIpv4) == true);
	Test(oIpv4.Parse("0.0.0.0") == true);
	Test(_local_CmpIpv4(oIpv4, 0, 0, 0, 0) == true);
	
	Test(oIpv4.Parse("1.0.0.0") == true);
	Test(_local_CmpIpv4(oIpv4, 1, 0, 0, 0) == true);
	Test(oIpv4.Parse("255.0.0.0") == true);
	Test(_local_CmpIpv4(oIpv4, 255, 0, 0, 0) == true);
	Test(oIpv4.Parse("256.0.0.0") == false);
	Test(_local_IsIpvReset(oIpv4) == true);
	
	Test(oIpv4.Parse("0.1.0.0") == true);
	Test(_local_CmpIpv4(oIpv4, 0, 1, 0, 0) == true);
	Test(oIpv4.Parse("0.255.0.0") == true);
	Test(_local_CmpIpv4(oIpv4, 0, 255, 0, 0) == true);
	Test(oIpv4.Parse("0.256.0.0") == false);
	Test(_local_IsIpvReset(oIpv4) == true);
	
	Test(oIpv4.Parse("0.0.1.0") == true);
	Test(_local_CmpIpv4(oIpv4, 0, 0, 1, 0) == true);
	Test(oIpv4.Parse("0.0.255.0") == true);
	Test(_local_CmpIpv4(oIpv4, 0, 0, 255, 0) == true);
	Test(oIpv4.Parse("0.0.256.0") == false);
	Test(_local_IsIpvReset(oIpv4) == true);
	
	Test(oIpv4.Parse("0.0.0.1") == true);
	Test(_local_CmpIpv4(oIpv4, 0, 0, 0, 1) == true);
	Test(oIpv4.Parse("0.0.0.255") == true);
	Test(_local_CmpIpv4(oIpv4, 0, 0, 0, 255) == true);
	Test(oIpv4.Parse("0.0.0.256") == false);
	Test(_local_IsIpvReset(oIpv4) == true);
	
	Test(oIpv4.Parse(".0.0.0") == false);
	Test(_local_IsIpvReset(oIpv4) == true);
	Test(oIpv4.Parse("0..0.0") == false);
	Test(_local_IsIpvReset(oIpv4) == true);
	Test(oIpv4.Parse("0.0..0") == false);
	Test(_local_IsIpvReset(oIpv4) == true);
	Test(oIpv4.Parse("0.0.0.") == false);
	Test(_local_IsIpvReset(oIpv4) == true);
	Test(oIpv4.Parse("0.0.0.0.") == false);
	Test(_local_IsIpvReset(oIpv4) == true);
	
	
	// CValidate() (custom validate)
	
	oValBeg = oStatVal1;
	oValEnd = oValBeg;
	Test(_local_Ipv4AddrExt::CValidate(oValBeg, oValEnd++) == false);
	Test(_local_Ipv4AddrExt::CValidate(oValBeg, oValEnd++) == false);
	Test(_local_Ipv4AddrExt::CValidate(oValBeg, oValEnd++) == false);
	Test(_local_Ipv4AddrExt::CValidate(oValBeg, oValEnd++) == false);
	Test(_local_Ipv4AddrExt::CValidate(oValBeg, oValEnd++) == false);
	Test(_local_Ipv4AddrExt::CValidate(oValBeg, oValEnd++) == false);
	Test(_local_Ipv4AddrExt::CValidate(oValBeg, oValEnd++) == false);
	Test(_local_Ipv4AddrExt::CValidate(oValBeg, oValEnd++) == false);
	Test(_local_Ipv4AddrExt::CValidate(oValBeg, oValEnd++) == false);
	Test(_local_Ipv4AddrExt::CValidate(oValBeg, oValEnd++) == false);
	Test(_local_Ipv4AddrExt::CValidate(oValBeg, oValEnd++) == false);
	Test(_local_Ipv4AddrExt::CValidate(oValBeg, oValEnd++) == false);
	Test(_local_Ipv4AddrExt::CValidate(oValBeg, oValEnd++) == false);
	Test(_local_Ipv4AddrExt::CValidate(oValBeg, oValEnd++) == false);
	Test(_local_Ipv4AddrExt::CValidate(oValBeg, oValEnd++) == false);
	Test(_local_Ipv4AddrExt::CValidate(oValBeg, oValEnd++) == false);
	
	oValBeg++;
	oValEnd = oValBeg;
	Test(_local_Ipv4AddrExt::CValidate(oValBeg, oValEnd++) == false);
	Test(_local_Ipv4AddrExt::CValidate(oValBeg, oValEnd++) == false);
	Test(_local_Ipv4AddrExt::CValidate(oValBeg, oValEnd++) == false);
	Test(_local_Ipv4AddrExt::CValidate(oValBeg, oValEnd++) == false);
	Test(_local_Ipv4AddrExt::CValidate(oValBeg, oValEnd++) == false);
	Test(_local_Ipv4AddrExt::CValidate(oValBeg, oValEnd++) == false);
	Test(_local_Ipv4AddrExt::CValidate(oValBeg, oValEnd++) == false);
	Test(_local_Ipv4AddrExt::CValidate(oValBeg, oValEnd++) == false);
	Test(_local_Ipv4AddrExt::CValidate(oValBeg, oValEnd++) == false);
	Test(_local_Ipv4AddrExt::CValidate(oValBeg, oValEnd++) == false);
	Test(_local_Ipv4AddrExt::CValidate(oValBeg, oValEnd++) == false);
	Test(_local_Ipv4AddrExt::CValidate(oValBeg, oValEnd++) == true);
	Test(_local_Ipv4AddrExt::CValidate(oValBeg, oValEnd++) == true);
	Test(_local_Ipv4AddrExt::CValidate(oValBeg, oValEnd++) == true);
	Test(_local_Ipv4AddrExt::CValidate(oValBeg, oValEnd++) == false);
	Test(_local_Ipv4AddrExt::CValidate(oValBeg, oValEnd++) == false);
	
	
	// CParse() (custom parse)
	
	oValBeg = oStatVal1;
	oValEnd = oValBeg;
	Test(oIpv4.CParse(oValBeg, oValEnd++) == false);
	Test(_local_IsIpvReset(oIpv4) == true);
	Test(oIpv4.CParse(oValBeg, oValEnd++) == false);
	Test(_local_IsIpvReset(oIpv4) == true);
	Test(oIpv4.CParse(oValBeg, oValEnd++) == false);
	Test(_local_IsIpvReset(oIpv4) == true);
	Test(oIpv4.CParse(oValBeg, oValEnd++) == false);
	Test(_local_IsIpvReset(oIpv4) == true);
	Test(oIpv4.CParse(oValBeg, oValEnd++) == false);
	Test(_local_IsIpvReset(oIpv4) == true);
	Test(oIpv4.CParse(oValBeg, oValEnd++) == false);
	Test(_local_IsIpvReset(oIpv4) == true);
	Test(oIpv4.CParse(oValBeg, oValEnd++) == false);
	Test(_local_IsIpvReset(oIpv4) == true);
	Test(oIpv4.CParse(oValBeg, oValEnd++) == false);
	Test(_local_IsIpvReset(oIpv4) == true);
	Test(oIpv4.CParse(oValBeg, oValEnd++) == false);
	Test(_local_IsIpvReset(oIpv4) == true);
	Test(oIpv4.CParse(oValBeg, oValEnd++) == false);
	Test(_local_IsIpvReset(oIpv4) == true);
	Test(oIpv4.CParse(oValBeg, oValEnd++) == false);
	Test(_local_IsIpvReset(oIpv4) == true);
	Test(oIpv4.CParse(oValBeg, oValEnd++) == false);
	Test(_local_IsIpvReset(oIpv4) == true);
	Test(oIpv4.CParse(oValBeg, oValEnd++) == false);
	Test(_local_IsIpvReset(oIpv4) == true);
	Test(oIpv4.CParse(oValBeg, oValEnd++) == false);
	Test(_local_IsIpvReset(oIpv4) == true);
	Test(oIpv4.CParse(oValBeg, oValEnd++) == false);
	Test(_local_IsIpvReset(oIpv4) == true);
	
	oValBeg++;
	oValEnd = oValBeg;
	Test(oIpv4.CParse(oValBeg, oValEnd++) == false);
	Test(_local_IsIpvReset(oIpv4) == true);
	Test(oIpv4.CParse(oValBeg, oValEnd++) == false);
	Test(_local_IsIpvReset(oIpv4) == true);
	Test(oIpv4.CParse(oValBeg, oValEnd++) == false);
	Test(_local_IsIpvReset(oIpv4) == true);
	Test(oIpv4.CParse(oValBeg, oValEnd++) == false);
	Test(_local_IsIpvReset(oIpv4) == true);
	Test(oIpv4.CParse(oValBeg, oValEnd++) == false);
	Test(_local_IsIpvReset(oIpv4) == true);
	Test(oIpv4.CParse(oValBeg, oValEnd++) == false);
	Test(_local_IsIpvReset(oIpv4) == true);
	Test(oIpv4.CParse(oValBeg, oValEnd++) == false);
	Test(_local_IsIpvReset(oIpv4) == true);
	Test(oIpv4.CParse(oValBeg, oValEnd++) == false);
	Test(_local_IsIpvReset(oIpv4) == true);
	Test(oIpv4.CParse(oValBeg, oValEnd++) == false);
	Test(_local_IsIpvReset(oIpv4) == true);
	Test(oIpv4.CParse(oValBeg, oValEnd++) == false);
	Test(_local_IsIpvReset(oIpv4) == true);
	Test(oIpv4.CParse(oValBeg, oValEnd++) == false);
	Test(_local_IsIpvReset(oIpv4) == true);
	Test(oIpv4.CParse(oValBeg, oValEnd++) == true);
	Test(_local_CmpIpv4(oIpv4, 10, 20, 100, 2) == true);
	Test(oIpv4.CParse(oValBeg, oValEnd++) == true);
	Test(_local_CmpIpv4(oIpv4, 10, 20, 100, 20) == true);
	Test(oIpv4.CParse(oValBeg, oValEnd++) == true);
	Test(_local_CmpIpv4(oIpv4, 10, 20, 100, 200) == true);
	Test(oIpv4.CParse(oValBeg, oValEnd++) == false);
	Test(_local_IsIpvReset(oIpv4) == true);
	
	
	// ToStr()
	
	Test(oIpv4.ToStr(oStr) == true);
	Test(StringCompare(oStr.Utf8(), "0.0.0.0") == 0);
	
	oIpv4.Data()[0] = 12;
	Test(oIpv4.ToStr(oStr) == true);
	Test(StringCompare(oStr.Utf8(), "12.0.0.0") == 0);
	oIpv4.Data()[0] = 255;
	Test(oIpv4.ToStr(oStr) == true);
	Test(StringCompare(oStr.Utf8(), "255.0.0.0") == 0);
	
	oIpv4.Data()[1] = 13;
	Test(oIpv4.ToStr(oStr) == true);
	Test(StringCompare(oStr.Utf8(), "255.13.0.0") == 0);
	oIpv4.Data()[1] = 254;
	Test(oIpv4.ToStr(oStr) == true);
	Test(StringCompare(oStr.Utf8(), "255.254.0.0") == 0);
	
	oIpv4.Data()[2] = 14;
	Test(oIpv4.ToStr(oStr) == true);
	Test(StringCompare(oStr.Utf8(), "255.254.14.0") == 0);
	oIpv4.Data()[2] = 253;
	Test(oIpv4.ToStr(oStr) == true);
	Test(StringCompare(oStr.Utf8(), "255.254.253.0") == 0);
	
	oIpv4.Data()[3] = 15;
	Test(oIpv4.ToStr(oStr) == true);
	Test(StringCompare(oStr.Utf8(), "255.254.253.15") == 0);
	oIpv4.Data()[3] = 252;
	Test(oIpv4.ToStr(oStr) == true);
	Test(StringCompare(oStr.Utf8(), "255.254.253.252") == 0);
	
	
	// Reset()
	
	oIpv4.Reset();
	Test(_local_IsIpvReset(oIpv4) == true);
	
	
	return 0;
}

static int TestIpv6Addr()
{
	PrintFn();
	
	
	_local_Ipv6AddrExt oIpv6;
	const char *oStatVal1 = ":::";
	const char *oStatVal2 = "::FEDC:BA98:7654:3210:FEDC:BA98:7654:3210::";
	const char *oStatVal3 = "FEDC::127.20.0.1.";
	const char *oValBeg;
	const char *oValEnd;
	acpl::String oStr;
	
	
	// Init
	
	Test(_local_IsIpvReset(oIpv6) == true);
	Test(oIpv6.IsAny() == true);
	Test(oIpv6.IsLoopback() == false);
	Test(oIpv6.IsMulticast() == false);
	
	
	// Constructor(s)
	
	Test(_local_CmpIpv6(acpl::Ipv6Addr(0x0123, 0x4567, 0x89AB, 0xCDEF, 0xFEDC, 0xBA98, 0x7654, 0x3210),
		0x0123, 0x4567, 0x89AB, 0xCDEF, 0xFEDC, 0xBA98, 0x7654, 0x3210) == true);
	
	Test(_local_CmpIpv6(acpl::Ipv6Addr(0xFEDC, 0xBA98, 0x7654, 0x3210, 0x0123, 0x4567, 127, 3, 2, 1),
		0xFEDC, 0xBA98, 0x7654, 0x3210, 0x0123, 0x4567, 127, 3, 2, 1) == true);
	
	Test(_local_CmpIpv6(acpl::Ipv6Addr(127, 3, 2, 1),
		127, 3, 2, 1) == true);
	
	
	// Setter(s)
	
	oIpv6.Set(0x0123, 0x4567, 0x89AB, 0xCDEF, 0xFEDC, 0xBA98, 0x7654, 0x3210);
	Test(_local_CmpIpv6(oIpv6, 0x0123, 0x4567, 0x89AB, 0xCDEF, 0xFEDC, 0xBA98, 0x7654, 0x3210) == true);
	
	oIpv6.Set(0xFEDC, 0xBA98, 0x7654, 0x3210, 0x0123, 0x4567, 127, 3, 2, 1);
	Test(_local_CmpIpv6(oIpv6, 0xFEDC, 0xBA98, 0x7654, 0x3210, 0x0123, 0x4567, 127, 3, 2, 1) == true);
	
	oIpv6.Set(127, 3, 2, 1);
	Test(_local_CmpIpv6(oIpv6, 127, 3, 2, 1) == true);
	
	
	// {Is,Set}Loopback()
	
	Test(oIpv6.IsAny() == false);
	Test(oIpv6.SetAny().IsAny() == true);
	Test(_local_CmpIpv6(oIpv6, 0, 0, 0, 0, 0, 0, 0, 0) == true);
	
	
	// {Is,Set}Loopback()
	
	Test(oIpv6.IsLoopback() == false);
	Test(oIpv6.SetLoopback().IsLoopback() == true);
	Test(_local_CmpIpv6(oIpv6, 0, 0, 0, 0, 0, 0, 0, 1) == true);
	
	
	// IsMulticast()
	
	Test(oIpv6.IsMulticast() == false);
	oIpv6.Set(0xFEFF, 0, 0, 0, 0, 0, 0, 0);
	Test(oIpv6.IsMulticast() == false);
	for (acpl::UInt16 i = 0xFEFF; i++ < 0xFFFF; )
	{
		oIpv6.Set(i, 0, 0, 0, 0, 0, 0, 0);
		Test(oIpv6.IsMulticast() == true);
	}
	
	
	// Validate()
	
	Test(_local_Ipv6AddrExt::Validate("") == false);
	Test(_local_Ipv6AddrExt::Validate(":") == false);
	Test(_local_Ipv6AddrExt::Validate("::") == true);
	Test(_local_Ipv6AddrExt::Validate(":::") == false);
	
	
	// Parse() (pure IPv6)
	
	Test(oIpv6.Parse("FEDC:BA98:7654:3210:FEDC:BA98:7654:3210") == true);
	Test(_local_CmpIpv6(oIpv6, 0xFEDC, 0xBA98, 0x7654, 0x3210, 0xFEDC, 0xBA98, 0x7654, 0x3210) == true);
	
	Test(oIpv6.Parse("::BA98:7654:3210:FEDC:BA98:7654:3210") == true);
	Test(_local_CmpIpv6(oIpv6, 0x0000, 0xBA98, 0x7654, 0x3210, 0xFEDC, 0xBA98, 0x7654, 0x3210) == true);
	
	Test(oIpv6.Parse("::7654:3210:FEDC:BA98:7654:3210") == true);
	Test(_local_CmpIpv6(oIpv6, 0x0000, 0x0000, 0x7654, 0x3210, 0xFEDC, 0xBA98, 0x7654, 0x3210) == true);
	
	Test(oIpv6.Parse("::3210:FEDC:BA98:7654:3210") == true);
	Test(_local_CmpIpv6(oIpv6, 0x0000, 0x0000, 0x0000, 0x3210, 0xFEDC, 0xBA98, 0x7654, 0x3210) == true);
	
	Test(oIpv6.Parse("::FEDC:BA98:7654:3210") == true);
	Test(_local_CmpIpv6(oIpv6, 0x0000, 0x0000, 0x0000, 0x0000, 0xFEDC, 0xBA98, 0x7654, 0x3210) == true);
	
	Test(oIpv6.Parse("::BA98:7654:3210") == true);
	Test(_local_CmpIpv6(oIpv6, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xBA98, 0x7654, 0x3210) == true);
	
	Test(oIpv6.Parse("::7654:3210") == true);
	Test(_local_CmpIpv6(oIpv6, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x7654, 0x3210) == true);
	
	Test(oIpv6.Parse("::3210") == true);
	Test(_local_CmpIpv6(oIpv6, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x3210) == true);
	
	Test(oIpv6.Parse("::") == true);
	Test(_local_CmpIpv6(oIpv6, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000) == true);
	
	Test(oIpv6.Parse("FEDC:BA98:7654:3210:FEDC:BA98:7654::") == true);
	Test(_local_CmpIpv6(oIpv6, 0xFEDC, 0xBA98, 0x7654, 0x3210, 0xFEDC, 0xBA98, 0x7654, 0x0000) == true);
	
	Test(oIpv6.Parse("FEDC:BA98:7654:3210:FEDC:BA98::") == true);
	Test(_local_CmpIpv6(oIpv6, 0xFEDC, 0xBA98, 0x7654, 0x3210, 0xFEDC, 0xBA98, 0x0000, 0x0000) == true);
	
	Test(oIpv6.Parse("FEDC:BA98:7654:3210:FEDC::") == true);
	Test(_local_CmpIpv6(oIpv6, 0xFEDC, 0xBA98, 0x7654, 0x3210, 0xFEDC, 0x0000, 0x0000, 0x0000) == true);
	
	Test(oIpv6.Parse("FEDC:BA98:7654:3210::") == true);
	Test(_local_CmpIpv6(oIpv6, 0xFEDC, 0xBA98, 0x7654, 0x3210, 0x0000, 0x0000, 0x0000, 0x0000) == true);
	
	Test(oIpv6.Parse("FEDC:BA98:7654::") == true);
	Test(_local_CmpIpv6(oIpv6, 0xFEDC, 0xBA98, 0x7654, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000) == true);
	
	Test(oIpv6.Parse("FEDC:BA98::") == true);
	Test(_local_CmpIpv6(oIpv6, 0xFEDC, 0xBA98, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000) == true);
	
	Test(oIpv6.Parse("FEDC::") == true);
	Test(_local_CmpIpv6(oIpv6, 0xFEDC, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000) == true);
	
	Test(oIpv6.Parse("FEDC::7654:3210:FEDC:BA98:7654:3210") == true);
	Test(_local_CmpIpv6(oIpv6, 0xFEDC, 0x0000, 0x7654, 0x3210, 0xFEDC, 0xBA98, 0x7654, 0x3210) == true);
	
	Test(oIpv6.Parse("FEDC::3210:FEDC:BA98:7654:3210") == true);
	Test(_local_CmpIpv6(oIpv6, 0xFEDC, 0x0000, 0x0000, 0x3210, 0xFEDC, 0xBA98, 0x7654, 0x3210) == true);
	
	Test(oIpv6.Parse("FEDC::FEDC:BA98:7654:3210") == true);
	Test(_local_CmpIpv6(oIpv6, 0xFEDC, 0x0000, 0x0000, 0x0000, 0xFEDC, 0xBA98, 0x7654, 0x3210) == true);
	
	Test(oIpv6.Parse("FEDC::BA98:7654:3210") == true);
	Test(_local_CmpIpv6(oIpv6, 0xFEDC, 0x0000, 0x0000, 0x0000, 0x0000, 0xBA98, 0x7654, 0x3210) == true);
	
	Test(oIpv6.Parse("FEDC::7654:3210") == true);
	Test(_local_CmpIpv6(oIpv6, 0xFEDC, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x7654, 0x3210) == true);
	
	Test(oIpv6.Parse("FEDC::3210") == true);
	Test(_local_CmpIpv6(oIpv6, 0xFEDC, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x3210) == true);
	
	Test(oIpv6.Parse("FEDC:BA98:7654:3210::BA98:7654:3210") == true);
	Test(_local_CmpIpv6(oIpv6, 0xFEDC, 0xBA98, 0x7654, 0x3210, 0x0000, 0xBA98, 0x7654, 0x3210) == true);
	
	Test(oIpv6.Parse("FEDC:BA98:7654::BA98:7654:3210") == true);
	Test(_local_CmpIpv6(oIpv6, 0xFEDC, 0xBA98, 0x7654, 0x0000, 0x0000, 0xBA98, 0x7654, 0x3210) == true);
	
	Test(oIpv6.Parse("FEDC:BA98:7654::7654:3210") == true);
	Test(_local_CmpIpv6(oIpv6, 0xFEDC, 0xBA98, 0x7654, 0x0000, 0x0000, 0x0000, 0x7654, 0x3210) == true);
	
	Test(oIpv6.Parse("FEDC:BA98::7654:3210") == true);
	Test(_local_CmpIpv6(oIpv6, 0xFEDC, 0xBA98, 0x0000, 0x0000, 0x0000, 0x0000, 0x7654, 0x3210) == true);
	
	Test(oIpv6.Parse("FEDC:BA98::3210") == true);
	Test(_local_CmpIpv6(oIpv6, 0xFEDC, 0xBA98, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x3210) == true);
	
	
	// Parse() (with IPv4 mode)
	
	Test(oIpv6.Parse("FEDC:BA98:7654:3210:FEDC:BA98:127.20.0.1") == true);
	Test(_local_CmpIpv6(oIpv6, 0xFEDC, 0xBA98, 0x7654, 0x3210, 0xFEDC, 0xBA98, 127, 20, 0, 1) == true);
	
	Test(oIpv6.Parse("::BA98:7654:3210:FEDC:BA98:127.20.0.1") == true);
	Test(_local_CmpIpv6(oIpv6, 0x0000, 0xBA98, 0x7654, 0x3210, 0xFEDC, 0xBA98, 127, 20, 0, 1) == true);
	
	Test(oIpv6.Parse("::7654:3210:FEDC:BA98:127.20.0.1") == true);
	Test(_local_CmpIpv6(oIpv6, 0x0000, 0x0000, 0x7654, 0x3210, 0xFEDC, 0xBA98, 127, 20, 0, 1) == true);
	
	Test(oIpv6.Parse("::3210:FEDC:BA98:127.20.0.1") == true);
	Test(_local_CmpIpv6(oIpv6, 0x0000, 0x0000, 0x0000, 0x3210, 0xFEDC, 0xBA98, 127, 20, 0, 1) == true);
	
	Test(oIpv6.Parse("::FEDC:BA98:127.20.0.1") == true);
	Test(_local_CmpIpv6(oIpv6, 0x0000, 0x0000, 0x0000, 0x0000, 0xFEDC, 0xBA98, 127, 20, 0, 1) == true);
	
	Test(oIpv6.Parse("::BA98:127.20.0.1") == true);
	Test(_local_CmpIpv6(oIpv6, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xBA98, 127, 20, 0, 1) == true);
	
	Test(oIpv6.Parse("::127.20.0.1") == true);
	Test(_local_CmpIpv6(oIpv6, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 127, 20, 0, 1) == true);
	
	Test(oIpv6.Parse("FEDC:BA98:7654:3210:FEDC::127.20.0.1") == true);
	Test(_local_CmpIpv6(oIpv6, 0xFEDC, 0xBA98, 0x7654, 0x3210, 0xFEDC, 0x0000, 127, 20, 0, 1) == true);
	
	Test(oIpv6.Parse("FEDC:BA98:7654:3210::127.20.0.1") == true);
	Test(_local_CmpIpv6(oIpv6, 0xFEDC, 0xBA98, 0x7654, 0x3210, 0x0000, 0x0000, 127, 20, 0, 1) == true);
	
	Test(oIpv6.Parse("FEDC:BA98:7654::127.20.0.1") == true);
	Test(_local_CmpIpv6(oIpv6, 0xFEDC, 0xBA98, 0x7654, 0x0000, 0x0000, 0x0000, 127, 20, 0, 1) == true);
	
	Test(oIpv6.Parse("FEDC:BA98::127.20.0.1") == true);
	Test(_local_CmpIpv6(oIpv6, 0xFEDC, 0xBA98, 0x0000, 0x0000, 0x0000, 0x0000, 127, 20, 0, 1) == true);
	
	Test(oIpv6.Parse("FEDC::127.20.0.1") == true);
	Test(_local_CmpIpv6(oIpv6, 0xFEDC, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 127, 20, 0, 1) == true);
	
	
	// Parse() (invalid)
	
	Test(oIpv6.Parse("::FEDC:BA98:7654:3210:FEDC:BA98:7654:3210") == false);
	Test(oIpv6.Parse("FEDC::BA98:7654:3210:FEDC:BA98:7654:3210") == false);
	Test(oIpv6.Parse("FEDC:BA98::7654:3210:FEDC:BA98:7654:3210") == false);
	Test(oIpv6.Parse("FEDC:BA98:7654::3210:FEDC:BA98:7654:3210") == false);
	Test(oIpv6.Parse("FEDC:BA98:7654:3210::FEDC:BA98:7654:3210") == false);
	Test(oIpv6.Parse("FEDC:BA98:7654:3210:FEDC::BA98:7654:3210") == false);
	Test(oIpv6.Parse("FEDC:BA98:7654:3210:FEDC:BA98::7654:3210") == false);
	Test(oIpv6.Parse("FEDC:BA98:7654:3210:FEDC:BA98:7654::3210") == false);
	Test(oIpv6.Parse("FEDC:BA98:7654:3210:FEDC:BA98:7654:3210::") == false);
	
	Test(oIpv6.Parse("FEDC:BA98:7654:3210:FEDC:BA98:7654") == false);
	Test(oIpv6.Parse("FEDC:BA98:7654:3210:FEDC:BA98:") == false);
	Test(oIpv6.Parse("FEDC:BA98:7654:3210:FEDC:BA98") == false);
	Test(oIpv6.Parse("FEDC:BA98:7654:3210:FEDC:") == false);
	Test(oIpv6.Parse("FEDC:BA98:7654:3210:FEDC") == false);
	Test(oIpv6.Parse("FEDC:BA98:7654:3210:") == false);
	Test(oIpv6.Parse("FEDC:BA98:7654:3210") == false);
	Test(oIpv6.Parse("FEDC:BA98:7654:") == false);
	Test(oIpv6.Parse("FEDC:BA98:7654") == false);
	Test(oIpv6.Parse("FEDC:BA98:") == false);
	Test(oIpv6.Parse("FEDC:BA98") == false);
	Test(oIpv6.Parse("FEDC:") == false);
	Test(oIpv6.Parse("FEDC") == false);
	
	Test(oIpv6.Parse("::FEDC:BA98:7654:3210:FEDC:BA98:127.20.0.1") == false);
	Test(oIpv6.Parse("FEDC::BA98:7654:3210:FEDC:BA98:127.20.0.1") == false);
	Test(oIpv6.Parse("FEDC:BA98::7654:3210:FEDC:BA98:127.20.0.1") == false);
	Test(oIpv6.Parse("FEDC:BA98:7654::3210:FEDC:BA98:127.20.0.1") == false);
	Test(oIpv6.Parse("FEDC:BA98:7654:3210::FEDC:BA98:127.20.0.1") == false);
	Test(oIpv6.Parse("FEDC:BA98:7654:3210:FEDC::BA98:127.20.0.1") == false);
	Test(oIpv6.Parse("FEDC:BA98:7654:3210:FEDC:BA98::127.20.0.1") == false);
	
	Test(oIpv6.Parse("FEDC:BA98:7654:3210:FEDC:127.20.0.1") == false);
	Test(oIpv6.Parse("FEDC:BA98:7654:3210:127.20.0.1") == false);
	Test(oIpv6.Parse("FEDC:BA98:7654:127.20.0.1") == false);
	Test(oIpv6.Parse("FEDC:BA98:127.20.0.1") == false);
	Test(oIpv6.Parse("FEDC:127.20.0.1") == false);
	Test(oIpv6.Parse("127.20.0.1") == false);
	
	
	// CParse() (pure IPv6)
	
	oValBeg = oStatVal1;
	oValEnd = oValBeg;
	Test(oIpv6.CParse(oValBeg, oValEnd++) == false);
	Test(_local_IsIpvReset(oIpv6) == true);
	Test(oIpv6.CParse(oValBeg, oValEnd++) == false);
	Test(_local_IsIpvReset(oIpv6) == true);
	Test(oIpv6.CParse(oValBeg, oValEnd++) == true);
	Test(_local_CmpIpv6(oIpv6, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000) == true);
	Test(oIpv6.CParse(oValBeg, oValEnd++) == false);
	Test(_local_IsIpvReset(oIpv6) == true);
	Test(oIpv6.CParse(oValBeg, oValEnd++) == false);
	Test(_local_IsIpvReset(oIpv6) == true);
	
	oValBeg = oStatVal2;
	oValEnd = oValBeg;
	Test(oIpv6.CParse(oValBeg, oValEnd++) == false);
	Test(_local_IsIpvReset(oIpv6) == true);
	Test(oIpv6.CParse(oValBeg, oValEnd++) == false);
	Test(_local_IsIpvReset(oIpv6) == true);
	Test(oIpv6.CParse(oValBeg, oValEnd++) == true);
	Test(_local_CmpIpv6(oIpv6, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000) == true);
	Test(oIpv6.CParse(oValBeg, oValEnd++) == true);
	Test(_local_CmpIpv6(oIpv6, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x000F) == true);
	Test(oIpv6.CParse(oValBeg, oValEnd++) == true);
	Test(_local_CmpIpv6(oIpv6, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x00FE) == true);
	Test(oIpv6.CParse(oValBeg, oValEnd++) == true);
	Test(_local_CmpIpv6(oIpv6, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0FED) == true);
	Test(oIpv6.CParse(oValBeg, oValEnd++) == true);
	Test(_local_CmpIpv6(oIpv6, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFEDC) == true);
	Test(oIpv6.CParse(oValBeg, oValEnd++) == false);
	Test(_local_IsIpvReset(oIpv6) == true);
	Test(oIpv6.CParse(oValBeg, oValEnd++) == true);
	Test(_local_CmpIpv6(oIpv6, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFEDC, 0x000B) == true);
	Test(oIpv6.CParse(oValBeg, oValEnd++) == true);
	Test(_local_CmpIpv6(oIpv6, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFEDC, 0x00BA) == true);
	Test(oIpv6.CParse(oValBeg, oValEnd++) == true);
	Test(_local_CmpIpv6(oIpv6, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFEDC, 0x0BA9) == true);
	Test(oIpv6.CParse(oValBeg, oValEnd++) == true);
	Test(_local_CmpIpv6(oIpv6, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFEDC, 0xBA98) == true);
	Test(oIpv6.CParse(oValBeg, oValEnd++) == false);
	Test(_local_IsIpvReset(oIpv6) == true);
	Test(oIpv6.CParse(oValBeg, oValEnd++) == true);
	Test(_local_CmpIpv6(oIpv6, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFEDC, 0xBA98, 0x0007) == true);
	Test(oIpv6.CParse(oValBeg, oValEnd++) == true);
	Test(_local_CmpIpv6(oIpv6, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFEDC, 0xBA98, 0x0076) == true);
	Test(oIpv6.CParse(oValBeg, oValEnd++) == true);
	Test(_local_CmpIpv6(oIpv6, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFEDC, 0xBA98, 0x0765) == true);
	Test(oIpv6.CParse(oValBeg, oValEnd++) == true);
	Test(_local_CmpIpv6(oIpv6, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0xFEDC, 0xBA98, 0x7654) == true);
	Test(oIpv6.CParse(oValBeg, oValEnd++) == false);
	Test(_local_IsIpvReset(oIpv6) == true);
	Test(oIpv6.CParse(oValBeg, oValEnd++) == true);
	Test(_local_CmpIpv6(oIpv6, 0x0000, 0x0000, 0x0000, 0x0000, 0xFEDC, 0xBA98, 0x7654, 0x0003) == true);
	Test(oIpv6.CParse(oValBeg, oValEnd++) == true);
	Test(_local_CmpIpv6(oIpv6, 0x0000, 0x0000, 0x0000, 0x0000, 0xFEDC, 0xBA98, 0x7654, 0x0032) == true);
	Test(oIpv6.CParse(oValBeg, oValEnd++) == true);
	Test(_local_CmpIpv6(oIpv6, 0x0000, 0x0000, 0x0000, 0x0000, 0xFEDC, 0xBA98, 0x7654, 0x0321) == true);
	Test(oIpv6.CParse(oValBeg, oValEnd++) == true);
	Test(_local_CmpIpv6(oIpv6, 0x0000, 0x0000, 0x0000, 0x0000, 0xFEDC, 0xBA98, 0x7654, 0x3210) == true);
	Test(oIpv6.CParse(oValBeg, oValEnd++) == false);
	Test(_local_IsIpvReset(oIpv6) == true);
	Test(oIpv6.CParse(oValBeg, oValEnd++) == true);
	Test(_local_CmpIpv6(oIpv6, 0x0000, 0x0000, 0x0000, 0xFEDC, 0xBA98, 0x7654, 0x3210, 0x000F) == true);
	Test(oIpv6.CParse(oValBeg, oValEnd++) == true);
	Test(_local_CmpIpv6(oIpv6, 0x0000, 0x0000, 0x0000, 0xFEDC, 0xBA98, 0x7654, 0x3210, 0x00FE) == true);
	Test(oIpv6.CParse(oValBeg, oValEnd++) == true);
	Test(_local_CmpIpv6(oIpv6, 0x0000, 0x0000, 0x0000, 0xFEDC, 0xBA98, 0x7654, 0x3210, 0x0FED) == true);
	Test(oIpv6.CParse(oValBeg, oValEnd++) == true);
	Test(_local_CmpIpv6(oIpv6, 0x0000, 0x0000, 0x0000, 0xFEDC, 0xBA98, 0x7654, 0x3210, 0xFEDC) == true);
	Test(oIpv6.CParse(oValBeg, oValEnd++) == false);
	Test(_local_IsIpvReset(oIpv6) == true);
	Test(oIpv6.CParse(oValBeg, oValEnd++) == true);
	Test(_local_CmpIpv6(oIpv6, 0x0000, 0x0000, 0xFEDC, 0xBA98, 0x7654, 0x3210, 0xFEDC, 0x000B) == true);
	Test(oIpv6.CParse(oValBeg, oValEnd++) == true);
	Test(_local_CmpIpv6(oIpv6, 0x0000, 0x0000, 0xFEDC, 0xBA98, 0x7654, 0x3210, 0xFEDC, 0x00BA) == true);
	Test(oIpv6.CParse(oValBeg, oValEnd++) == true);
	Test(_local_CmpIpv6(oIpv6, 0x0000, 0x0000, 0xFEDC, 0xBA98, 0x7654, 0x3210, 0xFEDC, 0x0BA9) == true);
	Test(oIpv6.CParse(oValBeg, oValEnd++) == true);
	Test(_local_CmpIpv6(oIpv6, 0x0000, 0x0000, 0xFEDC, 0xBA98, 0x7654, 0x3210, 0xFEDC, 0xBA98) == true);
	Test(oIpv6.CParse(oValBeg, oValEnd++) == false);
	Test(_local_IsIpvReset(oIpv6) == true);
	Test(oIpv6.CParse(oValBeg, oValEnd++) == true);
	Test(_local_CmpIpv6(oIpv6, 0x0000, 0xFEDC, 0xBA98, 0x7654, 0x3210, 0xFEDC, 0xBA98, 0x0007) == true);
	Test(oIpv6.CParse(oValBeg, oValEnd++) == true);
	Test(_local_CmpIpv6(oIpv6, 0x0000, 0xFEDC, 0xBA98, 0x7654, 0x3210, 0xFEDC, 0xBA98, 0x0076) == true);
	Test(oIpv6.CParse(oValBeg, oValEnd++) == true);
	Test(_local_CmpIpv6(oIpv6, 0x0000, 0xFEDC, 0xBA98, 0x7654, 0x3210, 0xFEDC, 0xBA98, 0x0765) == true);
	Test(oIpv6.CParse(oValBeg, oValEnd++) == true);
	Test(_local_CmpIpv6(oIpv6, 0x0000, 0xFEDC, 0xBA98, 0x7654, 0x3210, 0xFEDC, 0xBA98, 0x7654) == true);
	Test(oIpv6.CParse(oValBeg, oValEnd++) == false);
	Test(_local_IsIpvReset(oIpv6) == true);
	Test(oIpv6.CParse(oValBeg, oValEnd++) == false);
	Test(_local_IsIpvReset(oIpv6) == true);
	Test(oIpv6.CParse(oValBeg, oValEnd++) == false);
	Test(_local_IsIpvReset(oIpv6) == true);
	Test(oIpv6.CParse(oValBeg, oValEnd++) == false);
	Test(_local_IsIpvReset(oIpv6) == true);
	Test(oIpv6.CParse(oValBeg, oValEnd++) == false);
	Test(_local_IsIpvReset(oIpv6) == true);
	Test(oIpv6.CParse(oValBeg, oValEnd++) == false);
	Test(_local_IsIpvReset(oIpv6) == true);
	Test(oIpv6.CParse(oValBeg, oValEnd++) == false);
	Test(_local_IsIpvReset(oIpv6) == true);
	Test(oIpv6.CParse(oValBeg, oValEnd++) == false);
	Test(_local_IsIpvReset(oIpv6) == true);
	
	oValBeg++;
	oValEnd = oValBeg;
	for (acpl::UInt8 i = 0; i < 43; i++)
		TestFM(oIpv6.CParse(oValBeg, oValEnd++) == false, "i=%u", i);
	
	oValBeg++;
	oValEnd = oValBeg;
	for (acpl::UInt8 i = 0; i < 35; i++)
		TestFM(oIpv6.CParse(oValBeg, oValEnd++) == false, "i=%u", i);
	for (acpl::UInt8 i = 0; i < 5; i++)
		TestFM(oIpv6.CParse(oValBeg, oValEnd++) == true, "i=%u", i);
	Test(oIpv6.CParse(oValBeg, oValEnd++) == false);
	Test(oIpv6.CParse(oValBeg, oValEnd++) == false);
	
	oValBeg++;
	oValEnd = oValBeg;
	for (acpl::UInt8 i = 0; i < 34; i++)
		TestFM(oIpv6.CParse(oValBeg, oValEnd++) == false, "i=%u", i);
	for (acpl::UInt8 i = 0; i < 5; i++)
		TestFM(oIpv6.CParse(oValBeg, oValEnd++) == true, "i=%u", i);
	Test(oIpv6.CParse(oValBeg, oValEnd++) == false);
	Test(oIpv6.CParse(oValBeg, oValEnd++) == false);
	
	oValBeg++;
	oValEnd = oValBeg;
	for (acpl::UInt8 i = 0; i < 33; i++)
		TestFM(oIpv6.CParse(oValBeg, oValEnd++) == false, "i=%u", i);
	for (acpl::UInt8 i = 0; i < 5; i++)
		TestFM(oIpv6.CParse(oValBeg, oValEnd++) == true, "i=%u", i);
	Test(oIpv6.CParse(oValBeg, oValEnd++) == false);
	Test(oIpv6.CParse(oValBeg, oValEnd++) == false);
	
	oValBeg++;
	oValEnd = oValBeg;
	for (acpl::UInt8 i = 0; i < 32; i++)
		TestFM(oIpv6.CParse(oValBeg, oValEnd++) == false, "i=%u", i);
	for (acpl::UInt8 i = 0; i < 5; i++)
		TestFM(oIpv6.CParse(oValBeg, oValEnd++) == true, "i=%u", i);
	Test(oIpv6.CParse(oValBeg, oValEnd++) == false);
	Test(oIpv6.CParse(oValBeg, oValEnd++) == false);
	
	
	// CParse() (with IPv4 mode)
	
	oValBeg = oStatVal3;
	oValEnd = oValBeg;
	Test(oIpv6.CParse(oValBeg, oValEnd++) == false);
	Test(_local_IsIpvReset(oIpv6) == true);
	Test(oIpv6.CParse(oValBeg, oValEnd++) == false);
	Test(_local_IsIpvReset(oIpv6) == true);
	Test(oIpv6.CParse(oValBeg, oValEnd++) == false);
	Test(_local_IsIpvReset(oIpv6) == true);
	Test(oIpv6.CParse(oValBeg, oValEnd++) == false);
	Test(_local_IsIpvReset(oIpv6) == true);
	Test(oIpv6.CParse(oValBeg, oValEnd++) == false);
	Test(_local_IsIpvReset(oIpv6) == true);
	Test(oIpv6.CParse(oValBeg, oValEnd++) == false);
	Test(_local_IsIpvReset(oIpv6) == true);
	Test(oIpv6.CParse(oValBeg, oValEnd++) == true);
	Test(_local_CmpIpv6(oIpv6, 0xFEDC, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000) == true);
	Test(oIpv6.CParse(oValBeg, oValEnd++) == true);
	Test(_local_CmpIpv6(oIpv6, 0xFEDC, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0001) == true);
	Test(oIpv6.CParse(oValBeg, oValEnd++) == true);
	Test(_local_CmpIpv6(oIpv6, 0xFEDC, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0012) == true);
	Test(oIpv6.CParse(oValBeg, oValEnd++) == true);
	Test(_local_CmpIpv6(oIpv6, 0xFEDC, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0127) == true);
	Test(oIpv6.CParse(oValBeg, oValEnd++) == false);
	Test(_local_IsIpvReset(oIpv6) == true);
	Test(oIpv6.CParse(oValBeg, oValEnd++) == false);
	Test(_local_IsIpvReset(oIpv6) == true);
	Test(oIpv6.CParse(oValBeg, oValEnd++) == false);
	Test(_local_IsIpvReset(oIpv6) == true);
	Test(oIpv6.CParse(oValBeg, oValEnd++) == false);
	Test(_local_IsIpvReset(oIpv6) == true);
	Test(oIpv6.CParse(oValBeg, oValEnd++) == false);
	Test(_local_IsIpvReset(oIpv6) == true);
	Test(oIpv6.CParse(oValBeg, oValEnd++) == false);
	Test(_local_IsIpvReset(oIpv6) == true);
	Test(oIpv6.CParse(oValBeg, oValEnd++) == true);
	Test(_local_CmpIpv6(oIpv6, 0xFEDC, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 127, 20, 0, 1) == true);
	Test(oIpv6.CParse(oValBeg, oValEnd++) == false);
	Test(_local_IsIpvReset(oIpv6) == true);
	Test(oIpv6.CParse(oValBeg, oValEnd++) == false);
	Test(_local_IsIpvReset(oIpv6) == true);
	
	
	// ToStr()
	
	// NOTE: `oIpv6` is already reset
	Test(oIpv6.ToStr(oStr) == true);
	Test(StringCompare(oStr.Utf8(), "0:0:0:0:0:0:0:0") == 0);
	
	oIpv6.Data()[15] = 0x10;
	Test(oIpv6.ToStr(oStr) == true);
	Test(StringCompare(oStr.Utf8(), "0:0:0:0:0:0:0:10") == 0);
	
	oIpv6.Data()[14] = 0x32;
	Test(oIpv6.ToStr(oStr) == true);
	Test(StringCompare(oStr.Utf8(), "0:0:0:0:0:0:0:3210") == 0);
	
	oIpv6.Data()[13] = 0x54;
	Test(oIpv6.ToStr(oStr) == true);
	Test(StringCompare(oStr.Utf8(), "0:0:0:0:0:0:54:3210") == 0);
	
	oIpv6.Data()[12] = 0x76;
	Test(oIpv6.ToStr(oStr) == true);
	Test(StringCompare(oStr.Utf8(), "0:0:0:0:0:0:7654:3210") == 0);
	
	oIpv6.Data()[11] = 0x98;
	Test(oIpv6.ToStr(oStr) == true);
	Test(StringCompare(oStr.Utf8(), "0:0:0:0:0:98:7654:3210") == 0);
	
	oIpv6.Data()[10] = 0xBA;
	Test(oIpv6.ToStr(oStr) == true);
	Test(StringCompare(oStr.Utf8(), "0:0:0:0:0:BA98:7654:3210") == 0);
	
	oIpv6.Data()[ 9] = 0xDC;
	Test(oIpv6.ToStr(oStr) == true);
	Test(StringCompare(oStr.Utf8(), "0:0:0:0:DC:BA98:7654:3210") == 0);
	
	oIpv6.Data()[ 8] = 0xFE;
	Test(oIpv6.ToStr(oStr) == true);
	Test(StringCompare(oStr.Utf8(), "0:0:0:0:FEDC:BA98:7654:3210") == 0);
	
	oIpv6.Data()[ 7] = 0x10;
	Test(oIpv6.ToStr(oStr) == true);
	Test(StringCompare(oStr.Utf8(), "0:0:0:10:FEDC:BA98:7654:3210") == 0);
	
	oIpv6.Data()[ 6] = 0x32;
	Test(oIpv6.ToStr(oStr) == true);
	Test(StringCompare(oStr.Utf8(), "0:0:0:3210:FEDC:BA98:7654:3210") == 0);
	
	oIpv6.Data()[ 5] = 0x54;
	Test(oIpv6.ToStr(oStr) == true);
	Test(StringCompare(oStr.Utf8(), "0:0:54:3210:FEDC:BA98:7654:3210") == 0);
	
	oIpv6.Data()[ 4] = 0x76;
	Test(oIpv6.ToStr(oStr) == true);
	Test(StringCompare(oStr.Utf8(), "0:0:7654:3210:FEDC:BA98:7654:3210") == 0);
	
	oIpv6.Data()[ 3] = 0x98;
	Test(oIpv6.ToStr(oStr) == true);
	Test(StringCompare(oStr.Utf8(), "0:98:7654:3210:FEDC:BA98:7654:3210") == 0);
	
	oIpv6.Data()[ 2] = 0xBA;
	Test(oIpv6.ToStr(oStr) == true);
	Test(StringCompare(oStr.Utf8(), "0:BA98:7654:3210:FEDC:BA98:7654:3210") == 0);
	
	oIpv6.Data()[ 1] = 0xDC;
	Test(oIpv6.ToStr(oStr) == true);
	Test(StringCompare(oStr.Utf8(), "DC:BA98:7654:3210:FEDC:BA98:7654:3210") == 0);
	
	oIpv6.Data()[ 0] = 0xFE;
	Test(oIpv6.ToStr(oStr) == true);
	Test(StringCompare(oStr.Utf8(), "FEDC:BA98:7654:3210:FEDC:BA98:7654:3210") == 0);
	
	
	return 0;
}

static int TestDomainName()
{
	PrintFn();
	
	
	_local_DomainNameExt oDn;
	const char *oStatVal1 = "-a.a-3.a.";
	const char *oValBeg;
	const char *oValEnd;
	acpl::String oStr;
	acpl::ArrayEdaNone<acpl::String>::Type oSdns;
	
	
	// Validate()
	
	Test(_local_DomainNameExt::Validate("") == false);
	Test(_local_DomainNameExt::Validate(".") == false);
	Test(_local_DomainNameExt::Validate("-") == false);
	Test(_local_DomainNameExt::Validate("3") == true);
	Test(_local_DomainNameExt::Validate("a") == true);
	
	Test(_local_DomainNameExt::Validate("a.") == false);
	Test(_local_DomainNameExt::Validate("a-") == false);
	Test(_local_DomainNameExt::Validate("aa") == true);
	Test(_local_DomainNameExt::Validate("a3") == true);
	Test(_local_DomainNameExt::Validate("3a") == true);
	
	Test(_local_DomainNameExt::Validate("a.a") == true);
	Test(_local_DomainNameExt::Validate("a3.a") == true);
	Test(_local_DomainNameExt::Validate("a.a3") == true);
	Test(_local_DomainNameExt::Validate("a.a-3") == true);
	Test(_local_DomainNameExt::Validate("a.a------3") == true);
	
	
	// Parse()
	
	Test(oDn.Parse("") == false);
	Test(oDn.GetStr().IsEmpty() == true);
	Test(oDn.Parse("127.0.0.1") == false);
	Test(oDn.GetStr().IsEmpty() == true);
	Test(oDn.Parse("a.few.sub-domains.example.org") == true);
	Test(StringCompare(oDn.GetStr().Utf8(), "a.few.sub-domains.example.org") == 0);
	
	
	// CValidate()
	
	oValBeg = oStatVal1;
	oValEnd = oValBeg;
	Test(_local_DomainNameExt::CValidate(oValBeg, oValEnd++) == false);
	Test(_local_DomainNameExt::CValidate(oValBeg, oValEnd++) == false);
	Test(_local_DomainNameExt::CValidate(oValBeg, oValEnd++) == false);
	Test(_local_DomainNameExt::CValidate(oValBeg, oValEnd++) == false);
	Test(_local_DomainNameExt::CValidate(oValBeg, oValEnd++) == false);
	Test(_local_DomainNameExt::CValidate(oValBeg, oValEnd++) == false);
	Test(_local_DomainNameExt::CValidate(oValBeg, oValEnd++) == false);
	Test(_local_DomainNameExt::CValidate(oValBeg, oValEnd++) == false);
	Test(_local_DomainNameExt::CValidate(oValBeg, oValEnd++) == false);
	Test(_local_DomainNameExt::CValidate(oValBeg, oValEnd++) == false);
	
	oValBeg++;
	oValEnd = oValBeg;
	Test(_local_DomainNameExt::CValidate(oValBeg, oValEnd++) == false);
	Test(_local_DomainNameExt::CValidate(oValBeg, oValEnd++) == true);
	Test(_local_DomainNameExt::CValidate(oValBeg, oValEnd++) == false);
	Test(_local_DomainNameExt::CValidate(oValBeg, oValEnd++) == true);
	Test(_local_DomainNameExt::CValidate(oValBeg, oValEnd++) == false);
	Test(_local_DomainNameExt::CValidate(oValBeg, oValEnd++) == true);
	Test(_local_DomainNameExt::CValidate(oValBeg, oValEnd++) == false);
	Test(_local_DomainNameExt::CValidate(oValBeg, oValEnd++) == true);
	Test(_local_DomainNameExt::CValidate(oValBeg, oValEnd++) == false);
	Test(_local_DomainNameExt::CValidate(oValBeg, oValEnd++) == false);
	
	
	// CParse()
	
	oValBeg = oStatVal1 + 1;
	oValEnd = oValBeg;
	Test(oDn.CParse(oValBeg, oValEnd++) == false);
	Test(oDn.GetStr().IsEmpty() == true);
	Test(oDn.CParse(oValBeg, oValEnd++) == true);
	Test(StringCompare(oDn.GetStr().Utf8(), "a") == 0);
	Test(oDn.CParse(oValBeg, oValEnd++) == false);
	Test(oDn.GetStr().IsEmpty() == true);
	Test(oDn.CParse(oValBeg, oValEnd++) == true);
	Test(StringCompare(oDn.GetStr().Utf8(), "a.a") == 0);
	Test(oDn.CParse(oValBeg, oValEnd++) == false);
	Test(oDn.GetStr().IsEmpty() == true);
	Test(oDn.CParse(oValBeg, oValEnd++) == true);
	Test(StringCompare(oDn.GetStr().Utf8(), "a.a-3") == 0);
	Test(oDn.CParse(oValBeg, oValEnd++) == false);
	Test(oDn.GetStr().IsEmpty() == true);
	Test(oDn.CParse(oValBeg, oValEnd++) == true);
	Test(StringCompare(oDn.GetStr().Utf8(), "a.a-3.a") == 0);
	Test(oDn.CParse(oValBeg, oValEnd++) == false);
	Test(oDn.GetStr().IsEmpty() == true);
	Test(oDn.CParse(oValBeg, oValEnd++) == false);
	Test(oDn.GetStr().IsEmpty() == true);
	
	
	// ToStr()
	
	oStr.Set("test");
	// NOTE: `oDn` is already reset
	
	Test(oDn.ToStr(oStr) == false);
	Test(StringCompare(oStr.Utf8(), "") == 0);
	
	Test(oDn.Parse("a.few.sub-domains.example.org") == true);
	Test(StringCompare(oDn.GetStr().Utf8(), "a.few.sub-domains.example.org") == 0);
	
	Test(oDn.ToStr(oStr) == true);
	Test(StringCompare(oStr.Utf8(), "a.few.sub-domains.example.org") == 0);
	
	
	// ExplodeBySubdomains()
	
	// NOTE: `oDn` is already set
	Test(oDn.ExplodeBySubdomains(oSdns) == true);
	Test(oSdns.Count() == 5);
	Test(StringCompare(oSdns.Get(0).Utf8(), "org") == 0);
	Test(StringCompare(oSdns.Get(1).Utf8(), "example") == 0);
	Test(StringCompare(oSdns.Get(2).Utf8(), "sub-domains") == 0);
	Test(StringCompare(oSdns.Get(3).Utf8(), "few") == 0);
	Test(StringCompare(oSdns.Get(4).Utf8(), "a") == 0);
	
	oDn.Reset();
	Test(oDn.ExplodeBySubdomains(oSdns) == false);
	Test(oSdns.Count() == 0);
	
	
	return 0;
}

static int TestNetworkPort()
{
	PrintFn();
	
	
	acpl::NetworkPort oPort;
	
	
	Test(oPort.Get() == 0);
	
	oPort.Set(0x1234);
	Test(oPort.Get() == 0x1234);
	Test(oPort.Data()[0] == 0x12);
	Test(oPort.Data()[1] == 0x34);
	
	
	return 0;
}


static inline int _local_IsResolvedSuccessful(acpl::DomainName::ResolveStatus nResStat)
{
	switch (nResStat)
	{
		case acpl::DomainName::rsSuccess:
			return 0;
		
		case acpl::DomainName::rsSystemError:
			TestM(false, "system error");
			break;
		
		case acpl::DomainName::rsNotFound:
			TestM(false, "domain name not found");
			break;
		
		case acpl::DomainName::rsDnsTempFail:
			TestM(false, "DNS temporal failure");
			break;
		
		case acpl::DomainName::rsDnsPermFail:
			TestM(false, "DNS permanent failure");
			break;
	}
	
	Test(false);
}

template <class tType>
static inline void _local_PrintIpvxAddrs(const char *nDomainName, const char *nIpType, const tType &nIpvxAddrList)
{	
	PrintOut("%s address(es) for: %s\n", nIpType, nDomainName);
	
	for (acpl::SizeT i = 0; i < nIpvxAddrList.Count(); i++)
		PrintOut("    %s\n", acpl::String::IpAddrRef(nIpvxAddrList.Get(i)).Utf8())
}

static int TestResolver(const char *nDomainName)
{
	acpl::DomainName oDn;
	acpl::DomainName::Ipv4AddrList oIpv4List;
	acpl::DomainName::Ipv6AddrList oIpv6List;
	
	TestFM(oDn.Parse(acpl::String::Ref(nDomainName, true)) == true, "invalid domain name: %s", nDomainName);
	
	Test(_local_IsResolvedSuccessful(oDn.Resolve(oIpv4List, oIpv6List)) == 0);
	_local_PrintIpvxAddrs(nDomainName, "IPv4", oIpv4List);
	_local_PrintIpvxAddrs(nDomainName, "IPv6", oIpv6List);
	
	
	return 0;
}


SectionFuncMain(netaddr)
{
	if (argc < 1)
	{
		Test(TestNetworkAddr() == 0);
		Test(TestIpv4Addr() == 0);
		Test(TestIpv6Addr() == 0);
		Test(TestDomainName() == 0);
		Test(TestNetworkPort() == 0);
		
		PrintOut("All Passed!\n");
	}
	else
	if (argc == 2 && StringCompare(argv[0], "resolve") == 0)
	{
		Test(TestResolver(argv[1]) == 0)
	}
	else
		return PrintUnrecParams(acplDbgSrcLoc, lSectionName, argc, argv);
	
	return 0;
}

SectionFuncParamsBegin(netaddr)
SectionFuncParamsAdd("\"resolve\" domain_name", "Resolve a domain name into IPv4 and IPv6 addresses")
// SectionFuncParamsAdd("param [param [param ...]]", "description")
SectionFuncParamsEnd(netaddr)
