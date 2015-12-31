#ifndef ACPL_NET_ADDRESS_H
#define ACPL_NET_ADDRESS_H

#include "../Endian.h"
#include "../String.h"

namespace acpl
{
	template <acpl::UInt8 tByteSize>
	class NetworkByteDataBase
	{
			acplHideClassCopy(NetworkByteDataBase);
		
		public:
			typedef acpl::UInt8 DataType[tByteSize];
		
		private:
			DataType mData;
		
		protected:
			inline NetworkByteDataBase() { }
			
			inline void IReset() { for (acpl::UInt8 i = sizeof(DataType); i-- != 0; ) this->mData[i] = 0; }
			inline void IGet(DataType &nData) const { for (acpl::UInt8 i = sizeof(DataType); i-- != 0; ) nData[i] = this->mData[i]; }
			inline void ISet(const DataType &nData) { for (acpl::UInt8 i = sizeof(DataType); i-- != 0; ) this->mData[i] = nData[i]; }
		
		public:
			inline virtual ~NetworkByteDataBase() { }
			
			inline const DataType &Data() const { return this->mData; }
			inline DataType &Data() { return this->mData; }
	};
	
	class NetworkAddr
	{
			acplHideClassCopy(NetworkAddr);
		
		public:
			enum Type
			{
				tNone,
				tIpv4Addr,
				tIpv6Addr,
				tDomainName
			};
		
		protected:
			inline NetworkAddr() { }
			static acpl::NetworkAddr::Type Identify(const char *nBegPtr, const char *nEndPtr);
		
		public:
			inline virtual ~NetworkAddr() { }
			
			inline static acpl::NetworkAddr::Type Identify(const acpl::String::Ref &nAddrStr, acpl::SizeT nMaxCount = acpl::Num<acpl::SizeT>::Max(), acpl::SizeT nStartOffset = 0) { return acpl::NetworkAddr::Identify(acpl::String(nAddrStr, nMaxCount, nStartOffset)); }
			inline static acpl::NetworkAddr::Type Identify(const acpl::String &nAddrStr)  { return acpl::NetworkAddr::Identify(nAddrStr.Utf8(), NULL); }
			
			virtual bool Parse(const acpl::String::Ref &nAddrStr, acpl::SizeT nMaxCount = acpl::Num<acpl::SizeT>::Max(), acpl::SizeT nStartOffset = 0) = 0;
			virtual bool Parse(const acpl::String &nAddrStr) = 0;
			virtual bool ToStr(acpl::String &nAddrStr) const = 0;
			virtual void Reset() = 0;
	};
	
	class Ipv4Addr : public acpl::NetworkByteDataBase<4>, public acpl::NetworkAddr
	{
			acplHideClassCopyI(Ipv4Addr, acpl::NetworkByteDataBase<4>(), acpl::NetworkAddr());
		
		protected:
			static bool Parse(const char *nBegPtr, const char *nEndPtr, acpl::Ipv4Addr *nObj);
		
		public:
			inline Ipv4Addr() { this->IReset(); }
			inline Ipv4Addr(acpl::UInt8 nByte1, acpl::UInt8 nByte2, acpl::UInt8 nByte3, acpl::UInt8 nByte4) { this->Set(nByte1, nByte2, nByte3, nByte4); }
			inline virtual ~Ipv4Addr() { }
			
			inline static bool Validate(const acpl::String::Ref &nAddrStr, acpl::SizeT nMaxCount = acpl::Num<acpl::SizeT>::Max(), acpl::SizeT nStartOffset = 0) { return acpl::Ipv4Addr::Validate(acpl::String(nAddrStr, nMaxCount, nStartOffset)); }
			inline static bool Validate(const acpl::String &nAddrStr)  { return acpl::Ipv4Addr::Parse(nAddrStr.Utf8(), NULL, NULL); }
			
