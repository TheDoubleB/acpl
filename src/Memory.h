#ifndef ACPL_MEMORY_H
#define ACPL_MEMORY_H

#include "Io.h"

namespace acpl
{

	class Mem
	{
		public:
			static void *BaseAlloc(acpl::SizeT nSize);
			static void *BaseRealloc(void *nPtr, acpl::SizeT nNewSize);
			static void *BaseResize(void *nPtr, acpl::SizeT nPtrSize, acpl::SizeT nNewSize, acpl::SizeT nMoveOffsetBytes);
			static void BaseFree(void *nPtr);
			
			static void *BaseAllocAligned(acpl::SizeT nAlignBytes, acpl::SizeT nSize);
			static void *BaseReallocAligned(void *nPtr, acpl::SizeT nPtrSize, acpl::SizeT nAlignBytes, acpl::SizeT nNewSize);
			static void *BaseResizeAligned(void *nPtr, acpl::SizeT nPtrSize, acpl::SizeT nAlignBytes, acpl::SizeT nNewSize, acpl::SizeT nMoveOffsetBytes);
			static void BaseFreeAligned(void *nPtr);
			
			static void BaseCopy(void *nDestPtr, const void *nSrcPtr, acpl::SizeT nCountBytes);
			static void BaseMove(void *nDestPtr, const void *nSrcPtr, acpl::SizeT nCountBytes);
			static void BaseSet(void *nPtr, acpl::UInt8 nByte, acpl::SizeT nCountBytes);
			
			
			template <class tType>
			static bool Alloc(tType *&nPtr, acpl::SizeT nCount)
			{
				tType *oPtr = static_cast<tType *>(acpl::Mem::BaseAlloc(nCount * sizeof(tType)));
				
				if (oPtr != NULL)
				{
					nPtr = oPtr;
					return true;
				}
				else
					return false;
			}
			
			template <class tType>
			static bool Realloc(tType *&nPtr, acpl::SizeT nNewCount)
			{
				tType *oPtr = static_cast<tType *>(acpl::Mem::BaseRealloc(nPtr, nNewCount * sizeof(tType)));

				if (oPtr != NULL || nNewCount == 0)
				{
					nPtr = oPtr;
					return true;
				}
				else
					return false;
			}
			
			template <class tType>
			static bool Resize(tType *&nPtr, acpl::SizeT nCurCount, acpl::SizeT nNewCount, acpl::SizeT nMoveOffset)
			{
				tType *oPtr = static_cast<tType *>(acpl::Mem::BaseResize(
						nPtr,
						nCurCount * sizeof(tType),
						nNewCount * sizeof(tType),
						nMoveOffset * sizeof(tType)
					));
				
				if (oPtr != NULL || nNewCount == 0)
				{
					nPtr = oPtr;
					return true;
				}
				else
					return false;
			}
			
			template <class tType>
			static void Free(tType *&nPtr)
			{
				if (nPtr != NULL)
				{
					acpl::Mem::BaseFree(nPtr);
					nPtr = NULL;
				}
			}
			
			
			template <class tType>
			static bool AllocAligned(tType *&nPtr, acpl::SizeT nAlignCount, acpl::SizeT nCount)
			{
				tType *oPtr = static_cast<tType *>(acpl::Mem::BaseAllocAligned(nAlignCount * sizeof(tType), nCount * sizeof(tType)));
				
				if (oPtr != NULL)
				{
					nPtr = oPtr;
					return true;
				}
				else
					return false;
			}
			
			template <class tType>
			static bool ReallocAligned(tType *&nPtr, acpl::SizeT nCurCount, acpl::SizeT nAlignCount, acpl::SizeT nNewCount)
			{
				tType *oPtr = static_cast<tType *>(acpl::Mem::BaseReallocAligned(
						nPtr,
						nCurCount * sizeof(tType),
						nAlignCount * sizeof(tType),
						nNewCount * sizeof(tType)
					));
				
				if (oPtr != NULL || nNewCount == 0)
				{
					nPtr = oPtr;
					return true;
				}
				else
					return false;
			}
			
