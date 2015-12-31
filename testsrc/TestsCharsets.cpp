#include "Tests.h"

#include "../src/Charsets.h"
#include "../src/_cp/Memory.inc.cpp"
#include "../src/_i18n/charsets/Charset_UTF_8.inc.cpp"

#define _local_TranscodeUnicodeChars(N_INIT_VAL, N_CMP_SYM, N_CMP_VAL) \
	for (oUccIn = N_INIT_VAL; oUccIn N_CMP_SYM N_CMP_VAL; oUccIn++) \
	{ \
		TestFM(oFuncEnc(oUccIn, oBfr, sizeof(oBfr)) > 0, "%s encode for code point U+%X", nCharset, oUccIn); \
		TestFM(oFuncDec(oBfr, sizeof(oBfr), oUccOut) > 0, "%s decode", nCharset); \
		TestFM(oUccIn == oUccOut, "%s values mismatch: in == %u (0x%X), out == %u (0x%X)", nCharset, oUccIn, oUccIn, oUccOut, oUccOut); \
	}

static int GetCharset(const char *nCharset, acpl::Charsets::Fpc &nCsFpc)
{
	TestFM(acpl::Charsets::GetCharset(nCharset, acpl::Charsets::bomNone, nCsFpc) == true, "Invalid charset '%s'", nCharset);
	return 0;
}

static int TestUnicodeCharset(const char *nCharset, acpl::Unichar nExclFrom = 0, acpl::Unichar nExclTo = 0, acpl::Unichar nLastUcc = 0x10FFFF)
{
	acpl::Charsets::Fpc oCsFpc;
	Test(GetCharset(nCharset, oCsFpc) == 0);
	
	acpl::Charsets::Fpc::fpEncode oFuncEnc = oCsFpc.Encode();
	acpl::Charsets::Fpc::fpDecode oFuncDec = oCsFpc.Decode();
	
	
	acpl::Unichar oUccIn, oUccOut;
	acpl::UInt8 oBfr[6];
	
	
	if (nExclFrom == nExclTo)
	{
		_local_TranscodeUnicodeChars(0x00, <=, nLastUcc);
	}
	else
	if (nExclFrom < nExclTo)
	{
		_local_TranscodeUnicodeChars(0x00, <, nExclFrom);
		_local_TranscodeUnicodeChars(nExclTo + 1, <=, nLastUcc);
	}
	else
	{
		PrintErr("Programmer error: What are you trying to say? (%s:%d)\n", __FUNCTION__, __LINE__);
		return 1;
	}
	
	
	PrintOut("Charset '%s' successful (U+%04X->U+%04X", nCharset, 0, nLastUcc);
	if (nExclFrom < nExclTo)
		PrintOut("; excl. U+%04X->U+%04X", nExclFrom, nExclTo);
	PrintOut(")\n");
	
	return 0;
}

static int TestUnicodeCharsets()
{
	PrintFn();
	
	Test(TestUnicodeCharset("UTF-8") == 0);
	Test(TestUnicodeCharset("UTF-16", 0xD800, 0xDFFF) == 0);
	Test(TestUnicodeCharset("UTF-16BE", 0xD800, 0xDFFF) == 0);
	Test(TestUnicodeCharset("UTF-16LE", 0xD800, 0xDFFF) == 0);
	Test(TestUnicodeCharset("UTF-32") == 0);
	Test(TestUnicodeCharset("UTF-32BE") == 0);
	Test(TestUnicodeCharset("UTF-32LE") == 0);
	Test(TestUnicodeCharset("UCS-2", 0, 0, 0xFFFF) == 0);
	Test(TestUnicodeCharset("UCS-2BE", 0, 0, 0xFFFF) == 0);
	Test(TestUnicodeCharset("UCS-2LE", 0, 0, 0xFFFF) == 0);
	Test(TestUnicodeCharset("UCS-4") == 0);
	Test(TestUnicodeCharset("UCS-4BE") == 0);
	Test(TestUnicodeCharset("UCS-4LE") == 0);
	
	PrintOut("\n");
	
	return 0;
}

