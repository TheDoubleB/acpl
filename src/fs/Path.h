#ifndef ACPL_FS_PATH_H
#define ACPL_FS_PATH_H

#include "../String.h"
#include "../Bits.h"

namespace acpl
{
	class File;
	class Dir;
	
	class Path
	{
			acplHideClassCopy(Path);
		
		public:
			enum LastError
			{
				leNone,
				leSysErr,
				leNoSupport,
				leIsEmpty,
				leTooLong,
				leNonExist,
				leNotDir,
				leSymlinkLoop,
				leNoPerm
			};
			
			class String
			{
					acplHideClassCopy(String);
				
				public:
					typedef acpl::Mem::Block<acpl::Pathchar>::Type Holder;
				
				private:
					acpl::Path::String::Holder mStrBuf;
				
				protected:
					friend class acpl::Path;
					inline const acpl::Path::String::Holder &GetHolder() const { return this->mStrBuf; }
					acpl::Path::String::Holder &GetHolder() { return this->mStrBuf; }
				
				public:
					inline String() { }
					inline String(const acpl::String::Ref &nPath) { this->Set(nPath); }
					inline virtual ~String() { }
					
					inline const acpl::Pathchar *GetStr() const { return ((this->mStrBuf.Length() > 0) ? this->mStrBuf.Data() : reinterpret_cast<const acpl::Pathchar *>(L"")); }
					inline bool IsEmpty() const { return (this->mStrBuf.Length() == 0 || this->mStrBuf.Data()[0] == 0); }
					
					acpl::Path::String &Set(const acpl::Path::String &nPath);
					acpl::Path::String &Set(const acpl::String::Ref &nPath);
					acpl::Path::String &Apnd(const acpl::Path::String &nPath);
					acpl::Path::String &Apnd(const acpl::String::Ref &nPath);
					inline acpl::Path::String &Clear() { this->mStrBuf.Clear(); return *this; }
			};
			
			class Permission
			{
					acplHideClassCopy(Permission);
				
				public:
					enum FlagSingle
					{
						fsNone = 00000,
						fsOthE = 00001,
						fsOthW = 00002,
						fsOthR = 00004,
						fsGrpE = 00010,
						fsGrpW = 00020,
						fsGrpR = 00040,
						fsUsrE = 00100,
						fsUsrW = 00200,
						fsUsrR = 00400,
						fsStck = 01000,
						fsSgid = 02000,
						fsSuid = 04000
					};
					
					enum FlagGroup
					{
						fgNone = 00000,
						fgOth  = 00007,
						fgGrp  = 00070,
						fgUsr  = 00700,
						fgAll  = 00777,
						fgFull = 07777
					};
					
					typedef char Str[11];
				
				private:
					acpl::UInt16 mVal;
				
				public:
					inline Permission() { this->Clear(); }
					inline Permission(acpl::UInt16 nVal) { this->SetValue(nVal); }
					inline virtual ~Permission() { }
					
					inline acpl::UInt16 GetValue() const { return this->mVal; }
					inline void SetValue(acpl::UInt16 nVal) { this->mVal = (nVal & acpl::Path::Permission::fgFull); }
					inline void Clear() { this->mVal = 0; }
					
					inline bool HasSetuid()   const { return acpl::Bits::AllSet(this->mVal, acpl::Path::Permission::fsSuid); }
					inline bool HasSetgid()   const { return acpl::Bits::AllSet(this->mVal, acpl::Path::Permission::fsSgid); }
					inline bool HasSticky()   const { return acpl::Bits::AllSet(this->mVal, acpl::Path::Permission::fsStck); }
					inline bool CanUsrRead()  const { return acpl::Bits::AllSet(this->mVal, acpl::Path::Permission::fsUsrR); }
					inline bool CanUsrWrite() const { return acpl::Bits::AllSet(this->mVal, acpl::Path::Permission::fsUsrW); }
					inline bool CanUsrExec()  const { return acpl::Bits::AllSet(this->mVal, acpl::Path::Permission::fsUsrE); }
					inline bool CanGrpRead()  const { return acpl::Bits::AllSet(this->mVal, acpl::Path::Permission::fsGrpR); }
					inline bool CanGrpWrite() const { return acpl::Bits::AllSet(this->mVal, acpl::Path::Permission::fsGrpW); }
					inline bool CanGrpExec()  const { return acpl::Bits::AllSet(this->mVal, acpl::Path::Permission::fsGrpE); }
					inline bool CanOthRead()  const { return acpl::Bits::AllSet(this->mVal, acpl::Path::Permission::fsOthR); }
					inline bool CanOthWrite() const { return acpl::Bits::AllSet(this->mVal, acpl::Path::Permission::fsOthW); }
					inline bool CanOthExec()  const { return acpl::Bits::AllSet(this->mVal, acpl::Path::Permission::fsOthE); }
					
					void MakeStr(acpl::Path::Permission::Str &nModeStr) const;
			};
			
			class StatBase
			{
					acplHideClassCopy(StatBase);
				
				public:
					enum Type
					{
						tUnknown,
						tFile,
						tDir,
						tSymLink,
						tDevBlock,
						tDevChar,
						tSocket,
						tPipe
					};
					typedef acpl::Enum<acpl::Path::StatBase::Type, acpl::UInt8> TypeCont;
					
