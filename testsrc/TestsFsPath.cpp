#include "Tests.h"

#include "TestsFsCmn.inc.cpp"

#include "../src/fs/Path.h"
#include "../src/Time.h"


#if   (acplArch == acplArchX8632)
#	define _local_Int64FmtPx "ll"
#elif (acplArch == acplArchX8664)
#	define _local_Int64FmtPx "l"
#else
#	error Definition of _local_Int64FmtPx missing for this architecture
#endif


#define _local_BoolToStr(N_EXPR) ((N_EXPR) ? "true" : "false")

#define _local_TestPex(N_EXPR, N_PATH, N_LINE_ADDED, N_FMT, ...) \
	TestFM(N_EXPR, "Path='" _local_PathcharFmt "' @ %lu: " N_FMT, N_PATH, static_cast<acpl::SizeT>(N_LINE_ADDED), __VA_ARGS__);

#define _local_TestPathStrLine(N_PATH, N_EXPECT_PATH, N_LINE_ADDED) \
	_local_TestPex(StringCompare(N_PATH, N_EXPECT_PATH) == 0, N_PATH, N_LINE_ADDED, "ExpectedPath='" _local_PathcharFmt "'", N_EXPECT_PATH)

#define _local_TestPathStr(N_PATH, N_EXPECT_PATH) \
	_local_TestPathStrLine(N_PATH, N_EXPECT_PATH, __LINE__)

// NOTE: _local_PathcharPx() can only appear in the following two because others are not used with const strings
#define _local_TestPathObj(N_PATH_OBJ, N_EXPECT_PATH) \
	_local_TestPathStr(N_PATH_OBJ.GetStr(), _local_PathcharPx(N_EXPECT_PATH))

#define _local_TestStringObj(N_OBJ, N_EXPECT_STR) \
	TestFM(StringCompare(N_OBJ.GetStr(), _local_PathcharPx(N_EXPECT_STR)) == 0, "Path='" _local_PathcharFmt "', ExpectedPath='" _local_PathcharFmt "'", N_OBJ.GetStr(), _local_PathcharPx(N_EXPECT_STR))


// PEX = Path expects
// CTD = Cut trailing delimiters
// IAR = Is absolute/root

struct _local_PexCtd
{
	acpl::SizeT sLine;
	const char *sPath;
	const acpl::Pathchar *sExpect;
};
#define _local_PexCtdAdd(N_PATH, N_EXPECT) { __LINE__, N_PATH, _local_PathcharPx(N_EXPECT) },
#define _local_PexCtdEnd() { __LINE__, NULL, NULL },
static inline int _local_TestPexCtd(const _local_PexCtd *oPex)
{
	acpl::Path oPath;
	
	while (oPex->sPath != NULL)
	{
		oPath.Set(oPex->sPath);
		_local_TestPathStrLine(oPath.GetStr(), oPex->sExpect, oPex->sLine);
		oPex++;
	}
	
	return 0;
}

struct _local_PexIar
{
	acpl::SizeT sLine;
	const char *sPath;
	bool sIsAbs;
	bool sIsRoot;
};
#define _local_PexIarAdd(N_PATH, N_IS_ABS, N_IS_ROOT) { __LINE__, N_PATH, N_IS_ABS, N_IS_ROOT },
#define _local_PexIarEnd() { __LINE__, NULL, false, false },
static inline int _local_TestPexIar(const _local_PexIar *oPex)
{
	acpl::Path oPath;
	
	while (oPex->sPath != NULL)
	{
		oPath.Set(oPex->sPath);
		_local_TestPex((oPath.IsAbsolute() == oPex->sIsAbs && oPath.IsRoot() == oPex->sIsRoot), oPath.GetStr(), __LINE__,
				  "AbsRoot=(%s,%s) ExpectedAbsRoot=(%s,%s)",
				  _local_BoolToStr(oPath.IsAbsolute()),
				  _local_BoolToStr(oPath.IsRoot()),
				  _local_BoolToStr(oPex->sIsAbs),
				  _local_BoolToStr(oPex->sIsRoot));
		oPex++;
	}
	
	return 0;
}

class _local_StatBase_Ext : public acpl::Path::StatBase
{
	public:
		inline void Clear() { acpl::Path::StatBase::Clear(); }
		inline acpl::Path::StatBase::InfoStat &GetInfoStat()  { return acpl::Path::StatBase::GetInfoStat(); }
};


static int TestString()
{
	PrintFn();
	
	acpl::Path::String oStr;
	acpl::Path::String oAltStr("pqr");
	acpl::String oSelfStr("abc");
	acpl::Unichar oUcBfr[4] = { 'm', 'n', 'o', 0 };
	
	
	// Init
	Test(oStr.IsEmpty() == true);
	_local_TestStringObj(oStr, "");
	
	// Clear() on init
	oStr.Clear();
	Test(oStr.IsEmpty() == true);
	_local_TestStringObj(oStr, "");
	
	// Set() methods
	oStr.Set(oSelfStr);
	_local_TestStringObj(oStr, "abc");
	oStr.Set(acpl::String::Ref("def", false));
	_local_TestStringObj(oStr, "def");
	oStr.Set(acpl::String::Ref("ghi", true));
	_local_TestStringObj(oStr, "ghi");
	oStr.Set(L"jkl");
	_local_TestStringObj(oStr, "jkl");
	oStr.Set(oUcBfr);
	_local_TestStringObj(oStr, "mno");
	oStr.Set(oAltStr);
	_local_TestStringObj(oStr, "pqr");
	
	// Clear() on non-init
	oStr.Clear();
	Test(oStr.IsEmpty() == true);
	_local_TestStringObj(oStr, "");
	
	// Apnd() methods
	oStr.Apnd(oSelfStr);
	_local_TestStringObj(oStr, "abc");
	oStr.Apnd(acpl::String::Ref("def", false));
	_local_TestStringObj(oStr, "abcdef");
	oStr.Apnd(acpl::String::Ref("ghi", true));
	_local_TestStringObj(oStr, "abcdefghi");
	oStr.Apnd(L"jkl");
	_local_TestStringObj(oStr, "abcdefghijkl");
	oStr.Apnd(oUcBfr);
	_local_TestStringObj(oStr, "abcdefghijklmno");
	oStr.Apnd(oAltStr);
	_local_TestStringObj(oStr, "abcdefghijklmnopqr");
	oStr.Apnd(oSelfStr); // a repeat to see if the actual append works correctly for every option
	_local_TestStringObj(oStr, "abcdefghijklmnopqrabc");
	
	// Various (invalid) sequences that will result in '?'
	oStr.Set("abc""\xDF""def"); // Valid first byte, but one too few follow bytes
	_local_TestStringObj(oStr, "abc?def");
	oStr.Set("abc""\xEF\xBF""def");
	_local_TestStringObj(oStr, "abc??def");
	oStr.Set("abc""\xF7\xBF\xBF""def");
	_local_TestStringObj(oStr, "abc???def");
	oStr.Set("abc""\xFB\xBF\xBF\xBF""def");
	_local_TestStringObj(oStr, "abc????def");
	oStr.Set("abc""\xFD\xBF\xBF\xBF\xBF""def");
	_local_TestStringObj(oStr, "abc?????def");
	oStr.Set("abc""\xFD\xBF\xBF\xBF\xBF\xBF""def"); // Valid sequence, but cannot be encoded in any acpl::Pathchar encoding
	_local_TestStringObj(oStr, "abc?def");
	
	return 0;
}

