#ifndef ACPL_I18N_CHARSETS_CHARSET_ISO_8859_16_2001_INC_CPP
#define ACPL_I18N_CHARSETS_CHARSET_ISO_8859_16_2001_INC_CPP

// WARNING: Do not modify this file because it was generated using i18nproc

#include "../../Charsets.h"
#include "../../_cp/Memory.inc.cpp"

namespace acpl
{
	namespace i18n
	{
		class Charset_ISO_8859_16_2001
		{
			public:
				static inline bool IsCompatible(const char *nMimeName, acpl::Charsets::ByteOrderMask nBom, acpl::Charsets::Fpc &nFpc)
				{
					if (nMimeName != NULL &&
						acpl::pi::_string_ascii_strcasecmp(nMimeName, "ISO-8859-16") != 0 &&
						acpl::pi::_string_ascii_strcasecmp(nMimeName, "ISO_8859-16") != 0 &&
						acpl::pi::_string_ascii_strcasecmp(nMimeName, "latin10") != 0 &&
						acpl::pi::_string_ascii_strcasecmp(nMimeName, "l10") != 0 &&
						acpl::pi::_string_ascii_strcasecmp(nMimeName, "iso-ir-226") != 0)
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
					return "ISO-8859-16";
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
								oChar == 0xA7 ||
								oChar == 0xA9 ||
								oChar == 0xAB ||
								oChar == 0xAD ||
								(oChar >= 0xB0 && oChar <= 0xB1) ||
								(oChar >= 0xB6 && oChar <= 0xB7) ||
								oChar == 0xBB ||
								(oChar >= 0xC0 && oChar <= 0xC2) ||
								oChar == 0xC4 ||
								(oChar >= 0xC6 && oChar <= 0xCF) ||
								(oChar >= 0xD2 && oChar <= 0xD4) ||
								oChar == 0xD6 ||
								(oChar >= 0xD9 && oChar <= 0xDC) ||
								(oChar >= 0xDF && oChar <= 0xE2) ||
								oChar == 0xE4 ||
								(oChar >= 0xE6 && oChar <= 0xEF) ||
								(oChar >= 0xF2 && oChar <= 0xF4) ||
								oChar == 0xF6 ||
								(oChar >= 0xF9 && oChar <= 0xFC) ||
								oChar == 0xFF
							)
						{
							nUnicodeChar = static_cast<acpl::Unichar>(oChar);
							return 1;
						}
						
						// Unique mappings
						if (oChar == 0xA1) nUnicodeChar = 0x0104; else
						if (oChar == 0xA2) nUnicodeChar = 0x0105; else
						if (oChar == 0xA3) nUnicodeChar = 0x0141; else
						if (oChar == 0xA4) nUnicodeChar = 0x20AC; else
						if (oChar == 0xA5) nUnicodeChar = 0x201E; else
						if (oChar == 0xA6) nUnicodeChar = 0x0160; else
						if (oChar == 0xA8) nUnicodeChar = 0x0161; else
						if (oChar == 0xAA) nUnicodeChar = 0x0218; else
						if (oChar == 0xAC) nUnicodeChar = 0x0179; else
						if (oChar == 0xAE) nUnicodeChar = 0x017A; else
						if (oChar == 0xAF) nUnicodeChar = 0x017B; else
						if (oChar == 0xB2) nUnicodeChar = 0x010C; else
						if (oChar == 0xB3) nUnicodeChar = 0x0142; else
						if (oChar == 0xB4) nUnicodeChar = 0x017D; else
						if (oChar == 0xB5) nUnicodeChar = 0x201D; else
						if (oChar == 0xB8) nUnicodeChar = 0x017E; else
						if (oChar == 0xB9) nUnicodeChar = 0x010D; else
						if (oChar == 0xBA) nUnicodeChar = 0x0219; else
						if (oChar == 0xBC) nUnicodeChar = 0x0152; else
						if (oChar == 0xBD) nUnicodeChar = 0x0153; else
						if (oChar == 0xBE) nUnicodeChar = 0x0178; else
						if (oChar == 0xBF) nUnicodeChar = 0x017C; else
						if (oChar == 0xC3) nUnicodeChar = 0x0102; else
						if (oChar == 0xC5) nUnicodeChar = 0x0106; else
						if (oChar == 0xD0) nUnicodeChar = 0x0110; else
						if (oChar == 0xD1) nUnicodeChar = 0x0143; else
						if (oChar == 0xD5) nUnicodeChar = 0x0150; else
						if (oChar == 0xD7) nUnicodeChar = 0x015A; else
						if (oChar == 0xD8) nUnicodeChar = 0x0170; else
						if (oChar == 0xDD) nUnicodeChar = 0x0118; else
						if (oChar == 0xDE) nUnicodeChar = 0x021A; else
						if (oChar == 0xE3) nUnicodeChar = 0x0103; else
						if (oChar == 0xE5) nUnicodeChar = 0x0107; else
						if (oChar == 0xF0) nUnicodeChar = 0x0111; else
						if (oChar == 0xF1) nUnicodeChar = 0x0144; else
						if (oChar == 0xF5) nUnicodeChar = 0x0151; else
						if (oChar == 0xF7) nUnicodeChar = 0x015B; else
						if (oChar == 0xF8) nUnicodeChar = 0x0171; else
						if (oChar == 0xFD) nUnicodeChar = 0x0119; else
						if (oChar == 0xFE) nUnicodeChar = 0x021B; else
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
								nUnicodeChar == 0xA7 ||
								nUnicodeChar == 0xA9 ||
								nUnicodeChar == 0xAB ||
								nUnicodeChar == 0xAD ||
								(nUnicodeChar >= 0xB0 && nUnicodeChar <= 0xB1) ||
								(nUnicodeChar >= 0xB6 && nUnicodeChar <= 0xB7) ||
								nUnicodeChar == 0xBB ||
								(nUnicodeChar >= 0xC0 && nUnicodeChar <= 0xC2) ||
								nUnicodeChar == 0xC4 ||
								(nUnicodeChar >= 0xC6 && nUnicodeChar <= 0xCF) ||
								(nUnicodeChar >= 0xD2 && nUnicodeChar <= 0xD4) ||
								nUnicodeChar == 0xD6 ||
								(nUnicodeChar >= 0xD9 && nUnicodeChar <= 0xDC) ||
								(nUnicodeChar >= 0xDF && nUnicodeChar <= 0xE2) ||
								nUnicodeChar == 0xE4 ||
								(nUnicodeChar >= 0xE6 && nUnicodeChar <= 0xEF) ||
								(nUnicodeChar >= 0xF2 && nUnicodeChar <= 0xF4) ||
								nUnicodeChar == 0xF6 ||
								(nUnicodeChar >= 0xF9 && nUnicodeChar <= 0xFC) ||
								nUnicodeChar == 0xFF
							)
						{
							*nBfr = static_cast<acpl::UInt8>(nUnicodeChar);
							return 1;
						}
						
