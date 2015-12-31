#ifndef ACPL_I18N_CHARSETS_CHARSET_UTF_32_INC_CPP
#define ACPL_I18N_CHARSETS_CHARSET_UTF_32_INC_CPP

#include "../../Charsets.h"
#include "../../Endian.h"
#include "../../_cp/Memory.inc.cpp"

namespace acpl
{
	namespace i18n
	{
		class Charset_UTF_32
		{
			public:
				static inline bool IsCompatible(const char *nMimeName, acpl::Charsets::ByteOrderMask nBom, acpl::Charsets::Fpc &nFpc)
				{
					if (nMimeName != NULL)
					{
						if (acpl::pi::_string_ascii_strcasecmp("UTF-32", nMimeName) == 0 || acpl::pi::_string_ascii_strcasecmp("utf32", nMimeName) == 0)
						{
							if (nBom == acpl::Charsets::bomNone)
							{
								if (acpl::Endian::IsNativeLittle() == true)
									nBom = acpl::Charsets::bom4ByteLe;
								else
									nBom = acpl::Charsets::bom4ByteBe;
							}
						}
						else
						if (acpl::pi::_string_ascii_strcasecmp("UTF-32BE", nMimeName) == 0 || acpl::pi::_string_ascii_strcasecmp("utf32be", nMimeName) == 0)
							nBom = acpl::Charsets::bom4ByteBe;
						else
						if (acpl::pi::_string_ascii_strcasecmp("UTF-32LE", nMimeName) == 0 || acpl::pi::_string_ascii_strcasecmp("utf32le", nMimeName) == 0)
							nBom = acpl::Charsets::bom4ByteLe;
					}
					
					
					if (nBom == acpl::Charsets::bom4ByteLe)
					{
						nFpc.Set(
								IsCompatible,
								LE_GetMimeName,
								IsBomSuggested,
								LE_CreateBom,
								MinSeqSize,
								MaxSeqSize,
								LE_Decode,
								LE_Encode
							);
					}
					else
					if (nBom == acpl::Charsets::bom4ByteBe)
					{
						nFpc.Set(
								IsCompatible,
								BE_GetMimeName,
								IsBomSuggested,
								BE_CreateBom,
								MinSeqSize,
								MaxSeqSize,
								BE_Decode,
								BE_Encode
							);
					}
					else
						return false;
					
					
					return true;
				}
				
				static inline bool IsBomSuggested()
				{
					return true;
				}
				
				static inline acpl::SizeT MinSeqSize()
				{
					return 4;
				}
				
				static inline acpl::SizeT MaxSeqSize()
				{
					return 4;
				}
				
				
				static inline acpl::SizeT CreateBom(acpl::UInt8 *nBfr, acpl::SizeT nBfrSize, bool nIsLe)
				{
					if (nBfrSize >= 4)
					{
						nBfr[0] = 0x00;
						nBfr[1] = 0x00;
						
						if (nIsLe == true)
							acpl::Endian::ToLittle<acpl::UInt16>(0xFEFF, (nBfr + 2));
						else
							acpl::Endian::ToBig<acpl::UInt16>(0xFEFF, (nBfr + 2));
						
						return 4;
					}
					else
						return 0;
				}
				
				static inline acpl::SizeT Decode(const acpl::UInt8 *nBfr, acpl::SizeT nBfrSize, acpl::Unichar &nUnicodeChar, bool nIsLe)
				{
					if (nBfrSize >= 4)
					{
						acpl::Unichar oUnicodeChar;
						
						if (nIsLe == true)
							acpl::Endian::FromLittle(oUnicodeChar, nBfr);
						else
							acpl::Endian::FromBig(oUnicodeChar, nBfr);
						
						if (oUnicodeChar <= 0x10FFFF)
						{
							nUnicodeChar = oUnicodeChar;
							return 4;
						}
					}
					
					return 0;
				}
				
				static inline acpl::SizeT Encode(acpl::Unichar nUnicodeChar, acpl::UInt8 *nBfr, acpl::SizeT nBfrSize, bool nIsLe)
				{
					if (nUnicodeChar <= 0x10FFFF && nBfrSize >= 4)
					{
						if (nIsLe == true)
							acpl::Endian::ToLittle(nUnicodeChar, nBfr);
						else
							acpl::Endian::ToBig(nUnicodeChar, nBfr);
						
						return 4;
					}
					
					return 0;
				}
				
				
				static inline const char *BE_GetMimeName()
				{
					return "UTF-32BE";
				}
				
				static inline acpl::SizeT BE_CreateBom(acpl::UInt8 *nBfr, acpl::SizeT nBfrSize)
				{
					return CreateBom(nBfr, nBfrSize, false);
				}
				
				static inline acpl::SizeT BE_Decode(const acpl::UInt8 *nBfr, acpl::SizeT nBfrSize, acpl::Unichar &nUnicodeChar)
				{
					return Decode(nBfr, nBfrSize, nUnicodeChar, false);
				}
				
				static inline acpl::SizeT BE_Encode(acpl::Unichar nUnicodeChar, acpl::UInt8 *nBfr, acpl::SizeT nBfrSize)
				{
					return Encode(nUnicodeChar, nBfr, nBfrSize, false);
				}
				
				static inline const char *LE_GetMimeName()
				{
					return "UTF-32LE";
				}
				
				static inline acpl::SizeT LE_CreateBom(acpl::UInt8 *nBfr, acpl::SizeT nBfrSize)
				{
					return CreateBom(nBfr, nBfrSize, true);
				}
				
				static inline acpl::SizeT LE_Decode(const acpl::UInt8 *nBfr, acpl::SizeT nBfrSize, acpl::Unichar &nUnicodeChar)
				{
					return Decode(nBfr, nBfrSize, nUnicodeChar, true);
				}
				
				static inline acpl::SizeT LE_Encode(acpl::Unichar nUnicodeChar, acpl::UInt8 *nBfr, acpl::SizeT nBfrSize)
				{
					return Encode(nUnicodeChar, nBfr, nBfrSize, true);
				}
		};
	}
}

#endif // ACPL_I18N_CHARSETS_CHARSET_UTF_32_INC_CPP