			template <class tType>
			static bool ResizeAligned(tType *&nPtr, acpl::SizeT nCurCount, acpl::SizeT nAlignCount, acpl::SizeT nNewCount, acpl::SizeT nMoveOffset)
			{
				tType *oPtr = static_cast<tType *>(acpl::Mem::BaseResizeAligned(
						nPtr,
						nCurCount * sizeof(tType),
						nAlignCount * sizeof(tType),
						nNewCount * sizeof(tType),
						nMoveOffset * sizeof(tType)
					));
				
				if (oPtr != NULL || nNewCount == 0)
				{
					nPtr = oPtr;
					return true;
				}
				else
					return false;
			}
			
			template <class tType>
			static void FreeAligned(tType *&nPtr)
			{
				if (nPtr != NULL)
				{
					acpl::Mem::BaseFreeAligned(nPtr);
					nPtr = NULL;
				}
			}
			
			
			template <class tType>
			static void Copy(tType *nDestPtr, const tType *nSrcPtr, acpl::SizeT nCount)
			{
				acpl::Mem::BaseCopy(nDestPtr, nSrcPtr, (nCount * sizeof(tType)));
			}
			
			template <class tType>
			static void Move(tType *nDestPtr, const tType *nSrcPtr, acpl::SizeT nCount)
			{
				acpl::Mem::BaseMove(nDestPtr, nSrcPtr, (nCount * sizeof(tType)));
			}
			
			template <class tType, class tTypeCompat>
			static void Set(tType *nPtr, tTypeCompat nValue, acpl::SizeT nCount)
			{
				while (nCount--)
					nPtr[nCount] = nValue;
			}
			
			template <class tType>
			static void SetByte(tType *nPtr, acpl::UInt8 nByte, acpl::SizeT nCount)
			{
				acpl::Mem::BaseSet(nPtr, nByte, (nCount * sizeof(tType)));
			}
			
			template <class tType>
			static acpl::SInt8 Cmp(const tType *nPtr1, const tType *nPtr2, acpl::SizeT nCount, acpl::SizeT &nDiffIdx)
			{
				for (nDiffIdx = 0; nDiffIdx < nCount; nDiffIdx++)
				{
					if (nPtr1[nDiffIdx] == nPtr2[nDiffIdx])
						continue;
					else
					if (nPtr1[nDiffIdx] < nPtr2[nDiffIdx])
						return -1;
					else
					if (nPtr1[nDiffIdx] > nPtr2[nDiffIdx])
						return 1;
				}
				
				return 0;
			}
			
			template <class tType>
			static acpl::SInt8 Cmp(const tType *nPtr1, const tType *nPtr2, acpl::SizeT nCount)
			{
				acpl::SizeT oDiffIdx;
				return acpl::Mem::Cmp<tType>(nPtr1, nPtr2, nCount, oDiffIdx);
			}
			
			
			template <class tType>
			static void Delete(tType *&nPtr)
			{
				if (nPtr != NULL)
				{
					delete(nPtr);
					nPtr = NULL;
				}
			}
			
			template <class tType>
			static void DeleteArray(tType *&nPtr)
			{
				if (nPtr != NULL)
				{
					delete[](nPtr);
					nPtr = NULL;
				}
			}
			
			
			template <class tType>
			class RaiiFree
			{
					acplHideClassCopyIa(RaiiFree<tType>, nRaiiFree, mRef(nRaiiFree.mRef));
				
				private:
					bool mEnabled;
					tType &mRef;
				
				public:
					RaiiFree(tType &nRef)
						: mEnabled(true), mRef(nRef)
					{}
					~RaiiFree()
					{
						if (this->mEnabled == true)
							acpl::Mem::Free(this->mRef);
					}
					
					void Enable()
					{
						this->mEnabled = true;
					}
					void Disable()
					{
						this->mEnabled = false;
					}
			};
			
