#include "Tests.h"

#include "../src/Array.h"


class _local_ArrayElemNorm
{
	private:
		acpl::UInt32 mInt;
	
	public:
		_local_ArrayElemNorm()
		{
			this->mInt = 0;
		}
		_local_ArrayElemNorm(const _local_ArrayElemNorm &nArrayNorm)
		{
			this->mInt = nArrayNorm.mInt;
		}
		_local_ArrayElemNorm(acpl::UInt32 nInt)
		{
			this->mInt = nInt;
		}
		_local_ArrayElemNorm(acpl::UInt32 nInt1, acpl::UInt8 nInt2)
		{
			this->mInt = nInt1 + nInt2;
		}
		_local_ArrayElemNorm(acpl::UInt32 nInt1, acpl::UInt8 nInt2, acpl::UInt16 nInt3)
		{
			this->mInt = nInt1 + nInt2 + nInt3;
		}
		_local_ArrayElemNorm(acpl::UInt32 nInt1, acpl::UInt8 nInt2, acpl::UInt16 nInt3, acpl::UInt64 nInt4)
		{
			this->mInt = nInt1 + nInt2 + nInt3 + static_cast<acpl::UInt32>(nInt4);
		}
		_local_ArrayElemNorm(acpl::UInt32 nInt1, acpl::UInt8 nInt2, acpl::UInt16 nInt3, acpl::UInt64 nInt4, acpl::SInt32 nInt5)
		{
			this->mInt = nInt1 + nInt2 + nInt3 + static_cast<acpl::UInt32>(nInt4) + nInt5;
		}
		_local_ArrayElemNorm(acpl::UInt32 nInt1, acpl::UInt8 nInt2, acpl::UInt16 nInt3, acpl::UInt64 nInt4, acpl::SInt32 nInt5, acpl::SInt8 nInt6)
		{
			this->mInt = nInt1 + nInt2 + nInt3 + static_cast<acpl::UInt32>(nInt4) + nInt5 + nInt6;
		}
		_local_ArrayElemNorm(acpl::UInt32 nInt1, acpl::UInt8 nInt2, acpl::UInt16 nInt3, acpl::UInt64 nInt4, acpl::SInt32 nInt5, acpl::SInt8 nInt6, acpl::SInt16 nInt7)
		{
			this->mInt = nInt1 + nInt2 + nInt3 + static_cast<acpl::UInt32>(nInt4) + nInt5 + nInt6 + nInt7;
		}
		_local_ArrayElemNorm(acpl::UInt32 nInt1, acpl::UInt8 nInt2, acpl::UInt16 nInt3, acpl::UInt64 nInt4, acpl::SInt32 nInt5, acpl::SInt8 nInt6, acpl::SInt16 nInt7, acpl::SInt64 nInt8)
		{
			this->mInt = nInt1 + nInt2 + nInt3 + static_cast<acpl::UInt32>(nInt4) + nInt5 + nInt6 + nInt7 + static_cast<acpl::UInt32>(nInt8);
		}
		_local_ArrayElemNorm(acpl::UInt32 nInt1, acpl::UInt8 nInt2, acpl::UInt16 nInt3, acpl::UInt64 nInt4, acpl::SInt32 nInt5, acpl::SInt8 nInt6, acpl::SInt16 nInt7, acpl::SInt64 nInt8, acpl::Float64 nFloat1)
		{
			this->mInt = nInt1 + nInt2 + nInt3 + static_cast<acpl::UInt32>(nInt4) + nInt5 + nInt6 + nInt7 + static_cast<acpl::UInt32>(nInt8) + static_cast<acpl::SInt32>(nFloat1);
		}
		_local_ArrayElemNorm(acpl::UInt32 nInt1, acpl::UInt8 nInt2, acpl::UInt16 nInt3, acpl::UInt64 nInt4, acpl::SInt32 nInt5, acpl::SInt8 nInt6, acpl::SInt16 nInt7, acpl::SInt64 nInt8, acpl::Float64 nFloat1, acpl::Float32 nFloat2)
		{
			this->mInt = nInt1 + nInt2 + nInt3 + static_cast<acpl::UInt32>(nInt4) + nInt5 + nInt6 + nInt7 + static_cast<acpl::UInt32>(nInt8) + static_cast<acpl::SInt32>(nFloat1) + static_cast<acpl::SInt32>(nFloat2);
		}
		~_local_ArrayElemNorm()
		{
			this->mInt = 0;
		}
		
		const _local_ArrayElemNorm &operator=(const _local_ArrayElemNorm &nArrayNorm)
		{
			this->mInt = nArrayNorm.mInt;
			return *this;
		}
		bool operator==(const _local_ArrayElemNorm &nElem)
		{
			return (this->mInt == nElem.mInt);
		}
		
		const _local_ArrayElemNorm &Ref()
		{
			return *this;
		}
		const _local_ArrayElemNorm &Ref() const
		{
			return *this;
		}
		const _local_ArrayElemNorm *Ptr()
		{
			return this;
		}
		const _local_ArrayElemNorm *Ptr() const
		{
			return this;
		}
		
		static acpl::SInt8 CmpVal(const _local_ArrayElemNorm &nElem1, const _local_ArrayElemNorm &nElem2)
		{
			return acpl::Mem::SortCmpFw(nElem1.mInt, nElem2.mInt);
		}
		static acpl::SInt8 CmpPtr(_local_ArrayElemNorm * const &nElem1, _local_ArrayElemNorm * const &nElem2)
		{
			return acpl::Mem::SortCmpFw((*nElem1).mInt, (*nElem2).mInt);
		}
		
		acpl::UInt32 GetInt() const
		{
			return this->mInt;
		}
};

class _local_ArrayElemRest
{
		acplHideClassCopy(_local_ArrayElemRest);
	
	private:
		acpl::UInt32 mInt;
	
	public:
		_local_ArrayElemRest()
		{
			this->mInt = 0;
		}
		_local_ArrayElemRest(acpl::UInt32 nInt)
		{
			this->mInt = nInt;
		}
		_local_ArrayElemRest(acpl::UInt32 nInt1, acpl::UInt8 nInt2)
		{
			this->mInt = nInt1 + nInt2;
		}
		_local_ArrayElemRest(acpl::UInt32 nInt1, acpl::UInt8 nInt2, acpl::UInt16 nInt3)
		{
			this->mInt = nInt1 + nInt2 + nInt3;
		}
		_local_ArrayElemRest(acpl::UInt32 nInt1, acpl::UInt8 nInt2, acpl::UInt16 nInt3, acpl::UInt64 nInt4)
		{
			this->mInt = nInt1 + nInt2 + nInt3 + static_cast<acpl::UInt32>(nInt4);
		}
		_local_ArrayElemRest(acpl::UInt32 nInt1, acpl::UInt8 nInt2, acpl::UInt16 nInt3, acpl::UInt64 nInt4, acpl::SInt32 nInt5)
		{
			this->mInt = nInt1 + nInt2 + nInt3 + static_cast<acpl::UInt32>(nInt4) + nInt5;
		}
		_local_ArrayElemRest(acpl::UInt32 nInt1, acpl::UInt8 nInt2, acpl::UInt16 nInt3, acpl::UInt64 nInt4, acpl::SInt32 nInt5, acpl::SInt8 nInt6)
		{
			this->mInt = nInt1 + nInt2 + nInt3 + static_cast<acpl::UInt32>(nInt4) + nInt5 + nInt6;
		}
		_local_ArrayElemRest(acpl::UInt32 nInt1, acpl::UInt8 nInt2, acpl::UInt16 nInt3, acpl::UInt64 nInt4, acpl::SInt32 nInt5, acpl::SInt8 nInt6, acpl::SInt16 nInt7)
		{
			this->mInt = nInt1 + nInt2 + nInt3 + static_cast<acpl::UInt32>(nInt4) + nInt5 + nInt6 + nInt7;
		}
		_local_ArrayElemRest(acpl::UInt32 nInt1, acpl::UInt8 nInt2, acpl::UInt16 nInt3, acpl::UInt64 nInt4, acpl::SInt32 nInt5, acpl::SInt8 nInt6, acpl::SInt16 nInt7, acpl::SInt64 nInt8)
		{
			this->mInt = nInt1 + nInt2 + nInt3 + static_cast<acpl::UInt32>(nInt4) + nInt5 + nInt6 + nInt7 + static_cast<acpl::UInt32>(nInt8);
		}
		_local_ArrayElemRest(acpl::UInt32 nInt1, acpl::UInt8 nInt2, acpl::UInt16 nInt3, acpl::UInt64 nInt4, acpl::SInt32 nInt5, acpl::SInt8 nInt6, acpl::SInt16 nInt7, acpl::SInt64 nInt8, acpl::Float64 nFloat1)
		{
			this->mInt = nInt1 + nInt2 + nInt3 + static_cast<acpl::UInt32>(nInt4) + nInt5 + nInt6 + nInt7 + static_cast<acpl::UInt32>(nInt8) + static_cast<acpl::SInt32>(nFloat1);
		}
		_local_ArrayElemRest(acpl::UInt32 nInt1, acpl::UInt8 nInt2, acpl::UInt16 nInt3, acpl::UInt64 nInt4, acpl::SInt32 nInt5, acpl::SInt8 nInt6, acpl::SInt16 nInt7, acpl::SInt64 nInt8, acpl::Float64 nFloat1, acpl::Float32 nFloat2)
		{
			this->mInt = nInt1 + nInt2 + nInt3 + static_cast<acpl::UInt32>(nInt4) + nInt5 + nInt6 + nInt7 + static_cast<acpl::UInt32>(nInt8) + static_cast<acpl::SInt32>(nFloat1) + static_cast<acpl::SInt32>(nFloat2);
		}
		~_local_ArrayElemRest()
		{
			this->mInt = 0;
		}
		
		bool operator==(const _local_ArrayElemRest &nElem)
		{
			return (this->mInt == nElem.mInt);
		}
		
		const _local_ArrayElemRest &Ref()
		{
			return *this;
		}
		const _local_ArrayElemRest &Ref() const
		{
			return *this;
		}
		const _local_ArrayElemRest *Ptr()
		{
			return this;
		}
		const _local_ArrayElemRest *Ptr() const
		{
			return this;
		}
		
		static acpl::SInt8 CmpVal(const _local_ArrayElemRest &nElem1, const _local_ArrayElemRest &nElem2)
		{
			return acpl::Mem::SortCmpFw(nElem1.mInt, nElem2.mInt);
		}
		static acpl::SInt8 CmpPtr(_local_ArrayElemRest * const &nElem1, _local_ArrayElemRest * const &nElem2)
		{
			return acpl::Mem::SortCmpFw((*nElem1).mInt, (*nElem2).mInt);
		}
		
		acpl::UInt32 GetInt() const
		{
			return this->mInt;
		}
};

