#include "_i18n/charsets/Charsets.inc.cpp"

// NOTE: Method acpl::Charsets::GetCharset is implemented in "_i18n/charsets/Charsets.cpp" rather than here


acpl::Charsets::ByteOrderMask acpl::Charsets::DecodeBom(const acpl::UInt8 *nBfr, acpl::SizeT nBfrSize, acpl::SizeT &nBomByteSize) // static
{
	nBomByteSize = 0;
	
	
	if (nBfrSize < 2)
		return bomNone;
	
	
	if (nBfr[0] == 0xFE && nBfr[1] == 0xFF)
	{
		nBomByteSize = 2;
		return bom2ByteBe;
	}
	else
	if (nBfr[0] == 0xFF && nBfr[1] == 0xFE)
	{
		nBomByteSize = 2;
		return bom2ByteLe;
	}
	
	
	if (nBfrSize < 3)
		return bomNone;
	
	
	if (nBfr[0] == 0xEF && nBfr[1] == 0xBB && nBfr[2] == 0xBF)
	{
		nBomByteSize = 3;
		return bom1Byte;
	}
	
	
	if (nBfrSize < 4)
		return bomNone;
	
	
	if (nBfr[0] == 0x00 && nBfr[1] == 0x00 && nBfr[2] == 0xFE && nBfr[3] == 0xFF)
	{
		nBomByteSize = 4;
		return bom4ByteBe;
	}
	else
	if (nBfr[0] == 0x00 && nBfr[1] == 0x00 && nBfr[2] == 0xFF && nBfr[3] == 0xFE)
	{
		nBomByteSize = 4;
		return bom4ByteLe;
	}
	
	
	return bomNone;
}



// 
// Charsets::Fpc
// 

acpl::Charsets::Fpc::Fpc()
{
	this->mIsCompatible = NULL;
	this->mGetMimeName = NULL;
	this->mIsBomSuggested = NULL;
	this->mCreateBom = NULL;
	this->mMinSeqSize = NULL;
	this->mMaxSeqSize = NULL;
	this->mDecode = NULL;
	this->mEncode = NULL;
}


// PUBLIC

void acpl::Charsets::Fpc::Set(
		fpIsCompatible nIsCompatible,
		fpGetMimeName nGetMimeName,
		fpIsBomSuggested nIsBomSuggested,
		fpCreateBom nCreateBom,
		fpMinSeqSize nMinSeqSize,
		fpMaxSeqSize nMaxSeqSize,
		fpDecode nDecode,
		fpEncode nEncode
	)
{
	this->mIsCompatible = nIsCompatible;
	this->mGetMimeName = nGetMimeName;
	this->mIsBomSuggested = nIsBomSuggested;
	this->mCreateBom = nCreateBom;
	this->mMinSeqSize = nMinSeqSize;
	this->mMaxSeqSize = nMaxSeqSize;
	this->mDecode = nDecode;
	this->mEncode = nEncode;
}