			inline virtual bool Parse(const acpl::String::Ref &nAddrStr, acpl::SizeT nMaxCount = acpl::Num<acpl::SizeT>::Max(), acpl::SizeT nStartOffset = 0) { return this->Parse(acpl::String(nAddrStr, nMaxCount, nStartOffset)); }
			inline virtual bool Parse(const acpl::String &nAddrStr) { return acpl::Ipv4Addr::Parse(nAddrStr.Utf8(), NULL, this); }
			virtual bool ToStr(acpl::String &nAddrStr) const;
			inline virtual void Reset() { this->IReset(); }
			
			inline void Get(DataType &nData) const { this->IGet(nData); }
			inline acpl::Ipv4Addr &Set(const DataType &nData) { this->ISet(nData); return *this; }
			
			inline acpl::Ipv4Addr &Set(acpl::UInt8 nByte1, acpl::UInt8 nByte2, acpl::UInt8 nByte3, acpl::UInt8 nByte4)
			{
				this->Data()[0] = nByte1;
				this->Data()[1] = nByte2;
				this->Data()[2] = nByte3;
				this->Data()[3] = nByte4;
				return *this;
			}
			
			inline bool IsAny() const
			{
				return (
					this->Data()[0] == 0 &&  this->Data()[1] == 0 &&
					this->Data()[2] == 0 &&  this->Data()[3] == 0);
			}
			
			inline acpl::Ipv4Addr &SetAny()
			{
				this->Set(0, 0, 0, 0);
				return *this;
			}
			
			inline bool IsLoopback() const
			{
				return (this->Data()[0] == 127);
			}
			
			inline acpl::Ipv4Addr &SetLoopback()
			{
				this->Set(127, 0, 0, 1);
				return *this;
			}
			
			inline bool IsMulticast() const { return ((this->Data()[0] & 0xF0) == 0xE0); }
	};
	
	class Ipv6Addr : public acpl::NetworkByteDataBase<16>, public acpl::NetworkAddr
	{
			acplHideClassCopyI(Ipv6Addr, acpl::NetworkByteDataBase<16>(), acpl::NetworkAddr());
		
		protected:
			inline void Set(acpl::UInt8 nGrpIdx, acpl::UInt16 nGrpVal)
			{
				acpl::Endian::ToBig(nGrpVal, (this->Data() + (nGrpIdx * 2)));
			}
			
			inline void Set(acpl::UInt16 nGrp1, acpl::UInt16 nGrp2, acpl::UInt16 nGrp3, acpl::UInt16 nGrp4, acpl::UInt16 nGrp5, acpl::UInt16 nGrp6)
			{
				this->Set(0, nGrp1);
				this->Set(1, nGrp2);
				this->Set(2, nGrp3);
				this->Set(3, nGrp4);
				this->Set(4, nGrp5);
				this->Set(5, nGrp6);
			}
			
			static bool Parse(const char *nBegPtr, const char *nEndPtr, acpl::Ipv6Addr *nObj);
		
		public:
			inline Ipv6Addr() { this->IReset(); }
			inline Ipv6Addr(acpl::UInt16 nGrp1, acpl::UInt16 nGrp2, acpl::UInt16 nGrp3, acpl::UInt16 nGrp4, acpl::UInt16 nGrp5, acpl::UInt16 nGrp6, acpl::UInt16 nGrp7, acpl::UInt16 nGrp8) { this->Set(nGrp1, nGrp2, nGrp3, nGrp4, nGrp5, nGrp6, nGrp7, nGrp8); }
			inline Ipv6Addr(acpl::UInt16 nGrp1, acpl::UInt16 nGrp2, acpl::UInt16 nGrp3, acpl::UInt16 nGrp4, acpl::UInt16 nGrp5, acpl::UInt16 nGrp6, acpl::UInt8 nGrp7Byte1, acpl::UInt8 nGrp7Byte2, acpl::UInt8 nGrp8Byte1, acpl::UInt8 nGrp8Byte2) { this->Set(nGrp1, nGrp2, nGrp3, nGrp4, nGrp5, nGrp6, nGrp7Byte1, nGrp7Byte2, nGrp8Byte1, nGrp8Byte2); }
			inline Ipv6Addr(acpl::UInt8 nGrp7Byte1, acpl::UInt8 nGrp7Byte2, acpl::UInt8 nGrp8Byte1, acpl::UInt8 nGrp8Byte2) { this->Set(nGrp7Byte1, nGrp7Byte2, nGrp8Byte1, nGrp8Byte2); }
			inline virtual ~Ipv6Addr() { }
			
