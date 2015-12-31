#ifndef ACPL_I18N_CHARSETS_CHARSET_ISO_8859_6_1999_INC_CPP
#define ACPL_I18N_CHARSETS_CHARSET_ISO_8859_6_1999_INC_CPP

// WARNING: Do not modify this file because it was generated using i18nproc

#include "../../Charsets.h"
#include "../../_cp/Memory.inc.cpp"

namespace acpl
{
	namespace i18n
	{
		class Charset_ISO_8859_6_1999
		{
			public:
				static inline bool IsCompatible(const char *nMimeName, acpl::Charsets::ByteOrderMask nBom, acpl::Charsets::Fpc &nFpc)
				{
					if (nMimeName != NULL &&
						acpl::pi::_string_ascii_strcasecmp(nMimeName, "ISO-8859-6") != 0 &&
						acpl::pi::_string_ascii_strcasecmp(nMimeName, "ISO_8859-6") != 0 &&
						acpl::pi::_string_ascii_strcasecmp(nMimeName, "ECMA-114") != 0 &&
						acpl::pi::_string_ascii_strcasecmp(nMimeName, "ASMO-708") != 0 &&
						acpl::pi::_string_ascii_strcasecmp(nMimeName, "arabic") != 0 &&
						acpl::pi::_string_ascii_strcasecmp(nMimeName, "iso-ir-127") != 0 &&
						acpl::pi::_string_ascii_strcasecmp(nMimeName, "csISOLatinArabic") != 0)
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
					return "ISO-8859-6";
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
								oChar == 0xAD
							)
						{
							nUnicodeChar = static_cast<acpl::Unichar>(oChar);
							return 1;
						}
						
						// Unique mappings
						if (oChar == 0xAC) nUnicodeChar = 0x060C; else
						if (oChar == 0xBB) nUnicodeChar = 0x061B; else
						if (oChar == 0xBF) nUnicodeChar = 0x061F; else
						if (oChar == 0xC1) nUnicodeChar = 0x0621; else
						if (oChar == 0xC2) nUnicodeChar = 0x0622; else
						if (oChar == 0xC3) nUnicodeChar = 0x0623; else
						if (oChar == 0xC4) nUnicodeChar = 0x0624; else
						if (oChar == 0xC5) nUnicodeChar = 0x0625; else
						if (oChar == 0xC6) nUnicodeChar = 0x0626; else
						if (oChar == 0xC7) nUnicodeChar = 0x0627; else
						if (oChar == 0xC8) nUnicodeChar = 0x0628; else
						if (oChar == 0xC9) nUnicodeChar = 0x0629; else
						if (oChar == 0xCA) nUnicodeChar = 0x062A; else
						if (oChar == 0xCB) nUnicodeChar = 0x062B; else
						if (oChar == 0xCC) nUnicodeChar = 0x062C; else
						if (oChar == 0xCD) nUnicodeChar = 0x062D; else
						if (oChar == 0xCE) nUnicodeChar = 0x062E; else
						if (oChar == 0xCF) nUnicodeChar = 0x062F; else
						if (oChar == 0xD0) nUnicodeChar = 0x0630; else
						if (oChar == 0xD1) nUnicodeChar = 0x0631; else
						if (oChar == 0xD2) nUnicodeChar = 0x0632; else
						if (oChar == 0xD3) nUnicodeChar = 0x0633; else
						if (oChar == 0xD4) nUnicodeChar = 0x0634; else
						if (oChar == 0xD5) nUnicodeChar = 0x0635; else
						if (oChar == 0xD6) nUnicodeChar = 0x0636; else
						if (oChar == 0xD7) nUnicodeChar = 0x0637; else
						if (oChar == 0xD8) nUnicodeChar = 0x0638; else
						if (oChar == 0xD9) nUnicodeChar = 0x0639; else
						if (oChar == 0xDA) nUnicodeChar = 0x063A; else
						if (oChar == 0xE0) nUnicodeChar = 0x0640; else
						if (oChar == 0xE1) nUnicodeChar = 0x0641; else
						if (oChar == 0xE2) nUnicodeChar = 0x0642; else
						if (oChar == 0xE3) nUnicodeChar = 0x0643; else
						if (oChar == 0xE4) nUnicodeChar = 0x0644; else
						if (oChar == 0xE5) nUnicodeChar = 0x0645; else
						if (oChar == 0xE6) nUnicodeChar = 0x0646; else
						if (oChar == 0xE7) nUnicodeChar = 0x0647; else
						if (oChar == 0xE8) nUnicodeChar = 0x0648; else
						if (oChar == 0xE9) nUnicodeChar = 0x0649; else
						if (oChar == 0xEA) nUnicodeChar = 0x064A; else
						if (oChar == 0xEB) nUnicodeChar = 0x064B; else
						if (oChar == 0xEC) nUnicodeChar = 0x064C; else
						if (oChar == 0xED) nUnicodeChar = 0x064D; else
						if (oChar == 0xEE) nUnicodeChar = 0x064E; else
						if (oChar == 0xEF) nUnicodeChar = 0x064F; else
						if (oChar == 0xF0) nUnicodeChar = 0x0650; else
						if (oChar == 0xF1) nUnicodeChar = 0x0651; else
						if (oChar == 0xF2) nUnicodeChar = 0x0652; else
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
								nUnicodeChar == 0xAD
							)
						{
							*nBfr = static_cast<acpl::UInt8>(nUnicodeChar);
							return 1;
						}
						
