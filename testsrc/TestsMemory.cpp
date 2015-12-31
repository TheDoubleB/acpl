#include "Tests.h"

#include "../src/Memory.h"


static void _local_WriteBytes(void *nBfr, acpl::SizeT nBfrOffset, acpl::SizeT nBytesCount, acpl::SizeT nValOffset)
{
	acpl::UInt8 *oBfr = static_cast<acpl::UInt8 *>(nBfr) + nBfrOffset;
	while (nBytesCount-- != 0)
		oBfr[nBytesCount] = static_cast<acpl::UInt8>(nBytesCount + nValOffset);
}

static bool _local_TestBytes(const void *nBfr, acpl::SizeT nBfrOffset, acpl::SizeT nBytesCount, acpl::SizeT nValOffset)
{
	const acpl::UInt8 *oBfr = static_cast<const acpl::UInt8 *>(nBfr) + nBfrOffset;
	while (nBytesCount-- != 0)
		if (oBfr[nBytesCount] != static_cast<acpl::UInt8>(nBytesCount + nValOffset))
			return false;
	
	return true;
}

template <class tType, class tTypeCompat>
static bool _local_TestVals(const tType *nBfr, acpl::SizeT nBfrOffset, acpl::SizeT nCount, tTypeCompat nVal)
{
	while (nCount-- != 0)
		if (nBfr[nCount + nBfrOffset] != nVal)
			return false;
	
	return true;
}

static void _local_WriteBytes(acpl::UInt8 *nBfr, acpl::SizeT nBfrSize)
{
	static acpl::UInt8 oStatBfr[] = { 9, 2, 7, 13, 8, 27, 7, 6, 18 }; // 9
	while (nBfrSize--)
		nBfr[nBfrSize] = oStatBfr[nBfrSize % sizeof(oStatBfr)];
}

static class _local_TestByteIoChannel : public acpl::IoChannel
{
	public:
		virtual bool Read(void *nData, acpl::SizeT nDataSize, acpl::SizeT &nBytesRead)
		{
			_local_WriteBytes(nData, 0, nDataSize, 0);
			nBytesRead = nDataSize;
			return true;
		}
		virtual bool Write(const void *, acpl::SizeT nDataSize, acpl::SizeT &nBytesWritten)
		{
			nBytesWritten = nDataSize;
			return true;
		}
} lTestByteIoChannel;


static int TestBaseManagement()
{
	PrintFn();
	
	
	acpl::UInt8 *oPtr = NULL;
	
	
	// Alloc + Free
	Test((oPtr = static_cast<acpl::UInt8 *>(acpl::Mem::BaseAlloc(10))) != NULL);
	_local_WriteBytes(oPtr, 0, 10, 0);
	Test(_local_TestBytes(oPtr, 0, 10, 0) == true);
	acpl::Mem::BaseFree(oPtr);
	oPtr = NULL;
	
	
	// Realloc (with NULL pointer; works as Alloc) + Free
	Test((oPtr = static_cast<acpl::UInt8 *>(acpl::Mem::BaseRealloc(oPtr, 10))) != NULL);
	_local_WriteBytes(oPtr, 0, 10, 0);
	Test(_local_TestBytes(oPtr, 0, 10, 0) == true);
	acpl::Mem::BaseFree(oPtr);
	oPtr = NULL;
	
	// Alloc + Realloc (to bigger) + Free
	Test((oPtr = static_cast<acpl::UInt8 *>(acpl::Mem::BaseAlloc(10))) != NULL);
	_local_WriteBytes(oPtr, 0, 10, 0);
	Test(_local_TestBytes(oPtr, 0, 10, 0) == true);
	Test((oPtr = static_cast<acpl::UInt8 *>(acpl::Mem::BaseRealloc(oPtr, 20))) != NULL);
	Test(_local_TestBytes(oPtr, 0, 10, 0) == true);
	_local_WriteBytes(oPtr, 0, 20, 0);
	Test(_local_TestBytes(oPtr, 0, 20, 0) == true);
	acpl::Mem::BaseFree(oPtr);
	oPtr = NULL;
	
	// Alloc + Realloc (to smaller) + Free
	Test((oPtr = static_cast<acpl::UInt8 *>(acpl::Mem::BaseAlloc(20))) != NULL);
	_local_WriteBytes(oPtr, 0, 20, 0);
	Test(_local_TestBytes(oPtr, 0, 20, 0) == true);
	Test((oPtr = static_cast<acpl::UInt8 *>(acpl::Mem::BaseRealloc(oPtr, 10))) != NULL);
	Test(_local_TestBytes(oPtr, 0, 10, 0) == true);
	_local_WriteBytes(oPtr, 0, 10, 0);
	Test(_local_TestBytes(oPtr, 0, 10, 0) == true);
	acpl::Mem::BaseFree(oPtr);
	oPtr = NULL;
	
	// Alloc + Realloc (to 0; works as Free)
	Test((oPtr = static_cast<acpl::UInt8 *>(acpl::Mem::BaseAlloc(10))) != NULL);
	_local_WriteBytes(oPtr, 0, 10, 0);
	Test(_local_TestBytes(oPtr, 0, 10, 0) == true);
	Test((oPtr = static_cast<acpl::UInt8 *>(acpl::Mem::BaseRealloc(oPtr, 0))) == NULL);
	// No Free + NULLify needed
	
	
	// Resize (with NULL pointer; works as Alloc) + Free
	Test((oPtr = static_cast<acpl::UInt8 *>(acpl::Mem::BaseResize(oPtr, 0, 10, 0))) != NULL);
	_local_WriteBytes(oPtr, 0, 10, 0);
	Test(_local_TestBytes(oPtr, 0, 10, 0) == true);
	acpl::Mem::BaseFree(oPtr);
	oPtr = NULL;
	
	// Alloc + Resize (to bigger) + Free
	Test((oPtr = static_cast<acpl::UInt8 *>(acpl::Mem::BaseAlloc(10))) != NULL);
	_local_WriteBytes(oPtr, 0, 10, 0);
	Test(_local_TestBytes(oPtr, 0, 10, 0) == true);
	Test((oPtr = static_cast<acpl::UInt8 *>(acpl::Mem::BaseResize(oPtr, 10, 20, 0))) != NULL);
	Test(_local_TestBytes(oPtr, 0, 10, 0) == true);
	_local_WriteBytes(oPtr, 0, 20, 0);
	Test(_local_TestBytes(oPtr, 0, 20, 0) == true);
	acpl::Mem::BaseFree(oPtr);
	oPtr = NULL;
	
	// Alloc + Resize (to smaller) + Free
	Test((oPtr = static_cast<acpl::UInt8 *>(acpl::Mem::BaseAlloc(20))) != NULL);
	_local_WriteBytes(oPtr, 0, 20, 0);
	Test(_local_TestBytes(oPtr, 0, 20, 0) == true);
	Test((oPtr = static_cast<acpl::UInt8 *>(acpl::Mem::BaseResize(oPtr, 20, 10, 0))) != NULL);
	Test(_local_TestBytes(oPtr, 0, 10, 0) == true);
	_local_WriteBytes(oPtr, 0, 10, 0);
	Test(_local_TestBytes(oPtr, 0, 10, 0) == true);
	acpl::Mem::BaseFree(oPtr);
	oPtr = NULL;
	
	// Alloc + Resize (to 0; works as Free)
	Test((oPtr = static_cast<acpl::UInt8 *>(acpl::Mem::BaseAlloc(10))) != NULL);
	_local_WriteBytes(oPtr, 0, 10, 0);
	Test(_local_TestBytes(oPtr, 0, 10, 0) == true);
	Test((oPtr = static_cast<acpl::UInt8 *>(acpl::Mem::BaseResize(oPtr, 10, 0, 0))) == NULL);
	// No Free + NULLify needed
	
	
	// Alloc + Resize (to bigger with offset) + Free
	Test((oPtr = static_cast<acpl::UInt8 *>(acpl::Mem::BaseAlloc(10))) != NULL);
	_local_WriteBytes(oPtr, 0, 10, 0);
	Test(_local_TestBytes(oPtr, 0, 10, 0) == true);
	Test((oPtr = static_cast<acpl::UInt8 *>(acpl::Mem::BaseResize(oPtr, 10, 20, 5))) != NULL);
	_local_WriteBytes(oPtr, 5, 15, 10); // write the new space with succeeding values
	Test(_local_TestBytes(oPtr, 0, 20, 5) == true);
	acpl::Mem::BaseFree(oPtr);
	oPtr = NULL;
	
	// Alloc + Resize (to smaller with offset) + Free
	Test((oPtr = static_cast<acpl::UInt8 *>(acpl::Mem::BaseAlloc(20))) != NULL);
	_local_WriteBytes(oPtr, 0, 20, 0);
	Test(_local_TestBytes(oPtr, 0, 20, 0) == true);
	Test((oPtr = static_cast<acpl::UInt8 *>(acpl::Mem::BaseResize(oPtr, 20, 10, 5))) != NULL);
	// No writes needed; space now smaller
	Test(_local_TestBytes(oPtr, 0, 10, 5) == true);
	acpl::Mem::BaseFree(oPtr);
	oPtr = NULL;
	
	
	// Alloc, Realloc, Resize to SIZE_MAX (fail scenarios)
#if (acplCRuntime != acplCRuntimeMscrt) // Don't try to overallocate memory in MSCRT
	Test((oPtr = static_cast<acpl::UInt8 *>(acpl::Mem::BaseAlloc(acpl::Num<acpl::SizeT>::Max()))) == NULL);
	Test((oPtr = static_cast<acpl::UInt8 *>(acpl::Mem::BaseRealloc(oPtr, acpl::Num<acpl::SizeT>::Max()))) == NULL);
	Test((oPtr = static_cast<acpl::UInt8 *>(acpl::Mem::BaseResize(oPtr, 0, acpl::Num<acpl::SizeT>::Max(), 0))) == NULL);
#endif
	
	
	return 0;
}

