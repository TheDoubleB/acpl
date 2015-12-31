#ifndef ACPL_ARRAY_H
#define ACPL_ARRAY_H

#include "Memory.h"


// NOTE: The following is a custom placement new operator infrastructure so that we won't need to #include <new>
namespace acpl { template <class tType, void (*tDestroyAction)(tType *)> class ArrayBase; }
template <class tType, void (*tDestroyAction)(tType *)> inline void *operator new   (size_t, const acpl::ArrayBase<tType, tDestroyAction> *, tType *nPtr, acpl::SizeT nOff) { return (nPtr + nOff); }
template <class tType, void (*tDestroyAction)(tType *)> inline void  operator delete(void *, const acpl::ArrayBase<tType, tDestroyAction> *, tType *, acpl::SizeT) { }



namespace acpl
{
	
	template <class tType, void (*tDestroyAction)(tType *)>
	class ArrayBase
	{
			acplHideClassCopy(ArrayBase);
		
		private:
			acpl::SizeT mGrowSize;
			tType *mItems;
			acpl::SizeT mItemsSize;
			acpl::SizeT mItemsLen;
			
			inline bool IGrow()
			{
				return (this->mItemsLen < this->mItemsSize || this->Grow(this->mGrowSize) == true);
			}
		
		protected:
			
		
		public:
			ArrayBase(acpl::SizeT nGrowSize = 16)
			{
				this->mGrowSize = nGrowSize;
				this->mItems = NULL;
				this->mItemsSize = 0;
				this->mItemsLen = 0;
			}
			virtual ~ArrayBase()
			{
				this->Purge();
			}
			
			acpl::SizeT Count() const
			{
				return this->mItemsLen;
			}
			
			tType &Get(acpl::SizeT nItemIdx)
			{
				return this->mItems[nItemIdx];
			}
			const tType &Get(acpl::SizeT nItemIdx) const
			{
				return this->mItems[nItemIdx];
			}
			bool Get(acpl::SizeT nItemIdx, tType &nItem)
			{
				if (nItemIdx < this->mItemsLen)
				{
					nItem = this->mItems[nItemIdx];
					return true;
				}
				else
					return false;
			}
			tType &GetFirst()
			{
				return this->mItems[0];
			}
			const tType &GetFirst() const
			{
				return this->mItems[0];
			}
			bool GetFirst(tType &nItem)
			{
				if (this->mItemsLen != 0)
				{
					nItem = this->mItems[0];
					return true;
				}
				else
					return false;
			}
			tType &GetLast()
			{
				return this->mItems[this->mItemsLen - 1];
			}
			const tType &GetLast() const
			{
				return this->mItems[this->mItemsLen - 1];
			}
			bool GetLast(tType &nItem)
			{
				if (this->mItemsLen != 0)
				{
					nItem = this->mItems[this->mItemsLen - 1];
					return true;
				}
				else
					return false;
			}
			
