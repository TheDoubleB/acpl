#include "Path.h"

#include "../_cp/Filesys.inc.cpp"
#include "../_cp/Memory.inc.cpp"


// 
// Path
// 

// LOCAL

typedef const acpl::String::Ref                 &_local_Path_StrRef;
typedef acpl::Mem::Block<acpl::Pathchar>::Type  &_local_Path_BlkRef_PC;
typedef acpl::Mem::Block<char>::Type            &_local_Path_BlkRef_MB;
typedef acpl::Mem::Block<wchar_t>::Type         &_local_Path_BlkRef_WC;

static inline bool _local_Path_FindLastDelim(const acpl::Path::String::Holder &nHld, acpl::SizeT &nOff)
{
	nOff = nHld.Length();
	while (nOff > 0)
		if (acpl::cp::_fs_path_char_is_delim(nHld.Data()[--nOff]) == true)  // <<< NOTE: there is a '--' for nOff !!!
			return true; // Skip possible trailing delimiters and don't break while they are present ?
	
	return false;
}

static inline bool _local_Path_FindLastDot(const acpl::Path::String::Holder &nHld, acpl::SizeT &nOff)
{
	nOff = nHld.Length();
	while (nOff > 0)
		if (nHld.Data()[--nOff] == '.')  // <<< NOTE: there is a '--' for nOff on only the first comparing!!! && NOTE: '.' is quite unhygenic with wchar_t, should be L'.'
			return true;
	
	return false;
}

static inline bool _local_Path_GetBaseName(const acpl::Path::String::Holder &nHldPath, acpl::Path::String::Holder &nHldBaseName) // returns true if nHldBaseName is not empty
{
	nHldBaseName.Clear();
	
	if (nHldPath.Length() != 0 && nHldPath.Data()[0] != 0) // <<< NOTE: c/p of negated IsEmpty() method
	{
		acpl::SizeT oCurOff = 0;
		
		if (acpl::cp::_fs_path_is_root_dir(nHldPath) != true)
		{
			// find the last delimiter or the beginning of the path
			if (_local_Path_FindLastDelim(nHldPath, oCurOff) == true)
				oCurOff++;
			else
				// NOTE: If this relative path (no delimiter was found) contains drive letter as well, move beyond drive designator
				oCurOff += acpl::cp::_fs_path_elem_rel_with_drive_offset(nHldPath);
		}
		
		nHldBaseName.Apnd((nHldPath.Data() + oCurOff), (nHldPath.Length() - oCurOff)); // <<< NOTE: will (should!) have the NULL terminator
		
		if (nHldBaseName.Length() == 0 || nHldBaseName.Data()[0] == 0) // IsEmpty
			nHldBaseName.Clear().Apnd(acpl::cp::_fs_path_get_current_dir(), (acpl::cp::_fs_path_get_current_dir_len() + 1)); // with NULL terminator
		
		return true;
	}
	
	return false;
}

template <class tStr>
static inline void _local_Path_Child(acpl::Path::String &nStr, acpl::Path::String::Holder &nHld, const tStr &nChild)
{
	if (nStr.IsEmpty() == true)
		nStr.Clear().Apnd(nChild);
	else
	{
		nHld.CutRight(1); // cut NULL terminator
		
		if (acpl::cp::_fs_path_is_root_dir(nHld) != true)
		{
			// If is not root dir and it is not a relative path that includes only drive letter (e.g. "C:")
			if (acpl::cp::_fs_path_elem_rel_with_drive_offset(nHld) < nHld.Length())
				nHld.Apnd(acpl::cp::_fs_path_get_dir_delim(), acpl::cp::_fs_path_get_dir_delim_len());
		}
		else
		{
			// If is root dir but does not end with a trailing delimiter (e.g. "\\\\server")
			if (acpl::cp::_fs_path_char_is_delim(nHld.Data()[nHld.Length() - 1]) != true)
				nHld.Apnd(acpl::cp::_fs_path_get_dir_delim(), acpl::cp::_fs_path_get_dir_delim_len());
		}
		
		nHld.Apnd(reinterpret_cast<const acpl::Pathchar *>(L""), 1);
		nStr.Apnd(nChild);
	}
}


// PUBLIC

