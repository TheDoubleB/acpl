#include "Uri.h"

#include "net/Address.h"
#include "Raii.h"
#include "_cp/Memory.inc.cpp"


// INTERNAL URI HACK TYPE

class _local_Uri_Hack_T
{
	public:
		acpl::Uri *mUri;
		acpl::String *mScheme;
		bool *mHasAuthority;
		acpl::String *mUserInfo;
		acpl::String *mHost;
		acpl::Uri::HostType *mHostType;
		acpl::UInt16 *mPort;
		acpl::String *mPath;
		acpl::String *mQuery;
		acpl::String *mFragment;
		
		inline _local_Uri_Hack_T()
		{
			this->mUri = NULL;
			this->mScheme = NULL;
			this->mHasAuthority = NULL;
			this->mUserInfo = NULL;
			this->mHost = NULL;
			this->mHostType = NULL;
			this->mPort = NULL;
			this->mPath = NULL;
			this->mQuery = NULL;
			this->mFragment = NULL;
		}
		
		inline void Clear()
		{
			if (this->mUri != NULL)
				this->mUri->Clear();
		}
};


// INTERNAL

static inline bool _local_Uri_IsUnreserved(char nChar)
{
	return (acpl::pi::_string_ascii_is_alnum(nChar) == true || nChar == '-' || nChar == '.' || nChar == '_' || nChar == '~');
}

static inline bool _local_Uri_IsSubDelimiter(char nChar)
{
	return (nChar == '!' || nChar == '$' || nChar == '&' || nChar == '\'' || nChar == '(' || nChar == ')' || nChar == '*' || nChar == '+' || nChar == ',' || nChar == ';' || nChar == '=');
}

static inline bool _local_Uri_IsPctEncoded(const char *&nCurPos, const char *nEnd)
{
	if ((nCurPos + 3) <= nEnd && nCurPos[0] == '%' && acpl::pi::_string_ascii_is_xdigit(nCurPos[1]) == true && acpl::pi::_string_ascii_is_xdigit(nCurPos[2]) == true)
	{
		nCurPos += 2; // only 2 (not 3) because the loops that use this function will increment the pointer by 1 at the end of iteration
		return true;
	}
	else
		return false;
}

static inline bool _local_Uri_IsPchar(const char *&nCurPos, const char *nEnd)
{
	return (_local_Uri_IsPctEncoded(nCurPos, nEnd) == true || _local_Uri_IsUnreserved(*nCurPos) == true || _local_Uri_IsSubDelimiter(*nCurPos) || *nCurPos == ':' || *nCurPos == '@');
}


static inline bool _local_Uri_ValidateIpvFutureAddr(const char *nAddrStr, acpl::SizeT nAddrStrLen)
{
	if (nAddrStrLen < 4 || (nAddrStr[0] != 'v' && nAddrStr[0] != 'V') || acpl::pi::_string_ascii_is_xdigit(nAddrStr[1]) != true || nAddrStr[2] != '.')
		return false;
	
	const char *oAddrStrEnd = nAddrStr + nAddrStrLen;
	
	if (oAddrStrEnd < nAddrStr)
		oAddrStrEnd = reinterpret_cast<const char *>(-1);
	
	nAddrStr += 3;
	
	
	while (nAddrStr < oAddrStrEnd && (_local_Uri_IsUnreserved(*nAddrStr) == true || _local_Uri_IsSubDelimiter(*nAddrStr) == true || *nAddrStr == ':'))
		nAddrStr++;
	
	return (nAddrStr >= oAddrStrEnd || *nAddrStr == '\0');
}

static inline bool _local_Uri_ValidateRegisteredName(const char *nRegName, acpl::SizeT nRegNameLen)
{
	const char *oRegNameEnd = nRegName + nRegNameLen;
	
	if (oRegNameEnd < nRegName)
		oRegNameEnd = reinterpret_cast<const char *>(-1);
	
	while (_local_Uri_IsPctEncoded(nRegName, reinterpret_cast<const char *>(-1)) == true || _local_Uri_IsUnreserved(*nRegName) == true || _local_Uri_IsSubDelimiter(*nRegName) == true)
		nRegName++;
	
	return (*nRegName == '\0');
}