			inline static bool Validate(const acpl::String::Ref &nAddrStr, acpl::SizeT nMaxCount = acpl::Num<acpl::SizeT>::Max(), acpl::SizeT nStartOffset = 0) { return acpl::Ipv6Addr::Validate(acpl::String(nAddrStr, nMaxCount, nStartOffset)); }
			inline static bool Validate(const acpl::String &nAddrStr)  { return acpl::Ipv6Addr::Parse(nAddrStr.Utf8(), NULL, NULL); }
			
			inline virtual bool Parse(const acpl::String::Ref &nAddrStr, acpl::SizeT nMaxCount = acpl::Num<acpl::SizeT>::Max(), acpl::SizeT nStartOffset = 0) { return this->Parse(acpl::String(nAddrStr, nMaxCount, nStartOffset)); }
			inline virtual bool Parse(const acpl::String &nAddrStr) { return acpl::Ipv6Addr::Parse(nAddrStr.Utf8(), NULL, this); }
			virtual bool ToStr(acpl::String &nAddrStr) const;
			inline virtual void Reset() { this->IReset(); }
			
			inline void Get(DataType &nData) const { this->IGet(nData); }
			inline acpl::Ipv6Addr &Set(const DataType &nData) { this->ISet(nData); return *this; }
			
			inline acpl::Ipv6Addr &Set(acpl::UInt16 nGrp1, acpl::UInt16 nGrp2, acpl::UInt16 nGrp3, acpl::UInt16 nGrp4, acpl::UInt16 nGrp5, acpl::UInt16 nGrp6, acpl::UInt16 nGrp7, acpl::UInt16 nGrp8)
			{
				this->Set(nGrp1, nGrp2, nGrp3, nGrp4, nGrp5, nGrp6);
				this->Set(6, nGrp7);
				this->Set(7, nGrp8);
				return *this;
			}
			
			inline acpl::Ipv6Addr &Set(acpl::UInt16 nGrp1, acpl::UInt16 nGrp2, acpl::UInt16 nGrp3, acpl::UInt16 nGrp4, acpl::UInt16 nGrp5, acpl::UInt16 nGrp6, acpl::UInt8 nGrp7Byte1, acpl::UInt8 nGrp7Byte2, acpl::UInt8 nGrp8Byte1, acpl::UInt8 nGrp8Byte2)
			{
				this->Set(nGrp1, nGrp2, nGrp3, nGrp4, nGrp5, nGrp6);
				this->Data()[12] = nGrp7Byte1;
				this->Data()[13] = nGrp7Byte2;
				this->Data()[14] = nGrp8Byte1;
				this->Data()[15] = nGrp8Byte2;
				return *this;
			}
			
			inline acpl::Ipv6Addr &Set(acpl::UInt8 nGrp7Byte1, acpl::UInt8 nGrp7Byte2, acpl::UInt8 nGrp8Byte1, acpl::UInt8 nGrp8Byte2)
			{
				this->Set(0, 0, 0, 0, 0, 0, nGrp7Byte1, nGrp7Byte2, nGrp8Byte1, nGrp8Byte2);
				return *this;
			}
			
			inline bool IsAny() const
			{
				for (acpl::UInt8 i = sizeof(acpl::Ipv6Addr::DataType); i-- != 0; )
					if (this->Data()[i] != 0)
						return false;
				
				return true;
			}
			
			inline acpl::Ipv6Addr &SetAny()
			{
				this->Set(0, 0, 0, 0, 0, 0, 0, 0);
				return *this;
			}
			
