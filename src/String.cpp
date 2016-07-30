#include "String.h"

#include "_cp/Memory.inc.cpp"
#include "_i18n/charsets/Charset_UTF_8.inc.cpp"

#include "net/Address.h"
#include "Time.h"


// 
// acpl::String
// 

static inline bool _local_String_Gen_IsBlank(const acpl::UInt8 *nPtr)
{
	return (*nPtr == 0x20 || *nPtr == 0x09); // U+0020=space, U+0009=hor-tab
}

static inline bool _local_String_Gen_IsBlank(const char *nPtr)
{
	return (_local_String_Gen_IsBlank(reinterpret_cast<const acpl::UInt8 *>(nPtr)));
}

static inline bool _local_String_Gen_IsWspace(const acpl::UInt8 *nPtr)
{
	return (
			*nPtr == 0x20 || *nPtr == 0x09 || *nPtr == 0x0A ||					// U+0020=space, U+0009=hor-tab, U+000A=line-feed
			*nPtr == 0x0D || *nPtr == 0x0B || *nPtr == 0x0C ||					// U+000D-carriage-return, U+000B=ver-tab, U+000C=form-feed
			(nPtr[0] == 0xC2 && (nPtr[1] == 0x85 || nPtr[1] == 0xA0)) ||		// U+0085=next-line, U+00A0=nbsp
			(nPtr[0] == 0xE1 && (
				(nPtr[1] == 0x9A && nPtr[2] == 0x80) ||							// U+1680=ogham-space-mark
				(nPtr[1] == 0xA0 && nPtr[2] == 0x8E))) ||						// U+180E=mongolian-vowel-separator
			(nPtr[0] == 0xE2 && (
				(nPtr[1] == 0x80 && (
					(nPtr[2] >= 0x80 && nPtr[2] <= 0x8A) ||						// U+2000=en-quad, U+2001=em-quad, U+2002=en-space, U+2003=em-space, U+2004=three-per-em-space, U+2005=four-per-em-space, U+2006=six-per-em-space, U+2007=figure-space, U+2008=punctuation-space, U+2009=thin-space, U+200A=hair-space
					nPtr[2] == 0xA8 || nPtr[2] == 0xA9 || nPtr[2] == 0xAF)) ||	// U+2028=line-separator, U+2029=paragraph-separator, U+202F=narrow-nbsp
				(nPtr[1] == 0x81 && nPtr[2] == 0x9F))) ||						// U+205F=medium-mathematical-space
			(nPtr[0] == 0xE3 && nPtr[1] == 0x80 && nPtr[2] == 0x80)				// U+3000=ideographic-space
		);
}

static inline bool _local_String_Gen_IsWspace(const char *nPtr)
{
	return (_local_String_Gen_IsWspace(reinterpret_cast<const acpl::UInt8 *>(nPtr)));
}

static inline bool _local_String_Gen_AllocRefString(char *&nAlloced, const acpl::String::Ref &nRef, acpl::SizeT nMaxCount = acpl::Num<acpl::SizeT>::Max(), acpl::SizeT nStartOffset = 0)
{
	acpl::SizeT oRefOff = 0;
	acpl::SizeT oEncByteSize = 0;
	
	nRef.Measure(nMaxCount, nStartOffset, oRefOff, oEncByteSize);
	if (acpl::Mem::Alloc(nAlloced, oEncByteSize + 1) == true)
	{
		nRef.Convert(oRefOff, oEncByteSize, nAlloced);
		nAlloced[oEncByteSize] = '\0';
		return true;
	}
	
	return false;
}

static inline const char *_local_String_Gen_GetRefString(char *&nStrAlloced, const acpl::String::Ref &nRef)
{
	if (nRef.GetType() == acpl::String::Ref::rtSelf)
		return static_cast<const char *>(nRef.GetPtr());
	else
	{
		if (_local_String_Gen_AllocRefString(nStrAlloced, nRef) == true)
			return nStrAlloced;
		else
			return "";
	}
}