			template <class tType>
			class RaiiFreeAligned
			{
					acplHideClassCopyIa(RaiiFreeAligned<tType>, nRaiiFreeAligned, mRef(nRaiiFreeAligned.mRef));
				
				private:
					bool mEnabled;
					tType &mRef;
				
				public:
					RaiiFreeAligned(tType &nRef)
						: mEnabled(true), mRef(nRef)
					{}
					~RaiiFreeAligned()
					{
						if (this->mEnabled == true)
							acpl::Mem::FreeAligned(this->mRef);
					}
					
					void Enable()
					{
						this->mEnabled = true;
					}
					void Disable()
					{
						this->mEnabled = false;
					}
			};
			
			template <class tType>
			class RaiiDelete
			{
					acplHideClassCopyIa(RaiiDelete<tType>, nRaiiDelete, mRef(nRaiiDelete.mRef));
				
				private:
					bool mEnabled;
					tType &mRef;
				
				public:
					RaiiDelete(tType &nRef)
						: mEnabled(true), mRef(nRef)
					{}
					~RaiiDelete()
					{
						if (this->mEnabled == true)
							acpl::Mem::Delete(this->mRef);
					}
					
					void Enable()
					{
						this->mEnabled = true;
					}
					void Disable()
					{
						this->mEnabled = false;
					}

			};
			
			template <class tType>
			class RaiiDeleteArray
			{
					acplHideClassCopyIa(RaiiDeleteArray<tType>, nRaiiDeleteArray, mRef(nRaiiDeleteArray.mRef));
				
				private:
					bool mEnabled;
					tType &mRef;
				
				public:
					RaiiDeleteArray(tType &nRef)
						: mEnabled(true), mRef(nRef)
					{}
					~RaiiDeleteArray()
					{
						if (this->mEnabled == true)
							acpl::Mem::DeleteArray(this->mRef);
					}
					
					void Enable()
					{
						this->mEnabled = true;
					}
					void Disable()
					{
						this->mEnabled = false;
					}

			};
			
			
			template <class tType>
			class MgrEda
			{
					acplHideClassCopy(MgrEda);
				
				public:
					static void None(tType *)
					{

					}
					static void Free(tType *nPtr)
					{
						acpl::Mem::Free(*nPtr);
					}
					static void FreeAligned(tType *nPtr)
					{
						acpl::Mem::FreeAligned(*nPtr);
					}
					static void Delete(tType *nPtr)
					{
						acpl::Mem::Delete(*nPtr);
					}
					static void DeleteArray(tType *nPtr)
					{
						acpl::Mem::DeleteArray(*nPtr);
					}
			};
			
			template <class tType>
			class MgrBlock
			{
				public:
					static bool Resize(tType *&nPtr, acpl::SizeT nCurCount, acpl::SizeT nNewCount, acpl::SizeT nMoveOffset)
					{
						return acpl::Mem::Resize(nPtr, nCurCount, nNewCount, nMoveOffset);
					}
					static void Free(tType *&nPtr)
					{
						acpl::Mem::Free(nPtr);
					}
			};
			
			template <class tType, acpl::SizeT tAlignCount>
			class MgrBlockAligned
			{
				public:
					static bool Resize(tType *&nPtr, acpl::SizeT nCurCount, acpl::SizeT nNewCount, acpl::SizeT nMoveOffset)
					{
						return acpl::Mem::ResizeAligned(nPtr, nCurCount, tAlignCount, nNewCount + tAlignCount, nMoveOffset);
					}
					static void Free(tType *&nPtr)
					{
						acpl::Mem::FreeAligned(nPtr);
					}
			};
			
			
			template <class tType, class tAlloc>
			class BlockBase
			{
					acplHideClassCopy(BlockBase);
				
				private:
					acpl::SizeT mGrowCount;
					tType *mDataBase;
					tType *mDataCur;
					tType *mDataEnd;
					acpl::SizeT mLen;
					
