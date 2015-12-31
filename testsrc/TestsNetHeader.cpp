#include "Tests.h"

#include "../src/net/Header.h"


static inline bool _local_CmpCsum(const acpl::IpHeader::Checksum::DataType &nCsData, acpl::UInt8 nByte1, acpl::UInt8 nByte2)
{
	return (nCsData[0] == nByte1 && nCsData[1] == nByte2);
}

static inline int _local_TestCsum(const void *nBfr, acpl::SizeT nBfrSize, acpl::UInt8 nByte1, acpl::UInt8 nByte2)
{
	acpl::IpHeader::Checksum oCs;
	acpl::IpHeader::Checksum::DataType oCsData;
	
	oCs.Add(nBfr, nBfrSize);
	oCs.Calc(oCsData);
	Test(_local_CmpCsum(oCsData, nByte1, nByte2) == true);
	oCs.Add(oCsData, sizeof(oCsData));
	Test(oCs.Verify() == true);
	
	return 0;
}

template <class tArrayType>
static inline bool _local_ArrayIsZero(const tArrayType &nArray)
{
	for (acpl::SizeT i = sizeof(nArray); i-- != 0; )
		if (nArray[i] != 0)
			return false;
	
	return true;
}

template <class tArrayType>
static inline bool _local_ArrayCmp(const tArrayType &nArray1, const tArrayType &nArray2)
{
	for (acpl::SizeT i = sizeof(nArray1); i-- != 0; )
		if (nArray1[i] != nArray2[i])
			return false;
	
	return true;
}


static int TestIpHeader()
{
	PrintFn();
	
	
	// Protocol IDs
	
	Test(static_cast<acpl::UInt8>(acpl::IpHeader::piNone)    == 0);
	Test(static_cast<acpl::UInt8>(acpl::IpHeader::piHopOpt) == 0);
	Test(static_cast<acpl::UInt8>(acpl::IpHeader::piIcmp)    == 1);
	Test(static_cast<acpl::UInt8>(acpl::IpHeader::piIgmp)    == 2);
	Test(static_cast<acpl::UInt8>(acpl::IpHeader::piTcp)     == 6);
	Test(static_cast<acpl::UInt8>(acpl::IpHeader::piUdp)     == 17);
	Test(static_cast<acpl::UInt8>(acpl::IpHeader::piIcmpv6)  == 58);
	
	
	// Checksum calculator
	
	const acpl::UInt8 oBfr[] = {
			0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF,
			0xFE, 0xDC, 0xBA, 0x98, 0x76, 0x54, 0x32, 0x10
		};
	
	Test(_local_TestCsum(oBfr,  1, 0xFE, 0xFF) == 0);
	Test(_local_TestCsum(oBfr,  2, 0xFE, 0xDC) == 0);
	Test(_local_TestCsum(oBfr,  3, 0xB9, 0xDC) == 0);
	Test(_local_TestCsum(oBfr,  4, 0xB9, 0x75) == 0);
	Test(_local_TestCsum(oBfr,  5, 0x30, 0x75) == 0);
	Test(_local_TestCsum(oBfr,  6, 0x2F, 0xCA) == 0);
	Test(_local_TestCsum(oBfr,  7, 0x62, 0xC9) == 0);
	Test(_local_TestCsum(oBfr,  8, 0x61, 0xDA) == 0);
	Test(_local_TestCsum(oBfr,  9, 0x63, 0xD9) == 0);
	Test(_local_TestCsum(oBfr, 10, 0x62, 0xFD) == 0);
	Test(_local_TestCsum(oBfr, 11, 0xA8, 0xFC) == 0);
	Test(_local_TestCsum(oBfr, 12, 0xA8, 0x64) == 0);
	Test(_local_TestCsum(oBfr, 13, 0x32, 0x64) == 0);
	Test(_local_TestCsum(oBfr, 14, 0x32, 0x10) == 0);
	Test(_local_TestCsum(oBfr, 15, 0x00, 0x10) == 0);
	Test(_local_TestCsum(oBfr, 16, 0x00, 0x00) == 0);
	
	
	return 0;
}

