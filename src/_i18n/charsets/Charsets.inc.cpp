#ifndef ACPL_I18N_CHARSETS_CHARSETS_INC_CPP
#define ACPL_I18N_CHARSETS_CHARSETS_INC_CPP

#include "../../Charsets.h"

// ISO/IEC 10646 (Unicode) character sets
#include "Charset_UTF_8.inc.cpp"
#include "Charset_UTF_16.inc.cpp"
#include "Charset_UCS_2.inc.cpp"
#include "Charset_UCS_4.inc.cpp"
#include "Charset_UTF_32.inc.cpp"

// ISO/IEC 8859 character sets
// TODO: Optimize ISO-8858 charsets
#include "Charset_ISO_8859_1_1998.inc.cpp"
#include "Charset_ISO_8859_2_1999.inc.cpp"
#include "Charset_ISO_8859_3_1999.inc.cpp"
#include "Charset_ISO_8859_4_1998.inc.cpp"
#include "Charset_ISO_8859_5_1999.inc.cpp"
#include "Charset_ISO_8859_6_1999.inc.cpp"
#include "Charset_ISO_8859_7_2003.inc.cpp"
#include "Charset_ISO_8859_8_1999.inc.cpp"
#include "Charset_ISO_8859_9_1999.inc.cpp"
#include "Charset_ISO_8859_10_1998.inc.cpp"
#include "Charset_ISO_8859_11_2001.inc.cpp"
#include "Charset_ISO_8859_13_1998.inc.cpp"
#include "Charset_ISO_8859_14_1998.inc.cpp"
#include "Charset_ISO_8859_15_1999.inc.cpp"
#include "Charset_ISO_8859_16_2001.inc.cpp"

namespace acpl
{
	static acpl::Charsets::Fpc::fpIsCompatible lCharsets[] =
	{
		// ISO/IEC 10646 (Unicode) character sets (WARNING: These have to be the first on the list)
		acpl::i18n::Charset_UTF_8::IsCompatible, // WARNING: UTF-8 has to be the first in the list
		acpl::i18n::Charset_UTF_16::IsCompatible, // WARNING: UTF-16 has to come before UCS-2
		acpl::i18n::Charset_UCS_2::IsCompatible,
		acpl::i18n::Charset_UCS_4::IsCompatible, // WARNING: UCS-4 has to come before UTF-32
		acpl::i18n::Charset_UTF_32::IsCompatible,
		
		// ISO/IEC 8859 character sets
		acpl::i18n::Charset_ISO_8859_1_1998::IsCompatible, // WARNING: ISO-8859-1 has to be the first in the list
		acpl::i18n::Charset_ISO_8859_2_1999::IsCompatible,
		acpl::i18n::Charset_ISO_8859_3_1999::IsCompatible,
		acpl::i18n::Charset_ISO_8859_4_1998::IsCompatible,
		acpl::i18n::Charset_ISO_8859_5_1999::IsCompatible,
		acpl::i18n::Charset_ISO_8859_6_1999::IsCompatible,
		acpl::i18n::Charset_ISO_8859_7_2003::IsCompatible,
		acpl::i18n::Charset_ISO_8859_8_1999::IsCompatible,
		acpl::i18n::Charset_ISO_8859_9_1999::IsCompatible,
		acpl::i18n::Charset_ISO_8859_10_1998::IsCompatible,
		acpl::i18n::Charset_ISO_8859_11_2001::IsCompatible,
		acpl::i18n::Charset_ISO_8859_13_1998::IsCompatible,
		acpl::i18n::Charset_ISO_8859_14_1998::IsCompatible,
		acpl::i18n::Charset_ISO_8859_15_1999::IsCompatible,
		acpl::i18n::Charset_ISO_8859_16_2001::IsCompatible,
		
		// End of list
		NULL
	};
}

bool acpl::Charsets::GetCharset(const char *nMimeName, acpl::Charsets::ByteOrderMask nBom, acpl::Charsets::Fpc &nFpc) // static
{
	for (acpl::Charsets::Fpc::fpIsCompatible *oCurCharsets = lCharsets; oCurCharsets != NULL; oCurCharsets++)
		if ((*oCurCharsets)(nMimeName, nBom, nFpc) == true)
			return true;
	
	return false;
}

#endif // ACPL_I18N_CHARSETS_CHARSETS_INC_CPP