#define _local_String_Gen_MakeRefString(N_VAR, N_REF) \
 	char *N_VAR ## Alloced = NULL; \
 	acpl::Mem::RaiiFree<char *> N_VAR ## Raii(N_VAR ## Alloced); \
 	const char *N_VAR = _local_String_Gen_GetRefString(N_VAR ## Alloced, N_REF);

static inline bool _local_String_Gen_PreparePos(const acpl::Mem::Block<char>::Type &nStrBuf, bool nBef, const char *&nPosPtr)
{
	if (nPosPtr >= nStrBuf.Data() && nPosPtr < (nStrBuf.Data() + nStrBuf.Length()))
	{
		if (acpl::pi::_string_utf8_check_main_byte(*nPosPtr) != true || (nBef != true && *nPosPtr != '\0'))
			acpl::pi::_string_utf8_next_seq(nPosPtr);
		
		return true;
	}
	else
		return false;
}

// PUBLIC

bool acpl::String::IsAllBlanks() const
{
	const char *oPtr = ((this->mStrBuf.Length() != 0) ? this->mStrBuf.Data() : "");
	for (; _local_String_Gen_IsBlank(oPtr) == true; acpl::pi::_string_utf8_next_seq(oPtr));
	return (*oPtr == '\0');
}

bool acpl::String::IsAllWspaces() const
{
	const char *oPtr = ((this->mStrBuf.Length() != 0) ? this->mStrBuf.Data() : "");
	for (; _local_String_Gen_IsWspace(oPtr) == true; acpl::pi::_string_utf8_next_seq(oPtr));
	return (*oPtr == '\0');
}

acpl::SizeT acpl::String::Length() const
{
	acpl::SizeT oLen = 0;
	
	for (const char *oPtr = this->Utf8(); *oPtr != '\0'; acpl::pi::_string_utf8_next_seq(oPtr))
		oLen++;
	
	return oLen;
}


static inline bool _local_String_Asgn_PrepareRef(const acpl::String::Ref &nRef, acpl::SizeT nMaxCount, acpl::SizeT nStartOffset, acpl::SizeT &nRefOff, acpl::SizeT &nEncByteSize)
{
	if (nRef.IsNull() == false)
	{
		nRef.Measure(nMaxCount, nStartOffset, nRefOff, nEncByteSize);
		return (nEncByteSize != 0);
	}
	else
		return false;
}


static void _local_String_Set(const acpl::String::Ref &nRef, acpl::SizeT nRefOff, acpl::SizeT nEncByteSize, acpl::Mem::Block<char>::Type &nStrBuf)
{
	if (nStrBuf.SetLength(nEncByteSize + 1) == true)
	{
		char *oData = nStrBuf.Data();
		nRef.Convert(nRefOff, nEncByteSize, oData);
		oData[nEncByteSize] = '\0';
	}
	else
		nStrBuf.Clear().Apnd("", 1);
}

acpl::String &acpl::String::Set(const acpl::String::Ref &nRef, acpl::SizeT nMaxCount, acpl::SizeT nStartOffset)
{
	acpl::SizeT oRefOff;
	acpl::SizeT oEncByteSize;
	
	if (_local_String_Asgn_PrepareRef(nRef, nMaxCount, nStartOffset, oRefOff, oEncByteSize) == true)
		_local_String_Set(nRef, oRefOff, oEncByteSize, this->mStrBuf);
	else
		this->Clear();
	
	return *this;
}

static void _local_String_Apnd(const acpl::String::Ref &nRef, acpl::SizeT nRefOff, acpl::SizeT nEncByteSize, acpl::Mem::Block<char>::Type &nStrBuf, acpl::SizeT nCurSize)
{
	if (nStrBuf.SetLength(nCurSize + nEncByteSize) == true)
	{
		char *oData = nStrBuf.Data();
		nCurSize--;
		nRef.Convert(nRefOff, nEncByteSize, oData + nCurSize);
		oData[nCurSize + nEncByteSize] = '\0';
	}
}

acpl::String &acpl::String::Apnd(const acpl::String::Ref &nRef, acpl::SizeT nMaxCount, acpl::SizeT nStartOffset)
{
	acpl::SizeT oRefOff;
	acpl::SizeT oEncByteSize;
	
	if (_local_String_Asgn_PrepareRef(nRef, nMaxCount, nStartOffset, oRefOff, oEncByteSize) == true)
	{
		acpl::SizeT oCurSize = this->mStrBuf.Length();
		if (oCurSize > 1)
			_local_String_Apnd(nRef, oRefOff, oEncByteSize, this->mStrBuf, oCurSize);
		else
			_local_String_Set(nRef, oRefOff, oEncByteSize, this->mStrBuf);
	}
	
	return *this;
}

static void _local_String_Ins(const acpl::String::Ref &nRef, acpl::SizeT nRefOff, acpl::SizeT nEncByteSize, acpl::Mem::Block<char>::Type &nStrBuf, acpl::SizeT nCurSize, acpl::SizeT nByteOff)
{
	if (nStrBuf.SetLength(nCurSize + nEncByteSize) == true)
	{
		char *oData = nStrBuf.Data();
		char *oPos = oData + nByteOff;
		acpl::Mem::Move(oPos + nEncByteSize, oPos, oData + nCurSize - oPos);
		nRef.Convert(nRefOff, nEncByteSize, oPos);
	}
}

acpl::String &acpl::String::Ins(acpl::SizeT nIndex, const acpl::String::Ref &nRef, acpl::SizeT nMaxCount, acpl::SizeT nStartOffset)
{
	acpl::SizeT oRefOff;
	acpl::SizeT oEncByteSize;
	
	if (_local_String_Asgn_PrepareRef(nRef, nMaxCount, nStartOffset, oRefOff, oEncByteSize) == true)
	{
		acpl::SizeT oCurSize = this->mStrBuf.Length();
		if (oCurSize > 1)
		{
			const char *oData = this->mStrBuf.Data();
			const char *oPos = oData;
			acpl::pi::_string_utf8_skip_fwd(oPos, nIndex);
			
			if (*oPos != '\0')
				_local_String_Ins(nRef, oRefOff, oEncByteSize, this->mStrBuf, oCurSize, oPos - oData);
			else
				_local_String_Apnd(nRef, oRefOff, oEncByteSize, this->mStrBuf, oCurSize);
		}
		else
			_local_String_Set(nRef, oRefOff, oEncByteSize, this->mStrBuf);
	}
	
	return *this;
}

acpl::String &acpl::String::Ins(const acpl::String::Pos &nAtPos, bool nBef, const acpl::String::Ref &nRef, acpl::SizeT nMaxCount, acpl::SizeT nStartOffset)
{
	acpl::SizeT oRefOff;
	acpl::SizeT oEncByteSize;
	
	if (_local_String_Asgn_PrepareRef(nRef, nMaxCount, nStartOffset, oRefOff, oEncByteSize) == true)
	{
		acpl::SizeT oCurSize = this->mStrBuf.Length();
		if (oCurSize > 1)
		{
			const char *oPos = nAtPos.Utf8();
			if (_local_String_Gen_PreparePos(this->mStrBuf, nBef, oPos) == true)
			{
				if (*oPos != '\0')
					_local_String_Ins(nRef, oRefOff, oEncByteSize, this->mStrBuf, oCurSize, oPos - this->mStrBuf.Data());
				else
					_local_String_Apnd(nRef, oRefOff, oEncByteSize, this->mStrBuf, oCurSize);
			}
		}
		else
			_local_String_Set(nRef, oRefOff, oEncByteSize, this->mStrBuf);
	}
	
	return *this;
}

static void _local_String_Repl(const acpl::String::Ref &nRef, acpl::SizeT nRefOff, acpl::SizeT nEncByteSize, acpl::Mem::Block<char>::Type &nStrBuf, acpl::SizeT nCurSize, acpl::SizeT nBegByteOff, acpl::SizeT nEndByteOff)
{
	if (nStrBuf.SetLength(nCurSize + nEncByteSize) == true)
	{
		char *oData = nStrBuf.Data();
		char *oBegPos = oData + nBegByteOff;
		char *oEndPos = oData + nEndByteOff;
		char *oReplEndPos = oBegPos + nEncByteSize;
		
		if (oReplEndPos != oEndPos)
			acpl::Mem::Move(oReplEndPos, oEndPos, nStrBuf.Data() + nCurSize - oEndPos);
		
		nRef.Convert(nRefOff, nEncByteSize, oBegPos);
		
		nStrBuf.SetLength(nCurSize + nEncByteSize - (nEndByteOff - nBegByteOff));
	}
}

acpl::String &acpl::String::Repl(acpl::SizeT nIndex, acpl::SizeT nCount, const acpl::String::Ref &nRef, acpl::SizeT nMaxCount, acpl::SizeT nStartOffset)
{
	acpl::SizeT oRefOff;
	acpl::SizeT oEncByteSize;
	
	if (_local_String_Asgn_PrepareRef(nRef, nMaxCount, nStartOffset, oRefOff, oEncByteSize) == true)
	{
		acpl::SizeT oCurSize = this->mStrBuf.Length();
		if (oCurSize > 1)
		{
			const char *oData = this->mStrBuf.Data();
			const char *oBegPos = oData;
			acpl::pi::_string_utf8_skip_fwd(oBegPos, nIndex);
			
			if (*oBegPos != '\0')
			{
				const char *oEndPos = oBegPos;
				acpl::pi::_string_utf8_skip_fwd(oEndPos, nCount);
				
				if (oEndPos > oBegPos)
					_local_String_Repl(nRef, oRefOff, oEncByteSize, this->mStrBuf, oCurSize, oBegPos - oData, oEndPos - oData);
				else
					_local_String_Ins(nRef, oRefOff, oEncByteSize, this->mStrBuf, oCurSize, oBegPos - oData);
			}
			else
				_local_String_Apnd(nRef, oRefOff, oEncByteSize, this->mStrBuf, oCurSize);
		}
		else
			_local_String_Set(nRef, oRefOff, oEncByteSize, this->mStrBuf);
	}
	
	return *this;
}

acpl::String &acpl::String::Repl(const acpl::String::Pos &nAtPos, bool nBef, acpl::SizeT nCount, const acpl::String::Ref &nRef, acpl::SizeT nMaxCount, acpl::SizeT nStartOffset)
{
	acpl::SizeT oRefOff;
	acpl::SizeT oEncByteSize;
	
	if (_local_String_Asgn_PrepareRef(nRef, nMaxCount, nStartOffset, oRefOff, oEncByteSize) == true)
	{
		acpl::SizeT oCurSize = this->mStrBuf.Length();
		if (oCurSize > 1)
		{
			const char *oBegPos = nAtPos.Utf8();
			if (_local_String_Gen_PreparePos(this->mStrBuf, nBef, oBegPos) == true)
			{
				if (*oBegPos != '\0')
				{
					const char *oData = this->mStrBuf.Data();
					const char *oEndPos = oBegPos;
					acpl::pi::_string_utf8_skip_fwd(oEndPos, nCount);
					
					if (oEndPos > oBegPos)
						_local_String_Repl(nRef, oRefOff, oEncByteSize, this->mStrBuf, oCurSize, oBegPos - oData, oEndPos - oData);
					else
						_local_String_Ins(nRef, oRefOff, oEncByteSize, this->mStrBuf, oCurSize, oBegPos - oData);
				}
				else
					_local_String_Apnd(nRef, oRefOff, oEncByteSize, this->mStrBuf, oCurSize);
			}
		}
		else
			_local_String_Set(nRef, oRefOff, oEncByteSize, this->mStrBuf);
	}
	
	return *this;
}

acpl::String &acpl::String::Repl(const acpl::String::Pos &nFromPos, bool nBef, const acpl::String::Pos &nToPos, bool nExcl, const acpl::String::Ref &nRef, acpl::SizeT nMaxCount, acpl::SizeT nStartOffset)
{
	acpl::SizeT oRefOff;
	acpl::SizeT oEncByteSize;
	
	if (_local_String_Asgn_PrepareRef(nRef, nMaxCount, nStartOffset, oRefOff, oEncByteSize) == true)
	{
		acpl::SizeT oCurSize = this->mStrBuf.Length();
		if (oCurSize > 1)
		{
			const char *oBegPos = nFromPos.Utf8();
			if (_local_String_Gen_PreparePos(this->mStrBuf, nBef, oBegPos) == true)
			{
				if (*oBegPos != '\0')
				{
					const char *oEndPos = nToPos.Utf8();
					if (nFromPos.IsSameString(nToPos) == true && _local_String_Gen_PreparePos(this->mStrBuf, nExcl, oEndPos) == true)
					{
						const char *oData = this->mStrBuf.Data();
						
						if (oEndPos > oBegPos)
							_local_String_Repl(nRef, oRefOff, oEncByteSize, this->mStrBuf, oCurSize, oBegPos - oData, oEndPos - oData);
						else
							_local_String_Ins(nRef, oRefOff, oEncByteSize, this->mStrBuf, oCurSize, oBegPos - oData);
					}
				}
				else
					_local_String_Apnd(nRef, oRefOff, oEncByteSize, this->mStrBuf, oCurSize);
			}
		}
		else
			_local_String_Set(nRef, oRefOff, oEncByteSize, this->mStrBuf);
	}
	
	return *this;
}



static inline void _local_String_Pad(const char *nEncChar, acpl::SizeT nEncCharSize, char *nPtr, acpl::SizeT nNumBytes)
{
	for (acpl::SizeT i = 0; i < nNumBytes; i++)
		nPtr[i] = nEncChar[i % nEncCharSize];
}

acpl::String &acpl::String::PadLeft(acpl::Unichar nUc, acpl::SizeT nNewLen)
{
	char oEncChar[7];
	acpl::SizeT oEncCharSize = acpl::i18n::Charset_UTF_8::Encode(nUc, reinterpret_cast<acpl::UInt8 *>(oEncChar), 6);
	oEncChar[oEncCharSize] = '\0';
	
	acpl::SizeT oLen = this->Length();
	if (nNewLen > oLen && *oEncChar != '\0')
	{
		acpl::SizeT oEncStrSize = oEncCharSize * (nNewLen - oLen);
		
		if (this->mStrBuf.Length() == 0)
		{
			this->mStrBuf.SetLength(oEncStrSize + 1);
			_local_String_Pad(oEncChar, oEncCharSize, this->mStrBuf.Data(), oEncStrSize);
			this->mStrBuf.Data()[oEncStrSize] = '\0';
		}
		else
		{
			acpl::SizeT oCurSize = this->mStrBuf.Length();
			this->mStrBuf.SetLength(oCurSize + oEncStrSize);
			acpl::Mem::Move(this->mStrBuf.Data() + oEncStrSize, this->mStrBuf.Data(), oCurSize);
			_local_String_Pad(oEncChar, oEncCharSize, this->mStrBuf.Data(), oEncStrSize);
		}
	}
	
	return *this;
}

acpl::String &acpl::String::PadRight(acpl::Unichar nUc, acpl::SizeT nNewLen)
{
	char oEncChar[7];
	acpl::SizeT oEncCharSize = acpl::i18n::Charset_UTF_8::Encode(nUc, reinterpret_cast<acpl::UInt8 *>(oEncChar), 6);
	oEncChar[oEncCharSize] = '\0';
	
	acpl::SizeT oLen = this->Length();
	if (nNewLen > oLen && *oEncChar != '\0')
	{
		acpl::SizeT oEncStrSize = oEncCharSize * (nNewLen - oLen);
		
		if (this->mStrBuf.Length() == 0)
		{
			this->mStrBuf.SetLength(oEncStrSize + 1);
			_local_String_Pad(oEncChar, oEncCharSize, this->mStrBuf.Data(), oEncStrSize);
			this->mStrBuf.Data()[oEncStrSize] = '\0';
		}
		else
		{
			acpl::SizeT oCurSize = this->mStrBuf.Length();
			this->mStrBuf.SetLength(oCurSize + oEncStrSize);
			oCurSize--; // without NULL terminator
			_local_String_Pad(oEncChar, oEncCharSize, this->mStrBuf.Data() + oCurSize, oEncStrSize);
			this->mStrBuf.Data()[oCurSize + oEncStrSize] = '\0';
		}
	}
	
	return *this;
}


acpl::String &acpl::String::Clear()
{
	this->mStrBuf.Clear();
	this->mStrBuf.Apnd("", 1);
	return *this;
}

acpl::String &acpl::String::Purge()
{
	this->mStrBuf.Purge();
	return *this;
}

acpl::String &acpl::String::Cut(acpl::SizeT nIndex, acpl::SizeT nCount)
{
	if (nCount != 0 && this->mStrBuf.Length() != 0)
	{
		const char *oData = this->mStrBuf.Data();
		const char *oBegPos = oData;
		acpl::pi::_string_utf8_skip_fwd(oBegPos, nIndex);
		
		if (*oBegPos != '\0')
		{
			const char *oEndPos = oBegPos;
			acpl::pi::_string_utf8_skip_fwd(oEndPos, nCount);
			
			this->mStrBuf.Cut((oBegPos - oData), (oEndPos - oBegPos));
		}
	}

	return *this;
}

acpl::String &acpl::String::Cut(const acpl::String::Pos &nAtPos, bool nBef, acpl::SizeT nCount)
{
	const char *oBegPos = nAtPos.Utf8();
	if (nCount != 0 && _local_String_Gen_PreparePos(this->mStrBuf, nBef, oBegPos) == true && *oBegPos != '\0') // `this->mStrBuf.Length()` is checked implicitly in `_local_String_Gen_PreparePos()`
	{
		const char *oEndPos = oBegPos;
		acpl::pi::_string_utf8_skip_fwd(oEndPos, nCount);
		
		this->mStrBuf.Cut((oBegPos - this->mStrBuf.Data()), (oEndPos - oBegPos));
	}
	
	return *this;
}

acpl::String &acpl::String::Cut(const acpl::String::Pos &nFromPos, bool nBef, const acpl::String::Pos &nToPos, bool nExcl)
{
	const char *oBegPos = nFromPos.Utf8();
	if (nFromPos.IsSameString(nToPos) == true && _local_String_Gen_PreparePos(this->mStrBuf, nBef, oBegPos) == true && *oBegPos != '\0') // `this->mStrBuf.Length()` is checked implicitly in `_local_String_Gen_PreparePos()`
	{
		const char *oEndPos = nToPos.Utf8();
		if (_local_String_Gen_PreparePos(this->mStrBuf, nExcl, oEndPos) == true && oEndPos > oBegPos)
			this->mStrBuf.Cut((oBegPos - this->mStrBuf.Data()), (oEndPos - oBegPos));
	}
	
	return *this;
}

acpl::String &acpl::String::CutLeft(acpl::SizeT nCount)
{
	if (nCount != 0 && this->mStrBuf.Length() != 0)
	{
		const char *oData = this->mStrBuf.Data();
		const char *oPos = oData;
		acpl::pi::_string_utf8_skip_fwd(oPos, nCount);
		this->mStrBuf.Cut(0, (oPos - oData));
	}
	
	return *this;
}

acpl::String &acpl::String::CutLeft(const acpl::String::Pos &nAtPos, bool nExcl)
{
	const char *oPos = nAtPos.Utf8();
	if (_local_String_Gen_PreparePos(this->mStrBuf, nExcl, oPos) == true) // `this->mStrBuf.Length()` is checked implicitly in `_local_String_Gen_PreparePos()`
		this->mStrBuf.Cut(0, (oPos - this->mStrBuf.Data()));
	
	return *this;
}

acpl::String &acpl::String::CutRight(acpl::SizeT nCount)
{
	if (nCount != 0 && this->mStrBuf.Length() != 0)
	{
		char *oData = this->mStrBuf.Data();
		char *oPos = oData + this->mStrBuf.Length();
		acpl::pi::_string_utf8_skip_bwd(const_cast<const char *&>(oPos), oData, ++nCount); // `++nCount` because the pointer is positioned on the NULL terminator
		*oPos = '\0';
		this->mStrBuf.SetLength(oPos - oData + 1);
	}
	
	return *this;
}

acpl::String &acpl::String::CutRight(const acpl::String::Pos &nAtPos, bool nExcl)
{
	const char *oPos = nAtPos.Utf8();
	if (_local_String_Gen_PreparePos(this->mStrBuf, !nExcl, oPos) == true) // negated nExcl!! `this->mStrBuf.Length()` is checked implicitly in `_local_String_Gen_PreparePos()`
	{
		this->mStrBuf.SetLength(oPos - this->mStrBuf.Data() + 1);
		this->mStrBuf.Data()[this->mStrBuf.Length() - 1] = '\0';
	}
	
	return *this;
}

acpl::String &acpl::String::Trim()
{
	if (this->mStrBuf.Length() != 0)
	{
		char *oData = this->mStrBuf.Data();
		char *oPos = oData + this->mStrBuf.Length() - 1; // on NULL terminator
		
		
		// TrimRight
		
		while (oPos > oData)
		{
			acpl::pi::_string_utf8_prev_seq(const_cast<const char *&>(oPos));
			if (_local_String_Gen_IsWspace(oPos) != true)
				break;
		}
		
		if (oPos != oData)
		{
			acpl::pi::_string_utf8_next_seq(const_cast<const char *&>(oPos));
			if (*oPos != '\0')
			{
				*oPos = '\0';
				this->mStrBuf.SetLength(oPos - oData + 1);
			}
		}
		else
		{
			this->mStrBuf.Clear().Apnd("", 1);
			return *this;
		}
		
		
		// TrimLeft
		
		oPos = oData;
		
		while (*oPos != '\0')
		{
			if (_local_String_Gen_IsWspace(oPos) != true)
				break;
			acpl::pi::_string_utf8_next_seq(const_cast<const char *&>(oPos));
		}
		
		if (*oPos != '\0')
			this->mStrBuf.Cut(0, (oPos - oData));
		else
			this->mStrBuf.Clear().Apnd("", 1);
	}
	
	return *this;
}

acpl::String &acpl::String::TrimLeft()
{
	if (this->mStrBuf.Length() != 0)
	{
		char *oData = this->mStrBuf.Data();
		char *oPos = oData;
		
		while (*oPos != '\0')
		{
			if (_local_String_Gen_IsWspace(oPos) != true)
				break;
			acpl::pi::_string_utf8_next_seq(const_cast<const char *&>(oPos));
		}
		
		if (*oPos != '\0')
			this->mStrBuf.Cut(0, (oPos - oData));
		else
			this->mStrBuf.Clear().Apnd("", 1);
	}
	
	return *this;
}

acpl::String &acpl::String::TrimRight()
{
	if (this->mStrBuf.Length() != 0)
	{
		char *oData = this->mStrBuf.Data();
		char *oPos = oData + this->mStrBuf.Length() - 1; // on NULL terminator
		
		while (oPos > oData)
		{
			acpl::pi::_string_utf8_prev_seq(const_cast<const char *&>(oPos));
			if (_local_String_Gen_IsWspace(oPos) != true)
				break;
		}
		
		if (oPos != oData)
		{
			acpl::pi::_string_utf8_next_seq(const_cast<const char *&>(oPos));
			if (*oPos != '\0')
			{
				*oPos = '\0';
				this->mStrBuf.SetLength(oPos - oData + 1);
			}
		}
		else
			this->mStrBuf.Clear().Apnd("", 1);
	}
	
	return *this;
}


void acpl::String::ToUtf8(acpl::Mem::Block<char>::Type &nChars, bool nClearBfr) const
{
	if (nClearBfr == true)
		nChars.Clear();
	
	if (this->mStrBuf.Length() != 0)
		nChars.Apnd(this->mStrBuf.Data(), this->mStrBuf.Length());
	else
		nChars.Apnd("", 1);
}

void acpl::String::ToMbs(acpl::Mem::Block<char>::Type &nChars, bool nClearBfr) const
{
	if (nClearBfr == true)
		nChars.Clear();
	
	if (this->mStrBuf.Length() != 0)
	{
		acpl::cp::_string_mb_state_t oMbState;
		acpl::cp::_string_mb_state_init(oMbState);
		acpl::cp::_string_wchar_cont_t oWc;
		acpl::cp::_string_mbc_cont_t oMbc;
		acpl::Unichar oUc = 0;
		const char *oCur = this->mStrBuf.Data();
		
		while (*oCur != '\0')
		{
			acpl::SizeT oSeqSize = acpl::i18n::Charset_UTF_8::Decode(reinterpret_cast<const acpl::UInt8 *>(oCur), 6, oUc);
			acpl::cp::_string_wchar_seq_encode(oUc, oWc);
			acpl::SizeT oMbcSize = acpl::pi::_string_wchar_to_mbc(oMbc, oWc, oMbState);
			nChars.Apnd(oMbc, oMbcSize);
			oCur += oSeqSize;
		}
		
		acpl::cp::_string_mb_state_destroy(oMbState);
	}
	
	nChars.Apnd("", 1);
}

void acpl::String::ToWcs(acpl::Mem::Block<wchar_t>::Type &nChars, bool nClearBfr) const
{
	if (nClearBfr == true)
		nChars.Clear();
	
	if (this->mStrBuf.Length() != 0)
	{
		acpl::cp::_string_wchar_cont_t oWc;
		acpl::Unichar oUc = 0;
		const char *oCur = this->mStrBuf.Data();
		
		while (*oCur != '\0')
		{
			acpl::SizeT oSeqSize = acpl::i18n::Charset_UTF_8::Decode(reinterpret_cast<const acpl::UInt8 *>(oCur), 6, oUc);
			acpl::SizeT oWcSize = acpl::cp::_string_wchar_seq_encode(oUc, oWc);
			nChars.Apnd(oWc, oWcSize);
			oCur += oSeqSize;
		}
	}
	
	nChars.Apnd(L"", 1);
}

void acpl::String::ToUcs(acpl::Mem::Block<acpl::Unichar>::Type &nChars, bool nClearBfr) const
{
	if (nClearBfr == true)
		nChars.Clear();
	
	acpl::Unichar oUc;
	
	if (this->mStrBuf.Length() != 0)
	{
		const char *oCur = this->mStrBuf.Data();
		
		while (*oCur != '\0')
		{
			acpl::SizeT oSeqSize = acpl::i18n::Charset_UTF_8::Decode(reinterpret_cast<const acpl::UInt8 *>(oCur), 6, oUc);
			nChars.Apnd(&oUc, 1);
			oCur += oSeqSize;
		}
	}
	
	oUc = 0; // Adds NULL terminator
	nChars.Apnd(&oUc, 1);
}


// PRIVATE

acpl::UInt64 acpl::String::ToInt(acpl::UInt8 nBase, acpl::UInt8 nTypeInfo, bool nSwapDecMarks, bool &nSuccess)
{
	// Init
	nSuccess = false;
	
	// Validate base (valid bases are 2-36 inclusive, or special value 0).
	if (nBase == 1 || nBase > 36)
		return 0;
	
	const char *oStr = this->Utf8();
	bool oIsNeg = false; // Is the number negative
	const char oDigitGrp = ((nSwapDecMarks == true) ? '.' : ',');
	
	// Position oStr to the first non-whitespace character.
	for (; _local_String_Gen_IsWspace(oStr) == true; acpl::pi::_string_utf8_next_seq(oStr));
	
	if (*oStr == '\0') // The string contains no meaningful data
		return 0;
	
	
	// Determine whether a single '+' or '-' is present. If one of these signs
	// appears the number must be in decimal form (base == 10). If a '-' is
	// present, the tType must be a signed type.
	if (*oStr == '-')
	{
		if ((nTypeInfo & 0x80) == 0 || (nBase != 0 && nBase != 10))
			return 0;
		oIsNeg = true;
		nBase = 10;
		oStr++;
	}
	else
	if (*oStr == '+')
	{
		if (nBase != 0 && nBase != 10)
			return 0;
		nBase = 10;
		oStr++;
	}
	
	
	// Determine base if nBase was defined as 0. If string starts with a leading
	// "0x", the number is in hex (16) base. If string starts with a leading
	// '0', the number is in octal (8) base. If it starts with anything else it
	// is presumed to be a decimal (10) base.
	if (nBase == 0)
	{
		if (*oStr == '0')
		{
			if (*++oStr == 'x' || *oStr == 'X')
			{
				nBase = 16;
				oStr++;
			}
			else
				nBase = 8;
		}
		else
			nBase = 10;
	}
	else
	if (nBase == 16 && oStr[0] == '0' && (oStr[1] == 'x' || oStr[1] == 'X')) // If base is 16, the string may start with "0x"
		oStr += 2;
	
	
	if (*oStr == '\0') // The string contains no meaningful data
		return 0;
	
	
	// Needed vars and their initializations
	acpl::UInt64 oValue = 0;
	acpl::UInt64 oValueCur = 0;
	acpl::UInt64 oValueMax = 0;
	
	// Set initial max value
	switch (nTypeInfo & 0x7F)
	{
		case 1:  oValueMax = acpl::Const::UI64(0x00000000, 0x000000FF); break;
		case 2:  oValueMax = acpl::Const::UI64(0x00000000, 0x0000FFFF); break;
		case 4:  oValueMax = acpl::Const::UI64(0x00000000, 0xFFFFFFFF); break;
		default: oValueMax = acpl::Const::UI64(0xFFFFFFFF, 0xFFFFFFFF); break; // case for 8 (or, indeed, any other)
	}
	
	// Adjust max value for signed type in decimal base and possible negative value
	if (nBase == 10 && (nTypeInfo & 0x80) != 0)
	{
		oValueMax >>= 1;
		if (oIsNeg == true)
			oValueMax += 1;
	}
	
	
	// Process string; `oValueCur` is already initialized
	for (; *oStr != '\0'; oStr++)
	{
		if (acpl::pi::_string_ascii_char2num(*oStr, nBase, oValueCur) == true)
		{
			// Check the overflow
			if (oValueCur > oValueMax || (oValueCur / nBase) != oValue)
				break; // handled after loop
			
			oValue = oValueCur;
		}
		else
		if (*oStr != oDigitGrp || nBase != 10)
			break; // possible whitespaces or invalid char (certainly not a digit groupping in decimal base); handled after loop
	}
	
	// Invert value if it is negative
	if (oIsNeg == true)
		oValue *= -1;
	
	
	// Check for possible trailing whitespaces and NULL terminator
	for (; _local_String_Gen_IsWspace(oStr) == true; acpl::pi::_string_utf8_next_seq(oStr));
	
	if (*oStr == '\0')
		nSuccess = true;
	
	
	return oValue;
}

acpl::Float::Largest acpl::String::ToFloat(bool nSwapDecMarks, bool &nSuccess)
{
	// Init
	nSuccess = false;
	
	const char *oStr = this->Utf8();
	bool oIsNeg = false; // Is the number negative
	bool oIsExpNeg = false; // Is the exponent negative
	const char oDecPoint = ((nSwapDecMarks == true) ? ',' : '.');
	const char oDigitGrp = ((nSwapDecMarks == true) ? '.' : ',');
	
	// Position oStr to the first non-whitespace character.
	for (; _local_String_Gen_IsWspace(oStr) == true; acpl::pi::_string_utf8_next_seq(oStr));
	
	if (*oStr == '\0') // The string contains no meaningful data
		return 0;
	
	
	// Determine whether a single '+' or '-' is present.
	if (*oStr == '-')
	{
		oIsNeg = true;
		oStr++;
	}
	else
	if (*oStr == '+')
		oStr++;
	
	
	// Needed vars and their initializations
	acpl::Float::Largest oValue = 0.0;
	acpl::Float::Largest oDivisor = 1.0;
	acpl::UInt32 oExponent = 0;
	
	
	// Process string for whole part of the number; `oValue` is already initialized
	for (; *oStr != '\0'; oStr++)
	{
		if (acpl::pi::_string_ascii_char2num(*oStr, 10, oValue) == true);
		else
		if (*oStr == oDecPoint)
		{
			oStr++;
			break;
		}
		else
		if (*oStr != oDigitGrp)
			break; // possible 'e' or 'E' for exponent, whitespaces, or invalid char (certainly not a digit groupping in decimal base); handled after loop
	}
	
	// Process string for fraction part of the number (if it exists); `oValue` is already set and `oDivisor` initialized
	for (; *oStr != '\0'; oStr++)
	{
		if (acpl::pi::_string_ascii_char2num(*oStr, 10, oValue) == true)
			oDivisor *= 10;
		else
			break; // possible 'e' or 'E' for exponent, whitespaces, or invalid char; handled after loop
	}
	
	// Determine whether 'e' or 'E' for exponent and a single '+' or '-' is
	// present.
	if (*oStr == 'e' || *oStr == 'E') // we have the exponent part as well
	{
		oStr++;
		if (*oStr == '-')
		{
			oIsExpNeg = true;
			oStr++;
		}
		else
		if (*oStr == '+')
			oStr++;
	}
	
	// Process string for exponent part of the number (if it exists); `oValue` and `oDivisor` are already set
	for (; *oStr != '\0'; oStr++)
	{
		if (acpl::pi::_string_ascii_char2num(*oStr, 10, oExponent) == true)
		{
			if (oExponent >= 50000) // x86 extended precision (largest supported) format has a range of approximately 3.65e-4951 to 1.18e+4932
				oExponent /= 10;    // Reduce the exponent; (exponent > 5000) = infinity, (exponent < -5000) = out-of-scope small number
		}
		else
			break; // possible whitespaces or invalid char; handled after loop
	}
	
	// Limit exponent to 5000
	if (oExponent > 5000)
		oExponent = 5000;
	
	
	// Invert value if it is negative
	if (oIsNeg == true)
		oValue *= -1;
	
	
	// Check for possible trailing whitespaces and NULL terminator
	for (; _local_String_Gen_IsWspace(oStr) == true; acpl::pi::_string_utf8_next_seq(oStr));
	
	if (*oStr == '\0')
		nSuccess = true;
	
	
	// Recalculate `oValue` with `oDivisor`
	oValue /= oDivisor;
	
	// Recalculate `oValue` with `oExponent`
	if (oIsExpNeg == true)
		while (oExponent-- != 0) oValue /= 10.0;
	else
		while (oExponent-- != 0) oValue *= 10.0;
	
	
	return oValue;
}



//
// acpl::String::Ref
//

static inline void _local_String_Ref_Measure_Self(const char *nStr, acpl::SizeT nStrMaxLen, acpl::SizeT nStrOff, acpl::SizeT &nRefOff, acpl::SizeT &nEncByteSize)
{
	// Find the starting offset
	const char *oHead = nStr;
	acpl::pi::_string_utf8_skip_fwd(oHead, nStrOff);
	nRefOff = (oHead - nStr);
	if (*oHead == '\0')
		return;
	
	// Find the end
	const char *oTail = oHead;
	acpl::pi::_string_utf8_skip_fwd(oTail, nStrMaxLen);
	nEncByteSize = (oTail - oHead);
}

static inline bool _local_String_Ref_Measure_Utf8_DecSeqMainByte(acpl::UInt8 nByte, acpl::UInt8 &nExpSeqSize, acpl::Unichar &nUc) // Returns true if non-NULL byte
{
	if (nByte == 0x00) { return false; }
	else
	if (nByte <  0xC0) { nExpSeqSize = 1; }
	else
	if (nByte <  0xE0) { nExpSeqSize = 2; nUc = (nByte & 0x1F); }
	else
	if (nByte <  0xF0) { nExpSeqSize = 3; nUc = (nByte & 0x0F); }
	else
	if (nByte <  0xF8) { nExpSeqSize = 4; nUc = (nByte & 0x07); }
	else
	if (nByte <  0xFC) { nExpSeqSize = 5; nUc = (nByte & 0x03); }
	else
	if (nByte <  0xFE) { nExpSeqSize = 6; nUc = (nByte & 0x01); }
	else
	                   { nExpSeqSize = 1; }
	
	return true;
}
static inline bool _local_String_Ref_Measure_Utf8_DecSeqFollowBytes(const char *nPtr, acpl::UInt8 nExpSeqSize, acpl::Unichar &nUc)
{
	for (acpl::UInt8 i = 1; i < nExpSeqSize; i++)
		if (acpl::pi::_string_utf8_dec_follow_byte(nPtr[i], nUc) != true)
			return false;
	
	return true;
}
static inline void _local_String_Ref_Measure_Utf8(const char *nStr, acpl::SizeT nStrMaxLen, acpl::SizeT nStrOff, acpl::SizeT &nRefOff, acpl::SizeT &nEncByteSize)
{
	acpl::UInt8 oExpSeqSize;
	acpl::Unichar oUc;
	
	// Find the starting offset (nRefOff == 0)
	while (nStrOff--)
	{
		if (_local_String_Ref_Measure_Utf8_DecSeqMainByte(nStr[nRefOff], oExpSeqSize, oUc) != true)
			return;
		else
		if (oExpSeqSize == 1 || _local_String_Ref_Measure_Utf8_DecSeqFollowBytes(nStr + nRefOff, oExpSeqSize, oUc) != true)
			nRefOff++;
		else
		if (oUc != 0)
			nRefOff += oExpSeqSize;
		else
			return;
	}
	
	// Find the end  (nEncByteSize == 0)
	acpl::SizeT oTmpOff = nRefOff;
	while (nStrMaxLen--)
	{
		if (_local_String_Ref_Measure_Utf8_DecSeqMainByte(nStr[oTmpOff], oExpSeqSize, oUc) != true)
			break;
		else
		if (oExpSeqSize == 1 || _local_String_Ref_Measure_Utf8_DecSeqFollowBytes(nStr + oTmpOff, oExpSeqSize, oUc) != true)
		{
			oTmpOff++;
			nEncByteSize++;
		}
		else
		if (oUc != 0)
		{
			oTmpOff += oExpSeqSize;
			nEncByteSize += acpl::pi::_string_utf8_enc_size(oUc);
		}
		else
			break;
	}
}

static inline void _local_String_Ref_Measure_Mbs(const char *nStr, acpl::SizeT nStrMaxLen, acpl::SizeT nStrOff, acpl::SizeT &nRefOff, acpl::SizeT &nEncByteSize)
{
	acpl::cp::_string_wchar_cont_t oWcCont;
	acpl::cp::_string_mb_state_t oMbState;
	acpl::cp::_string_mb_state_init(oMbState);
	
	// Find the starting offset (nRefOff == 0)
	while (nStrOff--)
	{
		if (acpl::pi::_string_mbc_to_wchar(oWcCont, nStr, nRefOff, oMbState) != true) // nRefOff is appropriately increased by the function
		{
			acpl::cp::_string_mb_state_destroy(oMbState);
			return;
		}
	}
	
	// Find the end (nEncByteSize == 0)
	acpl::SizeT oTmpOff = nRefOff;
	while (nStrMaxLen--)
	{
		if (acpl::pi::_string_mbc_to_wchar(oWcCont, nStr, oTmpOff, oMbState) != true) // oTmpOff is appropriately increased by the function
			break;
		
		acpl::Unichar oUc;
		acpl::cp::_string_wchar_seq_decode(oWcCont, oUc);
		nEncByteSize += acpl::pi::_string_utf8_enc_size(oUc);
	}
	
	acpl::cp::_string_mb_state_destroy(oMbState);
}

static inline void _local_String_Ref_Measure_Wcs(const wchar_t *nStr, acpl::SizeT nStrMaxLen, acpl::SizeT nStrOff, acpl::SizeT &nRefOff, acpl::SizeT &nEncByteSize)
{
	// Find the starting offset (nRefOff == 0)
	while (nStrOff--)
	{
		if (nStr[nRefOff] == L'\0')
			return;
		
		nRefOff += acpl::cp::_string_wchar_seq_size(nStr + nRefOff);
	}
	
	// Find the end (nEncByteSize == 0)
	acpl::SizeT oTmpOff = nRefOff;
	while (nStrMaxLen--)
	{
		if (nStr[oTmpOff] == L'\0')
			break;
		
		acpl::Unichar oUc;
		oTmpOff += acpl::cp::_string_wchar_seq_decode(nStr + oTmpOff, oUc);
		nEncByteSize += acpl::pi::_string_utf8_enc_size(oUc);
	}
}

static inline void _local_String_Ref_Measure_Ucs(const acpl::Unichar *nStr, acpl::SizeT nStrMaxLen, acpl::SizeT nStrOff, acpl::SizeT &nRefOff, acpl::SizeT &nEncByteSize)
{
	// Find the starting offset (nRefOff == 0)
	while (nStrOff--)
	{
		if (nStr[nRefOff] == '\0')
			return;
		
		nRefOff++;
	}
	
	// Find the end (nEncByteSize == 0)
	acpl::SizeT oTmpOff = nRefOff;
	while (nStrMaxLen--)
	{
		if (nStr[oTmpOff] == '\0')
			break;
		
		nEncByteSize += acpl::pi::_string_utf8_enc_size(nStr[oTmpOff++]);
	}
}

void acpl::String::Ref::Measure(acpl::SizeT nStrMaxLen, acpl::SizeT nStrOff, acpl::SizeT &nRefOff, acpl::SizeT &nEncByteSize) const
{
	nRefOff = 0;
	nEncByteSize = 0;
	
	if (this->IsNull() != true)
	{
		switch (this->GetType())
		{
			case acpl::String::Ref::rtNone:
				break;
			case acpl::String::Ref::rtSelf:
				return _local_String_Ref_Measure_Self(static_cast<const char *>(this->GetPtr()), nStrMaxLen, nStrOff, nRefOff, nEncByteSize);
			case acpl::String::Ref::rtUtf8:
				return _local_String_Ref_Measure_Utf8(static_cast<const char *>(this->GetPtr()), nStrMaxLen, nStrOff, nRefOff, nEncByteSize);
			case acpl::String::Ref::rtMbs:
				return _local_String_Ref_Measure_Mbs(static_cast<const char *>(this->GetPtr()), nStrMaxLen, nStrOff, nRefOff, nEncByteSize);
			case acpl::String::Ref::rtWcs:
				return _local_String_Ref_Measure_Wcs(static_cast<const wchar_t *>(this->GetPtr()), nStrMaxLen, nStrOff, nRefOff, nEncByteSize);
			case acpl::String::Ref::rtUcs:
				return _local_String_Ref_Measure_Ucs(static_cast<const acpl::Unichar *>(this->GetPtr()), nStrMaxLen, nStrOff, nRefOff, nEncByteSize);
		}
	}
}

static inline void _local_String_Ref_Convert_Self(const char *nStr, acpl::SizeT nRefOff, acpl::SizeT nEncByteSize, char *nDst)
{
	acpl::Mem::BaseCopy(nDst, nStr + nRefOff, nEncByteSize);
}

static inline bool _local_String_Ref_Convert_Utf8_DecSeqFollowBytes(const char *nPtr, acpl::UInt8 nExpSeqSize, acpl::Unichar &nUc)
{
	for (acpl::UInt8 i = 1; i < nExpSeqSize; i++)
	{
		if (acpl::pi::_string_utf8_dec_follow_byte(nPtr[i], nUc) != true)
		{
			nUc = '?';
			return false;
		}
	}
	return true;
}
static inline void _local_String_Ref_Convert_Utf8(const char *nStr, acpl::SizeT nRefOff, acpl::SizeT nEncByteSize, char *nDst)
{
	nStr += nRefOff;
	const char *oDstEnd = nDst + nEncByteSize;
	
	while (nDst < oDstEnd)
	{
		acpl::UInt8 oFirstByte = *nStr;
		acpl::UInt8 oExpSeqSize = 1; // expected sequence size (first byte + follow bytes) (at least 1 byte)
		acpl::Unichar oUc;
		
		if (oFirstByte < 0x80) { oUc = oFirstByte; }
		else
		if (oFirstByte < 0xC0) { oUc = '?'; }
		else
		if (oFirstByte < 0xE0) { oExpSeqSize = 2; oUc = (oFirstByte & 0x1F); }
		else
		if (oFirstByte < 0xF0) { oExpSeqSize = 3; oUc = (oFirstByte & 0x0F); }
		else
		if (oFirstByte < 0xF8) { oExpSeqSize = 4; oUc = (oFirstByte & 0x07); }
		else
		if (oFirstByte < 0xFC) { oExpSeqSize = 5; oUc = (oFirstByte & 0x03); }
		else
		if (oFirstByte < 0xFE) { oExpSeqSize = 6; oUc = (oFirstByte & 0x01); }
		else
		                       { oUc = '?'; }
		
		if (oExpSeqSize == 1 || _local_String_Ref_Convert_Utf8_DecSeqFollowBytes(nStr, oExpSeqSize, oUc) != true)
		{
			nStr++;
			*nDst++ = static_cast<char>(oUc);
		}
		else
		{
			nStr += oExpSeqSize;
			nDst += acpl::i18n::Charset_UTF_8::Encode(oUc, reinterpret_cast<acpl::UInt8 *>(nDst), 6);
		}
	}
}

static inline void _local_String_Ref_Convert_Mbs(const char *nStr, acpl::SizeT nRefOff, acpl::SizeT nEncByteSize, char *nDst)
{
	acpl::cp::_string_wchar_cont_t oWcCont;
	acpl::cp::_string_mb_state_t oMbState;
	acpl::cp::_string_mb_state_init(oMbState);
	
	const char *oDstEnd = nDst + nEncByteSize;
	
	while (nDst < oDstEnd)
	{
		acpl::pi::_string_mbc_to_wchar(oWcCont, nStr, nRefOff, oMbState); // nRefOff is appropriately increased by the function
		
		acpl::Unichar oUc;
		acpl::cp::_string_wchar_seq_decode(oWcCont, oUc);
		
		acpl::SizeT oEncSize = acpl::i18n::Charset_UTF_8::Encode(oUc, reinterpret_cast<acpl::UInt8 *>(nDst), 6);
		
		if (oEncSize != 0)
			nDst += oEncSize;
		else
			*nDst++ = '?';
	}
	
	acpl::cp::_string_mb_state_destroy(oMbState);
}

static inline void _local_String_Ref_Convert_Wcs(const wchar_t *nStr, acpl::SizeT nRefOff, acpl::SizeT nEncByteSize, char *nDst)
{
	const char *oEndPtr = nDst + nEncByteSize;
	
	while (nDst < oEndPtr)
	{
		acpl::Unichar oUc;
		nRefOff += acpl::cp::_string_wchar_seq_decode(nStr + nRefOff, oUc);
		
		acpl::SizeT oEncSize = acpl::i18n::Charset_UTF_8::Encode(oUc, reinterpret_cast<acpl::UInt8 *>(nDst), 6);
		
		if (oEncSize != 0)
			nDst += oEncSize;
		else
			*nDst++ = '?';
	}
}

static inline void _local_String_Ref_Convert_Ucs(const acpl::Unichar *nStr, acpl::SizeT nRefOff, acpl::SizeT nEncByteSize, char *nDst)
{
	const char *oEndPtr = nDst + nEncByteSize;
	
	while (nDst < oEndPtr)
	{
		acpl::SizeT oEncSize = acpl::i18n::Charset_UTF_8::Encode(nStr[nRefOff++], reinterpret_cast<acpl::UInt8 *>(nDst), 6);
		
		if (oEncSize != 0)
			nDst += oEncSize;
		else
			*nDst++ = '?';
	}
}

void acpl::String::Ref::Convert(acpl::SizeT nRefOff, acpl::SizeT nEncByteSize, char *nDst) const
{
	if (this->IsNull() != true)
	{
		switch (this->GetType())
		{
			case acpl::String::Ref::rtNone:
				break;
			case acpl::String::Ref::rtSelf:
				return _local_String_Ref_Convert_Self(static_cast<const char *>(this->GetPtr()), nRefOff, nEncByteSize, nDst);
			case acpl::String::Ref::rtUtf8:
				return _local_String_Ref_Convert_Utf8(static_cast<const char *>(this->GetPtr()), nRefOff, nEncByteSize, nDst);
			case acpl::String::Ref::rtMbs:
				return _local_String_Ref_Convert_Mbs(static_cast<const char *>(this->GetPtr()), nRefOff, nEncByteSize, nDst);
			case acpl::String::Ref::rtWcs:
				return _local_String_Ref_Convert_Wcs(static_cast<const wchar_t *>(this->GetPtr()), nRefOff, nEncByteSize, nDst);
			case acpl::String::Ref::rtUcs:
				return _local_String_Ref_Convert_Ucs(static_cast<const acpl::Unichar *>(this->GetPtr()), nRefOff, nEncByteSize, nDst);
		}
	}
}



// 
// acpl::String::SelfRefBase
// 



// 
// acpl::String::Pos
// 

// PUBLIC

static inline bool _local_String_Pos_DecSeq(const void *nPtr, acpl::Unichar &nUnichar)
{
	nUnichar = '\0';
	return (acpl::i18n::Charset_UTF_8::Decode(static_cast<const acpl::UInt8 *>(nPtr), 6, nUnichar) > 0);
}

static inline bool _local_String_Pos_Substr_Cmp(const char *nStr, const char *nSubStr)
{
	for (; *nStr == *nSubStr && *nSubStr != '\0'; nStr++, nSubStr++) ;
	return (*nSubStr == '\0');
}

static inline bool _local_String_Pos_Substr_Next(const acpl::String *nString, acpl::SizeT nFromOff, const char *nSubStr, acpl::SizeT &nOutOff)
{
	if (nString != NULL && nString->ByteLength() > 0)
	{
		const char *oBegPtr = nString->Utf8();
		const char *oCurPtr = oBegPtr + nFromOff;
		
		while (*oCurPtr != '\0')
		{
			if (_local_String_Pos_Substr_Cmp(oCurPtr, nSubStr) == true)
			{
				nOutOff = oCurPtr - oBegPtr;
				return true;
			}
			oCurPtr++;
		}
	}
	
	return false;
}

static inline bool _local_String_Pos_Substr_Prev(const acpl::String *nString, acpl::SizeT nFromOff, const char *nSubStr, acpl::SizeT &nOutOff)
{
	if (nString != NULL && nString->ByteLength() > 0)
	{
		const char *oBegPtr = nString->Utf8();
		const char *oCurPtr = oBegPtr + nFromOff;
		
		while (oCurPtr >= oBegPtr)
		{
			if (_local_String_Pos_Substr_Cmp(oCurPtr, nSubStr) == true)
			{
				nOutOff = oCurPtr - oBegPtr;
				return true;
			}
			oCurPtr--;
		}
	}
	
	return false;
}


acpl::Unichar acpl::String::Pos::GetUnichar() const
{
	if (this->IsInBounds() == true)
	{
		acpl::Unichar oUc;
		_local_String_Pos_DecSeq((this->mString->Utf8() + this->mTgt.uOff), oUc);
		return oUc;
	}
	else
		return 0;
}

bool acpl::String::Pos::GetUnichar(acpl::Unichar &nChar) const
{
	if (this->IsInBounds() == true)
		return _local_String_Pos_DecSeq((this->mString->Utf8() + this->mTgt.uOff), nChar);
	else
		return 0;
}


bool acpl::String::Pos::IsFirst() const
{
	return (this->mString != NULL && this->mString->ByteLength() > 0 && this->mTgt.uOff == 0);
}

bool acpl::String::Pos::IsLast() const
{
	if (this->IsInBounds() == true)
	{
		const char *oPtr = this->mString->Utf8() + this->mTgt.uOff;
		acpl::pi::_string_utf8_next_seq(oPtr);
		return (*oPtr == '\0');
	}
	
	return false;
}


bool acpl::String::Pos::First()
{
	if (this->mString != NULL && this->mString->ByteLength() > 0)
	{
		this->mTgt.uOff = 0;
		return true;
	}
	else
		return false;
}

bool acpl::String::Pos::First(acpl::Unichar nUc)
{
	char oEncChar[7];
	oEncChar[acpl::i18n::Charset_UTF_8::Encode(nUc, reinterpret_cast<acpl::UInt8 *>(oEncChar), 6)] = '\0';
	return _local_String_Pos_Substr_Next(this->mString, 0, oEncChar, this->mTgt.uOff);
}

bool acpl::String::Pos::First(const acpl::String::Ref &nSubStr)
{
	_local_String_Gen_MakeRefString(oStr, nSubStr)
	return _local_String_Pos_Substr_Next(this->mString, 0, oStr, this->mTgt.uOff);
}

bool acpl::String::Pos::Last()
{
	if (this->mString != NULL && this->mString->ByteLength() > 0)
	{
		const char *oBegPtr = this->mString->Utf8();
		const char *oCurPtr = oBegPtr + this->mString->ByteLength(); // position pointer at NULL terminator
		acpl::pi::_string_utf8_prev_seq(oCurPtr);
		this->mTgt.uOff = oCurPtr - oBegPtr;
		return true;
	}
	else
		return false;
}

bool acpl::String::Pos::Last(acpl::Unichar nUc)
{
	char oEncChar[7];
	oEncChar[acpl::i18n::Charset_UTF_8::Encode(nUc, reinterpret_cast<acpl::UInt8 *>(oEncChar), 6)] = '\0';
	return _local_String_Pos_Substr_Prev(this->mString, this->mString->ByteLength(), oEncChar, this->mTgt.uOff);
}

bool acpl::String::Pos::Last(const acpl::String::Ref &nSubStr)
{
	_local_String_Gen_MakeRefString(oStr, nSubStr)
	return _local_String_Pos_Substr_Prev(this->mString, this->mString->ByteLength(), oStr, this->mTgt.uOff);
}

bool acpl::String::Pos::Next()
{
	if (this->IsInBounds() == true)
	{
		const char *oBegPtr = this->mString->Utf8();
		const char *oCurPtr = oBegPtr + this->mTgt.uOff;
		acpl::pi::_string_utf8_next_seq(oCurPtr);
		if (*oCurPtr != '\0')
		{
			this->mTgt.uOff = oCurPtr - oBegPtr;
			return true;
		}
	}
	
	return false;
}

bool acpl::String::Pos::Next(acpl::Unichar nUc)
{
	char oEncChar[7];
	oEncChar[acpl::i18n::Charset_UTF_8::Encode(nUc, reinterpret_cast<acpl::UInt8 *>(oEncChar), 6)] = '\0';
	return _local_String_Pos_Substr_Next(this->mString, this->mTgt.uOff + 1, oEncChar, this->mTgt.uOff);
}

bool acpl::String::Pos::Next(const acpl::String::Ref &nSubStr)
{
	_local_String_Gen_MakeRefString(oStr, nSubStr)
	return _local_String_Pos_Substr_Next(this->mString, this->mTgt.uOff + 1, oStr, this->mTgt.uOff);
}

bool acpl::String::Pos::Prev()
{
	if (this->mString != NULL && this->mTgt.uOff > 0 && this->mString->ByteLength() > this->mTgt.uOff)
	{
		const char *oBegPtr = this->mString->Utf8();
		const char *oCurPtr = oBegPtr + this->mTgt.uOff;
		acpl::pi::_string_utf8_prev_seq(oCurPtr);
		this->mTgt.uOff = oCurPtr - oBegPtr;
		return true;
	}
	
	return false;
}

bool acpl::String::Pos::Prev(acpl::Unichar nUc)
{
	char oEncChar[7];
	oEncChar[acpl::i18n::Charset_UTF_8::Encode(nUc, reinterpret_cast<acpl::UInt8 *>(oEncChar), 6)] = '\0';
	return _local_String_Pos_Substr_Prev(this->mString, this->mTgt.uOff - 1, oEncChar, this->mTgt.uOff);
}

bool acpl::String::Pos::Prev(const acpl::String::Ref &nSubStr)
{
	_local_String_Gen_MakeRefString(oStr, nSubStr)
	return _local_String_Pos_Substr_Prev(this->mString, this->mTgt.uOff - 1, oStr, this->mTgt.uOff);
}


static inline void _local_String_Pos_Forth(const char *nBegPtr, acpl::SizeT nInitOff, acpl::SizeT nCount, acpl::SizeT &nOff)
{
	const char *oCurPtr = nBegPtr + nInitOff;
	
	acpl::pi::_string_utf8_skip_fwd(oCurPtr, nCount);
	
	if (*oCurPtr == '\0')
		acpl::pi::_string_utf8_prev_seq(oCurPtr);
	
	nOff = oCurPtr - nBegPtr;
}

bool acpl::String::Pos::Forth(acpl::SizeT nCount)
{
	if (this->IsInBounds() == true)
	{
		_local_String_Pos_Forth(this->mString->Utf8(), this->mTgt.uOff, nCount, this->mTgt.uOff);
		return true;
	}
	
	return false;
}

static inline void _local_String_Pos_Back(const char *nBegPtr, acpl::SizeT nInitOff, acpl::SizeT nCount, acpl::SizeT &nOff)
{
	const char *oCurPtr = nBegPtr + nInitOff;
	
	acpl::pi::_string_utf8_skip_bwd(oCurPtr, nBegPtr, nCount);
	
	nOff = oCurPtr - nBegPtr;
}

bool acpl::String::Pos::Back(acpl::SizeT nCount)
{
	if (this->mString != NULL && this->mTgt.uOff > 0 && this->mString->ByteLength() > this->mTgt.uOff)
	{
		_local_String_Pos_Back(this->mString->Utf8(), this->mTgt.uOff, nCount, this->mTgt.uOff);
		return true;
	}
	
	return false;
}

bool acpl::String::Pos::SkipFirst(acpl::SizeT nCount)
{
	if (this->mString != NULL && this->mString->ByteLength() > 0)
	{
		_local_String_Pos_Forth(this->mString->Utf8(), 0, nCount, this->mTgt.uOff);
		return true;
	}
	else
		return false;
}

bool acpl::String::Pos::SkipLast(acpl::SizeT nCount)
{
	if (this->mString != NULL && this->mString->ByteLength() > 0)
	{
		_local_String_Pos_Back(this->mString->Utf8(), this->mString->ByteLength(), nCount + 1, this->mTgt.uOff); // `nCount + 1` because the pointer is positioned on the NULL terminator
		return true;
	}
	else
		return false;
}



// 
// acpl::String::PermRef
// 



// 
// acpl::String::AllocPermRef
// 

void acpl::String::AllocPermRef::Alloc(const acpl::String::Ref &nRef, acpl::SizeT nMaxCount, acpl::SizeT nStartOffset)
{
	acpl::Mem::Free(this->mAlloced);
	_local_String_Gen_AllocRefString(this->mAlloced, nRef, nMaxCount, nStartOffset);
}




// 
// acpl::String::IntRef
// 

// PRIVATE

// Description of `nBaseAgg`:
//   * (1) upper_case flag
//   * (7) base
// 
// Description of `nPaddingAgg`:
//   * (8) padding_count
//   * (8) padding_char

void acpl::String::IntRef::Conv(const acpl::String::IntRef::Type &nArg, acpl::UInt8 nBaseAgg, acpl::UInt16 nPaddingAgg, const char *nPrepand)
{
	// this->mTgt.uPtr will be set to oCurPos value at the end of the function
	
	bool oUppercase = ((nBaseAgg & 0x80) != 0);
	nBaseAgg &= 0x7F;
	
	char *oCurPos = this->mStr + 64;
	*oCurPos = '\0';
	
	
	if (nBaseAgg >= 2 && nBaseAgg <= 36)
	{
		// Conversion
		
		acpl::UInt64 oValue = nArg.GetValue();
		bool oAddNegSign = (nBaseAgg == 10 && (nArg.GetMeta() & 0x80) != 0);
		
		
		if (oAddNegSign == true)
			oValue *= -1;
		else
		{
			switch (nArg.GetMeta() & 0x7F)
			{
				case 1: oValue &= 0x000000FF; break;
				case 2: oValue &= 0x0000FFFF; break;
				case 4: oValue &= 0xFFFFFFFF; break;
				// no case needed for 8 (or, indeed, any other)
			}
		}
		
		
		acpl::pi::_string_ascii_num2char(oValue, nBaseAgg, oUppercase, oCurPos);
		
		if (oAddNegSign == true)
			*--oCurPos = '-';
		
		
		// Padding
		
		acpl::UInt8 oTmpLen = (this->mStr + (sizeof(this->mStr) - 1) - oCurPos); // calculate current length
		acpl::UInt8 oPadding = (nPaddingAgg >> 8);
		
		if (oPadding > oTmpLen)
		{
			oPadding -= oTmpLen;
			nPaddingAgg &= 0xFF;
			
			if (nPaddingAgg < 0x20 || nPaddingAgg > 0x7E)
				nPaddingAgg = ' ';
			
			if (nPaddingAgg == '0' && oAddNegSign == true)
				oCurPos++;
			
			while (oPadding > 0 && oCurPos > this->mStr)
			{
				*--oCurPos = static_cast<char>(nPaddingAgg);
				oPadding--;
			}
			
			if (nPaddingAgg == '0' && oAddNegSign == true)
				*--oCurPos = '-';
		}
		
		
		// Prepand
		
		if (nPrepand != NULL && oCurPos > this->mStr)
		{
			for (oTmpLen = 0; nPrepand[oTmpLen] != '\0'; oTmpLen++) ; // calculate `nPrepand` string length
			
			while (oTmpLen-- > 0 && oCurPos > this->mStr)
				*--oCurPos = nPrepand[oTmpLen];
		}
	}
	else
	{
		oCurPos -= 10;
		acpl::Mem::Copy(oCurPos, "(inv-base)", 10);
	}
	
	this->mTgt.uPtr = oCurPos;
}



// 
// acpl::String::PtrRef
// 



// 
// acpl::String::FloatRef
// 

void acpl::String::FloatRef::Conv(acpl::Float::Largest nArg, bool nSwapDecMarks)
{
	const char oDecMark = ((nSwapDecMarks == true) ? ',' : '.');
	acpl::Float::Largest oVal = nArg;
	char *oCurPos = this->mStr;
	
	// Absolutize the value
	if (acpl::Float::IsNeg(oVal) == true)
	{
		oVal *= -1.0;
		*oCurPos++ = '-';
	}
	else
		*oCurPos++ = '+';
	
	
	if (oVal == 0.0)
	{
		*oCurPos++ = '0';
	}
	else
	if (oVal == acpl::Float::Inf<acpl::Float::Largest>())
	{
		*oCurPos++ = 'i';
		*oCurPos++ = 'n';
		*oCurPos++ = 'f';
	}
	else
	if (acpl::Float::IsNaN(oVal) == true)
	{
		*oCurPos++ = 'n';
		*oCurPos++ = 'a';
		*oCurPos++ = 'n';
	}
	else
	{
		// Decimal exponent
		acpl::SInt16 oExp = 0;
		
		// Rounding threshold weight
		acpl::Float::Largest oRndThr = 0.0000000000001L;
		if (acpl::Float::HasExtPrecRT() == true)
			oRndThr /= 100.0; // can be even smaller for extended precision
		
		// Make `1.0 <= oVal < 10.0` with a corresponding exponent
		while (oVal >= 10.0)
		{
			oVal /= 10.0;
			oExp++;
		}
		while (oVal < 1.0)
		{
			oVal *= 10.0;
			oExp--;
		}
		
		// Adjust rounding threshold weight for near-edge of denormal fraction
		// values due to garbage creep-in because of the divisions of `oVal`.
		// Second to last denormal fraction will be slightly skewed and the last
		// denormal fraction can be skewed quite a lot.
		for (acpl::SInt16 i = ((acpl::Float::HasExtPrecRT() == true) ? -4936 : -310);
			i >= oExp && oRndThr < ((acpl::Float::HasExtPrecRT() == true) ? 0.1L : 0.01L); i--)
			oRndThr *= 10.0;
		
		for (acpl::UInt8 i = 0; i < 22; i++) // will not reach 22, but just in case
		{
			acpl::UInt8 oCurDigit = static_cast<acpl::UInt8>(oVal);
			
			acpl::Float::Largest oValFloor = static_cast<acpl::Float::Largest>(oCurDigit);
			acpl::Float::Largest oValCeil = static_cast<acpl::Float::Largest>(oCurDigit + 1);
			
			if (i == 1)
				*oCurPos++ = oDecMark;
			
			if ((oVal - oValFloor) < oRndThr)
			{
				*oCurPos++ = static_cast<acpl::UInt8>(oValFloor) + '0';
				break;
			}
			else
			if ((oValCeil - oVal) < oRndThr)
			{
				if (oValCeil < 10.0)
					*oCurPos++ = static_cast<acpl::UInt8>(oValCeil) + '0';
				else
				{
					*oCurPos++ = static_cast<acpl::UInt8>(oValCeil / 10.0) + '0';
					oExp++;
				}
				break;
			}
			else
				*oCurPos++ = oCurDigit + '0';
			
			oVal -= oValFloor;
			oVal *= 10;
			oRndThr *= 10;
		}
		
		if (oExp != 0)
		{
			*oCurPos++ = 'e';
			if (oExp < 0)
			{
				*oCurPos++ = '-';
				oExp *= -1;
			}
			
			for (acpl::SInt16 i = 10000; i != 0; i /= 10)
			{
				if ((oExp % i) >= oExp && acpl::pi::_string_ascii_is_digit(oCurPos[-1]) != true)
					continue;
				*oCurPos++ = (oExp / i) + '0';
				oExp %= i;
			}
		}
	}
	
	*oCurPos++ = '\0';
	
	this->mTgt.uPtr = this->mStr + ((this->mStr[0] == '-') ? 0 : 1);
}



// 
// acpl::String::IpAddrRef
// 

static inline void _local_String_IpAddrRef_Ipv42Str(const acpl::UInt8 *nBytes, const acpl::UInt8 *&nCurByte, char *&nCurPos)
{
	while (nCurByte > nBytes)
	{
		nCurByte--;
		acpl::UInt8 oCurByte = *nCurByte;
		acpl::pi::_string_ascii_num2char(oCurByte, 10, true, nCurPos);
		
		if (nCurByte > nBytes)
			*--nCurPos = '.';
	}
}

acpl::String::IpAddrRef::IpAddrRef(const acpl::Ipv4Addr &nIpv4Addr)
{
	char *oCurPos = this->mStr + 45;
	*oCurPos = '\0'; // set NULL terminator
	
	const acpl::UInt8 *oCurByte = nIpv4Addr.Data() + 4; // end byte
	_local_String_IpAddrRef_Ipv42Str(nIpv4Addr.Data(), oCurByte, oCurPos);
		
	this->mTgt.uPtr = oCurPos;
}

static inline void _local_String_IpAddrRef_Ipv62Str(const acpl::UInt8 *nBytes, const acpl::UInt8 *&nCurByte, bool /*nOptimize*/, bool nIpv4Mode, char *&nCurPos)
{
	if (nIpv4Mode == true)
	{
		_local_String_IpAddrRef_Ipv42Str((nBytes + 12), nCurByte, nCurPos);
		*--nCurPos = ':';
	}
	
	// TODO: `nOptimize` optimizations
	
	while (nCurByte > nBytes)
	{
		nCurByte -= 2;
		acpl::UInt16 oCurGrp = (static_cast<acpl::UInt16>(nCurByte[0]) << 8) | static_cast<acpl::UInt16>(nCurByte[1]);
		acpl::pi::_string_ascii_num2char(oCurGrp, 16, true, nCurPos);
		
		if (nCurByte > nBytes)
			*--nCurPos = ':';
	}
}

acpl::String::IpAddrRef::IpAddrRef(const acpl::Ipv6Addr &nIpv6Addr, bool nOptimize, bool nIpv4Mode)
{
	char *oCurPos = this->mStr + 45;
	*oCurPos = '\0'; // set NULL terminator
	
	const acpl::UInt8 *oCurByte = nIpv6Addr.Data() + 16; // end byte
	_local_String_IpAddrRef_Ipv62Str(nIpv6Addr.Data(), oCurByte, nOptimize, nIpv4Mode, oCurPos);
	
	this->mTgt.uPtr = oCurPos;
}



// 
// acpl::String::NetAddrRef
// 

acpl::String::NetAddrRef::NetAddrRef(const acpl::NetworkAddr &nNetAddr)
{
	acpl::String oStr;
	nNetAddr.ToStr(oStr);
	this->Alloc(oStr.Utf8());
}



//
// acpl::String::RefList
//

acpl::String::RefList &acpl::String::RefList::String(const acpl::String::Ref &nArg, acpl::SizeT nMaxCount, acpl::SizeT nStartOffset)
{
	if (nArg.IsNull() != true)
		this->mList.Add(new acpl::String::StringRef(nArg, nMaxCount, nStartOffset));
	else
		this->mList.Add(new acpl::String::StringRef("(null)"));
	
	return *this;
}



// 
// acpl::String::FmtBase
// 

void acpl::String::FmtRefBase::SetFmt(const acpl::String::Ref &nRef)
{
	acpl::Mem::Free(this->mFmt);
	_local_String_Gen_AllocRefString(this->mFmt, nRef);
}



// 
// acpl::String::FmtRef
// 

void acpl::String::FmtRef::Format(const acpl::String::RefList &nRefList)
{
	const char *oCurPos = ((this->FmtUtf8() != NULL) ? this->FmtUtf8() : "");
	const char *oArgStart = NULL; // start of the possible current argument
	acpl::SizeT oArgOffset = 0;
	acpl::Mem::Block<char>::Type oTmpBlock;
	
	for (; *oCurPos != '\0'; oCurPos++)
	{
		if (oArgStart != NULL) // (possible) argument start
		{
			if (oArgStart[1] <= '9' && oArgStart[1] >= '0') // "\$[0-9]" signature
				oArgOffset = oArgStart[1] - '0';
			else
			if (oArgStart[1] == '{') // "\$\{[0-9]+\}" signature
			{
				if (oCurPos[0] <= '9' && oCurPos[0] >= '0')
				{
					oArgOffset *= 10;
					oArgOffset += oCurPos[0] - '0';
					continue;
				}
				else
				if (oCurPos[0] == '}'); // do not `continue` !!!
				else // non-numberic char appeared; cancel everything
				{
					oTmpBlock.Apnd(oArgStart, (oCurPos - oArgStart + 1));
					oArgStart = NULL;
					continue;
				}
			}
			
			
			// was a valid argument signature
			const acpl::String::Ref *oRef = ((oArgOffset < nRefList.List().Count()) ? nRefList.List().Get(oArgOffset) : NULL);
			if (oRef != NULL)
			{
				acpl::SizeT oRefOff = 0;
				acpl::SizeT oEncByteSize = 0;
				acpl::SizeT oCurLen = oTmpBlock.Length();
				
				oRef->Measure(acpl::Num<acpl::SizeT>::Max(), 0, oRefOff, oEncByteSize);
				if (oTmpBlock.SetLength(oCurLen + oEncByteSize) == true)
					oRef->Convert(oRefOff, oEncByteSize, oTmpBlock.Data() + oCurLen);
			}
			else
				oTmpBlock.Apnd("(no-arg)", 8); // without NULL terminator !!!
			
			oArgStart = NULL;
		}
		else
		if (oCurPos[0] == '$')
		{
			oArgOffset = 0;
			
			if (oCurPos[1] <= '9' && oCurPos[1] >= '0') // starts with "\$[0-9]"
				oArgStart = oCurPos;
			else
			if (oCurPos[1] == '{' && oCurPos[2] != '}') // starts with "\$\{" but is not "\$\{\}"
				oArgStart = oCurPos++; // so that `oCurPos` will be positioned at the number part of the argument
			else
			if (oCurPos[1] != '\0')
				oTmpBlock.Apnd(oCurPos++, 2); // append the '$' sign and the following char
			else
				oTmpBlock.Apnd(oCurPos, 1); // append the '$' sign but not the NULL terminator
		}
		else
		if (oCurPos[0] == '\\') // escaped
		{
			if (oCurPos[1] != '\0')
				oTmpBlock.Apnd(++oCurPos, 1); // append the char without the '\\'
		}
		else
			oTmpBlock.Apnd(oCurPos, 1);
	}
	
	
	if (oArgStart != NULL)
		oTmpBlock.Apnd(oArgStart, (oCurPos - oArgStart));
	
	oTmpBlock.Apnd("", 1);
	
	this->Alloc(oTmpBlock.Data());
}



// 
// acpl::String::FmtDtRef
// 

static void _local_String_FmtDtRef_ApndInt(const acpl::String::IntRef::Type &nArg, acpl::UInt8 nPadding, acpl::Mem::Block<char>::Type &nBlock)
{
	acpl::String::IntRef oIr(nArg, 10, false, nPadding, '0');
	acpl::SizeT oRefOff = 0;
	acpl::SizeT oEncByteSize = 0;
	acpl::SizeT oBlockLen = nBlock.Length();
	
	oIr.Measure(acpl::Num<acpl::SizeT>::Max(), 0, oRefOff, oEncByteSize);
	if (nBlock.SetLength(oBlockLen + oEncByteSize) == true)
		oIr.Convert(oRefOff, oEncByteSize, nBlock.Data() + oBlockLen);
}

void acpl::String::FmtDtRef::Format(const acpl::DateTime &nDateTime)
{
	const char *oCurPos = ((this->FmtUtf8() != NULL) ? this->FmtUtf8() : "");
// 	const char *oArgStart = NULL; // start of the possible current argument
// 	acpl::SizeT oArgOffset = 0;
	acpl::Mem::Block<char>::Type oTmpBlock;
	
	for (; *oCurPos != '\0'; oCurPos++)
	{
		if (oCurPos[0] == '%')
		{
			switch (oCurPos[1])
			{
				case 'C': // "%C" = The century number (year/100) as a 2-digit integer.
					_local_String_FmtDtRef_ApndInt((nDateTime.GetYear() % 10000) / 100, 2, oTmpBlock);
					break;
				
				case 'd': // "%d" = The day of the month as a decimal number (range 01 to 31).
					_local_String_FmtDtRef_ApndInt(nDateTime.GetDay(), 2, oTmpBlock);
					break;
				
				case 'F': // "%F" = Equivalent to "%Y-%m-%d" (the ISO 8601 date format).
					_local_String_FmtDtRef_ApndInt(nDateTime.GetYear() % 10000, 4, oTmpBlock);
					oTmpBlock.Apnd("-", 1);
					_local_String_FmtDtRef_ApndInt(nDateTime.GetMonth(), 2, oTmpBlock);
					oTmpBlock.Apnd("-", 1);
					_local_String_FmtDtRef_ApndInt(nDateTime.GetDay(), 2, oTmpBlock);
					break;
				
				case 'H': // "%H" = The hour as a decimal number using a 24-hour clock (range 00 to 23).
					_local_String_FmtDtRef_ApndInt(nDateTime.GetHour(), 2, oTmpBlock);
					break;
				
				case 'm': // "%m" = The month as a decimal number (range 01 to 12).
					_local_String_FmtDtRef_ApndInt(nDateTime.GetMonth(), 2, oTmpBlock);
					break;
				
				case 'M': // "%M" = The minute as a decimal number (range 00 to 59).
					_local_String_FmtDtRef_ApndInt(nDateTime.GetMin(), 2, oTmpBlock);
					break;
				
				case 'S': // "%S" = The second as a decimal number (range 00 to 60).  (The range is up to 60 to allow for occasional leap seconds.)
					_local_String_FmtDtRef_ApndInt(nDateTime.GetSec(), 2, oTmpBlock);
					break;
				
				case 'T': // "%T" = The time in 24-hour notation (%H:%M:%S).
					_local_String_FmtDtRef_ApndInt(nDateTime.GetHour(), 2, oTmpBlock);
					oTmpBlock.Apnd(":", 1);
					_local_String_FmtDtRef_ApndInt(nDateTime.GetMin(), 2, oTmpBlock);
					oTmpBlock.Apnd(":", 1);
					_local_String_FmtDtRef_ApndInt(nDateTime.GetSec(), 2, oTmpBlock);
					break;
				
				case 'y': // "%y" = The year as a decimal number without a century (range 00 to 99).
					_local_String_FmtDtRef_ApndInt(nDateTime.GetYear() % 100, 2, oTmpBlock);
					break;
				
				case 'Y': // "%Y" = The year as a decimal number including the century.
					_local_String_FmtDtRef_ApndInt(nDateTime.GetYear() % 10000, 4, oTmpBlock);
					break;
				
				case '%': // "%%" = escaped '%' sign
					oTmpBlock.Apnd("%", 1);
					break;
				
				case '\0': // "%\0" = dollar sign is the last char in the string 
					oTmpBlock.Apnd("%", 1);
					oCurPos--;
					break;
				
				default: // not a valid %[a-zA-Z] switch
					oTmpBlock.Apnd(oCurPos, 2);
					break;
			}
			
			oCurPos++;
		}
		else
			oTmpBlock.Apnd(oCurPos, 1);
	}
	
	oTmpBlock.Apnd("\0", 1);
	
	this->Alloc(oTmpBlock.Data());
}
