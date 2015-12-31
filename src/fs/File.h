#ifndef ACPL_FS_FILE_H
#define ACPL_FS_FILE_H

#include "Path.h"

namespace acpl
{
	
	class File : public acpl::IoChannel
	{
			acplHideClassCopyI(File, acpl::IoChannel());
		
		public:
			typedef union
			{
				int uInt;
				void *uVptr;
			} Handle;
			
			enum LastError
			{
				leNone,
				leSysErr,
				leNoSupport,
				leInvalAm,
				leInvalWh,
				lePathEmpty,
				lePathTooLong,
				leExists,
				leNonExist,
				leIsDir,
				leNotDir,
				leSymlinkLoop,
				leNoPerm,
				leInUse,
				leTooLarge,
				leWouldBlk,
				leIsOpen,
				leNotOpen,
				leInvalOff,
				leNoWrite,
				leNoRead
			};
			
			enum AccessMode
			{
				amNone = 0,
				amReadOnly = 1,
				amWriteOnly = 2,
				amReadWrite = 3
			};
		
		private:
			mutable acpl::Enum<acpl::File::LastError, acpl::UInt8> mLastError;
			acpl::Path mPath;
			acpl::File::Handle mHandle;
			
			void Init();
		
		public:
			inline File() { this->Init(); this->ResetLastError(); }
			inline File(const acpl::String::Ref &nPath) { this->Init(); this->ResetLastError(); this->mPath.Set(nPath); }
			inline virtual ~File() { this->Close(); }
			
			inline acpl::File::LastError GetLastError() const { return this->mLastError.Value(); }
			inline void ResetLastError() { this->mLastError = acpl::File::leNone; }
			
			inline const acpl::Path &GetPath() const { return this->mPath; }
			inline acpl::Path &GetPath() { return this->mPath; }
			
			bool IsOpen() const;
			bool Open(acpl::File::AccessMode nMode, bool nTruncate = false, bool nCreate = false, const acpl::Path::Permission &nPerm = 0644);
			void Close();
			bool Unlink();
			
			bool GetOffset(acpl::OffT &nOffset) const;
			bool SetOffset(acpl::OffT nOffset, acpl::File::Whence nWhence, acpl::OffT &nNewOffset);
			inline bool SetOffset(acpl::OffT nOffset, acpl::File::Whence nWhence = acpl::File::whStart) { acpl::OffT oNewOffset; return this->SetOffset(nOffset, nWhence, oNewOffset); }
			virtual bool Write(const void *nData, acpl::SizeT nDataSize, acpl::SizeT &nBytesWritten);
			virtual bool Read(void *nData, acpl::SizeT nDataSize, acpl::SizeT &nBytesRead);
	};
	
}

#endif // ACPL_FS_FILE_H
