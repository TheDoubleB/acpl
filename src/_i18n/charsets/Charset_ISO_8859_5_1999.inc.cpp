#ifndef ACPL_I18N_CHARSETS_CHARSET_ISO_8859_5_1999_INC_CPP
#define ACPL_I18N_CHARSETS_CHARSET_ISO_8859_5_1999_INC_CPP

// WARNING: Do not modify this file because it was generated using i18nproc

#include "../../Charsets.h"
#include "../../_cp/Memory.inc.cpp"

namespace acpl
{
	namespace i18n
	{
		class Charset_ISO_8859_5_1999
		{
			public:
				static inline bool IsCompatible(const char *nMimeName, acpl::Charsets::ByteOrderMask nBom, acpl::Charsets::Fpc &nFpc)
				{
					if (nMimeName != NULL &&
						acpl::pi::_string_ascii_strcasecmp(nMimeName, "ISO-8859-5") != 0 &&
						acpl::pi::_string_ascii_strcasecmp(nMimeName, "ISO_8859-5") != 0 &&
						acpl::pi::_string_ascii_strcasecmp(nMimeName, "cyrillic") != 0 &&
						acpl::pi::_string_ascii_strcasecmp(nMimeName, "iso-ir-144") != 0 &&
						acpl::pi::_string_ascii_strcasecmp(nMimeName, "csISOLatinCyrillic") != 0)
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
					return "ISO-8859-5";
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
								oChar == 0xAD
							)
						{
							nUnicodeChar = static_cast<acpl::Unichar>(oChar);
							return 1;
						}
						
