#ifndef ACPL_CHARSETS_H
#define ACPL_CHARSETS_H

#include "Memory.h"

namespace acpl
{
	class Charsets
	{
			acplHideClassCopy(Charsets);
		
		public:
			enum ByteOrderMask
			{
				bomNone,
				bom1Byte,
				bom2ByteBe,
				bom2ByteLe,
				bom4ByteBe,
				bom4ByteLe
			};
			
			class Fpc
			{
				acplHideClassCopy(Fpc);
				
				public:
					typedef bool (*fpIsCompatible)(const char *nMimeName, acpl::Charsets::ByteOrderMask nBom, acpl::Charsets::Fpc &nFpc);
					typedef const char *(*fpGetMimeName)();
					typedef bool (*fpIsBomSuggested)();
					typedef acpl::SizeT (*fpCreateBom)(acpl::UInt8 *nBfr, acpl::SizeT nBfrSize);
					typedef acpl::SizeT (*fpMinSeqSize)();
					typedef acpl::SizeT (*fpMaxSeqSize)();
					typedef acpl::SizeT (*fpDecode)(const acpl::UInt8 *nBfr, acpl::SizeT nBfrSize, acpl::Unichar &nUnicodeChar);
					typedef acpl::SizeT (*fpEncode)(acpl::Unichar nUnicodeChar, acpl::UInt8 *nBfr, acpl::SizeT nBfrSize);
				
				private:
					fpIsCompatible mIsCompatible;
					fpGetMimeName mGetMimeName;
					fpIsBomSuggested mIsBomSuggested;
					fpCreateBom mCreateBom;
					fpMinSeqSize mMinSeqSize;
					fpMaxSeqSize mMaxSeqSize;
					fpDecode mDecode;
					fpEncode mEncode;
				
				public:
					Fpc();
					
					void Set(
							fpIsCompatible nIsCompatible,
							fpGetMimeName nGetMimeName,
							fpIsBomSuggested nIsBomSuggested,
							fpCreateBom nCreateBom,
							fpMinSeqSize nMinSeqSize,
							fpMaxSeqSize nMaxSeqSize,
							fpDecode nDecode,
							fpEncode nEncode
						);
					
					inline fpIsCompatible	IsCompatible()		{ return this->mIsCompatible; }
					inline fpGetMimeName	GetMimeName()		{ return this->mGetMimeName; }
					inline fpIsBomSuggested	IsBomSuggested()	{ return this->mIsBomSuggested; }
					inline fpCreateBom		CreateBom()			{ return this->mCreateBom; }
					inline fpMinSeqSize		MinSeqSize()		{ return this->mMinSeqSize; }
					inline fpMaxSeqSize		MaxSeqSize()		{ return this->mMaxSeqSize; }
					inline fpDecode			Decode()			{ return this->mDecode; }
					inline fpEncode			Encode()			{ return this->mEncode; }
				
			};
		
		public:
			static bool GetCharset(const char *nMimeName, acpl::Charsets::ByteOrderMask nBom, acpl::Charsets::Fpc &nFpc);
			static acpl::Charsets::ByteOrderMask DecodeBom(const acpl::UInt8 *nBfr, acpl::SizeT nBfrSize, acpl::SizeT &nBomByteSize);
	};
}

#endif // ACPL_CHARSETS_H