			bool GetIndexOf(const tType &nItem, acpl::SizeT &nIdx) const
			{
				for (nIdx = 0; nIdx < this->mItemsLen; nIdx++)
					if (this->mItems[nIdx] == nItem)
						return true;
				
				return false;
			}
			acpl::SizeT GetIndexOf(const tType &nItem) const
			{
				for (acpl::SizeT i = 0; i < this->mItemsLen; i++)
					if (this->mItems[i] == nItem)
						return i;
				
				return acpl::Num<acpl::SizeT>::Max();
			}
			
// 			bool FindIndexOf(const tType &nKeyValue, acpl::SInt8 (*nComparator)(const tType &nItem1, const tType &nItem2), acpl::SizeT &nIdx) const
// 			{
// 				for (nIdx = 0; nIdx < this->mItemsLen; nIdx++)
// 					if (nComparator(this->mItems[nIdx], nKeyValue) == 0)
// 						return true;
// 				
// 				return false;
// 			}
// 			acpl::SizeT FindIndexOf(const tType &nKeyValue, acpl::SInt8 (*nComparator)(const tType &nItem1, const tType &nItem2)) const
// 			{
// 				acpl::SizeT oIdx;
// 				if (this->FindIndexOf(nKeyValue, nComparator, oIdx) == true)
// 					return oIdx;
// 				else
// 					return acpl::Num<acpl::SizeT>::Max();
// 			}
// 			template <class tKeyType, class tTypeClass>
// 			bool FindIndexOf(const tKeyType &nKeyValue, tKeyType (tTypeClass::*nMethod)(), acpl::SInt8 (*nComparator)(const tKeyType &nItem1, const tKeyType &nItem2), acpl::SizeT &nIdx)
// 			{
// 				for (nIdx = 0; nIdx < this->mItemsLen; nIdx++)
// 					if (nComparator((this->mItems[nIdx].*nMethod)(), nKeyValue) == 0)
// 						return true;
// 				
// 				return false;
// 			}
// 			template <class tKeyType, class tTypeClass>
// 			acpl::SizeT FindIndexOf(const tKeyType &nKeyValue, tKeyType (tTypeClass::*nMethod)(), acpl::SInt8 (*nComparator)(const tKeyType &nItem1, const tKeyType &nItem2))
// 			{
// 				acpl::SizeT oIdx;
// 				if (this->FindIndexOf<tKeyType, tTypeClass>(nKeyValue, nMethod, nComparator, oIdx) == true)
// 					return oIdx;
// 				else
// 					return acpl::Num<acpl::SizeT>::Max();
// 			}
// 			template <class tKeyType, class tTypeClass>
// 			bool FindIndexOf(const tKeyType &nKeyValue, tKeyType (tTypeClass::*nMethod)() const, acpl::SInt8 (*nComparator)(const tKeyType &nItem1, const tKeyType &nItem2), acpl::SizeT &nIdx) const
// 			{
// 				for (nIdx = 0; nIdx < this->mItemsLen; nIdx++)
// 					if (nComparator((this->mItems[nIdx].*nMethod)(), nKeyValue) == 0)
// 						return true;
// 				
// 				return false;
// 			}
// 			template <class tKeyType, class tTypeClass>
// 			acpl::SizeT FindIndexOf(const tKeyType &nKeyValue, tKeyType (tTypeClass::*nMethod)() const, acpl::SInt8 (*nComparator)(const tKeyType &nItem1, const tKeyType &nItem2)) const
// 			{
// 				acpl::SizeT oIdx;
// 				if (this->FindIndexOf<tKeyType, tTypeClass>(nKeyValue, nMethod, nComparator, oIdx) == true)
// 					return oIdx;
// 				else
// 					return acpl::Num<acpl::SizeT>::Max();
// 			}
// 			
// 			bool Find(const tType &nKeyValue, acpl::SInt8 (*nComparator)(const tType &nItem1, const tType &nItem2), tType &nItem)
// 			{
// 				acpl::SizeT oItemIdx;
// 				if (this->FindIndexOf(nKeyValue, nComparator, oItemIdx) == true)
// 				{	
// 					nItem = this->mItems[oItemIdx];
// 					return true;
// 				}
// 				else
// 					return false;
// 			}
// 			template <class tKeyType, class tTypeClass>
// 			bool Find(const tKeyType &nKeyValue, tKeyType (tTypeClass::*nMethod)(), acpl::SInt8 (*nComparator)(const tKeyType &nItem1, const tKeyType &nItem2), tType &nItem)
// 			{
// 				acpl::SizeT oItemIdx;
// 				if (this->FindIndexOf<tKeyType, tTypeClass>(nKeyValue, nMethod, nComparator, oItemIdx) == true)
// 				{
// 					nItem = this->mItems[oItemIdx];
// 					return true;
// 				}
// 				else
// 					return false;
// 			}
// 			template <class tKeyType, class tTypeClass>
// 			bool Find(const tKeyType &nKeyValue, tKeyType (tTypeClass::*nMethod)() const, acpl::SInt8 (*nComparator)(const tKeyType &nItem1, const tKeyType &nItem2), tType &nItem) const
// 			{
// 				acpl::SizeT oItemIdx;
// 				if (this->FindIndexOf<tKeyType, tTypeClass>(nKeyValue, nMethod, nComparator, oItemIdx) == true)
// 				{
// 					nItem = this->mItems[oItemIdx];
// 					return true;
// 				}
// 				else
// 					return false;
// 			}
			