static int TestArrayNormVal()
{
	PrintFn();
	
	
	acpl::ArrayEdaNone<_local_ArrayElemNorm>::Type oNarr;
	const acpl::ArrayEdaNone<_local_ArrayElemNorm>::Type &oCarr = oNarr;
	_local_ArrayElemNorm oElem(32);
	acpl::SizeT oIdx;
	
	
	// Init
	
	Test(oNarr.Count() == 0);
	Test(oNarr.Get(0, oElem) == false);
	Test(oElem == 32);
	Test(oNarr.GetFirst(oElem) == false);
	Test(oElem == 32);
	Test(oNarr.GetLast(oElem) == false);
	Test(oElem == 32);
	Test(oNarr.GetIndexOf(oElem, oIdx) == false);
	Test(oNarr.GetIndexOf(oElem) == acpl::Num<acpl::SizeT>::Max());
	
// 	Test(oNarr.FindIndexOf(_local_ArrayElemNorm(32), &_local_ArrayElemNorm::CmpVal, oIdx) == false);
// 	Test(oNarr.FindIndexOf(_local_ArrayElemNorm(32), &_local_ArrayElemNorm::CmpVal) == acpl::Num<acpl::SizeT>::Max());
// 	Test(oNarr.FindIndexOf<const _local_ArrayElemNorm &>(_local_ArrayElemNorm(32), &_local_ArrayElemNorm::Ref, &_local_ArrayElemNorm::CmpVal, oIdx) == false);
// 	Test(oNarr.FindIndexOf<const _local_ArrayElemNorm &>(_local_ArrayElemNorm(32), &_local_ArrayElemNorm::Ref, &_local_ArrayElemNorm::CmpVal) == acpl::Num<acpl::SizeT>::Max());
// 	Test(oCarr.FindIndexOf<const _local_ArrayElemNorm &>(_local_ArrayElemNorm(32), &_local_ArrayElemNorm::Ref, &_local_ArrayElemNorm::CmpVal, oIdx) == false);
// 	Test(oCarr.FindIndexOf<const _local_ArrayElemNorm &>(_local_ArrayElemNorm(32), &_local_ArrayElemNorm::Ref, &_local_ArrayElemNorm::CmpVal) == acpl::Num<acpl::SizeT>::Max());
// 	
// 	Test(oNarr.Find(_local_ArrayElemNorm(32), &_local_ArrayElemNorm::CmpVal, oElem) == false);
// 	Test(oNarr.Find<const _local_ArrayElemNorm &>(_local_ArrayElemNorm(32), &_local_ArrayElemNorm::Ref, &_local_ArrayElemNorm::CmpVal, oElem) == false);
// 	Test(oCarr.Find<const _local_ArrayElemNorm &>(_local_ArrayElemNorm(32), &_local_ArrayElemNorm::Ref, &_local_ArrayElemNorm::CmpVal, oElem) == false);
	
	Test(oNarr.Swap(0, 1) == false);
	Test(oNarr.Swap(0, oElem) == false);
	Test(oNarr.Swap(0, _local_ArrayElemNorm(32), oElem) == false);
	Test(oNarr.Replace(0, _local_ArrayElemNorm(32)) == false);
	Test(oNarr.Discard(0, oElem) == false);
	Test(oNarr.Remove(0) == false);
	Test(oNarr.RemoveHead(1) == false);
	Test(oNarr.RemoveTail(1) == false);
	Test(oNarr.Remove(0, 1) == false);
	Test(oNarr.Move(0, 1) == false);
	
	Test(oNarr.Shrink() == true);
	oNarr.Clear();
	oNarr.Purge();
	
	
	// Add() overloads + Clear
	
	Test(oNarr.Add() == true);
	Test(oNarr.Count() == 1);
	Test(oCarr.Count() == 1);
	Test(oNarr.GetLast().GetInt() == 0);
	Test(oNarr.Add(oElem) == true);
	Test(oNarr.Count() == 2);
	Test(oNarr.GetLast().GetInt() == 32);
	Test(oNarr.Add(64) == true);
	Test(oNarr.Count() == 3);
	Test(oNarr.GetLast().GetInt() == 64);
	Test(oNarr.Add(64, 32) == true);
	Test(oNarr.Count() == 4);
	Test(oNarr.GetLast().GetInt() == 96);
	Test(oNarr.Add(64, 32, 16) == true);
	Test(oNarr.Count() == 5);
	Test(oNarr.GetLast().GetInt() == 112);
	Test(oNarr.Add(64, 32, 16, 8) == true);
	Test(oNarr.Count() == 6);
	Test(oNarr.GetLast().GetInt() == 120);
	Test(oNarr.Add(64, 32, 16, 8, -2) == true);
	Test(oNarr.Count() == 7);
	Test(oNarr.GetLast().GetInt() == 118);
	Test(oNarr.Add(64, 32, 16, 8, -2, -4) == true);
	Test(oNarr.Count() == 8);
	Test(oNarr.GetLast().GetInt() == 114);
	Test(oNarr.Add(64, 32, 16, 8, -2, -4, -8) == true);
	Test(oNarr.Count() == 9);
	Test(oNarr.GetLast().GetInt() == 106);
	Test(oNarr.Add(64, 32, 16, 8, -2, -4, -8, -16) == true);
	Test(oNarr.Count() == 10);
	Test(oNarr.GetLast().GetInt() == 90);
	Test(oNarr.Add(64, 32, 16, 8, -2, -4, -8, -16, 8.5) == true);
	Test(oNarr.Count() == 11);
	Test(oNarr.GetLast().GetInt() == 98); // even though the result should have been 98.5
	Test(oNarr.Add(64, 32, 16, 8, -2, -4, -8, -16, 8.5, static_cast<acpl::Float32>(-4.7)) == true);
	Test(oNarr.Count() == 12);
	Test(oNarr.GetLast().GetInt() == 94); // even though the result should have been 93.8
	oNarr.Clear();
	Test(oNarr.Count() == 0);
	
	
	// Add() + Swap()s && Replace() + Clear()
	
	Test(oNarr.Add(16) == true);
	Test(oNarr.Add(32) == true);
	Test(oNarr.Count() == 2);
	Test(oNarr.Get(0).GetInt() == 16);
	Test(oNarr.Get(1).GetInt() == 32);
	Test(oNarr.Swap(0, 0) == true);
	Test(oNarr.Get(0).GetInt() == 16);
	Test(oNarr.Get(1).GetInt() == 32);
	Test(oNarr.Swap(0, 1) == true);
	Test(oNarr.Get(0).GetInt() == 32);
	Test(oNarr.Get(1).GetInt() == 16);
	Test(oNarr.Add(64) == true);
	Test(oNarr.Count() == 3);
	Test(oNarr.Get(0).GetInt() == 32);
	Test(oNarr.Get(1).GetInt() == 16);
	Test(oNarr.Get(2).GetInt() == 64);
	Test(oNarr.Swap(0, 2) == true);
	Test(oNarr.Get(0).GetInt() == 64);
	Test(oNarr.Get(1).GetInt() == 16);
	Test(oNarr.Get(2).GetInt() == 32);
	Test(oNarr.Swap(1, 2) == true);
	Test(oNarr.Get(0).GetInt() == 64);
	Test(oNarr.Get(1).GetInt() == 32);
	Test(oNarr.Get(2).GetInt() == 16);
	Test(oNarr.Swap(2, 3) == false);
	Test(oNarr.Swap(3, 2) == false);
	
	Test(oNarr.Swap(0, oElem) == true);
	Test(oElem.GetInt() == 64);
	Test(oNarr.Get(0).GetInt() == 32);
	Test(oNarr.Get(1).GetInt() == 32);
	Test(oNarr.Get(2).GetInt() == 16);
	Test(oNarr.Swap(1, oElem) == true);
	Test(oElem.GetInt() == 32);
	Test(oNarr.Get(0).GetInt() == 32);
	Test(oNarr.Get(1).GetInt() == 64);
	Test(oNarr.Get(2).GetInt() == 16);
	Test(oNarr.Swap(2, oElem) == true);
	Test(oElem.GetInt() == 16);
	Test(oNarr.Get(0).GetInt() == 32);
	Test(oNarr.Get(1).GetInt() == 64);
	Test(oNarr.Get(2).GetInt() == 32);
	Test(oNarr.Swap(3, oElem) == false);
	Test(oElem.GetInt() == 16);
	
	Test(oNarr.Swap(0, _local_ArrayElemNorm(16), oElem) == true);
	Test(oElem.GetInt() == 32);
	Test(oNarr.Get(0).GetInt() == 16);
	Test(oNarr.Get(1).GetInt() == 64);
	Test(oNarr.Get(2).GetInt() == 32);
	Test(oNarr.Swap(1, _local_ArrayElemNorm(32), oElem) == true);
	Test(oElem.GetInt() == 64);
	Test(oNarr.Get(0).GetInt() == 16);
	Test(oNarr.Get(1).GetInt() == 32);
	Test(oNarr.Get(2).GetInt() == 32);
	Test(oNarr.Swap(2, _local_ArrayElemNorm(64), oElem) == true);
	Test(oElem.GetInt() == 32);
	Test(oNarr.Get(0).GetInt() == 16);
	Test(oNarr.Get(1).GetInt() == 32);
	Test(oNarr.Get(2).GetInt() == 64);
	Test(oNarr.Swap(3, _local_ArrayElemNorm(128), oElem) == false);
	Test(oElem.GetInt() == 32);
	
	Test(oNarr.Replace(0, _local_ArrayElemNorm(64)) == true);
	Test(oNarr.Get(0).GetInt() == 64);
	Test(oNarr.Get(1).GetInt() == 32);
	Test(oNarr.Get(2).GetInt() == 64);
	Test(oNarr.Replace(1, _local_ArrayElemNorm(16)) == true);
	Test(oNarr.Get(0).GetInt() == 64);
	Test(oNarr.Get(1).GetInt() == 16);
	Test(oNarr.Get(2).GetInt() == 64);
	Test(oNarr.Replace(2, _local_ArrayElemNorm(32)) == true);
	Test(oNarr.Get(0).GetInt() == 64);
	Test(oNarr.Get(1).GetInt() == 16);
	Test(oNarr.Get(2).GetInt() == 32);
	Test(oNarr.Replace(3, _local_ArrayElemNorm(128)) == false);
	
	oNarr.Clear();
	
	
	// Add() + Discard() && Remove() (classic) + Clear()
	
	Test(oNarr.Add(16) == true);
	Test(oNarr.Add(32) == true);
	Test(oNarr.Add(64) == true);
	Test(oNarr.Count() == 3);
	Test(oNarr.Discard(3, oElem) == false);
	Test(oNarr.Count() == 3);
	Test(oNarr.Get(0).GetInt() == 16);
	Test(oNarr.Get(1).GetInt() == 32);
	Test(oNarr.Get(2).GetInt() == 64);
	Test(oNarr.Discard(0, oElem) == true);
	Test(oNarr.Count() == 2);
	Test(oNarr.Get(0).GetInt() == 32);
	Test(oNarr.Get(1).GetInt() == 64);
	Test(oElem.GetInt() == 16);
	Test(oNarr.Discard(1, oElem) == true);
	Test(oNarr.Count() == 1);
	Test(oNarr.Get(0).GetInt() == 32);
	Test(oElem.GetInt() == 64);
	Test(oNarr.Discard(0, oElem) == true);
	Test(oNarr.Count() == 0);
	Test(oElem.GetInt() == 32);
	Test(oNarr.Discard(0, oElem) == false);
	Test(oNarr.Count() == 0);
	Test(oElem.GetInt() == 32);
	
	Test(oNarr.Add(16) == true);
	Test(oNarr.Add(32) == true);
	Test(oNarr.Add(64) == true);
	Test(oNarr.Count() == 3);
	Test(oNarr.Get(0).GetInt() == 16);
	Test(oNarr.Get(1).GetInt() == 32);
	Test(oNarr.Get(2).GetInt() == 64);
	Test(oNarr.Remove(3) == false);
	Test(oNarr.Count() == 3);
	Test(oNarr.Get(0).GetInt() == 16);
	Test(oNarr.Get(1).GetInt() == 32);
	Test(oNarr.Get(2).GetInt() == 64);
	Test(oNarr.Remove(0) == true);
	Test(oNarr.Count() == 2);
	Test(oNarr.Get(0).GetInt() == 32);
	Test(oNarr.Get(1).GetInt() == 64);
	Test(oNarr.Remove(1) == true);
	Test(oNarr.Count() == 1);
	Test(oNarr.Get(0).GetInt() == 32);
	Test(oNarr.Remove(0) == true);
	Test(oNarr.Count() == 0);
	Test(oNarr.Remove(0) == false);
	Test(oNarr.Count() == 0);
	
	oNarr.Clear();
	
	
	// Add() + Remove{Head,Tail}() && Remove() (advanced) + Clear()
	
	Test(oNarr.RemoveHead(0) == true);
	Test(oNarr.Add(16) == true);
	Test(oNarr.Add(32) == true);
	Test(oNarr.Add(64) == true);
	Test(oNarr.Add(128) == true);
	Test(oNarr.Add(256) == true);
	Test(oNarr.RemoveHead(0) == true);
	Test(oNarr.Count() == 5);
	Test(oNarr.Get(0).GetInt() == 16);
	Test(oNarr.Get(1).GetInt() == 32);
	Test(oNarr.Get(2).GetInt() == 64);
	Test(oNarr.Get(3).GetInt() == 128);
	Test(oNarr.Get(4).GetInt() == 256);
	Test(oNarr.RemoveHead(3) == true);
	Test(oNarr.Count() == 2);
	Test(oNarr.Get(0).GetInt() == 128);
	Test(oNarr.Get(1).GetInt() == 256);
	Test(oNarr.RemoveHead(2) == true);
	Test(oNarr.Count() == 0);
	Test(oNarr.Add(16) == true);
	Test(oNarr.Add(32) == true);
	Test(oNarr.Count() == 2);
	Test(oNarr.RemoveHead(3) == true);
	Test(oNarr.Count() == 0);
	
	Test(oNarr.RemoveTail(0) == true);
	Test(oNarr.Add(16) == true);
	Test(oNarr.Add(32) == true);
	Test(oNarr.Add(64) == true);
	Test(oNarr.Add(128) == true);
	Test(oNarr.Add(256) == true);
	Test(oNarr.RemoveTail(0) == true);
	Test(oNarr.Count() == 5);
	Test(oNarr.Get(0).GetInt() == 16);
	Test(oNarr.Get(1).GetInt() == 32);
	Test(oNarr.Get(2).GetInt() == 64);
	Test(oNarr.Get(3).GetInt() == 128);
	Test(oNarr.Get(4).GetInt() == 256);
	Test(oNarr.RemoveTail(3) == true);
	Test(oNarr.Count() == 2);
	Test(oNarr.Get(0).GetInt() == 16);
	Test(oNarr.Get(1).GetInt() == 32);
	Test(oNarr.RemoveTail(2) == true);
	Test(oNarr.Count() == 0);
	Test(oNarr.Add(16) == true);
	Test(oNarr.Add(32) == true);
	Test(oNarr.Count() == 2);
	Test(oNarr.RemoveTail(3) == true);
	Test(oNarr.Count() == 0);
	
	Test(oNarr.Remove(0, 0) == true);
	Test(oNarr.Add(16) == true);
	Test(oNarr.Add(32) == true);
	Test(oNarr.Add(64) == true);
	Test(oNarr.Add(128) == true);
	Test(oNarr.Add(256) == true);
	Test(oNarr.Remove(0, 0) == true);
	Test(oNarr.Count() == 5);
	Test(oNarr.Get(0).GetInt() == 16);
	Test(oNarr.Get(1).GetInt() == 32);
	Test(oNarr.Get(2).GetInt() == 64);
	Test(oNarr.Get(3).GetInt() == 128);
	Test(oNarr.Get(4).GetInt() == 256);
	Test(oNarr.Remove(1, 3) == true);
	Test(oNarr.Count() == 2);
	Test(oNarr.Get(0).GetInt() == 16);
	Test(oNarr.Get(1).GetInt() == 256);
	Test(oNarr.Add(32) == true);
	Test(oNarr.Add(64) == true);
	Test(oNarr.Add(128) == true);
	Test(oNarr.Remove(0, 3) == true);
	Test(oNarr.Count() == 2);
	Test(oNarr.Get(0).GetInt() == 64);
	Test(oNarr.Get(1).GetInt() == 128);
	Test(oNarr.Remove(0, 3) == true);
	Test(oNarr.Count() == 0);
	Test(oNarr.Add(16) == true);
	Test(oNarr.Add(32) == true);
	Test(oNarr.Add(64) == true);
	Test(oNarr.Add(128) == true);
	Test(oNarr.Add(256) == true);
	Test(oNarr.Count() == 5);
	Test(oNarr.Remove(4, 2) == true);
	Test(oNarr.Count() == 4);
	Test(oNarr.Get(0).GetInt() == 16);
	Test(oNarr.Get(1).GetInt() == 32);
	Test(oNarr.Get(2).GetInt() == 64);
	Test(oNarr.Get(3).GetInt() == 128);
	Test(oNarr.Remove(1, 3) == true);
	Test(oNarr.Count() == 1);
	Test(oNarr.Get(0).GetInt() == 16);
	oNarr.Clear();
	
	
	// Add() + Move() + Clear()
	
	Test(oNarr.Move(0, 0) == false);
	Test(oNarr.Move(0, 1) == false);
	Test(oNarr.Move(1, 0) == false);
	Test(oNarr.Move(1, 1) == false);
	Test(oNarr.Add(16) == true);
	Test(oNarr.Add(32) == true);
	Test(oNarr.Add(64) == true);
	Test(oNarr.Count() == 3);
	Test(oNarr.Get(0).GetInt() == 16);
	Test(oNarr.Get(1).GetInt() == 32);
	Test(oNarr.Get(2).GetInt() == 64);
	Test(oNarr.Move(0, 0) == true);
	Test(oNarr.Get(0).GetInt() == 16);
	Test(oNarr.Get(1).GetInt() == 32);
	Test(oNarr.Get(2).GetInt() == 64);
	Test(oNarr.Move(0, 0) == true);
	Test(oNarr.Get(0).GetInt() == 16);
	Test(oNarr.Get(1).GetInt() == 32);
	Test(oNarr.Get(2).GetInt() == 64);
	Test(oNarr.Move(0, 1) == true);
	Test(oNarr.Get(0).GetInt() == 32);
	Test(oNarr.Get(1).GetInt() == 16);
	Test(oNarr.Get(2).GetInt() == 64);
	Test(oNarr.Move(1, 0) == true);
	Test(oNarr.Get(0).GetInt() == 16);
	Test(oNarr.Get(1).GetInt() == 32);
	Test(oNarr.Get(2).GetInt() == 64);
	Test(oNarr.Move(2, 0) == true);
	Test(oNarr.Get(0).GetInt() == 64);
	Test(oNarr.Get(1).GetInt() == 16);
	Test(oNarr.Get(2).GetInt() == 32);
	Test(oNarr.Move(0, 2) == true);
	Test(oNarr.Get(0).GetInt() == 16);
	Test(oNarr.Get(1).GetInt() == 32);
	Test(oNarr.Get(2).GetInt() == 64);
	Test(oNarr.Move(0, 3) == true); // over limit
	Test(oNarr.Get(0).GetInt() == 32);
	Test(oNarr.Get(1).GetInt() == 64);
	Test(oNarr.Get(2).GetInt() == 16);
	Test(oNarr.Move(1, 3) == true); // over limit
	Test(oNarr.Get(0).GetInt() == 32);
	Test(oNarr.Get(1).GetInt() == 16);
	Test(oNarr.Get(2).GetInt() == 64);
	Test(oNarr.Move(2, 3) == true); // over limit
	Test(oNarr.Get(0).GetInt() == 32);
	Test(oNarr.Get(1).GetInt() == 16);
	Test(oNarr.Get(2).GetInt() == 64);
	oNarr.Clear();
	
	
	// Add() + Sort() + Clear()
	
	oNarr.Sort(&_local_ArrayElemNorm::CmpVal);
	Test(oNarr.Add(64) == true);
	oNarr.Sort(&_local_ArrayElemNorm::CmpVal);
	Test(oNarr.Get(0).GetInt() == 64);
	Test(oNarr.Add(32) == true);
	oNarr.Sort(&_local_ArrayElemNorm::CmpVal);
	Test(oNarr.Get(0).GetInt() == 32);
	Test(oNarr.Get(1).GetInt() == 64);
	Test(oNarr.Add(16) == true);
	oNarr.Sort(&_local_ArrayElemNorm::CmpVal);
	Test(oNarr.Get(0).GetInt() == 16);
	Test(oNarr.Get(1).GetInt() == 32);
	Test(oNarr.Get(2).GetInt() == 64);
	oNarr.Clear();
	
	
	return 0;
}