static int TestBaseManagementAligned()
{
	PrintFn();
	
	
	acpl::UInt8 *oPtr = NULL;
	
	
	// Alloc + Free
	Test((oPtr = static_cast<acpl::UInt8 *>(acpl::Mem::BaseAllocAligned(64, 16))) != NULL);
	Test((reinterpret_cast<acpl::SizeT>(oPtr) % 64) == 0);
	_local_WriteBytes(oPtr, 0, 16, 0);
	Test(_local_TestBytes(oPtr, 0, 16, 0) == true);
	acpl::Mem::BaseFreeAligned(oPtr);
	oPtr = NULL;
	
	
	// Realloc (with NULL pointer; works as Alloc) + Free
	Test((oPtr = static_cast<acpl::UInt8 *>(acpl::Mem::BaseReallocAligned(oPtr, 0, 64, 16))) != NULL);
	Test((reinterpret_cast<acpl::SizeT>(oPtr) % 64) == 0);
	_local_WriteBytes(oPtr, 0, 16, 0);
	Test(_local_TestBytes(oPtr, 0, 16, 0) == true);
	acpl::Mem::BaseFreeAligned(oPtr);
	oPtr = NULL;
	
	// Alloc + Realloc (to bigger) + Free
	Test((oPtr = static_cast<acpl::UInt8 *>(acpl::Mem::BaseAllocAligned(64, 16))) != NULL);
	Test((reinterpret_cast<acpl::SizeT>(oPtr) % 64) == 0);
	_local_WriteBytes(oPtr, 0, 16, 0);
	Test(_local_TestBytes(oPtr, 0, 16, 0) == true);
	Test((oPtr = static_cast<acpl::UInt8 *>(acpl::Mem::BaseReallocAligned(oPtr, 16, 64, 32))) != NULL);
	Test((reinterpret_cast<acpl::SizeT>(oPtr) % 64) == 0);
	Test(_local_TestBytes(oPtr, 0, 16, 0) == true);
	_local_WriteBytes(oPtr, 0, 32, 0);
	Test(_local_TestBytes(oPtr, 0, 32, 0) == true);
	acpl::Mem::BaseFreeAligned(oPtr);
	oPtr = NULL;
	
	// Alloc + Realloc (to smaller) + Free
	Test((oPtr = static_cast<acpl::UInt8 *>(acpl::Mem::BaseAllocAligned(64, 32))) != NULL);
	Test((reinterpret_cast<acpl::SizeT>(oPtr) % 64) == 0);
	_local_WriteBytes(oPtr, 0, 32, 0);
	Test(_local_TestBytes(oPtr, 0, 32, 0) == true);
	Test((oPtr = static_cast<acpl::UInt8 *>(acpl::Mem::BaseReallocAligned(oPtr, 32, 64, 16))) != NULL);
	Test((reinterpret_cast<acpl::SizeT>(oPtr) % 64) == 0);
	Test(_local_TestBytes(oPtr, 0, 16, 0) == true);
	_local_WriteBytes(oPtr, 0, 16, 0);
	Test(_local_TestBytes(oPtr, 0, 16, 0) == true);
	acpl::Mem::BaseFreeAligned(oPtr);
	oPtr = NULL;
	
	// Alloc + Realloc (to 0; works as Free)
	Test((oPtr = static_cast<acpl::UInt8 *>(acpl::Mem::BaseAllocAligned(64, 16))) != NULL);
	Test((reinterpret_cast<acpl::SizeT>(oPtr) % 64) == 0);
	_local_WriteBytes(oPtr, 0, 16, 0);
	Test(_local_TestBytes(oPtr, 0, 16, 0) == true);
	Test((oPtr = static_cast<acpl::UInt8 *>(acpl::Mem::BaseReallocAligned(oPtr, 16, 64, 0))) == NULL);
	// No Free + NULLify needed
	
	
	// Resize (with NULL pointer; works as Alloc) + Free
	Test((oPtr = static_cast<acpl::UInt8 *>(acpl::Mem::BaseResizeAligned(oPtr, 0, 64, 16, 0))) != NULL);
	Test((reinterpret_cast<acpl::SizeT>(oPtr) % 64) == 0);
	_local_WriteBytes(oPtr, 0, 16, 0);
	Test(_local_TestBytes(oPtr, 0, 16, 0) == true);
	acpl::Mem::BaseFreeAligned(oPtr);
	oPtr = NULL;
	
	// Alloc + Resize (to bigger) + Free
	Test((oPtr = static_cast<acpl::UInt8 *>(acpl::Mem::BaseAllocAligned(64, 16))) != NULL);
	Test((reinterpret_cast<acpl::SizeT>(oPtr) % 64) == 0);
	_local_WriteBytes(oPtr, 0, 16, 0);
	Test(_local_TestBytes(oPtr, 0, 16, 0) == true);
	Test((oPtr = static_cast<acpl::UInt8 *>(acpl::Mem::BaseResizeAligned(oPtr, 16, 64, 32, 0))) != NULL);
	Test((reinterpret_cast<acpl::SizeT>(oPtr) % 64) == 0);
	Test(_local_TestBytes(oPtr, 0, 16, 0) == true);
	_local_WriteBytes(oPtr, 0, 32, 0);
	Test(_local_TestBytes(oPtr, 0, 32, 0) == true);
	acpl::Mem::BaseFreeAligned(oPtr);
	oPtr = NULL;
	
	// Alloc + Resize (to smaller) + Free
	Test((oPtr = static_cast<acpl::UInt8 *>(acpl::Mem::BaseAllocAligned(64, 32))) != NULL);
	Test((reinterpret_cast<acpl::SizeT>(oPtr) % 64) == 0);
	_local_WriteBytes(oPtr, 0, 32, 0);
	Test(_local_TestBytes(oPtr, 0, 32, 0) == true);
	Test((oPtr = static_cast<acpl::UInt8 *>(acpl::Mem::BaseResizeAligned(oPtr, 32, 64, 16, 0))) != NULL);
	Test((reinterpret_cast<acpl::SizeT>(oPtr) % 64) == 0);
	Test(_local_TestBytes(oPtr, 0, 16, 0) == true);
	_local_WriteBytes(oPtr, 0, 16, 0);
	Test(_local_TestBytes(oPtr, 0, 16, 0) == true);
	acpl::Mem::BaseFreeAligned(oPtr);
	oPtr = NULL;
	
	// Alloc + Resize (to 0; works as Free)
	Test((oPtr = static_cast<acpl::UInt8 *>(acpl::Mem::BaseAllocAligned(64, 16))) != NULL);
	Test((reinterpret_cast<acpl::SizeT>(oPtr) % 64) == 0);
	_local_WriteBytes(oPtr, 0, 16, 0);
	Test(_local_TestBytes(oPtr, 0, 16, 0) == true);
	Test((oPtr = static_cast<acpl::UInt8 *>(acpl::Mem::BaseResizeAligned(oPtr, 16, 64, 0, 0))) == NULL);
	// No Free + NULLify needed
	
	
	// Alloc + Resize (to bigger with offset) + Free
	Test((oPtr = static_cast<acpl::UInt8 *>(acpl::Mem::BaseAllocAligned(64, 16))) != NULL);
	Test((reinterpret_cast<acpl::SizeT>(oPtr) % 64) == 0);
	_local_WriteBytes(oPtr, 0, 16, 0);
	Test(_local_TestBytes(oPtr, 0, 16, 0) == true);
	Test((oPtr = static_cast<acpl::UInt8 *>(acpl::Mem::BaseResizeAligned(oPtr, 16, 64, 32, 8))) != NULL);
	Test((reinterpret_cast<acpl::SizeT>(oPtr) % 64) == 0);
	_local_WriteBytes(oPtr, 8, 24, 16); // write the new space with succeeding values
	Test(_local_TestBytes(oPtr, 0, 32, 8) == true);
	acpl::Mem::BaseFreeAligned(oPtr);
	oPtr = NULL;
	
	// Alloc + Resize (to smaller with offset) + Free
	Test((oPtr = static_cast<acpl::UInt8 *>(acpl::Mem::BaseAllocAligned(64, 32))) != NULL);
	Test((reinterpret_cast<acpl::SizeT>(oPtr) % 64) == 0);
	_local_WriteBytes(oPtr, 0, 32, 0);
	Test(_local_TestBytes(oPtr, 0, 32, 0) == true);
	Test((oPtr = static_cast<acpl::UInt8 *>(acpl::Mem::BaseResizeAligned(oPtr, 32, 64, 16, 8))) != NULL);
	Test((reinterpret_cast<acpl::SizeT>(oPtr) % 64) == 0);
	// No writes needed; space now smaller
	Test(_local_TestBytes(oPtr, 0, 16, 8) == true);
	acpl::Mem::BaseFreeAligned(oPtr);
	oPtr = NULL;
	
	
	// Alloc, Realloc, Resize to SIZE_MAX (fail scenarios)
#if (acplCRuntime != acplCRuntimeMscrt) // Don't try to overallocate memory in MSCRT
	Test((oPtr = static_cast<acpl::UInt8 *>(acpl::Mem::BaseAllocAligned(64, acpl::Num<acpl::SizeT>::Max()))) == NULL);
	Test((oPtr = static_cast<acpl::UInt8 *>(acpl::Mem::BaseReallocAligned(oPtr, 0, 64, acpl::Num<acpl::SizeT>::Max()))) == NULL);
	Test((oPtr = static_cast<acpl::UInt8 *>(acpl::Mem::BaseResizeAligned(oPtr, 0, 64, acpl::Num<acpl::SizeT>::Max(), 0))) == NULL);
#endif
	
	// Alloc, Realloc, Resize with SIZET_MAX alignment (fail scenarios)
#if (acplCRuntime != acplCRuntimeMscrt) // Don't try to overallocate memory in MSCRT
	Test((oPtr = static_cast<acpl::UInt8 *>(acpl::Mem::BaseAllocAligned(acpl::Num<acpl::SizeT>::Max(), 16))) == NULL);
	Test((oPtr = static_cast<acpl::UInt8 *>(acpl::Mem::BaseReallocAligned(oPtr, 0, acpl::Num<acpl::SizeT>::Max(), 16))) == NULL);
	Test((oPtr = static_cast<acpl::UInt8 *>(acpl::Mem::BaseResizeAligned(oPtr, 0, acpl::Num<acpl::SizeT>::Max(), 16, 0))) == NULL);
#endif
	
	
	return 0;
}

static int TestBaseMovement()
{
	PrintFn();
	
	
	acpl::UInt8 oBfr[10];
	
	
	// Memcopy 5 bytes from +0 location to +5 location
	_local_WriteBytes(oBfr, 0, 10, 0);
	acpl::Mem::BaseCopy((oBfr + 5), oBfr, 5);
	Test(_local_TestBytes(oBfr, 5, 5, 0) == true);
	
// 	// Memcopy 9 bytes from +0 location to +1 location
// 	// WARNING: Don't test this because results should be unpredictable due to overlapping!
// 	_local_WriteBytes(oBfr, 0, 10, 0);
// 	acpl::Mem::BaseCopy((oBfr + 1), oBfr, 9);
// 	Test(_local_TestBytes(oBfr, 1, 9, 0) == false);
	
// 	// Memcopy 9 bytes from +1 location to +0 location
// 	// WARNING: Don't test this because results should be unpredictable due to overlapping!
// 	_local_WriteBytes(oBfr, 0, 10, 0);
// 	acpl::Mem::BaseCopy(oBfr, (oBfr + 1), 9);
// 	Test(_local_TestBytes(oBfr, 0, 9, 1) == false);
	
	
	// Memmove 5 bytes from +0 location to +5 location
	_local_WriteBytes(oBfr, 0, 10, 0);
	acpl::Mem::BaseMove((oBfr + 5), oBfr, 5);
	Test(_local_TestBytes(oBfr, 5, 5, 0) == true);
	
	// Memmove 9 bytes from +0 location to +1 location
	_local_WriteBytes(oBfr, 0, 10, 0);
	acpl::Mem::BaseMove((oBfr + 1), oBfr, 9);
	Test(_local_TestBytes(oBfr, 1, 9, 0) == true);
	
	// Memmove 9 bytes from +1 location to +0 location
	_local_WriteBytes(oBfr, 0, 10, 0);
	acpl::Mem::BaseMove(oBfr, (oBfr + 1), 9);
	Test(_local_TestBytes(oBfr, 0, 9, 1) == true);
	
	
	// Memset a buffer to value 0x47
	acpl::Mem::BaseSet(oBfr, 0x47, 10);
	for (acpl::UInt8 i = 0; i < 10; i++)
		TestFM(oBfr[i] == 0x47, "i=%u", i);
	
	
	return 0;
}

