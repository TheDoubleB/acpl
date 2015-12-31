#ifndef ACPL_I18N_CHARSETS_CHARSET_UTF_8_INC_CPP
#define ACPL_I18N_CHARSETS_CHARSET_UTF_8_INC_CPP

#include "../../Charsets.h"
#include "../../_cp/Memory.inc.cpp"

namespace acpl
{
	namespace i18n
	{
		class Charset_UTF_8
		{
			public:
				static inline bool IsCompatible(const char *nMimeName, acpl::Charsets::ByteOrderMask nBom, acpl::Charsets::Fpc &nFpc)
				{
					if (nMimeName != NULL && acpl::pi::_string_ascii_strcasecmp("UTF-8", nMimeName) != 0 && acpl::pi::_string_ascii_strcasecmp("utf8", nMimeName) != 0)
						return false;
					
					if (nBom != acpl::Charsets::bomNone && nBom != acpl::Charsets::bom1Byte)
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
					return "UTF-8";
				}
				
				static inline bool IsBomSuggested()
				{
					return false;
				}
				
				static inline acpl::SizeT CreateBom(acpl::UInt8 *nBfr, acpl::SizeT nBfrSize)
				{
					if (nBfrSize >= 3)
					{
						nBfr[0] = 0xEF;
						nBfr[1] = 0xBB;
						nBfr[2] = 0xBF;
						return 3;
					}
					else
						return 0;
				}
				
				static inline acpl::SizeT MinSeqSize()
				{
					return 1;
				}
				
				static inline acpl::SizeT MaxSeqSize()
				{
					return 6;
				}
				
				static inline acpl::SizeT Decode(const acpl::UInt8 *nBfr, acpl::SizeT nBfrSize, acpl::Unichar &nUnicodeChar)
				{
					if (nBfrSize >= 1)
					{
						acpl::UInt8 oBfrFb = *nBfr;
						
						if (oBfrFb < 0x80)
						{
							nUnicodeChar = oBfrFb;
							return 1;
						}
						else
						if (oBfrFb < 0xC0)
							return 0;
						else
						if (oBfrFb < 0xE0)
						{
							if (nBfrSize >= 2)
							{
								nUnicodeChar =
									((oBfrFb & 0x1F) << 6) |
									(nBfr[1] & 0x3F);
								return 2;
							}
						}
						else
						if (oBfrFb < 0xF0)
						{
							if (nBfrSize >= 3)
							{
								nUnicodeChar =
									((oBfrFb & 0x0F) << 12) |
									((nBfr[1] & 0x3F) << 6) |
									(nBfr[2] & 0x3F);
								return 3;
							}
						}
						else
						if (oBfrFb < 0xF8)
						{
							if (nBfrSize >= 4)
							{
								nUnicodeChar =
									((oBfrFb & 0x07) << 18) |
									((nBfr[1] & 0x3F) << 12) |
									((nBfr[2] & 0x3F) << 6) |
									(nBfr[3] & 0x3F);
								return 4;
							}
						}
						else
						if (oBfrFb < 0xFC)
						{
							if (nBfrSize >= 5)
							{
								nUnicodeChar =
									((oBfrFb & 0x03) << 24) |
									((nBfr[1] & 0x3F) << 18) |
									((nBfr[2] & 0x3F) << 12) |
									((nBfr[3] & 0x3F) << 6) |
									(nBfr[4] & 0x3F);
								return 5;
							}
						}
						else
						if (oBfrFb < 0xFE)
						{
							if (nBfrSize >= 6)
							{
								nUnicodeChar =
									((oBfrFb & 0x01) << 30) |
									((nBfr[1] & 0x3F) << 24) |
									((nBfr[2] & 0x3F) << 18) |
									((nBfr[3] & 0x3F) << 12) |
									((nBfr[4] & 0x3F) << 6) |
									(nBfr[5] & 0x3F);
								return 6;
							}
						}
					}
					
					return 0;
				}
				
				static inline acpl::SizeT Encode(acpl::Unichar nUnicodeChar, acpl::UInt8 *nBfr, acpl::SizeT nBfrSize)
				{
					if (nUnicodeChar < 0x80 && nBfrSize >= 1)
					{
						nBfr[0] = static_cast<acpl::UInt8>(nUnicodeChar);
						return 1;
					}
					else
					if (nUnicodeChar < 0x800 && nBfrSize >= 2)
					{
						nBfr[0] = static_cast<acpl::UInt8>(0xC0 | (nUnicodeChar >> 6));
						nBfr[1] = static_cast<acpl::UInt8>(0x80 | (nUnicodeChar & 0x3F));
						return 2;
					}
					else
					if (nUnicodeChar < 0x10000 && nBfrSize >= 3)
					{
						nBfr[0] = static_cast<acpl::UInt8>(0xE0 | (nUnicodeChar >> 12));
						nBfr[1] = static_cast<acpl::UInt8>(0x80 | ((nUnicodeChar >> 6) & 0x3F));
						nBfr[2] = static_cast<acpl::UInt8>(0x80 | (nUnicodeChar & 0x3F));
						return 3;
					}
					else
					if (nUnicodeChar < 0x200000 && nBfrSize >= 4)
					{
						nBfr[0] = static_cast<acpl::UInt8>(0xF0 | (nUnicodeChar >> 18));
						nBfr[1] = static_cast<acpl::UInt8>(0x80 | ((nUnicodeChar >> 12) & 0x3F));
						nBfr[2] = static_cast<acpl::UInt8>(0x80 | ((nUnicodeChar >> 6) & 0x3F));
						nBfr[3] = static_cast<acpl::UInt8>(0x80 | (nUnicodeChar & 0x3F));
						return 4;
					}
					else
					if (nUnicodeChar < 0x4000000 && nBfrSize >= 5)
					{
						nBfr[0] = static_cast<acpl::UInt8>(0xF8 | (nUnicodeChar >> 24));
						nBfr[1] = static_cast<acpl::UInt8>(0x80 | ((nUnicodeChar >> 18) & 0x3F));
						nBfr[2] = static_cast<acpl::UInt8>(0x80 | ((nUnicodeChar >> 12) & 0x3F));
						nBfr[3] = static_cast<acpl::UInt8>(0x80 | ((nUnicodeChar >> 6) & 0x3F));
						nBfr[4] = static_cast<acpl::UInt8>(0x80 | (nUnicodeChar & 0x3F));
						return 5;
					}
					else
					if (nUnicodeChar < 0x80000000 && nBfrSize >= 6)
					{
						nBfr[0] = static_cast<acpl::UInt8>(0xFC | (nUnicodeChar >> 30));
						nBfr[1] = static_cast<acpl::UInt8>(0x80 | ((nUnicodeChar >> 24) & 0x3F));
						nBfr[2] = static_cast<acpl::UInt8>(0x80 | ((nUnicodeChar >> 18) & 0x3F));
						nBfr[3] = static_cast<acpl::UInt8>(0x80 | ((nUnicodeChar >> 12) & 0x3F));
						nBfr[4] = static_cast<acpl::UInt8>(0x80 | ((nUnicodeChar >> 6) & 0x3F));
						nBfr[5] = static_cast<acpl::UInt8>(0x80 | (nUnicodeChar & 0x3F));
						return 6;
					}
					else
						return 0;
				}
		};
	}
}

#endif // ACPL_I18N_CHARSETS_CHARSET_UTF_8_INC_CPP