static inline bool _local_Uri_SemiValidateScheme(const char *&nCurPos, const char *nUriEnd, acpl::Uri::ValidationLevel nLevel)
{
	if (nCurPos >= nUriEnd || acpl::pi::_string_ascii_is_alpha(*nCurPos) != true)
		return (nLevel == acpl::Uri::vlUriRef && (nCurPos >= nUriEnd || *nCurPos != ':'));
	
	const char *oLast = nUriEnd - 1;
	
	while (++nCurPos < oLast && (acpl::pi::_string_ascii_is_alnum(*nCurPos) || *nCurPos == '+' || *nCurPos == '-' || *nCurPos == '.')) ;
	
	return true;
}

static inline void _local_Uri_SemiValidateUserInfo(const char *&nCurPos, const char *nUriEnd)
{
	while (nCurPos < nUriEnd && (_local_Uri_IsPctEncoded(nCurPos, nUriEnd) == true || _local_Uri_IsUnreserved(*nCurPos) == true || _local_Uri_IsSubDelimiter(*nCurPos) == true || *nCurPos == ':'))
		nCurPos++;
}

static inline void _local_Uri_SemiValidatePath(const char *&nCurPos, const char *nUriEnd)
{
	while (nCurPos < nUriEnd && (_local_Uri_IsPchar(nCurPos, nUriEnd) == true || *nCurPos == '/'))
		nCurPos++;
}

static inline void _local_Uri_SemiValidateQuery(const char *&nCurPos, const char *nUriEnd)
{
	while (nCurPos < nUriEnd && (_local_Uri_IsPchar(nCurPos, nUriEnd) == true || *nCurPos == '/' || *nCurPos == '?'))
		nCurPos++;
}

static inline void _local_Uri_SemiValidateFragment(const char *&nCurPos, const char *nUriEnd)
{
	while (nCurPos < nUriEnd && (_local_Uri_IsPchar(nCurPos, nUriEnd) == true || *nCurPos == '/' || *nCurPos == '?'))
		nCurPos++;
}


static inline bool _local_Uri_ValidateScheme(const char *nScheme)
{
	return (_local_Uri_SemiValidateScheme(nScheme, reinterpret_cast<const char *>(-1), acpl::Uri::vlUriRef) == true && *nScheme == '\0');
}

static inline bool _local_Uri_ValidateUserInfo(const char *nUserInfo)
{
	_local_Uri_SemiValidateUserInfo(nUserInfo, reinterpret_cast<const char *>(-1));
	return (*nUserInfo == '\0');
}

static inline bool _local_Uri_ValidateHost(const char *nHost, acpl::Uri::HostType &nHostType)
{
	if ((nHostType == acpl::Uri::htUndef || nHostType == acpl::Uri::htIpv4Addr) && acpl::Ipv4Addr::Validate(nHost) == true)
		nHostType = acpl::Uri::htIpv4Addr;
	else
	if ((nHostType == acpl::Uri::htUndef || nHostType == acpl::Uri::htDomainName) && acpl::DomainName::Validate(nHost) == true)
		nHostType = acpl::Uri::htDomainName;
	else
	if ((nHostType == acpl::Uri::htUndef || nHostType == acpl::Uri::htIpv6Addr) && acpl::Ipv6Addr::Validate(nHost) == true)
		nHostType = acpl::Uri::htIpv6Addr;
	else
	if ((nHostType == acpl::Uri::htUndef || nHostType == acpl::Uri::htIpvFutureAddr) && _local_Uri_ValidateIpvFutureAddr(nHost, acpl::Num<acpl::SizeT>::Max()) == true)
		nHostType = acpl::Uri::htIpvFutureAddr;
	else
	if ((nHostType == acpl::Uri::htUndef || nHostType == acpl::Uri::htRegisteredName) && _local_Uri_ValidateRegisteredName(nHost, acpl::Num<acpl::SizeT>::Max()) == true)
		nHostType = acpl::Uri::htRegisteredName;
	else
		return false;
	
	return true;
}