static int TestManagement()
{
	PrintFn();
	
	
	acpl::UInt8  *oPtr8  = NULL;
	acpl::UInt16 *oPtr16 = NULL;
	acpl::UInt32 *oPtr32 = NULL;
	acpl::UInt64 *oPtr64 = NULL;
	
	
	Test(acpl::Mem::Alloc(oPtr8, 10) == true);
	Test(oPtr8 != NULL);
	_local_WriteBytes(oPtr8, 0, (10 * sizeof(*oPtr8)), 0);
	Test(_local_TestBytes(oPtr8, 0, (10 * sizeof(*oPtr8)), 0) == true);
	
	Test(acpl::Mem::Realloc(oPtr8, 20) == true);
	Test(oPtr8 != NULL);
	Test(_local_TestBytes(oPtr8, 0, (10 * sizeof(*oPtr8)), 0) == true);
	_local_WriteBytes(oPtr8, 0, (20 * sizeof(*oPtr8)), 0);
	Test(_local_TestBytes(oPtr8, 0, (20 * sizeof(*oPtr8)), 0) == true);
	
	Test(acpl::Mem::Resize(oPtr8, 20, 40, 10) == true);
	Test(oPtr8 != NULL);
	Test(_local_TestBytes(oPtr8, 0, (10 * sizeof(*oPtr8)), (10 * sizeof(*oPtr8))) == true);
	_local_WriteBytes(oPtr8, 0, (40 * sizeof(*oPtr8)), 0);
	Test(_local_TestBytes(oPtr8, 0, (40 * sizeof(*oPtr8)), 0) == true);
	
	acpl::Mem::Free(oPtr8);
	Test(oPtr8 == NULL);
	acpl::Mem::Free(oPtr8); // even though the oPtr8 is NULL `acpl::Mem::Free` handles that
	
	
	Test(acpl::Mem::Alloc(oPtr16, 10) == true);
	Test(oPtr16 != NULL);
	_local_WriteBytes(oPtr16, 0, (10 * sizeof(*oPtr16)), 0);
	Test(_local_TestBytes(oPtr16, 0, (10 * sizeof(*oPtr16)), 0) == true);
	
	Test(acpl::Mem::Realloc(oPtr16, 20) == true);
	Test(oPtr16 != NULL);
	Test(_local_TestBytes(oPtr16, 0, (10 * sizeof(*oPtr16)), 0) == true);
	_local_WriteBytes(oPtr16, 0, (20 * sizeof(*oPtr16)), 0);
	Test(_local_TestBytes(oPtr16, 0, (20 * sizeof(*oPtr16)), 0) == true);
	
	Test(acpl::Mem::Resize(oPtr16, 20, 40, 10) == true);
	Test(oPtr16 != NULL);
	Test(_local_TestBytes(oPtr16, 0, (10 * sizeof(*oPtr16)), (10 * sizeof(*oPtr16))) == true);
	_local_WriteBytes(oPtr16, 0, (40 * sizeof(*oPtr16)), 0);
	Test(_local_TestBytes(oPtr16, 0, (40 * sizeof(*oPtr16)), 0) == true);
	
	acpl::Mem::Free(oPtr16);
	Test(oPtr16 == NULL);
	acpl::Mem::Free(oPtr16); // even though the oPtr16 is NULL `acpl::Mem::Free` handles that
	
	
	Test(acpl::Mem::Alloc(oPtr32, 10) == true);
	Test(oPtr32 != NULL);
	_local_WriteBytes(oPtr32, 0, (10 * sizeof(*oPtr32)), 0);
	Test(_local_TestBytes(oPtr32, 0, (10 * sizeof(*oPtr32)), 0) == true);
	
	Test(acpl::Mem::Realloc(oPtr32, 20) == true);
	Test(oPtr32 != NULL);
	Test(_local_TestBytes(oPtr32, 0, (10 * sizeof(*oPtr32)), 0) == true);
	_local_WriteBytes(oPtr32, 0, (20 * sizeof(*oPtr32)), 0);
	Test(_local_TestBytes(oPtr32, 0, (20 * sizeof(*oPtr32)), 0) == true);
	
	Test(acpl::Mem::Resize(oPtr32, 20, 40, 10) == true);
	Test(oPtr32 != NULL);
	Test(_local_TestBytes(oPtr32, 0, (10 * sizeof(*oPtr32)), (10 * sizeof(*oPtr32))) == true);
	_local_WriteBytes(oPtr32, 0, (40 * sizeof(*oPtr32)), 0);
	Test(_local_TestBytes(oPtr32, 0, (40 * sizeof(*oPtr32)), 0) == true);
	
	acpl::Mem::Free(oPtr32);
	Test(oPtr32 == NULL);
	acpl::Mem::Free(oPtr32); // even though the oPtr32 is NULL `acpl::Mem::Free` handles that
	
	
	Test(acpl::Mem::Alloc(oPtr64, 10) == true);
	Test(oPtr64 != NULL);
	_local_WriteBytes(oPtr64, 0, (10 * sizeof(*oPtr64)), 0);
	Test(_local_TestBytes(oPtr64, 0, (10 * sizeof(*oPtr64)), 0) == true);
	
	Test(acpl::Mem::Realloc(oPtr64, 20) == true);
	Test(oPtr64 != NULL);
	Test(_local_TestBytes(oPtr64, 0, (10 * sizeof(*oPtr64)), 0) == true);
	_local_WriteBytes(oPtr64, 0, (20 * sizeof(*oPtr64)), 0);
	Test(_local_TestBytes(oPtr64, 0, (20 * sizeof(*oPtr64)), 0) == true);
	
	Test(acpl::Mem::Resize(oPtr64, 20, 40, 10) == true);
	Test(oPtr64 != NULL);
	Test(_local_TestBytes(oPtr64, 0, (10 * sizeof(*oPtr64)), (10 * sizeof(*oPtr64))) == true);
	_local_WriteBytes(oPtr64, 0, (40 * sizeof(*oPtr64)), 0);
	Test(_local_TestBytes(oPtr64, 0, (40 * sizeof(*oPtr64)), 0) == true);
	
	acpl::Mem::Free(oPtr64);
	Test(oPtr64 == NULL);
	acpl::Mem::Free(oPtr64); // even though the oPtr64 is NULL `acpl::Mem::Free` handles that
	
	
	// Alloc, Realloc, Resize to SIZE_MAX (fail scenarios)
#if (acplCRuntime != acplCRuntimeMscrt) // Don't try to overallocate memory in MSCRT
	oPtr8 = reinterpret_cast<acpl::UInt8 *>(0x47); // A fake pointer used for checking the untouchedness
	Test(acpl::Mem::Alloc(oPtr8, acpl::Num<acpl::SizeT>::Max()) == false);
	Test(oPtr8 == reinterpret_cast<acpl::UInt8 *>(0x47));
	oPtr8 = NULL;
	Test(acpl::Mem::Realloc(oPtr8, acpl::Num<acpl::SizeT>::Max()) == false);
	Test(oPtr8 == NULL);
	Test(acpl::Mem::Resize(oPtr8, 0, acpl::Num<acpl::SizeT>::Max(), 0) == false);
	Test(oPtr8 == NULL);
#endif
	
	
	return 0;
}