					inline bool EnsureCount(acpl::SizeT nCount)
					{
						if ((this->mDataCur + nCount) <= this->mDataEnd)
							return true;
						else
						{
							acpl::SizeT oNewSize = (nCount + this->mGrowCount);
							if (tAlloc::Resize(
									this->mDataBase,
									(this->mDataEnd - this->mDataBase), // cur size
									oNewSize,
									(this->mDataCur - this->mDataBase)) == true)
							{
								this->mDataCur = this->mDataBase;
								this->mDataEnd = (this->mDataBase + oNewSize);
								return true;
							}
							else
							{
								return false;
							}
						}
					}
				
				public:
					BlockBase(acpl::SizeT nGrowCount = 256)
					{
						this->mGrowCount = nGrowCount;
						this->mDataBase = NULL;
						this->mDataCur = NULL;
						this->mDataEnd = NULL;
						this->mLen = 0;
					}
					virtual ~BlockBase()
					{
						this->Purge();
						this->mGrowCount = 0;
					}
					
					bool Apnd(const tType *nData, acpl::SizeT nCount)
					{
						acpl::SizeT oLen = (this->mLen + nCount);
						if (this->EnsureCount(oLen) == false)
							return false;
						
						acpl::Mem::Copy((this->mDataCur + this->mLen), nData, nCount);
						this->mLen = oLen;
						
						return true;
					}
					bool Apnd(acpl::IoChannel *nIoChan, acpl::SizeT nCount, acpl::SizeT &nCountAppended)
					{
						acpl::SizeT oLen = (this->mLen + nCount);
						if (this->EnsureCount(oLen) == false)
							return false;
						
						if (nIoChan->Read((this->mDataCur + this->mLen), sizeof(tType) * nCount, nCountAppended) == false)
							return false;
						
						nCountAppended /= sizeof(tType);
						this->mLen += nCountAppended;
						return true;
					}
					bool Apnd(acpl::IoChannel *nIoChan, acpl::SizeT nCount)
					{
						acpl::SizeT oAppended = 0;
						return this->Apnd(nIoChan, nCount, oAppended);
					}
					bool Ins(acpl::SizeT nIndex, const tType *nData, acpl::SizeT nCount)
					{
						acpl::SizeT oLen = (this->mLen + nCount);
						if (this->EnsureCount(oLen) == false)
							return false;
						
						tType *oDataIdx = (this->mDataCur + nIndex);
						acpl::Mem::Move((oDataIdx + nCount), oDataIdx, (this->mLen - nIndex));
						acpl::Mem::Copy(oDataIdx, nData, nCount);
						this->mLen = oLen;
						return true;
					}
					bool Ins(acpl::SizeT nIndex, acpl::IoChannel *nIoChan, acpl::SizeT nCount, acpl::SizeT &nCountInserted)
					{
						acpl::SizeT oLen = (this->mLen + nCount);
						if (this->EnsureCount(oLen) == false)
							return false;
						
						acpl::SizeT oResidual = (this->mLen - nIndex);
						tType *oDataIdx = (this->mDataCur + nIndex);
						
						acpl::Mem::Move((oDataIdx + nCount), oDataIdx, oResidual);
						if (nIoChan->Read(oDataIdx, sizeof(tType) * nCount, nCountInserted) == true)
						{
							nCountInserted /= sizeof(tType);
							if (nCountInserted != nCount)
								acpl::Mem::Move((oDataIdx + nCountInserted), (oDataIdx + nCount), oResidual);
							
							this->mLen += nCountInserted;
							return true;
						}
						else
						{
							acpl::Mem::Move(oDataIdx, (oDataIdx + nCount), oResidual);
							return false;
						}
					}
					bool Ins(acpl::SizeT nIndex, acpl::IoChannel *nIoChan, acpl::SizeT nCount)
					{
						acpl::SizeT oInserted = 0;
						return this->Ins(nIndex, nIoChan, nCount, oInserted);
					}
					bool Fill(acpl::SizeT nIndex, tType nValue, acpl::SizeT nCount)
					{
						acpl::SizeT oLen = (this->mLen + nCount);
						if (this->EnsureCount(oLen) == false)
							return false;
						
						tType *oDataIdx = (this->mDataCur + nIndex);
						acpl::Mem::Move((oDataIdx + nCount), oDataIdx, this->mLen - nIndex);
						acpl::Mem::Set(oDataIdx, nValue, nCount);
						this->mLen = oLen;
						return true;
					}
					bool FillByte(acpl::SizeT nIndex, acpl::UInt8 nByte, acpl::SizeT nCount)
					{
						acpl::SizeT oLen = (this->mLen + nCount);
						if (this->EnsureCount(oLen) == false)
							return false;
						
						tType *oDataIdx = (this->mDataCur + nIndex);
						acpl::Mem::Move((oDataIdx + nCount), oDataIdx, this->mLen - nIndex);
						acpl::Mem::SetByte(oDataIdx, nByte, nCount);
						this->mLen = oLen;
						return true;
					}
					bool Grow(acpl::SizeT nCount)
					{
						acpl::SizeT oCurSize = (this->mDataEnd - this->mDataBase);
						acpl::SizeT oNewSize = (oCurSize + nCount);
						if (tAlloc::Resize(this->mDataBase, oCurSize, oNewSize, (this->mDataCur - this->mDataBase)) == false)
							return false;
						
						this->mDataCur = this->mDataBase;
						this->mDataEnd = (this->mDataBase + oNewSize);
						return true;
					}
					bool Shrink()
					{
						acpl::SizeT oCurSize = (this->mDataEnd - this->mDataBase);
						if (oCurSize == this->mLen)
							return true;
						
						if (tAlloc::Resize(this->mDataBase, oCurSize, this->mLen, (this->mDataCur - this->mDataBase)) == false)
							return false;
						
						this->mDataCur = this->mDataBase;
						this->mDataEnd = (this->mDataBase + this->mLen);
						return true;
					}
					acpl::SizeT Length() const
					{
						return this->mLen;
					}
					bool SetLength(acpl::SizeT nLen)
					{
						if (this->EnsureCount(nLen) == false)
							return false;
						
						this->mLen = nLen;
						return true;
					}
					const tType *Data(acpl::SizeT &nCount) const
					{
						nCount = this->mLen;
						return this->mDataCur;
					}
					tType *Data(acpl::SizeT &nCount)
					{
						nCount = this->mLen;
						return this->mDataCur;
					}
					const tType *Data() const
					{
						return this->mDataCur;
					}
					tType *Data()
					{
						return this->mDataCur;
					}
					acpl::Mem::BlockBase<tType, tAlloc> &Clear()
					{
						this->mDataCur = this->mDataBase;
						this->mLen = 0;
						return *this;
					}
					acpl::Mem::BlockBase<tType, tAlloc> &Purge()
					{
						tAlloc::Free(this->mDataBase);
						this->mDataCur = NULL;
						this->mDataEnd = NULL;
						this->mLen = 0;
						return *this;
					}
					acpl::Mem::BlockBase<tType, tAlloc> &Cut(acpl::SizeT nOffset, acpl::SizeT nCount)
					{
						if (nCount == 0)
							return *this;
						else
						if (nOffset == 0)
							return this->CutLeft(nCount);
						else
						if (nOffset < this->mLen)
						{
							acpl::SizeT oToEnd = (this->mLen - nOffset);
							
							if (nCount < oToEnd)
							{
								tType *oDataOff = (this->mDataCur + nOffset);
								acpl::Mem::Move(oDataOff, (oDataOff + nCount), (oToEnd - nCount));
								this->mLen -= nCount;
							}
							else
								return this->CutRight(oToEnd);
						}
						
						return *this;
					}
					acpl::Mem::BlockBase<tType, tAlloc> &CutLeft(acpl::SizeT nCount)
					{
						if (this->mLen > nCount)
						{
							this->mDataCur += nCount;
							this->mLen -= nCount;
						}
						else
						{
							this->mDataCur = this->mDataBase;
							this->mLen = 0;
						}
						
						return *this;
					}
					acpl::Mem::BlockBase<tType, tAlloc> &CutRight(acpl::SizeT nCount)
					{
						if (this->mLen > nCount)
							this->mLen -= nCount;
						else
						{
							this->mDataCur = this->mDataBase;
							this->mLen = 0;
						}
						
						return *this;
					}
			};
			
