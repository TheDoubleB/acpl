#ifndef ACPL_I18N_CHARSETS_CHARSET_UTF_16_INC_CPP
#define ACPL_I18N_CHARSETS_CHARSET_UTF_16_INC_CPP

#include "../../Charsets.h"
#include "../../Endian.h"
#include "../../_cp/Memory.inc.cpp"

namespace acpl
{
	namespace i18n
	{
		class Charset_UTF_16
		{
			public:
				static inline bool IsCompatible(const char *nMimeName, acpl::Charsets::ByteOrderMask nBom, acpl::Charsets::Fpc &nFpc)
				{
					if (nMimeName != NULL)
					{
						if (acpl::pi::_string_ascii_strcasecmp("UTF-16", nMimeName) == 0 || acpl::pi::_string_ascii_strcasecmp("utf16", nMimeName) == 0)
						{
							if (nBom == acpl::Charsets::bomNone)
							{
								if (acpl::Endian::IsNativeLittle() == true)
									nBom = acpl::Charsets::bom2ByteLe;
								else
									nBom = acpl::Charsets::bom2ByteBe;
							}
						}
						else
						if (acpl::pi::_string_ascii_strcasecmp("UTF-16BE", nMimeName) == 0 || acpl::pi::_string_ascii_strcasecmp("utf16be", nMimeName) == 0)
							nBom = acpl::Charsets::bom2ByteBe;
						else
						if (acpl::pi::_string_ascii_strcasecmp("UTF-16LE", nMimeName) == 0 || acpl::pi::_string_ascii_strcasecmp("utf16le", nMimeName) == 0)
							nBom = acpl::Charsets::bom2ByteLe;
					}
					
					
					if (nBom == acpl::Charsets::bom2ByteLe)
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
					if (nBom == acpl::Charsets::bom2ByteBe)
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
					return 2;
				}
				
				static inline acpl::SizeT MaxSeqSize()
				{
					return 4;
				}
				
				
				static inline acpl::SizeT CreateBom(acpl::UInt8 *nBfr, acpl::SizeT nBfrSize, bool nIsLe)
				{
					if (nBfrSize >= 2)
					{
						if (nIsLe == true)
							acpl::Endian::ToLittle<acpl::UInt16>(0xFEFF, nBfr);
						else
							acpl::Endian::ToBig<acpl::UInt16>(0xFEFF, nBfr);
						
						return 2;
					}
					else
						return 0;
				}
				
				static inline acpl::SizeT Decode(const acpl::UInt8 *nBfr, acpl::SizeT nBfrSize, acpl::Unichar &nUnicodeChar, bool nIsLe)
				{
					if (nBfrSize >= 2)
					{
						acpl::UInt16 oVal1, oVal2;
						
						if (nIsLe == true)
							acpl::Endian::FromLittle(oVal1, nBfr);
						else
							acpl::Endian::FromBig(oVal1, nBfr);
						
						if (oVal1 < 0xD800 || oVal1 > 0xDFFF)
						{
							nUnicodeChar = oVal1;
							return 2;
						}
						else
						if (oVal1 < 0xDC00 && nBfrSize >= 4)
						{
							if (nIsLe == true)
								acpl::Endian::FromLittle(oVal2, (nBfr + 2));
							else
								acpl::Endian::FromBig(oVal2, (nBfr + 2));
							
							if (oVal2 >= 0xDC00 && oVal2 <= 0xDFFF)
							{
								nUnicodeChar = (
									((oVal1 & 0x3FF) << 10) |
									(oVal2 & 0x3FF))
									+ 0x10000;
								
								return 4;
							}
						}
					}
					
					return 0;
				}
				
				static inline acpl::SizeT Encode(acpl::Unichar nUnicodeChar, acpl::UInt8 *nBfr, acpl::SizeT nBfrSize, bool nIsLe)
				{
					if (nUnicodeChar <= 0xFFFF && nBfrSize >= 2)
					{
						if (nUnicodeChar < 0xD800 || nUnicodeChar > 0xDFFF)
						{
							if (nIsLe == true)
								acpl::Endian::ToLittle(static_cast<acpl::UInt16>(nUnicodeChar), nBfr);
							else
								acpl::Endian::ToBig(static_cast<acpl::UInt16>(nUnicodeChar), nBfr);
							
							return 2;
						}
					}
					else
					if (nUnicodeChar <= 0x10FFFF && nBfrSize >= 4)
					{
						if (nIsLe == true)
						{
							acpl::Endian::ToLittle(static_cast<acpl::UInt16>(0xD800 | ((nUnicodeChar - 0x10000) >> 10)), nBfr);
							acpl::Endian::ToLittle(static_cast<acpl::UInt16>(0xDC00 | ((nUnicodeChar - 0x10000) & 0x3FF)), (nBfr + 2));
						}
						else
						{
							acpl::Endian::ToBig(static_cast<acpl::UInt16>(0xD800 | ((nUnicodeChar - 0x10000) >> 10)), nBfr);
							acpl::Endian::ToBig(static_cast<acpl::UInt16>(0xDC00 | ((nUnicodeChar - 0x10000) & 0x3FF)), (nBfr + 2));
						}
						
						return 4;
					}
					
					return 0;
				}
				
				
				static inline const char *BE_GetMimeName()
				{
					return "UTF-16BE";
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
					return "UTF-16LE";
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

#endif // ACPL_I18N_CHARSETS_CHARSET_UTF_16_INC_CPP