static int TestManagementAligned()
{
	PrintFn();
	
	
	acpl::UInt8  *oPtr8  = NULL;
	acpl::UInt16 *oPtr16 = NULL;
	acpl::UInt32 *oPtr32 = NULL;
	acpl::UInt64 *oPtr64 = NULL;
	
	
	Test(acpl::Mem::AllocAligned(oPtr8, 8, 16) == true);
	Test(oPtr8 != NULL);
	_local_WriteBytes(oPtr8, 0, (16 * sizeof(*oPtr8)), 0);
	Test(_local_TestBytes(oPtr8, 0, (16 * sizeof(*oPtr8)), 0) == true);
	
	Test(acpl::Mem::ReallocAligned(oPtr8, 16, 8, 32) == true);
	Test(oPtr8 != NULL);
	Test(_local_TestBytes(oPtr8, 0, (16 * sizeof(*oPtr8)), 0) == true);
	_local_WriteBytes(oPtr8, 0, (32 * sizeof(*oPtr8)), 0);
	Test(_local_TestBytes(oPtr8, 0, (32 * sizeof(*oPtr8)), 0) == true);
	
	Test(acpl::Mem::ResizeAligned(oPtr8, 32, 8, 64, 16) == true);
	Test(oPtr8 != NULL);
	Test(_local_TestBytes(oPtr8, 0, (16 * sizeof(*oPtr8)), (16 * sizeof(*oPtr8))) == true);
	_local_WriteBytes(oPtr8, 0, (64 * sizeof(*oPtr8)), 0);
	Test(_local_TestBytes(oPtr8, 0, (64 * sizeof(*oPtr8)), 0) == true);
	
	acpl::Mem::FreeAligned(oPtr8);
	Test(oPtr8 == NULL);
	acpl::Mem::FreeAligned(oPtr8); // even though the oPtr8 is NULL `acpl::Mem::FreeAligned` handles that
	
	
	Test(acpl::Mem::AllocAligned(oPtr16, 8, 16) == true);
	Test(oPtr16 != NULL);
	_local_WriteBytes(oPtr16, 0, (16 * sizeof(*oPtr16)), 0);
	Test(_local_TestBytes(oPtr16, 0, (16 * sizeof(*oPtr16)), 0) == true);
	
	Test(acpl::Mem::ReallocAligned(oPtr16, 16, 8, 32) == true);
	Test(oPtr16 != NULL);
	Test(_local_TestBytes(oPtr16, 0, (16 * sizeof(*oPtr16)), 0) == true);
	_local_WriteBytes(oPtr16, 0, (32 * sizeof(*oPtr16)), 0);
	Test(_local_TestBytes(oPtr16, 0, (32 * sizeof(*oPtr16)), 0) == true);
	
	Test(acpl::Mem::ResizeAligned(oPtr16, 32, 8, 64, 16) == true);
	Test(oPtr16 != NULL);
	Test(_local_TestBytes(oPtr16, 0, (16 * sizeof(*oPtr16)), (16 * sizeof(*oPtr16))) == true);
	_local_WriteBytes(oPtr16, 0, (64 * sizeof(*oPtr16)), 0);
	Test(_local_TestBytes(oPtr16, 0, (64 * sizeof(*oPtr16)), 0) == true);
	
	acpl::Mem::FreeAligned(oPtr16);
	Test(oPtr16 == NULL);
	acpl::Mem::FreeAligned(oPtr16); // even though the oPtr8 is NULL `acpl::Mem::FreeAligned` handles that
	
	
	Test(acpl::Mem::AllocAligned(oPtr32, 8, 16) == true);
	Test(oPtr32 != NULL);
	_local_WriteBytes(oPtr32, 0, (16 * sizeof(*oPtr32)), 0);
	Test(_local_TestBytes(oPtr32, 0, (16 * sizeof(*oPtr32)), 0) == true);
	
	Test(acpl::Mem::ReallocAligned(oPtr32, 16, 8, 32) == true);
	Test(oPtr32 != NULL);
	Test(_local_TestBytes(oPtr32, 0, (16 * sizeof(*oPtr32)), 0) == true);
	_local_WriteBytes(oPtr32, 0, (32 * sizeof(*oPtr32)), 0);
	Test(_local_TestBytes(oPtr32, 0, (32 * sizeof(*oPtr32)), 0) == true);
	
	Test(acpl::Mem::ResizeAligned(oPtr32, 32, 8, 64, 16) == true);
	Test(oPtr32 != NULL);
	Test(_local_TestBytes(oPtr32, 0, (16 * sizeof(*oPtr32)), (16 * sizeof(*oPtr32))) == true);
	_local_WriteBytes(oPtr32, 0, (64 * sizeof(*oPtr32)), 0);
	Test(_local_TestBytes(oPtr32, 0, (64 * sizeof(*oPtr32)), 0) == true);
	
	acpl::Mem::FreeAligned(oPtr32);
	Test(oPtr32 == NULL);
	acpl::Mem::FreeAligned(oPtr32); // even though the oPtr8 is NULL `acpl::Mem::FreeAligned` handles that
	
	
	Test(acpl::Mem::AllocAligned(oPtr64, 8, 16) == true);
	Test(oPtr64 != NULL);
	_local_WriteBytes(oPtr64, 0, (16 * sizeof(*oPtr64)), 0);
	Test(_local_TestBytes(oPtr64, 0, (16 * sizeof(*oPtr64)), 0) == true);
	
	Test(acpl::Mem::ReallocAligned(oPtr64, 16, 8, 32) == true);
	Test(oPtr64 != NULL);
	Test(_local_TestBytes(oPtr64, 0, (16 * sizeof(*oPtr64)), 0) == true);
	_local_WriteBytes(oPtr64, 0, (32 * sizeof(*oPtr64)), 0);
	Test(_local_TestBytes(oPtr64, 0, (32 * sizeof(*oPtr64)), 0) == true);
	
	Test(acpl::Mem::ResizeAligned(oPtr64, 32, 8, 64, 16) == true);
	Test(oPtr64 != NULL);
	Test(_local_TestBytes(oPtr64, 0, (16 * sizeof(*oPtr64)), (16 * sizeof(*oPtr64))) == true);
	_local_WriteBytes(oPtr64, 0, (64 * sizeof(*oPtr64)), 0);
	Test(_local_TestBytes(oPtr64, 0, (64 * sizeof(*oPtr64)), 0) == true);
	
	acpl::Mem::FreeAligned(oPtr64);
	Test(oPtr64 == NULL);
	acpl::Mem::FreeAligned(oPtr64); // even though the oPtr8 is NULL `acpl::Mem::FreeAligned` handles that
	
	
	// Alloc, Realloc, Resize to SIZE_MAX (fail scenarios)
#if (acplCRuntime != acplCRuntimeMscrt) // Don't try to overallocate memory in MSCRT
	oPtr8 = reinterpret_cast<acpl::UInt8 *>(0x47); // A fake pointer used for checking the untouchedness
	Test(acpl::Mem::AllocAligned(oPtr8, 8, acpl::Num<acpl::SizeT>::Max()) == false);
	Test(oPtr8 == reinterpret_cast<acpl::UInt8 *>(0x47));
	Test(acpl::Mem::ReallocAligned(oPtr8, 0, 8, acpl::Num<acpl::SizeT>::Max()) == false);
	Test(oPtr8 == reinterpret_cast<acpl::UInt8 *>(0x47));
	Test(acpl::Mem::ResizeAligned(oPtr8, 0, 8, acpl::Num<acpl::SizeT>::Max(), 0) == false);
	Test(oPtr8 == reinterpret_cast<acpl::UInt8 *>(0x47));
	oPtr8 = NULL; // Restore the pointer to a more comfortable NULL
#endif
	
	
	// Alloc, Realloc, Resize with SIZET_MAX alignment (fail scenarios)
#if (acplCRuntime != acplCRuntimeMscrt) // Don't try to overallocate memory in MSCRT
	oPtr8 = reinterpret_cast<acpl::UInt8 *>(0x47); // A fake pointer used for checking the untouchedness
	Test(acpl::Mem::AllocAligned(oPtr8, acpl::Num<acpl::SizeT>::Max(), 64) == false);
	Test(oPtr8 == reinterpret_cast<acpl::UInt8 *>(0x47));
	Test(acpl::Mem::ReallocAligned(oPtr8, 0, acpl::Num<acpl::SizeT>::Max(), 64) == false);
	Test(oPtr8 == reinterpret_cast<acpl::UInt8 *>(0x47));
	Test(acpl::Mem::ResizeAligned(oPtr8, 0, acpl::Num<acpl::SizeT>::Max(), 64, 0) == false);
	Test(oPtr8 == reinterpret_cast<acpl::UInt8 *>(0x47));
	oPtr8 = NULL; // Restore the pointer to a more comfortable NULL
#endif
	
	
	return 0;
}

static int TestMovement()
{
	PrintFn();
	
	
	acpl::UInt32 oBfr[10];
	
	
	// Memcopy 5 bytes from +0 location to +5 location
	_local_WriteBytes(oBfr, 0, (10 * sizeof(*oBfr)), 0);
	acpl::Mem::Copy((oBfr + 5), oBfr, 5);
	Test(_local_TestBytes(oBfr, (5 * sizeof(*oBfr)), (5 * sizeof(*oBfr)), 0) == true);
	
// 	// Memcopy 9 bytes from +0 location to +1 location
// 	// WARNING: Don't test this because results should be unpredictable due to overlapping!
// 	_local_WriteBytes(oBfr, 0, (10 * sizeof(*oBfr)), 0);
// 	acpl::Mem::Copy((oBfr + 1), oBfr, 9);
// 	Test(_local_TestBytes(oBfr, (1 * sizeof(*oBfr)), (9 * sizeof(*oBfr)), 0) == false);
	
// 	// Memcopy 9 bytes from +1 location to +0 location
// 	// WARNING: Don't test this because results should be unpredictable due to overlapping!
// 	_local_WriteBytes(oBfr, 0, (10 * sizeof(*oBfr)), 0);
// 	acpl::Mem::Copy(oBfr, (oBfr + 1), 9);
// 	Test(_local_TestBytes(oBfr, 0, (9 * sizeof(*oBfr)), (1 * sizeof(*oBfr))) == false);
	
	
	// Memmove 5 bytes from +0 location to +5 location
	_local_WriteBytes(oBfr, 0, (10 * sizeof(*oBfr)), 0);
	acpl::Mem::Move((oBfr + 5), oBfr, 5);
	Test(_local_TestBytes(oBfr, (5 * sizeof(*oBfr)), (5 * sizeof(*oBfr)), 0) == true);
	
	// Memmove 9 bytes from +0 location to +1 location
	_local_WriteBytes(oBfr, 0, (10 * sizeof(*oBfr)), 0);
	acpl::Mem::Move((oBfr + 1), oBfr, 9);
	Test(_local_TestBytes(oBfr, (1 * sizeof(*oBfr)), (9 * sizeof(*oBfr)), 0) == true);
	
	// Memmove 9 bytes from +1 location to +0 location
	_local_WriteBytes(oBfr, 0, (10 * sizeof(*oBfr)), 0);
	acpl::Mem::Move(oBfr, (oBfr + 1), 9);
	Test(_local_TestBytes(oBfr, 0, (9 * sizeof(*oBfr)), (1 * sizeof(*oBfr))) == true);
	
	
	// Memset to TYPE value 0x47
	acpl::Mem::Set(oBfr, 0x47, 10);
	for (acpl::UInt8 i = 0; i < 10; i++)
		TestFM(oBfr[i] == 0x00000047, "i=%u", i);
	
	// Memset to BYTE value 0x47
	acpl::Mem::SetByte(oBfr, 0x47, 10);
	for (acpl::UInt8 i = 0; i < 10; i++)
		TestFM(oBfr[i] == 0x47474747, "i=%u", i);
	
	
	// Memcmp 5 bytes from +0 location and 5 bytes from +5 location (we already have a buffer full of 0x47474747 values)
	Test(acpl::Mem::Cmp(oBfr, (oBfr + 5), 5) ==  0);
	oBfr[0]--;
	Test(acpl::Mem::Cmp(oBfr, (oBfr + 5), 5) == -1);
	oBfr[0] += 2;
	Test(acpl::Mem::Cmp(oBfr, (oBfr + 5), 5) ==  1);
	
	acpl::SizeT oDiffIdx;
	Test(acpl::Mem::Cmp(oBfr, (oBfr + 5), 5, oDiffIdx) ==  1);
	Test(oDiffIdx == 0);
	oBfr[0]--;
	Test(acpl::Mem::Cmp(oBfr, (oBfr + 5), 5, oDiffIdx) ==  0);
	Test(oDiffIdx == 5);
	oBfr[4]--;
	Test(acpl::Mem::Cmp(oBfr, (oBfr + 5), 5, oDiffIdx) == -1);
	Test(oDiffIdx == 4);
	
	
	return 0;
}

static int TestDeleting()
{
	PrintFn();
	
	
	acpl::UInt32 *oPtr;
	
	
	// new() and delete()
	oPtr = new acpl::UInt32;
	Test(oPtr != NULL);
	acpl::Mem::Delete(oPtr);
	Test(oPtr == NULL);
	acpl::Mem::Delete(oPtr); // even though the oPtr is NULL `acpl::Mem::Delete` handles that
	
	// new()[] and delete()[]
	oPtr = new acpl::UInt32[10];
	Test(oPtr != NULL);
	acpl::Mem::DeleteArray(oPtr);
	Test(oPtr == NULL);
	acpl::Mem::DeleteArray(oPtr); // even though the oPtr is NULL `acpl::Mem::DeleteArray` handles that
	
	
	return 0;
}

