#ifndef ACPL_STRING_H
#define ACPL_STRING_H

#include "Memory.h"
#include "Array.h"

namespace acpl
{
	
	class DateTime;
	class NetworkAddr;
	class Ipv4Addr;
	class Ipv6Addr;
	
	
	class String
	{
			acplHideClassAsop(String);
		
		public:
			class Ref
			{
					acplHideClassCopy(Ref);
				
				public:
					enum Type
					{
						rtNone,
						rtSelf,
						rtUtf8,
						rtMbs,
						rtWcs,
						rtUcs
					};
				
				private:
					acpl::UInt8 mType;
					
					inline void Set(const void *nPtr, acpl::String::Ref::Type nType) { this->mTgt.uPtr = nPtr; this->mType = nType; }
				
				protected:
					union
					{
						const void *uPtr;
						acpl::SizeT uOff;
					} mTgt;
					
					inline Ref(acpl::String::Ref::Type nType) { this->Set(NULL, nType); }
				
				public:
					inline Ref(const char *nStr, bool nIsLocale = false) { this->Set(nStr, (nIsLocale == true) ? acpl::String::Ref::rtMbs : acpl::String::Ref::rtUtf8); }
					inline Ref(const wchar_t *nStr) { this->Set(nStr, acpl::String::Ref::rtWcs); }
					inline Ref(const acpl::Unichar *nStr) { this->Set(nStr, acpl::String::Ref::rtUcs); }
					inline Ref(const acpl::String &nStr) { this->Set((nStr.IsNull() == false) ? nStr.Utf8() : NULL, acpl::String::Ref::rtSelf); }
					inline Ref(const acpl::String *nStr) { this->Set((nStr != NULL && nStr->IsNull() == false) ? nStr->Utf8() : NULL, acpl::String::Ref::rtSelf); }
					inline virtual ~Ref() { this->Set(NULL, acpl::String::Ref::rtNone); }
					
					inline virtual bool IsNull() const { return (this->mTgt.uPtr == NULL); }
					inline virtual const void *GetPtr() const { return this->mTgt.uPtr; }
					inline acpl::String::Ref::Type GetType() const { return static_cast<acpl::String::Ref::Type>(this->mType); }
					
					void Measure(acpl::SizeT nStrMaxLen, acpl::SizeT nStrOff, acpl::SizeT &nRefOff, acpl::SizeT &nEncByteSize) const;
					void Convert(acpl::SizeT nRefOff, acpl::SizeT nEncByteSize, char *nDst) const;
				
			};
			
			class SelfRefBase : public acpl::String::Ref
			{
					acplHideClassCopyI(SelfRefBase, acpl::String::Ref(acpl::String::Ref::rtSelf));
				
				protected:
					inline SelfRefBase() : acpl::String::Ref(acpl::String::Ref::rtSelf) { }
				
				public:
					virtual const char *Utf8() const = 0;
			};
			
			class Pos : public acpl::String::SelfRefBase
			{
					acplHideClassAsop(Pos);
				
				private:
					const acpl::String *mString;
				
				public:
					inline Pos() : acpl::String::SelfRefBase() { this->Attach(NULL); }
					inline Pos(const acpl::String &nString) : acpl::String::SelfRefBase() { this->Attach(&nString); }
					inline Pos(const acpl::String *nString) : acpl::String::SelfRefBase() { this->Attach(nString); }
					inline Pos(const acpl::String::Pos &nPos) : acpl::String::SelfRefBase() { this->Attach(nPos); }
					inline virtual ~Pos() { this->Attach(NULL); }
					
					inline acpl::String::Pos &Attach(const acpl::String &nString) { return this->Attach(&nString); }
					inline acpl::String::Pos &Attach(const acpl::String *nString) { this->mTgt.uOff = 0; this->mString = nString; return *this; }
					inline acpl::String::Pos &Attach(const acpl::String::Pos &nPos) { this->mTgt.uOff = nPos.mTgt.uOff; this->mString = nPos.mString; return *this; }
					inline const acpl::String *GetString() const { return this->mString; }
					inline bool IsSameString(const acpl::String::Pos &nPos) const { return (this->mString == nPos.mString); }
					
					inline bool IsInBounds() const { return (this->mString != NULL && this->mString->ByteLength() > this->mTgt.uOff); }
					