						// Unique mappings
						if (nUnicodeChar == 0x0104) *nBfr = 0xA1; else
						if (nUnicodeChar == 0x0105) *nBfr = 0xA2; else
						if (nUnicodeChar == 0x0141) *nBfr = 0xA3; else
						if (nUnicodeChar == 0x20AC) *nBfr = 0xA4; else
						if (nUnicodeChar == 0x201E) *nBfr = 0xA5; else
						if (nUnicodeChar == 0x0160) *nBfr = 0xA6; else
						if (nUnicodeChar == 0x0161) *nBfr = 0xA8; else
						if (nUnicodeChar == 0x0218) *nBfr = 0xAA; else
						if (nUnicodeChar == 0x0179) *nBfr = 0xAC; else
						if (nUnicodeChar == 0x017A) *nBfr = 0xAE; else
						if (nUnicodeChar == 0x017B) *nBfr = 0xAF; else
						if (nUnicodeChar == 0x010C) *nBfr = 0xB2; else
						if (nUnicodeChar == 0x0142) *nBfr = 0xB3; else
						if (nUnicodeChar == 0x017D) *nBfr = 0xB4; else
						if (nUnicodeChar == 0x201D) *nBfr = 0xB5; else
						if (nUnicodeChar == 0x017E) *nBfr = 0xB8; else
						if (nUnicodeChar == 0x010D) *nBfr = 0xB9; else
						if (nUnicodeChar == 0x0219) *nBfr = 0xBA; else
						if (nUnicodeChar == 0x0152) *nBfr = 0xBC; else
						if (nUnicodeChar == 0x0153) *nBfr = 0xBD; else
						if (nUnicodeChar == 0x0178) *nBfr = 0xBE; else
						if (nUnicodeChar == 0x017C) *nBfr = 0xBF; else
						if (nUnicodeChar == 0x0102) *nBfr = 0xC3; else
						if (nUnicodeChar == 0x0106) *nBfr = 0xC5; else
						if (nUnicodeChar == 0x0110) *nBfr = 0xD0; else
						if (nUnicodeChar == 0x0143) *nBfr = 0xD1; else
						if (nUnicodeChar == 0x0150) *nBfr = 0xD5; else
						if (nUnicodeChar == 0x015A) *nBfr = 0xD7; else
						if (nUnicodeChar == 0x0170) *nBfr = 0xD8; else
						if (nUnicodeChar == 0x0118) *nBfr = 0xDD; else
						if (nUnicodeChar == 0x021A) *nBfr = 0xDE; else
						if (nUnicodeChar == 0x0103) *nBfr = 0xE3; else
						if (nUnicodeChar == 0x0107) *nBfr = 0xE5; else
						if (nUnicodeChar == 0x0111) *nBfr = 0xF0; else
						if (nUnicodeChar == 0x0144) *nBfr = 0xF1; else
						if (nUnicodeChar == 0x0151) *nBfr = 0xF5; else
						if (nUnicodeChar == 0x015B) *nBfr = 0xF7; else
						if (nUnicodeChar == 0x0171) *nBfr = 0xF8; else
						if (nUnicodeChar == 0x0119) *nBfr = 0xFD; else
						if (nUnicodeChar == 0x021B) *nBfr = 0xFE; else
							return 0;
						
						return 1;
					}
					
					return 0;
				}
		};
	}
}

#endif // ACPL_I18N_CHARSETS_CHARSET_ISO_8859_16_2001_INC_CPP
