#ifndef ACPL_NET_HEADER_H
#define ACPL_NET_HEADER_H

#include "Address.h"

namespace acpl
{
	class IpHeader
	{
		public:
			enum ProtocolId
			{
				piNone = 0,
				piHopOpt = 0,
				piIcmp = 1,
				piIgmp = 2,
				piTcp = 6,
				piUdp = 17,
				piIcmpv6 = 58
			};
			
			class Checksum
			{
				public:
					typedef acpl::UInt8 DataType[2];
				
				private:
					acpl::UInt32 mSum;
					
				public:
					inline Checksum() { this->Reset(); }
					inline virtual ~Checksum() { }
					
					inline void Reset() { this->mSum = 0; }
					inline void DataClear(acpl::IpHeader::Checksum::DataType &nCsum) { nCsum[0] = 0; nCsum[1] = 0; }
					
					void Add(const void *nBfr, acpl::SizeT nBfrLen);
					void Calc(acpl::IpHeader::Checksum::DataType &nCsum);
					bool Verify();
			};
	};
	
	class Ipv4Header
	{
			acplHideClassCopy(Ipv4Header);
		
		// http://tools.ietf.org/html/rfc791
		
		private:
			struct Structure
			{
				acpl::UInt8 sVerIhl;
				acpl::UInt8 sTos;
				acpl::UInt8 sLen[2];
				acpl::UInt8 sId[2];
				acpl::UInt8 sFrag[2];
				acpl::UInt8 sTtl;
				acpl::UInt8 sProto;
				acpl::IpHeader::Checksum::DataType sCsum;
				acpl::Ipv4Addr::DataType sSrcAddr;
				acpl::Ipv4Addr::DataType sDstAddr;
			};
		
		public:
			typedef acpl::UInt8 DataType[sizeof(acpl::Ipv4Header::Structure)];
			
			enum TosPrec
			{
				tpRout     = 0x00, // 000xxxxx
				tpPrio     = 0x20, // 001xxxxx
				tpImmed    = 0x40, // 010xxxxx
				tpFlash    = 0x60, // 011xxxxx
				tpFlashOvr = 0x80, // 100xxxxx
				tpCritEcp  = 0xA0, // 101xxxxx
				tpInwkCtrl = 0xC0, // 110xxxxx
				tpNwkCtrl  = 0xE0  // 111xxxxx
			};
			
			enum TosFlags
			{
				tfDelay    = 0x10, // xxx1xxxx
				tfTput     = 0x08, // xxxx1xxx
				tfRel      = 0x04  // xxxxx1xx
			};
			
			enum FragFlags
			{
				ffDont = 0x02, // x1x.....
				ffMore = 0x01  // xx1.....
			};
		
		private:
			union
			{
				acpl::Ipv4Header::Structure uStruct;
				acpl::Ipv4Header::DataType  uBytes;
			} mData;
		
		public:
			inline Ipv4Header() { this->Reset(); }
			inline virtual ~Ipv4Header() { this->Reset(); }
			
			inline acpl::Ipv4Header::DataType &Data() { return this->mData.uBytes; }
			inline const acpl::Ipv4Header::DataType &Data() const { return this->mData.uBytes; }
			
			inline void Reset()
			{
				acpl::Mem::Set(this->mData.uBytes, 0, sizeof(this->mData.uBytes));
				this->mData.uStruct.sVerIhl = 0x40;
			}
			
			inline acpl::UInt8 GetIhl() const { return (this->mData.uStruct.sVerIhl & 0x0F); }
			inline void SetIhl(acpl::UInt8 nIhl) { this->mData.uStruct.sVerIhl = 0x40 | (nIhl & 0x0F); }
			
			inline acpl::UInt8 GetTos() const { return this->mData.uStruct.sTos; }
			inline void SetTos(acpl::UInt8 nTos) { this->mData.uStruct.sTos = (nTos & 0xFC); }
			
			inline acpl::UInt16 GetLen() const { return acpl::Endian::FromBig<acpl::UInt16>(this->mData.uStruct.sLen); }
			inline void SetLen(acpl::UInt16 nLen) { acpl::Endian::ToBig(nLen, this->mData.uStruct.sLen); }
			