			bool Add()
			{
				return (this->IGrow() == true && (new (this, this->mItems, this->mItemsLen++) tType()) != NULL);
			}
			template <class tCt1>
			bool Add( tCt1  nCa1)
			{
				return (this->IGrow() == true && (new (this, this->mItems, this->mItemsLen++) tType(nCa1)) != NULL);
			}
			template <class tCt1, class tCt2>
			bool Add( tCt1  nCa1, tCt2  nCa2)
			{
				return (this->IGrow() == true && (new (this, this->mItems, this->mItemsLen++) tType(nCa1, nCa2)) != NULL);
			}
			template <class tCt1, class tCt2, class tCt3>
			bool Add( tCt1  nCa1, tCt2  nCa2, tCt3  nCa3)
			{
				return (this->IGrow() == true && (new (this, this->mItems, this->mItemsLen++) tType(nCa1, nCa2, nCa3)) != NULL);
			}
			template <class tCt1, class tCt2, class tCt3, class tCt4>
			bool Add( tCt1  nCa1, tCt2  nCa2, tCt3  nCa3, tCt4  nCa4)
			{
				return (this->IGrow() == true && (new (this, this->mItems, this->mItemsLen++) tType(nCa1, nCa2, nCa3, nCa4)) != NULL);
			}
			template <class tCt1, class tCt2, class tCt3, class tCt4, class tCt5>
			bool Add( tCt1  nCa1, tCt2  nCa2, tCt3  nCa3, tCt4  nCa4, tCt5  nCa5)
			{
				return (this->IGrow() == true && (new (this, this->mItems, this->mItemsLen++) tType(nCa1, nCa2, nCa3, nCa4, nCa5)) != NULL);
			}
			template <class tCt1, class tCt2, class tCt3, class tCt4, class tCt5, class tCt6>
			bool Add( tCt1  nCa1, tCt2  nCa2, tCt3  nCa3, tCt4  nCa4, tCt5  nCa5, tCt6  nCa6)
			{
				return (this->IGrow() == true && (new (this, this->mItems, this->mItemsLen++) tType(nCa1, nCa2, nCa3, nCa4, nCa5, nCa6)) != NULL);
			}
			template <class tCt1, class tCt2, class tCt3, class tCt4, class tCt5, class tCt6, class tCt7>
			bool Add( tCt1  nCa1, tCt2  nCa2, tCt3  nCa3, tCt4  nCa4, tCt5  nCa5, tCt6  nCa6, tCt7  nCa7)
			{
				return (this->IGrow() == true && (new (this, this->mItems, this->mItemsLen++) tType(nCa1, nCa2, nCa3, nCa4, nCa5, nCa6, nCa7)) != NULL);
			}
			template <class tCt1, class tCt2, class tCt3, class tCt4, class tCt5, class tCt6, class tCt7, class tCt8>
			bool Add( tCt1  nCa1, tCt2  nCa2, tCt3  nCa3, tCt4  nCa4, tCt5  nCa5, tCt6  nCa6, tCt7  nCa7, tCt8  nCa8)
			{
				return (this->IGrow() == true && (new (this, this->mItems, this->mItemsLen++) tType(nCa1, nCa2, nCa3, nCa4, nCa5, nCa6, nCa7, nCa8)) != NULL);
			}
			template <class tCt1, class tCt2, class tCt3, class tCt4, class tCt5, class tCt6, class tCt7, class tCt8, class tCt9>
			bool Add( tCt1  nCa1, tCt2  nCa2, tCt3  nCa3, tCt4  nCa4, tCt5  nCa5, tCt6  nCa6, tCt7  nCa7, tCt8  nCa8, tCt9  nCa9)
			{
				return (this->IGrow() == true && (new (this, this->mItems, this->mItemsLen++) tType(nCa1, nCa2, nCa3, nCa4, nCa5, nCa6, nCa7, nCa8, nCa9)) != NULL);
			}
			template <class tCt1, class tCt2, class tCt3, class tCt4, class tCt5, class tCt6, class tCt7, class tCt8, class tCt9, class tCt10>
			bool Add( tCt1  nCa1, tCt2  nCa2, tCt3  nCa3, tCt4  nCa4, tCt5  nCa5, tCt6  nCa6, tCt7  nCa7, tCt8  nCa8, tCt9  nCa9, tCt10 nCa10)
			{
				return (this->IGrow() == true && (new (this, this->mItems, this->mItemsLen++) tType(nCa1, nCa2, nCa3, nCa4, nCa5, nCa6, nCa7, nCa8, nCa9, nCa10)) != NULL);
			}
			
