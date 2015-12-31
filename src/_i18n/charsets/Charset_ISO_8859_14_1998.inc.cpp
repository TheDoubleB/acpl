#ifndef ACPL_I18N_CHARSETS_CHARSET_ISO_8859_14_1998_INC_CPP
#define ACPL_I18N_CHARSETS_CHARSET_ISO_8859_14_1998_INC_CPP

// WARNING: Do not modify this file because it was generated using i18nproc

#include "../../Charsets.h"
#include "../../_cp/Memory.inc.cpp"

namespace acpl
{
	namespace i18n
	{
		class Charset_ISO_8859_14_1998
		{
			public:
				static inline bool IsCompatible(const char *nMimeName, acpl::Charsets::ByteOrderMask nBom, acpl::Charsets::Fpc &nFpc)
				{
					if (nMimeName != NULL &&
						acpl::pi::_string_ascii_strcasecmp(nMimeName, "ISO-8859-14") != 0 &&
						acpl::pi::_string_ascii_strcasecmp(nMimeName, "ISO_8859-14") != 0 &&
						acpl::pi::_string_ascii_strcasecmp(nMimeName, "latin8") != 0 &&
						acpl::pi::_string_ascii_strcasecmp(nMimeName, "l8") != 0 &&
						acpl::pi::_string_ascii_strcasecmp(nMimeName, "iso-ir-199") != 0 &&
						acpl::pi::_string_ascii_strcasecmp(nMimeName, "iso-celtic") != 0)
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
					return "ISO-8859-14";
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
								oChar == 0xA7 ||
								oChar == 0xA9 ||
								(oChar >= 0xAD && oChar <= 0xAE) ||
								oChar == 0xB6 ||
								(oChar >= 0xC0 && oChar <= 0xCF) ||
								(oChar >= 0xD1 && oChar <= 0xD6) ||
								(oChar >= 0xD8 && oChar <= 0xDD) ||
								(oChar >= 0xDF && oChar <= 0xEF) ||
								(oChar >= 0xF1 && oChar <= 0xF6) ||
								(oChar >= 0xF8 && oChar <= 0xFD) ||
								oChar == 0xFF
							)
						{
							nUnicodeChar = static_cast<acpl::Unichar>(oChar);
							return 1;
						}
						
						// Unique mappings
						if (oChar == 0xA1) nUnicodeChar = 0x1E02; else
						if (oChar == 0xA2) nUnicodeChar = 0x1E03; else
						if (oChar == 0xA4) nUnicodeChar = 0x010A; else
						if (oChar == 0xA5) nUnicodeChar = 0x010B; else
						if (oChar == 0xA6) nUnicodeChar = 0x1E0A; else
						if (oChar == 0xA8) nUnicodeChar = 0x1E80; else
						if (oChar == 0xAA) nUnicodeChar = 0x1E82; else
						if (oChar == 0xAB) nUnicodeChar = 0x1E0B; else
						if (oChar == 0xAC) nUnicodeChar = 0x1EF2; else
						if (oChar == 0xAF) nUnicodeChar = 0x0178; else
						if (oChar == 0xB0) nUnicodeChar = 0x1E1E; else
						if (oChar == 0xB1) nUnicodeChar = 0x1E1F; else
						if (oChar == 0xB2) nUnicodeChar = 0x0120; else
						if (oChar == 0xB3) nUnicodeChar = 0x0121; else
						if (oChar == 0xB4) nUnicodeChar = 0x1E40; else
						if (oChar == 0xB5) nUnicodeChar = 0x1E41; else
						if (oChar == 0xB7) nUnicodeChar = 0x1E56; else
						if (oChar == 0xB8) nUnicodeChar = 0x1E81; else
						if (oChar == 0xB9) nUnicodeChar = 0x1E57; else
						if (oChar == 0xBA) nUnicodeChar = 0x1E83; else
						if (oChar == 0xBB) nUnicodeChar = 0x1E60; else
						if (oChar == 0xBC) nUnicodeChar = 0x1EF3; else
						if (oChar == 0xBD) nUnicodeChar = 0x1E84; else
						if (oChar == 0xBE) nUnicodeChar = 0x1E85; else
						if (oChar == 0xBF) nUnicodeChar = 0x1E61; else
						if (oChar == 0xD0) nUnicodeChar = 0x0174; else
						if (oChar == 0xD7) nUnicodeChar = 0x1E6A; else
						if (oChar == 0xDE) nUnicodeChar = 0x0176; else
						if (oChar == 0xF0) nUnicodeChar = 0x0175; else
						if (oChar == 0xF7) nUnicodeChar = 0x1E6B; else
						if (oChar == 0xFE) nUnicodeChar = 0x0177; else
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
								nUnicodeChar == 0xA7 ||
								nUnicodeChar == 0xA9 ||
								(nUnicodeChar >= 0xAD && nUnicodeChar <= 0xAE) ||
								nUnicodeChar == 0xB6 ||
								(nUnicodeChar >= 0xC0 && nUnicodeChar <= 0xCF) ||
								(nUnicodeChar >= 0xD1 && nUnicodeChar <= 0xD6) ||
								(nUnicodeChar >= 0xD8 && nUnicodeChar <= 0xDD) ||
								(nUnicodeChar >= 0xDF && nUnicodeChar <= 0xEF) ||
								(nUnicodeChar >= 0xF1 && nUnicodeChar <= 0xF6) ||
								(nUnicodeChar >= 0xF8 && nUnicodeChar <= 0xFD) ||
								nUnicodeChar == 0xFF
							)
						{
							*nBfr = static_cast<acpl::UInt8>(nUnicodeChar);
							return 1;
						}
						