			inline acpl::UInt16 GetId() const { return acpl::Endian::FromBig<acpl::UInt16>(this->mData.uStruct.sId); }
			inline void SetId(acpl::UInt16 nId) { acpl::Endian::ToBig(nId, this->mData.uStruct.sId); }
			
			inline acpl::UInt8 GetFragFlags() const { return (acpl::Endian::FromBig<acpl::UInt8>(this->mData.uStruct.sFrag) >> 5); }
			inline void SetFragFlags(acpl::UInt8 nFragFlags) { acpl::Endian::ToBig<acpl::UInt8>((nFragFlags << 5), 0x60, this->mData.uStruct.sFrag); }
			inline acpl::UInt16 GetFragOff() const { return acpl::Endian::FromBig<acpl::UInt16>(0x1FFF, this->mData.uStruct.sFrag); }
			inline void SetFragOff(acpl::UInt16 nFragOff) { acpl::Endian::ToBig(nFragOff, 0x1FFF, this->mData.uStruct.sFrag); }
			
			inline acpl::UInt8 GetTtl() const { return this->mData.uStruct.sTtl; }
			inline void SetTtl(acpl::UInt8 nTtl) { this->mData.uStruct.sTtl = nTtl; }
			
			inline acpl::UInt8 GetProto() const { return this->mData.uStruct.sProto; }
			inline void SetProto(acpl::UInt8 nProto) { this->mData.uStruct.sProto = nProto; }
			
			inline acpl::IpHeader::Checksum::DataType &GetCsum() { return this->mData.uStruct.sCsum; }
			inline const acpl::IpHeader::Checksum::DataType &GetCsum() const { return this->mData.uStruct.sCsum; }
			
			inline acpl::Ipv4Addr::DataType &GetSrcAddr() { return this->mData.uStruct.sSrcAddr; }
			inline const acpl::Ipv4Addr::DataType &GetSrcAddr() const { return this->mData.uStruct.sSrcAddr; }
			
			inline acpl::Ipv4Addr::DataType &GetDstAddr() { return this->mData.uStruct.sDstAddr; }
			inline const acpl::Ipv4Addr::DataType &GetDstAddr() const { return this->mData.uStruct.sDstAddr; }
			
			inline void AddTcpPshCsum(acpl::IpHeader::Checksum &nCs, acpl::UInt16 nTcpLen) const
			{
				acpl::UInt8 oExtras[4] = { 0x00, this->mData.uStruct.sProto, static_cast<acpl::UInt8>(nTcpLen >> 8), static_cast<acpl::UInt8>(nTcpLen) };
				nCs.Add((this->Data() + 12), (sizeof(acpl::Ipv4Addr::DataType) * 2));
				nCs.Add(oExtras, sizeof(oExtras));
			}
	};
	
	class Ipv6Header
	{
			acplHideClassCopy(Ipv6Header);
		
		// http://tools.ietf.org/html/rfc2460
		
		private:
			struct Structure
			{
				acpl::UInt8 sVerTcFl[4];
				acpl::UInt8 sPlen[2];
				acpl::UInt8 sNeh;
				acpl::UInt8 sHol;
				acpl::Ipv6Addr::DataType sSrcAddr;
				acpl::Ipv6Addr::DataType sDstAddr;
			};
		
		public:
			typedef acpl::UInt8 DataType[sizeof(acpl::Ipv6Header::Structure)];
		
		private:
			union
			{
				acpl::Ipv6Header::Structure uStruct;
				acpl::Ipv6Header::DataType  uBytes;
			} mData;
		
		public:
			inline Ipv6Header() { this->Reset(); }
			inline virtual ~Ipv6Header() { }
			
			inline acpl::Ipv6Header::DataType &Data() { return this->mData.uBytes; }
			inline const acpl::Ipv6Header::DataType &Data() const { return this->mData.uBytes; }
			
			inline void Reset()
			{
				acpl::Mem::Set(this->mData.uBytes, 0, sizeof(this->mData.uBytes));
				this->mData.uStruct.sVerTcFl[0] = 0x60;
			}
			