static int TestPerm()
{
	PrintFn();
	
	
	acpl::Path::Permission oAm(0xFFFF);
	acpl::Path::Permission::Str oPermStr;
	
	
	// Test limits
	
	Test(oAm.GetValue() == 0xFFF);
	oAm.SetValue(0x1FFF);
	Test(oAm.GetValue() == 0xFFF);
	
	
	// Test elements (current)
	
	Test(oAm.HasSetuid()   == true);
	Test(oAm.HasSetgid()   == true);
	Test(oAm.HasSticky()   == true);
	Test(oAm.CanUsrRead()  == true);
	Test(oAm.CanUsrWrite() == true);
	Test(oAm.CanUsrExec()  == true);
	Test(oAm.CanGrpRead()  == true);
	Test(oAm.CanGrpWrite() == true);
	Test(oAm.CanGrpExec()  == true);
	Test(oAm.CanOthRead()  == true);
	Test(oAm.CanOthWrite() == true);
	Test(oAm.CanOthExec()  == true);
	
	
	// Test elements (each)
	
	oAm.SetValue(00000);
	Test(oAm.HasSetuid()   == false);
	Test(oAm.HasSetgid()   == false);
	Test(oAm.HasSticky()   == false);
	Test(oAm.CanUsrRead()  == false);
	Test(oAm.CanUsrWrite() == false);
	Test(oAm.CanUsrExec()  == false);
	Test(oAm.CanGrpRead()  == false);
	Test(oAm.CanGrpWrite() == false);
	Test(oAm.CanGrpExec()  == false);
	Test(oAm.CanOthRead()  == false);
	Test(oAm.CanOthWrite() == false);
	Test(oAm.CanOthExec()  == false);
	
	oAm.SetValue(04000);
	Test(oAm.HasSetuid()   == true);
	Test(oAm.HasSetgid()   == false);
	Test(oAm.HasSticky()   == false);
	Test(oAm.CanUsrRead()  == false);
	Test(oAm.CanUsrWrite() == false);
	Test(oAm.CanUsrExec()  == false);
	Test(oAm.CanGrpRead()  == false);
	Test(oAm.CanGrpWrite() == false);
	Test(oAm.CanGrpExec()  == false);
	Test(oAm.CanOthRead()  == false);
	Test(oAm.CanOthWrite() == false);
	Test(oAm.CanOthExec()  == false);
	
	oAm.SetValue(02000);
	Test(oAm.HasSetuid()   == false);
	Test(oAm.HasSetgid()   == true);
	Test(oAm.HasSticky()   == false);
	Test(oAm.CanUsrRead()  == false);
	Test(oAm.CanUsrWrite() == false);
	Test(oAm.CanUsrExec()  == false);
	Test(oAm.CanGrpRead()  == false);
	Test(oAm.CanGrpWrite() == false);
	Test(oAm.CanGrpExec()  == false);
	Test(oAm.CanOthRead()  == false);
	Test(oAm.CanOthWrite() == false);
	Test(oAm.CanOthExec()  == false);
	
	oAm.SetValue(01000);
	Test(oAm.HasSetuid()   == false);
	Test(oAm.HasSetgid()   == false);
	Test(oAm.HasSticky()   == true);
	Test(oAm.CanUsrRead()  == false);
	Test(oAm.CanUsrWrite() == false);
	Test(oAm.CanUsrExec()  == false);
	Test(oAm.CanGrpRead()  == false);
	Test(oAm.CanGrpWrite() == false);
	Test(oAm.CanGrpExec()  == false);
	Test(oAm.CanOthRead()  == false);
	Test(oAm.CanOthWrite() == false);
	Test(oAm.CanOthExec()  == false);
	
	oAm.SetValue(00400);
	Test(oAm.HasSetuid()   == false);
	Test(oAm.HasSetgid()   == false);
	Test(oAm.HasSticky()   == false);
	Test(oAm.CanUsrRead()  == true);
	Test(oAm.CanUsrWrite() == false);
	Test(oAm.CanUsrExec()  == false);
	Test(oAm.CanGrpRead()  == false);
	Test(oAm.CanGrpWrite() == false);
	Test(oAm.CanGrpExec()  == false);
	Test(oAm.CanOthRead()  == false);
	Test(oAm.CanOthWrite() == false);
	Test(oAm.CanOthExec()  == false);
	
	oAm.SetValue(00200);
	Test(oAm.HasSetuid()   == false);
	Test(oAm.HasSetgid()   == false);
	Test(oAm.HasSticky()   == false);
	Test(oAm.CanUsrRead()  == false);
	Test(oAm.CanUsrWrite() == true);
	Test(oAm.CanUsrExec()  == false);
	Test(oAm.CanGrpRead()  == false);
	Test(oAm.CanGrpWrite() == false);
	Test(oAm.CanGrpExec()  == false);
	Test(oAm.CanOthRead()  == false);
	Test(oAm.CanOthWrite() == false);
	Test(oAm.CanOthExec()  == false);
	
	oAm.SetValue(00100);
	Test(oAm.HasSetuid()   == false);
	Test(oAm.HasSetgid()   == false);
	Test(oAm.HasSticky()   == false);
	Test(oAm.CanUsrRead()  == false);
	Test(oAm.CanUsrWrite() == false);
	Test(oAm.CanUsrExec()  == true);
	Test(oAm.CanGrpRead()  == false);
	Test(oAm.CanGrpWrite() == false);
	Test(oAm.CanGrpExec()  == false);
	Test(oAm.CanOthRead()  == false);
	Test(oAm.CanOthWrite() == false);
	Test(oAm.CanOthExec()  == false);
	
	oAm.SetValue(00040);
	Test(oAm.HasSetuid()   == false);
	Test(oAm.HasSetgid()   == false);
	Test(oAm.HasSticky()   == false);
	Test(oAm.CanUsrRead()  == false);
	Test(oAm.CanUsrWrite() == false);
	Test(oAm.CanUsrExec()  == false);
	Test(oAm.CanGrpRead()  == true);
	Test(oAm.CanGrpWrite() == false);
	Test(oAm.CanGrpExec()  == false);
	Test(oAm.CanOthRead()  == false);
	Test(oAm.CanOthWrite() == false);
	Test(oAm.CanOthExec()  == false);
	
	oAm.SetValue(00020);
	Test(oAm.HasSetuid()   == false);
	Test(oAm.HasSetgid()   == false);
	Test(oAm.HasSticky()   == false);
	Test(oAm.CanUsrRead()  == false);
	Test(oAm.CanUsrWrite() == false);
	Test(oAm.CanUsrExec()  == false);
	Test(oAm.CanGrpRead()  == false);
	Test(oAm.CanGrpWrite() == true);
	Test(oAm.CanGrpExec()  == false);
	Test(oAm.CanOthRead()  == false);
	Test(oAm.CanOthWrite() == false);
	Test(oAm.CanOthExec()  == false);
	
	oAm.SetValue(00010);
	Test(oAm.HasSetuid()   == false);
	Test(oAm.HasSetgid()   == false);
	Test(oAm.HasSticky()   == false);
	Test(oAm.CanUsrRead()  == false);
	Test(oAm.CanUsrWrite() == false);
	Test(oAm.CanUsrExec()  == false);
	Test(oAm.CanGrpRead()  == false);
	Test(oAm.CanGrpWrite() == false);
	Test(oAm.CanGrpExec()  == true);
	Test(oAm.CanOthRead()  == false);
	Test(oAm.CanOthWrite() == false);
	Test(oAm.CanOthExec()  == false);
	
	oAm.SetValue(00004);
	Test(oAm.HasSetuid()   == false);
	Test(oAm.HasSetgid()   == false);
	Test(oAm.HasSticky()   == false);
	Test(oAm.CanUsrRead()  == false);
	Test(oAm.CanUsrWrite() == false);
	Test(oAm.CanUsrExec()  == false);
	Test(oAm.CanGrpRead()  == false);
	Test(oAm.CanGrpWrite() == false);
	Test(oAm.CanGrpExec()  == false);
	Test(oAm.CanOthRead()  == true);
	Test(oAm.CanOthWrite() == false);
	Test(oAm.CanOthExec()  == false);
	
	oAm.SetValue(00002);
	Test(oAm.HasSetuid()   == false);
	Test(oAm.HasSetgid()   == false);
	Test(oAm.HasSticky()   == false);
	Test(oAm.CanUsrRead()  == false);
	Test(oAm.CanUsrWrite() == false);
	Test(oAm.CanUsrExec()  == false);
	Test(oAm.CanGrpRead()  == false);
	Test(oAm.CanGrpWrite() == false);
	Test(oAm.CanGrpExec()  == false);
	Test(oAm.CanOthRead()  == false);
	Test(oAm.CanOthWrite() == true);
	Test(oAm.CanOthExec()  == false);
	
	oAm.SetValue(00001);
	Test(oAm.HasSetuid()   == false);
	Test(oAm.HasSetgid()   == false);
	Test(oAm.HasSticky()   == false);
	Test(oAm.CanUsrRead()  == false);
	Test(oAm.CanUsrWrite() == false);
	Test(oAm.CanUsrExec()  == false);
	Test(oAm.CanGrpRead()  == false);
	Test(oAm.CanGrpWrite() == false);
	Test(oAm.CanGrpExec()  == false);
	Test(oAm.CanOthRead()  == false);
	Test(oAm.CanOthWrite() == false);
	Test(oAm.CanOthExec()  == true);
	
	
	// Test MakeStr()
	
	oAm.SetValue(00000);
	oAm.MakeStr(oPermStr);
	Test(StringCompare(oPermStr, "----------") == 0);
	
	// Access flags
	oAm.SetValue(00001);
	oAm.MakeStr(oPermStr);
	Test(StringCompare(oPermStr, "---------x") == 0);
	oAm.SetValue(00002);
	oAm.MakeStr(oPermStr);
	Test(StringCompare(oPermStr, "--------w-") == 0);
	oAm.SetValue(00004);
	oAm.MakeStr(oPermStr);
	Test(StringCompare(oPermStr, "-------r--") == 0);
	oAm.SetValue(00010);
	oAm.MakeStr(oPermStr);
	Test(StringCompare(oPermStr, "------x---") == 0);
	oAm.SetValue(00020);
	oAm.MakeStr(oPermStr);
	Test(StringCompare(oPermStr, "-----w----") == 0);
	oAm.SetValue(00040);
	oAm.MakeStr(oPermStr);
	Test(StringCompare(oPermStr, "----r-----") == 0);
	oAm.SetValue(00100);
	oAm.MakeStr(oPermStr);
	Test(StringCompare(oPermStr, "---x------") == 0);
	oAm.SetValue(00200);
	oAm.MakeStr(oPermStr);
	Test(StringCompare(oPermStr, "--w-------") == 0);
	oAm.SetValue(00400);
	oAm.MakeStr(oPermStr);
	Test(StringCompare(oPermStr, "-r--------") == 0);
	
	// Upper flags
	oAm.SetValue(01000);
	oAm.MakeStr(oPermStr);
	Test(StringCompare(oPermStr, "---------T") == 0);
	oAm.SetValue(01001);
	oAm.MakeStr(oPermStr);
	Test(StringCompare(oPermStr, "---------t") == 0);
	oAm.SetValue(02000);
	oAm.MakeStr(oPermStr);
	Test(StringCompare(oPermStr, "------S---") == 0);
	oAm.SetValue(02010);
	oAm.MakeStr(oPermStr);
	Test(StringCompare(oPermStr, "------s---") == 0);
	oAm.SetValue(04000);
	oAm.MakeStr(oPermStr);
	Test(StringCompare(oPermStr, "---S------") == 0);
	oAm.SetValue(04100);
	oAm.MakeStr(oPermStr);
	Test(StringCompare(oPermStr, "---s------") == 0);
	
	// Full flags
	oAm.SetValue(00777);
	oAm.MakeStr(oPermStr);
	Test(StringCompare(oPermStr, "-rwxrwxrwx") == 0);
	oAm.SetValue(07000);
	oAm.MakeStr(oPermStr);
	Test(StringCompare(oPermStr, "---S--S--T") == 0);
	oAm.SetValue(07111);
	oAm.MakeStr(oPermStr);
	Test(StringCompare(oPermStr, "---s--s--t") == 0);
	oAm.SetValue(07777);
	oAm.MakeStr(oPermStr);
	Test(StringCompare(oPermStr, "-rwsrwsrwt") == 0);
	
	
	return 0;
}