						// Unique mappings
						if (nUnicodeChar == 0x1E02) *nBfr = 0xA1; else
						if (nUnicodeChar == 0x1E03) *nBfr = 0xA2; else
						if (nUnicodeChar == 0x010A) *nBfr = 0xA4; else
						if (nUnicodeChar == 0x010B) *nBfr = 0xA5; else
						if (nUnicodeChar == 0x1E0A) *nBfr = 0xA6; else
						if (nUnicodeChar == 0x1E80) *nBfr = 0xA8; else
						if (nUnicodeChar == 0x1E82) *nBfr = 0xAA; else
						if (nUnicodeChar == 0x1E0B) *nBfr = 0xAB; else
						if (nUnicodeChar == 0x1EF2) *nBfr = 0xAC; else
						if (nUnicodeChar == 0x0178) *nBfr = 0xAF; else
						if (nUnicodeChar == 0x1E1E) *nBfr = 0xB0; else
						if (nUnicodeChar == 0x1E1F) *nBfr = 0xB1; else
						if (nUnicodeChar == 0x0120) *nBfr = 0xB2; else
						if (nUnicodeChar == 0x0121) *nBfr = 0xB3; else
						if (nUnicodeChar == 0x1E40) *nBfr = 0xB4; else
						if (nUnicodeChar == 0x1E41) *nBfr = 0xB5; else
						if (nUnicodeChar == 0x1E56) *nBfr = 0xB7; else
						if (nUnicodeChar == 0x1E81) *nBfr = 0xB8; else
						if (nUnicodeChar == 0x1E57) *nBfr = 0xB9; else
						if (nUnicodeChar == 0x1E83) *nBfr = 0xBA; else
						if (nUnicodeChar == 0x1E60) *nBfr = 0xBB; else
						if (nUnicodeChar == 0x1EF3) *nBfr = 0xBC; else
						if (nUnicodeChar == 0x1E84) *nBfr = 0xBD; else
						if (nUnicodeChar == 0x1E85) *nBfr = 0xBE; else
						if (nUnicodeChar == 0x1E61) *nBfr = 0xBF; else
						if (nUnicodeChar == 0x0174) *nBfr = 0xD0; else
						if (nUnicodeChar == 0x1E6A) *nBfr = 0xD7; else
						if (nUnicodeChar == 0x0176) *nBfr = 0xDE; else
						if (nUnicodeChar == 0x0175) *nBfr = 0xF0; else
						if (nUnicodeChar == 0x1E6B) *nBfr = 0xF7; else
						if (nUnicodeChar == 0x0177) *nBfr = 0xFE; else
							return 0;
						
						return 1;
					}
					
					return 0;
				}
		};
	}
}

#endif // ACPL_I18N_CHARSETS_CHARSET_ISO_8859_14_1998_INC_CPP