bool acpl::Path::IsAbsolute() const
{
	return acpl::cp::_fs_path_is_absolute(this->mStr.GetHolder());
}

bool acpl::Path::IsRoot() const
{
	return acpl::cp::_fs_path_is_root_dir(this->mStr.GetHolder());
}


acpl::Path &acpl::Path::Current()
{
	if (this->IsEmpty() == true)
		this->mStr.GetHolder().Clear().Apnd(acpl::cp::_fs_path_get_current_dir(), (acpl::cp::_fs_path_get_current_dir_len() + 1)); // with NULL terminator
	
	return *this;
}

acpl::Path &acpl::Path::Parent()
{
	acpl::Path::String::Holder &oHld = this->mStr.GetHolder();
	
	if (this->IsEmpty() == true)
	{
		oHld.Clear().Apnd(acpl::cp::_fs_path_get_parent_dir(), (acpl::cp::_fs_path_get_parent_dir_len() + 1)); // with NULL terminator
		return *this;
	}
	
	if (acpl::cp::_fs_path_is_root_dir(oHld) == true)
		return *this;
	
	
	// find the last delimiter or the beginning of the path
	acpl::SizeT oCurOff;
	bool oIsRelWithDrive = false;
	if (_local_Path_FindLastDelim(oHld, oCurOff) == true)
		oCurOff++;
	else
	{
		// NOTE: If this relative path (no delimiter was found) contains drive letter as well, move beyond drive designator
		if ((oCurOff += acpl::cp::_fs_path_elem_rel_with_drive_offset(oHld)) > 0)
			oIsRelWithDrive = true;
	}
	
	
	if (acpl::cp::_fs_path_elem_is_parent_dir(oHld, oCurOff) == true)
	{
		// The last/only element is a parent dir designator, so just append another parent dir designator
		oHld.SetLength(oCurOff + acpl::cp::_fs_path_get_parent_dir_len()); // cut the (possible) NULL terminator(s)
		oHld.Apnd(acpl::cp::_fs_path_get_dir_delim(), acpl::cp::_fs_path_get_dir_delim_len());
		oHld.Apnd(acpl::cp::_fs_path_get_parent_dir(), (acpl::cp::_fs_path_get_parent_dir_len() + 1)); // with NULL terminator
	}
	else
	if (acpl::cp::_fs_path_elem_is_current_dir(oHld, oCurOff) == true)
	{
		// The last/only element is a current dir designator, so just replace it with a parent dir designator
		oHld.SetLength(oCurOff);
		oHld.Apnd(acpl::cp::_fs_path_get_parent_dir(), (acpl::cp::_fs_path_get_parent_dir_len() + 1)); // with NULL terminator
	}
	else
	{
		// The last/only element is a named element
		
		// Cut the element
		oHld.SetLength(oCurOff);
		
		// Cut trailing delimiter(s)
		while (acpl::cp::_fs_path_is_root_dir(oHld) != true && oHld.Length() > 0 && acpl::cp::_fs_path_char_is_delim(oHld.Data()[oHld.Length() - 1]) == true)
			oHld.CutRight(acpl::cp::_fs_path_get_dir_delim_len());
		
		// If the result is empty, append a current dir designator, otherwise append only NULL terminator
		if (this->IsEmpty() == true || oIsRelWithDrive == true)
			oHld.Apnd(acpl::cp::_fs_path_get_current_dir(), (acpl::cp::_fs_path_get_current_dir_len() + 1)); // <<< with NULL terminator
		else
			oHld.Apnd(reinterpret_cast<const acpl::Pathchar *>(L""), 1);
	}
	
	return *this;
}

acpl::Path &acpl::Path::Child(const acpl::Path::String &nChild)
{
	_local_Path_Child(this->mStr, this->mStr.GetHolder(), nChild);
	this->Ctd();
	return *this;
}

acpl::Path &acpl::Path::Child(const acpl::String::Ref &nChild)
{
	_local_Path_Child(this->mStr, this->mStr.GetHolder(), nChild);
	this->Ctd();
	return *this;
}