						// Unique mappings
						if (nUnicodeChar == 0x060C) *nBfr = 0xAC; else
						if (nUnicodeChar == 0x061B) *nBfr = 0xBB; else
						if (nUnicodeChar == 0x061F) *nBfr = 0xBF; else
						if (nUnicodeChar == 0x0621) *nBfr = 0xC1; else
						if (nUnicodeChar == 0x0622) *nBfr = 0xC2; else
						if (nUnicodeChar == 0x0623) *nBfr = 0xC3; else
						if (nUnicodeChar == 0x0624) *nBfr = 0xC4; else
						if (nUnicodeChar == 0x0625) *nBfr = 0xC5; else
						if (nUnicodeChar == 0x0626) *nBfr = 0xC6; else
						if (nUnicodeChar == 0x0627) *nBfr = 0xC7; else
						if (nUnicodeChar == 0x0628) *nBfr = 0xC8; else
						if (nUnicodeChar == 0x0629) *nBfr = 0xC9; else
						if (nUnicodeChar == 0x062A) *nBfr = 0xCA; else
						if (nUnicodeChar == 0x062B) *nBfr = 0xCB; else
						if (nUnicodeChar == 0x062C) *nBfr = 0xCC; else
						if (nUnicodeChar == 0x062D) *nBfr = 0xCD; else
						if (nUnicodeChar == 0x062E) *nBfr = 0xCE; else
						if (nUnicodeChar == 0x062F) *nBfr = 0xCF; else
						if (nUnicodeChar == 0x0630) *nBfr = 0xD0; else
						if (nUnicodeChar == 0x0631) *nBfr = 0xD1; else
						if (nUnicodeChar == 0x0632) *nBfr = 0xD2; else
						if (nUnicodeChar == 0x0633) *nBfr = 0xD3; else
						if (nUnicodeChar == 0x0634) *nBfr = 0xD4; else
						if (nUnicodeChar == 0x0635) *nBfr = 0xD5; else
						if (nUnicodeChar == 0x0636) *nBfr = 0xD6; else
						if (nUnicodeChar == 0x0637) *nBfr = 0xD7; else
						if (nUnicodeChar == 0x0638) *nBfr = 0xD8; else
						if (nUnicodeChar == 0x0639) *nBfr = 0xD9; else
						if (nUnicodeChar == 0x063A) *nBfr = 0xDA; else
						if (nUnicodeChar == 0x0640) *nBfr = 0xE0; else
						if (nUnicodeChar == 0x0641) *nBfr = 0xE1; else
						if (nUnicodeChar == 0x0642) *nBfr = 0xE2; else
						if (nUnicodeChar == 0x0643) *nBfr = 0xE3; else
						if (nUnicodeChar == 0x0644) *nBfr = 0xE4; else
						if (nUnicodeChar == 0x0645) *nBfr = 0xE5; else
						if (nUnicodeChar == 0x0646) *nBfr = 0xE6; else
						if (nUnicodeChar == 0x0647) *nBfr = 0xE7; else
						if (nUnicodeChar == 0x0648) *nBfr = 0xE8; else
						if (nUnicodeChar == 0x0649) *nBfr = 0xE9; else
						if (nUnicodeChar == 0x064A) *nBfr = 0xEA; else
						if (nUnicodeChar == 0x064B) *nBfr = 0xEB; else
						if (nUnicodeChar == 0x064C) *nBfr = 0xEC; else
						if (nUnicodeChar == 0x064D) *nBfr = 0xED; else
						if (nUnicodeChar == 0x064E) *nBfr = 0xEE; else
						if (nUnicodeChar == 0x064F) *nBfr = 0xEF; else
						if (nUnicodeChar == 0x0650) *nBfr = 0xF0; else
						if (nUnicodeChar == 0x0651) *nBfr = 0xF1; else
						if (nUnicodeChar == 0x0652) *nBfr = 0xF2; else
							return 0;
						
						return 1;
					}
					
					return 0;
				}
		};
	}
}

#endif // ACPL_I18N_CHARSETS_CHARSET_ISO_8859_6_1999_INC_CPP