static int TestArrayNormPtr()
{
	PrintFn();
	
	
	acpl::ArrayEdaDelete<_local_ArrayElemNorm *>::Type oNarr;
	const acpl::ArrayEdaDelete<_local_ArrayElemNorm *>::Type &oCarr = oNarr;
	_local_ArrayElemNorm *oElem = NULL;
	_local_ArrayElemNorm *oElemCon = new _local_ArrayElemNorm(32);
	acpl::Mem::RaiiDelete<_local_ArrayElemNorm *> oRdElemCon(oElemCon);
	acpl::SizeT oIdx;
	
	
	// Init
	
	Test(oNarr.Count() == 0);
	Test(oNarr.Get(0, oElem) == false);
	Test(oElem == NULL);
	Test(oNarr.GetFirst(oElem) == false);
	Test(oElem == NULL);
	Test(oNarr.GetLast(oElem) == false);
	Test(oElem == NULL);
	Test(oNarr.GetIndexOf(oElem, oIdx) == false);
	Test(oNarr.GetIndexOf(oElem) == acpl::Num<acpl::SizeT>::Max());
	
// 	Test(oNarr.FindIndexOf(_local_ArrayElemNorm(32), &_local_ArrayElemNorm::CmpVal, oIdx) == false);
// 	Test(oNarr.FindIndexOf(_local_ArrayElemNorm(32), &_local_ArrayElemNorm::CmpVal) == acpl::Num<acpl::SizeT>::Max());
// 	Test(oNarr.FindIndexOf<const _local_ArrayElemNorm &>(_local_ArrayElemNorm(32), &_local_ArrayElemNorm::Ref, &_local_ArrayElemNorm::CmpVal, oIdx) == false);
// 	Test(oNarr.FindIndexOf<const _local_ArrayElemNorm &>(_local_ArrayElemNorm(32), &_local_ArrayElemNorm::Ref, &_local_ArrayElemNorm::CmpVal) == acpl::Num<acpl::SizeT>::Max());
// 	Test(oCarr.FindIndexOf<const _local_ArrayElemNorm &>(_local_ArrayElemNorm(32), &_local_ArrayElemNorm::Ref, &_local_ArrayElemNorm::CmpVal, oIdx) == false);
// 	Test(oCarr.FindIndexOf<const _local_ArrayElemNorm &>(_local_ArrayElemNorm(32), &_local_ArrayElemNorm::Ref, &_local_ArrayElemNorm::CmpVal) == acpl::Num<acpl::SizeT>::Max());
// 	
// 	Test(oNarr.Find(_local_ArrayElemNorm(32), &_local_ArrayElemNorm::CmpVal, oElem) == false);
// 	Test(oNarr.Find<const _local_ArrayElemNorm &>(_local_ArrayElemNorm(32), &_local_ArrayElemNorm::Ref, &_local_ArrayElemNorm::CmpVal, oElem) == false);
// 	Test(oCarr.Find<const _local_ArrayElemNorm &>(_local_ArrayElemNorm(32), &_local_ArrayElemNorm::Ref, &_local_ArrayElemNorm::CmpVal, oElem) == false);
	
	Test(oNarr.Swap(0, 1) == false);
	Test(oNarr.Swap(0, oElem) == false);
	Test(oNarr.Swap(0, oElemCon, oElem) == false);
	Test(oNarr.Replace(0, oElemCon) == false);
	Test(oNarr.Discard(0, oElem) == false);
	Test(oNarr.Remove(0) == false);
	Test(oNarr.RemoveHead(1) == false);
	Test(oNarr.RemoveTail(1) == false);
	Test(oNarr.Remove(0, 1) == false);
	Test(oNarr.Move(0, 1) == false);
	
	Test(oNarr.Shrink() == true);
	oNarr.Clear();
	oNarr.Purge();
	
	
	// Add() overloads + Clear
	
	Test(oNarr.Add() == true);
	Test(oNarr.Count() == 1);
	Test(oCarr.Count() == 1);
	Test(oNarr.GetLast() == NULL);
	Test(oNarr.Add(oElem) == true);
	Test(oNarr.Count() == 2);
	Test(oNarr.GetLast() == NULL);
	Test(oNarr.Add(new _local_ArrayElemNorm(64)) == true);
	Test(oNarr.Count() == 3);
	Test(oNarr.GetLast()->GetInt() == 64);
	Test(oNarr.Add(new _local_ArrayElemNorm(64, 32)) == true);
	Test(oNarr.Count() == 4);
	Test(oNarr.GetLast()->GetInt() == 96);
	Test(oNarr.Add(new _local_ArrayElemNorm(64, 32, 16)) == true);
	Test(oNarr.Count() == 5);
	Test(oNarr.GetLast()->GetInt() == 112);
	Test(oNarr.Add(new _local_ArrayElemNorm(64, 32, 16, 8)) == true);
	Test(oNarr.Count() == 6);
	Test(oNarr.GetLast()->GetInt() == 120);
	Test(oNarr.Add(new _local_ArrayElemNorm(64, 32, 16, 8, -2)) == true);
	Test(oNarr.Count() == 7);
	Test(oNarr.GetLast()->GetInt() == 118);
	Test(oNarr.Add(new _local_ArrayElemNorm(64, 32, 16, 8, -2, -4)) == true);
	Test(oNarr.Count() == 8);
	Test(oNarr.GetLast()->GetInt() == 114);
	Test(oNarr.Add(new _local_ArrayElemNorm(64, 32, 16, 8, -2, -4, -8)) == true);
	Test(oNarr.Count() == 9);
	Test(oNarr.GetLast()->GetInt() == 106);
	Test(oNarr.Add(new _local_ArrayElemNorm(64, 32, 16, 8, -2, -4, -8, -16)) == true);
	Test(oNarr.Count() == 10);
	Test(oNarr.GetLast()->GetInt() == 90);
	Test(oNarr.Add(new _local_ArrayElemNorm(64, 32, 16, 8, -2, -4, -8, -16, 8.5)) == true);
	Test(oNarr.Count() == 11);
	Test(oNarr.GetLast()->GetInt() == 98); // even though the result should have been 98.5
	Test(oNarr.Add(new _local_ArrayElemNorm(64, 32, 16, 8, -2, -4, -8, -16, 8.5, static_cast<acpl::Float32>(-4.7))) == true);
	Test(oNarr.Count() == 12);
	Test(oNarr.GetLast()->GetInt() == 94); // even though the result should have been 93.8
	oNarr.Clear();
	Test(oNarr.Count() == 0);
	
	
	// Add() + Swap()s && Replace() + Clear()
	
	Test(oNarr.Add(new _local_ArrayElemNorm(16)) == true);
	Test(oNarr.Add(new _local_ArrayElemNorm(32)) == true);
	Test(oNarr.Count() == 2);
	Test(oNarr.Get(0)->GetInt() == 16);
	Test(oNarr.Get(1)->GetInt() == 32);
	Test(oNarr.Swap(0, 0) == true);
	Test(oNarr.Get(0)->GetInt() == 16);
	Test(oNarr.Get(1)->GetInt() == 32);
	Test(oNarr.Swap(0, 1) == true);
	Test(oNarr.Get(0)->GetInt() == 32);
	Test(oNarr.Get(1)->GetInt() == 16);
	Test(oNarr.Add(new _local_ArrayElemNorm(64)) == true);
	Test(oNarr.Count() == 3);
	Test(oNarr.Get(0)->GetInt() == 32);
	Test(oNarr.Get(1)->GetInt() == 16);
	Test(oNarr.Get(2)->GetInt() == 64);
	Test(oNarr.Swap(0, 2) == true);
	Test(oNarr.Get(0)->GetInt() == 64);
	Test(oNarr.Get(1)->GetInt() == 16);
	Test(oNarr.Get(2)->GetInt() == 32);
	Test(oNarr.Swap(1, 2) == true);
	Test(oNarr.Get(0)->GetInt() == 64);
	Test(oNarr.Get(1)->GetInt() == 32);
	Test(oNarr.Get(2)->GetInt() == 16);
	Test(oNarr.Swap(2, 3) == false);
	Test(oNarr.Swap(3, 2) == false);
	
	Test(oNarr.Swap(0, oElemCon) == true);
	Test(oElemCon->GetInt() == 64);
	Test(oNarr.Get(0)->GetInt() == 32);
	Test(oNarr.Get(1)->GetInt() == 32);
	Test(oNarr.Get(2)->GetInt() == 16);
	Test(oNarr.Swap(1, oElemCon) == true);
	Test(oElemCon->GetInt() == 32);
	Test(oNarr.Get(0)->GetInt() == 32);
	Test(oNarr.Get(1)->GetInt() == 64);
	Test(oNarr.Get(2)->GetInt() == 16);
	Test(oNarr.Swap(2, oElemCon) == true);
	Test(oElemCon->GetInt() == 16);
	Test(oNarr.Get(0)->GetInt() == 32);
	Test(oNarr.Get(1)->GetInt() == 64);
	Test(oNarr.Get(2)->GetInt() == 32);
	Test(oNarr.Swap(3, oElemCon) == false);
	Test(oElemCon->GetInt() == 16);
	
	Test(oNarr.Swap(0, new _local_ArrayElemNorm(16), oElem) == true);
	Test(oElem->GetInt() == 32);
	Test(oNarr.Get(0)->GetInt() == 16);
	Test(oNarr.Get(1)->GetInt() == 64);
	Test(oNarr.Get(2)->GetInt() == 32);
	acpl::Mem::Delete(oElem);
	Test(oNarr.Swap(1, new _local_ArrayElemNorm(32), oElem) == true);
	Test(oElem->GetInt() == 64);
	Test(oNarr.Get(0)->GetInt() == 16);
	Test(oNarr.Get(1)->GetInt() == 32);
	Test(oNarr.Get(2)->GetInt() == 32);
	acpl::Mem::Delete(oElem);
	Test(oNarr.Swap(2, new _local_ArrayElemNorm(64), oElem) == true);
	Test(oElem->GetInt() == 32);
	Test(oNarr.Get(0)->GetInt() == 16);
	Test(oNarr.Get(1)->GetInt() == 32);
	Test(oNarr.Get(2)->GetInt() == 64);
	// WARNING: Don't test this because it will be a memory leak. We know this works.
	// Test(oNarr.Swap(3, new _local_ArrayElemNorm(128), oElem) == false);
	// Test(oElem->GetInt() == 32);
	acpl::Mem::Delete(oElem);
	
	Test(oNarr.Replace(0, new _local_ArrayElemNorm(64)) == true);
	Test(oNarr.Get(0)->GetInt() == 64);
	Test(oNarr.Get(1)->GetInt() == 32);
	Test(oNarr.Get(2)->GetInt() == 64);
	Test(oNarr.Replace(1, new _local_ArrayElemNorm(16)) == true);
	Test(oNarr.Get(0)->GetInt() == 64);
	Test(oNarr.Get(1)->GetInt() == 16);
	Test(oNarr.Get(2)->GetInt() == 64);
	Test(oNarr.Replace(2, new _local_ArrayElemNorm(32)) == true);
	Test(oNarr.Get(0)->GetInt() == 64);
	Test(oNarr.Get(1)->GetInt() == 16);
	Test(oNarr.Get(2)->GetInt() == 32);
	// WARNING: Don't test this because it will be a memory leak. We know this works.
	// Test(oNarr.Replace(3, new _local_ArrayElemNorm(128)) == false);
	
	oNarr.Clear();
	
	
	// Add() + Discard() && Remove() (classic) + Clear()
	
	Test(oNarr.Add(new _local_ArrayElemNorm(16)) == true);
	Test(oNarr.Add(new _local_ArrayElemNorm(32)) == true);
	Test(oNarr.Add(new _local_ArrayElemNorm(64)) == true);
	Test(oNarr.Count() == 3);
	Test(oNarr.Discard(3, oElem) == false);
	Test(oNarr.Count() == 3);
	Test(oNarr.Get(0)->GetInt() == 16);
	Test(oNarr.Get(1)->GetInt() == 32);
	Test(oNarr.Get(2)->GetInt() == 64);
	Test(oNarr.Discard(0, oElem) == true);
	Test(oNarr.Count() == 2);
	Test(oNarr.Get(0)->GetInt() == 32);
	Test(oNarr.Get(1)->GetInt() == 64);
	Test(oElem->GetInt() == 16);
	acpl::Mem::Delete(oElem);
	Test(oNarr.Discard(1, oElem) == true);
	Test(oNarr.Count() == 1);
	Test(oNarr.Get(0)->GetInt() == 32);
	Test(oElem->GetInt() == 64);
	acpl::Mem::Delete(oElem);
	Test(oNarr.Discard(0, oElem) == true);
	Test(oNarr.Count() == 0);
	Test(oElem->GetInt() == 32);
	Test(oNarr.Discard(0, oElem) == false);
	Test(oNarr.Count() == 0);
	Test(oElem->GetInt() == 32);
	acpl::Mem::Delete(oElem);
	
	Test(oNarr.Add(new _local_ArrayElemNorm(16)) == true);
	Test(oNarr.Add(new _local_ArrayElemNorm(32)) == true);
	Test(oNarr.Add(new _local_ArrayElemNorm(64)) == true);
	Test(oNarr.Count() == 3);
	Test(oNarr.Get(0)->GetInt() == 16);
	Test(oNarr.Get(1)->GetInt() == 32);
	Test(oNarr.Get(2)->GetInt() == 64);
	Test(oNarr.Remove(3) == false);
	Test(oNarr.Count() == 3);
	Test(oNarr.Get(0)->GetInt() == 16);
	Test(oNarr.Get(1)->GetInt() == 32);
	Test(oNarr.Get(2)->GetInt() == 64);
	Test(oNarr.Remove(0) == true);
	Test(oNarr.Count() == 2);
	Test(oNarr.Get(0)->GetInt() == 32);
	Test(oNarr.Get(1)->GetInt() == 64);
	Test(oNarr.Remove(1) == true);
	Test(oNarr.Count() == 1);
	Test(oNarr.Get(0)->GetInt() == 32);
	Test(oNarr.Remove(0) == true);
	Test(oNarr.Count() == 0);
	Test(oNarr.Remove(0) == false);
	Test(oNarr.Count() == 0);
	
	oNarr.Clear();
	
	
	// Add() + Remove{Head,Tail}() && Remove() (advanced) + Clear()
	
	Test(oNarr.RemoveHead(0) == true);
	Test(oNarr.Add(new _local_ArrayElemNorm(16)) == true);
	Test(oNarr.Add(new _local_ArrayElemNorm(32)) == true);
	Test(oNarr.Add(new _local_ArrayElemNorm(64)) == true);
	Test(oNarr.Add(new _local_ArrayElemNorm(128)) == true);
	Test(oNarr.Add(new _local_ArrayElemNorm(256)) == true);
	Test(oNarr.RemoveHead(0) == true);
	Test(oNarr.Count() == 5);
	Test(oNarr.Get(0)->GetInt() == 16);
	Test(oNarr.Get(1)->GetInt() == 32);
	Test(oNarr.Get(2)->GetInt() == 64);
	Test(oNarr.Get(3)->GetInt() == 128);
	Test(oNarr.Get(4)->GetInt() == 256);
	Test(oNarr.RemoveHead(3) == true);
	Test(oNarr.Count() == 2);
	Test(oNarr.Get(0)->GetInt() == 128);
	Test(oNarr.Get(1)->GetInt() == 256);
	Test(oNarr.RemoveHead(2) == true);
	Test(oNarr.Count() == 0);
	Test(oNarr.Add(new _local_ArrayElemNorm(16)) == true);
	Test(oNarr.Add(new _local_ArrayElemNorm(32)) == true);
	Test(oNarr.Count() == 2);
	Test(oNarr.RemoveHead(3) == true);
	Test(oNarr.Count() == 0);
	
	Test(oNarr.RemoveTail(0) == true);
	Test(oNarr.Add(new _local_ArrayElemNorm(16)) == true);
	Test(oNarr.Add(new _local_ArrayElemNorm(32)) == true);
	Test(oNarr.Add(new _local_ArrayElemNorm(64)) == true);
	Test(oNarr.Add(new _local_ArrayElemNorm(128)) == true);
	Test(oNarr.Add(new _local_ArrayElemNorm(256)) == true);
	Test(oNarr.RemoveTail(0) == true);
	Test(oNarr.Count() == 5);
	Test(oNarr.Get(0)->GetInt() == 16);
	Test(oNarr.Get(1)->GetInt() == 32);
	Test(oNarr.Get(2)->GetInt() == 64);
	Test(oNarr.Get(3)->GetInt() == 128);
	Test(oNarr.Get(4)->GetInt() == 256);
	Test(oNarr.RemoveTail(3) == true);
	Test(oNarr.Count() == 2);
	Test(oNarr.Get(0)->GetInt() == 16);
	Test(oNarr.Get(1)->GetInt() == 32);
	Test(oNarr.RemoveTail(2) == true);
	Test(oNarr.Count() == 0);
	Test(oNarr.Add(new _local_ArrayElemNorm(16)) == true);
	Test(oNarr.Add(new _local_ArrayElemNorm(32)) == true);
	Test(oNarr.Count() == 2);
	Test(oNarr.RemoveTail(3) == true);
	Test(oNarr.Count() == 0);
	
	Test(oNarr.Remove(0, 0) == true);
	Test(oNarr.Add(new _local_ArrayElemNorm(16)) == true);
	Test(oNarr.Add(new _local_ArrayElemNorm(32)) == true);
	Test(oNarr.Add(new _local_ArrayElemNorm(64)) == true);
	Test(oNarr.Add(new _local_ArrayElemNorm(128)) == true);
	Test(oNarr.Add(new _local_ArrayElemNorm(256)) == true);
	Test(oNarr.Remove(0, 0) == true);
	Test(oNarr.Count() == 5);
	Test(oNarr.Get(0)->GetInt() == 16);
	Test(oNarr.Get(1)->GetInt() == 32);
	Test(oNarr.Get(2)->GetInt() == 64);
	Test(oNarr.Get(3)->GetInt() == 128);
	Test(oNarr.Get(4)->GetInt() == 256);
	Test(oNarr.Remove(1, 3) == true);
	Test(oNarr.Count() == 2);
	Test(oNarr.Get(0)->GetInt() == 16);
	Test(oNarr.Get(1)->GetInt() == 256);
	Test(oNarr.Add(new _local_ArrayElemNorm(32)) == true);
	Test(oNarr.Add(new _local_ArrayElemNorm(64)) == true);
	Test(oNarr.Add(new _local_ArrayElemNorm(128)) == true);
	Test(oNarr.Remove(0, 3) == true);
	Test(oNarr.Count() == 2);
	Test(oNarr.Get(0)->GetInt() == 64);
	Test(oNarr.Get(1)->GetInt() == 128);
	Test(oNarr.Remove(0, 3) == true);
	Test(oNarr.Count() == 0);
	Test(oNarr.Add(new _local_ArrayElemNorm(16)) == true);
	Test(oNarr.Add(new _local_ArrayElemNorm(32)) == true);
	Test(oNarr.Add(new _local_ArrayElemNorm(64)) == true);
	Test(oNarr.Add(new _local_ArrayElemNorm(128)) == true);
	Test(oNarr.Add(new _local_ArrayElemNorm(256)) == true);
	Test(oNarr.Count() == 5);
	Test(oNarr.Remove(4, 2) == true);
	Test(oNarr.Count() == 4);
	Test(oNarr.Get(0)->GetInt() == 16);
	Test(oNarr.Get(1)->GetInt() == 32);
	Test(oNarr.Get(2)->GetInt() == 64);
	Test(oNarr.Get(3)->GetInt() == 128);
	Test(oNarr.Remove(1, 3) == true);
	Test(oNarr.Count() == 1);
	Test(oNarr.Get(0)->GetInt() == 16);
	oNarr.Clear();
	
	
	// Add() + Move() + Clear()
	
	Test(oNarr.Move(0, 0) == false);
	Test(oNarr.Move(0, 1) == false);
	Test(oNarr.Move(1, 0) == false);
	Test(oNarr.Move(1, 1) == false);
	Test(oNarr.Add(new _local_ArrayElemNorm(16)) == true);
	Test(oNarr.Add(new _local_ArrayElemNorm(32)) == true);
	Test(oNarr.Add(new _local_ArrayElemNorm(64)) == true);
	Test(oNarr.Count() == 3);
	Test(oNarr.Get(0)->GetInt() == 16);
	Test(oNarr.Get(1)->GetInt() == 32);
	Test(oNarr.Get(2)->GetInt() == 64);
	Test(oNarr.Move(0, 0) == true);
	Test(oNarr.Get(0)->GetInt() == 16);
	Test(oNarr.Get(1)->GetInt() == 32);
	Test(oNarr.Get(2)->GetInt() == 64);
	Test(oNarr.Move(0, 0) == true);
	Test(oNarr.Get(0)->GetInt() == 16);
	Test(oNarr.Get(1)->GetInt() == 32);
	Test(oNarr.Get(2)->GetInt() == 64);
	Test(oNarr.Move(0, 1) == true);
	Test(oNarr.Get(0)->GetInt() == 32);
	Test(oNarr.Get(1)->GetInt() == 16);
	Test(oNarr.Get(2)->GetInt() == 64);
	Test(oNarr.Move(1, 0) == true);
	Test(oNarr.Get(0)->GetInt() == 16);
	Test(oNarr.Get(1)->GetInt() == 32);
	Test(oNarr.Get(2)->GetInt() == 64);
	Test(oNarr.Move(2, 0) == true);
	Test(oNarr.Get(0)->GetInt() == 64);
	Test(oNarr.Get(1)->GetInt() == 16);
	Test(oNarr.Get(2)->GetInt() == 32);
	Test(oNarr.Move(0, 2) == true);
	Test(oNarr.Get(0)->GetInt() == 16);
	Test(oNarr.Get(1)->GetInt() == 32);
	Test(oNarr.Get(2)->GetInt() == 64);
	Test(oNarr.Move(0, 3) == true); // over limit
	Test(oNarr.Get(0)->GetInt() == 32);
	Test(oNarr.Get(1)->GetInt() == 64);
	Test(oNarr.Get(2)->GetInt() == 16);
	Test(oNarr.Move(1, 3) == true); // over limit
	Test(oNarr.Get(0)->GetInt() == 32);
	Test(oNarr.Get(1)->GetInt() == 16);
	Test(oNarr.Get(2)->GetInt() == 64);
	Test(oNarr.Move(2, 3) == true); // over limit
	Test(oNarr.Get(0)->GetInt() == 32);
	Test(oNarr.Get(1)->GetInt() == 16);
	Test(oNarr.Get(2)->GetInt() == 64);
	oNarr.Clear();
	
	
	// Add() + Sort() + Clear()
	
	oNarr.Sort(&_local_ArrayElemNorm::CmpPtr);
	Test(oNarr.Add(new _local_ArrayElemNorm(64)) == true);
	oNarr.Sort(&_local_ArrayElemNorm::CmpPtr);
	Test(oNarr.Get(0)->GetInt() == 64);
	Test(oNarr.Add(new _local_ArrayElemNorm(32)) == true);
	oNarr.Sort(&_local_ArrayElemNorm::CmpPtr);
	Test(oNarr.Get(0)->GetInt() == 32);
	Test(oNarr.Get(1)->GetInt() == 64);
	Test(oNarr.Add(new _local_ArrayElemNorm(16)) == true);
	oNarr.Sort(&_local_ArrayElemNorm::CmpPtr);
	Test(oNarr.Get(0)->GetInt() == 16);
	Test(oNarr.Get(1)->GetInt() == 32);
	Test(oNarr.Get(2)->GetInt() == 64);
	oNarr.Clear();
	
	
	return 0;
}