static int TestRaiiReleases()
{
	PrintFn();
	
	
	acpl::UInt32 *oPtr;
	
	
	// Alloc + RaiiFree
	Test(acpl::Mem::Alloc(oPtr, 10));
	Test(oPtr != NULL);
	{
		// WARNING: This is a deliberate block! 
		acpl::Mem::RaiiFree<acpl::UInt32 *> oRaiiPtr(oPtr);
	}
	Test(oPtr == NULL);
	
	// Alloc + RaiiFree + Disable + Free
	Test(acpl::Mem::Alloc(oPtr, 10));
	Test(oPtr != NULL);
	{
		// WARNING: This is a deliberate block! 
		acpl::Mem::RaiiFree<acpl::UInt32 *> oRaiiPtr(oPtr);
		oRaiiPtr.Disable();
	}
	Test(oPtr != NULL);
	acpl::Mem::Free(oPtr);
	
	// Alloc + RaiiFree + Disable + Enable
	Test(acpl::Mem::Alloc(oPtr, 10));
	Test(oPtr != NULL);
	{
		// WARNING: This is a deliberate block! 
		acpl::Mem::RaiiFree<acpl::UInt32 *> oRaiiPtr(oPtr);
		oRaiiPtr.Disable();
		oRaiiPtr.Enable();
	}
	Test(oPtr == NULL);
	
	
	// AllocAligned + RaiiFreeAligned
	Test(acpl::Mem::AllocAligned(oPtr, 64, 16));
	Test(oPtr != NULL);
	{
		// WARNING: This is a deliberate block! 
		acpl::Mem::RaiiFreeAligned<acpl::UInt32 *> oRaiiPtr(oPtr);
	}
	Test(oPtr == NULL);
	
	// AllocAligned + RaiiFreeAligned + Disable + FreeAligned
	Test(acpl::Mem::AllocAligned(oPtr, 64, 16));
	Test(oPtr != NULL);
	{
		// WARNING: This is a deliberate block! 
		acpl::Mem::RaiiFreeAligned<acpl::UInt32 *> oRaiiPtr(oPtr);
		oRaiiPtr.Disable();
	}
	Test(oPtr != NULL);
	acpl::Mem::FreeAligned(oPtr);
	
	// AllocAligned + RaiiFreeAligned + Disable + Enable
	Test(acpl::Mem::AllocAligned(oPtr, 64, 16));
	Test(oPtr != NULL);
	{
		// WARNING: This is a deliberate block! 
		acpl::Mem::RaiiFreeAligned<acpl::UInt32 *> oRaiiPtr(oPtr);
		oRaiiPtr.Disable();
		oRaiiPtr.Enable();
	}
	Test(oPtr == NULL);
	
	
	// new() + RaiiDelete
	oPtr = new acpl::UInt32;
	Test(oPtr != NULL);
	{
		// WARNING: This is a deliberate block! 
		acpl::Mem::RaiiDelete<acpl::UInt32 *> oRaiiPtr(oPtr);
	};
	Test(oPtr == NULL);
	
	// new() + RaiiDelete + Disable + Free
	oPtr = new acpl::UInt32;
	Test(oPtr != NULL);
	{
		// WARNING: This is a deliberate block! 
		acpl::Mem::RaiiDelete<acpl::UInt32 *> oRaiiPtr(oPtr);
		oRaiiPtr.Disable();
	}
	Test(oPtr != NULL);
	acpl::Mem::Delete(oPtr);
	
	// new() + RaiiDelete + Disable + Enable
	oPtr = new acpl::UInt32;
	Test(oPtr != NULL);
	{
		// WARNING: This is a deliberate block! 
		acpl::Mem::RaiiDelete<acpl::UInt32 *> oRaiiPtr(oPtr);
		oRaiiPtr.Disable();
		oRaiiPtr.Enable();
	}
	Test(oPtr == NULL);
	
	
	// new()[] + RaiiDeleteArray
	oPtr = new acpl::UInt32[10];
	Test(oPtr != NULL);
	{
		// WARNING: This is a deliberate block! 
		acpl::Mem::RaiiDeleteArray<acpl::UInt32 *> oRaiiPtr(oPtr);
	};
	Test(oPtr == NULL);
	
	// new()[] + RaiiDeleteArray + Disable + Free
	oPtr = new acpl::UInt32[10];
	Test(oPtr != NULL);
	{
		// WARNING: This is a deliberate block! 
		acpl::Mem::RaiiDeleteArray<acpl::UInt32 *> oRaiiPtr(oPtr);
		oRaiiPtr.Disable();
	}
	Test(oPtr != NULL);
	acpl::Mem::DeleteArray(oPtr);
	
	// new()[] + RaiiDeleteArray + Disable + Enable
	oPtr = new acpl::UInt32[10];
	Test(oPtr != NULL);
	{
		// WARNING: This is a deliberate block! 
		acpl::Mem::RaiiDeleteArray<acpl::UInt32 *> oRaiiPtr(oPtr);
		oRaiiPtr.Disable();
		oRaiiPtr.Enable();
	}
	Test(oPtr == NULL);
	
	
	return 0;
}

static int TestManagementHelpers()
{
	PrintFn();
	
	
	acpl::UInt32 *oPtr;
	
	
	oPtr = NULL;
	acpl::Mem::MgrEda<acpl::UInt32 *>::None(&oPtr);
	Test(oPtr == NULL);
	
	Test(acpl::Mem::Alloc(oPtr, 10) == true);
	Test(oPtr != NULL);
	acpl::Mem::MgrEda<acpl::UInt32 *>::Free(&oPtr);
	Test(oPtr == NULL);
	
	Test(acpl::Mem::AllocAligned(oPtr, 64, 16) == true);
	Test(oPtr != NULL);
	acpl::Mem::MgrEda<acpl::UInt32 *>::FreeAligned(&oPtr);
	Test(oPtr == NULL);
	
	oPtr = new acpl::UInt32;
	Test(oPtr != NULL);
	acpl::Mem::MgrEda<acpl::UInt32 *>::Delete(&oPtr);
	Test(oPtr == NULL);
	
	oPtr = new acpl::UInt32[10];
	Test(oPtr != NULL);
	acpl::Mem::MgrEda<acpl::UInt32 *>::DeleteArray(&oPtr);
	Test(oPtr == NULL);
	
	
	Test(acpl::Mem::MgrBlock<acpl::UInt32>::Resize(oPtr, 0, 10, 0) == true);
	Test(oPtr != NULL);
	acpl::Mem::MgrBlock<acpl::UInt32>::Free(oPtr);
	Test(oPtr == NULL);
	
	
	Test(acpl::Mem::MgrBlockAligned<acplMacroVa(acpl::UInt32, 64)>::Resize(oPtr, 0, 16, 0) == true);
	Test(oPtr != NULL);
	acpl::Mem::MgrBlockAligned<acpl::UInt32, 64>::Free(oPtr);
	Test(oPtr == NULL);
	
	
	return 0;
}

