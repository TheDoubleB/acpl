#ifndef ACPL_CP_MSCRT_MEMORY_INC_CPP
#define ACPL_CP_MSCRT_MEMORY_INC_CPP

#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <wchar.h>

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
			return _aligned_malloc(nSize, nAlignment);
		}
		
		static inline void _mem_free_aligned(void *nPtr)
		{
			return _aligned_free(nPtr);
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
		
		typedef wchar_t _string_wchar_cont_t[2];
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
			size_t oRetVal;
			
			if (wcrtomb_s(&oRetVal, nMbcCont, sizeof(nMbcCont), nWcCont[0], &nMbState) != 0)
				oRetVal = static_cast<size_t>(-1);
			
			return static_cast<acpl::SSizeT>(oRetVal);
		}
		
		static inline acpl::SizeT _string_wchar_seq_size(const wchar_t *nSeqStart)
		{
			if (nSeqStart[0] < 0xD800 || nSeqStart[0] > 0xDFFF)
				return 1;
			else
			if (nSeqStart[0] < 0xDC00 && nSeqStart[1] >= 0xDC00 && nSeqStart[1] <= 0xDFFF)
				return 2;
			else
				return 1;
		}
		
		static inline acpl::SizeT _string_wchar_seq_decode(const wchar_t *nSeqStart, acpl::Unichar &nUnichar)
		{
			if (nSeqStart[0] < 0xD800 || nSeqStart[0] > 0xDFFF)
			{
				nUnichar = nSeqStart[0];
				return 1;
			}
			else
			if (nSeqStart[0] < 0xDC00 && nSeqStart[1] >= 0xDC00 && nSeqStart[1] <= 0xDFFF)
			{
				nUnichar = (((nSeqStart[0] & 0x3FF) << 10) | (nSeqStart[1] & 0x3FF)) + 0x10000;
				return 2;
			}
			else
			{
				nUnichar = '?';
				return 1;
			}
		}
		
		static inline acpl::SizeT _string_wchar_seq_encode(acpl::Unichar nUnichar, acpl::cp::_string_wchar_cont_t &nWcCont)
		{
			if (nUnichar <= 0xFFFF)
			{
				if (nUnichar < 0xD800 || nUnichar > 0xDFFF) // this `if` needs to be nested because of return value
				{
					nWcCont[0] = nUnichar;
					return 1;
				}
			}
			else
			if (nUnichar <= 0x10FFFF)
			{
				nWcCont[0] = (0xD800 | ((nUnichar - 0x10000) >> 10));
				nWcCont[1] = (0xDC00 | ((nUnichar - 0x10000) & 0x3FF));
				return 2;
			}
			
			nWcCont[0] = L'?';
			return 1;
		}
	}
	
}

#endif // ACPL_CP_MSCRT_MEMORY_INC_CPP