			inline acpl::UInt8 GetTrac() const { return static_cast<acpl::UInt8>(acpl::Endian::FromBig<acpl::UInt16>(0x0FF0, this->mData.uStruct.sVerTcFl) >> 4); }
			inline void SetTrac(acpl::UInt8 nTrac) { acpl::Endian::ToBig<acpl::UInt16>((static_cast<acpl::UInt16>(nTrac) << 4), 0x0FF0, this->mData.uStruct.sVerTcFl); }
			
			inline acpl::UInt32 GetFlab() const { return acpl::Endian::FromBig<acpl::UInt32>(0x000FFFFF, this->mData.uStruct.sVerTcFl); }
			inline void SetFlab(acpl::UInt32 nFlab) { acpl::Endian::ToBig(nFlab, 0x000FFFFF, this->mData.uStruct.sVerTcFl); }
			
			inline acpl::UInt16 GetPlen() const { return acpl::Endian::FromBig<acpl::UInt16>(this->mData.uStruct.sPlen); }
			inline void SetPlen(acpl::UInt16 nPlen) { acpl::Endian::ToBig(nPlen, this->mData.uStruct.sPlen); }
			
			inline acpl::UInt8 GetNeh() const { return this->mData.uStruct.sNeh; }
			inline void SetNeh(acpl::UInt8 nNeh) { this->mData.uStruct.sNeh = nNeh; }
			
			inline acpl::UInt8 GetHol() const { return this->mData.uStruct.sHol; }
			inline void SetHol(acpl::UInt8 nHol) { this->mData.uStruct.sHol = nHol; }
			
			inline acpl::Ipv6Addr::DataType &GetSrcAddr() { return this->mData.uStruct.sSrcAddr; }
			inline const acpl::Ipv6Addr::DataType &GetSrcAddr() const { return this->mData.uStruct.sSrcAddr; }
			
			inline acpl::Ipv6Addr::DataType &GetDstAddr() { return this->mData.uStruct.sDstAddr; }
			inline const acpl::Ipv6Addr::DataType &GetDstAddr() const { return this->mData.uStruct.sDstAddr; }
			
			inline void AddTcpPshCsum(acpl::IpHeader::Checksum &nCs, acpl::UInt32 nTcpLen) const
			{
				acpl::UInt8 oExtras[8] = {
					static_cast<acpl::UInt8>(nTcpLen >> 24),
					static_cast<acpl::UInt8>(nTcpLen >> 16),
					static_cast<acpl::UInt8>(nTcpLen >> 8),
					static_cast<acpl::UInt8>(nTcpLen),
					0x00, 0x00, 0x00, this->mData.uStruct.sNeh };
				nCs.Add((this->Data() + 8), (sizeof(acpl::Ipv6Addr::DataType) * 2));
				nCs.Add(oExtras, sizeof(oExtras));
			}
	};
	
	class IpHeaderTcp
	{
			acplHideClassCopy(IpHeaderTcp);
		
		// http://tools.ietf.org/html/rfc793
		
		private:
			struct Structure
			{
				acpl::NetworkPort::DataType sSrcPort;
				acpl::NetworkPort::DataType sDstPort;
				acpl::UInt8 sSeqNum[4];
				acpl::UInt8 sAckNum[4];
				acpl::UInt8 sDataOff;
				acpl::UInt8 sCtrlBits;
				acpl::UInt8 sWndw[2];
				acpl::IpHeader::Checksum::DataType sCsum;
				acpl::UInt8 sUrgPtr[2];
			};
		
		public:
			typedef acpl::UInt8 DataType[sizeof(acpl::IpHeaderTcp::Structure)];
			
			enum CtrlBits
			{
				cbUrg = 0x20,
				cbAck = 0x10,
				cbPsh = 0x08,
				cbRst = 0x04,
				cbSyn = 0x02,
				cbFin = 0x01
			};
		
		private:
			union
			{
				acpl::IpHeaderTcp::Structure uStruct;
				acpl::IpHeaderTcp::DataType  uBytes;
			} mData;
		