					inline virtual bool IsNull() const { return (this->mString == NULL); }
					inline virtual const void *GetPtr() const { return ((this->IsInBounds() == true) ? this->mString->Utf8() + this->mTgt.uOff : NULL); }
					inline virtual const char *Utf8() const { return ((this->IsInBounds() == true) ? this->mString->Utf8() + this->mTgt.uOff : ""); }
					
					inline char GetChar() const { return ((this->IsInBounds() == true) ? this->mString->Utf8()[this->mTgt.uOff] : '\0'); }
					acpl::Unichar GetUnichar() const;
					bool GetUnichar(acpl::Unichar &nChar) const;
					
					bool IsFirst() const;
					bool IsLast() const;
					
					// TODO: Add `bool nCaseSensitive = true` to unichar and substring ones
					bool First();
					bool First(acpl::Unichar nUc);
					bool First(const acpl::String::Ref &nSubStr);
					bool Last();
					bool Last(acpl::Unichar nUc);
					bool Last(const acpl::String::Ref &nSubStr);
					bool Next();
					bool Next(acpl::Unichar nUc);
					bool Next(const acpl::String::Ref &nSubStr);
					bool Prev();
					bool Prev(acpl::Unichar nUc);
					bool Prev(const acpl::String::Ref &nSubStr);
					
					bool Forth(acpl::SizeT nCount);
					bool Back(acpl::SizeT nCount);
					bool SkipFirst(acpl::SizeT nCount);
					bool SkipLast(acpl::SizeT nCount);
					
					// TODO: Add ToInt && ToFloat from the position
			};
			
			class PermRef : public acpl::String::SelfRefBase
			{
					acplHideClassCopyI(PermRef, acpl::String::SelfRefBase());
				
				protected:
					inline PermRef() { }
			};
			
			class AllocPermRef : public acpl::String::PermRef
			{
					acplHideClassCopyI(AllocPermRef, acpl::String::PermRef());
				
				private:
					char *mAlloced;
				
				protected:
					inline AllocPermRef() : acpl::String::PermRef(), mAlloced(NULL) { }
					void Alloc(const acpl::String::Ref &nRef, acpl::SizeT nMaxCount = acpl::Num<acpl::SizeT>::Max(), acpl::SizeT nStartOffset = 0);
				
				public:
					inline virtual ~AllocPermRef() { acpl::Mem::Free(this->mAlloced); }
					
					inline virtual bool IsNull() const { return (this->mAlloced == NULL); }
					inline virtual const void *GetPtr() const { return this->mAlloced; }
					inline virtual const char *Utf8() const { return ((this->mAlloced != NULL) ? this->mAlloced : ""); }
			};
			
			class StringRef : public acpl::String::AllocPermRef
			{
					acplHideClassCopyI(StringRef, acpl::String::AllocPermRef());
				
				public:
					inline StringRef(const acpl::String::Ref &nRef, acpl::SizeT nMaxCount = acpl::Num<acpl::SizeT>::Max(), acpl::SizeT nStartOffset = 0) { this->Alloc(nRef, nMaxCount, nStartOffset); }
			};
			
			class IntRef : public acpl::String::PermRef
			{
					acplHideClassCopyI(IntRef, acpl::String::PermRef());
				
				public:
					class Type
					{
							acplHideClassCopy(Type);
						
						private:
							acpl::UInt64 mValue;
							acpl::UInt8 mMeta;
						
						public:
							// `nValue` cast to signed type is a trick for float
							// types (doesn't affect the unsigned integer types)
							template <class tType>
							inline Type(tType nValue) : mValue(static_cast<acpl::SInt64>(nValue)), mMeta(sizeof(tType))
							{
								// The complication with `<=` and '!=' for
								// `nValue` instead of just `<` is used because
								// GCC goes bonkers when `tType` is an unsigned
								// type.
								if (acpl::Num<tType>::IsSigned() == true && nValue <= 0 && nValue != 0)
									this->mMeta |= 0x80;
							}
														
							inline acpl::UInt64 GetValue() const { return this->mValue; }
							inline acpl::UInt8  GetMeta()  const { return this->mMeta; }
					};
				
