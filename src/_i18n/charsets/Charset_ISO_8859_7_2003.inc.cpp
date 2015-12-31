#ifndef ACPL_I18N_CHARSETS_CHARSET_ISO_8859_7_2003_INC_CPP
#define ACPL_I18N_CHARSETS_CHARSET_ISO_8859_7_2003_INC_CPP

// WARNING: Do not modify this file because it was generated using i18nproc

#include "../../Charsets.h"
#include "../../_cp/Memory.inc.cpp"

namespace acpl
{
	namespace i18n
	{
		class Charset_ISO_8859_7_2003
		{
			public:
				static inline bool IsCompatible(const char *nMimeName, acpl::Charsets::ByteOrderMask nBom, acpl::Charsets::Fpc &nFpc)
				{
					if (nMimeName != NULL &&
						acpl::pi::_string_ascii_strcasecmp(nMimeName, "ISO-8859-7") != 0 &&
						acpl::pi::_string_ascii_strcasecmp(nMimeName, "ISO_8859-7") != 0 &&
						acpl::pi::_string_ascii_strcasecmp(nMimeName, "ELOT_928") != 0 &&
						acpl::pi::_string_ascii_strcasecmp(nMimeName, "ECMA-118") != 0 &&
						acpl::pi::_string_ascii_strcasecmp(nMimeName, "greek") != 0 &&
						acpl::pi::_string_ascii_strcasecmp(nMimeName, "greek8") != 0 &&
						acpl::pi::_string_ascii_strcasecmp(nMimeName, "iso-ir-126") != 0 &&
						acpl::pi::_string_ascii_strcasecmp(nMimeName, "csISOLatinGreek") != 0)
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
					return "ISO-8859-7";
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
								oChar == 0xA3 ||
								(oChar >= 0xA6 && oChar <= 0xA9) ||
								(oChar >= 0xAB && oChar <= 0xAD) ||
								(oChar >= 0xB0 && oChar <= 0xB3) ||
								oChar == 0xB7 ||
								oChar == 0xBB ||
								oChar == 0xBD
							)
						{
							nUnicodeChar = static_cast<acpl::Unichar>(oChar);
							return 1;
						}
						