static inline bool _local_Uri_ValidatePath(const char *nPath)
{
	_local_Uri_SemiValidatePath(nPath, reinterpret_cast<const char *>(-1));
	return (*nPath == '\0');
}

static inline bool _local_Uri_ValidateQuery(const char *nQuery)
{
	_local_Uri_SemiValidateQuery(nQuery, reinterpret_cast<const char *>(-1));
	return (*nQuery == '\0');
}

static inline bool _local_Uri_ValidateFragment(const char *nFragment)
{
	_local_Uri_SemiValidateFragment(nFragment, reinterpret_cast<const char *>(-1));
	return (*nFragment == '\0');
}


static inline void _local_Uri_PctEncode(const char *nRaw, const char *nRawEnd, acpl::String &nEncoded, bool nApndEncoded)
{
	static const char oAlphabet[] = "0123456789ABCDEF";
	acpl::Mem::Block<char>::Type oData;
	
	for (; nRaw < nRawEnd; nRaw++)
	{
		if (acpl::pi::_string_ascii_is_alnum(*nRaw) == true || *nRaw == '-' || *nRaw == '_' || *nRaw == '.' || *nRaw == '~')
			oData.Apnd(nRaw, 1);
		else
		{
			oData.Apnd("%", 1);
			oData.Apnd(oAlphabet + (static_cast<acpl::UInt8>(*nRaw) >> 4), 1);
			oData.Apnd(oAlphabet + (static_cast<acpl::UInt8>(*nRaw) & 0xF), 1);
		}
	}
	
	oData.Apnd("\0", 1);
	
	if (nApndEncoded == false)
		nEncoded.Set(oData.Data());
	else
		nEncoded.Apnd(oData.Data());
}

static inline void _local_Uri_PctDecode(const char *nEncoded, const char *nEncodedEnd, acpl::String &nRaw, bool nApndRaw)
{
	acpl::Mem::Block<char>::Type oData;
	
	for (; nEncoded < nEncodedEnd; nEncoded++)
	{
		if (*nEncoded != '%')
			oData.Apnd(nEncoded, 1);
		else
		{
			char oDecodChar = 0;
			
			acpl::pi::_string_ascii_char2num(nEncoded[1], 16, oDecodChar);
			acpl::pi::_string_ascii_char2num(nEncoded[2], 16, oDecodChar);
			
			oData.Apnd(&oDecodChar, 1);
			
			nEncoded += 2;
		}
	}
	
	oData.Apnd("\0", 1);
	
	if (nApndRaw == false)
		nRaw.Set(oData.Data());
	else
		nRaw.Apnd(oData.Data());
}


static inline void _local_Uri_ParseKvString(const char *nKvString, acpl::Uri::KeyValueList &nKeyValueList)
{
	nKeyValueList.Clear();
	
	if (nKvString == NULL || *nKvString == '\0')
		return;
	
	nKeyValueList.Add();
	
	
	const char *oLastStart = nKvString;
	
	for (; *nKvString != '\0'; nKvString++)
	{
		if (*nKvString == '&')
		{
			if (nKeyValueList.GetLast().GetKey().IsNull() == true)
			{
				if (nKvString > oLastStart)
				{
					_local_Uri_PctDecode(oLastStart, nKvString, nKeyValueList.GetLast().GetKey(), false);
					nKeyValueList.Add();
				}
				// else the query string starts with a '&' or there was "&&" located
			}
			else
			{
				_local_Uri_PctDecode(oLastStart, nKvString, nKeyValueList.GetLast().GetValue(), false);
				nKeyValueList.Add();
			}
			
			oLastStart = nKvString + 1;
		}
		else
		if (*nKvString == '=')
		{
			if (nKeyValueList.GetLast().GetKey().IsNull() == true)
			{
				_local_Uri_PctDecode(oLastStart, nKvString, nKeyValueList.GetLast().GetKey(), false);
				oLastStart = nKvString + 1;
			}
		}
	}
	
	
	if (nKvString > oLastStart)
	{
		if (nKeyValueList.GetLast().GetKey().IsNull() == true)
			_local_Uri_PctDecode(oLastStart, nKvString, nKeyValueList.GetLast().GetKey(), false);
		else
			_local_Uri_PctDecode(oLastStart, nKvString, nKeyValueList.GetLast().GetValue(), false);
	}
	
	if (nKeyValueList.GetLast().GetKey().IsNull() == true)
		nKeyValueList.RemoveTail(1);
}

