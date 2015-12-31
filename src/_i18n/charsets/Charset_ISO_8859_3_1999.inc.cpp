#ifndef ACPL_I18N_CHARSETS_CHARSET_ISO_8859_3_1999_INC_CPP
#define ACPL_I18N_CHARSETS_CHARSET_ISO_8859_3_1999_INC_CPP

// WARNING: Do not modify this file because it was generated using i18nproc

#include "../../Charsets.h"
#include "../../_cp/Memory.inc.cpp"

namespace acpl
{
	namespace i18n
	{
		class Charset_ISO_8859_3_1999
		{
			public:	
				static inline bool IsCompatible(const char *nMimeName, acpl::Charsets::ByteOrderMask nBom, acpl::Charsets::Fpc &nFpc)
				{
					if (nMimeName != NULL &&
						acpl::pi::_string_ascii_strcasecmp(nMimeName, "ISO-8859-3") != 0 &&
						acpl::pi::_string_ascii_strcasecmp(nMimeName, "ISO_8859-3") != 0 &&
						acpl::pi::_string_ascii_strcasecmp(nMimeName, "latin3") != 0 &&
						acpl::pi::_string_ascii_strcasecmp(nMimeName, "l3") != 0 &&
						acpl::pi::_string_ascii_strcasecmp(nMimeName, "iso-ir-109") != 0 &&
						acpl::pi::_string_ascii_strcasecmp(nMimeName, "csISOLatin3") != 0)
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
					return "ISO-8859-3";
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
								(oChar >= 0xA3 && oChar <= 0xA4) ||
								(oChar >= 0xA7 && oChar <= 0xA8) ||
								oChar == 0xAD ||
												oChar == 0xB0 ||
								(oChar >= 0xB2 && oChar <= 0xB5) ||
								(oChar >= 0xB7 && oChar <= 0xB8) ||
								oChar == 0xBD ||
								(oChar >= 0xC0 && oChar <= 0xC2) ||
								oChar == 0xC4 ||
								(oChar >= 0xC7 && oChar <= 0xCF) ||
								(oChar >= 0xD1 && oChar <= 0xD4) ||
								(oChar >= 0xD6 && oChar <= 0xD7) ||
								(oChar >= 0xD9 && oChar <= 0xDC) ||
								(oChar >= 0xDF && oChar <= 0xE2) ||
								oChar == 0xE4 ||
								(oChar >= 0xE7 && oChar <= 0xEF) ||
								(oChar >= 0xF1 && oChar <= 0xF4) ||
								(oChar >= 0xF6 && oChar <= 0xF7) ||
								(oChar >= 0xF9 && oChar <= 0xFC)
							)
						{
							nUnicodeChar = static_cast<acpl::Unichar>(oChar);
							return 1;
						}
						
