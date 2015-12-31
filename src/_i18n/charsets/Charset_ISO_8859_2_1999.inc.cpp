#ifndef ACPL_I18N_CHARSETS_CHARSET_ISO_8859_2_1999_INC_CPP
#define ACPL_I18N_CHARSETS_CHARSET_ISO_8859_2_1999_INC_CPP

// WARNING: Do not modify this file because it was generated using i18nproc

#include "../../Charsets.h"
#include "../../_cp/Memory.inc.cpp"

namespace acpl
{
	namespace i18n
	{
		class Charset_ISO_8859_2_1999
		{
			public:
				static inline bool IsCompatible(const char *nMimeName, acpl::Charsets::ByteOrderMask nBom, acpl::Charsets::Fpc &nFpc)
				{
					if (nMimeName != NULL &&
						acpl::pi::_string_ascii_strcasecmp(nMimeName, "ISO-8859-2") != 0 &&
						acpl::pi::_string_ascii_strcasecmp(nMimeName, "ISO_8859-2") != 0 &&
						acpl::pi::_string_ascii_strcasecmp(nMimeName, "latin2") != 0 &&
						acpl::pi::_string_ascii_strcasecmp(nMimeName, "l2") != 0 &&
						acpl::pi::_string_ascii_strcasecmp(nMimeName, "iso-ir-101") != 0 &&
						acpl::pi::_string_ascii_strcasecmp(nMimeName, "csISOLatin2") != 0)
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
					return "ISO-8859-2";
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
								oChar == 0xA4 ||
								(oChar >= 0xA7 && oChar <= 0xA8) ||
								oChar == 0xAD ||
								oChar == 0xB0 ||
								oChar == 0xB4 ||
								oChar == 0xB8 ||
								(oChar >= 0xC1 && oChar <= 0xC2) ||
								oChar == 0xC4 ||
								oChar == 0xC7 ||
								oChar == 0xC9 ||
								oChar == 0xCB ||
								(oChar >= 0xCD && oChar <= 0xCE) ||
								(oChar >= 0xD3 && oChar <= 0xD4) ||
								(oChar >= 0xD6 && oChar <= 0xD7) ||
								oChar == 0xDA ||
								(oChar >= 0xDC && oChar <= 0xDD) ||
								oChar == 0xDF ||
								(oChar >= 0xE1 && oChar <= 0xE2) ||
								oChar == 0xE4 ||
								oChar == 0xE7 ||
								oChar == 0xE9 ||
								oChar == 0xEB ||
								(oChar >= 0xED && oChar <= 0xEE) ||
								(oChar >= 0xF3 && oChar <= 0xF4) ||
								(oChar >= 0xF6 && oChar <= 0xF7) ||
								oChar == 0xFA ||
								(oChar >= 0xFC && oChar <= 0xFD)
							)
						{
							nUnicodeChar = static_cast<acpl::Unichar>(oChar);
							return 1;
						}
						
