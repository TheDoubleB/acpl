#ifndef ACPL_CP_GLIBC_MEMORY_INC_CPP
#define ACPL_CP_GLIBC_MEMORY_INC_CPP

#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <limits.h>

#include "../../Memory.h"

namespace acpl
{
	
	namespace cp
	{
		// 
		// Memory allocation (non-aligned)
		// 
		
		static inline void *_mem_alloc(acpl::SizeT nSize)
		{
			return malloc(nSize);
		}
		
		static inline void *_mem_realloc(void *nPtr, acpl::SizeT nSize)
		{
			return realloc(nPtr, nSize);
		}
		
		static inline void _mem_free(void *nPtr)
		{
			return free(nPtr);
		}
		
		
		// 
		// Memory allocation (aligned)
		// 
		
		static inline void *_mem_alloc_aligned(acpl::SizeT nAlignment, acpl::SizeT nSize)
		{
			void *oPtr = NULL;
			if (posix_memalign(&oPtr, nAlignment, nSize) == 0)
				return oPtr;
			else
				return NULL;
		}
		
		static inline void _mem_free_aligned(void *nPtr)
		{
			return free(nPtr);
		}
		
		
		// 
		// Memory movement
		// 
		
		static inline void *_mem_copy(void *nDest, const void *nSrc, acpl::SizeT nSize)
		{
			return memcpy(nDest, nSrc, nSize);
		}
		
		static inline void *_mem_move(void *nDest, const void *nSrc, acpl::SizeT nSize)
		{
			return memmove(nDest, nSrc, nSize);
		}
		
		static inline void *_mem_set(void *nPtr, acpl::UInt8 nByte, acpl::SizeT nSize)
		{
			return memset(nPtr, nByte, nSize);
		}
		
		
		// 
		// String
		// 
		
		typedef wchar_t _string_wchar_cont_t[1];
		typedef char _string_mbc_cont_t[6]; // MB_LEN_MAX is weird
		typedef mbstate_t _string_mb_state_t;
		
		static inline void _string_mb_state_init(acpl::cp::_string_mb_state_t &nMbState)
		{
			acpl::Mem::SetByte(&nMbState, 0, 1);
		}
		
		static inline void _string_mb_state_destroy(acpl::cp::_string_mb_state_t &)
		{
			
		}
		
		static inline acpl::SSizeT _string_mbc_to_wchar(acpl::cp::_string_wchar_cont_t &nWcCont, const char *nMbs, acpl::SizeT nMbsSize, acpl::cp::_string_mb_state_t &nMbState)
		{
			return static_cast<acpl::SSizeT>(mbrtowc(nWcCont, nMbs, nMbsSize, &nMbState));
		}
		
		static inline acpl::SSizeT _string_wchar_to_mbc(acpl::cp::_string_mbc_cont_t &nMbcCont, const acpl::cp::_string_wchar_cont_t &nWcCont, acpl::cp::_string_mb_state_t &nMbState)
		{
			return static_cast<acpl::SSizeT>(wcrtomb(nMbcCont, nWcCont[0], &nMbState));
		}
		
		static inline acpl::SizeT _string_wchar_seq_size(const wchar_t *) // sequence size for wchar_t is always 1 in glibc
		{
			return 1;
		}
		
		static inline acpl::SizeT _string_wchar_seq_decode(const wchar_t *nSeqStart, acpl::Unichar &nUnichar) // sequence size for wchar_t is always 1 in glibc
		{
			nUnichar = *nSeqStart; // wchar_t in glibc is a 4-byte type in UCS-4 encoding just like acpl::Unichar
			return 1;
		}
		
		static inline acpl::SizeT _string_wchar_seq_encode(acpl::Unichar nUnichar, acpl::cp::_string_wchar_cont_t &nWcCont) // sequence size for wchar_t is always 1 in glibc
		{
			nWcCont[0] = nUnichar; // wchar_t in glibc is a 4-byte type in UCS-4 encoding just like acpl::Unichar
			return 1;
		}
	}
	
}

#endif // ACPL_CP_GLIBC_MEMORY_INC_CPP