			bool Swap(acpl::SizeT nItem1Idx, acpl::SizeT nItem2Idx)
			{
				if (nItem1Idx < this->mItemsLen && nItem2Idx < this->mItemsLen)
				{
					if (nItem1Idx != nItem2Idx)
					{
						acpl::UInt8 oItem1[sizeof(tType)];
						acpl::Mem::BaseCopy(oItem1, (this->mItems + nItem1Idx), sizeof(tType));
						acpl::Mem::BaseCopy((this->mItems + nItem1Idx), (this->mItems + nItem2Idx), sizeof(tType));
						acpl::Mem::BaseCopy((this->mItems + nItem2Idx), oItem1, sizeof(tType));
					}
					return true;
				}
				else
					return false;
			}
			bool Swap(acpl::SizeT nItemIdx, tType &nSwapInOutItem)
			{
				if (nItemIdx < this->mItemsLen)
				{
					acpl::UInt8 oSwapOutItem[sizeof(tType)];
					acpl::Mem::BaseCopy(oSwapOutItem, (this->mItems + nItemIdx), sizeof(tType));
					acpl::Mem::BaseCopy((this->mItems + nItemIdx), &nSwapInOutItem, sizeof(tType));
					acpl::Mem::BaseCopy(&nSwapInOutItem, oSwapOutItem, sizeof(tType));
					return true;
				}
				else
					return false;
			}
			bool Swap(acpl::SizeT nItemIdx, const tType &nSwapInItem, tType &nSwapOutItem)
			{
				if (nItemIdx < this->mItemsLen)
				{
					nSwapOutItem.~tType();
					acpl::Mem::BaseCopy(&nSwapOutItem, (this->mItems + nItemIdx), sizeof(tType));
					new (this, this->mItems, nItemIdx) tType(nSwapInItem);
					return true;
				}
				else
					return false;
			}
			bool Replace(acpl::SizeT nItemIdx, const tType &nNewItem)
			{
				if (nItemIdx < this->mItemsLen)
				{
					tDestroyAction((this->mItems + nItemIdx));
					this->mItems[nItemIdx].~tType();
					new (this, this->mItems, nItemIdx) tType(nNewItem);
					return true;
				}
				else
					return false;
			}
			bool Discard(acpl::SizeT nItemIdx, tType &nDiscardedItem)
			{
				if (nItemIdx < this->mItemsLen)
				{
					nDiscardedItem.~tType();
					acpl::Mem::BaseCopy(&nDiscardedItem, (this->mItems + nItemIdx), sizeof(tType));
					acpl::Mem::Move((this->mItems + nItemIdx), (this->mItems + nItemIdx + 1), (this->mItemsLen - nItemIdx - 1));
					this->mItemsLen--;
					return true;
				}
				else
					return false;
			}
			bool Remove(acpl::SizeT nItemIdx)
			{
				if (nItemIdx < this->mItemsLen)
				{
					tDestroyAction((this->mItems + nItemIdx));
					this->mItems[nItemIdx].~tType();
					acpl::Mem::Move((this->mItems + nItemIdx), (this->mItems + nItemIdx + 1), (this->mItemsLen - nItemIdx - 1));
					this->mItemsLen--;
					return true;
				}
				else
					return false;
			}
			bool RemoveHead(acpl::SizeT nCount)
			{
				if (nCount == 0)
					return true;
				else
				if (this->mItemsLen != 0)
				{
					if (this->mItemsLen > nCount)
					{
						for (acpl::SizeT i = nCount; i--;)
						{
							tDestroyAction((this->mItems + i));
							this->mItems[i].~tType();
						}
						acpl::Mem::Move(this->mItems, (this->mItems + nCount), (this->mItemsLen - nCount));
						this->mItemsLen -= nCount;
					}
					else
						this->Clear();
					
					return true;
				}
				else
					return false;
			}
			bool RemoveTail(acpl::SizeT nCount)
			{
				if (nCount == 0)
					return true;
				else
				if (this->mItemsLen != 0)
				{
					if (this->mItemsLen > nCount)
					{
						for (acpl::SizeT i = this->mItemsLen, iEnd = (this->mItemsLen - nCount); i-- > iEnd;)
						{
							tDestroyAction((this->mItems + i));
							this->mItems[i].~tType();
						}
						this->mItemsLen -= nCount;
					}
					else
						this->Clear();
					
					return true;
				}
				else
					return false;
			}
			bool Remove(acpl::SizeT nItemIdx, acpl::SizeT nCount)
			{
				if (nCount == 0)
					return true;
				else
				if (nItemIdx == 0)
					return this->RemoveHead(nCount);
				else
				if (nItemIdx < this->mItemsLen)
				{
					acpl::SizeT oToEnd = (this->mItemsLen - nItemIdx);
					
					if (nCount < oToEnd)
					{
						acpl::SizeT oAfterGap = (nItemIdx + nCount);
						for (acpl::SizeT i = oAfterGap; i-- > nItemIdx;)
						{
							tDestroyAction((this->mItems + i));
							this->mItems[i].~tType();
						}
						acpl::Mem::Move((this->mItems + nItemIdx), (this->mItems + oAfterGap), (this->mItemsLen - oAfterGap));
						this->mItemsLen -= nCount;
						
						return true;
					}
					else
						return this->RemoveTail(oToEnd);
				}
				else
					return false;
			}
			bool Move(acpl::SizeT nItemIdx, acpl::SizeT nPositionIdx)
			{
				if (nItemIdx < this->mItemsLen)
				{
					if (nPositionIdx >= this->mItemsLen)
						nPositionIdx = this->mItemsLen - 1;
					
					if (nItemIdx != nPositionIdx)
					{
						acpl::UInt8 oMovingItem[sizeof(tType)];
						acpl::Mem::BaseCopy(oMovingItem, (this->mItems + nItemIdx), sizeof(tType));
						if (nItemIdx > nPositionIdx) // Moving up the list
							acpl::Mem::Move((this->mItems + nPositionIdx + 1), (this->mItems + nPositionIdx), (nItemIdx - nPositionIdx));
						else
						if (nItemIdx < nPositionIdx) // Moving down the list
							acpl::Mem::Move((this->mItems + nItemIdx), (this->mItems + nItemIdx + 1), (nPositionIdx - nItemIdx));
						acpl::Mem::BaseCopy((this->mItems + nPositionIdx), oMovingItem, sizeof(tType));
					}
					return true;
				}
				else
					return false;
			}
			void Sort(acpl::SInt8 (*nComparator)(const tType &nItem1, const tType &nItem2))
			{
				if (this->mItemsLen > 1)
					acpl::Mem::Quicksort<tType>(this->mItems, 0, (this->mItemsLen - 1), nComparator);
			}
 			