static int TestBlock()
{
	PrintFn();
	
	
	acpl::Mem::Block<acpl::UInt32>::Type oBlock;
	const acpl::Mem::Block<acpl::UInt32>::Type &oBlockConst = oBlock;
	acpl::SizeT oCount;
	
	acpl::UInt32 oBfr[128];
	_local_WriteBytes(oBfr, 0, sizeof(oBfr), 0);
	
	
	// Init
	
	Test(oBlock.Data() == NULL);
	Test(oBlockConst.Data() == NULL);
	Test(oBlock.Data(oCount) == NULL);
	Test(oCount == 0);
	Test(oBlockConst.Data(oCount) == NULL);
	Test(oCount == 0);
	
	Test(oBlock.Length() == 0);
	Test(oBlock.Shrink() == true);
	oBlock.Clear();
	Test(oBlock.Length() == 0);
	oBlock.Purge();
	Test(oBlock.Length() == 0);
	
	oBlock.Cut(0, 0);
	oBlock.Cut(1, 0);
	oBlock.Cut(0, 1);
	oBlock.Cut(1, 1);
	oBlock.CutLeft(0);
	oBlock.CutLeft(1);
	oBlock.CutRight(0);
	oBlock.CutRight(1);
	
	
	// SetLenght() + tests
	
	Test(oBlock.SetLength(128) == true);
	Test(oBlock.Length() == 128);
	Test(oBlock.Data() != NULL);
	Test(oBlockConst.Data() != NULL);
	Test(oBlock.Data(oCount) != NULL);
	Test(oCount == 128);
	Test(oBlockConst.Data(oCount) != NULL);
	Test(oCount == 128);
	
	Test(oBlock.SetLength(0) == true);
	Test(oBlock.Length() == 0);
	Test(oBlock.Data() != NULL);
	Test(oBlockConst.Data() != NULL);
	Test(oBlock.Data(oCount) != NULL);
	Test(oCount == 0);
	Test(oBlockConst.Data(oCount) != NULL);
	Test(oCount == 0);
	
	Test(oBlock.SetLength(128) == true);
	oBlock.Clear();
	Test(oBlock.Length() == 0);
	Test(oBlock.Data() != NULL);
	Test(oBlockConst.Data() != NULL);
	Test(oBlock.Data(oCount) != NULL);
	Test(oCount == 0);
	Test(oBlockConst.Data(oCount) != NULL);
	Test(oCount == 0);
	
	Test(oBlock.SetLength(128) == true);
	oBlock.Purge();
	Test(oBlock.Length() == 0);
	Test(oBlock.Data() == NULL);
	Test(oBlockConst.Data() == NULL);
	Test(oBlock.Data(oCount) == NULL);
	Test(oCount == 0);
	Test(oBlockConst.Data(oCount) == NULL);
	Test(oCount == 0);
	
	
	// Apnd()'s + tests
	
	oBlock.Clear();
	Test(oBlock.Apnd(oBfr, 128) == true);
	Test(oBlock.Length() == 128);
	Test(oBlock.Data() != NULL);
	Test(_local_TestBytes(oBlock.Data(), 0, (128 * sizeof(*oBfr)), 0) == true);
	
	Test(oBlock.Apnd(oBfr, 128) == true);
	Test(oBlock.Length() == 256);
	Test(oBlock.Data() != NULL);
	Test(_local_TestBytes(oBlock.Data(), 0, (128 * sizeof(*oBfr)), 0) == true);
	Test(_local_TestBytes(oBlock.Data(), (128 * sizeof(*oBfr)), (128 * sizeof(*oBfr)), 0) == true);
	
	oBlock.Clear();
	Test(oBlock.Apnd(&lTestByteIoChannel, 128, oCount) == true);
	Test(oCount == 128);
	Test(oBlock.Length() == 128);
	Test(oBlock.Data() != NULL);
	Test(_local_TestBytes(oBlock.Data(), 0, (128 * sizeof(*oBfr)), 0) == true);
	
	Test(oBlock.Apnd(&lTestByteIoChannel, 128, oCount) == true);
	Test(oCount == 128);
	Test(oBlock.Length() == 256);
	Test(oBlock.Data() != NULL);
	Test(_local_TestBytes(oBlock.Data(), 0, (128 * sizeof(*oBfr)), 0) == true);
	Test(_local_TestBytes(oBlock.Data(), (128 * sizeof(*oBfr)), (128 * sizeof(*oBfr)), 0) == true);
	
	oBlock.Clear();
	Test(oBlock.Apnd(&lTestByteIoChannel, 128) == true);
	Test(oBlock.Length() == 128);
	Test(oBlock.Data() != NULL);
	Test(_local_TestBytes(oBlock.Data(), 0, (128 * sizeof(*oBfr)), 0) == true);
	
	Test(oBlock.Apnd(&lTestByteIoChannel, 128) == true);
	Test(oBlock.Length() == 256);
	Test(oBlock.Data() != NULL);
	Test(_local_TestBytes(oBlock.Data(), 0, (128 * sizeof(*oBfr)), 0) == true);
	Test(_local_TestBytes(oBlock.Data(), (128 * sizeof(*oBfr)), (128 * sizeof(*oBfr)), 0) == true);
	
	
	// Ins()'s + tests
	
	oBlock.Clear();
	Test(oBlock.Ins(0, oBfr, 128) == true);
	Test(oBlock.Length() == 128);
	Test(oBlock.Data() != NULL);
	Test(_local_TestBytes(oBlock.Data(), 0, (128 * sizeof(*oBfr)), 0) == true);
	
	Test(oBlock.Ins(128, oBfr, 128) == true); // at the end
	Test(oBlock.Length() == 256);
	Test(oBlock.Data() != NULL);
	Test(_local_TestBytes(oBlock.Data(), 0, (128 * sizeof(*oBfr)), 0) == true);
	Test(_local_TestBytes(oBlock.Data(), (128 * sizeof(*oBfr)), (128 * sizeof(*oBfr)), 0) == true);
	
	Test(oBlock.Ins(0, oBfr, 128) == true); // at the beginning
	Test(oBlock.Length() == 384);
	Test(oBlock.Data() != NULL);
	Test(_local_TestBytes(oBlock.Data(), 0, (128 * sizeof(*oBfr)), 0) == true);
	Test(_local_TestBytes(oBlock.Data(), (128 * sizeof(*oBfr)), (128 * sizeof(*oBfr)), 0) == true);
	Test(_local_TestBytes(oBlock.Data(), (256 * sizeof(*oBfr)), (128 * sizeof(*oBfr)), 0) == true);
	
	Test(oBlock.Ins(64, oBfr, 128) == true); // in the middle
	Test(oBlock.Length() == 512);
	Test(oBlock.Data() != NULL);
	Test(_local_TestBytes(oBlock.Data(), 0, (64 * sizeof(*oBfr)), 0) == true);
	Test(_local_TestBytes(oBlock.Data(), (64  * sizeof(*oBfr)), (128 * sizeof(*oBfr)), 0) == true);
	Test(_local_TestBytes(oBlock.Data(), (192 * sizeof(*oBfr)), (64  * sizeof(*oBfr)), (64  * sizeof(*oBfr))) == true);
	Test(_local_TestBytes(oBlock.Data(), (128 * sizeof(*oBfr)), (128 * sizeof(*oBfr)), 0) == true);
	Test(_local_TestBytes(oBlock.Data(), (256 * sizeof(*oBfr)), (128 * sizeof(*oBfr)), 0) == true);
	
	oBlock.Clear();
	Test(oBlock.Ins(0, &lTestByteIoChannel, 128, oCount) == true);
	Test(oCount == 128);
	Test(oBlock.Length() == 128);
	Test(oBlock.Data() != NULL);
	Test(_local_TestBytes(oBlock.Data(), 0, (128 * sizeof(*oBfr)), 0) == true);
	
	Test(oBlock.Ins(128, &lTestByteIoChannel, 128, oCount) == true); // at the end
	Test(oCount == 128);
	Test(oBlock.Length() == 256);
	Test(oBlock.Data() != NULL);
	Test(_local_TestBytes(oBlock.Data(), 0, (128 * sizeof(*oBfr)), 0) == true);
	Test(_local_TestBytes(oBlock.Data(), (128 * sizeof(*oBfr)), (128 * sizeof(*oBfr)), 0) == true);
	
	Test(oBlock.Ins(0, &lTestByteIoChannel, 128, oCount) == true); // at the beginning
	Test(oCount == 128);
	Test(oBlock.Length() == 384);
	Test(oBlock.Data() != NULL);
	Test(_local_TestBytes(oBlock.Data(), 0, (128 * sizeof(*oBfr)), 0) == true);
	Test(_local_TestBytes(oBlock.Data(), (128 * sizeof(*oBfr)), (128 * sizeof(*oBfr)), 0) == true);
	Test(_local_TestBytes(oBlock.Data(), (256 * sizeof(*oBfr)), (128 * sizeof(*oBfr)), 0) == true);
	
	Test(oBlock.Ins(64, &lTestByteIoChannel, 128, oCount) == true); // in the middle
	Test(oCount == 128);
	Test(oBlock.Length() == 512);
	Test(oBlock.Data() != NULL);
	Test(_local_TestBytes(oBlock.Data(), 0, (64 * sizeof(*oBfr)), 0) == true);
	Test(_local_TestBytes(oBlock.Data(), (64  * sizeof(*oBfr)), (128 * sizeof(*oBfr)), 0) == true);
	Test(_local_TestBytes(oBlock.Data(), (192 * sizeof(*oBfr)), (64  * sizeof(*oBfr)), (64  * sizeof(*oBfr))) == true);
	Test(_local_TestBytes(oBlock.Data(), (128 * sizeof(*oBfr)), (128 * sizeof(*oBfr)), 0) == true);
	Test(_local_TestBytes(oBlock.Data(), (256 * sizeof(*oBfr)), (128 * sizeof(*oBfr)), 0) == true);
	
	oBlock.Clear();
	Test(oBlock.Ins(0, &lTestByteIoChannel, 128) == true);
	Test(oBlock.Length() == 128);
	Test(oBlock.Data() != NULL);
	Test(_local_TestBytes(oBlock.Data(), 0, (128 * sizeof(*oBfr)), 0) == true);
	
	Test(oBlock.Ins(128, &lTestByteIoChannel, 128) == true); // at the end
	Test(oBlock.Length() == 256);
	Test(oBlock.Data() != NULL);
	Test(_local_TestBytes(oBlock.Data(), 0, (128 * sizeof(*oBfr)), 0) == true);
	Test(_local_TestBytes(oBlock.Data(), (128 * sizeof(*oBfr)), (128 * sizeof(*oBfr)), 0) == true);
	
	Test(oBlock.Ins(0, &lTestByteIoChannel, 128) == true); // at the beginning
	Test(oBlock.Length() == 384);
	Test(oBlock.Data() != NULL);
	Test(_local_TestBytes(oBlock.Data(), 0, (128 * sizeof(*oBfr)), 0) == true);
	Test(_local_TestBytes(oBlock.Data(), (128 * sizeof(*oBfr)), (128 * sizeof(*oBfr)), 0) == true);
	Test(_local_TestBytes(oBlock.Data(), (256 * sizeof(*oBfr)), (128 * sizeof(*oBfr)), 0) == true);
	
	Test(oBlock.Ins(64, &lTestByteIoChannel, 128) == true); // in the middle
	Test(oBlock.Length() == 512);
	Test(oBlock.Data() != NULL);
	Test(_local_TestBytes(oBlock.Data(), 0, (64 * sizeof(*oBfr)), 0) == true);
	Test(_local_TestBytes(oBlock.Data(), (64  * sizeof(*oBfr)), (128 * sizeof(*oBfr)), 0) == true);
	Test(_local_TestBytes(oBlock.Data(), (192 * sizeof(*oBfr)), (64  * sizeof(*oBfr)), (64  * sizeof(*oBfr))) == true);
	Test(_local_TestBytes(oBlock.Data(), (128 * sizeof(*oBfr)), (128 * sizeof(*oBfr)), 0) == true);
	Test(_local_TestBytes(oBlock.Data(), (256 * sizeof(*oBfr)), (128 * sizeof(*oBfr)), 0) == true);
	
	
	// Fill() && FillByte() + tests
	
	oBlock.Clear();
	Test(oBlock.Fill(0, 0x47, 128) == true);
	Test(oBlock.Length() == 128);
	Test(oBlock.Data() != NULL);
	Test(_local_TestVals(oBlock.Data(), 0, 128, static_cast<acpl::UInt32>(0x00000047)) == true);
	
	Test(oBlock.Fill(128, 0x49, 128) == true); // add at the end
	Test(oBlock.Length() == 256);
	Test(oBlock.Data() != NULL);
	Test(_local_TestVals(oBlock.Data(), 0, 128, static_cast<acpl::UInt32>(0x00000047)) == true);
	Test(_local_TestVals(oBlock.Data(), 128, 128, static_cast<acpl::UInt32>(0x00000049)) == true);
	
	Test(oBlock.Fill(0, 0x51, 128) == true); // add at the beginning
	Test(oBlock.Length() == 384);
	Test(oBlock.Data() != NULL);
	Test(_local_TestVals(oBlock.Data(), 0, 128, static_cast<acpl::UInt32>(0x00000051)) == true);
	Test(_local_TestVals(oBlock.Data(), 128, 128, static_cast<acpl::UInt32>(0x00000047)) == true);
	Test(_local_TestVals(oBlock.Data(), 256, 128, static_cast<acpl::UInt32>(0x00000049)) == true);
	
	Test(oBlock.Fill(128, 0x53, 128) == true); // add in the middle
	Test(oBlock.Length() == 512);
	Test(oBlock.Data() != NULL);
	Test(_local_TestVals(oBlock.Data(), 0, 128, static_cast<acpl::UInt32>(0x00000051)) == true);
	Test(_local_TestVals(oBlock.Data(), 128, 128, static_cast<acpl::UInt32>(0x00000053)) == true);
	Test(_local_TestVals(oBlock.Data(), 256, 128, static_cast<acpl::UInt32>(0x00000047)) == true);
	Test(_local_TestVals(oBlock.Data(), 384, 128, static_cast<acpl::UInt32>(0x00000049)) == true);
	
	oBlock.Clear();
	Test(oBlock.FillByte(0, 0x47, 128) == true);
	Test(oBlock.Length() == 128);
	Test(oBlock.Data() != NULL);
	Test(_local_TestVals(oBlock.Data(), 0, 128, static_cast<acpl::UInt32>(0x47474747)) == true);
	
	Test(oBlock.FillByte(128, 0x49, 128) == true); // add at the end
	Test(oBlock.Length() == 256);
	Test(oBlock.Data() != NULL);
	Test(_local_TestVals(oBlock.Data(), 0, 128, static_cast<acpl::UInt32>(0x47474747)) == true);
	Test(_local_TestVals(oBlock.Data(), 128, 128, static_cast<acpl::UInt32>(0x49494949)) == true);
	
	Test(oBlock.FillByte(0, 0x51, 128) == true); // add at the beginning
	Test(oBlock.Length() == 384);
	Test(oBlock.Data() != NULL);
	Test(_local_TestVals(oBlock.Data(), 0, 128, static_cast<acpl::UInt32>(0x51515151)) == true);
	Test(_local_TestVals(oBlock.Data(), 128, 128, static_cast<acpl::UInt32>(0x47474747)) == true);
	Test(_local_TestVals(oBlock.Data(), 256, 128, static_cast<acpl::UInt32>(0x49494949)) == true);
	
	Test(oBlock.FillByte(128, 0x53, 128) == true); // add in the middle
	Test(oBlock.Length() == 512);
	Test(oBlock.Data() != NULL);
	Test(_local_TestVals(oBlock.Data(), 0, 128, static_cast<acpl::UInt32>(0x51515151)) == true);
	Test(_local_TestVals(oBlock.Data(), 128, 128, static_cast<acpl::UInt32>(0x53535353)) == true);
	Test(_local_TestVals(oBlock.Data(), 256, 128, static_cast<acpl::UInt32>(0x47474747)) == true);
	Test(_local_TestVals(oBlock.Data(), 384, 128, static_cast<acpl::UInt32>(0x49494949)) == true);
	
	
	// Cut{,Left,Right} + tests
	
	oBlock.Clear();
	Test(oBlock.Apnd(oBfr, 128) == true);
	
	oBlock.Cut(0, 0);
	Test(oBlock.Length() == 128);
	Test(oBlock.Data() != NULL);
	Test(_local_TestBytes(oBlock.Data(), 0, (128 * sizeof(*oBfr)), 0) == true);
	
	oBlock.Cut(1, 0);
	Test(oBlock.Length() == 128);
	Test(oBlock.Data() != NULL);
	Test(_local_TestBytes(oBlock.Data(), 0, (128 * sizeof(*oBfr)), 0) == true);
	
	oBlock.Cut(acpl::Num<acpl::SizeT>::Max(), 0);
	Test(oBlock.Length() == 128);
	Test(oBlock.Data() != NULL);
	Test(_local_TestBytes(oBlock.Data(), 0, (128 * sizeof(*oBfr)), 0) == true);
	
	oBlock.Cut(acpl::Num<acpl::SizeT>::Max(), 1);
	Test(oBlock.Length() == 128);
	Test(oBlock.Data() != NULL);
	Test(_local_TestBytes(oBlock.Data(), 0, (128 * sizeof(*oBfr)), 0) == true);
	
	oBlock.Cut(127, 1); // at the end
	Test(oBlock.Length() == 127);
	Test(oBlock.Data() != NULL);
	Test(_local_TestBytes(oBlock.Data(), 0, (127 * sizeof(*oBfr)), 0) == true);
	
	oBlock.Cut(126, acpl::Num<acpl::SizeT>::Max()); // at the end
	Test(oBlock.Length() == 126);
	Test(oBlock.Data() != NULL);
	Test(_local_TestBytes(oBlock.Data(), 0, (126 * sizeof(*oBfr)), 0) == true);
	
	oBlock.Cut(0, 1); // at the beginning
	Test(oBlock.Length() == 125);
	Test(oBlock.Data() != NULL);
	Test(_local_TestBytes(oBlock.Data(), 0, (125 * sizeof(*oBfr)), (1 * sizeof(*oBfr))) == true);
	
	oBlock.Cut(0, acpl::Num<acpl::SizeT>::Max()); // at the beginning
	Test(oBlock.Length() == 0);
	Test(oBlock.Data() != NULL);
	
	oBlock.Clear();
	Test(oBlock.Apnd(oBfr, 128) == true);
	oBlock.Cut(0, 127); // below limit
	Test(oBlock.Length() == 1);
	Test(oBlock.Data() != NULL);
	Test(_local_TestBytes(oBlock.Data(), 0, (1 * sizeof(*oBfr)), (127 * sizeof(*oBfr))) == true);
	
	oBlock.Clear();
	Test(oBlock.Apnd(oBfr, 128) == true);
	oBlock.Cut(0, 128); // on limit
	Test(oBlock.Length() == 0);
	Test(oBlock.Data() != NULL);
	
	oBlock.Clear();
	Test(oBlock.Apnd(oBfr, 128) == true);
	oBlock.Cut(0, 129); // over limit
	Test(oBlock.Length() == 0);
	Test(oBlock.Data() != NULL);
	
	oBlock.Clear();
	Test(oBlock.Apnd(oBfr, 128) == true);
	oBlock.Cut(126, 1); // below limit
	Test(oBlock.Length() == 127);
	Test(oBlock.Data() != NULL);
	Test(_local_TestBytes(oBlock.Data(), 0, (126 * sizeof(*oBfr)), 0) == true);
	Test(_local_TestBytes(oBlock.Data(), (126 * sizeof(*oBfr)), (1 * sizeof(*oBfr)), (127 * sizeof(*oBfr))) == true);
	
	oBlock.Clear();
	Test(oBlock.Apnd(oBfr, 128) == true);
	oBlock.Cut(126, 2); // on limit
	Test(oBlock.Length() == 126);
	Test(oBlock.Data() != NULL);
	Test(_local_TestBytes(oBlock.Data(), 0, (126 * sizeof(*oBfr)), 0) == true);
	
	oBlock.Clear();
	Test(oBlock.Apnd(oBfr, 128) == true);
	oBlock.Cut(126, 3); // over limit
	Test(oBlock.Length() == 126);
	Test(oBlock.Data() != NULL);
	Test(_local_TestBytes(oBlock.Data(), 0, (126 * sizeof(*oBfr)), 0) == true);
	
	oBlock.Clear();
	Test(oBlock.Apnd(oBfr, 128) == true);
	
	oBlock.CutLeft(0); // at the beginning
	Test(oBlock.Length() == 128);
	Test(oBlock.Data() != NULL);
	Test(_local_TestBytes(oBlock.Data(), 0, (128 * sizeof(*oBfr)), 0) == true);
	
	oBlock.CutLeft(1); // at the beginning
	Test(oBlock.Length() == 127);
	Test(oBlock.Data() != NULL);
	Test(_local_TestBytes(oBlock.Data(), 0, (127 * sizeof(*oBfr)), (1 * sizeof(*oBfr))) == true);
	
	oBlock.CutLeft(acpl::Num<acpl::SizeT>::Max()); // at the beginning
	Test(oBlock.Length() == 0);
	Test(oBlock.Data() != NULL);
	
	oBlock.Clear();
	Test(oBlock.Apnd(oBfr, 128) == true);
	oBlock.CutLeft(127); // below limit
	Test(oBlock.Length() == 1);
	Test(oBlock.Data() != NULL);
	Test(_local_TestBytes(oBlock.Data(), 0, (1 * sizeof(*oBfr)), (127 * sizeof(*oBfr))) == true);
	
	oBlock.Clear();
	Test(oBlock.Apnd(oBfr, 128) == true);
	oBlock.CutLeft(128); // on limit
	Test(oBlock.Length() == 0);
	Test(oBlock.Data() != NULL);
	
	oBlock.Clear();
	Test(oBlock.Apnd(oBfr, 128) == true);
	oBlock.CutLeft(129); // over limit
	Test(oBlock.Length() == 0);
	Test(oBlock.Data() != NULL);
	
	oBlock.Clear();
	Test(oBlock.Apnd(oBfr, 128) == true);
	
	oBlock.CutRight(0); // at the end
	Test(oBlock.Length() == 128);
	Test(oBlock.Data() != NULL);
	Test(_local_TestBytes(oBlock.Data(), 0, (128 * sizeof(*oBfr)), 0) == true);
	
	oBlock.CutRight(1); // at the end
	Test(oBlock.Length() == 127);
	Test(oBlock.Data() != NULL);
	Test(_local_TestBytes(oBlock.Data(), 0, (127 * sizeof(*oBfr)), 0) == true);
	
	oBlock.CutRight(acpl::Num<acpl::SizeT>::Max()); // at the end
	Test(oBlock.Length() == 0);
	Test(oBlock.Data() != NULL);
	
	oBlock.Clear();
	Test(oBlock.Apnd(oBfr, 128) == true);
	oBlock.CutRight(127); // below limit
	Test(oBlock.Length() == 1);
	Test(oBlock.Data() != NULL);
	Test(_local_TestBytes(oBlock.Data(), 0, (1 * sizeof(*oBfr)), 0) == true);
	
	oBlock.Clear();
	Test(oBlock.Apnd(oBfr, 128) == true);
	oBlock.CutRight(128); // on limit
	Test(oBlock.Length() == 0);
	Test(oBlock.Data() != NULL);
	
	oBlock.Clear();
	Test(oBlock.Apnd(oBfr, 128) == true);
	oBlock.CutRight(129); // on limit
	Test(oBlock.Length() == 0);
	Test(oBlock.Data() != NULL);
	
	
	// Grow() + Shrink() + tests ?? a bit hard to test this since it's more or less internal stuff
	
	
	return 0;
}