static int TestBasics()
{
	PrintFn();
	
	
	acpl::Path oPathDfl;
	acpl::Path oPath("abc");
	acpl::Path::String oPstr("pqr");
	acpl::Unichar oUcBfr[4] = { 'm', 'n', 'o', 0 };
	acpl::Path::Stat oStat;
	
	
	// Set constructor
	Test(oPathDfl.IsEmpty() == true);
	Test(oPathDfl.GetStr() != NULL);
	Test(oPathDfl.GetStr()[0] == 0);
	
	
	// Set() methods
	_local_TestPathObj(oPath, "abc");
	oPath.Set(acpl::String::Ref("def", false));
	_local_TestPathObj(oPath, "def");
	oPath.Set(acpl::String::Ref("ghi", true));
	_local_TestPathObj(oPath, "ghi");
	oPath.Set(L"jkl");
	_local_TestPathObj(oPath, "jkl");
	oPath.Set(oUcBfr);
	_local_TestPathObj(oPath, "mno");
	oPath.Set(oPstr);
	_local_TestPathObj(oPath, "pqr");
	
	
	// Apnd() methods
	oPath.Clear();
	Test(oPath.IsEmpty() == true);
	_local_TestPathObj(oPath, "");
	oPath.Apnd(acpl::String::Ref("def", false));
	_local_TestPathObj(oPath, "def");
	oPath.Apnd(acpl::String::Ref("ghi", true));
	_local_TestPathObj(oPath, "defghi");
	oPath.Apnd(L"jkl");
	_local_TestPathObj(oPath, "defghijkl");
	oPath.Apnd(oUcBfr);
	_local_TestPathObj(oPath, "defghijklmno");
	oPath.Apnd(oPstr);
	_local_TestPathObj(oPath, "defghijklmnopqr");
	
	
	return 0;
}

static int TestIsAbsRoot()
{
	PrintFn();
	
	// WARNING: Do not test CTD here!
	
	_local_PexIar oPex[] = {
			_local_PexIarAdd("", false, false)
#if   (acplCRuntime == acplCRuntimeGlibc)
			_local_PexIarAdd("/", true, true)
			_local_PexIarAdd("/tmp", true, false)
			_local_PexIarAdd("tmp", false, false)
#elif (acplCRuntime == acplCRuntimeMscrt)
			_local_PexIarAdd("C:\\", true, true)
			_local_PexIarAdd("C:/", true, true)
			_local_PexIarAdd("\\", true, true)
			_local_PexIarAdd("/", true, true)
			_local_PexIarAdd("\\\\server", true, true)
			
			_local_PexIarAdd("C:\\tmp", true, false)
			_local_PexIarAdd("C:/tmp", true, false)
			_local_PexIarAdd("\\tmp", true, false)
			_local_PexIarAdd("/tmp", true, false)
			_local_PexIarAdd("\\\\server\\tmp", true, false)
			
			_local_PexIarAdd("tmp", false, false)
			_local_PexIarAdd("C:tmp", false, false)
			_local_PexIarAdd("C:", false, false)
#else
#	error Add platform specific tests.
#endif
			_local_PexIarEnd()
		};
	
	
	Test(_local_TestPexIar(oPex) == 0);
		
	
	return 0;
}