static int TestSingleByteCharset(const char *nCharset)
{
	acpl::Charsets::Fpc oCsFpc;
	Test(GetCharset(nCharset, oCsFpc) == 0);
	
	acpl::Charsets::Fpc::fpEncode oFuncEnc = oCsFpc.Encode();
	acpl::Charsets::Fpc::fpDecode oFuncDec = oCsFpc.Decode();
	
	
	acpl::Unichar oUcc;
	acpl::UInt8 oCharOut;
	acpl::Mem::Block<acpl::UInt8>::Type oExcepts;
	
	for (acpl::UInt8 oCharIn = 0; ; oCharIn++)
	{
		if (oFuncDec(&oCharIn, 1, oUcc) > 0)
		{
			TestFM(oFuncEnc(oUcc, &oCharOut, 1) > 0, "%s encode for code point U+%X that decode returned for 0x%X", nCharset, oUcc, oCharIn);
			TestFM(oCharIn == oCharOut, "%s values mismatch: in == %u (0x%X), out == %u (0x%X), code point U+%X", nCharset, oCharIn, oCharIn, oCharOut, oCharOut, oUcc);
		}
		else
			oExcepts.Apnd(&oCharIn, 1);
		
		if (oCharIn == 0xFF)
			break;
	}
	
	
	PrintOut("Charset '%s' successful", nCharset);
	if (oExcepts.Length() > 0)
	{
		const acpl::UInt8 *oBfr = oExcepts.Data();
		acpl::SizeT oBfrLen = oExcepts.Length();
		
		PrintOut(" (no mappings for values:");
		for (acpl::SizeT i = 0; i < oBfrLen; i++)
		{
			if (i > 0)
				PrintOut(", ");
			if ((i % 8) == 0)
				PrintOut("\n        ");
			PrintOut("0x%02X", oBfr[i]);
		}
		PrintOut(")");
	}
	PrintOut("\n");
	
	return 0;
}

static int TestSingleByteCharsets()
{
	PrintFn();
	
	Test(TestSingleByteCharset("ISO-8859-1") == 0);
	Test(TestSingleByteCharset("ISO-8859-2") == 0);
	Test(TestSingleByteCharset("ISO-8859-3") == 0);
	Test(TestSingleByteCharset("ISO-8859-4") == 0);
	Test(TestSingleByteCharset("ISO-8859-5") == 0);
	Test(TestSingleByteCharset("ISO-8859-6") == 0);
	Test(TestSingleByteCharset("ISO-8859-7") == 0);
	Test(TestSingleByteCharset("ISO-8859-8") == 0);
	Test(TestSingleByteCharset("ISO-8859-9") == 0);
	Test(TestSingleByteCharset("ISO-8859-10") == 0);
	Test(TestSingleByteCharset("ISO-8859-11") == 0);
	Test(TestSingleByteCharset("ISO-8859-13") == 0);
	Test(TestSingleByteCharset("ISO-8859-14") == 0);
	Test(TestSingleByteCharset("ISO-8859-15") == 0);
	Test(TestSingleByteCharset("ISO-8859-16") == 0);
	
	PrintOut("\n");
	
	return 0;
}

static int TestCharsetMimeName(const char *nCharset)
{
	acpl::Charsets::Fpc oCsFpc;
	Test(GetCharset(nCharset, oCsFpc) == 0);
	
	TestFM(StringCompare(nCharset, oCsFpc.GetMimeName()()) == 0, "Mime name '%s' does not match '%s'", nCharset, oCsFpc.GetMimeName()());
	PrintOut("Mime name '%s' matches '%s'\n", nCharset, oCsFpc.GetMimeName()());
	
	return 0;
}

static int TestCharsetMimeNames()
{
	PrintFn();
	
	// WARNING: Do not test non-endianness-specific charsets, because the return
	// mime name is endianness-specific!
	
	Test(TestCharsetMimeName("UTF-8") == 0);
	Test(TestCharsetMimeName("UTF-16BE") == 0);
	Test(TestCharsetMimeName("UTF-16LE") == 0);
	Test(TestCharsetMimeName("UTF-32BE") == 0);
	Test(TestCharsetMimeName("UTF-32LE") == 0);
	Test(TestCharsetMimeName("UCS-2BE") == 0);
	Test(TestCharsetMimeName("UCS-2LE") == 0);
	Test(TestCharsetMimeName("UCS-4BE") == 0);
	Test(TestCharsetMimeName("UCS-4LE") == 0);
	
	Test(TestCharsetMimeName("ISO-8859-1") == 0);
	Test(TestCharsetMimeName("ISO-8859-2") == 0);
	Test(TestCharsetMimeName("ISO-8859-3") == 0);
	Test(TestCharsetMimeName("ISO-8859-4") == 0);
	Test(TestCharsetMimeName("ISO-8859-5") == 0);
	Test(TestCharsetMimeName("ISO-8859-6") == 0);
	Test(TestCharsetMimeName("ISO-8859-7") == 0);
	Test(TestCharsetMimeName("ISO-8859-8") == 0);
	Test(TestCharsetMimeName("ISO-8859-9") == 0);
	Test(TestCharsetMimeName("ISO-8859-10") == 0);
	Test(TestCharsetMimeName("ISO-8859-11") == 0);
	Test(TestCharsetMimeName("ISO-8859-13") == 0);
	Test(TestCharsetMimeName("ISO-8859-14") == 0);
	Test(TestCharsetMimeName("ISO-8859-15") == 0);
	Test(TestCharsetMimeName("ISO-8859-16") == 0);
	
	PrintOut("\n");
	
	return 0;
}