						// Unique mappings
						if (oChar == 0xA1) nUnicodeChar = 0x2018; else
						if (oChar == 0xA2) nUnicodeChar = 0x2019; else
						if (oChar == 0xA4) nUnicodeChar = 0x20AC; else
						if (oChar == 0xA5) nUnicodeChar = 0x20AF; else
						if (oChar == 0xAA) nUnicodeChar = 0x037A; else
						if (oChar == 0xAF) nUnicodeChar = 0x2015; else
						if (oChar == 0xB4) nUnicodeChar = 0x0384; else
						if (oChar == 0xB5) nUnicodeChar = 0x0385; else
						if (oChar == 0xB6) nUnicodeChar = 0x0386; else
						if (oChar == 0xB8) nUnicodeChar = 0x0388; else
						if (oChar == 0xB9) nUnicodeChar = 0x0389; else
						if (oChar == 0xBA) nUnicodeChar = 0x038A; else
						if (oChar == 0xBC) nUnicodeChar = 0x038C; else
						if (oChar == 0xBE) nUnicodeChar = 0x038E; else
						if (oChar == 0xBF) nUnicodeChar = 0x038F; else
						if (oChar == 0xC0) nUnicodeChar = 0x0390; else
						if (oChar == 0xC1) nUnicodeChar = 0x0391; else
						if (oChar == 0xC2) nUnicodeChar = 0x0392; else
						if (oChar == 0xC3) nUnicodeChar = 0x0393; else
						if (oChar == 0xC4) nUnicodeChar = 0x0394; else
						if (oChar == 0xC5) nUnicodeChar = 0x0395; else
						if (oChar == 0xC6) nUnicodeChar = 0x0396; else
						if (oChar == 0xC7) nUnicodeChar = 0x0397; else
						if (oChar == 0xC8) nUnicodeChar = 0x0398; else
						if (oChar == 0xC9) nUnicodeChar = 0x0399; else
						if (oChar == 0xCA) nUnicodeChar = 0x039A; else
						if (oChar == 0xCB) nUnicodeChar = 0x039B; else
						if (oChar == 0xCC) nUnicodeChar = 0x039C; else
						if (oChar == 0xCD) nUnicodeChar = 0x039D; else
						if (oChar == 0xCE) nUnicodeChar = 0x039E; else
						if (oChar == 0xCF) nUnicodeChar = 0x039F; else
						if (oChar == 0xD0) nUnicodeChar = 0x03A0; else
						if (oChar == 0xD1) nUnicodeChar = 0x03A1; else
						if (oChar == 0xD3) nUnicodeChar = 0x03A3; else
						if (oChar == 0xD4) nUnicodeChar = 0x03A4; else
						if (oChar == 0xD5) nUnicodeChar = 0x03A5; else
						if (oChar == 0xD6) nUnicodeChar = 0x03A6; else
						if (oChar == 0xD7) nUnicodeChar = 0x03A7; else
						if (oChar == 0xD8) nUnicodeChar = 0x03A8; else
						if (oChar == 0xD9) nUnicodeChar = 0x03A9; else
						if (oChar == 0xDA) nUnicodeChar = 0x03AA; else
						if (oChar == 0xDB) nUnicodeChar = 0x03AB; else
						if (oChar == 0xDC) nUnicodeChar = 0x03AC; else
						if (oChar == 0xDD) nUnicodeChar = 0x03AD; else
						if (oChar == 0xDE) nUnicodeChar = 0x03AE; else
						if (oChar == 0xDF) nUnicodeChar = 0x03AF; else
						if (oChar == 0xE0) nUnicodeChar = 0x03B0; else
						if (oChar == 0xE1) nUnicodeChar = 0x03B1; else
						if (oChar == 0xE2) nUnicodeChar = 0x03B2; else
						if (oChar == 0xE3) nUnicodeChar = 0x03B3; else
						if (oChar == 0xE4) nUnicodeChar = 0x03B4; else
						if (oChar == 0xE5) nUnicodeChar = 0x03B5; else
						if (oChar == 0xE6) nUnicodeChar = 0x03B6; else
						if (oChar == 0xE7) nUnicodeChar = 0x03B7; else
						if (oChar == 0xE8) nUnicodeChar = 0x03B8; else
						if (oChar == 0xE9) nUnicodeChar = 0x03B9; else
						if (oChar == 0xEA) nUnicodeChar = 0x03BA; else
						if (oChar == 0xEB) nUnicodeChar = 0x03BB; else
						if (oChar == 0xEC) nUnicodeChar = 0x03BC; else
						if (oChar == 0xED) nUnicodeChar = 0x03BD; else
						if (oChar == 0xEE) nUnicodeChar = 0x03BE; else
						if (oChar == 0xEF) nUnicodeChar = 0x03BF; else
						if (oChar == 0xF0) nUnicodeChar = 0x03C0; else
						if (oChar == 0xF1) nUnicodeChar = 0x03C1; else
						if (oChar == 0xF2) nUnicodeChar = 0x03C2; else
						if (oChar == 0xF3) nUnicodeChar = 0x03C3; else
						if (oChar == 0xF4) nUnicodeChar = 0x03C4; else
						if (oChar == 0xF5) nUnicodeChar = 0x03C5; else
						if (oChar == 0xF6) nUnicodeChar = 0x03C6; else
						if (oChar == 0xF7) nUnicodeChar = 0x03C7; else
						if (oChar == 0xF8) nUnicodeChar = 0x03C8; else
						if (oChar == 0xF9) nUnicodeChar = 0x03C9; else
						if (oChar == 0xFA) nUnicodeChar = 0x03CA; else
						if (oChar == 0xFB) nUnicodeChar = 0x03CB; else
						if (oChar == 0xFC) nUnicodeChar = 0x03CC; else
						if (oChar == 0xFD) nUnicodeChar = 0x03CD; else
						if (oChar == 0xFE) nUnicodeChar = 0x03CE; else
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
								nUnicodeChar == 0xA3 ||
								(nUnicodeChar >= 0xA6 && nUnicodeChar <= 0xA9) ||
								(nUnicodeChar >= 0xAB && nUnicodeChar <= 0xAD) ||
								(nUnicodeChar >= 0xB0 && nUnicodeChar <= 0xB3) ||
								nUnicodeChar == 0xB7 ||
								nUnicodeChar == 0xBB ||
								nUnicodeChar == 0xBD
							)
						{
							*nBfr = static_cast<acpl::UInt8>(nUnicodeChar);
							return 1;
						}
						
