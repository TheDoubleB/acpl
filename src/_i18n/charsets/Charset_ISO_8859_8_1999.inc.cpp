#ifndef ACPL_I18N_CHARSETS_CHARSET_ISO_8859_8_1999_INC_CPP
#define ACPL_I18N_CHARSETS_CHARSET_ISO_8859_8_1999_INC_CPP

// WARNING: Do not modify this file because it was generated using i18nproc

#include "../../Charsets.h"
#include "../../_cp/Memory.inc.cpp"

namespace acpl
{
	namespace i18n
	{
		class Charset_ISO_8859_8_1999
		{
			public:
				static inline bool IsCompatible(const char *nMimeName, acpl::Charsets::ByteOrderMask nBom, acpl::Charsets::Fpc &nFpc)
				{
					if (nMimeName != NULL &&
						acpl::pi::_string_ascii_strcasecmp(nMimeName, "ISO-8859-8") != 0 &&
						acpl::pi::_string_ascii_strcasecmp(nMimeName, "ISO_8859-8") != 0 &&
						acpl::pi::_string_ascii_strcasecmp(nMimeName, "hebrew") != 0 &&
						acpl::pi::_string_ascii_strcasecmp(nMimeName, "iso-ir-138") != 0 &&
						acpl::pi::_string_ascii_strcasecmp(nMimeName, "csISOLatinHebrew") != 0)
						return false;
					
					if (nBom != acpl::Charsets::bomNone)
						return false;
					
					nFpc.Set(
							IsCompatible,
							GetMimeName,
							IsBomSuggested,
							CreateBom,
							MinSeqSize,
							MaxSeqSize,
							Decode,
							Encode
						);

					return true;
				}
				
				static inline const char *GetMimeName()
				{
					return "ISO-8859-8";
				}
				
				static inline bool IsBomSuggested()
				{
					return false;
				}
				
				static inline acpl::SizeT CreateBom(acpl::UInt8 *, acpl::SizeT)
				{
					return 0;
				}
				
				static inline acpl::SizeT MinSeqSize()
				{
					return 1;
				}
				
				static inline acpl::SizeT MaxSeqSize()
				{
					return 1;
				}
				
				static inline acpl::SizeT Decode(const acpl::UInt8 *nBfr, acpl::SizeT nBfrSize, acpl::Unichar &nUnicodeChar)
				{
					if (nBfrSize >= 1)
					{
						acpl::UInt8 oChar = *nBfr;
						
						// Mirror mappings
						if (
								oChar <= 0xA0 ||
								(oChar >= 0xA2 && oChar <= 0xA9) ||
								(oChar >= 0xAB && oChar <= 0xB9) ||
								(oChar >= 0xBB && oChar <= 0xBE)
							)
						{
							nUnicodeChar = static_cast<acpl::Unichar>(oChar);
							return 1;
						}
						
						// Unique mappings
						if (oChar == 0xAA) nUnicodeChar = 0x000D7; else
						if (oChar == 0xBA) nUnicodeChar = 0x000F7; else
						if (oChar == 0xDF) nUnicodeChar = 0x2017; else
						if (oChar == 0xE0) nUnicodeChar = 0x05D0; else
						if (oChar == 0xE1) nUnicodeChar = 0x05D1; else
						if (oChar == 0xE2) nUnicodeChar = 0x05D2; else
						if (oChar == 0xE3) nUnicodeChar = 0x05D3; else
						if (oChar == 0xE4) nUnicodeChar = 0x05D4; else
						if (oChar == 0xE5) nUnicodeChar = 0x05D5; else
						if (oChar == 0xE6) nUnicodeChar = 0x05D6; else
						if (oChar == 0xE7) nUnicodeChar = 0x05D7; else
						if (oChar == 0xE8) nUnicodeChar = 0x05D8; else
						if (oChar == 0xE9) nUnicodeChar = 0x05D9; else
						if (oChar == 0xEA) nUnicodeChar = 0x05DA; else
						if (oChar == 0xEB) nUnicodeChar = 0x05DB; else
						if (oChar == 0xEC) nUnicodeChar = 0x05DC; else
						if (oChar == 0xED) nUnicodeChar = 0x05DD; else
						if (oChar == 0xEE) nUnicodeChar = 0x05DE; else
						if (oChar == 0xEF) nUnicodeChar = 0x05DF; else
						if (oChar == 0xF0) nUnicodeChar = 0x05E0; else
						if (oChar == 0xF1) nUnicodeChar = 0x05E1; else
						if (oChar == 0xF2) nUnicodeChar = 0x05E2; else
						if (oChar == 0xF3) nUnicodeChar = 0x05E3; else
						if (oChar == 0xF4) nUnicodeChar = 0x05E4; else
						if (oChar == 0xF5) nUnicodeChar = 0x05E5; else
						if (oChar == 0xF6) nUnicodeChar = 0x05E6; else
						if (oChar == 0xF7) nUnicodeChar = 0x05E7; else
						if (oChar == 0xF8) nUnicodeChar = 0x05E8; else
						if (oChar == 0xF9) nUnicodeChar = 0x05E9; else
						if (oChar == 0xFA) nUnicodeChar = 0x05EA; else
						if (oChar == 0xFD) nUnicodeChar = 0x200E; else
						if (oChar == 0xFE) nUnicodeChar = 0x200F; else
							return 0;
						
						return 1;
					}
					
					return 0;
				}
				