		public:
			inline IpHeaderTcp() { this->Reset(); }
			inline virtual ~IpHeaderTcp() { this->Reset(); }
			
			inline acpl::IpHeaderTcp::DataType &Data() { return this->mData.uBytes; }
			inline const acpl::IpHeaderTcp::DataType &Data() const { return this->mData.uBytes; }
			
			inline void Reset() { acpl::Mem::Set(this->mData.uBytes, 0, sizeof(this->mData.uBytes)); }
			
			inline const acpl::NetworkPort::DataType &GetSrcPort() const { return this->mData.uStruct.sSrcPort; }
			inline acpl::NetworkPort::DataType &GetSrcPort() { return this->mData.uStruct.sSrcPort; }
			
			inline const acpl::NetworkPort::DataType &GetDstPort() const { return this->mData.uStruct.sDstPort; }
			inline acpl::NetworkPort::DataType &GetDstPort() { return this->mData.uStruct.sDstPort; }
			
			inline acpl::UInt32 GetSeqNum() const { return acpl::Endian::FromBig<acpl::UInt32>(this->mData.uStruct.sSeqNum); }
			inline void SetSeqNum(acpl::UInt32 nSeqNum) { acpl::Endian::ToBig(nSeqNum, this->mData.uStruct.sSeqNum); }
			
			inline acpl::UInt32 GetAckNum() const { return acpl::Endian::FromBig<acpl::UInt32>(this->mData.uStruct.sAckNum); }
			inline void SetAckNum(acpl::UInt32 nAckNum) { acpl::Endian::ToBig(nAckNum, this->mData.uStruct.sAckNum); }
			
			inline acpl::UInt8 GetDataOff() const { return (this->mData.uStruct.sDataOff >> 4); }
			inline void SetDataOff(acpl::UInt8 nDataOff) { this->mData.uStruct.sDataOff = (nDataOff << 4); }
			
			inline acpl::UInt8 GetCtrlBits() const { return (this->mData.uStruct.sCtrlBits & 0x3F); }
			inline void SetCtrlBits(acpl::UInt8 nCtrlBits) { this->mData.uStruct.sCtrlBits = (nCtrlBits & 0x3F); }
			
			inline acpl::UInt16 GetWndw() const { return acpl::Endian::FromBig<acpl::UInt16>(this->mData.uStruct.sWndw); }
			inline void SetWndw(acpl::UInt16 nWndw) { acpl::Endian::ToBig(nWndw, this->mData.uStruct.sWndw); }
			
			inline acpl::IpHeader::Checksum::DataType &GetCsum() { return this->mData.uStruct.sCsum; }
			inline const acpl::IpHeader::Checksum::DataType &GetCsum() const { return this->mData.uStruct.sCsum; }
			
			inline acpl::UInt16 GetUrgPtr() const { return acpl::Endian::FromBig<acpl::UInt16>(this->mData.uStruct.sUrgPtr); }
			inline void SetUrgPtr(acpl::UInt16 nUrgPtr) { acpl::Endian::ToBig(nUrgPtr, this->mData.uStruct.sUrgPtr); }
	};
	
	class IpHeaderUdp
	{
			acplHideClassCopy(IpHeaderUdp);
		
		// http://tools.ietf.org/html/rfc768
		
		private:
			struct Structure
			{
				acpl::NetworkPort::DataType sSrcPort;
				acpl::NetworkPort::DataType sDstPort;
				acpl::UInt8 sLen[2];
				acpl::IpHeader::Checksum::DataType sCsum;
			};
		
		public:
			typedef acpl::UInt8 DataType[sizeof(acpl::IpHeaderUdp::Structure)];
		
		private:
			union
			{
				acpl::IpHeaderUdp::Structure uStruct;
				acpl::IpHeaderUdp::DataType  uBytes;
			} mData;
		
		public:
			inline IpHeaderUdp() { this->Reset(); }
			inline virtual ~IpHeaderUdp() { this->Reset(); }
			
			inline acpl::IpHeaderUdp::DataType &Data() { return this->mData.uBytes; }
			inline const acpl::IpHeaderUdp::DataType &Data() const { return this->mData.uBytes; }
			
