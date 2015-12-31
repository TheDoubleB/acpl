#include "Address.h"

#include "../_cp/Memory.inc.cpp"
#include "../_cp/Network.inc.cpp"


class _local_Ipv4AddrExt : public acpl::Ipv4Addr
{
	public:
		static inline bool CValidate(const char *nBegPtr, const char *nEndPtr) { return acpl::Ipv4Addr::Parse(nBegPtr, nEndPtr, NULL); }
};

class _local_Ipv6AddrExt : public acpl::Ipv6Addr
{
	public:
		static inline bool CValidate(const char *nBegPtr, const char *nEndPtr) { return acpl::Ipv6Addr::Parse(nBegPtr, nEndPtr, NULL); }
};

class _local_DomainNameExt : public acpl::DomainName
{
	public:
		static inline bool CValidate(const char *nBegPtr, const char *nEndPtr) { return acpl::DomainName::Parse(nBegPtr, nEndPtr, NULL); }
};



// 
// NetworkByteDataBase
// 



// 
// NetworkAddr
// 

acpl::NetworkAddr::Type acpl::NetworkAddr::Identify(const char *nBegPtr, const char *nEndPtr)
{
	if (_local_Ipv4AddrExt::CValidate(nBegPtr, nEndPtr) == true)
		return acpl::NetworkAddr::tIpv4Addr;
	else
	if (_local_Ipv6AddrExt::CValidate(nBegPtr, nEndPtr) == true)
		return acpl::NetworkAddr::tIpv6Addr;
	else
	if (_local_DomainNameExt::CValidate(nBegPtr, nEndPtr) == true)
		return acpl::NetworkAddr::tDomainName;
	else
		return acpl::NetworkAddr::tNone;
}



// 
// IpvxAddrBase
// 

template <class tType>
static inline void _local_IpvxAddr_CopyBytes(tType &nDest, const tType &nSrc)
{
	acpl::SizeT oCount = sizeof(tType);
	while (oCount--)
		nDest[oCount] = nSrc[oCount];
}



// 
// Ipv4Addr
// 

// PUBLIC

bool acpl::Ipv4Addr::ToStr(acpl::String &nAddrStr) const
{
	nAddrStr.Set(acpl::String::IpAddrRef(*this));
	return true;
}


// PROTECTED

static inline bool _local_Ipv4Addr_Parse(const char *&nBegPtr, const char *nEndPtr, acpl::UInt8 *nElems, acpl::UInt8 &nElemsOff)
{
	acpl::UInt16 oCurElem = 0;
	acpl::UInt8 oMaxElemsOff = nElemsOff + 3;
	
	for (; nBegPtr < nEndPtr && *nBegPtr != '\0'; nBegPtr++)
	{
		if (acpl::pi::_string_ascii_char2num(*nBegPtr, 10, oCurElem) == true)
		{
			if (oCurElem > 0xFF)
				return false;
		}
		else
		if (*nBegPtr == '.' && nElemsOff < oMaxElemsOff && nBegPtr[1] != '.')
		{
			nElems[nElemsOff++] = static_cast<acpl::UInt8>(oCurElem);
			oCurElem = 0;
		}
		else
			return false;
	}
	
	if (nElemsOff == oMaxElemsOff && nBegPtr[-1] != '.')
	{
		nElems[nElemsOff++] = static_cast<acpl::UInt8>(oCurElem);
		return true;
	}
	else
		return false;
}

bool acpl::Ipv4Addr::Parse(const char *nBegPtr, const char *nEndPtr, acpl::Ipv4Addr *nObj)
{
	if (nObj != NULL)
		nObj->Reset();
	
	if (nEndPtr == NULL)
		nEndPtr--; // to make it underflow so it will be larger than `nBegPtr`
	
	if (nEndPtr <= nBegPtr || nBegPtr == NULL || acpl::pi::_string_ascii_is_digit(*nBegPtr) != true)
		return false;
	
	
	acpl::Ipv4Addr::DataType oElems;
	acpl::UInt8 oElemsOff = 0;
	
	if (_local_Ipv4Addr_Parse(nBegPtr, nEndPtr, oElems, oElemsOff) == true)
	{
		if (nObj != NULL)
			_local_IpvxAddr_CopyBytes(nObj->Data(), oElems);
		
		return true;
	}
	else
		return false;
}



