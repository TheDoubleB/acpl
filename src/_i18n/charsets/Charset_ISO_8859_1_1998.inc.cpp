#ifndef ACPL_I18N_CHARSETS_CHARSET_ISO_8859_1_1998_INC_CPP
#define ACPL_I18N_CHARSETS_CHARSET_ISO_8859_1_1998_INC_CPP

// WARNING: Do not modify this file because it was generated using i18nproc

#include "../../Charsets.h"
#include "../../_cp/Memory.inc.cpp"

namespace acpl
{
	namespace i18n
	{
		class Charset_ISO_8859_1_1998
		{
			public:
				static inline bool IsCompatible(const char *nMimeName, acpl::Charsets::ByteOrderMask nBom, acpl::Charsets::Fpc &nFpc)
				{
					if (nMimeName != NULL &&
						acpl::pi::_string_ascii_strcasecmp(nMimeName, "ISO-8859-1") != 0 &&
						acpl::pi::_string_ascii_strcasecmp(nMimeName, "ISO_8859-1") != 0 &&
						acpl::pi::_string_ascii_strcasecmp(nMimeName, "latin1") != 0 &&
						acpl::pi::_string_ascii_strcasecmp(nMimeName, "l1") != 0 &&
						acpl::pi::_string_ascii_strcasecmp(nMimeName, "IBM819") != 0 &&
						acpl::pi::_string_ascii_strcasecmp(nMimeName, "CP819") != 0 &&
						acpl::pi::_string_ascii_strcasecmp(nMimeName, "iso-ir-100") != 0 &&
						acpl::pi::_string_ascii_strcasecmp(nMimeName, "csISOLatin1") != 0)
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
					return "ISO-8859-1";
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
						nUnicodeChar = static_cast<acpl::Unichar>(*nBfr);
						return 1;
					}
					
					return 0;
				}
				
				static inline acpl::SizeT Encode(acpl::Unichar nUnicodeChar, acpl::UInt8 *nBfr, acpl::SizeT nBfrSize)
				{
					if (nBfrSize >= 1)
					{
						if (nUnicodeChar <= 0xFF)
						{
							*nBfr = static_cast<acpl::UInt8>(nUnicodeChar);
							return 1;
						}
					}
					
					return 0;
				}
		};
	}
}

#endif // ACPL_I18N_CHARSETS_CHARSET_ISO_8859_1_1998_INC_CPP