						// Unique mappings
						if (oChar == 0xA1) nUnicodeChar = 0x0401; else
						if (oChar == 0xA2) nUnicodeChar = 0x0402; else
						if (oChar == 0xA3) nUnicodeChar = 0x0403; else
						if (oChar == 0xA4) nUnicodeChar = 0x0404; else
						if (oChar == 0xA5) nUnicodeChar = 0x0405; else
						if (oChar == 0xA6) nUnicodeChar = 0x0406; else
						if (oChar == 0xA7) nUnicodeChar = 0x0407; else
						if (oChar == 0xA8) nUnicodeChar = 0x0408; else
						if (oChar == 0xA9) nUnicodeChar = 0x0409; else
						if (oChar == 0xAA) nUnicodeChar = 0x040A; else
						if (oChar == 0xAB) nUnicodeChar = 0x040B; else
						if (oChar == 0xAC) nUnicodeChar = 0x040C; else
						if (oChar == 0xAE) nUnicodeChar = 0x040E; else
						if (oChar == 0xAF) nUnicodeChar = 0x040F; else
						if (oChar == 0xB0) nUnicodeChar = 0x0410; else
						if (oChar == 0xB1) nUnicodeChar = 0x0411; else
						if (oChar == 0xB2) nUnicodeChar = 0x0412; else
						if (oChar == 0xB3) nUnicodeChar = 0x0413; else
						if (oChar == 0xB4) nUnicodeChar = 0x0414; else
						if (oChar == 0xB5) nUnicodeChar = 0x0415; else
						if (oChar == 0xB6) nUnicodeChar = 0x0416; else
						if (oChar == 0xB7) nUnicodeChar = 0x0417; else
						if (oChar == 0xB8) nUnicodeChar = 0x0418; else
						if (oChar == 0xB9) nUnicodeChar = 0x0419; else
						if (oChar == 0xBA) nUnicodeChar = 0x041A; else
						if (oChar == 0xBB) nUnicodeChar = 0x041B; else
						if (oChar == 0xBC) nUnicodeChar = 0x041C; else
						if (oChar == 0xBD) nUnicodeChar = 0x041D; else
						if (oChar == 0xBE) nUnicodeChar = 0x041E; else
						if (oChar == 0xBF) nUnicodeChar = 0x041F; else
						if (oChar == 0xC0) nUnicodeChar = 0x0420; else
						if (oChar == 0xC1) nUnicodeChar = 0x0421; else
						if (oChar == 0xC2) nUnicodeChar = 0x0422; else
						if (oChar == 0xC3) nUnicodeChar = 0x0423; else
						if (oChar == 0xC4) nUnicodeChar = 0x0424; else
						if (oChar == 0xC5) nUnicodeChar = 0x0425; else
						if (oChar == 0xC6) nUnicodeChar = 0x0426; else
						if (oChar == 0xC7) nUnicodeChar = 0x0427; else
						if (oChar == 0xC8) nUnicodeChar = 0x0428; else
						if (oChar == 0xC9) nUnicodeChar = 0x0429; else
						if (oChar == 0xCA) nUnicodeChar = 0x042A; else
						if (oChar == 0xCB) nUnicodeChar = 0x042B; else
						if (oChar == 0xCC) nUnicodeChar = 0x042C; else
						if (oChar == 0xCD) nUnicodeChar = 0x042D; else
						if (oChar == 0xCE) nUnicodeChar = 0x042E; else
						if (oChar == 0xCF) nUnicodeChar = 0x042F; else
						if (oChar == 0xD0) nUnicodeChar = 0x0430; else
						if (oChar == 0xD1) nUnicodeChar = 0x0431; else
						if (oChar == 0xD2) nUnicodeChar = 0x0432; else
						if (oChar == 0xD3) nUnicodeChar = 0x0433; else
						if (oChar == 0xD4) nUnicodeChar = 0x0434; else
						if (oChar == 0xD5) nUnicodeChar = 0x0435; else
						if (oChar == 0xD6) nUnicodeChar = 0x0436; else
						if (oChar == 0xD7) nUnicodeChar = 0x0437; else
						if (oChar == 0xD8) nUnicodeChar = 0x0438; else
						if (oChar == 0xD9) nUnicodeChar = 0x0439; else
						if (oChar == 0xDA) nUnicodeChar = 0x043A; else
						if (oChar == 0xDB) nUnicodeChar = 0x043B; else
						if (oChar == 0xDC) nUnicodeChar = 0x043C; else
						if (oChar == 0xDD) nUnicodeChar = 0x043D; else
						if (oChar == 0xDE) nUnicodeChar = 0x043E; else
						if (oChar == 0xDF) nUnicodeChar = 0x043F; else
						if (oChar == 0xE0) nUnicodeChar = 0x0440; else
						if (oChar == 0xE1) nUnicodeChar = 0x0441; else
						if (oChar == 0xE2) nUnicodeChar = 0x0442; else
						if (oChar == 0xE3) nUnicodeChar = 0x0443; else
						if (oChar == 0xE4) nUnicodeChar = 0x0444; else
						if (oChar == 0xE5) nUnicodeChar = 0x0445; else
						if (oChar == 0xE6) nUnicodeChar = 0x0446; else
						if (oChar == 0xE7) nUnicodeChar = 0x0447; else
						if (oChar == 0xE8) nUnicodeChar = 0x0448; else
						if (oChar == 0xE9) nUnicodeChar = 0x0449; else
						if (oChar == 0xEA) nUnicodeChar = 0x044A; else
						if (oChar == 0xEB) nUnicodeChar = 0x044B; else
						if (oChar == 0xEC) nUnicodeChar = 0x044C; else
						if (oChar == 0xED) nUnicodeChar = 0x044D; else
						if (oChar == 0xEE) nUnicodeChar = 0x044E; else
						if (oChar == 0xEF) nUnicodeChar = 0x044F; else
						if (oChar == 0xF0) nUnicodeChar = 0x2116; else
						if (oChar == 0xF1) nUnicodeChar = 0x0451; else
						if (oChar == 0xF2) nUnicodeChar = 0x0452; else
						if (oChar == 0xF3) nUnicodeChar = 0x0453; else
						if (oChar == 0xF4) nUnicodeChar = 0x0454; else
						if (oChar == 0xF5) nUnicodeChar = 0x0455; else
						if (oChar == 0xF6) nUnicodeChar = 0x0456; else
						if (oChar == 0xF7) nUnicodeChar = 0x0457; else
						if (oChar == 0xF8) nUnicodeChar = 0x0458; else
						if (oChar == 0xF9) nUnicodeChar = 0x0459; else
						if (oChar == 0xFA) nUnicodeChar = 0x045A; else
						if (oChar == 0xFB) nUnicodeChar = 0x045B; else
						if (oChar == 0xFC) nUnicodeChar = 0x045C; else
						if (oChar == 0xFD) nUnicodeChar = 0x000A7; else
						if (oChar == 0xFE) nUnicodeChar = 0x045E; else
						if (oChar == 0xFF) nUnicodeChar = 0x045F; else
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
								nUnicodeChar == 0xAD
							)
						{
							*nBfr = static_cast<acpl::UInt8>(nUnicodeChar);
							return 1;
						}
						