static int TestIpv4Header()
{
	PrintFn();
	
	
	acpl::Ipv4Header oHead;
	acpl::Ipv4Addr oAddr;
	acpl::UInt8 oOpts[8];
	acpl::IpHeader::Checksum oCs;
	acpl::IpHeader::Checksum::DataType oCsData;
	
	
	// Subtypes
	
	Test(sizeof(acpl::Ipv4Header::DataType) == 20);
	
	Test(static_cast<acpl::UInt8>(acpl::Ipv4Header::tpRout)     == (0 << 5));
	Test(static_cast<acpl::UInt8>(acpl::Ipv4Header::tpPrio)     == (1 << 5));
	Test(static_cast<acpl::UInt8>(acpl::Ipv4Header::tpImmed)    == (2 << 5));
	Test(static_cast<acpl::UInt8>(acpl::Ipv4Header::tpFlash)    == (3 << 5));
	Test(static_cast<acpl::UInt8>(acpl::Ipv4Header::tpFlashOvr) == (4 << 5));
	Test(static_cast<acpl::UInt8>(acpl::Ipv4Header::tpCritEcp)  == (5 << 5));
	Test(static_cast<acpl::UInt8>(acpl::Ipv4Header::tpInwkCtrl) == (6 << 5));
	Test(static_cast<acpl::UInt8>(acpl::Ipv4Header::tpNwkCtrl)  == (7 << 5));
	
	Test(static_cast<acpl::UInt8>(acpl::Ipv4Header::tfDelay) == (1 << 4));
	Test(static_cast<acpl::UInt8>(acpl::Ipv4Header::tfTput)  == (1 << 3));
	Test(static_cast<acpl::UInt8>(acpl::Ipv4Header::tfRel)   == (1 << 2));
	
	Test(static_cast<acpl::UInt8>(acpl::Ipv4Header::ffDont) == (1 << 1));
	Test(static_cast<acpl::UInt8>(acpl::Ipv4Header::ffMore) == (1 << 0));
	
	
	// Init
	
	Test(oHead.Data()[0] == 0x40);
	Test(oHead.GetIhl() == 0x00);
	Test(oHead.GetTos() == 0x00);
	Test(oHead.GetLen() == 0);
	Test(oHead.GetId() == 0);
	Test(oHead.GetFragFlags() == 0x00);
	Test(oHead.GetFragOff() == 0);
	Test(oHead.GetTtl() == 0);
	Test(oHead.GetProto() == 0);
	Test(_local_ArrayIsZero(oHead.GetSrcAddr()) == true);
	Test(_local_ArrayIsZero(oHead.GetDstAddr()) == true);
	
	
	// {G,S}etter for IHL (compound data block)
	
	oHead.SetIhl(0);
	Test(oHead.Data()[0] == 0x40);
	Test(oHead.GetIhl() == 0x00);
	oHead.SetIhl(0x0F);
	Test(oHead.Data()[0] == 0x4F);
	Test(oHead.GetIhl() == 0x0F);
	oHead.SetIhl(0xFF); // trimmed
	Test(oHead.Data()[0] == 0x4F);
	Test(oHead.GetIhl() == 0x0F);
	oHead.SetIhl(0x10); // trimmed
	Test(oHead.Data()[0] == 0x40);
	Test(oHead.GetIhl() == 0x00);
	
	
	// {G,S}etter for TOS
	
	oHead.SetTos(0xFC);
	Test(oHead.GetTos() == 0xFC);
	oHead.SetTos(0xFF);
	Test(oHead.GetTos() == 0xFC);
	
	
	// {G,S}etters for fragment (compound data block)
	
	oHead.SetFragFlags(0x00);
	Test(oHead.GetFragFlags() == 0x00);
	Test(oHead.GetFragOff() == 0x0000);
	oHead.SetFragFlags(0x03);
	Test(oHead.GetFragFlags() == 0x03);
	Test(oHead.GetFragOff() == 0x0000);
	oHead.SetFragFlags(0x07); // trimmed
	Test(oHead.GetFragFlags() == 0x03);
	Test(oHead.GetFragOff() == 0x0000);
	oHead.SetFragFlags(0x04); // trimmed
	Test(oHead.GetFragFlags() == 0x00);
	Test(oHead.GetFragOff() == 0x0000);
	
	oHead.SetFragOff(0x0000);
	Test(oHead.GetFragFlags() == 0x00);
	Test(oHead.GetFragOff() == 0x0000);
	oHead.SetFragOff(0x1FFF);
	Test(oHead.GetFragFlags() == 0x00);
	Test(oHead.GetFragOff() == 0x1FFF);
	oHead.SetFragOff(0x3FFF); // trimmed
	Test(oHead.GetFragFlags() == 0x00);
	Test(oHead.GetFragOff() == 0x1FFF);
	oHead.SetFragOff(0x2000); // trimmed
	Test(oHead.GetFragFlags() == 0x00);
	Test(oHead.GetFragOff() == 0x0000);
	
	
	// {G,S}etters (general), data check
	
	oHead.Reset();
	Test(oHead.Data()[ 0] == 0x40);
	
	oHead.SetIhl(0x05);
	oHead.SetTos(0x68);
	oHead.SetLen(0x1234);
	oHead.SetId(0x4321);
	oHead.SetFragFlags(0x02);
	oHead.SetFragOff(0x1234);
	oHead.SetTtl(0x56);
	oHead.SetProto(0x78);
	oAddr.Set(10, 20, 100, 200).Get(oHead.GetSrcAddr());
	oAddr.Set(100, 200, 10, 20).Get(oHead.GetDstAddr());
	
	Test(oHead.GetIhl() == 0x05);
	Test(oHead.GetTos() == 0x68);
	Test(oHead.GetLen() == 0x1234);
	Test(oHead.GetId() == 0x4321);
	Test(oHead.GetFragFlags() == 0x02);
	Test(oHead.GetFragOff() == 0x1234);
	Test(oHead.GetTtl() == 0x56);
	Test(oHead.GetProto() == 0x78);
	Test(_local_ArrayCmp<const acpl::Ipv4Addr::DataType>(oHead.GetSrcAddr(), oAddr.Set(10, 20, 100, 200).Data()) == true);
	Test(_local_ArrayCmp<const acpl::Ipv4Addr::DataType>(oHead.GetDstAddr(), oAddr.Set(100, 200, 10, 20).Data()) == true);
	
	Test(oHead.Data()[ 0] == 0x45);
	Test(oHead.Data()[ 1] == 0x68);
	Test(oHead.Data()[ 2] == 0x12);
	Test(oHead.Data()[ 3] == 0x34);
	Test(oHead.Data()[ 4] == 0x43);
	Test(oHead.Data()[ 5] == 0x21);
	Test(oHead.Data()[ 6] == 0x52);
	Test(oHead.Data()[ 7] == 0x34);
	Test(oHead.Data()[ 8] == 0x56);
	Test(oHead.Data()[ 9] == 0x78);
	Test(oHead.Data()[10] == 0x00); // csum
	Test(oHead.Data()[11] == 0x00); // csum
	Test(oHead.Data()[12] == 0x0A);
	Test(oHead.Data()[13] == 0x14);
	Test(oHead.Data()[14] == 0x64);
	Test(oHead.Data()[15] == 0xC8);
	Test(oHead.Data()[16] == 0x64);
	Test(oHead.Data()[17] == 0xC8);
	Test(oHead.Data()[18] == 0x0A);
	Test(oHead.Data()[19] == 0x14);
	
	
	// Setup, data check, checksum (no opts)
	
	oHead.Reset();
	oHead.SetIhl(5);
	oHead.SetTos(0x00);
	oHead.SetLen(115);
	oHead.SetId(0);
	oHead.SetFragFlags(acpl::Ipv4Header::ffDont);
	oHead.SetFragOff(0);
	oHead.SetTtl(64);
	oHead.SetProto(17); // 17 == IPPROTO_UDP
	oAddr.Set(192, 168, 0, 1).Get(oHead.GetSrcAddr());
	oAddr.Set(192, 168, 0, 199).Get(oHead.GetDstAddr());
	
	Test(oHead.Data()[ 0] == 0x45);
	Test(oHead.Data()[ 1] == 0x00);
	Test(oHead.Data()[ 2] == 0x00);
	Test(oHead.Data()[ 3] == 0x73);
	Test(oHead.Data()[ 4] == 0x00);
	Test(oHead.Data()[ 5] == 0x00);
	Test(oHead.Data()[ 6] == 0x40);
	Test(oHead.Data()[ 7] == 0x00);
	Test(oHead.Data()[ 8] == 0x40);
	Test(oHead.Data()[ 9] == 0x11);
	Test(oHead.Data()[10] == 0x00); // csum
	Test(oHead.Data()[11] == 0x00); // csum
	Test(oHead.Data()[12] == 0xC0);
	Test(oHead.Data()[13] == 0xA8);
	Test(oHead.Data()[14] == 0x00);
	Test(oHead.Data()[15] == 0x01);
	Test(oHead.Data()[16] == 0xC0);
	Test(oHead.Data()[17] == 0xA8);
	Test(oHead.Data()[18] == 0x00);
	Test(oHead.Data()[19] == 0xC7);
	
	oCs.Reset();
	oCs.DataClear(oHead.GetCsum());
	oCs.Add(oHead.Data(), sizeof(oHead.Data()));
	oCs.Calc(oHead.GetCsum());
	Test(_local_CmpCsum(oHead.GetCsum(), 0xB8, 0x61) == true);
	oCs.Add(oHead.GetCsum(), sizeof(oHead.GetCsum()));
	Test(oCs.Verify() == true);
	
	
	// Setup Opts
	
	oOpts[0] = 0;
	oOpts[1] = 1;
	oOpts[2] = 0;
	oOpts[3] = 2;
	
	
	// Setup, data check, checksum (with opts) [everything already set]
	
	oCs.Reset();
	oCs.DataClear(oHead.GetCsum());
	oCs.Add(oHead.Data(), sizeof(oHead.Data()));
	oCs.Add(oOpts, 1);
	oCs.Calc(oHead.GetCsum());
	Test(_local_CmpCsum(oHead.GetCsum(), 0xB8, 0x61) == true);
	oCs.Add(oHead.GetCsum(), sizeof(oHead.GetCsum()));
	Test(oCs.Verify() == true);
	
	oCs.Reset();
	oCs.DataClear(oHead.GetCsum());
	oCs.Add(oHead.Data(), sizeof(oHead.Data()));
	oCs.Add(oOpts, 2);
	oCs.Calc(oHead.GetCsum());
	Test(_local_CmpCsum(oHead.GetCsum(), 0xB8, 0x60) == true);
	oCs.Add(oHead.GetCsum(), sizeof(oHead.GetCsum()));
	Test(oCs.Verify() == true);
	
	oCs.Reset();
	oCs.DataClear(oHead.GetCsum());
	oCs.Add(oHead.Data(), sizeof(oHead.Data()));
	oCs.Add(oOpts, 3);
	oCs.Calc(oHead.GetCsum());
	Test(_local_CmpCsum(oHead.GetCsum(), 0xB8, 0x60) == true);
	oCs.Add(oHead.GetCsum(), sizeof(oHead.GetCsum()));
	Test(oCs.Verify() == true);
	
	oCs.Reset();
	oCs.DataClear(oHead.GetCsum());
	oCs.Add(oHead.Data(), sizeof(oHead.Data()));
	oCs.Add(oOpts, 4);
	oCs.Calc(oHead.GetCsum());
	Test(_local_CmpCsum(oHead.GetCsum(), 0xB8, 0x5E) == true);
	oCs.Add(oHead.GetCsum(), sizeof(oHead.GetCsum()));
	Test(oCs.Verify() == true);
	
	
	// TCP pseudo-header checksum [everything accept Protocol field already set]
	
	oHead.SetProto(acpl::IpHeader::piTcp);
	
	oCs.Reset();
	oCs.DataClear(oCsData);
	oHead.AddTcpPshCsum(oCs, 0x0000);
	oCs.Calc(oCsData);
	Test(_local_CmpCsum(oCsData, 0x7D, 0xE0) == true);
	oCs.Add(oCsData, sizeof(oCsData));
	Test(oCs.Verify() == true);
	
	oCs.Reset();
	oCs.DataClear(oCsData);
	oHead.AddTcpPshCsum(oCs, 0x0034);
	oCs.Calc(oCsData);
	Test(_local_CmpCsum(oCsData, 0x7D, 0xAC) == true);
	oCs.Add(oCsData, sizeof(oCsData));
	Test(oCs.Verify() == true);
	
	oCs.Reset();
	oCs.DataClear(oCsData);
	oHead.AddTcpPshCsum(oCs, 0x1234);
	oCs.Calc(oCsData);
	Test(_local_CmpCsum(oCsData, 0x6B, 0xAC) == true);
	oCs.Add(oCsData, sizeof(oCsData));
	Test(oCs.Verify() == true);
	
	
	return 0;
}