static inline void _local_Uri_BuildKvString(const acpl::Uri::KeyValueList &nKeyValueList, acpl::String &nKvString)
{
	nKvString.Clear();
	
	if (nKeyValueList.Count() == 0)
		return;
	
	for (acpl::SizeT i = 0; i < nKeyValueList.Count(); i++)
	{
		if (i != 0)
			nKvString.Apnd("&");
		
		const acpl::Uri::KeyValue &oCurKv = nKeyValueList.Get(i);
		
		if (oCurKv.GetKey().IsEmpty() != true)
			_local_Uri_PctEncode(oCurKv.GetKey().Utf8(), (oCurKv.GetKey().Utf8() + oCurKv.GetKey().ByteLength()), nKvString, true);
		
		if (oCurKv.GetValue().IsEmpty() != true)
		{
			nKvString.Apnd("=");
			_local_Uri_PctEncode(oCurKv.GetValue().Utf8(), (oCurKv.GetValue().Utf8() + oCurKv.GetValue().ByteLength()), nKvString, true);
		}
	}
}


// PUBLIC

void acpl::Uri::Clear()
{
	this->mScheme.Clear();
	this->mHasAuthority = true;
	this->mUserInfo.Clear();
	this->mHost.Clear();
	this->mHostType = acpl::Uri::htUndef;
	this->mPort = 0;
	this->mPath.Clear();
	this->mQuery.Clear();
	this->mFragment.Clear();
}


bool acpl::Uri::Build(acpl::String &nString, acpl::Uri::ValidationLevel nLevel) const
{
	acpl::RaiiMethodR0<acpl::String, acpl::String &, &acpl::String::Clear> oRaiiClear(nString);
	
	
	nString.Clear();
	
	
	// Scheme
	
	if (this->mScheme.IsEmpty() != true)
		nString.Apnd(this->mScheme).Apnd(":");
	else
	if (nLevel != acpl::Uri::vlUriRef)
		return false;
	
	
	// Authority
	
	if (this->mHasAuthority == true)
	{
		nString.Apnd("//");
		
		
		// UserInfo
		
		if (this->mUserInfo.IsEmpty() != true)
			nString.Apnd(this->mUserInfo).Apnd("@");
		
		
		// Host
		
		if (nLevel == acpl::Uri::vlUrl && this->mHostType != acpl::Uri::htDomainName && this->mHostType != acpl::Uri::htIpv4Addr && this->mHostType != acpl::Uri::htIpv6Addr)
			return false;
		
		if (this->mHost.IsEmpty() != true)
		{
			switch (this->mHostType)
			{
				case acpl::Uri::htIpv4Addr:
				case acpl::Uri::htDomainName:
				case acpl::Uri::htRegisteredName:
					nString.Apnd(this->mHost);
					break;
				
				case acpl::Uri::htIpv6Addr:
				case acpl::Uri::htIpvFutureAddr:
					nString.Apnd("[").Apnd(this->mHost).Apnd("]");
					break;
				
				case acpl::Uri::htUndef:
					break;
			}
		}
		
		
		// Port
		
		if (this->mPort > 0)
			nString.Apnd(":").Apnd(acpl::String::IntRef(this->mPort));
	}
	else
	if (nLevel == acpl::Uri::vlUrl)
		return false;
	
	
	// Path
	
	if (this->mPath.IsEmpty() != true)
	{
		if (this->mHasAuthority == true)
		{
			if (this->mPath.Utf8()[0] != '/') // path is not absolute
				return false;
		}
		else
		{
			if (this->mPath.Utf8()[0] == '/' && this->mPath.Utf8()[1] == '/') // whether or not scheme is present, path cannot start with "//"
				return false;
			else
			if (this->mScheme.IsEmpty() == true) // scheme is empty
			{
				for (const char *oCurPos = this->mPath.Utf8(); *oCurPos != '\0' && *oCurPos != '/'; oCurPos++)
					if (*oCurPos == ':') // ':' appears before a '/' does
						return false;
			}
		}
		
		nString.Apnd(this->mPath);
	}
	
	
	// Query
	
	if (this->mQuery.IsEmpty() != true)
		nString.Apnd("?").Apnd(this->mQuery);
	
	
	// Fragment
	
	if (this->mFragment.IsEmpty() != true)
		nString.Apnd("#").Apnd(this->mFragment);
	
	
	oRaiiClear.Disable();
	return true;
}