static int PerfTestUnicodeCharset(const char *nCharset)
{
	acpl::Charsets::Fpc oCsFpc;
	Test(GetCharset(nCharset, oCsFpc) == 0);
	
	acpl::Charsets::Fpc::fpEncode oFuncEnc = oCsFpc.Encode();
	acpl::Charsets::Fpc::fpDecode oFuncDec = oCsFpc.Decode();
	
	acpl::Unichar oUccIn, oUccOut;
	acpl::UInt8 oBfr[6];
	
	PrintOut("Transcoding 1 billion characters between U+0000 and U+D7FF inclusive in '%s' charset\n", nCharset);
	for (acpl::SizeT i = 0; i < 1000000000; i++)
	{
		if ((i % 100000000) == 0 && i > 0)
			PrintOutFlush("\rmark: %lu00 millions", static_cast<unsigned long>(i / 100000000));
		
		oUccIn = i % 0xD800;
		TestFM(oFuncEnc(oUccIn, oBfr, sizeof(oBfr)) > 0, "%s encode for code point U+%X", nCharset, oUccIn);
		TestFM(oFuncDec(oBfr, sizeof(oBfr), oUccOut) > 0, "%s decode", nCharset);
		TestFM(oUccIn == oUccOut, "%s values mismatch: in == %u (0x%X), out == %u (0x%X)", nCharset, oUccIn, oUccIn, oUccOut, oUccOut);
	}
	PrintOut("\rmark: 1 billion   \n");
	
	return 0;
}

static int Uc2Utf8(int argc, char **argv)
{
	for (acpl::SizeT i = 0; i < static_cast<acpl::SizeT>(argc); i++)
	{
		const char *oCurCp = argv[i];
		
		TestFM(oCurCp[0] == 'U' && oCurCp[1] == '+', "invalid code point: %s", argv[i]);
		oCurCp += 2;
		
		acpl::Unichar oUc = 0;
		for (; *oCurCp != '\0'; oCurCp++)
		{
			oUc *= 16;
			
			if (*oCurCp >= '0' && *oCurCp <= '9')
				oUc += (*oCurCp - '0');
			else
			if (*oCurCp >= 'A' && *oCurCp <= 'F')
				oUc += (*oCurCp - 'A' + 10);
			else
			if (*oCurCp >= 'a' && *oCurCp <= 'a')
				oUc += (*oCurCp - 'a' + 10);
			else
				TestFM(false, "invalid code point: %s", argv[i])
		}
		
		acpl::UInt8 oEnc[7];
		oEnc[acpl::i18n::Charset_UTF_8::Encode(oUc, oEnc, 6)] = '\0';
		TestFM(*oEnc != '\0' || oUc == 0, "invalid code point: %s", argv[i]);
		
		PrintOut("U+%08X=\"", oUc);
		for (const acpl::UInt8 *oCurPos = oEnc; *oCurPos != '\0'; oCurPos++)
		{
			PrintOut("\\x%02X", *oCurPos);
		}
		PrintOut("\"\n");
	}
	
	return 0;
}


SectionFuncMain(charsets)
{
	if (argc < 1)
	{
		PrintOut("\n");
		Test(TestUnicodeCharsets() == 0);
		Test(TestSingleByteCharsets() == 0);
		Test(TestCharsetMimeNames() == 0);
	}
	else
	if (argc == 2 && StringCompare(argv[0], "perf") == 0)
	{
		Test(PerfTestUnicodeCharset(argv[1]) == 0);
	}
	else
	if (argc >= 2 && StringCompare(argv[0], "uc2utf8") == 0)
	{
		Test(Uc2Utf8(argc - 1, argv + 1) == 0);
	}
	else
		return PrintUnrecParams(acplDbgSrcLoc, lSectionName, argc, argv);
	
	return 0;
}

SectionFuncParamsBegin(charsets)
SectionFuncParamsAdd("\"perf\" charset", "performance test for specified charset")
SectionFuncParamsAdd("\"uc2utf8\" code-point [code-point [code-point ...]]", "convert \"U+xxxxxxxx\"-style unicode code point to a string-encoded UTF-8 sequence)")
// SectionFuncParamsAdd("param [param [param ...]]", "description")
SectionFuncParamsEnd(charsets)