				private:
					char mStr[65];
					void Conv(const acpl::String::IntRef::Type &nArg, acpl::UInt8 nBaseAgg, acpl::UInt16 nPaddingAgg, const char *nPrepand);
				
				protected:
					inline IntRef() : acpl::String::PermRef() { }
					inline void Conv(const acpl::String::IntRef::Type &nArg, acpl::UInt8 nBase, bool nUpperCase, acpl::UInt8 nPadding, char nPaddingChar, const char *nPrepand = NULL) { this->Conv(nArg, ((nBase & 0x7F) | (nUpperCase << 7)), ((nPadding << 8) | nPaddingChar), nPrepand); }
				
				public:
					inline IntRef(const acpl::String::IntRef::Type &nArg, acpl::UInt8 nBase = 10, bool nUpperCase = false, acpl::UInt8 nPadding = 0, char nPaddingChar = ' ') : acpl::String::PermRef() { this->Conv(nArg, nBase, nUpperCase, nPadding, nPaddingChar); }
					inline virtual const char *Utf8() const { return ((this->mTgt.uPtr != NULL) ? static_cast<const char *>(this->mTgt.uPtr) : ""); }
				
			};
			
			class UnicharRef : public acpl::String::IntRef
			{
					acplHideClassCopyI(UnicharRef, acpl::String::IntRef());
				
				public:
					inline UnicharRef(acpl::Unichar nArg, acpl::UInt8 nPadding = 4) { if (nPadding < 4) nPadding = 4; else if (nPadding > 8) nPadding = 8; this->Conv(nArg, 16, true, nPadding, '0', "U+"); }
			};
			
			class PtrRef : public acpl::String::IntRef
			{
					acplHideClassCopyI(PtrRef, acpl::String::IntRef());
				
				public:
					inline PtrRef(const void *nArg, bool nUpperCase = false, acpl::UInt8 nPadding = 0) { this->Conv(reinterpret_cast<acpl::UInt64>(nArg), 16, nUpperCase, ((nPadding <= 16) ? nPadding : 16), '0', "0x"); }
					inline static acpl::UInt8 NativePadding() { return (sizeof(void *) * 2); }
			};
			
			// TODO: create `FloatRef` class for `Float{32,64}` types
			
			class IpAddrRef : public acpl::String::PermRef
			{
					acplHideClassCopyI(IpAddrRef, acpl::String::PermRef());
				
				private:
					char mStr[46];
				
				public:
					IpAddrRef(const acpl::Ipv4Addr &nIpv4Addr);
					IpAddrRef(const acpl::Ipv6Addr &nIpv6Addr, bool nOptimize = false, bool nIpv4Mode = false);
					
					inline virtual const char *Utf8() const { return ((this->mTgt.uPtr != NULL) ? static_cast<const char *>(this->mTgt.uPtr) : ""); }
			};
			
			class NetAddrRef : public acpl::String::AllocPermRef
			{
					acplHideClassCopyI(NetAddrRef, acpl::String::AllocPermRef());
				
				public:
					NetAddrRef(const acpl::NetworkAddr &nNetAddr);
					inline NetAddrRef(const acpl::Ipv4Addr &nIpv4Addr) { this->Alloc(acpl::String::IpAddrRef(nIpv4Addr).Utf8()); }
					inline NetAddrRef(const acpl::Ipv6Addr &nIpv6Addr, bool nOptimize = false, bool nIpv4Mode = false) { this->Alloc(acpl::String::IpAddrRef(nIpv6Addr, nOptimize, nIpv4Mode).Utf8()); }
			};
			
			class RefList
			{
					acplHideClassCopy(RefList);
				
				public:
					typedef acpl::ArrayEdaDelete<acpl::String::PermRef *>::Type Type;
				
				private:
					acpl::String::RefList::Type mList;
				
				public:
					inline RefList() { }
					inline virtual ~RefList() { }
					
					inline acpl::String::RefList::Type &List() { return this->mList; }
					inline const acpl::String::RefList::Type &List() const { return this->mList; }
					
