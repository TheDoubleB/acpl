#ifndef ACPL_I18N_CHARSETS_CHARSET_ISO_8859_11_2001_INC_CPP
#define ACPL_I18N_CHARSETS_CHARSET_ISO_8859_11_2001_INC_CPP

// WARNING: Do not modify this file because it was generated using i18nproc

#include "../../Charsets.h"
#include "../../_cp/Memory.inc.cpp"

namespace acpl
{
	namespace i18n
	{
		class Charset_ISO_8859_11_2001
		{
			public:
				static inline bool IsCompatible(const char *nMimeName, acpl::Charsets::ByteOrderMask nBom, acpl::Charsets::Fpc &nFpc)
				{
					if (nMimeName != NULL &&
						acpl::pi::_string_ascii_strcasecmp(nMimeName, "ISO-8859-11") != 0 &&
						acpl::pi::_string_ascii_strcasecmp(nMimeName, "ISO_8859-11") != 0)
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
					return "ISO-8859-11";
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
								oChar <= 0xA0
							)
						{
							nUnicodeChar = static_cast<acpl::Unichar>(oChar);
							return 1;
						}
						
						// Unique mappings
						if (oChar == 0xA1) nUnicodeChar = 0x0E01; else
						if (oChar == 0xA2) nUnicodeChar = 0x0E02; else
						if (oChar == 0xA3) nUnicodeChar = 0x0E03; else
						if (oChar == 0xA4) nUnicodeChar = 0x0E04; else
						if (oChar == 0xA5) nUnicodeChar = 0x0E05; else
						if (oChar == 0xA6) nUnicodeChar = 0x0E06; else
						if (oChar == 0xA7) nUnicodeChar = 0x0E07; else
						if (oChar == 0xA8) nUnicodeChar = 0x0E08; else
						if (oChar == 0xA9) nUnicodeChar = 0x0E09; else
						if (oChar == 0xAA) nUnicodeChar = 0x0E0A; else
						if (oChar == 0xAB) nUnicodeChar = 0x0E0B; else
						if (oChar == 0xAC) nUnicodeChar = 0x0E0C; else
						if (oChar == 0xAD) nUnicodeChar = 0x0E0D; else
						if (oChar == 0xAE) nUnicodeChar = 0x0E0E; else
						if (oChar == 0xAF) nUnicodeChar = 0x0E0F; else
						if (oChar == 0xB0) nUnicodeChar = 0x0E10; else
						if (oChar == 0xB1) nUnicodeChar = 0x0E11; else
						if (oChar == 0xB2) nUnicodeChar = 0x0E12; else
						if (oChar == 0xB3) nUnicodeChar = 0x0E13; else
						if (oChar == 0xB4) nUnicodeChar = 0x0E14; else
						if (oChar == 0xB5) nUnicodeChar = 0x0E15; else
						if (oChar == 0xB6) nUnicodeChar = 0x0E16; else
						if (oChar == 0xB7) nUnicodeChar = 0x0E17; else
						if (oChar == 0xB8) nUnicodeChar = 0x0E18; else
						if (oChar == 0xB9) nUnicodeChar = 0x0E19; else
						if (oChar == 0xBA) nUnicodeChar = 0x0E1A; else
						if (oChar == 0xBB) nUnicodeChar = 0x0E1B; else
						if (oChar == 0xBC) nUnicodeChar = 0x0E1C; else
						if (oChar == 0xBD) nUnicodeChar = 0x0E1D; else
						if (oChar == 0xBE) nUnicodeChar = 0x0E1E; else
						if (oChar == 0xBF) nUnicodeChar = 0x0E1F; else
						if (oChar == 0xC0) nUnicodeChar = 0x0E20; else
						if (oChar == 0xC1) nUnicodeChar = 0x0E21; else
						if (oChar == 0xC2) nUnicodeChar = 0x0E22; else
						if (oChar == 0xC3) nUnicodeChar = 0x0E23; else
						if (oChar == 0xC4) nUnicodeChar = 0x0E24; else
						if (oChar == 0xC5) nUnicodeChar = 0x0E25; else
						if (oChar == 0xC6) nUnicodeChar = 0x0E26; else
						if (oChar == 0xC7) nUnicodeChar = 0x0E27; else
						if (oChar == 0xC8) nUnicodeChar = 0x0E28; else
						if (oChar == 0xC9) nUnicodeChar = 0x0E29; else
						if (oChar == 0xCA) nUnicodeChar = 0x0E2A; else
						if (oChar == 0xCB) nUnicodeChar = 0x0E2B; else
						if (oChar == 0xCC) nUnicodeChar = 0x0E2C; else
						if (oChar == 0xCD) nUnicodeChar = 0x0E2D; else
						if (oChar == 0xCE) nUnicodeChar = 0x0E2E; else
						if (oChar == 0xCF) nUnicodeChar = 0x0E2F; else
						if (oChar == 0xD0) nUnicodeChar = 0x0E30; else
						if (oChar == 0xD1) nUnicodeChar = 0x0E31; else
						if (oChar == 0xD2) nUnicodeChar = 0x0E32; else
						if (oChar == 0xD3) nUnicodeChar = 0x0E33; else
						if (oChar == 0xD4) nUnicodeChar = 0x0E34; else
						if (oChar == 0xD5) nUnicodeChar = 0x0E35; else
						if (oChar == 0xD6) nUnicodeChar = 0x0E36; else
						if (oChar == 0xD7) nUnicodeChar = 0x0E37; else
						if (oChar == 0xD8) nUnicodeChar = 0x0E38; else
						if (oChar == 0xD9) nUnicodeChar = 0x0E39; else
						if (oChar == 0xDA) nUnicodeChar = 0x0E3A; else
						if (oChar == 0xDF) nUnicodeChar = 0x0E3F; else
						if (oChar == 0xE0) nUnicodeChar = 0x0E40; else
						if (oChar == 0xE1) nUnicodeChar = 0x0E41; else
						if (oChar == 0xE2) nUnicodeChar = 0x0E42; else
						if (oChar == 0xE3) nUnicodeChar = 0x0E43; else
						if (oChar == 0xE4) nUnicodeChar = 0x0E44; else
						if (oChar == 0xE5) nUnicodeChar = 0x0E45; else
						if (oChar == 0xE6) nUnicodeChar = 0x0E46; else
						if (oChar == 0xE7) nUnicodeChar = 0x0E47; else
						if (oChar == 0xE8) nUnicodeChar = 0x0E48; else
						if (oChar == 0xE9) nUnicodeChar = 0x0E49; else
						if (oChar == 0xEA) nUnicodeChar = 0x0E4A; else
						if (oChar == 0xEB) nUnicodeChar = 0x0E4B; else
						if (oChar == 0xEC) nUnicodeChar = 0x0E4C; else
						if (oChar == 0xED) nUnicodeChar = 0x0E4D; else
						if (oChar == 0xEE) nUnicodeChar = 0x0E4E; else
						if (oChar == 0xEF) nUnicodeChar = 0x0E4F; else
						if (oChar == 0xF0) nUnicodeChar = 0x0E50; else
						if (oChar == 0xF1) nUnicodeChar = 0x0E51; else
						if (oChar == 0xF2) nUnicodeChar = 0x0E52; else
						if (oChar == 0xF3) nUnicodeChar = 0x0E53; else
						if (oChar == 0xF4) nUnicodeChar = 0x0E54; else
						if (oChar == 0xF5) nUnicodeChar = 0x0E55; else
						if (oChar == 0xF6) nUnicodeChar = 0x0E56; else
						if (oChar == 0xF7) nUnicodeChar = 0x0E57; else
						if (oChar == 0xF8) nUnicodeChar = 0x0E58; else
						if (oChar == 0xF9) nUnicodeChar = 0x0E59; else
						if (oChar == 0xFA) nUnicodeChar = 0x0E5A; else
						if (oChar == 0xFB) nUnicodeChar = 0x0E5B; else
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
								nUnicodeChar <= 0xA0
							)
						{
							*nBfr = static_cast<acpl::UInt8>(nUnicodeChar);
							return 1;
						}
						