static int TestByteBlockIo()
{
	PrintFn();
	
	
	acpl::Mem::ByteBlock::Type oBlock;
	acpl::Mem::ByteBlockIo oBlockIo;
	acpl::OffT oSize = 0, oPos = 0;
	acpl::SizeT oRetSize = 0;
	
	acpl::UInt8 oBfr[128];
	_local_WriteBytes(oBfr, 0, sizeof(oBfr), 0);
	
	
	// Init
	
	Test(oBlockIo.IsOpen() == false);
	Test(oBlockIo.Open(NULL) == false);
	Test(oBlockIo.GetSize() == -1);
	Test(oBlockIo.GetSize(oSize) == false);
	Test(oSize == 0);
	Test(oBlockIo.GetOffset() == -1);
	Test(oBlockIo.GetOffset(oPos) == false);
	Test(oPos == 0);
	Test(oBlockIo.Write(oBfr, 128, oRetSize) == false);
	Test(oRetSize == 0);
	Test(oBlockIo.Read(oBfr, 128, oRetSize) == false);
	Test(oRetSize == 0);
	
	
	// Open() + tests + Close()
	
	Test(oBlockIo.Open(&oBlock) == true);
	Test(oBlockIo.IsOpen() == true);
	Test(oBlockIo.GetSize() == 0);
	Test(oBlockIo.GetSize(oSize) == true);
	Test(oSize == 0);
	Test(oBlockIo.GetOffset() == 0);
	Test(oBlockIo.GetOffset(oPos) == true);
	Test(oPos == 0);
	oBlockIo.Close();
	Test(oBlockIo.IsOpen() == false);
	
	oBlock.Apnd(oBfr, 128);
	Test(oBlockIo.Open(&oBlock) == true);
	Test(oBlockIo.GetSize() == 128);
	Test(oBlockIo.GetSize(oSize) == true);
	Test(oSize == 128);
	Test(oBlockIo.GetOffset() == 0);
	Test(oBlockIo.GetOffset(oPos) == true);
	Test(oPos == 0);
	oBlockIo.Close();
	Test(oBlockIo.IsOpen() == false);
	oBlock.Clear();
	
	
	// Write() + tests
	
	Test(oBlockIo.Open(&oBlock) == true);
	Test(oBlockIo.Write(NULL, 0, oRetSize) == true);
	Test(oRetSize == 0);
	Test(oBlock.Length() == 0);
	Test(oBlockIo.GetSize() == 0);
	Test(oBlockIo.GetOffset() == 0);
	Test(oBlockIo.Write(oBfr, 128, oRetSize) == true);
	Test(oRetSize == 128);
	Test(oBlock.Length() == 128);
	Test(oBlockIo.GetSize() == 128);
	Test(oBlockIo.GetOffset() == 128);
	oBlockIo.Close();
	
	
	// (Re)Write() + tests
	
	Test(oBlockIo.Open(&oBlock) == true);
	Test(oBlockIo.GetSize() == 128);
	Test(oBlockIo.GetOffset() == 0);
	Test(oBlockIo.Write(oBfr, 128, oRetSize) == true);
	Test(oRetSize == 128);
	Test(oBlock.Length() == 128);
	Test(oBlockIo.GetSize() == 128);
	Test(oBlockIo.GetOffset() == 128);
	oBlockIo.Close();
	
	
	// Read() + tests
	
	acpl::Mem::SetByte(oBfr, 0x00, 128);
	Test(oBlockIo.Open(&oBlock) == true);
	Test(oBlockIo.Read(NULL, 0, oRetSize) == true);
	Test(oRetSize == 0);
	Test(oBlockIo.GetOffset(oPos) == true);
	Test(oPos == 0);
	Test(oBlockIo.Read(oBfr, 128, oRetSize) == true);
	Test(oRetSize == 128);
	Test(oBlockIo.GetOffset(oPos) == true);
	Test(oPos == 128);
	Test(acpl::Mem::Cmp(oBlock.Data(), oBfr, 128) == 0);
	oBlockIo.Close();
	
	
	// SetOffset() + tests
	
	Test(oBlockIo.Open(&oBlock) == true);
	Test(oBlockIo.SetOffset(0, acpl::IoChannel::whStart) == true);
	Test(oBlockIo.GetOffset() == 0);
	Test(oBlockIo.SetOffset(127, acpl::IoChannel::whStart) == true);
	Test(oBlockIo.GetOffset() == 127);
	Test(oBlockIo.SetOffset(128, acpl::IoChannel::whStart) == true);
	Test(oBlockIo.GetOffset() == 128);
	Test(oBlockIo.SetOffset(129, acpl::IoChannel::whStart) == true);
	Test(oBlockIo.GetOffset() == 129);
	Test(oBlockIo.SetOffset(-1, acpl::IoChannel::whStart) == false);
	Test(oBlockIo.GetOffset() == 129);
	Test(oBlockIo.SetOffset(0, acpl::IoChannel::whEnd) == true);
	Test(oBlockIo.GetOffset() == 128);
	Test(oBlockIo.SetOffset(1, acpl::IoChannel::whEnd) == true);
	Test(oBlockIo.GetOffset() == 129);
	Test(oBlockIo.SetOffset(-1, acpl::IoChannel::whEnd) == true);
	Test(oBlockIo.GetOffset() == 127);
	Test(oBlockIo.SetOffset(-128, acpl::IoChannel::whEnd) == true);
	Test(oBlockIo.GetOffset() == 0);
	Test(oBlockIo.SetOffset(-127, acpl::IoChannel::whEnd) == true);
	Test(oBlockIo.GetOffset() == 1);
	Test(oBlockIo.SetOffset(-129, acpl::IoChannel::whEnd) == false);
	Test(oBlockIo.GetOffset() == 1);
	Test(oBlockIo.SetOffset(9, acpl::IoChannel::whCurrent) == true);
	Test(oBlockIo.GetOffset() == 10);
	Test(oBlockIo.SetOffset(-1, acpl::IoChannel::whCurrent) == true);
	Test(oBlockIo.GetOffset() == 9);
	Test(oBlockIo.SetOffset(-9, acpl::IoChannel::whCurrent) == true);
	Test(oBlockIo.GetOffset() == 0);
	Test(oBlockIo.SetOffset(-1, acpl::IoChannel::whCurrent) == false);
	Test(oBlockIo.GetOffset() == 0);
	oBlockIo.Close();
	
	
	// SetOffset() + Read() + tests
	
	acpl::Mem::SetByte(oBfr, 0x00, 128);
	Test(oBlockIo.Open(&oBlock) == true);
	Test(oBlockIo.SetOffset(64, acpl::IoChannel::whStart) == true);
	Test(oBlockIo.GetOffset() == 64);
	Test(oBlockIo.Read(oBfr, 64, oRetSize) == true);
	Test(oRetSize == 64);
	Test(acpl::Mem::Cmp(oBlock.Data() + 64, oBfr, 64) == 0);
	Test(oBlockIo.SetOffset(64, acpl::IoChannel::whStart) == true);
	Test(oBlockIo.GetOffset() == 64);
	Test(oBlockIo.Read(oBfr, 65, oRetSize) == true);
	Test(oRetSize == 64);
	Test(acpl::Mem::Cmp(oBlock.Data() + 64, oBfr, 64) == 0);
	Test(oBlockIo.SetOffset(0, acpl::IoChannel::whEnd) == true);
	Test(oBlockIo.GetOffset() == 128);
	Test(oBlockIo.Read(oBfr, 1, oRetSize) == true);
	Test(oRetSize == 0);
	oBlockIo.Close();
	
	
	// SetOffset() + Write() + tests
	
	_local_WriteBytes(oBfr, 0, sizeof(oBfr), 0);
	oBlock.Clear();
	Test(oBlockIo.Open(&oBlock) == true);
	Test(oBlockIo.Write(oBfr, 128, oRetSize) == true);
	Test(oRetSize == 128);
	Test(oBlockIo.GetSize() == 128);
	Test(oBlockIo.GetOffset() == 128);
	Test(acpl::Mem::Cmp(oBlock.Data(), oBfr, 128) == 0);
	Test(oBlockIo.SetOffset(64, acpl::IoChannel::whStart) == true);
	Test(oBlockIo.GetOffset() == 64);
	Test(oBlockIo.Write(oBfr, 128, oRetSize) == true);
	Test(oRetSize == 128);
	Test(oBlockIo.GetSize() == 192);
	Test(oBlockIo.GetOffset() == 192);
	Test(acpl::Mem::Cmp(oBlock.Data(), oBfr, 64) == 0);
	Test(acpl::Mem::Cmp(oBlock.Data() + 64, oBfr, 128) == 0);
	Test(oBlockIo.Write(oBfr, 64, oRetSize) == true);
	Test(oRetSize == 64);
	Test(oBlockIo.GetSize() == 256);
	Test(oBlockIo.GetOffset() == 256);
	Test(acpl::Mem::Cmp(oBlock.Data(), oBfr, 64) == 0);
	Test(acpl::Mem::Cmp(oBlock.Data() + 64, oBfr, 128) == 0);
	Test(acpl::Mem::Cmp(oBlock.Data() + 192, oBfr, 64) == 0);
	Test(oBlockIo.SetOffset(64, acpl::IoChannel::whEnd) == true);
	Test(oBlockIo.GetOffset() == 320);
	Test(oBlockIo.Write(oBfr, 64, oRetSize) == true);
	Test(oRetSize == 64);
	Test(oBlockIo.GetSize() == 384);
	Test(oBlockIo.GetOffset() == 384);
	Test(acpl::Mem::Cmp(oBlock.Data(), oBfr, 64) == 0);
	Test(acpl::Mem::Cmp(oBlock.Data() + 64, oBfr, 128) == 0);
	Test(acpl::Mem::Cmp(oBlock.Data() + 192, oBfr, 64) == 0);
	Test(_local_TestVals(oBlock.Data(), 256, 64, 0x00) == true);
	Test(acpl::Mem::Cmp(oBlock.Data() + 320, oBfr, 64) == 0);
	oBlockIo.Close();
	
	return 0;
}


