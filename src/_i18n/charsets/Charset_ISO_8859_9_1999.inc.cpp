#ifndef ACPL_I18N_CHARSETS_CHARSET_ISO_8859_9_1999_INC_CPP
#define ACPL_I18N_CHARSETS_CHARSET_ISO_8859_9_1999_INC_CPP

// WARNING: Do not modify this file because it was generated using i18nproc

#include "../../Charsets.h"
#include "../../_cp/Memory.inc.cpp"

namespace acpl
{
	namespace i18n
	{
		class Charset_ISO_8859_9_1999
		{
			public:
				static inline bool IsCompatible(const char *nMimeName, acpl::Charsets::ByteOrderMask nBom, acpl::Charsets::Fpc &nFpc)
				{
					if (nMimeName != NULL &&
						acpl::pi::_string_ascii_strcasecmp(nMimeName, "ISO-8859-9") != 0 &&
						acpl::pi::_string_ascii_strcasecmp(nMimeName, "ISO_8859-9") != 0 &&
						acpl::pi::_string_ascii_strcasecmp(nMimeName, "latin5") != 0 &&
						acpl::pi::_string_ascii_strcasecmp(nMimeName, "l5") != 0 &&
						acpl::pi::_string_ascii_strcasecmp(nMimeName, "iso-ir-148") != 0 &&
						acpl::pi::_string_ascii_strcasecmp(nMimeName, "csISOLatin5") != 0)
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
					return "ISO-8859-9";
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
								oChar <= 0xCF ||
								(oChar >= 0xD1 && oChar <= 0xDC) ||
								(oChar >= 0xDF && oChar <= 0xEF) ||
								(oChar >= 0xF1 && oChar <= 0xFC) ||
								oChar == 0xFF
							)
						{
							nUnicodeChar = static_cast<acpl::Unichar>(oChar);
							return 1;
						}
						
						// Unique mappings
						if (oChar == 0xD0) nUnicodeChar = 0x011E; else
						if (oChar == 0xDD) nUnicodeChar = 0x0130; else
						if (oChar == 0xDE) nUnicodeChar = 0x015E; else
						if (oChar == 0xF0) nUnicodeChar = 0x011F; else
						if (oChar == 0xFD) nUnicodeChar = 0x0131; else
						if (oChar == 0xFE) nUnicodeChar = 0x015F; else
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
								nUnicodeChar <= 0xCF ||
								(nUnicodeChar >= 0xD1 && nUnicodeChar <= 0xDC) ||
								(nUnicodeChar >= 0xDF && nUnicodeChar <= 0xEF) ||
								(nUnicodeChar >= 0xF1 && nUnicodeChar <= 0xFC) ||
								nUnicodeChar == 0xFF
							)
						{
							*nBfr = static_cast<acpl::UInt8>(nUnicodeChar);
							return 1;
						}
						
						// Unique mappings
						if (nUnicodeChar == 0x011E) *nBfr = 0xD0; else
						if (nUnicodeChar == 0x0130) *nBfr = 0xDD; else
						if (nUnicodeChar == 0x015E) *nBfr = 0xDE; else
						if (nUnicodeChar == 0x011F) *nBfr = 0xF0; else
						if (nUnicodeChar == 0x0131) *nBfr = 0xFD; else
						if (nUnicodeChar == 0x015F) *nBfr = 0xFE; else
							return 0;
						
						return 1;
					}
					
					return 0;
				}
		};
	}
}

#endif // ACPL_I18N_CHARSETS_CHARSET_ISO_8859_9_1999_INC_CPP