			inline bool IsLoopback() const
			{
				for (acpl::UInt8 i = sizeof(acpl::Ipv6Addr::DataType) - 1; i-- != 0; )
					if (this->Data()[i] != 0)
						return false;
				
				return (this->Data()[15] == 1);
			}
			
			inline acpl::Ipv6Addr &SetLoopback()
			{
				this->Set(0, 0, 0, 0, 0, 0, 0, 1);
				return *this;
			}
			
			inline bool IsMulticast() const { return (this->Data()[0] == 0xFF); }
	};
	
	class DomainName : public NetworkAddr
	{
			acplHideClassCopyI(DomainName, acpl::NetworkAddr());
		
		public:
			typedef acpl::ArrayEdaNone<acpl::String>::Type SubdomainList;
			typedef acpl::ArrayEdaNone<acpl::Ipv4Addr>::Type Ipv4AddrList;
			typedef acpl::ArrayEdaNone<acpl::Ipv6Addr>::Type Ipv6AddrList;
			
			enum ResolveStatus
			{
				rsSuccess,
				rsSystemError,
				rsNotFound,
				rsDnsTempFail,
				rsDnsPermFail
			};
		
		private:
			acpl::String mDn;
		
		protected:
			static bool Parse(const char *nBegPtr, const char *nEndPtr, acpl::DomainName *nObj);
		
		public:
			inline DomainName() { this->Reset(); }
			inline virtual ~DomainName() { this->Reset(); }
			
			inline static bool Validate(const acpl::String::Ref &nAddrStr, acpl::SizeT nMaxCount = acpl::Num<acpl::SizeT>::Max(), acpl::SizeT nStartOffset = 0) { return acpl::DomainName::Validate(acpl::String(nAddrStr, nMaxCount, nStartOffset)); }
			inline static bool Validate(const acpl::String &nAddrStr) { return acpl::DomainName::Parse(nAddrStr.Utf8(), NULL, NULL); }
			
			inline virtual bool Parse(const acpl::String::Ref &nAddrStr, acpl::SizeT nMaxCount = acpl::Num<acpl::SizeT>::Max(), acpl::SizeT nStartOffset = 0) { return this->Parse(acpl::String(nAddrStr, nMaxCount, nStartOffset)); }
			inline virtual bool Parse(const acpl::String &nAddrStr) { return acpl::DomainName::Parse(nAddrStr.Utf8(), NULL, this); }
			virtual bool ToStr(acpl::String &nAddrStr) const;
			inline virtual void Reset() { this->mDn.Clear(); }
			
			inline const acpl::String &GetStr() const { return this->mDn; }
			bool ExplodeBySubdomains(acpl::DomainName::SubdomainList &nSubdomains);
			
			acpl::DomainName::ResolveStatus Resolve(acpl::DomainName::Ipv4AddrList &nAddr4List) const;
			acpl::DomainName::ResolveStatus Resolve(acpl::DomainName::Ipv6AddrList &nAddr6List) const;
			acpl::DomainName::ResolveStatus Resolve(acpl::DomainName::Ipv4AddrList &nAddr4List, acpl::DomainName::Ipv6AddrList &nAddr6List) const;
	};
	
	class NetworkPort : public acpl::NetworkByteDataBase<2>
	{
			acplHideClassCopyI(NetworkPort, acpl::NetworkByteDataBase<2>());
		
		public:
			inline NetworkPort() { this->IReset(); }
			inline NetworkPort(acpl::UInt16 nPort) { this->Set(nPort); }
			inline virtual ~NetworkPort() { }
			
			inline void Get(DataType &nData) const { this->IGet(nData); }
			inline acpl::NetworkPort &Set(const DataType &nData) { this->ISet(nData); return *this; }
			
			inline acpl::UInt16 Get() const { return acpl::Endian::FromBig<acpl::UInt16>(this->Data()); }
			inline acpl::NetworkPort &Set(acpl::UInt16 nPort) { acpl::Endian::ToBig(nPort, this->Data()); return *this; }
	};
	
}

#endif // ACPL_NET_ADDRESS_H