			inline void Reset() { acpl::Mem::Set(this->mData.uBytes, 0, sizeof(this->mData.uBytes)); }
			
			inline const acpl::NetworkPort::DataType &GetSrcPort() const { return this->mData.uStruct.sSrcPort; }
			inline acpl::NetworkPort::DataType &GetSrcPort() { return this->mData.uStruct.sSrcPort; }
			
			inline const acpl::NetworkPort::DataType &GetDstPort() const { return this->mData.uStruct.sDstPort; }
			inline acpl::NetworkPort::DataType &GetDstPort() { return this->mData.uStruct.sDstPort; }
			
			inline acpl::UInt16 GetLen() const { return acpl::Endian::FromBig<acpl::UInt16>(this->mData.uStruct.sLen); }
			inline void SetLen(acpl::UInt16 nLen) { acpl::Endian::ToBig(nLen, this->mData.uStruct.sLen); }
			
			inline acpl::IpHeader::Checksum::DataType &GetCsum() { return this->mData.uStruct.sCsum; }
			inline const acpl::IpHeader::Checksum::DataType &GetCsum() const { return this->mData.uStruct.sCsum; }
	};
	
	class IpHeaderIcmp
	{
			acplHideClassCopy(IpHeaderIcmp);
		
		// http://tools.ietf.org/html/rfc792
		
		private:
			struct Structure
			{
				acpl::UInt8 sType;
				acpl::UInt8 sCode;
				acpl::IpHeader::Checksum::DataType sCsum;
			};
		
		public:
			typedef acpl::UInt8 DataType[sizeof(acpl::IpHeaderIcmp::Structure)];
			
			enum MessageType
			{
				mtEchoRpl = 0,
				mtDstUnrch = 3,
				mtSrcQuench = 4,
				mtRedir = 5,
				mtEcho = 8,
				mtTimeExc = 11,
				mtParamProb = 12,
				mtTstamp = 13,
				mtTstampRpl = 14,
				mtInfoReq = 15,
				mtInfoRpl = 16
			};
		
		private:
			union DataUnion
			{
				acpl::IpHeaderIcmp::Structure uStruct;
				acpl::IpHeaderIcmp::DataType  uBytes;
			} mData;
		
		public:
			inline IpHeaderIcmp() { this->Reset(); }
			inline virtual ~IpHeaderIcmp() { }
			
			inline acpl::IpHeaderIcmp::DataType &Data() { return this->mData.uBytes; }
			inline const acpl::IpHeaderIcmp::DataType &Data() const { return this->mData.uBytes; }
			
			inline void Reset() { acpl::Mem::Set(this->mData.uBytes, 0, sizeof(this->mData.uBytes)); }
			
			inline acpl::UInt8 GetType() const { return this->mData.uStruct.sType; }
			inline void SetType(acpl::UInt8 nType) { this->mData.uStruct.sType = nType; }
			
			inline acpl::UInt8 GetCode() const { return this->mData.uStruct.sCode; }
			inline void SetCode(acpl::UInt8 nCode) { this->mData.uStruct.sCode = nCode; }
			
			inline const acpl::IpHeader::Checksum::DataType &GetCsum() const { return this->mData.uStruct.sCsum; }
			inline acpl::IpHeader::Checksum::DataType &GetCsum() { return this->mData.uStruct.sCsum; }
	};
	
	class IpHeaderIgmp
	{
			acplHideClassCopy(IpHeaderIgmp);
		
		// http://tools.ietf.org/html/rfc1112
		// http://tools.ietf.org/html/rfc2236
		// http://tools.ietf.org/html/rfc3376
		
		private:
			struct Structure
			{
				acpl::UInt8 sType;
				acpl::UInt8 sCode;
				acpl::IpHeader::Checksum::DataType sCsum;
			};
		
		public:
			typedef acpl::UInt8 DataType[sizeof(acpl::IpHeaderIgmp::Structure)];
			