						// Unique mappings
						if (nUnicodeChar == 0x0401) *nBfr = 0xA1; else
						if (nUnicodeChar == 0x0402) *nBfr = 0xA2; else
						if (nUnicodeChar == 0x0403) *nBfr = 0xA3; else
						if (nUnicodeChar == 0x0404) *nBfr = 0xA4; else
						if (nUnicodeChar == 0x0405) *nBfr = 0xA5; else
						if (nUnicodeChar == 0x0406) *nBfr = 0xA6; else
						if (nUnicodeChar == 0x0407) *nBfr = 0xA7; else
						if (nUnicodeChar == 0x0408) *nBfr = 0xA8; else
						if (nUnicodeChar == 0x0409) *nBfr = 0xA9; else
						if (nUnicodeChar == 0x040A) *nBfr = 0xAA; else
						if (nUnicodeChar == 0x040B) *nBfr = 0xAB; else
						if (nUnicodeChar == 0x040C) *nBfr = 0xAC; else
						if (nUnicodeChar == 0x040E) *nBfr = 0xAE; else
						if (nUnicodeChar == 0x040F) *nBfr = 0xAF; else
						if (nUnicodeChar == 0x0410) *nBfr = 0xB0; else
						if (nUnicodeChar == 0x0411) *nBfr = 0xB1; else
						if (nUnicodeChar == 0x0412) *nBfr = 0xB2; else
						if (nUnicodeChar == 0x0413) *nBfr = 0xB3; else
						if (nUnicodeChar == 0x0414) *nBfr = 0xB4; else
						if (nUnicodeChar == 0x0415) *nBfr = 0xB5; else
						if (nUnicodeChar == 0x0416) *nBfr = 0xB6; else
						if (nUnicodeChar == 0x0417) *nBfr = 0xB7; else
						if (nUnicodeChar == 0x0418) *nBfr = 0xB8; else
						if (nUnicodeChar == 0x0419) *nBfr = 0xB9; else
						if (nUnicodeChar == 0x041A) *nBfr = 0xBA; else
						if (nUnicodeChar == 0x041B) *nBfr = 0xBB; else
						if (nUnicodeChar == 0x041C) *nBfr = 0xBC; else
						if (nUnicodeChar == 0x041D) *nBfr = 0xBD; else
						if (nUnicodeChar == 0x041E) *nBfr = 0xBE; else
						if (nUnicodeChar == 0x041F) *nBfr = 0xBF; else
						if (nUnicodeChar == 0x0420) *nBfr = 0xC0; else
						if (nUnicodeChar == 0x0421) *nBfr = 0xC1; else
						if (nUnicodeChar == 0x0422) *nBfr = 0xC2; else
						if (nUnicodeChar == 0x0423) *nBfr = 0xC3; else
						if (nUnicodeChar == 0x0424) *nBfr = 0xC4; else
						if (nUnicodeChar == 0x0425) *nBfr = 0xC5; else
						if (nUnicodeChar == 0x0426) *nBfr = 0xC6; else
						if (nUnicodeChar == 0x0427) *nBfr = 0xC7; else
						if (nUnicodeChar == 0x0428) *nBfr = 0xC8; else
						if (nUnicodeChar == 0x0429) *nBfr = 0xC9; else
						if (nUnicodeChar == 0x042A) *nBfr = 0xCA; else
						if (nUnicodeChar == 0x042B) *nBfr = 0xCB; else
						if (nUnicodeChar == 0x042C) *nBfr = 0xCC; else
						if (nUnicodeChar == 0x042D) *nBfr = 0xCD; else
						if (nUnicodeChar == 0x042E) *nBfr = 0xCE; else
						if (nUnicodeChar == 0x042F) *nBfr = 0xCF; else
						if (nUnicodeChar == 0x0430) *nBfr = 0xD0; else
						if (nUnicodeChar == 0x0431) *nBfr = 0xD1; else
						if (nUnicodeChar == 0x0432) *nBfr = 0xD2; else
						if (nUnicodeChar == 0x0433) *nBfr = 0xD3; else
						if (nUnicodeChar == 0x0434) *nBfr = 0xD4; else
						if (nUnicodeChar == 0x0435) *nBfr = 0xD5; else
						if (nUnicodeChar == 0x0436) *nBfr = 0xD6; else
						if (nUnicodeChar == 0x0437) *nBfr = 0xD7; else
						if (nUnicodeChar == 0x0438) *nBfr = 0xD8; else
						if (nUnicodeChar == 0x0439) *nBfr = 0xD9; else
						if (nUnicodeChar == 0x043A) *nBfr = 0xDA; else
						if (nUnicodeChar == 0x043B) *nBfr = 0xDB; else
						if (nUnicodeChar == 0x043C) *nBfr = 0xDC; else
						if (nUnicodeChar == 0x043D) *nBfr = 0xDD; else
						if (nUnicodeChar == 0x043E) *nBfr = 0xDE; else
						if (nUnicodeChar == 0x043F) *nBfr = 0xDF; else
						if (nUnicodeChar == 0x0440) *nBfr = 0xE0; else
						if (nUnicodeChar == 0x0441) *nBfr = 0xE1; else
						if (nUnicodeChar == 0x0442) *nBfr = 0xE2; else
						if (nUnicodeChar == 0x0443) *nBfr = 0xE3; else
						if (nUnicodeChar == 0x0444) *nBfr = 0xE4; else
						if (nUnicodeChar == 0x0445) *nBfr = 0xE5; else
						if (nUnicodeChar == 0x0446) *nBfr = 0xE6; else
						if (nUnicodeChar == 0x0447) *nBfr = 0xE7; else
						if (nUnicodeChar == 0x0448) *nBfr = 0xE8; else
						if (nUnicodeChar == 0x0449) *nBfr = 0xE9; else
						if (nUnicodeChar == 0x044A) *nBfr = 0xEA; else
						if (nUnicodeChar == 0x044B) *nBfr = 0xEB; else
						if (nUnicodeChar == 0x044C) *nBfr = 0xEC; else
						if (nUnicodeChar == 0x044D) *nBfr = 0xED; else
						if (nUnicodeChar == 0x044E) *nBfr = 0xEE; else
						if (nUnicodeChar == 0x044F) *nBfr = 0xEF; else
						if (nUnicodeChar == 0x2116) *nBfr = 0xF0; else
						if (nUnicodeChar == 0x0451) *nBfr = 0xF1; else
						if (nUnicodeChar == 0x0452) *nBfr = 0xF2; else
						if (nUnicodeChar == 0x0453) *nBfr = 0xF3; else
						if (nUnicodeChar == 0x0454) *nBfr = 0xF4; else
						if (nUnicodeChar == 0x0455) *nBfr = 0xF5; else
						if (nUnicodeChar == 0x0456) *nBfr = 0xF6; else
						if (nUnicodeChar == 0x0457) *nBfr = 0xF7; else
						if (nUnicodeChar == 0x0458) *nBfr = 0xF8; else
						if (nUnicodeChar == 0x0459) *nBfr = 0xF9; else
						if (nUnicodeChar == 0x045A) *nBfr = 0xFA; else
						if (nUnicodeChar == 0x045B) *nBfr = 0xFB; else
						if (nUnicodeChar == 0x045C) *nBfr = 0xFC; else
						if (nUnicodeChar == 0x000A7) *nBfr = 0xFD; else
						if (nUnicodeChar == 0x045E) *nBfr = 0xFE; else
						if (nUnicodeChar == 0x045F) *nBfr = 0xFF; else
							return 0;
						
						return 1;
					}
					
					return 0;
				}
		};
	}
}

#endif // ACPL_I18N_CHARSETS_CHARSET_ISO_8859_5_1999_INC_CPP