bool acpl::Uri::SetScheme(const acpl::String::Ref &nScheme)
{
	acpl::String oScheme(nScheme);
	
	if (_local_Uri_ValidateScheme(oScheme.Utf8()) != true)
		return false;
	
	this->mScheme.Set(oScheme);
	return true;
}

bool acpl::Uri::SetUserInfo(const acpl::String::Ref &nUserInfo)
{
	acpl::String oUserInfo(nUserInfo);
	
	if (_local_Uri_ValidateUserInfo(oUserInfo.Utf8()) != true)
		return false;
	
	this->mUserInfo.Set(oUserInfo);
	return true;
}

bool acpl::Uri::SetHost(const acpl::String::Ref &nHost, acpl::Uri::HostType nHostTypeHint)
{
	acpl::String oHost(nHost);
	
	if (_local_Uri_ValidateHost(oHost.Utf8(), nHostTypeHint) != true)
		return false;
	
	this->mHost.Set(oHost);
	this->mHostType = nHostTypeHint;
	return true;
}


bool acpl::Uri::SetPath(const acpl::String::Ref &nPath)
{
	acpl::String oPath(nPath);
	
	if (_local_Uri_ValidatePath(oPath.Utf8()) != true)
		return false;
	
	this->mPath.Set(oPath);
	return true;
}

void acpl::Uri::GetQuery(acpl::Uri::KeyValueList &nKeyValueList) const
{
	_local_Uri_ParseKvString(this->mQuery.Utf8(), nKeyValueList);
}

bool acpl::Uri::SetQuery(const acpl::String::Ref &nQuery)
{
	acpl::String oQuery(nQuery);
	
	if (_local_Uri_ValidateQuery(oQuery.Utf8()) != true)
		return false;
	
	this->mQuery.Set(oQuery);
	return true;
}

void acpl::Uri::SetQuery(const acpl::Uri::KeyValueList &nKeyValueList)
{
	_local_Uri_BuildKvString(nKeyValueList, this->mQuery);
}

bool acpl::Uri::SetFragment(const acpl::String::Ref &nFragment)
{
	acpl::String oFragment(nFragment);
	
	if (_local_Uri_ValidateFragment(oFragment.Utf8()) != true)
		return false;
	
	this->mFragment.Set(oFragment);
	return true;
}


void acpl::Uri::PctEncode(const acpl::String::Ref &nRaw, acpl::String &nEncoded, bool nApndEncoded) // static
{
	acpl::String oRaw(nRaw);
	_local_Uri_PctEncode(oRaw.Utf8(), (oRaw.Utf8() + oRaw.ByteLength()), nEncoded, nApndEncoded);
}

