#ifndef ACPL_CP_MEMORY_INC_CPP
#define ACPL_CP_MEMORY_INC_CPP

#include "../Memory.h"


namespace acpl
{
	
	// 
	// (Common) string functions
	// 
	
	
	// ASCII
	
	namespace pi
	{
		static inline int _string_ascii_strcasecmp(const char *nStr1, const char *nStr2)
		{
			for (acpl::SizeT i = 0; ; i++)
			{
				char oChr1 = nStr1[i];
				if (oChr1 >= 'A' && oChr1 <= 'Z')
					oChr1 += 0x20;
				
				char oChr2 = nStr2[i];
				if (oChr2 >= 'A' && oChr2 <= 'Z')
					oChr2 += 0x20;
				
				if (oChr1 == oChr2)
				{
					if (oChr1 != '\0')
						continue;
					else
						break;
				}
				else
					return 1;
			}
			
			return 0;
		}
		
		static inline bool _string_ascii_is_between(char nChar, char nFirstChar, char nLastChar)
		{
			return (nChar <= nLastChar && nChar >= nFirstChar);
		}
		
		static inline bool _string_ascii_is_upper(char nChar)
		{
			return acpl::pi::_string_ascii_is_between(nChar, 'A', 'Z');
		}

		static inline bool _string_ascii_is_lower(char nChar)
		{
			return acpl::pi::_string_ascii_is_between(nChar, 'a', 'z');
		}

		static inline bool _string_ascii_is_alpha(char nChar)
		{
			return (acpl::pi::_string_ascii_is_lower(nChar) == true || acpl::pi::_string_ascii_is_upper(nChar) == true);
		}

		static inline bool _string_ascii_is_digit(char nChar)
		{
			return acpl::pi::_string_ascii_is_between(nChar, '0', '9');
		}

		static inline bool _string_ascii_is_xdigit(char nChar)
		{
			return (acpl::pi::_string_ascii_is_digit(nChar) == true || acpl::pi::_string_ascii_is_between(nChar, 'a', 'f') || acpl::pi::_string_ascii_is_between(nChar, 'A', 'F'));
		}
		
		static inline bool _string_ascii_is_alnum(char nChar)
		{
			return (acpl::pi::_string_ascii_is_alpha(nChar) == true || acpl::pi::_string_ascii_is_digit(nChar) == true);
		}
		
		template <class tType>
		static inline bool _string_ascii_char2num(char nChar, acpl::UInt8 nBase, tType &nNum)
		{
			if (acpl::pi::_string_ascii_is_digit(nChar) == true)
				nChar -= '0';
			else
			if (acpl::pi::_string_ascii_is_upper(nChar) == true)
				nChar -= 'A' - 10;
			else
			if (acpl::pi::_string_ascii_is_lower(nChar) == true)
				nChar -= 'a' - 10;
			else
				return false;
			
			if (nChar < nBase)
			{
				nNum *= nBase;
				nNum += nChar;
				return true;
			}
			else
				return false;
		}
		
		template <class tType>
		static inline void _string_ascii_num2char(tType &nValue, acpl::UInt8 nBase, bool nUppercase, char *&nCurPos)
		{
			const char *oAlphabet = (nUppercase == true) ? "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ" : "0123456789abcdefghijklmnopqrstuvwxyz";
			
			do
			{
				*--nCurPos = oAlphabet[nValue % nBase];
				nValue /= nBase;
			}
			while (nValue != 0);
		}
		
		
		// UTF-8
		// WARNING: For optimization reasons there is no sequence validity checking
		// in these functions so the inputs to these functions have to be VALID
		// sequences or parts of VALID sequences!!!
		
		static inline bool _string_utf8_check_main_byte(acpl::UInt8 nByte) // Returns true if byte is sequence's main byte, false otherwise
		{
			return ((nByte & 0xC0) != 0x80);
		}
		
		static inline bool _string_utf8_check_follow_byte(acpl::UInt8 nByte) // Returns true if byte is sequence's follow byte, false otherwise
		{
			return ((nByte & 0xC0) == 0x80);
		}
		
		static inline bool _string_utf8_dec_follow_byte(acpl::UInt8 nByte, acpl::Unichar &nUc)
		{
			if (acpl::pi::_string_utf8_check_follow_byte(nByte) == true)
			{
				nUc <<= 6;
				nUc |= (nByte & 0x3F);
				return true;
			}
			else
				return false;
		}
		
