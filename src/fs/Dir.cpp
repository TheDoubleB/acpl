#include "Dir.h"

#include "../_cp/Filesys.inc.cpp"

// 
// Dir::Entry
// 

// PUBLIC

void acpl::Dir::Entry::Clear()
{
	acpl::Path::StatBase::Clear();
	this->mInfoBasic.sName.Clear();
	this->mInfoBasic.sTypeRef = acpl::Dir::Entry::tUnknown;
	this->mInfoBasic.sIsHidden = false;
}

static inline bool _local_Dir_ListEntries_ProcessError(acpl::Enum<acpl::Dir::LastError, acpl::UInt8> &nLastError)
{
	acpl::Dir::LastError oLe = acpl::cp::_fs_dir_get_last_error(acpl::pi::_fs_dir_glec_list_entries);
	if (oLe != acpl::Dir::leNone)
	{
		nLastError = oLe;
		return false;
	}
	else
		return true;
}
bool acpl::Dir::ListEntries(acpl::Dir::EntryList &nEntries, bool nStat, acpl::Dir::fpFilter nFilterFunc) const
{
	nEntries.Clear();
	
	if (this->mPath.IsEmpty() == true)
	{
		this->mLastError = acpl::Dir::lePathEmpty;
		return false;
	}
	
	// WARNING: These two variables are intended for backend only; do not fiddle with them here!
	acpl::cp::_fs_dir_handle_t oHandle;
	
	acpl::Path::String::Holder oPathMod; // <<< NOTE: We need this because this path is manipulated with in the backend
	oPathMod.Apnd(this->mPath.GetStrHolder().Data(), this->mPath.GetStrHolder().Length());
	
	nEntries.Add();
	if (acpl::cp::_fs_dir_list_get_first(oHandle, nStat, oPathMod, nEntries.GetLast().GetInfoBasic(), nEntries.GetLast().GetInfoStat()) != true)
	{
		nEntries.Clear();
		return _local_Dir_ListEntries_ProcessError(this->mLastError);
	}
	
	do
	{
		if (nFilterFunc != NULL && nFilterFunc(nEntries.GetLast()) != true)
			nEntries.RemoveTail(1);
		
		nEntries.Add();
	}
	while (acpl::cp::_fs_dir_list_get_next(oHandle, nStat, oPathMod, nEntries.GetLast().GetInfoBasic(), nEntries.GetLast().GetInfoStat()) == true);
	
	nEntries.RemoveTail(1);
	
	acpl::cp::_fs_dir_list_end(oHandle);
	
	if (_local_Dir_ListEntries_ProcessError(this->mLastError) == true)
		return true;
	else
	{
		nEntries.Clear();
		return false;
	}
}


bool acpl::Dir::Create(const acpl::Path::Permission &nPerm)
{
	if (this->mPath.IsEmpty() == true)
	{
		this->mLastError = acpl::Dir::lePathEmpty;
		return false;
	}
	else
	if(acpl::cp::_fs_dir_create(this->mPath.GetStrHolder(), nPerm) == true)
		return true;
	else
		this->mLastError = acpl::cp::_fs_dir_get_last_error(acpl::pi::_fs_dir_glec_create);
	
	return false;
}

bool acpl::Dir::Remove()
{
	if (this->mPath.IsEmpty() == true)
	{
		this->mLastError = acpl::Dir::lePathEmpty;
		return false;
	}
	else
	if(acpl::cp::_fs_dir_remove(this->mPath.GetStrHolder()) == true)
		return true;
	else
		this->mLastError = acpl::cp::_fs_dir_get_last_error(acpl::pi::_fs_dir_glec_remove);
	
	return false;
}