void acpl::Uri::PctDecode(const acpl::String::Ref &nEncoded, acpl::String &nRaw, bool nApndRaw) // static
{
	acpl::String oEncoded(nEncoded);
	_local_Uri_PctDecode(oEncoded.Utf8(), (oEncoded.Utf8() + oEncoded.ByteLength()), nRaw, nApndRaw);
}


void acpl::Uri::ParseKvString(const acpl::String::Ref &nKvString, acpl::Uri::KeyValueList &nKeyValueList) // static
{
	acpl::String oQuery(nKvString);
	_local_Uri_ParseKvString(oQuery.Utf8(), nKeyValueList);
}

void acpl::Uri::BuildKvString(const acpl::Uri::KeyValueList &nKeyValueList, acpl::String &nKvString) // static
{
	_local_Uri_BuildKvString(nKeyValueList, nKvString);
}


static inline bool _local_Uri_ParseScheme(const char *&nCurPos, const char *nUriEnd, acpl::Uri::ValidationLevel nLevel, _local_Uri_Hack_T &nUriHt)
{
	const char *oFirst = nCurPos;
	
	if (_local_Uri_SemiValidateScheme(nCurPos, nUriEnd, nLevel) != true)
		return false;
	
	if (*nCurPos == ':')
	{
		if (nUriHt.mScheme != NULL)
			nUriHt.mScheme->Set(oFirst, (nCurPos - oFirst));
		
		nCurPos++;
		return true;
	}
	else
	{
		nCurPos = oFirst;
		return (nLevel == acpl::Uri::vlUriRef);
	}
}