					RefList &String(const acpl::String::Ref &nArg, acpl::SizeT nMaxCount = acpl::Num<acpl::SizeT>::Max(), acpl::SizeT nStartOffset = 0);
					inline RefList &Int(const acpl::String::IntRef::Type &nArg, acpl::UInt8 nBase = 10, bool nUpperCase = false, acpl::UInt8 nPadding = 0, char nPaddingChar = ' ') { this->mList.Add(new acpl::String::IntRef(nArg, nBase, nUpperCase, nPadding, nPaddingChar)); return *this; }
					// TODO: add `Float()`
 					// RefList &Float(acpl::Float nArg, acpl::UInt8 nPrecision = 6, char nDecimalPoint = '.', acpl::UInt8 nPadding = 0, char nPaddingChar = ' ');
 					// RefList &Float(acpl::Double nArg, acpl::UInt8 nPrecision = 6, char nDecimalPoint = '.', acpl::UInt8 nPadding = 0, char nPaddingChar = ' ');
			};
			
			class FmtRefBase : public acpl::String::AllocPermRef
			{
					acplHideClassCopyI(FmtRefBase, acpl::String::AllocPermRef());
				
				private:
					char *mFmt;
				
				protected:
					inline FmtRefBase() : acpl::String::AllocPermRef(), mFmt(NULL) { }
				
				public:
					inline virtual ~FmtRefBase() { acpl::Mem::Free(this->mFmt); }
					
					inline const char *FmtUtf8() const { return ((this->mFmt != NULL) ? this->mFmt : ""); }
					void SetFmt(const acpl::String::Ref &nRef);
			};
			
			class FmtRef : public acpl::String::FmtRefBase
			{
					acplHideClassCopyI(FmtRef, acpl::String::FmtRefBase());
				
				public:
					inline FmtRef() { }
					inline FmtRef(const acpl::String::Ref &nFmt) { this->SetFmt(nFmt); }
					inline FmtRef(const acpl::String::Ref &nFmt, const acpl::String::RefList &nRefList) { this->SetFmt(nFmt); this->Format(nRefList); }
					
					void Format(const acpl::String::RefList &nRefList);
			};
			
			class FmtDtRef : public acpl::String::FmtRefBase
			{
					acplHideClassCopyI(FmtDtRef, acpl::String::FmtRefBase());
				
				public:
					inline FmtDtRef() { }
					inline FmtDtRef(const acpl::String::Ref &nFmt) { this->SetFmt(nFmt); }
					inline FmtDtRef(const acpl::String::Ref &nFmt, const acpl::DateTime &nDateTime) { this->SetFmt(nFmt); this->Format(nDateTime); }
					
					void Format(const acpl::DateTime &nDateTime);
			};
		
		private:
			acpl::Mem::Block<char>::Type mStrBuf;
			
			acpl::UInt64 ToInt(acpl::UInt8 nBase, acpl::UInt8 nTypeInfo, bool nSwapDecMarks, bool &nSuccess);
			acpl::Float::Largest ToFloat(bool nSwapDecMarks, bool &nSuccess);
		
		public:
			inline String() : mStrBuf(32) { }
			inline String(const acpl::String      &nRef, acpl::SizeT nMaxCount = acpl::Num<acpl::SizeT>::Max(), acpl::SizeT nStartOffset = 0) : mStrBuf(32) { this->Set(nRef, nMaxCount, nStartOffset); }
			inline String(const acpl::String::Ref &nRef, acpl::SizeT nMaxCount = acpl::Num<acpl::SizeT>::Max(), acpl::SizeT nStartOffset = 0) : mStrBuf(32) { this->Set(nRef, nMaxCount, nStartOffset); }
			inline virtual ~String() { this->Purge(); }
			
			// TODO: Add a `SetGrowSize()` method
			
			inline bool IsNull()  const { return (this->mStrBuf.Length() == 0); }
			inline bool IsEmpty() const { return (this->mStrBuf.Length() == 0 || *this->mStrBuf.Data() == '\0'); }
			bool IsAllBlanks() const;
			bool IsAllWspaces() const;
			acpl::SizeT Length() const;
			inline acpl::SizeT ByteLength() const { return ((this->mStrBuf.Length() > 0) ? this->mStrBuf.Length() - 1 : 0); }
			
			inline const char *Utf8() const { return ((this->mStrBuf.Length() > 0) ? this->mStrBuf.Data() : ""); }
			
