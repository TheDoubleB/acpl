#include "Memory.h"

#include "_cp/Memory.inc.cpp"


// 
// Base allocation (non-aligned)
// 

void *acpl::Mem::BaseAlloc(acpl::SizeT nSize)
{
	return acpl::cp::_mem_alloc(nSize);
}

void *acpl::Mem::BaseRealloc(void *nPtr, acpl::SizeT nNewSize)
{
	return acpl::cp::_mem_realloc(nPtr, nNewSize);
}

void *acpl::Mem::BaseResize(void *nPtr, acpl::SizeT nPtrSize, acpl::SizeT nNewSize, acpl::SizeT nMoveOffsetBytes)
{
	if (nPtr != NULL && nNewSize != 0 && nMoveOffsetBytes < nPtrSize)
	{
		acpl::cp::_mem_move(
				nPtr,
				static_cast<const acpl::UInt8 *>(nPtr) + nMoveOffsetBytes,
				((nPtrSize - nMoveOffsetBytes) < nNewSize) ? (nPtrSize - nMoveOffsetBytes) : nNewSize
			);
	}
	
	return acpl::cp::_mem_realloc(nPtr, nNewSize);
}

void acpl::Mem::BaseFree(void *nPtr)
{
	acpl::cp::_mem_free(nPtr);
}


// 
// Base allocation (aligned)
// 

void *acpl::Mem::BaseAllocAligned(acpl::SizeT nAlignBytes, acpl::SizeT nSize)
{
	return acpl::cp::_mem_alloc_aligned(nAlignBytes, nSize);
}

void *acpl::Mem::BaseReallocAligned(void *nPtr, acpl::SizeT nPtrSize, acpl::SizeT nAlignBytes, acpl::SizeT nNewSize)
{
	void *oPtr = NULL;
	
	if (nNewSize != 0)
	{
		if ((oPtr = acpl::cp::_mem_alloc_aligned(nAlignBytes, nNewSize)) != NULL)
		{
			if (nPtr != NULL)
			{
				acpl::cp::_mem_copy(oPtr, nPtr, (nNewSize < nPtrSize) ? nNewSize : nPtrSize);
				acpl::cp::_mem_free_aligned(nPtr);
			}
		}
	}
	else
	{
		if (nPtr != NULL)
			acpl::cp::_mem_free_aligned(nPtr);
	}
	
	return oPtr;
}

void *acpl::Mem::BaseResizeAligned(void *nPtr, acpl::SizeT nPtrSize, acpl::SizeT nAlignBytes, acpl::SizeT nNewSize, acpl::SizeT nMoveOffsetBytes)
{
	void *oPtr = NULL;
	
	if (nNewSize != 0)
	{
		if ((oPtr = acpl::cp::_mem_alloc_aligned(nAlignBytes, nNewSize)) != NULL)
		{
			if (nPtr != NULL)
			{
				if (nMoveOffsetBytes < nPtrSize)
				{
					acpl::cp::_mem_copy(oPtr,
									  static_cast<const acpl::UInt8 *>(nPtr) + nMoveOffsetBytes,
									  ((nPtrSize - nMoveOffsetBytes) < nNewSize) ? (nPtrSize - nMoveOffsetBytes) : nNewSize);
				}
				
				acpl::cp::_mem_free_aligned(nPtr);
			}
		}
	}
	else
	{
		if (nPtr != NULL)
			acpl::cp::_mem_free_aligned(nPtr);
	}
	
	return oPtr;
}

void acpl::Mem::BaseFreeAligned(void *nPtr)
{
	acpl::cp::_mem_free_aligned(nPtr);
}


// 
// Base movement
// 

void acpl::Mem::BaseCopy(void *nDestPtr, const void *nSrcPtr, acpl::SizeT nCountBytes)
{
	acpl::cp::_mem_copy(nDestPtr, nSrcPtr, nCountBytes);
}