static int TestCutTrailDelims()
{
	PrintFn();
	
	
	_local_PexCtd oPex[] = {
#if   (acplCRuntime == acplCRuntimeGlibc)
			_local_PexCtdAdd("some/path/that/makes/us/happy", "some/path/that/makes/us/happy")
			_local_PexCtdAdd("some/path/that/makes/us/happy/", "some/path/that/makes/us/happy")
			_local_PexCtdAdd("some/path/that/makes/us/happy//////", "some/path/that/makes/us/happy")
			
			_local_PexCtdAdd("///", "/")
			_local_PexCtdAdd("/", "/")
#elif (acplCRuntime == acplCRuntimeMscrt)
			_local_PexCtdAdd("some\\path\\that\\makes\\us\\happy", "some\\path\\that\\makes\\us\\happy")
			_local_PexCtdAdd("some\\path\\that\\makes\\us\\happy\\", "some\\path\\that\\makes\\us\\happy")
			_local_PexCtdAdd("some\\path\\that\\makes\\us\\happy\\/\\/\\\\", "some\\path\\that\\makes\\us\\happy")
			
			_local_PexCtdAdd("\\/\\", "\\")
			_local_PexCtdAdd("/\\/", "/")
			_local_PexCtdAdd("\\", "\\")
			_local_PexCtdAdd("/", "/")
			
			_local_PexCtdAdd("C:\\/\\", "C:\\")
			_local_PexCtdAdd("C:/\\/", "C:/")
			_local_PexCtdAdd("C:\\", "C:\\")
			_local_PexCtdAdd("C:/", "C:/")
			
			_local_PexCtdAdd("\\\\", "\\") // "\\" is not a valid root dir
			_local_PexCtdAdd("\\\\server", "\\\\server")
			_local_PexCtdAdd("\\\\server\\", "\\\\server")
			_local_PexCtdAdd("\\\\server\\/\\/\\\\", "\\\\server")
#else
#	error Add platform specific tests.
#endif
			_local_PexCtdEnd()
		};
	
	
	Test(_local_TestPexCtd(oPex) == 0);
	
	
	return 0;
}

static int TestNavigators()
{
	PrintFn();
	
	
	acpl::Path oPath;
	acpl::Path::String oPstr("subchild");
	
	
#if   (acplCRuntime == acplCRuntimeGlibc)
	
	// Current()
	oPath.Clear().Current(); // if path is empty, it will add a current dir sign (".")
	_local_TestPathObj(oPath, ".");
	oPath.Current(); // if path is not empty, adding a delimiter + current dir sign ("/.") would be quite pointless
	_local_TestPathObj(oPath, ".");
	
	// Parent()
	oPath.Clear().Parent(); // if path is empty, it will add just a parent dir ("..")
	_local_TestPathObj(oPath, "..");
	oPath.Set("/").Parent(); // if path is a root dir, it will not add anything (as there is no parent dir of a root dir)
	_local_TestPathObj(oPath, "/");
	oPath.Set("aaa/bbb").Parent();
	_local_TestPathObj(oPath, "aaa");
	oPath.Set("/aaa").Parent();
	_local_TestPathObj(oPath, "/");
	oPath.Set("aaa///bbb").Parent(); // will remove excess delimiters
	_local_TestPathObj(oPath, "aaa");
	oPath.Set("///aaa").Parent();
	_local_TestPathObj(oPath, "/");
	oPath.Set(".").Parent(); // will just replace current dir with parent dir
	_local_TestPathObj(oPath, "..");
	oPath.Set("..").Parent();
	_local_TestPathObj(oPath, "../..");
	oPath.Set("aaa").Parent(); // if path consists of only one non-root element, that element is removed and replaced with current dir (".")
	_local_TestPathObj(oPath, ".");
	
	// Child()
	oPath.Clear().Child(""); // if path is empty and provided child in empty as well, the path will remain empty
	_local_TestPathObj(oPath, "");
	oPath.Child("child"); // if path is empty, it will add just [child] element
	_local_TestPathObj(oPath, "child");
	oPath.Child("subchild"); // adding a child on a non-empty path will result in adding of a current dir, a delimiter, and [child]
	_local_TestPathObj(oPath, "child/subchild");
	oPath.Set("/").Child("child"); // root dir child adds
	_local_TestPathObj(oPath, "/child");
	oPath.Child(oPstr);
	_local_TestPathObj(oPath, "/child/subchild");
	
#elif (acplCRuntime == acplCRuntimeMscrt)
	
	// Current()
	oPath.Clear().Current(); // if path is empty, it will add a current dir sign (".")
	_local_TestPathObj(oPath, ".");
	oPath.Current(); // if path is not empty, adding a delimiter + current dir sign ("/.") would be quite pointless
	_local_TestPathObj(oPath, ".");
	
	// Parent()
	oPath.Clear().Parent(); // if path is empty, it will add just a parent dir ("..")
	_local_TestPathObj(oPath, "..");
	oPath.Set("C:\\").Parent(); // if path is a root dir, it will not add anything (as there is no parent dir of a root dir)
	_local_TestPathObj(oPath, "C:\\");
	oPath.Set("\\\\server\\").Parent(); // the same test, only for "\\[server]" root dir
	_local_TestPathObj(oPath, "\\\\server");
	oPath.Set("aaa\\bbb").Parent();
	_local_TestPathObj(oPath, "aaa");
	oPath.Set("C:\\aaa").Parent();
	_local_TestPathObj(oPath, "C:\\");
	oPath.Set("\\\\server\\aaa").Parent();
	_local_TestPathObj(oPath, "\\\\server");
	oPath.Set("aaa\\/\\bbb").Parent(); // will remove excess delimiters
	_local_TestPathObj(oPath, "aaa");
	oPath.Set("C:\\/\\aaa").Parent();
	_local_TestPathObj(oPath, "C:\\");
	oPath.Set("\\\\server\\/\\aaa").Parent();
	_local_TestPathObj(oPath, "\\\\server");
	oPath.Set(".").Parent(); // will just replace current dir with parent dir
	_local_TestPathObj(oPath, "..");
	oPath.Set("..").Parent();
	_local_TestPathObj(oPath, "..\\..");
	oPath.Set("aaa").Parent(); // if path consists of only one non-root element, that element is removed and replaced with current dir (".")
	_local_TestPathObj(oPath, ".");
	oPath.Set("C:aaa\\bbb").Parent(); // relative path
	_local_TestPathObj(oPath, "C:aaa");
	oPath.Set("C:..").Parent(); // relative path with only parent dir element
	_local_TestPathObj(oPath, "C:..\\..");
	oPath.Set("C:.").Parent(); // relative path with only current dir element
	_local_TestPathObj(oPath, "C:..");
	oPath.Set("C:aaa").Parent(); // relative path with one named element
	_local_TestPathObj(oPath, "C:.");
	
	// Child()
	oPath.Clear().Child(""); // if path is empty and provided child in empty as well, the path will remain empty
	_local_TestPathObj(oPath, "");
	oPath.Child("child"); // if path is empty, it will add just [child] element
	_local_TestPathObj(oPath, "child");
	oPath.Child("subchild"); // adding a child on a non-empty path will result in adding of a current dir, a delimiter, and [child]
	_local_TestPathObj(oPath, "child\\subchild");
	oPath.Set("C:\\").Child("child"); // root dir child adds
	_local_TestPathObj(oPath, "C:\\child");
	oPath.Set("C:/").Child("child");
	_local_TestPathObj(oPath, "C:/child");
	oPath.Set("\\").Child("child");
	_local_TestPathObj(oPath, "\\child");
	oPath.Set("/").Child("child");
	_local_TestPathObj(oPath, "/child");
	oPath.Child(oPstr);
	_local_TestPathObj(oPath, "/child\\subchild");
	oPath.Set("\\\\server").Child("child");
	_local_TestPathObj(oPath, "\\\\server\\child");
	oPath.Set("C:").Child("child"); // child of the relative path including the drive designator
	_local_TestPathObj(oPath, "C:child");
	
#else
#	error Add platform specific tests.
#endif
		
	
	return 0;
}