						// Unique mappings
						if (oChar == 0xA1) nUnicodeChar = 0x0126; else
						if (oChar == 0xA2) nUnicodeChar = 0x02D8; else
						if (oChar == 0xA6) nUnicodeChar = 0x0124; else
						if (oChar == 0xA9) nUnicodeChar = 0x0130; else
						if (oChar == 0xAA) nUnicodeChar = 0x015E; else
						if (oChar == 0xAB) nUnicodeChar = 0x011E; else
						if (oChar == 0xAC) nUnicodeChar = 0x0134; else
						if (oChar == 0xAF) nUnicodeChar = 0x017B; else
						if (oChar == 0xB1) nUnicodeChar = 0x0127; else
						if (oChar == 0xB6) nUnicodeChar = 0x0125; else
						if (oChar == 0xB9) nUnicodeChar = 0x0131; else
						if (oChar == 0xBA) nUnicodeChar = 0x015F; else
						if (oChar == 0xBB) nUnicodeChar = 0x011F; else
						if (oChar == 0xBC) nUnicodeChar = 0x0135; else
						if (oChar == 0xBF) nUnicodeChar = 0x017C; else
						if (oChar == 0xC5) nUnicodeChar = 0x010A; else
						if (oChar == 0xC6) nUnicodeChar = 0x0108; else
						if (oChar == 0xD5) nUnicodeChar = 0x0120; else
						if (oChar == 0xD8) nUnicodeChar = 0x011C; else
						if (oChar == 0xDD) nUnicodeChar = 0x016C; else
						if (oChar == 0xDE) nUnicodeChar = 0x015C; else
						if (oChar == 0xE5) nUnicodeChar = 0x010B; else
						if (oChar == 0xE6) nUnicodeChar = 0x0109; else
						if (oChar == 0xF5) nUnicodeChar = 0x0121; else
						if (oChar == 0xF8) nUnicodeChar = 0x011D; else
						if (oChar == 0xFD) nUnicodeChar = 0x016D; else
						if (oChar == 0xFE) nUnicodeChar = 0x015D; else
						if (oChar == 0xFF) nUnicodeChar = 0x02D9; else
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
								(nUnicodeChar >= 0xA3 && nUnicodeChar <= 0xA4) ||
								(nUnicodeChar >= 0xA7 && nUnicodeChar <= 0xA8) ||
								nUnicodeChar == 0xAD ||
								nUnicodeChar == 0xB0 ||
								(nUnicodeChar >= 0xB2 && nUnicodeChar <= 0xB5) ||
								(nUnicodeChar >= 0xB7 && nUnicodeChar <= 0xB8) ||
								nUnicodeChar == 0xBD ||
								(nUnicodeChar >= 0xC0 && nUnicodeChar <= 0xC2) ||
								nUnicodeChar == 0xC4 ||
								(nUnicodeChar >= 0xC7 && nUnicodeChar <= 0xCF) ||
								(nUnicodeChar >= 0xD1 && nUnicodeChar <= 0xD4) ||
								(nUnicodeChar >= 0xD6 && nUnicodeChar <= 0xD7) ||
								(nUnicodeChar >= 0xD9 && nUnicodeChar <= 0xDC) ||
								(nUnicodeChar >= 0xDF && nUnicodeChar <= 0xE2) ||
								nUnicodeChar == 0xE4 ||
								(nUnicodeChar >= 0xE7 && nUnicodeChar <= 0xEF) ||
								(nUnicodeChar >= 0xF1 && nUnicodeChar <= 0xF4) ||
								(nUnicodeChar >= 0xF6 && nUnicodeChar <= 0xF7) ||
								(nUnicodeChar >= 0xF9 && nUnicodeChar <= 0xFC)
							)
						{
							*nBfr = static_cast<acpl::UInt8>(nUnicodeChar);
							return 1;
						}
						
						// Unique mappings
						if (nUnicodeChar == 0x0126) *nBfr = 0xA1; else
						if (nUnicodeChar == 0x02D8) *nBfr = 0xA2; else
						if (nUnicodeChar == 0x0124) *nBfr = 0xA6; else
						if (nUnicodeChar == 0x0130) *nBfr = 0xA9; else
						if (nUnicodeChar == 0x015E) *nBfr = 0xAA; else
						if (nUnicodeChar == 0x011E) *nBfr = 0xAB; else
						if (nUnicodeChar == 0x0134) *nBfr = 0xAC; else
						if (nUnicodeChar == 0x017B) *nBfr = 0xAF; else
						if (nUnicodeChar == 0x0127) *nBfr = 0xB1; else
						if (nUnicodeChar == 0x0125) *nBfr = 0xB6; else
						if (nUnicodeChar == 0x0131) *nBfr = 0xB9; else
						if (nUnicodeChar == 0x015F) *nBfr = 0xBA; else
						if (nUnicodeChar == 0x011F) *nBfr = 0xBB; else
						if (nUnicodeChar == 0x0135) *nBfr = 0xBC; else
						if (nUnicodeChar == 0x017C) *nBfr = 0xBF; else
						if (nUnicodeChar == 0x010A) *nBfr = 0xC5; else
						if (nUnicodeChar == 0x0108) *nBfr = 0xC6; else
						if (nUnicodeChar == 0x0120) *nBfr = 0xD5; else
						if (nUnicodeChar == 0x011C) *nBfr = 0xD8; else
						if (nUnicodeChar == 0x016C) *nBfr = 0xDD; else
						if (nUnicodeChar == 0x015C) *nBfr = 0xDE; else
						if (nUnicodeChar == 0x010B) *nBfr = 0xE5; else
						if (nUnicodeChar == 0x0109) *nBfr = 0xE6; else
						if (nUnicodeChar == 0x0121) *nBfr = 0xF5; else
						if (nUnicodeChar == 0x011D) *nBfr = 0xF8; else
						if (nUnicodeChar == 0x016D) *nBfr = 0xFD; else
						if (nUnicodeChar == 0x015D) *nBfr = 0xFE; else
						if (nUnicodeChar == 0x02D9) *nBfr = 0xFF; else
							return 0;
						
						return 1;
					}
					
					return 0;
				}
		};
	}
}

#endif // ACPL_I18N_CHARSETS_CHARSET_ISO_8859_3_1999_INC_CPP