			bool Grow(acpl::SizeT nCount)
			{
				if (acpl::Mem::Realloc(this->mItems, (this->mItemsSize + nCount)) == true)
				{
					this->mItemsSize += nCount;
					return true;
				}
				else
					return false;
			}
			bool Shrink()
			{
				if (acpl::Mem::Realloc(this->mItems, this->mItemsLen) == true)
				{
					this->mItemsSize = this->mItemsLen;
					return true;
				}
				else
					return false;
			}
			
			void Clear()
			{
				while (this->mItemsLen--)
				{
					tDestroyAction((this->mItems + this->mItemsLen));
					this->mItems[this->mItemsLen].~tType();
				}
				this->mItemsLen = 0;
			}
			void Purge()
			{
				this->Clear();
				acpl::Mem::Free(this->mItems);
				this->mItemsSize = 0;
			}
		
	};
	
	template <class tType>
	class ArrayEdaNone
	{
		public:
			typedef acpl::ArrayBase<tType, &acpl::Mem::MgrEda<tType>::None> Type;
	};
	
	template <class tType>
	class ArrayEdaFree
	{
		public:
			typedef acpl::ArrayBase<tType, &acpl::Mem::MgrEda<tType>::Free> Type;
	};
	
	template <class tType>
	class ArrayEdaFreeAligned
	{
		public:
			typedef acpl::ArrayBase<tType, &acpl::Mem::MgrEda<tType>::FreeAligned> Type;
	};
	
	template <class tType>
	class ArrayEdaDelete
	{
		public:
			typedef acpl::ArrayBase<tType, &acpl::Mem::MgrEda<tType>::Delete> Type;
	};
	
	template <class tType>
	class ArrayEdaDeleteArray
	{
		public:
			typedef acpl::ArrayBase<tType, &acpl::Mem::MgrEda<tType>::DeleteArray> Type;
	};
	
}

#endif // ACPL_ARRAY_H