static int TestGetPathParts()
{
	PrintFn();
	
	
	acpl::Path oPath;
	acpl::Path::String oRes;
	
#if   (acplCRuntime == acplCRuntimeGlibc)
	
	// GetDirName()
	oPath.Set("").GetDirName(oRes);
	_local_TestStringObj(oRes, ".");
	oPath.Set(".").GetDirName(oRes);
	_local_TestStringObj(oRes, ".");
	oPath.Set("..").GetDirName(oRes);
	_local_TestStringObj(oRes, ".");
	oPath.Set("aaa").GetDirName(oRes);
	_local_TestStringObj(oRes, ".");
	oPath.Set("aaa/bbb").GetDirName(oRes);
	_local_TestStringObj(oRes, "aaa");
	oPath.Set("aaa/bbb/ccc").GetDirName(oRes);
	_local_TestStringObj(oRes, "aaa/bbb");
	oPath.Set("aaa/bbb///ccc").GetDirName(oRes);
	_local_TestStringObj(oRes, "aaa/bbb");
	oPath.Set("/aaa").GetDirName(oRes);
	_local_TestStringObj(oRes, "/");
	oPath.Set("///aaa").GetDirName(oRes);
	_local_TestStringObj(oRes, "/");
	oPath.Set("/aaa/bbb").GetDirName(oRes);
	_local_TestStringObj(oRes, "/aaa");
	oPath.Set("/aaa/bbb/ccc").GetDirName(oRes);
	_local_TestStringObj(oRes, "/aaa/bbb");
	oPath.Set("/aaa/bbb///ccc").GetDirName(oRes);
	_local_TestStringObj(oRes, "/aaa/bbb");
	oPath.Set("../aaa").GetDirName(oRes);
	_local_TestStringObj(oRes, "..");
	
	// GetBaseName()
	oPath.Set("").GetBaseName(oRes);
	_local_TestStringObj(oRes, "");
	oPath.Set(".").GetBaseName(oRes);
	_local_TestStringObj(oRes, ".");
	oPath.Set("..").GetBaseName(oRes);
	_local_TestStringObj(oRes, "..");
	oPath.Set("aaa").GetBaseName(oRes);
	_local_TestStringObj(oRes, "aaa");
	oPath.Set("aaa/bbb").GetBaseName(oRes);
	_local_TestStringObj(oRes, "bbb");
	oPath.Set("aaa/bbb/ccc").GetBaseName(oRes);
	_local_TestStringObj(oRes, "ccc");
	oPath.Set("aaa/bbb///ccc").GetBaseName(oRes);
	_local_TestStringObj(oRes, "ccc");
	oPath.Set("/").GetBaseName(oRes); // absolute path
	_local_TestStringObj(oRes, "/");
	oPath.Set("/aaa").GetBaseName(oRes);
	_local_TestStringObj(oRes, "aaa");
	oPath.Set("///aaa").GetBaseName(oRes);
	_local_TestStringObj(oRes, "aaa");
	oPath.Set("/aaa/bbb").GetBaseName(oRes);
	_local_TestStringObj(oRes, "bbb");
	oPath.Set("/aaa/bbb/ccc").GetBaseName(oRes);
	_local_TestStringObj(oRes, "ccc");
	oPath.Set("/aaa/bbb///ccc").GetBaseName(oRes);
	_local_TestStringObj(oRes, "ccc");
	oPath.Set("../aaa").GetBaseName(oRes);
	_local_TestStringObj(oRes, "aaa");
	
	// GetFileName() && GetFileExt() - other tests are universal
	oPath.Set(".");
	oPath.GetFileName(oRes);
	_local_TestStringObj(oRes, ".");
	oPath.GetFileExt(oRes);
	_local_TestStringObj(oRes, "");
	oPath.Set("..");
	oPath.GetFileName(oRes);
	_local_TestStringObj(oRes, "..");
	oPath.GetFileExt(oRes);
	_local_TestStringObj(oRes, "");
	oPath.Set("///");
	oPath.GetFileName(oRes);
	_local_TestStringObj(oRes, "/");
	oPath.GetFileExt(oRes);
	_local_TestStringObj(oRes, "");
	oPath.Set("///aaa.bbb");
	oPath.GetFileName(oRes);
	_local_TestStringObj(oRes, "aaa");
	oPath.GetFileExt(oRes);
	_local_TestStringObj(oRes, "bbb");
	
#elif (acplCRuntime == acplCRuntimeMscrt)
	
	// GetDirName()
	oPath.Set("").GetDirName(oRes);
	_local_TestStringObj(oRes, ".");
	oPath.Set(".").GetDirName(oRes);
	_local_TestStringObj(oRes, ".");
	oPath.Set("..").GetDirName(oRes);
	_local_TestStringObj(oRes, ".");
	oPath.Set("aaa").GetDirName(oRes);
	_local_TestStringObj(oRes, ".");
	oPath.Set("aaa\\bbb").GetDirName(oRes);
	_local_TestStringObj(oRes, "aaa");
	oPath.Set("aaa\\bbb\\ccc").GetDirName(oRes);
	_local_TestStringObj(oRes, "aaa\\bbb");
	oPath.Set("aaa\\bbb\\/\\ccc").GetDirName(oRes);
	_local_TestStringObj(oRes, "aaa\\bbb");
	oPath.Set("C:\\").GetDirName(oRes);
	_local_TestStringObj(oRes, "C:\\");
	oPath.Set("C:/").GetDirName(oRes);
	_local_TestStringObj(oRes, "C:/");
	oPath.Set("\\").GetDirName(oRes);
	_local_TestStringObj(oRes, "\\");
	oPath.Set("/").GetDirName(oRes);
	_local_TestStringObj(oRes, "/");
	oPath.Set("\\\\server").GetDirName(oRes);
	_local_TestStringObj(oRes, "\\\\server");
	oPath.Set("C:\\aaa").GetDirName(oRes);
	_local_TestStringObj(oRes, "C:\\");
	oPath.Set("C:/aaa").GetDirName(oRes);
	_local_TestStringObj(oRes, "C:/");
	oPath.Set("\\aaa").GetDirName(oRes);
	_local_TestStringObj(oRes, "\\");
	oPath.Set("/aaa").GetDirName(oRes);
	_local_TestStringObj(oRes, "/");
	oPath.Set("\\\\server\\aaa").GetDirName(oRes);
	_local_TestStringObj(oRes, "\\\\server");
	oPath.Set("C:\\/\\aaa").GetDirName(oRes);
	_local_TestStringObj(oRes, "C:\\");
	oPath.Set("\\/\\aaa").GetDirName(oRes);
	_local_TestStringObj(oRes, "\\");
	oPath.Set("\\\\server\\/\\aaa").GetDirName(oRes);
	_local_TestStringObj(oRes, "\\\\server");
	oPath.Set("C:\\aaa\\bbb").GetDirName(oRes);
	_local_TestStringObj(oRes, "C:\\aaa");
	oPath.Set("C:\\aaa\\bbb\\ccc").GetDirName(oRes);
	_local_TestStringObj(oRes, "C:\\aaa\\bbb");
	oPath.Set("C:\\aaa\\bbb\\/\\ccc").GetDirName(oRes);
	_local_TestStringObj(oRes, "C:\\aaa\\bbb");
	oPath.Set("..\\aaa").GetDirName(oRes);
	_local_TestStringObj(oRes, "..");
	oPath.Set("C:").GetDirName(oRes);
	_local_TestStringObj(oRes, "C:.");
	oPath.Set("C:aaa").GetDirName(oRes);
	_local_TestStringObj(oRes, "C:.");
	oPath.Set("C:aaa\\bbb").GetDirName(oRes);
	_local_TestStringObj(oRes, "C:aaa");
	
	// GetBaseName()
	oPath.Set("").GetBaseName(oRes);
	_local_TestStringObj(oRes, "");
	oPath.Set(".").GetBaseName(oRes);
	_local_TestStringObj(oRes, ".");
	oPath.Set("..").GetBaseName(oRes);
	_local_TestStringObj(oRes, "..");
	oPath.Set("aaa").GetBaseName(oRes);
	_local_TestStringObj(oRes, "aaa");
	oPath.Set("aaa\\bbb").GetBaseName(oRes);
	_local_TestStringObj(oRes, "bbb");
	oPath.Set("aaa\\bbb\\ccc").GetBaseName(oRes);
	_local_TestStringObj(oRes, "ccc");
	oPath.Set("aaa\\bbb\\/\\ccc").GetBaseName(oRes);
	_local_TestStringObj(oRes, "ccc");
	oPath.Set("C:\\").GetBaseName(oRes); // absolute path
	_local_TestStringObj(oRes, "C:\\");
	oPath.Set("C:/").GetBaseName(oRes);
	_local_TestStringObj(oRes, "C:/");
	oPath.Set("\\").GetBaseName(oRes);
	_local_TestStringObj(oRes, "\\");
	oPath.Set("/").GetBaseName(oRes);
	_local_TestStringObj(oRes, "/");
	oPath.Set("\\\\server").GetBaseName(oRes);
	_local_TestStringObj(oRes, "\\\\server");
	oPath.Set("C:\\aaa").GetBaseName(oRes);
	_local_TestStringObj(oRes, "aaa");
	oPath.Set("\\aaa").GetBaseName(oRes);
	_local_TestStringObj(oRes, "aaa");
	oPath.Set("C:\\/\\aaa").GetBaseName(oRes);
	_local_TestStringObj(oRes, "aaa");
	oPath.Set("\\/\\aaa").GetBaseName(oRes);
	_local_TestStringObj(oRes, "aaa");
	oPath.Set("\\\\server\\aaa").GetBaseName(oRes);
	_local_TestStringObj(oRes, "aaa");
	oPath.Set("\\\\server\\/\\aaa").GetBaseName(oRes);
	_local_TestStringObj(oRes, "aaa");
	oPath.Set("C:\\aaa\\bbb").GetBaseName(oRes);
	_local_TestStringObj(oRes, "bbb");
	oPath.Set("C:\\aaa\\bbb\\ccc").GetBaseName(oRes);
	_local_TestStringObj(oRes, "ccc");
	oPath.Set("C:\\aaa\\bbb\\/\\ccc").GetBaseName(oRes);
	_local_TestStringObj(oRes, "ccc");
	oPath.Set("..\\aaa").GetBaseName(oRes);
	_local_TestStringObj(oRes, "aaa");
	oPath.Set("C:").GetBaseName(oRes);
	_local_TestStringObj(oRes, ".");
	oPath.Set("C:aaa").GetBaseName(oRes);
	_local_TestStringObj(oRes, "aaa");
	oPath.Set("C:aaa\\bbb").GetBaseName(oRes);
	_local_TestStringObj(oRes, "bbb");
	
	// GetFileName() && GetFileExt() - other tests are universal
	oPath.Set(".");
	oPath.GetFileName(oRes);
	_local_TestStringObj(oRes, ".");
	oPath.GetFileExt(oRes);
	_local_TestStringObj(oRes, "");
	oPath.Set("..");
	oPath.GetFileName(oRes);
	_local_TestStringObj(oRes, "..");
	oPath.GetFileExt(oRes);
	_local_TestStringObj(oRes, "");
	oPath.Set("\\\\\\");
	oPath.GetFileName(oRes);
	_local_TestStringObj(oRes, "\\");
	oPath.GetFileExt(oRes);
	_local_TestStringObj(oRes, "");
	oPath.Set("C:\\");
	oPath.GetFileName(oRes);
	_local_TestStringObj(oRes, "C:\\");
	oPath.GetFileExt(oRes);
	_local_TestStringObj(oRes, "");
	oPath.Set("\\\\server");
	oPath.GetFileName(oRes);
	_local_TestStringObj(oRes, "\\\\server");
	oPath.GetFileExt(oRes);
	_local_TestStringObj(oRes, "");
	oPath.Set("C:\\aaa.bbb");
	oPath.GetFileName(oRes);
	_local_TestStringObj(oRes, "aaa");
	oPath.GetFileExt(oRes);
	_local_TestStringObj(oRes, "bbb");
	oPath.Set("\\\\server.aaa"); // very special case
	oPath.GetFileName(oRes);
	_local_TestStringObj(oRes, "\\\\server.aaa");
	oPath.GetFileExt(oRes);
	_local_TestStringObj(oRes, "");
	
#else
#	error Add platform specific tests.
#endif
	
	
	// GetFileName() && GetFileExt() - universal tests
	oPath.Set("");
	oPath.GetFileName(oRes);
	_local_TestStringObj(oRes, "");
	oPath.GetFileExt(oRes);
	_local_TestStringObj(oRes, "");
	oPath.Set(".....");
	oPath.GetFileName(oRes);
	_local_TestStringObj(oRes, ".....");
	oPath.GetFileExt(oRes);
	_local_TestStringObj(oRes, "");
	oPath.Set(".aaa");
	oPath.GetFileName(oRes);
	_local_TestStringObj(oRes, ".aaa");
	oPath.GetFileExt(oRes);
	_local_TestStringObj(oRes, "");
	oPath.Set("..aaa");
	oPath.GetFileName(oRes);
	_local_TestStringObj(oRes, ".");
	oPath.GetFileExt(oRes);
	_local_TestStringObj(oRes, "aaa");
	oPath.Set(".aaa.");
	oPath.GetFileName(oRes);
	_local_TestStringObj(oRes, ".aaa.");
	oPath.GetFileExt(oRes);
	_local_TestStringObj(oRes, "");
	oPath.Set(".aaa.bbb");
	oPath.GetFileName(oRes);
	_local_TestStringObj(oRes, ".aaa");
	oPath.GetFileExt(oRes);
	_local_TestStringObj(oRes, "bbb");
		
	
	return 0;
}