void acpl::Mem::BaseMove(void *nDestPtr, const void *nSrcPtr, acpl::SizeT nCountBytes)
{
	acpl::cp::_mem_move(nDestPtr, nSrcPtr, nCountBytes);
}

void acpl::Mem::BaseSet(void *nPtr, acpl::UInt8 nByte, acpl::SizeT nCountBytes)
{
	acpl::cp::_mem_set(nPtr, nByte, nCountBytes);
}



//
// ByteBlockIo
//

// PUBLIC

bool acpl::Mem::ByteBlockIo::Open(acpl::Mem::ByteBlock::Type *nByteBlock)
{
	if (this->IsOpen() != true && nByteBlock != NULL)
	{
		this->mOff = 0;
		this->mBlock = nByteBlock;
		return true;
	}
	else
		return false;
}

void acpl::Mem::ByteBlockIo::Close()
{
	this->mOff = 0;
	this->mBlock = NULL;
}


bool acpl::Mem::ByteBlockIo::GetSize(acpl::OffT &nSize) const
{
	if (this->IsOpen() == true)
	{
		nSize = this->mBlock->Length();
		return true;
	}
	else
	{
		nSize = 0;
		return false;
	}
}

bool acpl::Mem::ByteBlockIo::SetSize(acpl::OffT nSize)
{
	if (this->IsOpen() == true)
		return this->mBlock->SetLength(((nSize > 0) ? static_cast<acpl::SizeT>(nSize) : 0));
	else
		return false;
}

bool acpl::Mem::ByteBlockIo::SetOffset(acpl::OffT nOffset, acpl::Mem::ByteBlockIo::Whence nWhence)
{
	if (this->IsOpen() != true)
		return false;
	
	acpl::OffT oRealPos = 0;
	
	switch (nWhence)
	{
		case acpl::Mem::ByteBlockIo::whStart:
			oRealPos = nOffset;
			break;
		case acpl::Mem::ByteBlockIo::whCurrent:
			oRealPos = (this->mOff + nOffset);
			break;
		case acpl::Mem::ByteBlockIo::whEnd:
			oRealPos = (this->mBlock->Length() + nOffset);
			break;
	}
	
	if (oRealPos < 0)
		return false;
	
	this->mOff = oRealPos;
	
	return true;
}


bool acpl::Mem::ByteBlockIo::Write(const void *nData, acpl::SizeT nDataSize, acpl::SizeT &nBytesWritten)
{
	nBytesWritten = 0;
	
	if (this->IsOpen() != true)
		return false;
	
	if (nDataSize == 0)
		return true;
	
	acpl::SizeT oNeedSize = (static_cast<acpl::SizeT>(this->mOff) + nDataSize);
	acpl::SizeT oCurSize = this->mBlock->Length();
	if (oNeedSize > oCurSize)
	{
		if (this->mBlock->SetLength(oNeedSize) == true)
			acpl::Mem::Set(&this->mBlock->Data()[oCurSize], 0, oNeedSize - oCurSize);
		else
			return false;
	}
	
	acpl::Mem::Copy(&this->mBlock->Data()[this->mOff], static_cast<const acpl::UInt8 *>(nData), nDataSize);
	this->mOff += nDataSize;
	
	nBytesWritten = nDataSize;
	
	return true;
}

bool acpl::Mem::ByteBlockIo::Read(void *nData, acpl::SizeT nDataSize, acpl::SizeT &nBytesRead)
{
	nBytesRead = 0;
	
	if (this->IsOpen() != true)
		return false;
	
	if (nDataSize == 0 || static_cast<acpl::SizeT>(this->mOff) >= this->mBlock->Length())
		return true;
	
	nBytesRead = (this->mBlock->Length() - static_cast<acpl::SizeT>(this->mOff));
	if (nBytesRead > nDataSize)
		nBytesRead = nDataSize;
	
	acpl::Mem::Copy(static_cast<acpl::UInt8 *>(nData), &this->mBlock->Data()[this->mOff], nBytesRead);
	this->mOff += nBytesRead;
	
	return true;
}