static inline bool _local_Uri_ParseAuth(const char *&nCurPos, const char *nUriEnd, acpl::Uri::ValidationLevel nLevel, _local_Uri_Hack_T &nUriHt)
{
	if ((nCurPos + 2) > nUriEnd || nCurPos[0] != '/' || nCurPos[1] != '/') // does not start with "//" at all
	{
		if (nUriHt.mHasAuthority != NULL)
			*nUriHt.mHasAuthority = false;
		return (nLevel != acpl::Uri::vlUrl);
	}
	else
	{
		if (nUriHt.mHasAuthority != NULL)
			*nUriHt.mHasAuthority = true;
	}
	
	nCurPos += 2;
	
	// starts with "//", "///", "//?", or "//#" which means that there is no authority
	// if the third char is '/', '?', or '#', it indicates the beginning of Path, Query, or Fragment, respectively
	if (nCurPos >= nUriEnd || nCurPos[0] == '\0' || nCurPos[0] == '/' || nCurPos[0] == '?' || nCurPos[0] == '#')
		return (nLevel != acpl::Uri::vlUrl);
	
	
	const char *oElemBegin = nCurPos;
	
	
	if (*oElemBegin != '[') // if the first char is '[' (invalid char for UserInfo), skip the UserInfo parsing (as it's not UserInfo), but it possibly is IPv6 or IPvFuture Host
	{
		// (possible) UserInfo
		
		_local_Uri_SemiValidateUserInfo(nCurPos, nUriEnd); // find the first delimiter or invalid char
		
		if (nCurPos >= nUriEnd || *nCurPos == '\0' || *nCurPos == '/' || *nCurPos == '?' || *nCurPos == '#')
			nCurPos = oElemBegin;
		else
		if (*nCurPos == '@') // nCurPos < oBfrEnd
		{
			if (nUriHt.mUserInfo != NULL)
				nUriHt.mUserInfo->Set(oElemBegin, (nCurPos - oElemBegin));
			nCurPos++;
		}
		else
			return false;
	}
	
	
	// (possible) Host
	
	if (nCurPos >= nUriEnd || *nCurPos == '\0' || *nCurPos == '/' || *nCurPos == '?' || *nCurPos == '#')
		return (nLevel != acpl::Uri::vlUrl);
	
	oElemBegin = nCurPos;
	const char *oHostEnd = nCurPos;
	
	if (*oElemBegin != '[')
	{
		// oElemBegin and oHostEnd are already set appropriately
		
		while (oHostEnd < nUriEnd && (_local_Uri_IsPctEncoded(oHostEnd, nUriEnd) == true || _local_Uri_IsUnreserved(*oHostEnd) == true || _local_Uri_IsSubDelimiter(*oHostEnd) == true))
			oHostEnd++;
		
		if (acpl::Ipv4Addr::Validate(oElemBegin, (oHostEnd - oElemBegin)) == true)
		{
			if (nUriHt.mHostType != NULL)
				*nUriHt.mHostType = acpl::Uri::htIpv4Addr;
		}
		else
		if (acpl::DomainName::Validate(oElemBegin, (oHostEnd - oElemBegin)) == true)
		{
			if (nUriHt.mHostType != NULL)
				*nUriHt.mHostType = acpl::Uri::htDomainName;
		}
		else
		if (nLevel != acpl::Uri::vlUrl)
		{
			if (nUriHt.mHostType != NULL)
				*nUriHt.mHostType = acpl::Uri::htRegisteredName; // is already validated registered name
		}
		else
			return false;
		
		nCurPos = oHostEnd;
	}
	else
	{
		// *oElemBegin == '['
		oHostEnd = ++oElemBegin;
		
		while (oHostEnd < nUriEnd && (_local_Uri_IsUnreserved(*oHostEnd) == true || _local_Uri_IsSubDelimiter(*oHostEnd) == true || *oHostEnd == ':'))
			oHostEnd++;
		
		if (oHostEnd >= nUriEnd || *oHostEnd != ']')
			return false;
		
		if (nLevel != acpl::Uri::vlUrl && _local_Uri_ValidateIpvFutureAddr(oElemBegin, (oHostEnd - oElemBegin)) == true)
		{
			if (nUriHt.mHostType != NULL)
				*nUriHt.mHostType = acpl::Uri::htIpvFutureAddr;
		}
		else
		if (acpl::Ipv6Addr::Validate(oElemBegin, (oHostEnd - oElemBegin)) == true)
		{
			if (nUriHt.mHostType != NULL)
				*nUriHt.mHostType = acpl::Uri::htIpv6Addr;
		}
		else
			return false;
		
		nCurPos = oHostEnd + 1;
	}
	
	
	if (nUriHt.mHost != NULL)
		nUriHt.mHost->Set(oElemBegin, (oHostEnd - oElemBegin));
	
	
	// (possible) Port
	
	if (nCurPos < nUriEnd && *nCurPos == ':')
	{
		acpl::UInt32 oPort = 0;
		while (++nCurPos < nUriEnd && acpl::pi::_string_ascii_char2num(*nCurPos, 10, oPort) == true)
		{
			if (oPort > 0xFFFF)
				return false;
		}
		
		if (nUriHt.mPort != NULL)
			*nUriHt.mPort = oPort;
	}
	
	
	return (nCurPos >= nUriEnd || *nCurPos == '\0' || *nCurPos == '/' || *nCurPos == '?' || *nCurPos == '#');
}

static inline bool _local_Uri_ParsePath(const char *&nCurPos, const char *nUriEnd, _local_Uri_Hack_T &nUriHt)
{
	if (nCurPos < nUriEnd && *nCurPos != '\0')
	{
		const char *oBegin = nCurPos;
		
		// if there was authority, the authority parser made sure that the first of possible path is '/'
		
		_local_Uri_SemiValidatePath(nCurPos, nUriEnd);
		
		if (nCurPos >= nUriEnd || *nCurPos == '\0' || *nCurPos == '?' || *nCurPos == '#')
		{
			if (nUriHt.mPath != NULL)
				nUriHt.mPath->Set(oBegin, (nCurPos - oBegin));
			return true;
		}
		else
			return false;
	}
	else
	{
		if (nUriHt.mPath != NULL)
			nUriHt.mPath->Set("");
		return true;
	}
}