static int TestResolve()
{
	PrintFn();
	
	
	acpl::Path oPath;
	
	
	// Init
	Test(oPath.GetLastError() == acpl::Path::leNone);
	
	// Empty path
	Test(oPath.Resolve() == false);
	Test(oPath.GetLastError() == acpl::Path::leIsEmpty);
	
	// Non-existant path
	oPath.Set("_nonexistantpath");
	Test(oPath.Resolve() == false);
	Test(oPath.GetLastError() == acpl::Path::leNonExist);
	
	// This exe file
	oPath.Set(acpl::String::Ref(gArgv0, true));
	Test(oPath.Resolve() == true);
	
	// (Obviously) non-existant "subdir" of exe file
	oPath.Child("_nonexistantpath");
	Test(oPath.Resolve() == false);
	Test(oPath.GetLastError() == acpl::Path::leNotDir);
	
	// This exe's parent dir
	oPath.Parent().Parent();
	Test(oPath.Resolve() == true);
	
	// Error Reset
	oPath.ResetLastError();
	Test(oPath.GetLastError() == acpl::Path::leNone);
	
	
	return 0;
}

static int TestStatBase()
{
	PrintFn();
	
	
	_local_StatBase_Ext oSb;
	acpl::Path::StatBase::InfoStat &oIs = oSb.GetInfoStat();
	acpl::Path::Permission::Str oPermStr;
	
	
	// Type tests
	
	Test(acpl::Path::StatBase::tUnknown == 0);
	Test(acpl::Path::StatBase::tFile == 1);
	Test(acpl::Path::StatBase::tDir == 2);
	Test(acpl::Path::StatBase::tSymLink == 3);
	Test(acpl::Path::StatBase::tDevBlock == 4);
	Test(acpl::Path::StatBase::tDevChar == 5);
	Test(acpl::Path::StatBase::tSocket == 6);
	Test(acpl::Path::StatBase::tPipe == 7);
	
	
	// Init tests
	
	Test(oSb.IsStatted() == false);
	Test(oSb.GetType() == acpl::Path::StatBase::tUnknown);
	Test(oSb.GetPerm().GetValue() == 0);
	Test(oSb.GetSize() == 0);
	Test(oSb.GetTimeCre() == 0);
	Test(oSb.GetTimeMod() == 0);
	Test(oSb.GetTimeAcc() == 0);
	
	
	// Increase fields
	
	oIs.sIsSet = true;
	oIs.sType = acpl::Path::StatBase::tFile;
	oIs.sPerm.SetValue(1);
	oIs.sSize++;
	oIs.sTimeCre++;
	oIs.sTimeMod++;
	oIs.sTimeAcc++;
	
	
	// Test increased fields
	
	Test(oSb.IsStatted() == true);
	Test(oSb.GetType() == acpl::Path::StatBase::tFile);
	Test(oSb.GetPerm().GetValue() == 1);
	Test(oSb.GetSize() == 1);
	Test(oSb.GetTimeCre() == 1);
	Test(oSb.GetTimeMod() == 1);
	Test(oSb.GetTimeAcc() == 1);
	
	
	// Clear
	
	oSb.Clear();
	
	
	// Test cleared fields
	
	Test(oSb.IsStatted() == false);
	Test(oSb.GetType() == acpl::Path::StatBase::tUnknown);
	Test(oSb.GetPerm().GetValue() == 0);
	Test(oSb.GetSize() == 0);
	Test(oSb.GetTimeCre() == 0);
	Test(oSb.GetTimeMod() == 0);
	Test(oSb.GetTimeAcc() == 0);
	
	
	// Test MakePermStr()
	
	oSb.MakePermStr(oPermStr);
	Test(oSb.GetType() == acpl::Path::StatBase::tUnknown);
	Test(StringCompare(oPermStr, "?---------") == 0);
	oIs.sType = acpl::Path::StatBase::tFile;
	oSb.MakePermStr(oPermStr);
	Test(oSb.GetType() == acpl::Path::StatBase::tFile);
	Test(StringCompare(oPermStr, "----------") == 0);
	oIs.sType = acpl::Path::StatBase::tDir;
	oSb.MakePermStr(oPermStr);
	Test(oSb.GetType() == acpl::Path::StatBase::tDir);
	Test(StringCompare(oPermStr, "d---------") == 0);
	oIs.sType = acpl::Path::StatBase::tSymLink;
	oSb.MakePermStr(oPermStr);
	Test(oSb.GetType() == acpl::Path::StatBase::tSymLink);
	Test(StringCompare(oPermStr, "l---------") == 0);
	oIs.sType = acpl::Path::StatBase::tDevBlock;
	oSb.MakePermStr(oPermStr);
	Test(oSb.GetType() == acpl::Path::StatBase::tDevBlock);
	Test(StringCompare(oPermStr, "b---------") == 0);
	oIs.sType = acpl::Path::StatBase::tDevChar;
	oSb.MakePermStr(oPermStr);
	Test(oSb.GetType() == acpl::Path::StatBase::tDevChar);
	Test(StringCompare(oPermStr, "c---------") == 0);
	oIs.sType = acpl::Path::StatBase::tSocket;
	oSb.MakePermStr(oPermStr);
	Test(oSb.GetType() == acpl::Path::StatBase::tSocket);
	Test(StringCompare(oPermStr, "s---------") == 0);
	oIs.sType = acpl::Path::StatBase::tPipe;
	oSb.MakePermStr(oPermStr);
	Test(oSb.GetType() == acpl::Path::StatBase::tPipe);
	Test(StringCompare(oPermStr, "p---------") == 0);
	oIs.sType = acpl::Path::StatBase::tUnknown;
	oSb.MakePermStr(oPermStr);
	Test(oSb.GetType() == acpl::Path::StatBase::tUnknown);
	Test(StringCompare(oPermStr, "?---------") == 0); // leave it at unknown
	
	oIs.sPerm.SetValue(0001);
	oSb.MakePermStr(oPermStr);
	Test(StringCompare(oPermStr, "?--------x") == 0);
	oIs.sPerm.SetValue(0002);
	oSb.MakePermStr(oPermStr);
	Test(StringCompare(oPermStr, "?-------w-") == 0);
	oIs.sPerm.SetValue(0004);
	oSb.MakePermStr(oPermStr);
	Test(StringCompare(oPermStr, "?------r--") == 0);
	oIs.sPerm.SetValue(0010);
	oSb.MakePermStr(oPermStr);
	Test(StringCompare(oPermStr, "?-----x---") == 0);
	oIs.sPerm.SetValue(0020);
	oSb.MakePermStr(oPermStr);
	Test(StringCompare(oPermStr, "?----w----") == 0);
	oIs.sPerm.SetValue(0040);
	oSb.MakePermStr(oPermStr);
	Test(StringCompare(oPermStr, "?---r-----") == 0);
	oIs.sPerm.SetValue(0100);
	oSb.MakePermStr(oPermStr);
	Test(StringCompare(oPermStr, "?--x------") == 0);
	oIs.sPerm.SetValue(0200);
	oSb.MakePermStr(oPermStr);
	Test(StringCompare(oPermStr, "?-w-------") == 0);
	oIs.sPerm.SetValue(0400);
	oSb.MakePermStr(oPermStr);
	Test(StringCompare(oPermStr, "?r--------") == 0);
	oIs.sPerm.SetValue(0777);
	oSb.MakePermStr(oPermStr);
	Test(StringCompare(oPermStr, "?rwxrwxrwx") == 0);
	
	
	return 0;
}