						// Unique mappings
						if (nUnicodeChar == 0x0E01) *nBfr = 0xA1; else
						if (nUnicodeChar == 0x0E02) *nBfr = 0xA2; else
						if (nUnicodeChar == 0x0E03) *nBfr = 0xA3; else
						if (nUnicodeChar == 0x0E04) *nBfr = 0xA4; else
						if (nUnicodeChar == 0x0E05) *nBfr = 0xA5; else
						if (nUnicodeChar == 0x0E06) *nBfr = 0xA6; else
						if (nUnicodeChar == 0x0E07) *nBfr = 0xA7; else
						if (nUnicodeChar == 0x0E08) *nBfr = 0xA8; else
						if (nUnicodeChar == 0x0E09) *nBfr = 0xA9; else
						if (nUnicodeChar == 0x0E0A) *nBfr = 0xAA; else
						if (nUnicodeChar == 0x0E0B) *nBfr = 0xAB; else
						if (nUnicodeChar == 0x0E0C) *nBfr = 0xAC; else
						if (nUnicodeChar == 0x0E0D) *nBfr = 0xAD; else
						if (nUnicodeChar == 0x0E0E) *nBfr = 0xAE; else
						if (nUnicodeChar == 0x0E0F) *nBfr = 0xAF; else
						if (nUnicodeChar == 0x0E10) *nBfr = 0xB0; else
						if (nUnicodeChar == 0x0E11) *nBfr = 0xB1; else
						if (nUnicodeChar == 0x0E12) *nBfr = 0xB2; else
						if (nUnicodeChar == 0x0E13) *nBfr = 0xB3; else
						if (nUnicodeChar == 0x0E14) *nBfr = 0xB4; else
						if (nUnicodeChar == 0x0E15) *nBfr = 0xB5; else
						if (nUnicodeChar == 0x0E16) *nBfr = 0xB6; else
						if (nUnicodeChar == 0x0E17) *nBfr = 0xB7; else
						if (nUnicodeChar == 0x0E18) *nBfr = 0xB8; else
						if (nUnicodeChar == 0x0E19) *nBfr = 0xB9; else
						if (nUnicodeChar == 0x0E1A) *nBfr = 0xBA; else
						if (nUnicodeChar == 0x0E1B) *nBfr = 0xBB; else
						if (nUnicodeChar == 0x0E1C) *nBfr = 0xBC; else
						if (nUnicodeChar == 0x0E1D) *nBfr = 0xBD; else
						if (nUnicodeChar == 0x0E1E) *nBfr = 0xBE; else
						if (nUnicodeChar == 0x0E1F) *nBfr = 0xBF; else
						if (nUnicodeChar == 0x0E20) *nBfr = 0xC0; else
						if (nUnicodeChar == 0x0E21) *nBfr = 0xC1; else
						if (nUnicodeChar == 0x0E22) *nBfr = 0xC2; else
						if (nUnicodeChar == 0x0E23) *nBfr = 0xC3; else
						if (nUnicodeChar == 0x0E24) *nBfr = 0xC4; else
						if (nUnicodeChar == 0x0E25) *nBfr = 0xC5; else
						if (nUnicodeChar == 0x0E26) *nBfr = 0xC6; else
						if (nUnicodeChar == 0x0E27) *nBfr = 0xC7; else
						if (nUnicodeChar == 0x0E28) *nBfr = 0xC8; else
						if (nUnicodeChar == 0x0E29) *nBfr = 0xC9; else
						if (nUnicodeChar == 0x0E2A) *nBfr = 0xCA; else
						if (nUnicodeChar == 0x0E2B) *nBfr = 0xCB; else
						if (nUnicodeChar == 0x0E2C) *nBfr = 0xCC; else
						if (nUnicodeChar == 0x0E2D) *nBfr = 0xCD; else
						if (nUnicodeChar == 0x0E2E) *nBfr = 0xCE; else
						if (nUnicodeChar == 0x0E2F) *nBfr = 0xCF; else
						if (nUnicodeChar == 0x0E30) *nBfr = 0xD0; else
						if (nUnicodeChar == 0x0E31) *nBfr = 0xD1; else
						if (nUnicodeChar == 0x0E32) *nBfr = 0xD2; else
						if (nUnicodeChar == 0x0E33) *nBfr = 0xD3; else
						if (nUnicodeChar == 0x0E34) *nBfr = 0xD4; else
						if (nUnicodeChar == 0x0E35) *nBfr = 0xD5; else
						if (nUnicodeChar == 0x0E36) *nBfr = 0xD6; else
						if (nUnicodeChar == 0x0E37) *nBfr = 0xD7; else
						if (nUnicodeChar == 0x0E38) *nBfr = 0xD8; else
						if (nUnicodeChar == 0x0E39) *nBfr = 0xD9; else
						if (nUnicodeChar == 0x0E3A) *nBfr = 0xDA; else
						if (nUnicodeChar == 0x0E3F) *nBfr = 0xDF; else
						if (nUnicodeChar == 0x0E40) *nBfr = 0xE0; else
						if (nUnicodeChar == 0x0E41) *nBfr = 0xE1; else
						if (nUnicodeChar == 0x0E42) *nBfr = 0xE2; else
						if (nUnicodeChar == 0x0E43) *nBfr = 0xE3; else
						if (nUnicodeChar == 0x0E44) *nBfr = 0xE4; else
						if (nUnicodeChar == 0x0E45) *nBfr = 0xE5; else
						if (nUnicodeChar == 0x0E46) *nBfr = 0xE6; else
						if (nUnicodeChar == 0x0E47) *nBfr = 0xE7; else
						if (nUnicodeChar == 0x0E48) *nBfr = 0xE8; else
						if (nUnicodeChar == 0x0E49) *nBfr = 0xE9; else
						if (nUnicodeChar == 0x0E4A) *nBfr = 0xEA; else
						if (nUnicodeChar == 0x0E4B) *nBfr = 0xEB; else
						if (nUnicodeChar == 0x0E4C) *nBfr = 0xEC; else
						if (nUnicodeChar == 0x0E4D) *nBfr = 0xED; else
						if (nUnicodeChar == 0x0E4E) *nBfr = 0xEE; else
						if (nUnicodeChar == 0x0E4F) *nBfr = 0xEF; else
						if (nUnicodeChar == 0x0E50) *nBfr = 0xF0; else
						if (nUnicodeChar == 0x0E51) *nBfr = 0xF1; else
						if (nUnicodeChar == 0x0E52) *nBfr = 0xF2; else
						if (nUnicodeChar == 0x0E53) *nBfr = 0xF3; else
						if (nUnicodeChar == 0x0E54) *nBfr = 0xF4; else
						if (nUnicodeChar == 0x0E55) *nBfr = 0xF5; else
						if (nUnicodeChar == 0x0E56) *nBfr = 0xF6; else
						if (nUnicodeChar == 0x0E57) *nBfr = 0xF7; else
						if (nUnicodeChar == 0x0E58) *nBfr = 0xF8; else
						if (nUnicodeChar == 0x0E59) *nBfr = 0xF9; else
						if (nUnicodeChar == 0x0E5A) *nBfr = 0xFA; else
						if (nUnicodeChar == 0x0E5B) *nBfr = 0xFB; else
							return 0;
						
						return 1;
					}
					
					return 0;
				}
		};
	}
}

#endif // ACPL_I18N_CHARSETS_CHARSET_ISO_8859_11_2001_INC_CPP