// 
// Ipv6Addr
// 

// PUBLIC

bool acpl::Ipv6Addr::ToStr(acpl::String &nAddrStr) const
{
	nAddrStr.Set(acpl::String::IpAddrRef(*this));
	return true;
}


// PROTECTED

static inline void _local_Ipv6Addr_SetElem(acpl::UInt32 nValue, acpl::Ipv6Addr::DataType &nElems, acpl::UInt8 &nElemsOff)
{
	nElems[nElemsOff++] = static_cast<acpl::UInt8>(nValue >> 8);
	nElems[nElemsOff++] = static_cast<acpl::UInt8>(nValue & 0xFF);
}

bool acpl::Ipv6Addr::Parse(const char *nBegPtr, const char *nEndPtr, acpl::Ipv6Addr *nObj)
{
	if (nObj != NULL)
		nObj->Reset();
	
	if (nEndPtr == NULL)
		nEndPtr--; // to make it underflow so it will be larger than `nBegPtr`
	
	if (nEndPtr <= nBegPtr || nBegPtr == NULL || *nBegPtr == '\0')
		return false;
	
	
	bool oIpv4Mode = false;
	acpl::Ipv6Addr::DataType oElems;
	acpl::Mem::Set(oElems, 0, sizeof(oElems));
	acpl::UInt8 oElemsOff = 0;
	acpl::UInt32 oCurElem = 0;
	acpl::UInt8 oWildcardOff = 0xFF; // wildcard == "::"; only values 0-15 will be valid
	
	
	if (*nBegPtr == ':')
	{
		if (++nBegPtr == nEndPtr || *nBegPtr != ':') // the address can begin with two colons, but not just one !
			return false;
		else
		if (++nBegPtr == nEndPtr || *nBegPtr == '\0') // the address contains only the wildcard ("::")
			return true; // NOTE: No copying to `nObj` needed !
		else
		{
			// the address contains the wildcard + additional elements
			// NOTE: the `nBegPtr` is already at the beginning of additional elements
			oElemsOff = 2;
			oWildcardOff = 0;
		}
	}
	
	const char *oLastStart = nBegPtr;
	
	for (; nBegPtr < nEndPtr && *nBegPtr != '\0'; nBegPtr++)
	{
		if (oElemsOff > 14)
			return false;
		else
		if (acpl::pi::_string_ascii_char2num(*nBegPtr, 16, oCurElem) == true)
		{
			if (oCurElem > 0xFFFF)
				return false;
		}
		else
		if (*nBegPtr == ':')
		{
			if (oElemsOff > 12 && oWildcardOff < 16)
				return false;
			else
			if (oLastStart != nBegPtr)
			{
				_local_Ipv6Addr_SetElem(oCurElem, oElems, oElemsOff);
				oCurElem = 0;
			}
			else
			if (oWildcardOff < 16)
				return false; // oLastStart == nAddrStr, but wildcard has already appeared
			else
			{
				oElemsOff += 2;
				oWildcardOff = oElemsOff; // oLastStart == nAddrStr; set the wildcard
			}
			
			oLastStart = nBegPtr + 1;
		}
		else
		if (*nBegPtr == '.')
		{
			nBegPtr = oLastStart;
			oIpv4Mode = true;
			break;
		}
		else
			return false;
	}
	
	if (oIpv4Mode == false)
	{
		if (oElemsOff <= 14)
			_local_Ipv6Addr_SetElem(oCurElem, oElems, oElemsOff);
		else
			oElemsOff += 2;
	}
	else
	if (oElemsOff > 12 || oElemsOff == 0 || _local_Ipv4Addr_Parse(nBegPtr, nEndPtr, oElems, oElemsOff) != true)
		return false;
	
	
	if (oWildcardOff < 16)
	{
		if (nBegPtr[-1] != ':' || nBegPtr[-2] == ':')
		{
			acpl::Mem::Move(&oElems[oWildcardOff + (16 - oElemsOff)], &oElems[oWildcardOff], (oElemsOff - oWildcardOff));
			acpl::Mem::Set(&oElems[oWildcardOff], 0x00, (16 - oElemsOff));
		}
		else
			return false;
	}
	else
	if (oWildcardOff > 16 && oElemsOff != 16)
		return false;
	
	
	if (nObj != NULL)
		_local_IpvxAddr_CopyBytes(nObj->Data(), oElems);
	
	
	return true;
}