				static inline acpl::SizeT Encode(acpl::Unichar nUnicodeChar, acpl::UInt8 *nBfr, acpl::SizeT nBfrSize)
				{
					if (nBfrSize >= 1)
					{
						// Mirror mappings
						if (
								nUnicodeChar <= 0xA0 ||
								(nUnicodeChar >= 0xA2 && nUnicodeChar <= 0xA9) ||
								(nUnicodeChar >= 0xAB && nUnicodeChar <= 0xB9) ||
								(nUnicodeChar >= 0xBB && nUnicodeChar <= 0xBE)
							)
						{
							*nBfr = static_cast<acpl::UInt8>(nUnicodeChar);
							return 1;
						}
						
						// Unique mappings
						if (nUnicodeChar == 0x000D7) *nBfr = 0xAA; else
						if (nUnicodeChar == 0x000F7) *nBfr = 0xBA; else
						if (nUnicodeChar == 0x2017) *nBfr = 0xDF; else
						if (nUnicodeChar == 0x05D0) *nBfr = 0xE0; else
						if (nUnicodeChar == 0x05D1) *nBfr = 0xE1; else
						if (nUnicodeChar == 0x05D2) *nBfr = 0xE2; else
						if (nUnicodeChar == 0x05D3) *nBfr = 0xE3; else
						if (nUnicodeChar == 0x05D4) *nBfr = 0xE4; else
						if (nUnicodeChar == 0x05D5) *nBfr = 0xE5; else
						if (nUnicodeChar == 0x05D6) *nBfr = 0xE6; else
						if (nUnicodeChar == 0x05D7) *nBfr = 0xE7; else
						if (nUnicodeChar == 0x05D8) *nBfr = 0xE8; else
						if (nUnicodeChar == 0x05D9) *nBfr = 0xE9; else
						if (nUnicodeChar == 0x05DA) *nBfr = 0xEA; else
						if (nUnicodeChar == 0x05DB) *nBfr = 0xEB; else
						if (nUnicodeChar == 0x05DC) *nBfr = 0xEC; else
						if (nUnicodeChar == 0x05DD) *nBfr = 0xED; else
						if (nUnicodeChar == 0x05DE) *nBfr = 0xEE; else
						if (nUnicodeChar == 0x05DF) *nBfr = 0xEF; else
						if (nUnicodeChar == 0x05E0) *nBfr = 0xF0; else
						if (nUnicodeChar == 0x05E1) *nBfr = 0xF1; else
						if (nUnicodeChar == 0x05E2) *nBfr = 0xF2; else
						if (nUnicodeChar == 0x05E3) *nBfr = 0xF3; else
						if (nUnicodeChar == 0x05E4) *nBfr = 0xF4; else
						if (nUnicodeChar == 0x05E5) *nBfr = 0xF5; else
						if (nUnicodeChar == 0x05E6) *nBfr = 0xF6; else
						if (nUnicodeChar == 0x05E7) *nBfr = 0xF7; else
						if (nUnicodeChar == 0x05E8) *nBfr = 0xF8; else
						if (nUnicodeChar == 0x05E9) *nBfr = 0xF9; else
						if (nUnicodeChar == 0x05EA) *nBfr = 0xFA; else
						if (nUnicodeChar == 0x200E) *nBfr = 0xFD; else
						if (nUnicodeChar == 0x200F) *nBfr = 0xFE; else
							return 0;
						
						return 1;
					}
					
					return 0;
				}
		};
	}
}

#endif // ACPL_I18N_CHARSETS_CHARSET_ISO_8859_8_1999_INC_CPP
