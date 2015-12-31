#include "Io.h"

#include "_cp/Basic.inc.cpp"

// 
// IoChannel
// 

acpl::IoChannel::IoChannel()
{
	
}

acpl::IoChannel::~IoChannel()
{
	
}

bool acpl::IoChannel::WriteAll(const void *nData, acpl::SizeT nDataSize)
{
	const acpl::UInt8 *oDataPtr = static_cast<const acpl::UInt8 *>(nData);
	const acpl::UInt8 *oDataEnd = (oDataPtr + nDataSize);
	acpl::SizeT oByteCount = 0;
	while (oDataPtr < oDataEnd)
	{
		if (this->Write(oDataPtr, (oDataEnd - oDataPtr), oByteCount) == true)
		{
			if (oByteCount > 0)
				oDataPtr += oByteCount;
			else
				return false;
		}
		else
			return false;
	}

	return true;
}

bool acpl::IoChannel::ReadAll(void *nData, acpl::SizeT nDataSize)
{
	acpl::UInt8 *oDataPtr = static_cast<acpl::UInt8 *>(nData);
	const acpl::UInt8 *oDataEnd = (oDataPtr + nDataSize);
	acpl::SizeT oByteCount = 0;
	while (oDataPtr < oDataEnd)
	{
		if (this->Read(oDataPtr, (oDataEnd - oDataPtr), oByteCount) == true)
		{
			if (oByteCount > 0)
				oDataPtr += oByteCount;
			else
				return false;
		}
		else
			return false;
	}

	return true;
}



// 
// IoNull
// 

bool acpl::IoNull::Write(const void *, acpl::SizeT nDataBytes, acpl::SizeT &nBytesWritten)
{
	nBytesWritten = nDataBytes;
	return true;
}

bool acpl::IoNull::Read(void *, acpl::SizeT, acpl::SizeT &nBytesRead)
{
	nBytesRead = 0;
	return false;
}



// 
// IoStd
// 

bool acpl::IoStd::ReadIn(void *nData, acpl::SizeT nDataSize, acpl::SizeT &nBytesRead)
{
	return acpl::cp::_iostd_in_read(nData, nDataSize, nBytesRead);
}

bool acpl::IoStd::ReadInAll(void *nData, acpl::SizeT nDataSize)
{
	acpl::UInt8 *oDataPtr = static_cast<acpl::UInt8 *>(nData);
	const acpl::UInt8 *oDataEnd = (oDataPtr + nDataSize);
	acpl::SizeT oByteCount = 0;
	
	while (oDataPtr < oDataEnd)
	{
		if (acpl::cp::_iostd_in_read(oDataPtr, (oDataEnd - oDataPtr), oByteCount) == true)
		{
			if (oByteCount > 0)
				oDataPtr += oByteCount;
			else
				return false;
		}
		else
			return false;
	}

	return true;
}

bool acpl::IoStd::WriteOut(const void *nData, acpl::SizeT nDataSize, acpl::SizeT &nBytesWritten)
{
	return acpl::cp::_iostd_out_write(nData, nDataSize, nBytesWritten);
}

bool acpl::IoStd::WriteOutAll(const void *nData, acpl::SizeT nDataSize)
{
	const acpl::UInt8 *oDataPtr = static_cast<const acpl::UInt8 *>(nData);
	const acpl::UInt8 *oDataEnd = (oDataPtr + nDataSize);
	acpl::SizeT oByteCount = 0;
	
	while (oDataPtr < oDataEnd)
	{
		if (acpl::cp::_iostd_out_write(oDataPtr, (oDataEnd - oDataPtr), oByteCount) == true)
		{
			if (oByteCount > 0)
				oDataPtr += oByteCount;
			else
				return false;
		}
		else
			return false;
	}
	
	return true;
}

bool acpl::IoStd::WriteErr(const void *nData, acpl::SizeT nDataSize, acpl::SizeT &nBytesWritten)
{
	return acpl::cp::_iostd_err_write(nData, nDataSize, nBytesWritten);
}

bool acpl::IoStd::WriteErrAll(const void *nData, acpl::SizeT nDataSize)
{
	const acpl::UInt8 *oDataPtr = static_cast<const acpl::UInt8 *>(nData);
	const acpl::UInt8 *oDataEnd = (oDataPtr + nDataSize);
	acpl::SizeT oByteCount = 0;
	
	while (oDataPtr < oDataEnd)
	{
		if (acpl::cp::_iostd_err_write(oDataPtr, (oDataEnd - oDataPtr), oByteCount) == true)
		{
			if (oByteCount > 0)
				oDataPtr += oByteCount;
			else
				return false;
		}
		else
			return false;
	}
	
	return true;
}