static int TestIpv6Header()
{
	PrintFn();
	
	
	acpl::Ipv6Header oHead;
	acpl::Ipv6Addr oAddr;
	acpl::IpHeader::Checksum oCs;
	acpl::IpHeader::Checksum::DataType oCsData;
	
	
	// Subtypes
	
	Test(sizeof(acpl::Ipv6Header::DataType) == 40);
	
	
	// Init
	
	Test(oHead.Data()[0] == 0x60);
	Test(oHead.GetTrac() == 0x00);
	Test(oHead.GetFlab() == 0x00000000);
	Test(oHead.GetPlen() == 0);
	Test(oHead.GetNeh() == 0x00);
	Test(oHead.GetHol() == 0x00);
	Test(_local_ArrayIsZero(oHead.GetSrcAddr()) == true);
	Test(_local_ArrayIsZero(oHead.GetDstAddr()) == true);
	
	
	// {G,S}etters for traffic class and flow label (compound data block)
	
	oHead.SetTrac(0x00);
	Test(oHead.Data()[0] == 0x60);
	Test(oHead.GetTrac() == 0x00);
	Test(oHead.GetFlab() == 0x00000000);
	oHead.SetTrac(0x0F);
	Test(oHead.Data()[0] == 0x60);
	Test(oHead.GetTrac() == 0x0F);
	Test(oHead.GetFlab() == 0x00000000);
	oHead.SetTrac(0xF0);
	Test(oHead.Data()[0] == 0x6F);
	Test(oHead.GetTrac() == 0xF0);
	Test(oHead.GetFlab() == 0x00000000);
	oHead.SetTrac(0xFF);
	Test(oHead.Data()[0] == 0x6F);
	Test(oHead.GetTrac() == 0xFF);
	Test(oHead.GetFlab() == 0x00000000);
	oHead.SetTrac(0x00);
	Test(oHead.Data()[0] == 0x60);
	Test(oHead.GetTrac() == 0x00);
	Test(oHead.GetFlab() == 0x00000000);
	
	oHead.SetFlab(0x00000000);
	Test(oHead.Data()[0] == 0x60);
	Test(oHead.GetTrac() == 0x00);
	Test(oHead.GetFlab() == 0x00000000);
	oHead.SetFlab(0x000FFFFF);
	Test(oHead.Data()[0] == 0x60);
	Test(oHead.GetTrac() == 0x00);
	Test(oHead.GetFlab() == 0x000FFFFF);
	oHead.SetFlab(0xFFFFFFFF); // trimmed
	Test(oHead.Data()[0] == 0x60);
	Test(oHead.GetTrac() == 0x00);
	Test(oHead.GetFlab() == 0x000FFFFF);
	oHead.SetFlab(0x00100000); // trimmed
	Test(oHead.Data()[0] == 0x60);
	Test(oHead.GetTrac() == 0x00);
	Test(oHead.GetFlab() == 0x00000000);
	
	
	// {G,S}etters (general), data check
	
	oHead.Reset();
	Test(oHead.Data()[ 0] == 0x60);
	
	oHead.SetTrac(0x12);
	oHead.SetFlab(0x00034567);
	oHead.SetPlen(0x4321);
	oHead.SetNeh(0x12);
	oHead.SetHol(0x34);
	oAddr.Set(0xFEDC, 0xBA98, 0x7654, 0x3210, 0xFEDC, 0xBA98, 0x7654, 0x3210).Get(oHead.GetSrcAddr());
	oAddr.Set(0x0123, 0x4567, 0x89AB, 0xCDEF, 0x0123, 0x4567, 0x89AB, 0xCDEF).Get(oHead.GetDstAddr());
	
	Test(oHead.GetTrac() == 0x12);
	Test(oHead.GetFlab() == 0x00034567);
	Test(oHead.GetPlen() == 0x4321);
	Test(oHead.GetNeh() == 0x12);
	Test(oHead.GetHol() == 0x34);
	Test(_local_ArrayCmp<const acpl::Ipv6Addr::DataType>(oHead.GetSrcAddr(), oAddr.Set(0xFEDC, 0xBA98, 0x7654, 0x3210, 0xFEDC, 0xBA98, 0x7654, 0x3210).Data()) == true);
	Test(_local_ArrayCmp<const acpl::Ipv6Addr::DataType>(oHead.GetDstAddr(), oAddr.Set(0x0123, 0x4567, 0x89AB, 0xCDEF, 0x0123, 0x4567, 0x89AB, 0xCDEF).Data()) == true);
	
	Test(oHead.Data()[ 0] == 0x61);
	Test(oHead.Data()[ 1] == 0x23);
	Test(oHead.Data()[ 2] == 0x45);
	Test(oHead.Data()[ 3] == 0x67);
	Test(oHead.Data()[ 4] == 0x43);
	Test(oHead.Data()[ 5] == 0x21);
	Test(oHead.Data()[ 6] == 0x12);
	Test(oHead.Data()[ 7] == 0x34);
	Test(oHead.Data()[ 8] == 0xFE); // srcaddr
	Test(oHead.Data()[ 9] == 0xDC);
	Test(oHead.Data()[10] == 0xBA);
	Test(oHead.Data()[11] == 0x98);
	Test(oHead.Data()[12] == 0x76);
	Test(oHead.Data()[13] == 0x54);
	Test(oHead.Data()[14] == 0x32);
	Test(oHead.Data()[15] == 0x10);
	Test(oHead.Data()[16] == 0xFE);
	Test(oHead.Data()[17] == 0xDC);
	Test(oHead.Data()[18] == 0xBA);
	Test(oHead.Data()[19] == 0x98);
	Test(oHead.Data()[20] == 0x76);
	Test(oHead.Data()[21] == 0x54);
	Test(oHead.Data()[22] == 0x32);
	Test(oHead.Data()[23] == 0x10);
	Test(oHead.Data()[24] == 0x01); // dstaddr
	Test(oHead.Data()[25] == 0x23);
	Test(oHead.Data()[26] == 0x45);
	Test(oHead.Data()[27] == 0x67);
	Test(oHead.Data()[28] == 0x89);
	Test(oHead.Data()[29] == 0xAB);
	Test(oHead.Data()[30] == 0xCD);
	Test(oHead.Data()[31] == 0xEF);
	Test(oHead.Data()[32] == 0x01);
	Test(oHead.Data()[33] == 0x23);
	Test(oHead.Data()[34] == 0x45);
	Test(oHead.Data()[35] == 0x67);
	Test(oHead.Data()[36] == 0x89);
	Test(oHead.Data()[37] == 0xAB);
	Test(oHead.Data()[38] == 0xCD);
	Test(oHead.Data()[39] == 0xEF);
	
	
	// TCP pseudo-header checksum [everything accept Protocol field already set]
	
	oHead.SetNeh(acpl::IpHeader::piTcp);
	
	oCs.Reset();
	oCs.DataClear(oCsData);
	oHead.AddTcpPshCsum(oCs, 0x00000000);
	oCs.Calc(oCsData);
	Test(_local_CmpCsum(oCsData, 0xFF, 0xF9) == true);
	oCs.Add(oCsData, sizeof(oCsData));
	Test(oCs.Verify() == true);
	
	oCs.Reset();
	oCs.DataClear(oCsData);
	oHead.AddTcpPshCsum(oCs, 0x00000067);
	oCs.Calc(oCsData);
	Test(_local_CmpCsum(oCsData, 0xFF, 0x92) == true);
	oCs.Add(oCsData, sizeof(oCsData));
	Test(oCs.Verify() == true);
	
	oCs.Reset();
	oCs.DataClear(oCsData);
	oHead.AddTcpPshCsum(oCs, 0x00004567);
	oCs.Calc(oCsData);
	Test(_local_CmpCsum(oCsData, 0xBA, 0x92) == true);
	oCs.Add(oCsData, sizeof(oCsData));
	Test(oCs.Verify() == true);
	
	oCs.Reset();
	oCs.DataClear(oCsData);
	oHead.AddTcpPshCsum(oCs, 0x00234567);
	oCs.Calc(oCsData);
	Test(_local_CmpCsum(oCsData, 0xBA, 0x6F) == true);
	oCs.Add(oCsData, sizeof(oCsData));
	Test(oCs.Verify() == true);
	
	oCs.Reset();
	oCs.DataClear(oCsData);
	oHead.AddTcpPshCsum(oCs, 0x01234567);
	oCs.Calc(oCsData);
	Test(_local_CmpCsum(oCsData, 0xB9, 0x6F) == true);
	oCs.Add(oCsData, sizeof(oCsData));
	Test(oCs.Verify() == true);
	
	
	return 0;
}

