#ifndef ACPL_IO_H
#define ACPL_IO_H

#include "Macros.h"
#include "Types.h"

namespace acpl
{
	class IoChannel
	{
			acplHideClassCopy(IoChannel);
		
		public:
			// TODO: enum LastIoError && virtual GetLastIoError() = 0
			
			enum Whence
			{
				whStart = 1,
				whCurrent = 2,
				whEnd = 3
			};
		
		public:
			IoChannel();
			virtual ~IoChannel();
			
			virtual bool WriteAll(const void *nData, acpl::SizeT nDataSize);
			virtual bool ReadAll(void *nData, acpl::SizeT nDataSize);
			
			virtual bool Write(const void *nData, acpl::SizeT nDataSize, acpl::SizeT &nBytesWritten) = 0;
			virtual bool Read(void *nData, acpl::SizeT nDataSize, acpl::SizeT &nBytesRead) = 0;
		
	};
	
	class IoNull : public acpl::IoChannel
	{
			acplHideClassCopyI(IoNull, acpl::IoChannel());
		
		public:
			inline IoNull() { }
			inline virtual ~IoNull() { }
			
			virtual bool Write(const void *nData, acpl::SizeT nDataSize, acpl::SizeT &nBytesWritten);
			virtual bool Read(void *nData, acpl::SizeT nDataSize, acpl::SizeT &nBytesRead);
	};
	
	class IoStd : public acpl::IoChannel
	{
			acplHideClassCopyI(IoStd, acpl::IoChannel());
		
		public:
			inline IoStd() { }
			inline virtual ~IoStd() { }
			
			inline virtual bool Read(void *nData, acpl::SizeT nDataSize, acpl::SizeT &nBytesRead) { return this->ReadIn(nData, nDataSize, nBytesRead); }
			inline virtual bool Write(const void *nData, acpl::SizeT nDataSize, acpl::SizeT &nBytesWritten) { return this->WriteOut(nData, nDataSize, nBytesWritten); }
			inline virtual bool ReadAll(void *nData, acpl::SizeT nDataSize) { return this->ReadInAll(nData, nDataSize); }
			inline virtual bool WriteAll(const void *nData, acpl::SizeT nDataSize) { return this->WriteOutAll(nData, nDataSize); }
			
			static bool ReadIn(void *nData, acpl::SizeT nDataSize, acpl::SizeT &nBytesRead);
			static bool ReadInAll(void *nData, acpl::SizeT nDataSize);
			static bool WriteOut(const void *nData, acpl::SizeT nDataSize, acpl::SizeT &nBytesWritten);
			static bool WriteOutAll(const void *nData, acpl::SizeT nDataSize);
			static bool WriteErr(const void *nData, acpl::SizeT nDataSize, acpl::SizeT &nBytesWritten);
			static bool WriteErrAll(const void *nData, acpl::SizeT nDataSize);
	};
}

#endif // ACPL_IO_H
