#ifndef ACPL_I18N_CHARSETS_CHARSET_ISO_8859_15_1999_INC_CPP
#define ACPL_I18N_CHARSETS_CHARSET_ISO_8859_15_1999_INC_CPP

// WARNING: Do not modify this file because it was generated using i18nproc

#include "../../Charsets.h"
#include "../../_cp/Memory.inc.cpp"

namespace acpl
{
	namespace i18n
	{
		class Charset_ISO_8859_15_1999
		{
			public:
				static inline bool IsCompatible(const char *nMimeName, acpl::Charsets::ByteOrderMask nBom, acpl::Charsets::Fpc &nFpc)
				{
					if (nMimeName != NULL &&
						acpl::pi::_string_ascii_strcasecmp(nMimeName, "ISO-8859-15") != 0 &&
						acpl::pi::_string_ascii_strcasecmp(nMimeName, "ISO_8859-15") != 0 &&
						acpl::pi::_string_ascii_strcasecmp(nMimeName, "latin9") != 0 &&
						acpl::pi::_string_ascii_strcasecmp(nMimeName, "l9") != 0)
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
					return "ISO-8859-15";
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
								oChar <= 0xA3 ||
								oChar == 0xA5 ||
								oChar == 0xA7 ||
								(oChar >= 0xA9 && oChar <= 0xB3) ||
								(oChar >= 0xB5 && oChar <= 0xB7) ||
								(oChar >= 0xB9 && oChar <= 0xBB) ||
								oChar >= 0xBF
							)
						{
							nUnicodeChar = static_cast<acpl::Unichar>(oChar);
							return 1;
						}
						
						// Unique mappings
						if (oChar == 0xA4) nUnicodeChar = 0x20AC; else
						if (oChar == 0xA6) nUnicodeChar = 0x0160; else
						if (oChar == 0xA8) nUnicodeChar = 0x0161; else
						if (oChar == 0xB4) nUnicodeChar = 0x017D; else
						if (oChar == 0xB8) nUnicodeChar = 0x017E; else
						if (oChar == 0xBC) nUnicodeChar = 0x0152; else
						if (oChar == 0xBD) nUnicodeChar = 0x0153; else
						if (oChar == 0xBE) nUnicodeChar = 0x0178; else
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
								nUnicodeChar <= 0xA3 ||
								nUnicodeChar == 0xA5 ||
								nUnicodeChar == 0xA7 ||
								(nUnicodeChar >= 0xA9 && nUnicodeChar <= 0xB3) ||
								(nUnicodeChar >= 0xB5 && nUnicodeChar <= 0xB7) ||
								(nUnicodeChar >= 0xB9 && nUnicodeChar <= 0xBB) ||
								(nUnicodeChar >= 0xBF && nUnicodeChar <= 0xFF)
							)
						{
							*nBfr = static_cast<acpl::UInt8>(nUnicodeChar);
							return 1;
						}
						
						// Unique mappings
						if (nUnicodeChar == 0x20AC) *nBfr = 0xA4; else
						if (nUnicodeChar == 0x0160) *nBfr = 0xA6; else
						if (nUnicodeChar == 0x0161) *nBfr = 0xA8; else
						if (nUnicodeChar == 0x017D) *nBfr = 0xB4; else
						if (nUnicodeChar == 0x017E) *nBfr = 0xB8; else
						if (nUnicodeChar == 0x0152) *nBfr = 0xBC; else
						if (nUnicodeChar == 0x0153) *nBfr = 0xBD; else
						if (nUnicodeChar == 0x0178) *nBfr = 0xBE; else
							return 0;
						
						return 1;
					}
					
					return 0;
				}
		};
	}
}

#endif // ACPL_I18N_CHARSETS_CHARSET_ISO_8859_15_1999_INC_CPP