static int TestArrayRestVal()
{
	PrintFn();
	
	
	acpl::ArrayEdaNone<_local_ArrayElemRest>::Type oNarr;
	const acpl::ArrayEdaNone<_local_ArrayElemRest>::Type &oCarr = oNarr;
	_local_ArrayElemRest oElem(32);
	acpl::SizeT oIdx;
	
	
	// Init
	
	Test(oNarr.Count() == 0);
	// WARNING: These will not work on a restricted class
	// Test(oNarr.Get(0, oElem) == false); 
	// Test(oElem == 32);
	// Test(oNarr.GetFirst(oElem) == false);
	// Test(oElem == 32);
	// Test(oNarr.GetLast(oElem) == false);
	// Test(oElem == 32);
	Test(oNarr.GetIndexOf(oElem, oIdx) == false);
	Test(oNarr.GetIndexOf(oElem) == acpl::Num<acpl::SizeT>::Max());
	
// 	Test(oNarr.FindIndexOf(_local_ArrayElemRest(32), &_local_ArrayElemRest::CmpVal, oIdx) == false);
// 	Test(oNarr.FindIndexOf(_local_ArrayElemRest(32), &_local_ArrayElemRest::CmpVal) == acpl::Num<acpl::SizeT>::Max());
// 	Test(oNarr.FindIndexOf<const _local_ArrayElemRest &>(_local_ArrayElemRest(32), &_local_ArrayElemRest::Ref, &_local_ArrayElemRest::CmpVal, oIdx) == false);
// 	Test(oNarr.FindIndexOf<const _local_ArrayElemRest &>(_local_ArrayElemRest(32), &_local_ArrayElemRest::Ref, &_local_ArrayElemRest::CmpVal) == acpl::Num<acpl::SizeT>::Max());
// 	Test(oCarr.FindIndexOf<const _local_ArrayElemRest &>(_local_ArrayElemRest(32), &_local_ArrayElemRest::Ref, &_local_ArrayElemRest::CmpVal, oIdx) == false);
// 	Test(oCarr.FindIndexOf<const _local_ArrayElemRest &>(_local_ArrayElemRest(32), &_local_ArrayElemRest::Ref, &_local_ArrayElemRest::CmpVal) == acpl::Num<acpl::SizeT>::Max());
// 	
// 	Test(oNarr.Find(_local_ArrayElemRest(32), &_local_ArrayElemRest::CmpVal, oElem) == false);
// 	Test(oNarr.Find<const _local_ArrayElemRest &>(_local_ArrayElemRest(32), &_local_ArrayElemRest::Ref, &_local_ArrayElemRest::CmpVal, oElem) == false);
// 	Test(oCarr.Find<const _local_ArrayElemRest &>(_local_ArrayElemRest(32), &_local_ArrayElemRest::Ref, &_local_ArrayElemRest::CmpVal, oElem) == false);
	
	Test(oNarr.Swap(0, 1) == false);
	Test(oNarr.Swap(0, oElem) == false);
	// WARNING: These will not work on a restricted class
	// Test(oNarr.Swap(0, _local_ArrayElemRest(32), oElem) == false);
	// Test(oNarr.Replace(0, _local_ArrayElemRest(32)) == false);
	Test(oNarr.Discard(0, oElem) == false);
	Test(oNarr.Remove(0) == false);
	Test(oNarr.RemoveHead(1) == false);
	Test(oNarr.RemoveTail(1) == false);
	Test(oNarr.Remove(0, 1) == false);
	Test(oNarr.Move(0, 1) == false);
	
	Test(oNarr.Shrink() == true);
	oNarr.Clear();
	oNarr.Purge();
	
	
	// Add() overloads + Clear
	
	Test(oNarr.Add() == true);
	Test(oNarr.Count() == 1);
	Test(oCarr.Count() == 1);
	Test(oNarr.GetLast().GetInt() == 0);
	// WARNING: This will not work on a restricted class
	// Test(oNarr.Add(oElem) == true);
	// Test(oNarr.Count() == 2);
	// Test(oNarr.GetLast().GetInt() == 32);
	Test(oNarr.Add(64) == true);
	Test(oNarr.Count() == 2);
	Test(oNarr.GetLast().GetInt() == 64);
	Test(oNarr.Add(64, 32) == true);
	Test(oNarr.Count() == 3);
	Test(oNarr.GetLast().GetInt() == 96);
	Test(oNarr.Add(64, 32, 16) == true);
	Test(oNarr.Count() == 4);
	Test(oNarr.GetLast().GetInt() == 112);
	Test(oNarr.Add(64, 32, 16, 8) == true);
	Test(oNarr.Count() == 5);
	Test(oNarr.GetLast().GetInt() == 120);
	Test(oNarr.Add(64, 32, 16, 8, -2) == true);
	Test(oNarr.Count() == 6);
	Test(oNarr.GetLast().GetInt() == 118);
	Test(oNarr.Add(64, 32, 16, 8, -2, -4) == true);
	Test(oNarr.Count() == 7);
	Test(oNarr.GetLast().GetInt() == 114);
	Test(oNarr.Add(64, 32, 16, 8, -2, -4, -8) == true);
	Test(oNarr.Count() == 8);
	Test(oNarr.GetLast().GetInt() == 106);
	Test(oNarr.Add(64, 32, 16, 8, -2, -4, -8, -16) == true);
	Test(oNarr.Count() == 9);
	Test(oNarr.GetLast().GetInt() == 90);
	Test(oNarr.Add(64, 32, 16, 8, -2, -4, -8, -16, 8.5) == true);
	Test(oNarr.Count() == 10);
	Test(oNarr.GetLast().GetInt() == 98); // even though the result should have been 98.5
	Test(oNarr.Add(64, 32, 16, 8, -2, -4, -8, -16, 8.5, static_cast<acpl::Float32>(-4.7)) == true);
	Test(oNarr.Count() == 11);
	Test(oNarr.GetLast().GetInt() == 94); // even though the result should have been 93.8
	oNarr.Clear();
	Test(oNarr.Count() == 0);
	
	
	// Add() + Swap()s && Replace() + Clear()
	
	Test(oNarr.Add(16) == true);
	Test(oNarr.Add(32) == true);
	Test(oNarr.Count() == 2);
	Test(oNarr.Get(0).GetInt() == 16);
	Test(oNarr.Get(1).GetInt() == 32);
	Test(oNarr.Swap(0, 0) == true);
	Test(oNarr.Get(0).GetInt() == 16);
	Test(oNarr.Get(1).GetInt() == 32);
	Test(oNarr.Swap(0, 1) == true);
	Test(oNarr.Get(0).GetInt() == 32);
	Test(oNarr.Get(1).GetInt() == 16);
	Test(oNarr.Add(64) == true);
	Test(oNarr.Count() == 3);
	Test(oNarr.Get(0).GetInt() == 32);
	Test(oNarr.Get(1).GetInt() == 16);
	Test(oNarr.Get(2).GetInt() == 64);
	Test(oNarr.Swap(0, 2) == true);
	Test(oNarr.Get(0).GetInt() == 64);
	Test(oNarr.Get(1).GetInt() == 16);
	Test(oNarr.Get(2).GetInt() == 32);
	Test(oNarr.Swap(1, 2) == true);
	Test(oNarr.Get(0).GetInt() == 64);
	Test(oNarr.Get(1).GetInt() == 32);
	Test(oNarr.Get(2).GetInt() == 16);
	Test(oNarr.Swap(2, 3) == false);
	Test(oNarr.Swap(3, 2) == false);
	
	Test(oNarr.Swap(0, oElem) == true);
	Test(oElem.GetInt() == 64);
	Test(oNarr.Get(0).GetInt() == 32);
	Test(oNarr.Get(1).GetInt() == 32);
	Test(oNarr.Get(2).GetInt() == 16);
	Test(oNarr.Swap(1, oElem) == true);
	Test(oElem.GetInt() == 32);
	Test(oNarr.Get(0).GetInt() == 32);
	Test(oNarr.Get(1).GetInt() == 64);
	Test(oNarr.Get(2).GetInt() == 16);
	Test(oNarr.Swap(2, oElem) == true);
	Test(oElem.GetInt() == 16);
	Test(oNarr.Get(0).GetInt() == 32);
	Test(oNarr.Get(1).GetInt() == 64);
	Test(oNarr.Get(2).GetInt() == 32);
	Test(oNarr.Swap(3, oElem) == false);
	Test(oElem.GetInt() == 16);
	
	// WARNING: These will not work on a restricted class
	// Test(oNarr.Swap(0, _local_ArrayElemRest(16), oElem) == true);
	// Test(oElem.GetInt() == 32);
	// Test(oNarr.Get(0).GetInt() == 16);
	// Test(oNarr.Get(1).GetInt() == 64);
	// Test(oNarr.Get(2).GetInt() == 32);
	// Test(oNarr.Swap(1, _local_ArrayElemRest(32), oElem) == true);
	// Test(oElem.GetInt() == 64);
	// Test(oNarr.Get(0).GetInt() == 16);
	// Test(oNarr.Get(1).GetInt() == 32);
	// Test(oNarr.Get(2).GetInt() == 32);
	// Test(oNarr.Swap(2, _local_ArrayElemRest(64), oElem) == true);
	// Test(oElem.GetInt() == 32);
	// Test(oNarr.Get(0).GetInt() == 16);
	// Test(oNarr.Get(1).GetInt() == 32);
	// Test(oNarr.Get(2).GetInt() == 64);
	// Test(oNarr.Swap(3, _local_ArrayElemRest(128), oElem) == false);
	// Test(oElem.GetInt() == 32);
	
	// WARNING: These will not work on a restricted class
	// Test(oNarr.Replace(0, _local_ArrayElemRest(64)) == true);
	// Test(oNarr.Get(0).GetInt() == 64);
	// Test(oNarr.Get(1).GetInt() == 32);
	// Test(oNarr.Get(2).GetInt() == 64);
	// Test(oNarr.Replace(1, _local_ArrayElemRest(16)) == true);
	// Test(oNarr.Get(0).GetInt() == 64);
	// Test(oNarr.Get(1).GetInt() == 16);
	// Test(oNarr.Get(2).GetInt() == 64);
	// Test(oNarr.Replace(2, _local_ArrayElemRest(32)) == true);
	// Test(oNarr.Get(0).GetInt() == 64);
	// Test(oNarr.Get(1).GetInt() == 16);
	// Test(oNarr.Get(2).GetInt() == 32);
	// Test(oNarr.Replace(3, _local_ArrayElemRest(128)) == false);
	
	oNarr.Clear();
	
	
	// Add() + Discard() && Remove() (classic) + Clear()
	
	Test(oNarr.Add(16) == true);
	Test(oNarr.Add(32) == true);
	Test(oNarr.Add(64) == true);
	Test(oNarr.Count() == 3);
	Test(oNarr.Discard(3, oElem) == false);
	Test(oNarr.Count() == 3);
	Test(oNarr.Get(0).GetInt() == 16);
	Test(oNarr.Get(1).GetInt() == 32);
	Test(oNarr.Get(2).GetInt() == 64);
	Test(oNarr.Discard(0, oElem) == true);
	Test(oNarr.Count() == 2);
	Test(oNarr.Get(0).GetInt() == 32);
	Test(oNarr.Get(1).GetInt() == 64);
	Test(oElem.GetInt() == 16);
	Test(oNarr.Discard(1, oElem) == true);
	Test(oNarr.Count() == 1);
	Test(oNarr.Get(0).GetInt() == 32);
	Test(oElem.GetInt() == 64);
	Test(oNarr.Discard(0, oElem) == true);
	Test(oNarr.Count() == 0);
	Test(oElem.GetInt() == 32);
	Test(oNarr.Discard(0, oElem) == false);
	Test(oNarr.Count() == 0);
	Test(oElem.GetInt() == 32);
	
	Test(oNarr.Add(16) == true);
	Test(oNarr.Add(32) == true);
	Test(oNarr.Add(64) == true);
	Test(oNarr.Count() == 3);
	Test(oNarr.Get(0).GetInt() == 16);
	Test(oNarr.Get(1).GetInt() == 32);
	Test(oNarr.Get(2).GetInt() == 64);
	Test(oNarr.Remove(3) == false);
	Test(oNarr.Count() == 3);
	Test(oNarr.Get(0).GetInt() == 16);
	Test(oNarr.Get(1).GetInt() == 32);
	Test(oNarr.Get(2).GetInt() == 64);
	Test(oNarr.Remove(0) == true);
	Test(oNarr.Count() == 2);
	Test(oNarr.Get(0).GetInt() == 32);
	Test(oNarr.Get(1).GetInt() == 64);
	Test(oNarr.Remove(1) == true);
	Test(oNarr.Count() == 1);
	Test(oNarr.Get(0).GetInt() == 32);
	Test(oNarr.Remove(0) == true);
	Test(oNarr.Count() == 0);
	Test(oNarr.Remove(0) == false);
	Test(oNarr.Count() == 0);
	
	oNarr.Clear();
	
	
	// Add() + Remove{Head,Tail}() && Remove() (advanced) + Clear()
	
	Test(oNarr.RemoveHead(0) == true);
	Test(oNarr.Add(16) == true);
	Test(oNarr.Add(32) == true);
	Test(oNarr.Add(64) == true);
	Test(oNarr.Add(128) == true);
	Test(oNarr.Add(256) == true);
	Test(oNarr.RemoveHead(0) == true);
	Test(oNarr.Count() == 5);
	Test(oNarr.Get(0).GetInt() == 16);
	Test(oNarr.Get(1).GetInt() == 32);
	Test(oNarr.Get(2).GetInt() == 64);
	Test(oNarr.Get(3).GetInt() == 128);
	Test(oNarr.Get(4).GetInt() == 256);
	Test(oNarr.RemoveHead(3) == true);
	Test(oNarr.Count() == 2);
	Test(oNarr.Get(0).GetInt() == 128);
	Test(oNarr.Get(1).GetInt() == 256);
	Test(oNarr.RemoveHead(2) == true);
	Test(oNarr.Count() == 0);
	Test(oNarr.Add(16) == true);
	Test(oNarr.Add(32) == true);
	Test(oNarr.Count() == 2);
	Test(oNarr.RemoveHead(3) == true);
	Test(oNarr.Count() == 0);
	
	Test(oNarr.RemoveTail(0) == true);
	Test(oNarr.Add(16) == true);
	Test(oNarr.Add(32) == true);
	Test(oNarr.Add(64) == true);
	Test(oNarr.Add(128) == true);
	Test(oNarr.Add(256) == true);
	Test(oNarr.RemoveTail(0) == true);
	Test(oNarr.Count() == 5);
	Test(oNarr.Get(0).GetInt() == 16);
	Test(oNarr.Get(1).GetInt() == 32);
	Test(oNarr.Get(2).GetInt() == 64);
	Test(oNarr.Get(3).GetInt() == 128);
	Test(oNarr.Get(4).GetInt() == 256);
	Test(oNarr.RemoveTail(3) == true);
	Test(oNarr.Count() == 2);
	Test(oNarr.Get(0).GetInt() == 16);
	Test(oNarr.Get(1).GetInt() == 32);
	Test(oNarr.RemoveTail(2) == true);
	Test(oNarr.Count() == 0);
	Test(oNarr.Add(16) == true);
	Test(oNarr.Add(32) == true);
	Test(oNarr.Count() == 2);
	Test(oNarr.RemoveTail(3) == true);
	Test(oNarr.Count() == 0);
	
	Test(oNarr.Remove(0, 0) == true);
	Test(oNarr.Add(16) == true);
	Test(oNarr.Add(32) == true);
	Test(oNarr.Add(64) == true);
	Test(oNarr.Add(128) == true);
	Test(oNarr.Add(256) == true);
	Test(oNarr.Remove(0, 0) == true);
	Test(oNarr.Count() == 5);
	Test(oNarr.Get(0).GetInt() == 16);
	Test(oNarr.Get(1).GetInt() == 32);
	Test(oNarr.Get(2).GetInt() == 64);
	Test(oNarr.Get(3).GetInt() == 128);
	Test(oNarr.Get(4).GetInt() == 256);
	Test(oNarr.Remove(1, 3) == true);
	Test(oNarr.Count() == 2);
	Test(oNarr.Get(0).GetInt() == 16);
	Test(oNarr.Get(1).GetInt() == 256);
	Test(oNarr.Add(32) == true);
	Test(oNarr.Add(64) == true);
	Test(oNarr.Add(128) == true);
	Test(oNarr.Remove(0, 3) == true);
	Test(oNarr.Count() == 2);
	Test(oNarr.Get(0).GetInt() == 64);
	Test(oNarr.Get(1).GetInt() == 128);
	Test(oNarr.Remove(0, 3) == true);
	Test(oNarr.Count() == 0);
	Test(oNarr.Add(16) == true);
	Test(oNarr.Add(32) == true);
	Test(oNarr.Add(64) == true);
	Test(oNarr.Add(128) == true);
	Test(oNarr.Add(256) == true);
	Test(oNarr.Count() == 5);
	Test(oNarr.Remove(4, 2) == true);
	Test(oNarr.Count() == 4);
	Test(oNarr.Get(0).GetInt() == 16);
	Test(oNarr.Get(1).GetInt() == 32);
	Test(oNarr.Get(2).GetInt() == 64);
	Test(oNarr.Get(3).GetInt() == 128);
	Test(oNarr.Remove(1, 3) == true);
	Test(oNarr.Count() == 1);
	Test(oNarr.Get(0).GetInt() == 16);
	oNarr.Clear();
	
	
	// Add() + Move() + Clear()
	
	Test(oNarr.Move(0, 0) == false);
	Test(oNarr.Move(0, 1) == false);
	Test(oNarr.Move(1, 0) == false);
	Test(oNarr.Move(1, 1) == false);
	Test(oNarr.Add(16) == true);
	Test(oNarr.Add(32) == true);
	Test(oNarr.Add(64) == true);
	Test(oNarr.Count() == 3);
	Test(oNarr.Get(0).GetInt() == 16);
	Test(oNarr.Get(1).GetInt() == 32);
	Test(oNarr.Get(2).GetInt() == 64);
	Test(oNarr.Move(0, 0) == true);
	Test(oNarr.Get(0).GetInt() == 16);
	Test(oNarr.Get(1).GetInt() == 32);
	Test(oNarr.Get(2).GetInt() == 64);
	Test(oNarr.Move(0, 0) == true);
	Test(oNarr.Get(0).GetInt() == 16);
	Test(oNarr.Get(1).GetInt() == 32);
	Test(oNarr.Get(2).GetInt() == 64);
	Test(oNarr.Move(0, 1) == true);
	Test(oNarr.Get(0).GetInt() == 32);
	Test(oNarr.Get(1).GetInt() == 16);
	Test(oNarr.Get(2).GetInt() == 64);
	Test(oNarr.Move(1, 0) == true);
	Test(oNarr.Get(0).GetInt() == 16);
	Test(oNarr.Get(1).GetInt() == 32);
	Test(oNarr.Get(2).GetInt() == 64);
	Test(oNarr.Move(2, 0) == true);
	Test(oNarr.Get(0).GetInt() == 64);
	Test(oNarr.Get(1).GetInt() == 16);
	Test(oNarr.Get(2).GetInt() == 32);
	Test(oNarr.Move(0, 2) == true);
	Test(oNarr.Get(0).GetInt() == 16);
	Test(oNarr.Get(1).GetInt() == 32);
	Test(oNarr.Get(2).GetInt() == 64);
	Test(oNarr.Move(0, 3) == true); // over limit
	Test(oNarr.Get(0).GetInt() == 32);
	Test(oNarr.Get(1).GetInt() == 64);
	Test(oNarr.Get(2).GetInt() == 16);
	Test(oNarr.Move(1, 3) == true); // over limit
	Test(oNarr.Get(0).GetInt() == 32);
	Test(oNarr.Get(1).GetInt() == 16);
	Test(oNarr.Get(2).GetInt() == 64);
	Test(oNarr.Move(2, 3) == true); // over limit
	Test(oNarr.Get(0).GetInt() == 32);
	Test(oNarr.Get(1).GetInt() == 16);
	Test(oNarr.Get(2).GetInt() == 64);
	oNarr.Clear();
	
	
	// Add() + Sort() + Clear()
	
	oNarr.Sort(&_local_ArrayElemRest::CmpVal);
	Test(oNarr.Add(64) == true);
	oNarr.Sort(&_local_ArrayElemRest::CmpVal);
	Test(oNarr.Get(0).GetInt() == 64);
	Test(oNarr.Add(32) == true);
	oNarr.Sort(&_local_ArrayElemRest::CmpVal);
	Test(oNarr.Get(0).GetInt() == 32);
	Test(oNarr.Get(1).GetInt() == 64);
	Test(oNarr.Add(16) == true);
	oNarr.Sort(&_local_ArrayElemRest::CmpVal);
	Test(oNarr.Get(0).GetInt() == 16);
	Test(oNarr.Get(1).GetInt() == 32);
	Test(oNarr.Get(2).GetInt() == 64);
	oNarr.Clear();
	
	
	return 0;
}