			enum MessageType
			{
				mtCreateReq   = 0x01,
				mtCreateRpl   = 0x02,
				mtJoinReq     = 0x03,
				mtJoinRpl     = 0x04,
				mtLeaveReq    = 0x05,
				mtLeaveRpl    = 0x06,
				mtConfirmReq  = 0x07,
				mtConfirmRpl  = 0x08,
				mtHmembQry    = 0x11,
				mtHmembRpt    = 0x12,
				mtDvmrp       = 0x13,
				mtPim         = 0x14,
				mtCiscoTm     = 0x15,
				mtHmembRptv2  = 0x16,
				mtLeavev2     = 0x17,
				mtMcastTrResp = 0x1E,
				mtMcastTr     = 0x1F,
				mtHmembRptv3  = 0x22,
				mtMcastRtrAdv = 0x30,
				mtMcastRtrSol = 0x31,
				mtMcastRtrTer = 0x32
			};
		
		private:
			union DataUnion
			{
				acpl::IpHeaderIgmp::Structure uStruct;
				acpl::IpHeaderIgmp::DataType  uBytes;
			} mData;
		
		public:
			inline IpHeaderIgmp() { this->Reset(); }
			inline virtual ~IpHeaderIgmp() { }
			
			inline acpl::IpHeaderIgmp::DataType &Data() { return this->mData.uBytes; }
			inline const acpl::IpHeaderIgmp::DataType &Data() const { return this->mData.uBytes; }
			
			inline void Reset() { acpl::Mem::Set(this->mData.uBytes, 0, sizeof(this->mData.uBytes)); }
			
			inline acpl::UInt8 GetType() const { return this->mData.uStruct.sType; }
			inline void SetType(acpl::UInt8 nType) { this->mData.uStruct.sType = nType; }
			
			inline acpl::UInt8 GetCode() const { return this->mData.uStruct.sCode; }
			inline void SetCode(acpl::UInt8 nCode) { this->mData.uStruct.sCode = nCode; }
			
			inline const acpl::IpHeader::Checksum::DataType &GetCsum() const { return this->mData.uStruct.sCsum; }
			inline acpl::IpHeader::Checksum::DataType &GetCsum() { return this->mData.uStruct.sCsum; }
	};
	
	class IpHeaderIcmpv6
	{
			acplHideClassCopy(IpHeaderIcmpv6);
		
		// http://tools.ietf.org/html/rfc2463
		
		private:
			struct Structure
			{
				acpl::UInt8 sType;
				acpl::UInt8 sCode;
				acpl::IpHeader::Checksum::DataType sCsum;
			};
		
		public:
			typedef acpl::UInt8 DataType[sizeof(acpl::IpHeaderIcmpv6::Structure)];
			
			enum MessageType
			{
				mtDstUnrch = 1,
				mtPktTooBig = 2,
				mtTimeExc = 3,
				mtParamProb = 4,
				mtEchoReq = 128,
				mtEchoRpl = 129
			};
		
		private:
			union DataUnion
			{
				acpl::IpHeaderIcmpv6::Structure uStruct;
				acpl::IpHeaderIcmpv6::DataType  uBytes;
			} mData;
		
		public:
			inline IpHeaderIcmpv6() { this->Reset(); }
			inline virtual ~IpHeaderIcmpv6() { }
			
			inline const acpl::IpHeaderIcmpv6::DataType &Data() const { return this->mData.uBytes; }
			inline acpl::IpHeaderIcmpv6::DataType &Data() { return this->mData.uBytes; }
			
			inline void Reset() { acpl::Mem::Set(this->mData.uBytes, 0, sizeof(this->mData.uBytes)); }
			
			inline acpl::UInt8 GetType() const { return this->mData.uStruct.sType; }
			inline void SetType(acpl::UInt8 nType) { this->mData.uStruct.sType = nType; }
			
			inline acpl::UInt8 GetCode() const { return this->mData.uStruct.sCode; }
			inline void SetCode(acpl::UInt8 nCode) { this->mData.uStruct.sCode = nCode; }
			
			inline const acpl::IpHeader::Checksum::DataType &GetCsum() const { return this->mData.uStruct.sCsum; }
			inline acpl::IpHeader::Checksum::DataType &GetCsum() { return this->mData.uStruct.sCsum; }
	};
}

#endif // ACPL_NET_HEADER_H