static int TestIpHeaderTcp()
{
	PrintFn();
	
	
	acpl::IpHeaderTcp oHead;
	acpl::NetworkPort oPort;
	
	
	// Subtypes
	
	Test(sizeof(acpl::IpHeaderTcp::DataType) == 20);
	
	Test(static_cast<acpl::UInt8>(acpl::IpHeaderTcp::cbUrg) == (1 << 5));
	Test(static_cast<acpl::UInt8>(acpl::IpHeaderTcp::cbAck) == (1 << 4));
	Test(static_cast<acpl::UInt8>(acpl::IpHeaderTcp::cbPsh) == (1 << 3));
	Test(static_cast<acpl::UInt8>(acpl::IpHeaderTcp::cbRst) == (1 << 2));
	Test(static_cast<acpl::UInt8>(acpl::IpHeaderTcp::cbSyn) == (1 << 1));
	Test(static_cast<acpl::UInt8>(acpl::IpHeaderTcp::cbFin) == (1 << 0));
	
	
	// Init
	
	Test(oPort.Set(oHead.GetSrcPort()).Get() == 0);
	Test(oPort.Set(oHead.GetDstPort()).Get() == 0);
	Test(oHead.GetSeqNum() == 0);
	Test(oHead.GetAckNum() == 0);
	Test(oHead.GetDataOff() == 0);
	Test(oHead.GetCtrlBits() == 0);
	Test(oHead.GetWndw() == 0);
	Test(oHead.GetUrgPtr() == 0);
	
	
	// {G,S}etters for data offset (limited data block)
	
	oHead.SetDataOff(0x00);
	Test(oHead.GetDataOff() == 0x00);
	oHead.SetDataOff(0x0F);
	Test(oHead.GetDataOff() == 0x0F);
	oHead.SetDataOff(0x1F); // trimmed
	Test(oHead.GetDataOff() == 0x0F);
	oHead.SetDataOff(0x10); // trimmed
	Test(oHead.GetDataOff() == 0x00);
	
	
	// {G,S}etters for control bits (limited data block)
	
	oHead.SetCtrlBits(0x00);
	Test(oHead.GetCtrlBits() == 0x00);
	oHead.SetCtrlBits(0x3F);
	Test(oHead.GetCtrlBits() == 0x3F);
	oHead.SetCtrlBits(0x7F); // trimmed
	Test(oHead.GetCtrlBits() == 0x3F);
	oHead.SetCtrlBits(0x40); // trimmed
	Test(oHead.GetCtrlBits() == 0x00);
	
	
	// {G,S}etters (general), data check
	
	oPort.Set(0x0123).Get(oHead.GetSrcPort());
	oPort.Set(0x4567).Get(oHead.GetDstPort());
	oHead.SetSeqNum(0x89ABCDEF);
	oHead.SetAckNum(0xFEDCAB98);
	oHead.SetDataOff(0x0F);
	oHead.SetCtrlBits(0x3F);
	oHead.SetWndw(0x7654);
	oHead.SetUrgPtr(0x3210);
	
	Test(oHead.Data()[ 0] == 0x01);
	Test(oHead.Data()[ 1] == 0x23);
	Test(oHead.Data()[ 2] == 0x45);
	Test(oHead.Data()[ 3] == 0x67);
	Test(oHead.Data()[ 4] == 0x89);
	Test(oHead.Data()[ 5] == 0xAB);
	Test(oHead.Data()[ 6] == 0xCD);
	Test(oHead.Data()[ 7] == 0xEF);
	Test(oHead.Data()[ 8] == 0xFE);
	Test(oHead.Data()[ 9] == 0xDC);
	Test(oHead.Data()[10] == 0xAB);
	Test(oHead.Data()[11] == 0x98);
	Test(oHead.Data()[12] == 0xF0);
	Test(oHead.Data()[13] == 0x3F);
	Test(oHead.Data()[14] == 0x76);
	Test(oHead.Data()[15] == 0x54);
	Test(oHead.Data()[16] == 0x00); // csum
	Test(oHead.Data()[17] == 0x00); // csum
	Test(oHead.Data()[18] == 0x32);
	Test(oHead.Data()[19] == 0x10);
	
	
	return 0;
}