// 
// DomainName
// 

// PUBLIC

bool acpl::DomainName::ToStr(acpl::String &nAddrStr) const
{
	if (this->mDn.IsEmpty() != true)
	{
		nAddrStr.Set(this->mDn);
		return true;
	}
	else
	{
		nAddrStr.Clear();
		return false;
	}
}


bool acpl::DomainName::ExplodeBySubdomains(acpl::DomainName::SubdomainList &nSubdomains)
{
	nSubdomains.Clear();
	
	if (this->mDn.IsEmpty() != true)
	{
		const char *oBeg = this->mDn.Utf8();
		const char *oCurPos = oBeg + this->mDn.ByteLength(); // on NULL terminator
		const char *oLastDot = oCurPos;
		
		for (; oCurPos >= oBeg; oCurPos--)
		{
			if (*oCurPos == '.')
			{
				nSubdomains.Add((oCurPos + 1), (oLastDot - oCurPos - 1));
				oLastDot = oCurPos;
			}
		}
		
		nSubdomains.Add((oCurPos + 1), (oLastDot - oCurPos - 1));
		
		return true;
	}
	else
		return false;
}


acpl::DomainName::ResolveStatus acpl::DomainName::Resolve(acpl::DomainName::Ipv4AddrList &nAddr4List) const
{
	nAddr4List.Clear();
	return acpl::cp::_net_address_resolve(this->mDn, &nAddr4List, NULL);
}

acpl::DomainName::ResolveStatus acpl::DomainName::Resolve(acpl::DomainName::Ipv6AddrList &nAddr6List) const
{
	nAddr6List.Clear();
	return acpl::cp::_net_address_resolve(this->mDn, NULL, &nAddr6List);
}

acpl::DomainName::ResolveStatus acpl::DomainName::Resolve(acpl::DomainName::Ipv4AddrList &nAddr4List, acpl::DomainName::Ipv6AddrList &nAddr6List) const
{
	nAddr4List.Clear();
	nAddr6List.Clear();
	return acpl::cp::_net_address_resolve(this->mDn, &nAddr4List, &nAddr6List);
}


// PROTECTED

bool acpl::DomainName::Parse(const char *nBegPtr, const char *nEndPtr, acpl::DomainName *nObj)
{
	if (nObj != NULL)
		nObj->Reset();
	
	if (_local_Ipv4AddrExt::CValidate(nBegPtr, nEndPtr) == true)
		return false;
	
	if (nEndPtr == NULL)
		nEndPtr--; // to make it underflow so it will be larger than `nBegPtr`
	
	if (nEndPtr <= nBegPtr || nBegPtr == NULL || acpl::pi::_string_ascii_is_alnum(*nBegPtr) != true)
		return false;
	
	
	const char *oCurPos = nBegPtr;
	
	for (; oCurPos < nEndPtr && *oCurPos != '\0'; oCurPos++)
	{
		if (acpl::pi::_string_ascii_is_alnum(*oCurPos) != true &&
			(*oCurPos != '.' || oCurPos[-1] == '.' || oCurPos[-1] == '-') &&
			(*oCurPos != '-' || oCurPos[-1] == '.'))
			return false;
	}
	
	if (oCurPos[-1] == '.' || oCurPos[-1] == '-')
		return false;
	
	
	if (nObj != NULL)
		nObj->mDn.Set(nBegPtr, (nEndPtr - nBegPtr));
	
	
	return true;
}



// 
// NetworkPort
// 
