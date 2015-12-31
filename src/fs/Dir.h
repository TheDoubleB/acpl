#ifndef ACPL_FS_DIR_H
#define ACPL_FS_DIR_H

#include "Path.h"

namespace acpl
{
	
	class Dir
	{
			acplHideClassCopy(Dir);
		
		public:
			enum LastError
			{
				leNone,
				leSysErr,
				leNoSupport,
				lePathEmpty,
				lePathTooLong,
				leExists,
				leNonExist,
				leNotDir,
				leSymlinkLoop,
				leNoPerm,
				leNotEmpty
			};
			
			class Entry : public acpl::Path::StatBase
			{
					acplHideClassCopyIa(Entry, nEntry, acpl::Path::StatBase(), mInfoBasic(nEntry.mInfoBasic.sTypeRef));
				
				public:
					class Name : public acpl::Path::String { };
					
					struct InfoBasic
					{
						inline InfoBasic(acpl::Path::StatBase::TypeCont &nTypeRef) : sTypeRef(nTypeRef) {  }
						acpl::Dir::Entry::Name sName;
						acpl::Path::StatBase::TypeCont &sTypeRef;
						bool sIsHidden;
					};
				
				private:
					acpl::Dir::Entry::InfoBasic mInfoBasic;
				
				protected:
					friend class acpl::Dir;
					inline acpl::Dir::Entry::InfoBasic &GetInfoBasic() { return this->mInfoBasic; }
				
				public:
					inline Entry() : mInfoBasic(GetInfoStat().sType) { this->Clear(); }
					inline virtual ~Entry() { }
					
					void Clear();
					
					inline const acpl::Dir::Entry::Name &GetName() const { return this->mInfoBasic.sName; }
					inline bool IsHidden() const { return this->mInfoBasic.sIsHidden; }
			};
			typedef acpl::ArrayEdaNone<acpl::Dir::Entry>::Type EntryList;
			
			typedef bool (*fpFilter)(const acpl::Dir::Entry &nEntry);
		
		private:
			mutable acpl::Enum<acpl::Dir::LastError, acpl::UInt8> mLastError;
			acpl::Path mPath;
		
		public:
			inline Dir() { this->ResetLastError(); }
			inline Dir(const acpl::String::Ref &nPath) { this->ResetLastError(); this->mPath.Set(nPath); }
			inline virtual ~Dir() { }
			
			inline acpl::Dir::LastError GetLastError() const { return this->mLastError.Value(); }
			inline void ResetLastError() { this->mLastError = acpl::Dir::leNone; }
			
			inline const acpl::Path &GetPath() const { return this->mPath; }
			inline acpl::Path &GetPath() { return this->mPath; }
			
			bool ListEntries(acpl::Dir::EntryList &nEntries, bool nStat = true, acpl::Dir::fpFilter nFilterFunc = NULL) const;
			
			bool Create(const acpl::Path::Permission &nPerm = 0755);
			bool Remove();
	};
	
}

#endif // ACPL_FS_DIR_H