static int TestIpHeaderUdp()
{
	PrintFn();
	
	
	acpl::IpHeaderUdp oHead;
	acpl::NetworkPort oPort;
	
	
	// Subtypes
	
	Test(sizeof(acpl::IpHeaderUdp::DataType) == 8);
	
	
	// Init
	
	Test(oPort.Set(oHead.GetSrcPort()).Get() == 0);
	Test(oPort.Set(oHead.GetDstPort()).Get() == 0);
	Test(oHead.GetLen() == 0);
	
	
	// {G,S}etters (general), data check
	
	oPort.Set(0x0123).Get(oHead.GetSrcPort());
	oPort.Set(0x4567).Get(oHead.GetDstPort());
	oHead.SetLen(0x89AB);
	
	Test(oHead.Data()[0] == 0x01);
	Test(oHead.Data()[1] == 0x23);
	Test(oHead.Data()[2] == 0x45);
	Test(oHead.Data()[3] == 0x67);
	Test(oHead.Data()[4] == 0x89);
	Test(oHead.Data()[5] == 0xAB);
	Test(oHead.Data()[6] == 0x00); // csum
	Test(oHead.Data()[7] == 0x00); // csum
	
	
	return 0;
}

static int TestIpHeaderIcmp()
{
	PrintFn();
	
	
	acpl::IpHeaderIcmp oHead;
	
	
	// Subtypes
	
	Test(sizeof(acpl::IpHeaderIcmp::DataType) == 4);
	
	Test(static_cast<acpl::UInt8>(acpl::IpHeaderIcmp::mtEchoRpl)   == 0);
	Test(static_cast<acpl::UInt8>(acpl::IpHeaderIcmp::mtDstUnrch)  == 3);
	Test(static_cast<acpl::UInt8>(acpl::IpHeaderIcmp::mtSrcQuench) == 4);
	Test(static_cast<acpl::UInt8>(acpl::IpHeaderIcmp::mtRedir)     == 5);
	Test(static_cast<acpl::UInt8>(acpl::IpHeaderIcmp::mtEcho)      == 8);
	Test(static_cast<acpl::UInt8>(acpl::IpHeaderIcmp::mtTimeExc)   == 11);
	Test(static_cast<acpl::UInt8>(acpl::IpHeaderIcmp::mtParamProb) == 12);
	Test(static_cast<acpl::UInt8>(acpl::IpHeaderIcmp::mtTstamp)    == 13);
	Test(static_cast<acpl::UInt8>(acpl::IpHeaderIcmp::mtTstampRpl) == 14);
	Test(static_cast<acpl::UInt8>(acpl::IpHeaderIcmp::mtInfoReq)   == 15);
	Test(static_cast<acpl::UInt8>(acpl::IpHeaderIcmp::mtInfoRpl)   == 16);
	
	
	// Init
	
	Test(oHead.GetType() == 0);
	Test(oHead.GetCode() == 0);
	
	
	// {G,S}etters (general), data check
	
	oHead.SetType(0x01);
	oHead.SetCode(0x23);
	
	Test(oHead.Data()[0] == 0x01);
	Test(oHead.Data()[1] == 0x23);
	Test(oHead.Data()[2] == 0x00); // csum
	Test(oHead.Data()[3] == 0x00); // csum
	
	
	return 0;
}