void acpl::Path::GetDirName(acpl::Path::String &nDirName) const
{
	acpl::Path::String::Holder &oOutHld = nDirName.GetHolder();
	oOutHld.Clear();
	
	if (this->IsEmpty() != true)
	{
		oOutHld.Apnd(this->mStr.GetHolder().Data(), this->mStr.GetHolder().Length());
		
		if (acpl::cp::_fs_path_is_root_dir(oOutHld) == true)
			return;
		
		// find the last delimiter or the beginning of the path
		acpl::SizeT oCurOff;
		if (_local_Path_FindLastDelim(oOutHld, oCurOff) == true)
			oCurOff++;
		else
		{
			// NOTE: If this relative path (no delimiter was found) contains drive letter as well, move beyond drive designator
			oCurOff += acpl::cp::_fs_path_elem_rel_with_drive_offset(oOutHld);
			
			oOutHld.SetLength(oCurOff);
			oOutHld.Apnd(acpl::cp::_fs_path_get_current_dir(), (acpl::cp::_fs_path_get_current_dir_len() + 1)); // with NULL terminator
			return;
		}
		
		
		// Cut the element
		oOutHld.SetLength(oCurOff);
		
		// Cut trailing delimiter(s)
		while (acpl::cp::_fs_path_is_root_dir(oOutHld) != true && oOutHld.Length() > 0 && acpl::cp::_fs_path_char_is_delim(oOutHld.Data()[oOutHld.Length() - 1]) == true)
			oOutHld.CutRight(acpl::cp::_fs_path_get_dir_delim_len());
		
		oOutHld.Apnd(reinterpret_cast<const acpl::Pathchar *>(L""), 1);
	}
	else
		// if path is empty or contains only base name, dir name is current dir
		oOutHld.Apnd(acpl::cp::_fs_path_get_current_dir(), (acpl::cp::_fs_path_get_current_dir_len() + 1)); // <<< NOTE: with NULL terminator
}

void acpl::Path::GetBaseName(acpl::Path::String &nBaseName) const
{
	_local_Path_GetBaseName(this->mStr.GetHolder(), nBaseName.GetHolder());
}


void acpl::Path::GetFileName(const acpl::Path::String &nPath, acpl::Path::String &nFileName) // static
{
	acpl::Path::String::Holder &oOutHld = nFileName.GetHolder();
	if (_local_Path_GetBaseName(nPath.GetHolder(), oOutHld) != true)
		// Base name is empty, nothing else to be done
		return;
	
	acpl::SizeT oCurOff;
	if (_local_Path_FindLastDot(oOutHld, oCurOff) != true || oCurOff == 0 ||
		acpl::cp::_fs_path_elem_is_current_dir(oOutHld, oCurOff) == true ||
		acpl::cp::_fs_path_elem_is_parent_dir(oOutHld, oCurOff) == true ||
		acpl::cp::_fs_path_is_root_dir(oOutHld) == true)
		// No dot found, the last dot is the first char, or the base name is current, parent or root dir - the whole base name is the file name
		return;
	
	oCurOff++;
	if (oCurOff == oOutHld.Length() || oOutHld.Data()[oCurOff] == 0)
		// The last dot is the last char - the whole base name is the file name
		return;
	
	oOutHld.SetLength(oCurOff);
	oOutHld.Data()[--oCurOff] = 0; // <<< NOTE: the '--' before oCurOff
}

void acpl::Path::GetFileExt(const acpl::Path::String &nPath, acpl::Path::String &nFileExt) // static
{
	acpl::Path::String::Holder &oOutHld = nFileExt.GetHolder();
	if (_local_Path_GetBaseName(nPath.GetHolder(), oOutHld) != true)
		// Base name is empty, nothing else to be done
		return;
	
	acpl::SizeT oCurOff;
	if (_local_Path_FindLastDot(oOutHld, oCurOff) != true || oCurOff == 0 ||
		acpl::cp::_fs_path_elem_is_current_dir(oOutHld, oCurOff) == true ||
		acpl::cp::_fs_path_elem_is_parent_dir(oOutHld, oCurOff) == true ||
		acpl::cp::_fs_path_is_root_dir(oOutHld) == true)
	{
		// No dot found, the last dot is the first char, or the base name is current, parent or root dir - file extension is not present
		oOutHld.SetLength(1); // <<< NOTE: 1 for NULL terminator
		oOutHld.Data()[0] = 0;
		return;
	}
	
	oOutHld.CutLeft(++oCurOff); // <<< NOTE: the '++' before oCurOff
}