						// Unique mappings
						if (oChar == 0xA1) nUnicodeChar = 0x0104; else
						if (oChar == 0xA2) nUnicodeChar = 0x02D8; else
						if (oChar == 0xA3) nUnicodeChar = 0x0141; else
						if (oChar == 0xA5) nUnicodeChar = 0x013D; else
						if (oChar == 0xA6) nUnicodeChar = 0x015A; else
						if (oChar == 0xA9) nUnicodeChar = 0x0160; else
						if (oChar == 0xAA) nUnicodeChar = 0x015E; else
						if (oChar == 0xAB) nUnicodeChar = 0x0164; else
						if (oChar == 0xAC) nUnicodeChar = 0x0179; else
						if (oChar == 0xAE) nUnicodeChar = 0x017D; else
						if (oChar == 0xAF) nUnicodeChar = 0x017B; else
						if (oChar == 0xB1) nUnicodeChar = 0x0105; else
						if (oChar == 0xB2) nUnicodeChar = 0x02DB; else
						if (oChar == 0xB3) nUnicodeChar = 0x0142; else
						if (oChar == 0xB5) nUnicodeChar = 0x013E; else
						if (oChar == 0xB6) nUnicodeChar = 0x015B; else
						if (oChar == 0xB7) nUnicodeChar = 0x02C7; else
						if (oChar == 0xB9) nUnicodeChar = 0x0161; else
						if (oChar == 0xBA) nUnicodeChar = 0x015F; else
						if (oChar == 0xBB) nUnicodeChar = 0x0165; else
						if (oChar == 0xBC) nUnicodeChar = 0x017A; else
						if (oChar == 0xBD) nUnicodeChar = 0x02DD; else
						if (oChar == 0xBE) nUnicodeChar = 0x017E; else
						if (oChar == 0xBF) nUnicodeChar = 0x017C; else
						if (oChar == 0xC0) nUnicodeChar = 0x0154; else
						if (oChar == 0xC3) nUnicodeChar = 0x0102; else
						if (oChar == 0xC5) nUnicodeChar = 0x0139; else
						if (oChar == 0xC6) nUnicodeChar = 0x0106; else
						if (oChar == 0xC8) nUnicodeChar = 0x010C; else
						if (oChar == 0xCA) nUnicodeChar = 0x0118; else
						if (oChar == 0xCC) nUnicodeChar = 0x011A; else
						if (oChar == 0xCF) nUnicodeChar = 0x010E; else
						if (oChar == 0xD0) nUnicodeChar = 0x0110; else
						if (oChar == 0xD1) nUnicodeChar = 0x0143; else
						if (oChar == 0xD2) nUnicodeChar = 0x0147; else
						if (oChar == 0xD5) nUnicodeChar = 0x0150; else
						if (oChar == 0xD8) nUnicodeChar = 0x0158; else
						if (oChar == 0xD9) nUnicodeChar = 0x016E; else
						if (oChar == 0xDB) nUnicodeChar = 0x0170; else
						if (oChar == 0xDE) nUnicodeChar = 0x0162; else
						if (oChar == 0xE0) nUnicodeChar = 0x0155; else
						if (oChar == 0xE3) nUnicodeChar = 0x0103; else
						if (oChar == 0xE5) nUnicodeChar = 0x013A; else
						if (oChar == 0xE6) nUnicodeChar = 0x0107; else
						if (oChar == 0xE8) nUnicodeChar = 0x010D; else
						if (oChar == 0xEA) nUnicodeChar = 0x0119; else
						if (oChar == 0xEC) nUnicodeChar = 0x011B; else
						if (oChar == 0xEF) nUnicodeChar = 0x010F; else
						if (oChar == 0xF0) nUnicodeChar = 0x0111; else
						if (oChar == 0xF1) nUnicodeChar = 0x0144; else
						if (oChar == 0xF2) nUnicodeChar = 0x0148; else
						if (oChar == 0xF5) nUnicodeChar = 0x0151; else
						if (oChar == 0xF8) nUnicodeChar = 0x0159; else
						if (oChar == 0xF9) nUnicodeChar = 0x016F; else
						if (oChar == 0xFB) nUnicodeChar = 0x0171; else
						if (oChar == 0xFE) nUnicodeChar = 0x0163; else
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
								nUnicodeChar == 0xA4 ||
								(nUnicodeChar >= 0xA7 && nUnicodeChar <= 0xA8) ||
								nUnicodeChar == 0xAD ||
								nUnicodeChar == 0xB0 ||
								nUnicodeChar == 0xB4 ||
								nUnicodeChar == 0xB8 ||
								(nUnicodeChar >= 0xC1 && nUnicodeChar <= 0xC2) ||
								nUnicodeChar == 0xC4 ||
								nUnicodeChar == 0xC7 ||
								nUnicodeChar == 0xC9 ||
								nUnicodeChar == 0xCB ||
								(nUnicodeChar >= 0xCD && nUnicodeChar <= 0xCE) ||
								(nUnicodeChar >= 0xD3 && nUnicodeChar <= 0xD4) ||
								(nUnicodeChar >= 0xD6 && nUnicodeChar <= 0xD7) ||
								nUnicodeChar == 0xDA ||
								(nUnicodeChar >= 0xDC && nUnicodeChar <= 0xDD) ||
								nUnicodeChar == 0xDF ||
								(nUnicodeChar >= 0xE1 && nUnicodeChar <= 0xE2) ||
								nUnicodeChar == 0xE4 ||
								nUnicodeChar == 0xE7 ||
								nUnicodeChar == 0xE9 ||
								nUnicodeChar == 0xEB ||
								(nUnicodeChar >= 0xED && nUnicodeChar <= 0xEE) ||
								(nUnicodeChar >= 0xF3 && nUnicodeChar <= 0xF4) ||
								(nUnicodeChar >= 0xF6 && nUnicodeChar <= 0xF7) ||
								nUnicodeChar == 0xFA ||
								(nUnicodeChar >= 0xFC && nUnicodeChar <= 0xFD)
							)
						{
							*nBfr = static_cast<acpl::UInt8>(nUnicodeChar);
							return 1;
						}
						
