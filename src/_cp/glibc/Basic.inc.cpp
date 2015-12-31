#ifndef ACPL_CP_GLIBC_BASIC_INC_CPP
#define ACPL_CP_GLIBC_BASIC_INC_CPP

#include <unistd.h>

namespace acpl
{
	
	namespace cp
	{
		// 
		// Stdio
		// 
		
		static inline bool _iostd_in_read(void *nData, acpl::SizeT nDataSize, acpl::SizeT &nBytesRead)
		{
			ssize_t oBytesRead = read(0, nData, nDataSize);
			
			if (oBytesRead != -1)
			{
				nBytesRead = static_cast<acpl::SizeT>(oBytesRead);
				return true;
			}
			else
			{
				nBytesRead = 0;
				return false;
			}
		}
		
		static inline bool _iostd_out_write(const void *nData, acpl::SizeT nDataSize, acpl::SizeT &nBytesWritten)
		{
			ssize_t oBytesWritten = write(1, nData, nDataSize);
			
			if (oBytesWritten != -1)
			{
				nBytesWritten = static_cast<acpl::SizeT>(oBytesWritten);
				return true;
			}
			else
			{
				nBytesWritten = 0;
				return false;
			}
		}
		
		static inline bool _iostd_err_write(const void *nData, acpl::SizeT nDataSize, acpl::SizeT &nBytesWritten)
		{
			ssize_t oBytesWritten = write(2, nData, nDataSize);
			
			if (oBytesWritten != -1)
			{
				nBytesWritten = static_cast<acpl::SizeT>(oBytesWritten);
				return true;
			}
			else
			{
				nBytesWritten = 0;
				return false;
			}
		}
	}
	
}


#endif // ACPL_CP_GLIBC_BASIC_INC_CPP