			template <class tType>
			class Block
			{
				public:
					typedef acpl::Mem::BlockBase<tType, acpl::Mem::MgrBlock<tType> > Type;
			};
			
			template <class tType, acpl::SizeT tAlignCount = (4 * sizeof(void *) / sizeof(tType))>
			class BlockAligned
			{
				public:
					typedef acpl::Mem::BlockBase<tType, acpl::Mem::MgrBlockAligned<tType, tAlignCount> > Type;
			};
			
			class ByteBlock
			{
				public:
					typedef acpl::Mem::Block<acpl::UInt8>::Type Type;
			};
			
			class ByteBlockIo : public acpl::IoChannel
			{
					acplHideClassCopyI(ByteBlockIo, acpl::IoChannel());
				
				private:
					acpl::OffT mOff;
					acpl::Mem::ByteBlock::Type *mBlock;
				
				public:
					inline ByteBlockIo() : mOff(0), mBlock(NULL) { }
					inline virtual ~ByteBlockIo() { this->Close(); }
					
					inline bool IsOpen() const { return (this->mBlock != NULL); }
					bool Open(acpl::Mem::ByteBlock::Type *nByteBlock);
					void Close();
					
					inline acpl::OffT GetSize() const { return ((this->IsOpen() == true) ? static_cast<acpl::OffT>(this->mBlock->Length()) : -1); }
					bool GetSize(acpl::OffT &nSize) const;
					bool SetSize(acpl::OffT nSize);
					inline acpl::OffT GetOffset() const { return ((this->IsOpen() == true) ? this->mOff : -1); }
					inline bool GetOffset(acpl::OffT &nOffset) const { nOffset = this->mOff; return this->IsOpen(); }
					bool SetOffset(acpl::OffT nOffset, acpl::Mem::ByteBlockIo::Whence nWhence = acpl::Mem::ByteBlockIo::whStart);
					