bool acpl::Path::Resolve()
{
	if (this->IsEmpty() == true)
		this->mLastError = acpl::Path::leIsEmpty;
	else
	if (acpl::cp::_fs_path_resolve(this->mStr.GetHolder()) == true)
		return true;
	else
		this->mLastError = acpl::cp::_fs_path_get_last_error(acpl::pi::_fs_path_glec_resolve);
	
	return false;
}


bool acpl::Path::SetPerm(const acpl::Path::Permission &nPerm)
{
	if (this->IsEmpty() == true)
		this->mLastError = acpl::Path::leIsEmpty;
	else
	if (acpl::cp::_fs_path_setperm(this->mStr.GetHolder(), nPerm) == true)
		return true;
	else
		this->mLastError = acpl::cp::_fs_path_get_last_error(acpl::pi::_fs_path_glec_setperm);
	
	return false;
}


void acpl::Path::SetUmask(acpl::UInt16 nVal) // static
{
	acpl::cp::_fs_path_umask(nVal);
}


// PRIVATE

void acpl::Path::Ctd() // CTD = Cut trailing delimiters
{
	acpl::Path::String::Holder &oHld = this->mStr.GetHolder();
	
	if (oHld.Data()[oHld.Length() - 1] == L'\0')
		oHld.CutRight(1);
	
	// Cut trailing delimiter(s)
	while (acpl::cp::_fs_path_is_root_dir(oHld) != true && oHld.Length() > 0 && acpl::cp::_fs_path_char_is_delim(oHld.Data()[oHld.Length() - 1]) == true)
		oHld.CutRight(acpl::cp::_fs_path_get_dir_delim_len());
	
	oHld.Apnd(reinterpret_cast<const acpl::Pathchar *>(L""), 1);
}




// 
// Path::String
// 