		static inline void _string_utf8_prev_seq(const char *&nPtr)
		{
			for (nPtr--; acpl::pi::_string_utf8_check_follow_byte(*nPtr) == true; nPtr--) ;
		}
		
		static inline void _string_utf8_next_seq(const char *&nPtr)
		{
			for (nPtr++; acpl::pi::_string_utf8_check_follow_byte(*nPtr) == true; nPtr++) ;
		}
		
		static inline bool _string_utf8_skip_bwd(const char *&nPtr, const char *nBegPtr, acpl::SizeT &nCount) // Returns true if nCount sequences were skipped, false otherwise (if beginning (nBegPtr) was reached)
		{
			while (nCount--)
			{
				if (nPtr == nBegPtr)
					return false;
				
				acpl::pi::_string_utf8_prev_seq(nPtr);
			}
			
			return true;
		}
		
		static inline bool _string_utf8_skip_fwd(const char *&nPtr, acpl::SizeT &nCount) // Returns true if nCount sequences were skipped, false otherwise (if NULL terminator was reached)
		{
			while (nCount--)
			{
				if (*nPtr == '\0')
					return false;
				
				acpl::pi::_string_utf8_next_seq(nPtr);
			}
			
			return true;
		}
		
		static inline acpl::SizeT _string_utf8_enc_size(acpl::Unichar nUnicodeChar) // Returns the size of encoded unicode code point in UTF-8 (if invalid, the size is still 1 because the char will be replaced with '?')
		{
			if (nUnicodeChar < 0x80)
				return 1;
			else
			if (nUnicodeChar < 0x800)
				return 2;
			else
			if (nUnicodeChar < 0x10000)
				return 3;
			else
			if (nUnicodeChar < 0x200000)
				return 4;
			else
			if (nUnicodeChar < 0x4000000)
				return 5;
			else
			if (nUnicodeChar < 0x80000000)
				return 6;
			else
				return 1; // even if invalid, the size is still 1 because the char will be replaced with '?'
		}
		
		// continues after includes
	}
}


#if   (acplCRuntime == acplCRuntimeGlibc)
#	include "glibc/Memory.inc.cpp"
#elif (acplCRuntime == acplCRuntimeMscrt)
#	include "mscrt/Memory.inc.cpp"
#else
#	error Inclusion of platform-specific `Memory.inc.cpp` required
#endif


namespace acpl
{
	
	// 
	// (Common) string functions (continuation)
	// 
	
	namespace pi
	{
		static inline bool _string_mbc_to_wchar(acpl::cp::_string_wchar_cont_t &nWcCont, const char *nMbs, acpl::SizeT &nMbsOff, acpl::cp::_string_mb_state_t &nMbState) // returns true if wchar was obtained (even if there was an decoding error in which case the wchar will be '?'), false otherwise (if a NULL terminator was detected)
		{
			acpl::SSizeT oConvRet = acpl::cp::_string_mbc_to_wchar(nWcCont, nMbs + nMbsOff, acpl::Num<acpl::SizeT>::Max(), nMbState);
			
			if (oConvRet > 0) // normal character
				nMbsOff += oConvRet;
			else
			if (oConvRet == 0) // NULL terminator
			{
				nWcCont[0] = L'\0';
				return false;
			}
			else // error; usually -1; -2 should never happen (and even if it does, it is most certainly an error)
			{
				nWcCont[0] = L'?';
				nMbsOff++;
			}
			
			return true;
		}
		
		static inline acpl::SizeT _string_wchar_to_mbc(acpl::cp::_string_mbc_cont_t &nMbcCont, const acpl::cp::_string_wchar_cont_t &nWcCont, acpl::cp::_string_mb_state_t &nMbState) // returns number of bytes encoded in MBC (even if there was an decoding error in which case the MBC will be '?')
		{
			acpl::SSizeT oConvRet = acpl::cp::_string_wchar_to_mbc(nMbcCont, nWcCont, nMbState);
			
			if (oConvRet > 0) // normal character
				return static_cast<acpl::SizeT>(oConvRet);
			else
			if (oConvRet == 0) // NULL terminator
				nMbcCont[0] = '\0';
			else // error
			{
				nMbcCont[0] = '?';
			}
			
			return 1;
		}
	}
	
}


#endif // ACPL_CP_MEMORY_INC_CPP