						// Unique mappings
						if (nUnicodeChar == 0x0104) *nBfr = 0xA1; else
						if (nUnicodeChar == 0x02D8) *nBfr = 0xA2; else
						if (nUnicodeChar == 0x0141) *nBfr = 0xA3; else
						if (nUnicodeChar == 0x013D) *nBfr = 0xA5; else
						if (nUnicodeChar == 0x015A) *nBfr = 0xA6; else
						if (nUnicodeChar == 0x0160) *nBfr = 0xA9; else
						if (nUnicodeChar == 0x015E) *nBfr = 0xAA; else
						if (nUnicodeChar == 0x0164) *nBfr = 0xAB; else
						if (nUnicodeChar == 0x0179) *nBfr = 0xAC; else
						if (nUnicodeChar == 0x017D) *nBfr = 0xAE; else
						if (nUnicodeChar == 0x017B) *nBfr = 0xAF; else
						if (nUnicodeChar == 0x0105) *nBfr = 0xB1; else
						if (nUnicodeChar == 0x02DB) *nBfr = 0xB2; else
						if (nUnicodeChar == 0x0142) *nBfr = 0xB3; else
						if (nUnicodeChar == 0x013E) *nBfr = 0xB5; else
						if (nUnicodeChar == 0x015B) *nBfr = 0xB6; else
						if (nUnicodeChar == 0x02C7) *nBfr = 0xB7; else
						if (nUnicodeChar == 0x0161) *nBfr = 0xB9; else
						if (nUnicodeChar == 0x015F) *nBfr = 0xBA; else
						if (nUnicodeChar == 0x0165) *nBfr = 0xBB; else
						if (nUnicodeChar == 0x017A) *nBfr = 0xBC; else
						if (nUnicodeChar == 0x02DD) *nBfr = 0xBD; else
						if (nUnicodeChar == 0x017E) *nBfr = 0xBE; else
						if (nUnicodeChar == 0x017C) *nBfr = 0xBF; else
						if (nUnicodeChar == 0x0154) *nBfr = 0xC0; else
						if (nUnicodeChar == 0x0102) *nBfr = 0xC3; else
						if (nUnicodeChar == 0x0139) *nBfr = 0xC5; else
						if (nUnicodeChar == 0x0106) *nBfr = 0xC6; else
						if (nUnicodeChar == 0x010C) *nBfr = 0xC8; else
						if (nUnicodeChar == 0x0118) *nBfr = 0xCA; else
						if (nUnicodeChar == 0x011A) *nBfr = 0xCC; else
						if (nUnicodeChar == 0x010E) *nBfr = 0xCF; else
						if (nUnicodeChar == 0x0110) *nBfr = 0xD0; else
						if (nUnicodeChar == 0x0143) *nBfr = 0xD1; else
						if (nUnicodeChar == 0x0147) *nBfr = 0xD2; else
						if (nUnicodeChar == 0x0150) *nBfr = 0xD5; else
						if (nUnicodeChar == 0x0158) *nBfr = 0xD8; else
						if (nUnicodeChar == 0x016E) *nBfr = 0xD9; else
						if (nUnicodeChar == 0x0170) *nBfr = 0xDB; else
						if (nUnicodeChar == 0x0162) *nBfr = 0xDE; else
						if (nUnicodeChar == 0x0155) *nBfr = 0xE0; else
						if (nUnicodeChar == 0x0103) *nBfr = 0xE3; else
						if (nUnicodeChar == 0x013A) *nBfr = 0xE5; else
						if (nUnicodeChar == 0x0107) *nBfr = 0xE6; else
						if (nUnicodeChar == 0x010D) *nBfr = 0xE8; else
						if (nUnicodeChar == 0x0119) *nBfr = 0xEA; else
						if (nUnicodeChar == 0x011B) *nBfr = 0xEC; else
						if (nUnicodeChar == 0x010F) *nBfr = 0xEF; else
						if (nUnicodeChar == 0x0111) *nBfr = 0xF0; else
						if (nUnicodeChar == 0x0144) *nBfr = 0xF1; else
						if (nUnicodeChar == 0x0148) *nBfr = 0xF2; else
						if (nUnicodeChar == 0x0151) *nBfr = 0xF5; else
						if (nUnicodeChar == 0x0159) *nBfr = 0xF8; else
						if (nUnicodeChar == 0x016F) *nBfr = 0xF9; else
						if (nUnicodeChar == 0x0171) *nBfr = 0xFB; else
						if (nUnicodeChar == 0x0163) *nBfr = 0xFE; else
						if (nUnicodeChar == 0x02D9) *nBfr = 0xFF; else
							return 0;
						
						return 1;
					}
					
					return 0;
				}
		};
	}
}

#endif // ACPL_I18N_CHARSETS_CHARSET_ISO_8859_2_1999_INC_CPP