static inline void _local_Path_String_Apnd_Single(acpl::cp::_string_mb_state_t &nMbState, _local_Path_BlkRef_MB nBlock, const acpl::cp::_string_wchar_cont_t &nWcc, acpl::SizeT)
{
	acpl::cp::_string_mbc_cont_t oMbc;
	nBlock.Apnd(oMbc, acpl::pi::_string_wchar_to_mbc(oMbc, nWcc, nMbState));
}
static inline void _local_Path_String_Apnd_Single(acpl::cp::_string_mb_state_t &, _local_Path_BlkRef_WC nBlock, const acpl::cp::_string_wchar_cont_t &nWcc, acpl::SizeT nWccLen)
{
	nBlock.Apnd(nWcc, nWccLen);
}
template <class tBlkRef, class tPathchar> // <<< NOTE: Needs to be a template, otherwise it won't compile
static inline void _local_Path_String_Apnd_Native(tBlkRef &nBlock, const tPathchar *nPath) // native, pure copy
{
	const tPathchar *oPathEnd = nPath;
	while (*oPathEnd++ != 0) ; // find and include NULL terminator
	nBlock.Apnd(nPath, (oPathEnd - nPath - 1)); // NOTE: `- 1` because we don't want the NULL terminator !!
}
static inline bool _local_Path_String_Apnd_Utf8_DecSeqFollowBytes(const char *nPtr, acpl::UInt8 nExpSeqSize, acpl::Unichar &nUc)
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
static inline void _local_Path_String_Apnd_Utf8(acpl::cp::_string_mb_state_t &nMbState, _local_Path_BlkRef_PC nBlock, const char *nPath)
{
	while (*nPath != 0)
	{
		acpl::UInt8 oFirstByte = *nPath;
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
		
		if (oExpSeqSize == 1 || _local_Path_String_Apnd_Utf8_DecSeqFollowBytes(nPath, oExpSeqSize, oUc) != true)
			nPath++;
		else
			nPath += oExpSeqSize;
		
		acpl::cp::_string_wchar_cont_t oWcc;
		_local_Path_String_Apnd_Single(nMbState, nBlock, oWcc, acpl::cp::_string_wchar_seq_encode(oUc, oWcc)); // <<< NOTE: the seq_encode function call
	}
}
static inline void _local_Path_String_Apnd_Mbs(acpl::cp::_string_mb_state_t &, _local_Path_BlkRef_MB nBlock, const char *nPath) // Separate for MB-only (Mbs->MB = native)
{
	_local_Path_String_Apnd_Native(nBlock, nPath);
}
static inline void _local_Path_String_Apnd_Mbs(acpl::cp::_string_mb_state_t &nMbState, _local_Path_BlkRef_WC nBlock, const char *nPath) // Separate for WC-only
{
	acpl::cp::_string_wchar_cont_t oWcc;
	acpl::SizeT oOff = 0;
	while (acpl::pi::_string_mbc_to_wchar(oWcc, nPath, oOff, nMbState) == true) // <<< NOTE: acpl::pi::_string_mbc_to_wchar returns false when NULL terminator is reached
	{
		_local_Path_String_Apnd_Single(nMbState, nBlock, oWcc, 1); // <<< NOTE: the length is always 1 for MSCRT
		nPath += oOff;
		oOff = 0;
	}
}
static inline void _local_Path_String_Apnd_Wcs(acpl::cp::_string_mb_state_t &nMbState, _local_Path_BlkRef_MB nBlock, const wchar_t *nPath) // Separate for MB-only
{
	while (*nPath != 0)
	{
		acpl::cp::_string_wchar_cont_t oWcc = { *nPath };
		_local_Path_String_Apnd_Single(nMbState, nBlock, oWcc, 1); // <<< NOTE: the length of the wchar container is 1
		nPath++;
	}
}
static inline void _local_Path_String_Apnd_Wcs(acpl::cp::_string_mb_state_t &, _local_Path_BlkRef_WC nBlock, const wchar_t *nPath) // Separate for WC-only (Wcs->WC = native)
{
	_local_Path_String_Apnd_Native(nBlock, nPath);
}
static inline void _local_Path_String_Apnd_Ucs(acpl::cp::_string_mb_state_t &nMbState, _local_Path_BlkRef_PC nBlock, const acpl::Unichar *nPath)
{
	while (*nPath != 0)
	{
		acpl::cp::_string_wchar_cont_t oWcc;
		_local_Path_String_Apnd_Single(nMbState, nBlock, oWcc, acpl::cp::_string_wchar_seq_encode(*nPath, oWcc)); // <<< NOTE: the seq_encode function call
		nPath++;
	}
}
static inline void _local_Path_String_Apnd(_local_Path_BlkRef_PC nBlock, _local_Path_StrRef nPath)
{
	if (nPath.IsNull() != true)
	{
		acpl::cp::_string_mb_state_t oMbState;
		acpl::cp::_string_mb_state_init(oMbState);
		
		switch (nPath.GetType())
		{
			case acpl::String::Ref::rtNone: // <<< NOTE: Will only append the NULL terminator
				break;
			
			case acpl::String::Ref::rtSelf: // <<< NOTE: no need for a separate Self function because we need to convert it anyway
			case acpl::String::Ref::rtUtf8:
				_local_Path_String_Apnd_Utf8(oMbState, nBlock, static_cast<const char *>(nPath.GetPtr()));
				break;
			
			case acpl::String::Ref::rtMbs:
				_local_Path_String_Apnd_Mbs(oMbState, nBlock, static_cast<const char *>(nPath.GetPtr()));
				break;
			
			case acpl::String::Ref::rtWcs:
				_local_Path_String_Apnd_Wcs(oMbState, nBlock, static_cast<const wchar_t *>(nPath.GetPtr()));
				break;
			
			case acpl::String::Ref::rtUcs:
				_local_Path_String_Apnd_Ucs(oMbState, nBlock, static_cast<const acpl::Unichar *>(nPath.GetPtr()));
				break;
		}
		
		nBlock.Apnd(reinterpret_cast<const acpl::Pathchar *>(L""), 1);
		
		acpl::cp::_string_mb_state_destroy(oMbState);
	}
}

acpl::Path::String &acpl::Path::String::Set(const acpl::Path::String &nPath)
{
	this->mStrBuf.Clear();
	_local_Path_String_Apnd_Native(this->mStrBuf, nPath.GetStr()); // will *not* append NULL terminator
	this->mStrBuf.Apnd(reinterpret_cast<const acpl::Pathchar *>(L""), 1);
	return *this;
}

acpl::Path::String &acpl::Path::String::Set(const acpl::String::Ref &nPath)
{
	this->mStrBuf.Clear();
	_local_Path_String_Apnd(this->mStrBuf, nPath); // will append NULL terminator
	return *this;
}