static int TestStat()
{
	PrintFn();
	
	
	acpl::Path oPath;
	acpl::Path::Stat oStat;
	
	
	// Init
	Test(oStat.GetLastError() == acpl::Path::leNone);
	Test(oStat.IsStatted() == false);
	Test(oStat.GetType() == acpl::Path::StatBase::tUnknown);
	Test(oStat.GetPerm().GetValue() == 0);
	Test(oStat.GetSize() == 0);
	Test(oStat.GetTimeCre() == 0);
	Test(oStat.GetTimeMod() == 0);
	Test(oStat.GetTimeAcc() == 0);
	
	
	// Stat empty path
	Test(oStat.DoStat(oPath, true) == false);
	Test(oStat.GetLastError() == acpl::Path::leIsEmpty);
	Test(oStat.IsStatted() == false);
	Test(oStat.GetType() == acpl::Path::StatBase::tUnknown);
	Test(oStat.GetPerm().GetValue() == 0);
	Test(oStat.GetSize() == 0);
	Test(oStat.GetTimeCre() == 0);
	Test(oStat.GetTimeMod() == 0);
	Test(oStat.GetTimeAcc() == 0);
	
	
	// Stat non-existant path
	oPath.Set("_nonexistantpath");
	Test(oStat.DoStat(oPath, true) == false);
	Test(oStat.GetLastError() == acpl::Path::leNonExist);
	Test(oStat.IsStatted() == false);
	Test(oStat.GetType() == acpl::Path::StatBase::tUnknown);
	Test(oStat.GetPerm().GetValue() == 0);
	Test(oStat.GetSize() == 0);
	Test(oStat.GetTimeCre() == 0);
	Test(oStat.GetTimeMod() == 0);
	Test(oStat.GetTimeAcc() == 0);
	
	
	// Stat this exe file
	oPath.Set(acpl::String::Ref(gArgv0, true));
	Test(oStat.DoStat(oPath, true) == true);
	Test(oStat.IsStatted() == true);
	Test(oStat.GetType() == acpl::Path::StatBase::tFile);
	Test(oStat.GetPerm().CanUsrRead() == true && oStat.GetPerm().CanUsrExec() == true); // we can only be sure that this exe is readable and executable by the user
	Test(oStat.GetSize() > 0);
	Test(oStat.GetTimeCre() > 0);
	Test(oStat.GetTimeMod() > 0);
	Test(oStat.GetTimeAcc() > 0);
	
	
	// Stat (obviously) non-existant "subdir" of exe file
	oPath.Child("_nonexistantpath");
	Test(oStat.DoStat(oPath, true) == false);
	Test(oStat.GetLastError() == acpl::Path::leNotDir);
	Test(oStat.IsStatted() == false);
	Test(oStat.GetType() == acpl::Path::StatBase::tUnknown);
	Test(oStat.GetPerm().GetValue() == 0);
	Test(oStat.GetSize() == 0);
	Test(oStat.GetTimeCre() == 0);
	Test(oStat.GetTimeMod() == 0);
	Test(oStat.GetTimeAcc() == 0);
	
	
	// Stat this exe's parent dir
	oPath.Parent().Parent();
	Test(oStat.DoStat(oPath, true) == true);
	Test(oStat.IsStatted() == true);
	Test(oStat.GetType() == acpl::Path::StatBase::tDir);
	Test(oStat.GetPerm().CanUsrRead() == true && oStat.GetPerm().CanUsrExec() == true); // we can only be sure that this exe's parent dir is readable and executable by the user
	// Cannot test size as it is different on different platforms (DIR in MSCRT is 0)
	Test(oStat.GetTimeCre() > 0);
	Test(oStat.GetTimeMod() > 0);
	Test(oStat.GetTimeAcc() > 0);
	
	
	// Error Reset
	oStat.ResetLastError();
	Test(oStat.GetLastError() == acpl::Path::leNone);
	
	
	return 0;
}