					virtual bool Write(const void *nData, acpl::SizeT nDataSize, acpl::SizeT &nBytesWritten);
					virtual bool Read(void *nData, acpl::SizeT nDataSize, acpl::SizeT &nBytesRead);
			};
			
			
			template <class tType>
			static acpl::SInt8 SortCmpFw(const tType &nItem1, const tType &nItem2)
			{
				if (nItem1 < nItem2)
					return -1;
				else
				if (nItem1 > nItem2)
					return 1;
				else
					return 0;
			}
			
			template <class tType>
			static acpl::SInt8 SortCmpBw(const tType &nItem1, const tType &nItem2)
			{
				if (nItem1 < nItem2)
					return 1;
				else
				if (nItem1 > nItem2)
					return -1;
				else
					return 0;
			}
			
			template <class tType>
			static void Quicksort(tType *nElements, acpl::SizeT nFirstOffset, acpl::SizeT nLastOffset, acpl::SInt8 (*nComparator)(const tType &, const tType &))
			{
				if (nLastOffset > nFirstOffset)
				{
					acpl::SizeT oPivotOffset = ((nLastOffset - nFirstOffset) / 2) + nFirstOffset;
					acpl::SizeT oLeftSwapOffset = nFirstOffset;
					if (oPivotOffset > nFirstOffset + 2)
						oLeftSwapOffset = oPivotOffset - 2;
					
					acpl::SizeT oRightSwapOffset = nLastOffset;
					if (oPivotOffset < nLastOffset - 2)
						oRightSwapOffset = oPivotOffset + 2;

					int oPivotMove = 0;
					bool oSwitch;
					acpl::UInt8 oTemp[sizeof(tType)];
					bool oDidIt = false;
					
					do
					{
						// Compare
						oPivotMove = 0;
						oSwitch = false;
						if (oPivotOffset > nFirstOffset)
						{
							if (nComparator(nElements[oPivotOffset - 1], nElements[oPivotOffset]) == 1)
							{
								oPivotMove--;
								oSwitch = true;
							}
						}

						if (oPivotOffset < nLastOffset)
						{
							if (nComparator(nElements[oPivotOffset + 1], nElements[oPivotOffset]) == -1)
							{
								oPivotMove++;
								oSwitch = true;
							}
						}

						// Switch
						if (oSwitch)
						{
							switch (oPivotMove)
							{
								case 0:
									acpl::Mem::BaseCopy(oTemp, (nElements + oPivotOffset + 1), sizeof(tType));
									acpl::Mem::BaseCopy((nElements + oPivotOffset + 1), (nElements + oPivotOffset - 1), sizeof(tType));
									acpl::Mem::BaseCopy((nElements + oPivotOffset - 1), oTemp, sizeof(tType));
									break;

								case 1:
									acpl::Mem::BaseCopy(oTemp, (nElements + oPivotOffset + 1), sizeof(tType));
									acpl::Mem::BaseCopy((nElements + oPivotOffset + 1), (nElements + oPivotOffset), sizeof(tType));
									acpl::Mem::BaseCopy((nElements + oPivotOffset), oTemp, sizeof(tType));
									break;

								case -1:
									acpl::Mem::BaseCopy(oTemp, (nElements + oPivotOffset - 1), sizeof(tType));
									acpl::Mem::BaseCopy((nElements + oPivotOffset - 1), (nElements + oPivotOffset), sizeof(tType));
									acpl::Mem::BaseCopy((nElements + oPivotOffset), oTemp, sizeof(tType));
									break;
							}
						}

						// Swap
						oPivotOffset += oPivotMove;

						oDidIt = false;

						if (oLeftSwapOffset >= nFirstOffset)
						{
							if (oLeftSwapOffset != oPivotOffset - 1 && oPivotOffset > nFirstOffset)
							{
								acpl::Mem::BaseCopy(oTemp, (nElements + oPivotOffset - 1), sizeof(tType));
								acpl::Mem::BaseCopy((nElements + oPivotOffset - 1), (nElements + oLeftSwapOffset), sizeof(tType));
								acpl::Mem::BaseCopy((nElements + oLeftSwapOffset), oTemp, sizeof(tType));
							}

							if (oLeftSwapOffset > 0)
							{
								oLeftSwapOffset--;
								oDidIt = true;
							}
						}

						if (oRightSwapOffset <= nLastOffset)
						{
							if (oRightSwapOffset != oPivotOffset + 1 && oPivotOffset < nLastOffset)
							{
								acpl::Mem::BaseCopy(oTemp, (nElements + oPivotOffset + 1), sizeof(tType));
								acpl::Mem::BaseCopy((nElements + oPivotOffset + 1), (nElements + oRightSwapOffset), sizeof(tType));
								acpl::Mem::BaseCopy((nElements + oRightSwapOffset), oTemp, sizeof(tType));
							}

							oRightSwapOffset++;
							oDidIt = true;
						}
					}
					while(oDidIt == true);

					if (oPivotOffset > (nFirstOffset + 1))
					{
						acpl::Mem::Quicksort<tType>(nElements, nFirstOffset, (oPivotOffset - 1), nComparator);
					}

					if ((oPivotOffset + 1) < nLastOffset)
					{
						acpl::Mem::Quicksort<tType>(nElements, (oPivotOffset + 1), nLastOffset, nComparator);
					}
				}
			}
	};

}

#endif // ACPL_MEMORY_H