acpl::Path::String &acpl::Path::String::Apnd(const acpl::Path::String &nPath)
{
	this->mStrBuf.CutRight(1); // cut NULL terminator
	_local_Path_String_Apnd_Native(this->mStrBuf, nPath.GetStr()); // will *not* append NULL terminator
	this->mStrBuf.Apnd(reinterpret_cast<const acpl::Pathchar *>(L""), 1);
	return *this;
}

acpl::Path::String &acpl::Path::String::Apnd(const acpl::String::Ref &nPath)
{
	this->mStrBuf.CutRight(1); // cut NULL terminator
	_local_Path_String_Apnd(this->mStrBuf, nPath); // will append NULL terminator
	return *this;
}




// 
// Path::Permission
// 

// PUBLIC

// NOTE: This function leaves the first char alone !!!
static inline void _local_Path_Permission_MakeStr(const acpl::Path::Permission &nAm, acpl::Path::Permission::Str &nModeStr)
{
	// access (usr:rwx)
	nModeStr[1] = ((nAm.CanUsrRead()  == true) ? 'r' : '-');
	nModeStr[2] = ((nAm.CanUsrWrite() == true) ? 'w' : '-');
	nModeStr[3] = ((nAm.HasSetuid()   == true) ? ((nAm.CanUsrExec()  == true) ? 's' : 'S') : ((nAm.CanUsrExec()  == true) ? 'x' : '-')); // with setuid correction
	
	// access (grp:rwx)
	nModeStr[4] = ((nAm.CanGrpRead()  == true) ? 'r' : '-');
	nModeStr[5] = ((nAm.CanGrpWrite() == true) ? 'w' : '-');
	nModeStr[6] = ((nAm.HasSetgid()   == true) ? ((nAm.CanGrpExec()  == true) ? 's' : 'S') : ((nAm.CanGrpExec()  == true) ? 'x' : '-')); // with setgid correction
	
	// access (oth:rwx)
	nModeStr[7] = ((nAm.CanOthRead()  == true) ? 'r' : '-');
	nModeStr[8] = ((nAm.CanOthWrite() == true) ? 'w' : '-');
	nModeStr[9] = ((nAm.HasSticky()   == true) ? ((nAm.CanOthExec()  == true) ? 't' : 'T') : ((nAm.CanOthExec()  == true) ? 'x' : '-')); // with sticky correction
	
	// NULL terminator
	nModeStr[10] = '\0';
}

void acpl::Path::Permission::MakeStr(acpl::Path::Permission::Str &nModeStr) const
{
	nModeStr[0] = '-';
	_local_Path_Permission_MakeStr(*this, nModeStr);
}




// 
// Path::StatBase
// 

// PUBLIC

void acpl::Path::StatBase::MakePermStr(acpl::Path::Permission::Str &nModeStr) const
{
	static const char oTypeAb[] = "?-dlbcsp"; // type "alphabet"
	nModeStr[0] = oTypeAb[(this->mInfoStat.sType.Value() % (sizeof(oTypeAb) - 1))];
	
	_local_Path_Permission_MakeStr(this->mInfoStat.sPerm, nModeStr);
}


// PROTECTED

void acpl::Path::StatBase::Clear()
{
	this->mInfoStat.sIsSet = false;
	this->mInfoStat.sType = acpl::Path::StatBase::tUnknown;
	this->mInfoStat.sPerm.Clear();
	this->mInfoStat.sSize = 0;
	this->mInfoStat.sTimeCre = 0;
	this->mInfoStat.sTimeMod = 0;
	this->mInfoStat.sTimeAcc = 0;
}




// 
// Path::Stat
// 

// PUBLIC

bool acpl::Path::Stat::DoStat(const acpl::Path &nPath, bool nResolveLink)
{
	this->Clear();
	
	if (nPath.IsEmpty() == true)
		this->mLastError = acpl::Path::leIsEmpty;
	else
	if (acpl::cp::_fs_path_stat(nPath.GetStrHolder(), nResolveLink, this->GetInfoStat()) == true)
		return true;
	else
		this->mLastError = acpl::cp::_fs_path_get_last_error(acpl::pi::_fs_path_glec_stat);
	
	return false;
}