static int TestIpHeaderIgmp()
{
	PrintFn();
	
	
	acpl::IpHeaderIgmp oHead;
	
	
	// Subtypes
	
	Test(sizeof(acpl::IpHeaderIgmp::DataType) == 4);
	
	Test(static_cast<acpl::UInt8>(acpl::IpHeaderIgmp::mtCreateReq)   == 0x01);
	Test(static_cast<acpl::UInt8>(acpl::IpHeaderIgmp::mtCreateRpl)   == 0x02);
	Test(static_cast<acpl::UInt8>(acpl::IpHeaderIgmp::mtJoinReq)     == 0x03);
	Test(static_cast<acpl::UInt8>(acpl::IpHeaderIgmp::mtJoinRpl)     == 0x04);
	Test(static_cast<acpl::UInt8>(acpl::IpHeaderIgmp::mtLeaveReq)    == 0x05);
	Test(static_cast<acpl::UInt8>(acpl::IpHeaderIgmp::mtLeaveRpl)    == 0x06);
	Test(static_cast<acpl::UInt8>(acpl::IpHeaderIgmp::mtConfirmReq)  == 0x07);
	Test(static_cast<acpl::UInt8>(acpl::IpHeaderIgmp::mtConfirmRpl)  == 0x08);
	Test(static_cast<acpl::UInt8>(acpl::IpHeaderIgmp::mtHmembQry)    == 0x11);
	Test(static_cast<acpl::UInt8>(acpl::IpHeaderIgmp::mtHmembRpt)    == 0x12);
	Test(static_cast<acpl::UInt8>(acpl::IpHeaderIgmp::mtDvmrp)       == 0x13);
	Test(static_cast<acpl::UInt8>(acpl::IpHeaderIgmp::mtPim)         == 0x14);
	Test(static_cast<acpl::UInt8>(acpl::IpHeaderIgmp::mtCiscoTm)     == 0x15);
	Test(static_cast<acpl::UInt8>(acpl::IpHeaderIgmp::mtHmembRptv2)  == 0x16);
	Test(static_cast<acpl::UInt8>(acpl::IpHeaderIgmp::mtLeavev2)     == 0x17);
	Test(static_cast<acpl::UInt8>(acpl::IpHeaderIgmp::mtMcastTrResp) == 0x1E);
	Test(static_cast<acpl::UInt8>(acpl::IpHeaderIgmp::mtMcastTr)     == 0x1F);
	Test(static_cast<acpl::UInt8>(acpl::IpHeaderIgmp::mtHmembRptv3)  == 0x22);
	Test(static_cast<acpl::UInt8>(acpl::IpHeaderIgmp::mtMcastRtrAdv) == 0x30);
	Test(static_cast<acpl::UInt8>(acpl::IpHeaderIgmp::mtMcastRtrSol) == 0x31);
	Test(static_cast<acpl::UInt8>(acpl::IpHeaderIgmp::mtMcastRtrTer) == 0x32);
	
	
	// Init
	
	Test(oHead.GetType() == 0);
	Test(oHead.GetCode() == 0);
	
	
	// {G,S}etters (general), data check
	
	oHead.SetType(0x01);
	oHead.SetCode(0x23);
	
	Test(oHead.Data()[0] == 0x01);
	Test(oHead.Data()[1] == 0x23);
	Test(oHead.Data()[2] == 0x00); // csum
	Test(oHead.Data()[3] == 0x00); // csum
	
	
	return 0;
}