static int TestResolveCustom(const char *nRelPath)
{
	acpl::Path oPath(acpl::String::Ref(nRelPath, true));
	
	PrintOut("RelPath=" _local_PathcharFmt "\nAbsPath=", oPath.GetStr());
	
	if (oPath.Resolve() == true)
		PrintOut("" _local_PathcharFmt "\n", oPath.GetStr())
	else
		PrintOut("[error]\n")
	
	return 0;
}

static const char *_local_Stat_TypeStr(acpl::Path::StatBase::Type nType)
{
	switch (nType)
	{
		case acpl::Path::StatBase::tUnknown:
			return "(unknown)";
			
		case acpl::Path::StatBase::tFile:
			return "regular file";
		
		case acpl::Path::StatBase::tDir:
			return "directory";
		
		case acpl::Path::StatBase::tSymLink:
			return "symbolic link";
		
		case acpl::Path::StatBase::tDevBlock:
			return "block device";
		
		case acpl::Path::StatBase::tDevChar:
			return "character device";
		
		case acpl::Path::StatBase::tSocket:
			return "UNIX domain socket";
		
		case acpl::Path::StatBase::tPipe:
			return "named pipe (FIFO)";
	}
	
	return "(fail)";
}
static int PrintStat(const char *nPath, bool nResolveLink)
{
	acpl::Path oPath(acpl::String::Ref(nPath, true));
	acpl::Path::Stat oStat;
	acpl::Path::Permission::Str oPermStr;
	static const char oDtFmt[] = "%Y-%m-%d %H:%M:%S";
	acpl::String::FmtDtRef oDtfCre(oDtFmt), oDtfMod(oDtFmt), oDtfAcc(oDtFmt);
	acpl::DateTime oDt(true);
	
	
	Test(oStat.DoStat(oPath, nResolveLink));
	
	Test(oPath.Resolve() == true);
	
	oStat.MakePermStr(oPermStr);
	
	oDt.SetUnixTime(oStat.GetTimeCre());
	oDtfCre.Format(oDt);
	oDt.SetUnixTime(oStat.GetTimeMod());
	oDtfMod.Format(oDt);
	oDt.SetUnixTime(oStat.GetTimeAcc());
	oDtfAcc.Format(oDt);
	
	PrintOut(
			"%sstat() info:\n"
			"Path .......  : %s\n"
			"Resolved ...  : " _local_PathcharFmt "\n"
			"Type .......  : %s\n"
			"Access .....  : 0%03o (%s)\n"
			"Size .......  : %" _local_Int64FmtPx "d bytes\n"
			"Time created  : %s (%" _local_Int64FmtPx "d)\n"
			"Time modified : %s (%" _local_Int64FmtPx "d)\n"
			"Time accessed : %s (%" _local_Int64FmtPx "d)\n",
			
			((nResolveLink == true) ? "" : "l"),
			 nPath,
			 oPath.GetStr(),
			 _local_Stat_TypeStr(oStat.GetType()),
			 oStat.GetPerm().GetValue(),
			 oPermStr,
			 oStat.GetSize(),
			 oDtfCre.Utf8(),
			 oStat.GetTimeCre(),
			 oDtfMod.Utf8(),
			 oStat.GetTimeMod(),
			 oDtfAcc.Utf8(),
			 oStat.GetTimeAcc()
		);
	
	
	return 0;
}


SectionFuncMain(fspath)
{
	if (argc < 1)
	{
		Test(TestString() == 0);
		Test(TestPerm() == 0);
		Test(TestBasics() == 0);
		Test(TestIsAbsRoot() == 0);
		Test(TestCutTrailDelims() == 0);
		Test(TestNavigators() == 0);
		Test(TestGetPathParts() == 0);
		Test(TestResolve() == 0);
		Test(TestStatBase() == 0);
		Test(TestStat() == 0);
		
		PrintOut("All Passed!\n");
	}
	else
	if (argc == 2 && StringCompare(argv[0], "abs") == 0)
	{
		Test(TestResolveCustom(argv[1]) == 0);
	}
	else
	if (argc == 2 && StringCompare(argv[0], "stat") == 0)
	{
		Test(PrintStat(argv[1], true) == 0);
	}
	else
	if (argc == 2 && StringCompare(argv[0], "lstat") == 0)
	{
		Test(PrintStat(argv[1], false) == 0);
	}
	else
		return PrintUnrecParams(acplDbgSrcLoc, lSectionName, argc, argv);
	
	return 0;
}

SectionFuncParamsBegin(fspath)
SectionFuncParamsAdd("\"abs\" relative_path", "absolutize a path")
SectionFuncParamsAdd("\"stat\" path", "stat() a path and print info")
SectionFuncParamsAdd("\"lstat\" path", "lstat() a path and print info (the same as stat() on MSCRT)")
// SectionFuncParamsAdd("param [param [param ...]]", "description")
SectionFuncParamsEnd(fspath)