			acpl::String &Set(const acpl::String::Ref &nRef, acpl::SizeT nMaxCount = acpl::Num<acpl::SizeT>::Max(), acpl::SizeT nStartOffset = 0);
			acpl::String &Apnd(const acpl::String::Ref &nRef, acpl::SizeT nMaxCount = acpl::Num<acpl::SizeT>::Max(), acpl::SizeT nStartOffset = 0);
			acpl::String &Ins(acpl::SizeT nIndex, const acpl::String::Ref &nRef, acpl::SizeT nMaxCount = acpl::Num<acpl::SizeT>::Max(), acpl::SizeT nStartOffset = 0);
			acpl::String &Ins(const acpl::String::Pos &nAtPos, bool nBef, const acpl::String::Ref &nRef, acpl::SizeT nMaxCount = acpl::Num<acpl::SizeT>::Max(), acpl::SizeT nStartOffset = 0);
			acpl::String &Repl(acpl::SizeT nIndex, acpl::SizeT nCount, const acpl::String::Ref &nRef, acpl::SizeT nMaxCount = acpl::Num<acpl::SizeT>::Max(), acpl::SizeT nStartOffset = 0);
			acpl::String &Repl(const acpl::String::Pos &nAtPos, bool nBef, acpl::SizeT nCount, const acpl::String::Ref &nRef, acpl::SizeT nMaxCount = acpl::Num<acpl::SizeT>::Max(), acpl::SizeT nStartOffset = 0);
			acpl::String &Repl(const acpl::String::Pos &nFromPos, bool nBef, const acpl::String::Pos &nToPos, bool nExcl, const acpl::String::Ref &nRef, acpl::SizeT nMaxCount = acpl::Num<acpl::SizeT>::Max(), acpl::SizeT nStartOffset = 0);
			
			// TODO: Create Fill() method that clears and sets the string with a unichar
			acpl::String &PadLeft(acpl::Unichar nUc, acpl::SizeT nNewLen);
			acpl::String &PadRight(acpl::Unichar nUc, acpl::SizeT nNewLen);
			
			acpl::String &Clear();
			acpl::String &Purge();
			acpl::String &Cut(acpl::SizeT nIndex, acpl::SizeT nCount);
			acpl::String &Cut(const acpl::String::Pos &nAtPos, bool nBef, acpl::SizeT nCount);
			acpl::String &Cut(const acpl::String::Pos &nFromPos, bool nBef, const acpl::String::Pos &nToPos, bool nExcl);
			acpl::String &CutLeft(acpl::SizeT nCount);
			acpl::String &CutLeft(const acpl::String::Pos &nAtPos, bool nExcl);
			acpl::String &CutRight(acpl::SizeT nCount);
			acpl::String &CutRight(const acpl::String::Pos &nAtPos, bool nExcl);
			acpl::String &Trim();
			acpl::String &TrimLeft();
			acpl::String &TrimRight();
			
			void ToUtf8(acpl::Mem::Block<char>::Type &nChars, bool nClearBfr = true) const;
			void ToMbs(acpl::Mem::Block<char>::Type &nChars, bool nClearBfr = true) const;
			void ToWcs(acpl::Mem::Block<wchar_t>::Type &nChars, bool nClearBfr = true) const;
			void ToUcs(acpl::Mem::Block<acpl::Unichar>::Type &nChars, bool nClearBfr = true) const;
			
			template <class tType>
			inline bool ToInt(tType &nValue, acpl::UInt8 nBase, bool nSwapDecMarks = false)
			{
				bool oSuccess;
				
				if (acpl::Num<tType>::IsSigned() == true)
					nValue = static_cast<tType>(static_cast<acpl::SInt64>(this->ToInt(nBase, sizeof(tType) | 0x80, nSwapDecMarks, oSuccess)));
				else
					nValue = static_cast<tType>(static_cast<acpl::UInt64>(this->ToInt(nBase, sizeof(tType), nSwapDecMarks, oSuccess)));
				
				return oSuccess;
			}
			
			template <class tType>
			inline bool ToFloat(tType &nValue, bool nSwapDecMarks = false)
			{
				bool oSuccess;
				nValue = static_cast<tType>(this->ToFloat(nSwapDecMarks, oSuccess));
				return oSuccess;
			}
		
	};
	
}

#endif // ACPL_STRING_H