static int TestComparators()
{
	PrintFn();
	
	
	acpl::SInt8 oVal1, oVal2;
	
	
	oVal1 = 0;
	oVal2 = 0;
	Test(acpl::Mem::SortCmpFw(oVal1, oVal2) == 0);
	Test(acpl::Mem::SortCmpBw(oVal1, oVal2) == 0);
	
	oVal1 = 0;
	oVal2 = 1;
	Test(acpl::Mem::SortCmpFw(oVal1, oVal2) == -1);
	Test(acpl::Mem::SortCmpBw(oVal1, oVal2) == 1);
	
	oVal1 = -1;
	oVal2 = 0;
	Test(acpl::Mem::SortCmpFw(oVal1, oVal2) == -1);
	Test(acpl::Mem::SortCmpBw(oVal1, oVal2) == 1);
	
	oVal1 = 1;
	oVal2 = 0;
	Test(acpl::Mem::SortCmpFw(oVal1, oVal2) == 1);
	Test(acpl::Mem::SortCmpBw(oVal1, oVal2) == -1);
	
	oVal1 = 0;
	oVal2 = -1;
	Test(acpl::Mem::SortCmpFw(oVal1, oVal2) == 1);
	Test(acpl::Mem::SortCmpBw(oVal1, oVal2) == -1);
	
	
	return 0;
}

static int TestQuicksort()
{
	PrintFn();
	
	
	acpl::UInt8 *oBfr;
	Test(acpl::Mem::Alloc(oBfr, 10) == true);
	acpl::Mem::RaiiFree<acpl::UInt8 *> oRfBfr(oBfr);
	
	
	{
		_local_WriteBytes(oBfr, 1);
		acpl::Mem::Quicksort<acpl::UInt8>(oBfr, 0, 0, acpl::Mem::SortCmpFw);
		Test(oBfr[0] == 9);
	}
	
	{
		_local_WriteBytes(oBfr, 2);
		acpl::Mem::Quicksort<acpl::UInt8>(oBfr, 0, 1, acpl::Mem::SortCmpFw);
		Test(oBfr[0] == 2);
		Test(oBfr[1] == 9);
	}
	
	{
		_local_WriteBytes(oBfr, 3);
		acpl::Mem::Quicksort<acpl::UInt8>(oBfr, 0, 2, acpl::Mem::SortCmpFw);
		Test(oBfr[0] == 2);
		Test(oBfr[1] == 7);
		Test(oBfr[2] == 9);
	}
	
	{
		_local_WriteBytes(oBfr, 3);
		acpl::Mem::Quicksort<acpl::UInt8>(oBfr, 1, 2, acpl::Mem::SortCmpFw);
		Test(oBfr[0] == 9);
		Test(oBfr[1] == 2);
		Test(oBfr[2] == 7);
	}
	
	{
		_local_WriteBytes(oBfr, 4);
		acpl::Mem::Quicksort<acpl::UInt8>(oBfr, 0, 3, acpl::Mem::SortCmpFw);
		Test(oBfr[0] == 2);
		Test(oBfr[1] == 7);
		Test(oBfr[2] == 9);
		Test(oBfr[3] == 13);
	}
	
	{
		_local_WriteBytes(oBfr, 4);
		acpl::Mem::Quicksort<acpl::UInt8>(oBfr, 0, 3, acpl::Mem::SortCmpBw);
		Test(oBfr[3] == 2);
		Test(oBfr[2] == 7);
		Test(oBfr[1] == 9);
		Test(oBfr[0] == 13);
	}
	
	{
		_local_WriteBytes(oBfr, 9);
		acpl::Mem::Quicksort<acpl::UInt8>(oBfr, 0, 8, acpl::Mem::SortCmpFw);
		Test(oBfr[0] == 2);
		Test(oBfr[1] == 6);
		Test(oBfr[2] == 7);
		Test(oBfr[3] == 7);
		Test(oBfr[4] == 8);
		Test(oBfr[5] == 9);
		Test(oBfr[6] == 13);
		Test(oBfr[7] == 18);
		Test(oBfr[8] == 27);
	}
	
	{
		_local_WriteBytes(oBfr, 10);
		acpl::Mem::Quicksort<acpl::UInt8>(oBfr, 0, 9, acpl::Mem::SortCmpFw);
		Test(oBfr[0] == 2);
		Test(oBfr[1] == 6);
		Test(oBfr[2] == 7);
		Test(oBfr[3] == 7);
		Test(oBfr[4] == 8);
		Test(oBfr[5] == 9);
		Test(oBfr[6] == 9);
		Test(oBfr[7] == 13);
		Test(oBfr[8] == 18);
		Test(oBfr[9] == 27);
	}
	
	
	return 0;
}


SectionFuncMain(memory)
{
	if (argc < 1)
	{
		Test(TestBaseManagement() == 0);
		Test(TestBaseManagementAligned() == 0);
		Test(TestBaseMovement() == 0);
		Test(TestManagement() == 0);
		Test(TestManagementAligned() == 0);
		Test(TestMovement() == 0);
		Test(TestDeleting() == 0);
		Test(TestRaiiReleases() == 0);
		Test(TestManagementHelpers() == 0);
		Test(TestBlock() == 0); // basically a test of acpl::Mem::BlockBase which is the same for acpl::Mem::Block, acpl::Mem::BlockAligned, and acpl::Mem::ByteBlock
		Test(TestByteBlockIo() == 0);
		Test(TestComparators() == 0);
		Test(TestQuicksort() == 0);
		
		PrintOut("All Passed!\n");
	}
	else
		return PrintUnrecParams(acplDbgSrcLoc, lSectionName, argc, argv);
	
	return 0;
}

SectionFuncParamsBegin(memory)
// SectionFuncParamsAdd("param [param [param ...]]", "description")
SectionFuncParamsEnd(memory)