static int TestIpHeaderIcmpv6()
{
	PrintFn();
	
	
	acpl::IpHeaderIcmpv6 oHead;
	
	
	// Subtypes
	
	Test(sizeof(acpl::IpHeaderIcmpv6::DataType) == 4);
	
	Test(static_cast<acpl::UInt8>(acpl::IpHeaderIcmpv6::mtDstUnrch)  == 1);
	Test(static_cast<acpl::UInt8>(acpl::IpHeaderIcmpv6::mtPktTooBig)  == 2);
	Test(static_cast<acpl::UInt8>(acpl::IpHeaderIcmpv6::mtTimeExc)   == 3);
	Test(static_cast<acpl::UInt8>(acpl::IpHeaderIcmpv6::mtParamProb) == 4);
	Test(static_cast<acpl::UInt8>(acpl::IpHeaderIcmpv6::mtEchoReq)   == 128);
	Test(static_cast<acpl::UInt8>(acpl::IpHeaderIcmpv6::mtEchoRpl)   == 129);
	
	
	// Init
	
	Test(oHead.GetType() == 0);
	Test(oHead.GetCode() == 0);
	
	
	// {G,S}etters (general), data check
	
	oHead.SetType(0x01);
	oHead.SetCode(0x23);
	
	Test(oHead.Data()[0] == 0x01);
	Test(oHead.Data()[1] == 0x23);
	Test(oHead.Data()[2] == 0x00); // csum
	Test(oHead.Data()[3] == 0x00); // csum
	
	
	return 0;
}


SectionFuncMain(netheader)
{
	if (argc < 1)
	{
		Test(TestIpHeader() == 0);
		Test(TestIpv4Header() == 0);
		Test(TestIpv6Header() == 0);
		Test(TestIpHeaderTcp() == 0);
		Test(TestIpHeaderUdp() == 0);
		Test(TestIpHeaderIcmp() == 0);
		Test(TestIpHeaderIgmp() == 0);
		Test(TestIpHeaderIcmpv6() == 0);
		
		PrintOut("All Passed!\n");
	}
	else
		return PrintUnrecParams(acplDbgSrcLoc, lSectionName, argc, argv);
	
	return 0;
}

SectionFuncParamsBegin(netheader)
// SectionFuncParamsAdd("param [param [param ...]]", "description")
SectionFuncParamsEnd(netheader)