						// Unique mappings
						if (nUnicodeChar == 0x2018) *nBfr = 0xA1; else
						if (nUnicodeChar == 0x2019) *nBfr = 0xA2; else
						if (nUnicodeChar == 0x20AC) *nBfr = 0xA4; else
						if (nUnicodeChar == 0x20AF) *nBfr = 0xA5; else
						if (nUnicodeChar == 0x037A) *nBfr = 0xAA; else
						if (nUnicodeChar == 0x2015) *nBfr = 0xAF; else
						if (nUnicodeChar == 0x0384) *nBfr = 0xB4; else
						if (nUnicodeChar == 0x0385) *nBfr = 0xB5; else
						if (nUnicodeChar == 0x0386) *nBfr = 0xB6; else
						if (nUnicodeChar == 0x0388) *nBfr = 0xB8; else
						if (nUnicodeChar == 0x0389) *nBfr = 0xB9; else
						if (nUnicodeChar == 0x038A) *nBfr = 0xBA; else
						if (nUnicodeChar == 0x038C) *nBfr = 0xBC; else
						if (nUnicodeChar == 0x038E) *nBfr = 0xBE; else
						if (nUnicodeChar == 0x038F) *nBfr = 0xBF; else
						if (nUnicodeChar == 0x0390) *nBfr = 0xC0; else
						if (nUnicodeChar == 0x0391) *nBfr = 0xC1; else
						if (nUnicodeChar == 0x0392) *nBfr = 0xC2; else
						if (nUnicodeChar == 0x0393) *nBfr = 0xC3; else
						if (nUnicodeChar == 0x0394) *nBfr = 0xC4; else
						if (nUnicodeChar == 0x0395) *nBfr = 0xC5; else
						if (nUnicodeChar == 0x0396) *nBfr = 0xC6; else
						if (nUnicodeChar == 0x0397) *nBfr = 0xC7; else
						if (nUnicodeChar == 0x0398) *nBfr = 0xC8; else
						if (nUnicodeChar == 0x0399) *nBfr = 0xC9; else
						if (nUnicodeChar == 0x039A) *nBfr = 0xCA; else
						if (nUnicodeChar == 0x039B) *nBfr = 0xCB; else
						if (nUnicodeChar == 0x039C) *nBfr = 0xCC; else
						if (nUnicodeChar == 0x039D) *nBfr = 0xCD; else
						if (nUnicodeChar == 0x039E) *nBfr = 0xCE; else
						if (nUnicodeChar == 0x039F) *nBfr = 0xCF; else
						if (nUnicodeChar == 0x03A0) *nBfr = 0xD0; else
						if (nUnicodeChar == 0x03A1) *nBfr = 0xD1; else
						if (nUnicodeChar == 0x03A3) *nBfr = 0xD3; else
						if (nUnicodeChar == 0x03A4) *nBfr = 0xD4; else
						if (nUnicodeChar == 0x03A5) *nBfr = 0xD5; else
						if (nUnicodeChar == 0x03A6) *nBfr = 0xD6; else
						if (nUnicodeChar == 0x03A7) *nBfr = 0xD7; else
						if (nUnicodeChar == 0x03A8) *nBfr = 0xD8; else
						if (nUnicodeChar == 0x03A9) *nBfr = 0xD9; else
						if (nUnicodeChar == 0x03AA) *nBfr = 0xDA; else
						if (nUnicodeChar == 0x03AB) *nBfr = 0xDB; else
						if (nUnicodeChar == 0x03AC) *nBfr = 0xDC; else
						if (nUnicodeChar == 0x03AD) *nBfr = 0xDD; else
						if (nUnicodeChar == 0x03AE) *nBfr = 0xDE; else
						if (nUnicodeChar == 0x03AF) *nBfr = 0xDF; else
						if (nUnicodeChar == 0x03B0) *nBfr = 0xE0; else
						if (nUnicodeChar == 0x03B1) *nBfr = 0xE1; else
						if (nUnicodeChar == 0x03B2) *nBfr = 0xE2; else
						if (nUnicodeChar == 0x03B3) *nBfr = 0xE3; else
						if (nUnicodeChar == 0x03B4) *nBfr = 0xE4; else
						if (nUnicodeChar == 0x03B5) *nBfr = 0xE5; else
						if (nUnicodeChar == 0x03B6) *nBfr = 0xE6; else
						if (nUnicodeChar == 0x03B7) *nBfr = 0xE7; else
						if (nUnicodeChar == 0x03B8) *nBfr = 0xE8; else
						if (nUnicodeChar == 0x03B9) *nBfr = 0xE9; else
						if (nUnicodeChar == 0x03BA) *nBfr = 0xEA; else
						if (nUnicodeChar == 0x03BB) *nBfr = 0xEB; else
						if (nUnicodeChar == 0x03BC) *nBfr = 0xEC; else
						if (nUnicodeChar == 0x03BD) *nBfr = 0xED; else
						if (nUnicodeChar == 0x03BE) *nBfr = 0xEE; else
						if (nUnicodeChar == 0x03BF) *nBfr = 0xEF; else
						if (nUnicodeChar == 0x03C0) *nBfr = 0xF0; else
						if (nUnicodeChar == 0x03C1) *nBfr = 0xF1; else
						if (nUnicodeChar == 0x03C2) *nBfr = 0xF2; else
						if (nUnicodeChar == 0x03C3) *nBfr = 0xF3; else
						if (nUnicodeChar == 0x03C4) *nBfr = 0xF4; else
						if (nUnicodeChar == 0x03C5) *nBfr = 0xF5; else
						if (nUnicodeChar == 0x03C6) *nBfr = 0xF6; else
						if (nUnicodeChar == 0x03C7) *nBfr = 0xF7; else
						if (nUnicodeChar == 0x03C8) *nBfr = 0xF8; else
						if (nUnicodeChar == 0x03C9) *nBfr = 0xF9; else
						if (nUnicodeChar == 0x03CA) *nBfr = 0xFA; else
						if (nUnicodeChar == 0x03CB) *nBfr = 0xFB; else
						if (nUnicodeChar == 0x03CC) *nBfr = 0xFC; else
						if (nUnicodeChar == 0x03CD) *nBfr = 0xFD; else
						if (nUnicodeChar == 0x03CE) *nBfr = 0xFE; else
							return 0;
						
						return 1;
					}
					
					return 0;
				}
		};
	}
}

#endif // ACPL_I18N_CHARSETS_CHARSET_ISO_8859_7_2003_INC_CPP