static int TestArrayRestPtr()
{
	PrintFn();
	
	
	acpl::ArrayEdaDelete<_local_ArrayElemRest *>::Type oNarr;
	const acpl::ArrayEdaDelete<_local_ArrayElemRest *>::Type &oCarr = oNarr;
	_local_ArrayElemRest *oElem = NULL;
	_local_ArrayElemRest *oElemCon = new _local_ArrayElemRest(32);
	acpl::Mem::RaiiDelete<_local_ArrayElemRest *> oRdElemCon(oElemCon);
	acpl::SizeT oIdx;
	
	
	// Init
	
	Test(oNarr.Count() == 0);
	Test(oNarr.Get(0, oElem) == false);
	Test(oElem == NULL);
	Test(oNarr.GetFirst(oElem) == false);
	Test(oElem == NULL);
	Test(oNarr.GetLast(oElem) == false);
	Test(oElem == NULL);
	Test(oNarr.GetIndexOf(oElem, oIdx) == false);
	Test(oNarr.GetIndexOf(oElem) == acpl::Num<acpl::SizeT>::Max());
	
// 	Test(oNarr.FindIndexOf(_local_ArrayElemRest(32), &_local_ArrayElemRest::CmpVal, oIdx) == false);
// 	Test(oNarr.FindIndexOf(_local_ArrayElemRest(32), &_local_ArrayElemRest::CmpVal) == acpl::Num<acpl::SizeT>::Max());
// 	Test(oNarr.FindIndexOf<const _local_ArrayElemRest &>(_local_ArrayElemRest(32), &_local_ArrayElemRest::Ref, &_local_ArrayElemRest::CmpVal, oIdx) == false);
// 	Test(oNarr.FindIndexOf<const _local_ArrayElemRest &>(_local_ArrayElemRest(32), &_local_ArrayElemRest::Ref, &_local_ArrayElemRest::CmpVal) == acpl::Num<acpl::SizeT>::Max());
// 	Test(oCarr.FindIndexOf<const _local_ArrayElemRest &>(_local_ArrayElemRest(32), &_local_ArrayElemRest::Ref, &_local_ArrayElemRest::CmpVal, oIdx) == false);
// 	Test(oCarr.FindIndexOf<const _local_ArrayElemRest &>(_local_ArrayElemRest(32), &_local_ArrayElemRest::Ref, &_local_ArrayElemRest::CmpVal) == acpl::Num<acpl::SizeT>::Max());
// 	
// 	Test(oNarr.Find(_local_ArrayElemRest(32), &_local_ArrayElemRest::CmpVal, oElem) == false);
// 	Test(oNarr.Find<const _local_ArrayElemRest &>(_local_ArrayElemRest(32), &_local_ArrayElemRest::Ref, &_local_ArrayElemRest::CmpVal, oElem) == false);
// 	Test(oCarr.Find<const _local_ArrayElemRest &>(_local_ArrayElemRest(32), &_local_ArrayElemRest::Ref, &_local_ArrayElemRest::CmpVal, oElem) == false);
	
	Test(oNarr.Swap(0, 1) == false);
	Test(oNarr.Swap(0, oElem) == false);
	Test(oNarr.Swap(0, oElemCon, oElem) == false);
	Test(oNarr.Replace(0, oElemCon) == false);
	Test(oNarr.Discard(0, oElem) == false);
	Test(oNarr.Remove(0) == false);
	Test(oNarr.RemoveHead(1) == false);
	Test(oNarr.RemoveTail(1) == false);
	Test(oNarr.Remove(0, 1) == false);
	Test(oNarr.Move(0, 1) == false);
	
	Test(oNarr.Shrink() == true);
	oNarr.Clear();
	oNarr.Purge();
	
	
	// Add() overloads + Clear
	
	Test(oNarr.Add() == true);
	Test(oNarr.Count() == 1);
	Test(oCarr.Count() == 1);
	Test(oNarr.GetLast() == NULL);
	Test(oNarr.Add(oElem) == true);
	Test(oNarr.Count() == 2);
	Test(oNarr.GetLast() == NULL);
	Test(oNarr.Add(new _local_ArrayElemRest(64)) == true);
	Test(oNarr.Count() == 3);
	Test(oNarr.GetLast()->GetInt() == 64);
	Test(oNarr.Add(new _local_ArrayElemRest(64, 32)) == true);
	Test(oNarr.Count() == 4);
	Test(oNarr.GetLast()->GetInt() == 96);
	Test(oNarr.Add(new _local_ArrayElemRest(64, 32, 16)) == true);
	Test(oNarr.Count() == 5);
	Test(oNarr.GetLast()->GetInt() == 112);
	Test(oNarr.Add(new _local_ArrayElemRest(64, 32, 16, 8)) == true);
	Test(oNarr.Count() == 6);
	Test(oNarr.GetLast()->GetInt() == 120);
	Test(oNarr.Add(new _local_ArrayElemRest(64, 32, 16, 8, -2)) == true);
	Test(oNarr.Count() == 7);
	Test(oNarr.GetLast()->GetInt() == 118);
	Test(oNarr.Add(new _local_ArrayElemRest(64, 32, 16, 8, -2, -4)) == true);
	Test(oNarr.Count() == 8);
	Test(oNarr.GetLast()->GetInt() == 114);
	Test(oNarr.Add(new _local_ArrayElemRest(64, 32, 16, 8, -2, -4, -8)) == true);
	Test(oNarr.Count() == 9);
	Test(oNarr.GetLast()->GetInt() == 106);
	Test(oNarr.Add(new _local_ArrayElemRest(64, 32, 16, 8, -2, -4, -8, -16)) == true);
	Test(oNarr.Count() == 10);
	Test(oNarr.GetLast()->GetInt() == 90);
	Test(oNarr.Add(new _local_ArrayElemRest(64, 32, 16, 8, -2, -4, -8, -16, 8.5)) == true);
	Test(oNarr.Count() == 11);
	Test(oNarr.GetLast()->GetInt() == 98); // even though the result should have been 98.5
	Test(oNarr.Add(new _local_ArrayElemRest(64, 32, 16, 8, -2, -4, -8, -16, 8.5, static_cast<acpl::Float32>(-4.7))) == true);
	Test(oNarr.Count() == 12);
	Test(oNarr.GetLast()->GetInt() == 94); // even though the result should have been 93.8
	oNarr.Clear();
	Test(oNarr.Count() == 0);
	
	
	// Add() + Swap()s && Replace() + Clear()
	
	Test(oNarr.Add(new _local_ArrayElemRest(16)) == true);
	Test(oNarr.Add(new _local_ArrayElemRest(32)) == true);
	Test(oNarr.Count() == 2);
	Test(oNarr.Get(0)->GetInt() == 16);
	Test(oNarr.Get(1)->GetInt() == 32);
	Test(oNarr.Swap(0, 0) == true);
	Test(oNarr.Get(0)->GetInt() == 16);
	Test(oNarr.Get(1)->GetInt() == 32);
	Test(oNarr.Swap(0, 1) == true);
	Test(oNarr.Get(0)->GetInt() == 32);
	Test(oNarr.Get(1)->GetInt() == 16);
	Test(oNarr.Add(new _local_ArrayElemRest(64)) == true);
	Test(oNarr.Count() == 3);
	Test(oNarr.Get(0)->GetInt() == 32);
	Test(oNarr.Get(1)->GetInt() == 16);
	Test(oNarr.Get(2)->GetInt() == 64);
	Test(oNarr.Swap(0, 2) == true);
	Test(oNarr.Get(0)->GetInt() == 64);
	Test(oNarr.Get(1)->GetInt() == 16);
	Test(oNarr.Get(2)->GetInt() == 32);
	Test(oNarr.Swap(1, 2) == true);
	Test(oNarr.Get(0)->GetInt() == 64);
	Test(oNarr.Get(1)->GetInt() == 32);
	Test(oNarr.Get(2)->GetInt() == 16);
	Test(oNarr.Swap(2, 3) == false);
	Test(oNarr.Swap(3, 2) == false);
	
	Test(oNarr.Swap(0, oElemCon) == true);
	Test(oElemCon->GetInt() == 64);
	Test(oNarr.Get(0)->GetInt() == 32);
	Test(oNarr.Get(1)->GetInt() == 32);
	Test(oNarr.Get(2)->GetInt() == 16);
	Test(oNarr.Swap(1, oElemCon) == true);
	Test(oElemCon->GetInt() == 32);
	Test(oNarr.Get(0)->GetInt() == 32);
	Test(oNarr.Get(1)->GetInt() == 64);
	Test(oNarr.Get(2)->GetInt() == 16);
	Test(oNarr.Swap(2, oElemCon) == true);
	Test(oElemCon->GetInt() == 16);
	Test(oNarr.Get(0)->GetInt() == 32);
	Test(oNarr.Get(1)->GetInt() == 64);
	Test(oNarr.Get(2)->GetInt() == 32);
	Test(oNarr.Swap(3, oElemCon) == false);
	Test(oElemCon->GetInt() == 16);
	
	Test(oNarr.Swap(0, new _local_ArrayElemRest(16), oElem) == true);
	Test(oElem->GetInt() == 32);
	Test(oNarr.Get(0)->GetInt() == 16);
	Test(oNarr.Get(1)->GetInt() == 64);
	Test(oNarr.Get(2)->GetInt() == 32);
	acpl::Mem::Delete(oElem);
	Test(oNarr.Swap(1, new _local_ArrayElemRest(32), oElem) == true);
	Test(oElem->GetInt() == 64);
	Test(oNarr.Get(0)->GetInt() == 16);
	Test(oNarr.Get(1)->GetInt() == 32);
	Test(oNarr.Get(2)->GetInt() == 32);
	acpl::Mem::Delete(oElem);
	Test(oNarr.Swap(2, new _local_ArrayElemRest(64), oElem) == true);
	Test(oElem->GetInt() == 32);
	Test(oNarr.Get(0)->GetInt() == 16);
	Test(oNarr.Get(1)->GetInt() == 32);
	Test(oNarr.Get(2)->GetInt() == 64);
	// WARNING: Don't test this because it will be a memory leak. We know this works.
	// Test(oNarr.Swap(3, new _local_ArrayElemRest(128), oElem) == false);
	// Test(oElem->GetInt() == 32);
	acpl::Mem::Delete(oElem);
	
	Test(oNarr.Replace(0, new _local_ArrayElemRest(64)) == true);
	Test(oNarr.Get(0)->GetInt() == 64);
	Test(oNarr.Get(1)->GetInt() == 32);
	Test(oNarr.Get(2)->GetInt() == 64);
	Test(oNarr.Replace(1, new _local_ArrayElemRest(16)) == true);
	Test(oNarr.Get(0)->GetInt() == 64);
	Test(oNarr.Get(1)->GetInt() == 16);
	Test(oNarr.Get(2)->GetInt() == 64);
	Test(oNarr.Replace(2, new _local_ArrayElemRest(32)) == true);
	Test(oNarr.Get(0)->GetInt() == 64);
	Test(oNarr.Get(1)->GetInt() == 16);
	Test(oNarr.Get(2)->GetInt() == 32);
	// WARNING: Don't test this because it will be a memory leak. We know this works.
	// Test(oNarr.Replace(3, new _local_ArrayElemRest(128)) == false);
	
	oNarr.Clear();
	
	
	// Add() + Discard() && Remove() (classic) + Clear()
	
	Test(oNarr.Add(new _local_ArrayElemRest(16)) == true);
	Test(oNarr.Add(new _local_ArrayElemRest(32)) == true);
	Test(oNarr.Add(new _local_ArrayElemRest(64)) == true);
	Test(oNarr.Count() == 3);
	Test(oNarr.Discard(3, oElem) == false);
	Test(oNarr.Count() == 3);
	Test(oNarr.Get(0)->GetInt() == 16);
	Test(oNarr.Get(1)->GetInt() == 32);
	Test(oNarr.Get(2)->GetInt() == 64);
	Test(oNarr.Discard(0, oElem) == true);
	Test(oNarr.Count() == 2);
	Test(oNarr.Get(0)->GetInt() == 32);
	Test(oNarr.Get(1)->GetInt() == 64);
	Test(oElem->GetInt() == 16);
	acpl::Mem::Delete(oElem);
	Test(oNarr.Discard(1, oElem) == true);
	Test(oNarr.Count() == 1);
	Test(oNarr.Get(0)->GetInt() == 32);
	Test(oElem->GetInt() == 64);
	acpl::Mem::Delete(oElem);
	Test(oNarr.Discard(0, oElem) == true);
	Test(oNarr.Count() == 0);
	Test(oElem->GetInt() == 32);
	Test(oNarr.Discard(0, oElem) == false);
	Test(oNarr.Count() == 0);
	Test(oElem->GetInt() == 32);
	acpl::Mem::Delete(oElem);
	
	Test(oNarr.Add(new _local_ArrayElemRest(16)) == true);
	Test(oNarr.Add(new _local_ArrayElemRest(32)) == true);
	Test(oNarr.Add(new _local_ArrayElemRest(64)) == true);
	Test(oNarr.Count() == 3);
	Test(oNarr.Get(0)->GetInt() == 16);
	Test(oNarr.Get(1)->GetInt() == 32);
	Test(oNarr.Get(2)->GetInt() == 64);
	Test(oNarr.Remove(3) == false);
	Test(oNarr.Count() == 3);
	Test(oNarr.Get(0)->GetInt() == 16);
	Test(oNarr.Get(1)->GetInt() == 32);
	Test(oNarr.Get(2)->GetInt() == 64);
	Test(oNarr.Remove(0) == true);
	Test(oNarr.Count() == 2);
	Test(oNarr.Get(0)->GetInt() == 32);
	Test(oNarr.Get(1)->GetInt() == 64);
	Test(oNarr.Remove(1) == true);
	Test(oNarr.Count() == 1);
	Test(oNarr.Get(0)->GetInt() == 32);
	Test(oNarr.Remove(0) == true);
	Test(oNarr.Count() == 0);
	Test(oNarr.Remove(0) == false);
	Test(oNarr.Count() == 0);
	
	oNarr.Clear();
	
	
	// Add() + Remove{Head,Tail}() && Remove() (advanced) + Clear()
	
	Test(oNarr.RemoveHead(0) == true);
	Test(oNarr.Add(new _local_ArrayElemRest(16)) == true);
	Test(oNarr.Add(new _local_ArrayElemRest(32)) == true);
	Test(oNarr.Add(new _local_ArrayElemRest(64)) == true);
	Test(oNarr.Add(new _local_ArrayElemRest(128)) == true);
	Test(oNarr.Add(new _local_ArrayElemRest(256)) == true);
	Test(oNarr.RemoveHead(0) == true);
	Test(oNarr.Count() == 5);
	Test(oNarr.Get(0)->GetInt() == 16);
	Test(oNarr.Get(1)->GetInt() == 32);
	Test(oNarr.Get(2)->GetInt() == 64);
	Test(oNarr.Get(3)->GetInt() == 128);
	Test(oNarr.Get(4)->GetInt() == 256);
	Test(oNarr.RemoveHead(3) == true);
	Test(oNarr.Count() == 2);
	Test(oNarr.Get(0)->GetInt() == 128);
	Test(oNarr.Get(1)->GetInt() == 256);
	Test(oNarr.RemoveHead(2) == true);
	Test(oNarr.Count() == 0);
	Test(oNarr.Add(new _local_ArrayElemRest(16)) == true);
	Test(oNarr.Add(new _local_ArrayElemRest(32)) == true);
	Test(oNarr.Count() == 2);
	Test(oNarr.RemoveHead(3) == true);
	Test(oNarr.Count() == 0);
	
	Test(oNarr.RemoveTail(0) == true);
	Test(oNarr.Add(new _local_ArrayElemRest(16)) == true);
	Test(oNarr.Add(new _local_ArrayElemRest(32)) == true);
	Test(oNarr.Add(new _local_ArrayElemRest(64)) == true);
	Test(oNarr.Add(new _local_ArrayElemRest(128)) == true);
	Test(oNarr.Add(new _local_ArrayElemRest(256)) == true);
	Test(oNarr.RemoveTail(0) == true);
	Test(oNarr.Count() == 5);
	Test(oNarr.Get(0)->GetInt() == 16);
	Test(oNarr.Get(1)->GetInt() == 32);
	Test(oNarr.Get(2)->GetInt() == 64);
	Test(oNarr.Get(3)->GetInt() == 128);
	Test(oNarr.Get(4)->GetInt() == 256);
	Test(oNarr.RemoveTail(3) == true);
	Test(oNarr.Count() == 2);
	Test(oNarr.Get(0)->GetInt() == 16);
	Test(oNarr.Get(1)->GetInt() == 32);
	Test(oNarr.RemoveTail(2) == true);
	Test(oNarr.Count() == 0);
	Test(oNarr.Add(new _local_ArrayElemRest(16)) == true);
	Test(oNarr.Add(new _local_ArrayElemRest(32)) == true);
	Test(oNarr.Count() == 2);
	Test(oNarr.RemoveTail(3) == true);
	Test(oNarr.Count() == 0);
	
	Test(oNarr.Remove(0, 0) == true);
	Test(oNarr.Add(new _local_ArrayElemRest(16)) == true);
	Test(oNarr.Add(new _local_ArrayElemRest(32)) == true);
	Test(oNarr.Add(new _local_ArrayElemRest(64)) == true);
	Test(oNarr.Add(new _local_ArrayElemRest(128)) == true);
	Test(oNarr.Add(new _local_ArrayElemRest(256)) == true);
	Test(oNarr.Remove(0, 0) == true);
	Test(oNarr.Count() == 5);
	Test(oNarr.Get(0)->GetInt() == 16);
	Test(oNarr.Get(1)->GetInt() == 32);
	Test(oNarr.Get(2)->GetInt() == 64);
	Test(oNarr.Get(3)->GetInt() == 128);
	Test(oNarr.Get(4)->GetInt() == 256);
	Test(oNarr.Remove(1, 3) == true);
	Test(oNarr.Count() == 2);
	Test(oNarr.Get(0)->GetInt() == 16);
	Test(oNarr.Get(1)->GetInt() == 256);
	Test(oNarr.Add(new _local_ArrayElemRest(32)) == true);
	Test(oNarr.Add(new _local_ArrayElemRest(64)) == true);
	Test(oNarr.Add(new _local_ArrayElemRest(128)) == true);
	Test(oNarr.Remove(0, 3) == true);
	Test(oNarr.Count() == 2);
	Test(oNarr.Get(0)->GetInt() == 64);
	Test(oNarr.Get(1)->GetInt() == 128);
	Test(oNarr.Remove(0, 3) == true);
	Test(oNarr.Count() == 0);
	Test(oNarr.Add(new _local_ArrayElemRest(16)) == true);
	Test(oNarr.Add(new _local_ArrayElemRest(32)) == true);
	Test(oNarr.Add(new _local_ArrayElemRest(64)) == true);
	Test(oNarr.Add(new _local_ArrayElemRest(128)) == true);
	Test(oNarr.Add(new _local_ArrayElemRest(256)) == true);
	Test(oNarr.Count() == 5);
	Test(oNarr.Remove(4, 2) == true);
	Test(oNarr.Count() == 4);
	Test(oNarr.Get(0)->GetInt() == 16);
	Test(oNarr.Get(1)->GetInt() == 32);
	Test(oNarr.Get(2)->GetInt() == 64);
	Test(oNarr.Get(3)->GetInt() == 128);
	Test(oNarr.Remove(1, 3) == true);
	Test(oNarr.Count() == 1);
	Test(oNarr.Get(0)->GetInt() == 16);
	oNarr.Clear();
	
	
	// Add() + Move() + Clear()
	
	Test(oNarr.Move(0, 0) == false);
	Test(oNarr.Move(0, 1) == false);
	Test(oNarr.Move(1, 0) == false);
	Test(oNarr.Move(1, 1) == false);
	Test(oNarr.Add(new _local_ArrayElemRest(16)) == true);
	Test(oNarr.Add(new _local_ArrayElemRest(32)) == true);
	Test(oNarr.Add(new _local_ArrayElemRest(64)) == true);
	Test(oNarr.Count() == 3);
	Test(oNarr.Get(0)->GetInt() == 16);
	Test(oNarr.Get(1)->GetInt() == 32);
	Test(oNarr.Get(2)->GetInt() == 64);
	Test(oNarr.Move(0, 0) == true);
	Test(oNarr.Get(0)->GetInt() == 16);
	Test(oNarr.Get(1)->GetInt() == 32);
	Test(oNarr.Get(2)->GetInt() == 64);
	Test(oNarr.Move(0, 0) == true);
	Test(oNarr.Get(0)->GetInt() == 16);
	Test(oNarr.Get(1)->GetInt() == 32);
	Test(oNarr.Get(2)->GetInt() == 64);
	Test(oNarr.Move(0, 1) == true);
	Test(oNarr.Get(0)->GetInt() == 32);
	Test(oNarr.Get(1)->GetInt() == 16);
	Test(oNarr.Get(2)->GetInt() == 64);
	Test(oNarr.Move(1, 0) == true);
	Test(oNarr.Get(0)->GetInt() == 16);
	Test(oNarr.Get(1)->GetInt() == 32);
	Test(oNarr.Get(2)->GetInt() == 64);
	Test(oNarr.Move(2, 0) == true);
	Test(oNarr.Get(0)->GetInt() == 64);
	Test(oNarr.Get(1)->GetInt() == 16);
	Test(oNarr.Get(2)->GetInt() == 32);
	Test(oNarr.Move(0, 2) == true);
	Test(oNarr.Get(0)->GetInt() == 16);
	Test(oNarr.Get(1)->GetInt() == 32);
	Test(oNarr.Get(2)->GetInt() == 64);
	Test(oNarr.Move(0, 3) == true); // over limit
	Test(oNarr.Get(0)->GetInt() == 32);
	Test(oNarr.Get(1)->GetInt() == 64);
	Test(oNarr.Get(2)->GetInt() == 16);
	Test(oNarr.Move(1, 3) == true); // over limit
	Test(oNarr.Get(0)->GetInt() == 32);
	Test(oNarr.Get(1)->GetInt() == 16);
	Test(oNarr.Get(2)->GetInt() == 64);
	Test(oNarr.Move(2, 3) == true); // over limit
	Test(oNarr.Get(0)->GetInt() == 32);
	Test(oNarr.Get(1)->GetInt() == 16);
	Test(oNarr.Get(2)->GetInt() == 64);
	oNarr.Clear();
	
	
	// Add() + Sort() + Clear()
	
	oNarr.Sort(&_local_ArrayElemRest::CmpPtr);
	Test(oNarr.Add(new _local_ArrayElemRest(64)) == true);
	oNarr.Sort(&_local_ArrayElemRest::CmpPtr);
	Test(oNarr.Get(0)->GetInt() == 64);
	Test(oNarr.Add(new _local_ArrayElemRest(32)) == true);
	oNarr.Sort(&_local_ArrayElemRest::CmpPtr);
	Test(oNarr.Get(0)->GetInt() == 32);
	Test(oNarr.Get(1)->GetInt() == 64);
	Test(oNarr.Add(new _local_ArrayElemRest(16)) == true);
	oNarr.Sort(&_local_ArrayElemRest::CmpPtr);
	Test(oNarr.Get(0)->GetInt() == 16);
	Test(oNarr.Get(1)->GetInt() == 32);
	Test(oNarr.Get(2)->GetInt() == 64);
	oNarr.Clear();
	
	
	return 0;
}


SectionFuncMain(array)
{
	if (argc < 1)
	{
		Test(TestArrayNormVal() == 0);
		Test(TestArrayNormPtr() == 0);
		Test(TestArrayRestVal() == 0);
		Test(TestArrayRestPtr() == 0);
		
		PrintOut("All Passed!\n");
	}
	else
		return PrintUnrecParams(acplDbgSrcLoc, lSectionName, argc, argv);
	
	return 0;
}

SectionFuncParamsBegin(array)
// SectionFuncParamsAdd("param [param [param ...]]", "description")
SectionFuncParamsEnd(array)