					struct InfoStat
					{
						bool sIsSet;
						acpl::Path::StatBase::TypeCont sType;
						acpl::Path::Permission sPerm;
						acpl::OffT sSize;
						acpl::TimeT sTimeCre;
						acpl::TimeT sTimeMod;
						acpl::TimeT sTimeAcc;
					};
				
				private:
					acpl::Path::StatBase::InfoStat mInfoStat;
				
				protected:
					inline StatBase() { this->Clear(); }
					inline acpl::Path::StatBase::InfoStat &GetInfoStat()  { return this->mInfoStat; }
					void Clear();
				
				public:
					inline virtual ~StatBase() { }
					
					inline bool IsStatted() const { return this->mInfoStat.sIsSet; }
					inline acpl::Path::StatBase::Type GetType() const { return this->mInfoStat.sType.Value(); }
					inline const acpl::Path::Permission &GetPerm() const { return this->mInfoStat.sPerm; }
					inline acpl::OffT GetSize() const { return this->mInfoStat.sSize; }
					inline acpl::TimeT GetTimeCre() const { return this->mInfoStat.sTimeCre; }
					inline acpl::TimeT GetTimeMod() const { return this->mInfoStat.sTimeMod; }
					inline acpl::TimeT GetTimeAcc() const { return this->mInfoStat.sTimeAcc; }
					
					void MakePermStr(acpl::Path::Permission::Str &nModeStr) const;
			};
			
			class Stat : public acpl::Path::StatBase
			{
					acplHideClassCopyI(Stat, acpl::Path::StatBase());
				
				private:
					mutable acpl::Enum<acpl::Path::LastError, acpl::UInt8> mLastError;
				
				public:
					inline Stat() { this->ResetLastError(); }
					inline virtual ~Stat() { }
					
					inline acpl::Path::LastError GetLastError() const { return this->mLastError.Value(); }
					inline void ResetLastError() { this->mLastError = acpl::Path::leNone; }
					
					inline void Clear() { acpl::Path::StatBase::Clear(); }
					
					bool DoStat(const acpl::Path &nPath, bool nResolveLink = true);
			};
		
		private:
			mutable acpl::Enum<acpl::Path::LastError, acpl::UInt8> mLastError;
			acpl::Path::String mStr;
			
			void Ctd();
		
		protected:
			friend class acpl::Path::Stat;
			friend class acpl::File;
			friend class acpl::Dir;
			const acpl::Path::String::Holder &GetStrHolder() const { return this->mStr.GetHolder(); }
		
		public:
			inline Path() { this->ResetLastError(); }
			inline Path(const acpl::String::Ref &nPath) { this->ResetLastError(); this->Set(nPath); }
			inline virtual ~Path() { }
			
			inline acpl::Path::LastError GetLastError() const { return this->mLastError.Value(); }
			inline void ResetLastError() { this->mLastError = acpl::Path::leNone; }
			
			inline const acpl::Pathchar *GetStr() const { return this->mStr.GetStr(); }
			inline bool IsEmpty() const { return this->mStr.IsEmpty(); }
			bool IsAbsolute() const;
			bool IsRoot() const;
			
			inline acpl::Path &Set(const acpl::Path::String  &nPath) { this->mStr.Set(nPath);  this->Ctd(); return *this; }
			inline acpl::Path &Set(const acpl::String::Ref   &nPath) { this->mStr.Set(nPath);  this->Ctd(); return *this; }
			inline acpl::Path &Apnd(const acpl::Path::String &nPath) { this->mStr.Apnd(nPath); this->Ctd(); return *this; }
			inline acpl::Path &Apnd(const acpl::String::Ref  &nPath) { this->mStr.Apnd(nPath); this->Ctd(); return *this; }
			inline acpl::Path &Clear() { this->mStr.Clear(); return *this; }
			
			acpl::Path &Current();
			acpl::Path &Parent();
			acpl::Path &Child(const acpl::Path::String &nChild);
			acpl::Path &Child(const acpl::String::Ref &nChild);
			
			void GetDirName(acpl::Path::String &nDirName) const;
			void GetBaseName(acpl::Path::String &nBaseName) const;
			inline void GetFileName(acpl::Path::String &nFileName) const { this->GetFileName(this->mStr, nFileName); }
			inline void GetFileExt(acpl::Path::String &nFileExt) const { this->GetFileExt(this->mStr, nFileExt); }
			
			static void GetFileName(const acpl::Path::String &nPath, acpl::Path::String &nFileName);
			static void GetFileExt(const acpl::Path::String &nPath, acpl::Path::String &nFileExt);
			
			// TODO:
			//   Add `bool Normalize()`, which gets rid of excess [curent_dir] and [parent_dir] signs
			//   Add `bool Absolutize()`, which *without resolving* combines [current_working_dir] and [path] and normalizes it
			bool Resolve();
			
			bool SetPerm(const acpl::Path::Permission &nPerm);
			// TODO:
			//   Add some sort of SetAttr* methods, which set (various) attributes on an element pointed to by path (like hidden flag on Win)
			//   (longterm) Add access control list (ACL) functionality
			
			// TODO:
			//   Add `bool Move()`, which renames/moves the element pointed to by path to another name/location
			
			static void SetUmask(acpl::UInt16 nVal);
	};
}

#endif // ACPL_FS_PATH_H
