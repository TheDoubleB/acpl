#include "File.h"

#include "../_cp/Filesys.inc.cpp"


// PUBLIC

bool acpl::File::IsOpen() const
{
	return acpl::cp::_fs_file_is_open(this->mHandle);
}

bool acpl::File::Open(acpl::File::AccessMode nMode, bool nTruncate, bool nCreate, const acpl::Path::Permission &nPerm)
{
	if (this->mPath.IsEmpty() == true)
	{
		this->mLastError = acpl::File::lePathEmpty;
		return false;
	}
	else
	if (nMode < 1 || nMode > 3)
	{
		this->mLastError = acpl::File::leInvalAm;
		return false;
	}
	else
	if (nTruncate == true && nMode < 2)
	{
		
		this->mLastError = acpl::File::leNoWrite;
		return false;
	}
	else
	if (acpl::cp::_fs_file_is_open(this->mHandle) == true)
	{
		this->mLastError = acpl::File::leIsOpen;
		return false;
	}
	else
	if(acpl::cp::_fs_file_open(this->mHandle, this->mPath.GetStrHolder(), nMode, nTruncate, nCreate, nPerm) == true)
		return true;
	else
		this->mLastError = acpl::cp::_fs_file_get_last_error(acpl::pi::_fs_file_glec_open);
	
	return false;
}

void acpl::File::Close()
{
	if (acpl::cp::_fs_file_is_open(this->mHandle) == true)
	{
		acpl::cp::_fs_file_close(this->mHandle);
		acpl::cp::_fs_file_init(this->mHandle);
	}
}

bool acpl::File::Unlink()
{
	if (this->mPath.IsEmpty() == true)
	{
		this->mLastError = acpl::File::lePathEmpty;
		return false;
	}
	else
	if(acpl::cp::_fs_file_unlink(this->mPath.GetStrHolder()) == true)
		return true;
	else
		this->mLastError = acpl::cp::_fs_file_get_last_error(acpl::pi::_fs_file_glec_unlink);
	
	return false;
}


bool acpl::File::GetOffset(acpl::OffT &nOffset) const
{
	if (acpl::cp::_fs_file_is_open(this->mHandle) != true)
	{
		this->mLastError = acpl::File::leNotOpen;
		return false;
	}
	else
	if(acpl::cp::_fs_file_getoff(this->mHandle, nOffset) == true)
		return true;
	else
		this->mLastError = acpl::cp::_fs_file_get_last_error(acpl::pi::_fs_file_glec_getoff);
	
	return false;
}

bool acpl::File::SetOffset(acpl::OffT nOffset, acpl::File::Whence nWhence, acpl::OffT &nNewOffset)
{
	if (acpl::cp::_fs_file_is_open(this->mHandle) != true)
	{
		this->mLastError = acpl::File::leNotOpen;
		return false;
	}
	else
	if (nWhence < 1 || nWhence > 3)
	{
		this->mLastError = acpl::File::leInvalWh;
		return false;
	}
	else
	if(acpl::cp::_fs_file_setoff(this->mHandle, nWhence, nOffset, nNewOffset) == true)
		return true;
	else
		this->mLastError = acpl::cp::_fs_file_get_last_error(acpl::pi::_fs_file_glec_setoff);
	
	return false;
}

bool acpl::File::Write(const void *nData, acpl::SizeT nDataSize, acpl::SizeT &nBytesWritten)
{
	if (acpl::cp::_fs_file_is_open(this->mHandle) != true)
	{
		this->mLastError = acpl::File::leNotOpen;
		return false;
	}
	else
	if (acpl::cp::_fs_file_write(this->mHandle, nData, nDataSize, nBytesWritten) == true)
		return true;
	else
		this->mLastError = acpl::cp::_fs_file_get_last_error(acpl::pi::_fs_file_glec_write);
	
	return false;
}

bool acpl::File::Read(void *nData, acpl::SizeT nDataSize, acpl::SizeT &nBytesRead)
{
	if (acpl::cp::_fs_file_is_open(this->mHandle) != true)
	{
		this->mLastError = acpl::File::leNotOpen;
		return false;
	}
	else
	if (acpl::cp::_fs_file_read(this->mHandle, nData, nDataSize, nBytesRead) == true)
		return true;
	else
		this->mLastError = acpl::cp::_fs_file_get_last_error(acpl::pi::_fs_file_glec_read);
	
	return false;
}


// PRIVATE

void acpl::File::Init()
{
	acpl::cp::_fs_file_init(this->mHandle);
}