static inline bool _local_Uri_ParseQuery(const char *&nCurPos, const char *nUriEnd, _local_Uri_Hack_T &nUriHt)
{
	if (nCurPos < nUriEnd && *nCurPos == '?')
	{
		const char *oBegin = ++nCurPos;
		
		_local_Uri_SemiValidateQuery(nCurPos, nUriEnd);
		
		if (nCurPos >= nUriEnd || *nCurPos == '\0' || *nCurPos == '#')
		{
			if (nUriHt.mQuery != NULL)
				nUriHt.mQuery->Set(oBegin, (nCurPos - oBegin));
			return true;
		}
		else
			return false;
	}
	else
	{
		if (nUriHt.mQuery != NULL)
			nUriHt.mQuery->Set("");
		return true;
	}
}

static inline bool _local_Uri_ParseFragment(const char *&nCurPos, const char *nUriEnd, _local_Uri_Hack_T &nUriHt)
{
	if (nCurPos < nUriEnd && *nCurPos == '#')
	{
		const char *oBegin = ++nCurPos;
		
		_local_Uri_SemiValidateFragment(nCurPos, nUriEnd);
		
		if (nCurPos >= nUriEnd || *nCurPos == '\0')
		{
			if (nUriHt.mFragment != NULL)
				nUriHt.mFragment->Set(oBegin, (nCurPos - oBegin));
			return true;
		}
		else
			return false;
	}
	else
	{
		if (nUriHt.mFragment != NULL)
			nUriHt.mFragment->Set("");
		return true;
	}
}

bool acpl::Uri::Parse(const acpl::String::Ref &nUri, acpl::Uri::ValidationLevel nLevel, acpl::Uri *nUriObj) // static
{
	_local_Uri_Hack_T oUriHt;
	acpl::RaiiMethod0<_local_Uri_Hack_T, &_local_Uri_Hack_T::Clear> oRcUriHt(oUriHt);
	
	if (nUriObj != NULL)
	{
		oUriHt.mUri = nUriObj;
		oUriHt.mScheme = &nUriObj->mScheme;
		oUriHt.mHasAuthority = &nUriObj->mHasAuthority;
		oUriHt.mUserInfo = &nUriObj->mUserInfo;
		oUriHt.mHost = &nUriObj->mHost;
		oUriHt.mHostType = &nUriObj->mHostType;
		oUriHt.mPort = &nUriObj->mPort;
		oUriHt.mPath = &nUriObj->mPath;
		oUriHt.mQuery = &nUriObj->mQuery;
		oUriHt.mFragment = &nUriObj->mFragment;
	}
	
	oUriHt.Clear();
	
	
	if (nUriObj != NULL)
	{
		nUriObj->Clear();
		nUriObj->mHasAuthority = false;
	}
	
	acpl::String oUri(nUri);
	
	if (oUri.IsEmpty() == true)
	{
		if (nLevel == acpl::Uri::vlUriRef)
		{
			oRcUriHt.Disable();
			return true;
		}
		else
			return false;
	}
	
	
	const char *oCurPos = oUri.Utf8();
	const char *oUriEnd = oCurPos + oUri.ByteLength();
	
	
	// (possible) Scheme
	if (_local_Uri_ParseScheme(oCurPos, oUriEnd, nLevel, oUriHt) != true)
		return false;
	
	// (possible) Authority == [ UserInfo "@" ] Host [ ":" Port ]
	if (_local_Uri_ParseAuth(oCurPos, oUriEnd, nLevel, oUriHt) != true)
		return false;
	
	// (possible) Path
	if (_local_Uri_ParsePath(oCurPos, oUriEnd, oUriHt) != true)
		return false;
	
	// (possible) Query
	if (_local_Uri_ParseQuery(oCurPos, oUriEnd, oUriHt) != true)
		return false;
	
	// (possible) Fragment
	if (_local_Uri_ParseFragment(oCurPos, oUriEnd, oUriHt) != true)
		return false;
	
	oRcUriHt.Disable();
	return true;
}
