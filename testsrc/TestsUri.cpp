#include "Tests.h"

#include "../src/Uri.h"


static int _local_TestUriElems(const acpl::Uri &nUri,
		const char *nScheme,
		bool nHasAuthority,
		const char *nUserInfo,
		const char *nHost,
		acpl::Uri::HostType nHostType,
		acpl::UInt16 nPort,
		const char *nPath,
		const char *nQuery,
		const char *nFragment
 	)
{
	Test(StringCompare(nUri.GetScheme().Utf8(), nScheme) == 0);
	Test(nUri.HasAuthority() == nHasAuthority);
	Test(StringCompare(nUri.GetUserInfo().Utf8(), nUserInfo) == 0);
	Test(StringCompare(nUri.GetHost().Utf8(), nHost) == 0);
	Test(nUri.GetHostType() == nHostType);
	Test(nUri.GetPort() == nPort);
	Test(StringCompare(nUri.GetPath().Utf8(), nPath) == 0);
	Test(StringCompare(nUri.GetQuery().Utf8(), nQuery) == 0);
	Test(StringCompare(nUri.GetFragment().Utf8(), nFragment) == 0);
	
	return 0;
}


static int TestPctEncDec()
{
	PrintFn();
	
	
	acpl::String oEnc, oDec;
	
	
	// Empty (set)
	acpl::Uri::PctEncode("", oEnc, false);
	acpl::Uri::PctDecode(oEnc, oDec, false);
	Test(oEnc.IsEmpty() == true);
	Test(oDec.IsEmpty() == true);
	
	// Empty (apnd)
	acpl::Uri::PctEncode("", oEnc, true);
	acpl::Uri::PctDecode(oEnc, oDec, true);
	Test(oEnc.IsEmpty() == true);
	Test(oDec.IsEmpty() == true);
	
	// No pct-enc chars (set)
	acpl::Uri::PctEncode("abc", oEnc, false);
	acpl::Uri::PctDecode(oEnc, oDec, false);
	Test(StringCompare(oEnc.Utf8(), "abc") == 0);
	Test(StringCompare(oDec.Utf8(), "abc") == 0);
	
	// No pct-enc chars (apnd)
	acpl::Uri::PctEncode("abc", oEnc, true);
	acpl::Uri::PctDecode(oEnc, oDec, true);
	Test(StringCompare(oEnc.Utf8(), "abcabc") == 0);
	Test(StringCompare(oDec.Utf8(), "abcabcabc") == 0);
	
	// US-ASCII pct-enc chars (set)
	acpl::Uri::PctEncode("# @", oEnc, false);
	acpl::Uri::PctDecode(oEnc, oDec, false);
	Test(StringCompare(oEnc.Utf8(), "%23%20%40") == 0);
	Test(StringCompare(oDec.Utf8(), "# @") == 0);
	
	// US-ASCII pct-enc chars (apnd)
	acpl::Uri::PctEncode("# @", oEnc, true);
	acpl::Uri::PctDecode(oEnc, oDec, true);
	Test(StringCompare(oEnc.Utf8(), "%23%20%40%23%20%40") == 0);
	Test(StringCompare(oDec.Utf8(), "# @# @# @") == 0);
	
	// Non US-ASCII pct-enc chars (set)
	acpl::Uri::PctEncode("\xC3\x9E\xC3\x97\xC2\xB6", oEnc, false);
	acpl::Uri::PctDecode(oEnc, oDec, false);
	Test(StringCompare(oEnc.Utf8(), "%C3%9E%C3%97%C2%B6") == 0);
	Test(StringCompare(oDec.Utf8(), "\xC3\x9E\xC3\x97\xC2\xB6") == 0);
	
	// Non US-ASCII pct-enc chars (apnd)
	acpl::Uri::PctEncode("\xC3\x9E\xC3\x97\xC2\xB6", oEnc, true);
	acpl::Uri::PctDecode(oEnc, oDec, true);
	Test(StringCompare(oEnc.Utf8(), "%C3%9E%C3%97%C2%B6%C3%9E%C3%97%C2%B6") == 0);
	Test(StringCompare(oDec.Utf8(), "\xC3\x9E\xC3\x97\xC2\xB6\xC3\x9E\xC3\x97\xC2\xB6\xC3\x9E\xC3\x97\xC2\xB6") == 0);
	
	
	return 0;
}

static int TestKvStringParseBuild()
{
	PrintFn();
	
	
	acpl::Uri::KeyValueList oKvList;
	acpl::String oQuery;
	
	
	// Empty
	acpl::Uri::ParseKvString("", oKvList);
	Test(oKvList.Count() == 0);
	acpl::Uri::BuildKvString(oKvList, oQuery);
	Test(oQuery.IsEmpty() == true);
	
	// Normal (1 KV)
	acpl::Uri::ParseKvString("abc=cba", oKvList);
	Test(oKvList.Count() == 1);
	acpl::Uri::BuildKvString(oKvList, oQuery);
	Test(StringCompare(oQuery.Utf8(), "abc=cba") == 0);
	
	// Normal (3 KVs)
	acpl::Uri::ParseKvString("abc=cba&def=fed&ghi=ihg", oKvList);
	Test(oKvList.Count() == 3);
	acpl::Uri::BuildKvString(oKvList, oQuery);
	Test(StringCompare(oQuery.Utf8(), "abc=cba&def=fed&ghi=ihg") == 0);
	
	// Only one '&' and no other KVs
	acpl::Uri::ParseKvString("&", oKvList);
	Test(oKvList.Count() == 0);
	acpl::Uri::BuildKvString(oKvList, oQuery);
	Test(oQuery.IsEmpty() == true);
	
	// Three '&' and no other KVs
	acpl::Uri::ParseKvString("&&&", oKvList);
	Test(oKvList.Count() == 0);
	acpl::Uri::BuildKvString(oKvList, oQuery);
	Test(oQuery.IsEmpty() == true);
	
	// Starts and ends with '&' and one KV
	acpl::Uri::ParseKvString("&abc=cba&", oKvList);
	Test(oKvList.Count() == 1);
	acpl::Uri::BuildKvString(oKvList, oQuery);
	Test(StringCompare(oQuery.Utf8(), "abc=cba") == 0);
	
	// 3 KVs, 1st key-only
	acpl::Uri::ParseKvString("abc&def=fed&ghi=ihg", oKvList);
	Test(oKvList.Count() == 3);
	acpl::Uri::BuildKvString(oKvList, oQuery);
	Test(StringCompare(oQuery.Utf8(), "abc&def=fed&ghi=ihg") == 0);
	
	// 3 KVs, 2nd key-only
	acpl::Uri::ParseKvString("abc=cba&def&ghi=ihg", oKvList);
	Test(oKvList.Count() == 3);
	acpl::Uri::BuildKvString(oKvList, oQuery);
	Test(StringCompare(oQuery.Utf8(), "abc=cba&def&ghi=ihg") == 0);
	
	// 3 KVs, 3rd key-only
	acpl::Uri::ParseKvString("abc=cba&def=fed&ghi", oKvList);
	Test(oKvList.Count() == 3);
	acpl::Uri::BuildKvString(oKvList, oQuery);
	Test(StringCompare(oQuery.Utf8(), "abc=cba&def=fed&ghi") == 0);
	
	// 3 KVs, 1st missing value
	acpl::Uri::ParseKvString("abc=&def=fed&ghi=ihg", oKvList);
	Test(oKvList.Count() == 3);
	acpl::Uri::BuildKvString(oKvList, oQuery);
	Test(StringCompare(oQuery.Utf8(), "abc&def=fed&ghi=ihg") == 0);
	
	// 3 KVs, 2nd missing value
	acpl::Uri::ParseKvString("abc=cba&def=&ghi=ihg", oKvList);
	Test(oKvList.Count() == 3);
	acpl::Uri::BuildKvString(oKvList, oQuery);
	Test(StringCompare(oQuery.Utf8(), "abc=cba&def&ghi=ihg") == 0);
	
	// 3 KVs, 3rd missing value
	acpl::Uri::ParseKvString("abc=cba&def=fed&ghi=", oKvList);
	Test(oKvList.Count() == 3);
	acpl::Uri::BuildKvString(oKvList, oQuery);
	Test(StringCompare(oQuery.Utf8(), "abc=cba&def=fed&ghi") == 0);
	
	// 3 KVs, 1st missing key
	acpl::Uri::ParseKvString("=cba&def=fed&ghi=ihg", oKvList);
	Test(oKvList.Count() == 3);
	acpl::Uri::BuildKvString(oKvList, oQuery);
	Test(StringCompare(oQuery.Utf8(), "=cba&def=fed&ghi=ihg") == 0);
	
	// 3 KVs, 2nd missing key
	acpl::Uri::ParseKvString("abc=cba&=fed&ghi=ihg", oKvList);
	Test(oKvList.Count() == 3);
	acpl::Uri::BuildKvString(oKvList, oQuery);
	Test(StringCompare(oQuery.Utf8(), "abc=cba&=fed&ghi=ihg") == 0);
	
	// 3 KVs, 3rd missing key
	acpl::Uri::ParseKvString("abc=cba&def=fed&=ihg", oKvList);
	Test(oKvList.Count() == 3);
	acpl::Uri::BuildKvString(oKvList, oQuery);
	Test(StringCompare(oQuery.Utf8(), "abc=cba&def=fed&=ihg") == 0);
	
	
	// 3 KVs completely pct-encoded
	acpl::Uri::ParseKvString("%61%62%63=%63%62%61&%64%65%66=%66%65%64&%67%68%69=%69%68%67", oKvList);
	Test(oKvList.Count() == 3);
	acpl::Uri::BuildKvString(oKvList, oQuery);
	Test(StringCompare(oQuery.Utf8(), "abc=cba&def=fed&ghi=ihg") == 0);
	
	
	// Looseness of parse, tightness of build
	acpl::Uri::ParseKvString("#?=:;@", oKvList);
	Test(oKvList.Count() == 1);
	acpl::Uri::BuildKvString(oKvList, oQuery);
	Test(StringCompare(oQuery.Utf8(), "%23%3F=%3A%3B%40") == 0);
	
	
	return 0;
}

static int TestInitSetGet()
{
	PrintFn();
	
	
	acpl::Uri oUri;
	acpl::Uri::KeyValueList oKvList;
	
	
	// Init
	
	Test(oUri.GetScheme().IsEmpty() == true);
	Test(oUri.HasAuthority() == true);
	Test(oUri.GetUserInfo().IsEmpty() == true);
	Test(oUri.GetHost().IsEmpty() == true);
	Test(oUri.GetHostType() == acpl::Uri::htUndef);
	Test(oUri.GetPort() == 0);
	Test(oUri.GetPath().IsEmpty() == true);
	Test(oUri.GetQuery().IsEmpty() == true);
	oUri.GetQuery(oKvList);
	Test(oKvList.Count() == 0);
	Test(oUri.GetFragment().IsEmpty() == true);
	
	
	// Scheme Set/Get
	
	Test(oUri.SetScheme("") == true);
	Test(oUri.SetScheme("3") == false);
	Test(oUri.SetScheme("-") == false);
	Test(oUri.SetScheme("a") == true);
	Test(oUri.SetScheme("a3") == true);
	Test(oUri.SetScheme("a3-.-+") == true);
	Test(StringCompare(oUri.GetScheme().Utf8(), "a3-.-+") == 0);
	Test(oUri.SetScheme("+a3-.-+") == false);
	Test(StringCompare(oUri.GetScheme().Utf8(), "a3-.-+") == 0);
	
	
	// HasAuthority Set/Get
	
	oUri.HasAuthority(false);
	Test(oUri.HasAuthority() == false);
	oUri.HasAuthority(true);
	Test(oUri.HasAuthority() == true);
	oUri.HasAuthority(false);
	Test(oUri.HasAuthority() == false);
	
	
	// UserInfo Set/Get
	
	Test(oUri.SetUserInfo("") == true);
	Test(oUri.SetUserInfo("a") == true);
	Test(oUri.SetUserInfo("%20") == true); // pct encoded
	Test(oUri.SetUserInfo("~") == true); // unreserved
	Test(oUri.SetUserInfo("$") == true); // sub delimiter
	Test(oUri.SetUserInfo(":") == true);
	Test(oUri.SetUserInfo("%20abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-._~!$&\'()*+,;=:") == true); // all of them
	Test(StringCompare(oUri.GetUserInfo().Utf8(), "%20abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-._~!$&\'()*+,;=:") == 0);
	Test(oUri.SetUserInfo("a@") == false);
	Test(StringCompare(oUri.GetUserInfo().Utf8(), "%20abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-._~!$&\'()*+,;=:") == 0);
	
	
	// Host Set/Get
	
	Test(oUri.SetHost("") == true);
	Test(oUri.GetHostType() == acpl::Uri::htRegisteredName);
	Test(StringCompare(oUri.GetHost().Utf8(), "") == 0);
	Test(oUri.SetHost("", acpl::Uri::htRegisteredName) == true);
	Test(oUri.GetHostType() == acpl::Uri::htRegisteredName);
	Test(StringCompare(oUri.GetHost().Utf8(), "") == 0);
	Test(oUri.SetHost("", acpl::Uri::htDomainName) == false);
	Test(StringCompare(oUri.GetHost().Utf8(), "") == 0);
	
	Test(oUri.SetHost("d0m4in") == true);
	Test(oUri.GetHostType() == acpl::Uri::htDomainName);
	Test(StringCompare(oUri.GetHost().Utf8(), "d0m4in") == 0);
	Test(oUri.SetHost("d0m4in", acpl::Uri::htDomainName) == true);
	Test(oUri.GetHostType() == acpl::Uri::htDomainName);
	Test(StringCompare(oUri.GetHost().Utf8(), "d0m4in") == 0);
	Test(oUri.SetHost("d0m4in", acpl::Uri::htIpv6Addr) == false);
	Test(StringCompare(oUri.GetHost().Utf8(), "d0m4in") == 0);
	
	Test(oUri.SetHost("127.0.0.1") == true);
	Test(oUri.GetHostType() == acpl::Uri::htIpv4Addr);
	Test(StringCompare(oUri.GetHost().Utf8(), "127.0.0.1") == 0);
	Test(oUri.SetHost("127.0.0.1", acpl::Uri::htIpv4Addr) == true);
	Test(oUri.GetHostType() == acpl::Uri::htIpv4Addr);
	Test(StringCompare(oUri.GetHost().Utf8(), "127.0.0.1") == 0);
	Test(oUri.SetHost("127.0.0.1", acpl::Uri::htIpvFutureAddr) == false);
	Test(StringCompare(oUri.GetHost().Utf8(), "127.0.0.1") == 0);
	
	Test(oUri.SetHost("127.0.0.a") == true);
	Test(oUri.GetHostType() == acpl::Uri::htDomainName);
	Test(StringCompare(oUri.GetHost().Utf8(), "127.0.0.a") == 0);
	Test(oUri.SetHost("127.0.0.a", acpl::Uri::htDomainName) == true);
	Test(oUri.GetHostType() == acpl::Uri::htDomainName);
	Test(StringCompare(oUri.GetHost().Utf8(), "127.0.0.a") == 0);
	Test(oUri.SetHost("127.0.0.a", acpl::Uri::htRegisteredName) == true);
	Test(oUri.GetHostType() == acpl::Uri::htRegisteredName);
	Test(StringCompare(oUri.GetHost().Utf8(), "127.0.0.a") == 0);
	Test(oUri.SetHost("127.0.0.a", acpl::Uri::htIpv4Addr) == false);
	Test(StringCompare(oUri.GetHost().Utf8(), "127.0.0.a") == 0);
	
	Test(oUri.SetHost("::127.0.0.1") == true);
	Test(oUri.GetHostType() == acpl::Uri::htIpv6Addr);
	Test(StringCompare(oUri.GetHost().Utf8(), "::127.0.0.1") == 0);
	Test(oUri.SetHost("::127.0.0.1", acpl::Uri::htIpv6Addr) == true);
	Test(oUri.GetHostType() == acpl::Uri::htIpv6Addr);
	Test(StringCompare(oUri.GetHost().Utf8(), "::127.0.0.1") == 0);
	Test(oUri.SetHost("::127.0.0.1", acpl::Uri::htIpv4Addr) == false);
	Test(StringCompare(oUri.GetHost().Utf8(), "::127.0.0.1") == 0);
	
	Test(oUri.SetHost("vD.2347--23-:3874") == true);
	Test(oUri.GetHostType() == acpl::Uri::htIpvFutureAddr);
	Test(StringCompare(oUri.GetHost().Utf8(), "vD.2347--23-:3874") == 0);
	Test(oUri.SetHost("vD.2347--23-:3874", acpl::Uri::htIpvFutureAddr) == true);
	Test(oUri.GetHostType() == acpl::Uri::htIpvFutureAddr);
	Test(StringCompare(oUri.GetHost().Utf8(), "vD.2347--23-:3874") == 0);
	Test(oUri.SetHost("vD.2347--23-:3874", acpl::Uri::htDomainName) == false);
	Test(StringCompare(oUri.GetHost().Utf8(), "vD.2347--23-:3874") == 0);
	
	Test(oUri.SetHost("D.23&47--23-3874") == true);
	Test(oUri.GetHostType() == acpl::Uri::htRegisteredName);
	Test(StringCompare(oUri.GetHost().Utf8(), "D.23&47--23-3874") == 0);
	Test(oUri.SetHost("D.23&47--23-3874", acpl::Uri::htRegisteredName) == true);
	Test(oUri.GetHostType() == acpl::Uri::htRegisteredName);
	Test(StringCompare(oUri.GetHost().Utf8(), "D.23&47--23-3874") == 0);
	Test(oUri.SetHost("D.23&47--23-3874", acpl::Uri::htIpv6Addr) == false);
	Test(StringCompare(oUri.GetHost().Utf8(), "D.23&47--23-3874") == 0);
	
	
	// Port
	
	oUri.SetPort(1234);
	Test(oUri.GetPort() == 1234);
	oUri.SetPort(0);
	Test(oUri.GetPort() == 0);
	oUri.SetPort(4321);
	Test(oUri.GetPort() == 4321);
	
	
	// Path
	
	Test(oUri.SetPath("") == true);
	Test(StringCompare(oUri.GetPath().Utf8(), "") == 0);
	Test(oUri.SetPath("abc") == true);
	Test(StringCompare(oUri.GetPath().Utf8(), "abc") == 0);
	Test(oUri.SetPath("/abc") == true);
	Test(StringCompare(oUri.GetPath().Utf8(), "/abc") == 0);
	Test(oUri.SetPath("%20abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-._~!$&\'()*+,;=:@") == true);
	Test(StringCompare(oUri.GetPath().Utf8(), "%20abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-._~!$&\'()*+,;=:@") == 0);
	Test(oUri.SetPath("a?") == false);
	Test(StringCompare(oUri.GetPath().Utf8(), "%20abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-._~!$&\'()*+,;=:@") == 0);
	
	
	// Query
	
	Test(oUri.SetQuery("") == true);
	Test(StringCompare(oUri.GetQuery().Utf8(), "") == 0);
	Test(oUri.SetQuery("%20abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-._~!$&\'()*+,;=:@?") == true);
	Test(StringCompare(oUri.GetQuery().Utf8(), "%20abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-._~!$&\'()*+,;=:@?") == 0);
	Test(oUri.SetQuery("a#") == false);
	Test(StringCompare(oUri.GetQuery().Utf8(), "%20abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-._~!$&\'()*+,;=:@?") == 0);
	
	
	// Query (KeyValue) - simple tests here as comprehensive tests are made in `TestKvStringParseBuild()`
	
	Test(oUri.SetQuery("abc=cba&def=fed&ghi=ihg") == true);
	oUri.GetQuery(oKvList);
	Test(oKvList.Count() == 3);
	Test(oUri.SetQuery("") == true);
	oUri.GetQuery(oKvList);
	Test(oKvList.Count() == 0);
	Test(oUri.SetQuery("abc=cba") == true);
	oUri.GetQuery(oKvList);
	Test(oKvList.Count() == 1);
	
	
	// Fragment
	
	Test(oUri.SetFragment("") == true);
	Test(StringCompare(oUri.GetFragment().Utf8(), "") == 0);
	Test(oUri.SetFragment("%20abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-._~!$&\'()*+,;=:@?") == true);
	Test(StringCompare(oUri.GetFragment().Utf8(), "%20abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-._~!$&\'()*+,;=:@?") == 0);
	Test(oUri.SetFragment("a#") == false);
	Test(StringCompare(oUri.GetFragment().Utf8(), "%20abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-._~!$&\'()*+,;=:@?") == 0);
	
	
	// Clear
	
	Test(oUri.GetScheme().IsEmpty() == false);
	Test(oUri.HasAuthority() == false);
	Test(oUri.GetUserInfo().IsEmpty() == false);
	Test(oUri.GetHost().IsEmpty() == false);
	Test(oUri.GetHostType() == acpl::Uri::htRegisteredName);
	Test(oUri.GetPort() == 4321);
	Test(oUri.GetPath().IsEmpty() == false);
	Test(oUri.GetQuery().IsEmpty() == false);
	oUri.GetQuery(oKvList);
	Test(oKvList.Count() == 1);
	Test(oUri.GetFragment().IsEmpty() == false);
	
	oUri.Clear();
	
	Test(oUri.GetScheme().IsEmpty() == true);
	Test(oUri.HasAuthority() == true);
	Test(oUri.GetUserInfo().IsEmpty() == true);
	Test(oUri.GetHost().IsEmpty() == true);
	Test(oUri.GetHostType() == acpl::Uri::htUndef);
	Test(oUri.GetPort() == 0);
	Test(oUri.GetPath().IsEmpty() == true);
	Test(oUri.GetQuery().IsEmpty() == true);
	oUri.GetQuery(oKvList);
	Test(oKvList.Count() == 0);
	Test(oUri.GetFragment().IsEmpty() == true);
	
	
	return 0;
}

static int TestParseValidate()
{
	PrintFn();
	
	
	acpl::Uri oUri;
	
	// NOTE: Detailed validation for different URI elements can be found in
	//       TestInitSetGet() function as they all use the same validation
	//       mechanisms in the background.
	
	
	// Scheme
	
	Test(acpl::Uri::Validate("", acpl::Uri::vlUri) == false); // URL and URI don't allow empty scheme
	Test(acpl::Uri::Validate("", acpl::Uri::vlUriRef) == true); // URI-Ref, however, is fine without any elements
	Test(oUri.Parse("", acpl::Uri::vlUriRef) == true);
	Test(_local_TestUriElems(oUri, "", false, "", "", acpl::Uri::htUndef, 0, "", "", "") == 0);
	Test(acpl::Uri::Validate("sch3m3", acpl::Uri::vlUri) == false); // missing ':'
	Test(acpl::Uri::Validate("sch3m3:", acpl::Uri::vlUri) == true); //
	Test(oUri.Parse("sch3m3:", acpl::Uri::vlUri) == true);
	Test(_local_TestUriElems(oUri, "sch3m3", false, "", "", acpl::Uri::htUndef, 0, "", "", "") == 0);
	
	
	// Scheme without authority for URL
	
	Test(acpl::Uri::Validate("sch3m3:///", acpl::Uri::vlUrl) == false);
	Test(acpl::Uri::Validate("sch3m3:///", acpl::Uri::vlUrl) == false);
	Test(acpl::Uri::Validate("sch3m3:///", acpl::Uri::vlUrl) == false);
	Test(acpl::Uri::Validate("sch3m3:///", acpl::Uri::vlUrl) == false);
	Test(acpl::Uri::Validate("sch3m3://?", acpl::Uri::vlUrl) == false);
	Test(acpl::Uri::Validate("sch3m3://#", acpl::Uri::vlUrl) == false);
	Test(acpl::Uri::Validate("sch3m3://@", acpl::Uri::vlUrl) == false);
	
	
	// Authority
	
	Test(oUri.Parse("sch3m3://d0m4in", acpl::Uri::vlUrl) == true);
	Test(_local_TestUriElems(oUri, "sch3m3", true, "", "d0m4in", acpl::Uri::htDomainName, 0, "", "", "") == 0);
	
	Test(acpl::Uri::Validate("sch3m3://:d0m4in", acpl::Uri::vlUrl) == false);
	
	Test(oUri.Parse("sch3m3://d0m4in:", acpl::Uri::vlUrl) == true);
	Test(_local_TestUriElems(oUri, "sch3m3", true, "", "d0m4in", acpl::Uri::htDomainName, 0, "", "", "") == 0);
	Test(acpl::Uri::Validate("sch3m3://d0m4in:a", acpl::Uri::vlUrl) == false);
	Test(oUri.Parse("sch3m3://d0m4in:65535", acpl::Uri::vlUrl) == true);
	Test(_local_TestUriElems(oUri, "sch3m3", true, "", "d0m4in", acpl::Uri::htDomainName, 65535, "", "", "") == 0);
	Test(acpl::Uri::Validate("sch3m3://d0m4in:65536", acpl::Uri::vlUrl) == false); // port number is larger than 65535 (maximum for 16-bit unsigned integer)
	
	Test(oUri.Parse("sch3m3://@d0m4in", acpl::Uri::vlUrl) == true);
	Test(_local_TestUriElems(oUri, "sch3m3", true, "", "d0m4in", acpl::Uri::htDomainName, 0, "", "", "") == 0);
	Test(oUri.Parse("sch3m3://us3r-inf0@d0m4in", acpl::Uri::vlUrl) == true);
	Test(_local_TestUriElems(oUri, "sch3m3", true, "us3r-inf0", "d0m4in", acpl::Uri::htDomainName, 0, "", "", "") == 0);
	
	Test(oUri.Parse("sch3m3://127.0.0.1:1234", acpl::Uri::vlUrl) == true);
	Test(_local_TestUriElems(oUri, "sch3m3", true, "", "127.0.0.1", acpl::Uri::htIpv4Addr, 1234, "", "", "") == 0);
	
	Test(acpl::Uri::Validate("sch3m3://[::127.0.0.1", acpl::Uri::vlUrl) == false);
	Test(oUri.Parse("sch3m3://[::127.0.0.1]:1234", acpl::Uri::vlUrl) == true);
	Test(_local_TestUriElems(oUri, "sch3m3", true, "", "::127.0.0.1", acpl::Uri::htIpv6Addr, 1234, "", "", "") == 0);
	
	
	Test(acpl::Uri::Validate("sch3m3://[v]:1234", acpl::Uri::vlUri) == false);
	Test(acpl::Uri::Validate("sch3m3://[vF]:1234", acpl::Uri::vlUri) == false);
	Test(acpl::Uri::Validate("sch3m3://[vF.]:1234", acpl::Uri::vlUri) == false);
	Test(acpl::Uri::Validate("sch3m3://[vF.?]:1234", acpl::Uri::vlUri) == false);
	Test(acpl::Uri::Validate("sch3m3://[vF.x--oe:2388]:1234", acpl::Uri::vlUrl) == false); // URL doesn't support IPvFuture
	Test(acpl::Uri::Validate("sch3m3://[vF.x--oe:2388]:1234", acpl::Uri::vlUri) == true);
	Test(oUri.Parse("sch3m3://[vF.x--oe:2388]:1234", acpl::Uri::vlUri) == true);
	Test(_local_TestUriElems(oUri, "sch3m3", true, "", "vF.x--oe:2388", acpl::Uri::htIpvFutureAddr, 1234, "", "", "") == 0);
	
	Test(acpl::Uri::Validate("sch3m3://aa-:1234", acpl::Uri::vlUrl) == false); // URL doesn't support registered names
	Test(acpl::Uri::Validate("sch3m3://aa-:1234", acpl::Uri::vlUri) == true);
	Test(oUri.Parse("sch3m3://aa-:1234", acpl::Uri::vlUri) == true);
	Test(_local_TestUriElems(oUri, "sch3m3", true, "", "aa-", acpl::Uri::htRegisteredName, 1234, "", "", "") == 0);
	
	Test(acpl::Uri::Validate("sch3m3://:1234", acpl::Uri::vlUrl) == false); // URL doesn't support registered names
	Test(acpl::Uri::Validate("sch3m3://:1234", acpl::Uri::vlUri) == true);
	Test(oUri.Parse("sch3m3://:1234", acpl::Uri::vlUri) == true);
	Test(_local_TestUriElems(oUri, "sch3m3", true, "", "", acpl::Uri::htRegisteredName, 1234, "", "", "") == 0);
	
	
	// Path
	
	Test(oUri.Parse("p4th", acpl::Uri::vlUriRef) == true);
	Test(_local_TestUriElems(oUri, "", false, "", "", acpl::Uri::htUndef, 0, "p4th", "", "") == 0);
	
	Test(oUri.Parse("/p4th", acpl::Uri::vlUriRef) == true);
	Test(_local_TestUriElems(oUri, "", false, "", "", acpl::Uri::htUndef, 0, "/p4th", "", "") == 0);
	
	Test(oUri.Parse("//d0m4in", acpl::Uri::vlUriRef) == true);
	Test(_local_TestUriElems(oUri, "", true, "", "d0m4in", acpl::Uri::htDomainName, 0, "", "", "") == 0); // this is no path but a domain
	
	Test(oUri.Parse("///p4th", acpl::Uri::vlUriRef) == true);
	Test(_local_TestUriElems(oUri, "", true, "", "", acpl::Uri::htUndef, 0, "/p4th", "", "") == 0);
	
	Test(oUri.Parse("////p4th", acpl::Uri::vlUriRef) == true);
	Test(_local_TestUriElems(oUri, "", true, "", "", acpl::Uri::htUndef, 0, "//p4th", "", "") == 0);
	
	Test(acpl::Uri::Validate(":p4th", acpl::Uri::vlUriRef) == false); // URI-Ref cannot start with a ':' with no scheme
	Test(oUri.Parse("sch3m3:p4th", acpl::Uri::vlUri) == true); // URI level fine
	Test(_local_TestUriElems(oUri, "sch3m3", false, "", "", acpl::Uri::htUndef, 0, "p4th", "", "") == 0);
	
	Test(oUri.Parse("sch3m3:/p4th", acpl::Uri::vlUri) == true);
	Test(_local_TestUriElems(oUri, "sch3m3", false, "", "", acpl::Uri::htUndef, 0, "/p4th", "", "") == 0);
	
	Test(oUri.Parse("sch3m3:/", acpl::Uri::vlUri) == true);
	Test(_local_TestUriElems(oUri, "sch3m3", false, "", "", acpl::Uri::htUndef, 0, "/", "", "") == 0);
	
	Test(oUri.Parse("sch3m3://", acpl::Uri::vlUri) == true);
	Test(_local_TestUriElems(oUri, "sch3m3", true, "", "", acpl::Uri::htUndef, 0, "", "", "") == 0); // no path
	
	Test(oUri.Parse("sch3m3:///", acpl::Uri::vlUri) == true);
	Test(_local_TestUriElems(oUri, "sch3m3", true, "", "", acpl::Uri::htUndef, 0, "/", "", "") == 0);
	
	Test(oUri.Parse("sch3m3://d0m4in", acpl::Uri::vlUri) == true);
	Test(_local_TestUriElems(oUri, "sch3m3", true, "", "d0m4in", acpl::Uri::htDomainName, 0, "", "", "") == 0);
	
	Test(oUri.Parse("sch3m3://d0m4in/", acpl::Uri::vlUri) == true);
	Test(_local_TestUriElems(oUri, "sch3m3", true, "", "d0m4in", acpl::Uri::htDomainName, 0, "/", "", "") == 0);
	
	Test(oUri.Parse("sch3m3://d0m4in?", acpl::Uri::vlUri) == true);
	Test(_local_TestUriElems(oUri, "sch3m3", true, "", "d0m4in", acpl::Uri::htDomainName, 0, "", "", "") == 0);
	
	Test(oUri.Parse("sch3m3://d0m4in#", acpl::Uri::vlUri) == true);
	Test(_local_TestUriElems(oUri, "sch3m3", true, "", "d0m4in", acpl::Uri::htDomainName, 0, "", "", "") == 0);
	
	
	// Query
	
	Test(oUri.Parse("?", acpl::Uri::vlUriRef) == true);
	Test(_local_TestUriElems(oUri, "", false, "", "", acpl::Uri::htUndef, 0, "", "", "") == 0);
	
	Test(oUri.Parse("?qu3ry", acpl::Uri::vlUriRef) == true);
	Test(_local_TestUriElems(oUri, "", false, "", "", acpl::Uri::htUndef, 0, "", "qu3ry", "") == 0);
	
	Test(oUri.Parse("p4th?qu3ry", acpl::Uri::vlUriRef) == true);
	Test(_local_TestUriElems(oUri, "", false, "", "", acpl::Uri::htUndef, 0, "p4th", "qu3ry", "") == 0);
	
	Test(oUri.Parse("sch3m3:?qu3ry", acpl::Uri::vlUri) == true);
	Test(_local_TestUriElems(oUri, "sch3m3", false, "", "", acpl::Uri::htUndef, 0, "", "qu3ry", "") == 0);
	
	Test(oUri.Parse("sch3m3:/?qu3ry", acpl::Uri::vlUri) == true);
	Test(_local_TestUriElems(oUri, "sch3m3", false, "", "", acpl::Uri::htUndef, 0, "/", "qu3ry", "") == 0);
	
	Test(oUri.Parse("sch3m3://?qu3ry", acpl::Uri::vlUri) == true);
	Test(_local_TestUriElems(oUri, "sch3m3", true, "", "", acpl::Uri::htUndef, 0, "", "qu3ry", "") == 0);
	
	Test(oUri.Parse("sch3m3://d0m4in?qu3ry", acpl::Uri::vlUri) == true);
	Test(_local_TestUriElems(oUri, "sch3m3", true, "", "d0m4in", acpl::Uri::htDomainName, 0, "", "qu3ry", "") == 0);
	
	Test(oUri.Parse("sch3m3:///?qu3ry", acpl::Uri::vlUri) == true);
	Test(_local_TestUriElems(oUri, "sch3m3", true, "", "", acpl::Uri::htUndef, 0, "/", "qu3ry", "") == 0);
	
	Test(oUri.Parse("sch3m3://d0m4in/?qu3ry", acpl::Uri::vlUri) == true);
	Test(_local_TestUriElems(oUri, "sch3m3", true, "", "d0m4in", acpl::Uri::htDomainName, 0, "/", "qu3ry", "") == 0);
	
	Test(oUri.Parse("sch3m3://d0m4in/?qu3ry#", acpl::Uri::vlUri) == true);
	Test(_local_TestUriElems(oUri, "sch3m3", true, "", "d0m4in", acpl::Uri::htDomainName, 0, "/", "qu3ry", "") == 0);
	
	
	// Fragment
	
	Test(oUri.Parse("#", acpl::Uri::vlUriRef) == true);
	Test(_local_TestUriElems(oUri, "", false, "", "", acpl::Uri::htUndef, 0, "", "", "") == 0);
	
	Test(oUri.Parse("#fr4gm3nt", acpl::Uri::vlUriRef) == true);
	Test(_local_TestUriElems(oUri, "", false, "", "", acpl::Uri::htUndef, 0, "", "", "fr4gm3nt") == 0);
	
	Test(oUri.Parse("?qu3ry#fr4gm3nt", acpl::Uri::vlUriRef) == true);
	Test(_local_TestUriElems(oUri, "", false, "", "", acpl::Uri::htUndef, 0, "", "qu3ry", "fr4gm3nt") == 0);
	
	Test(oUri.Parse("p4th?qu3ry#fr4gm3nt", acpl::Uri::vlUriRef) == true);
	Test(_local_TestUriElems(oUri, "", false, "", "", acpl::Uri::htUndef, 0, "p4th", "qu3ry", "fr4gm3nt") == 0);
	
	Test(oUri.Parse("sch3m3:#fr4gm3nt", acpl::Uri::vlUri) == true);
	Test(_local_TestUriElems(oUri, "sch3m3", false, "", "", acpl::Uri::htUndef, 0, "", "", "fr4gm3nt") == 0);
	
	Test(oUri.Parse("sch3m3:/#fr4gm3nt", acpl::Uri::vlUri) == true);
	Test(_local_TestUriElems(oUri, "sch3m3", false, "", "", acpl::Uri::htUndef, 0, "/", "", "fr4gm3nt") == 0);
	
	Test(oUri.Parse("sch3m3://#fr4gm3nt", acpl::Uri::vlUri) == true);
	Test(_local_TestUriElems(oUri, "sch3m3", true, "", "", acpl::Uri::htUndef, 0, "", "", "fr4gm3nt") == 0);
	
	Test(oUri.Parse("sch3m3://d0m4in#fr4gm3nt", acpl::Uri::vlUri) == true);
	Test(_local_TestUriElems(oUri, "sch3m3", true, "", "d0m4in", acpl::Uri::htDomainName, 0, "", "", "fr4gm3nt") == 0);
	
	Test(oUri.Parse("sch3m3:///#fr4gm3nt", acpl::Uri::vlUri) == true);
	Test(_local_TestUriElems(oUri, "sch3m3", true, "", "", acpl::Uri::htUndef, 0, "/", "", "fr4gm3nt") == 0);
	
	Test(oUri.Parse("sch3m3://d0m4in/#fr4gm3nt", acpl::Uri::vlUri) == true);
	Test(_local_TestUriElems(oUri, "sch3m3", true, "", "d0m4in", acpl::Uri::htDomainName, 0, "/", "", "fr4gm3nt") == 0);
	
	Test(oUri.Parse("sch3m3://d0m4in/?#fr4gm3nt", acpl::Uri::vlUri) == true);
	Test(_local_TestUriElems(oUri, "sch3m3", true, "", "d0m4in", acpl::Uri::htDomainName, 0, "/", "", "fr4gm3nt") == 0);
	
	
	// All element URL
	
	static const char *oUriStr = "sch3m3://us3r%20inf0@d0m4in:1234/p4th?qu3ry#fr4gm3nt";
	Test(acpl::Uri::Validate(oUriStr, acpl::Uri::vlUrl) == true);
	Test(oUri.Parse(oUriStr, acpl::Uri::vlUrl) == true);
	Test(_local_TestUriElems(oUri, "sch3m3", true, "us3r%20inf0", "d0m4in", acpl::Uri::htDomainName, 1234, "/p4th", "qu3ry", "fr4gm3nt") == 0);
	
	
	return 0;
}

static int TestBuildOnly()
{
	PrintFn();
	
	
	acpl::Uri oUri;
	acpl::String oStr;
	
	
	// Scheme
	
	oUri.HasAuthority(false);
	
	// empty scheme
	Test(oUri.Build(oStr, acpl::Uri::vlUrl) == false);
	Test(oUri.Build(oStr, acpl::Uri::vlUri) == false);
	Test(oUri.Build(oStr, acpl::Uri::vlUriRef) == true);
	Test(StringCompare(oStr.Utf8(), "") == 0);
	
	Test(oUri.SetScheme("sch3m3") == true);
	Test(oUri.Build(oStr, acpl::Uri::vlUrl) == false);
	Test(oUri.Build(oStr, acpl::Uri::vlUri) == true);
	Test(StringCompare(oStr.Utf8(), "sch3m3:") == 0);
	
	oUri.HasAuthority(true);
	Test(oUri.Build(oStr, acpl::Uri::vlUrl) == false);
	Test(oUri.Build(oStr, acpl::Uri::vlUri) == true);
	Test(StringCompare(oStr.Utf8(), "sch3m3://") == 0);
	
	
	// Authority
	
	oUri.SetPort(1234); // port num only
	Test(oUri.Build(oStr, acpl::Uri::vlUrl) == false);
	Test(oUri.Build(oStr, acpl::Uri::vlUri) == true);
	Test(StringCompare(oStr.Utf8(), "sch3m3://:1234") == 0);
	
	Test(oUri.SetUserInfo("us3r%20inf0") == true); // port num + user info
	Test(oUri.Build(oStr, acpl::Uri::vlUrl) == false);
	Test(oUri.Build(oStr, acpl::Uri::vlUri) == true);
	Test(StringCompare(oStr.Utf8(), "sch3m3://us3r%20inf0@:1234") == 0);
	
	oUri.SetPort(0); // user info only
	Test(oUri.Build(oStr, acpl::Uri::vlUrl) == false);
	Test(oUri.Build(oStr, acpl::Uri::vlUri) == true);
	Test(StringCompare(oStr.Utf8(), "sch3m3://us3r%20inf0@") == 0);
	
	Test(oUri.SetUserInfo("") == true); // no authority at all (for now)
	Test(oUri.Build(oStr, acpl::Uri::vlUrl) == false);
	Test(oUri.Build(oStr, acpl::Uri::vlUri) == true);
	Test(StringCompare(oStr.Utf8(), "sch3m3://") == 0);
	
	Test(oUri.SetHost("D.23&47--23-3874") == true); // registered name
	Test(oUri.Build(oStr, acpl::Uri::vlUrl) == false);
	Test(oUri.Build(oStr, acpl::Uri::vlUri) == true);
	Test(StringCompare(oStr.Utf8(), "sch3m3://D.23&47--23-3874") == 0);
	
	Test(oUri.SetHost("vD.2347--23-:3874") == true); // IPvFuture
	Test(oUri.Build(oStr, acpl::Uri::vlUrl) == false);
	Test(oUri.Build(oStr, acpl::Uri::vlUri) == true);
	Test(StringCompare(oStr.Utf8(), "sch3m3://[vD.2347--23-:3874]") == 0);
	
	Test(oUri.SetHost("d0m4in") == true); // domain name
	Test(oUri.Build(oStr, acpl::Uri::vlUrl) == true);
	Test(StringCompare(oStr.Utf8(), "sch3m3://d0m4in") == 0);
	
	Test(oUri.SetHost("127.0.0.1") == true); // IPv4
	Test(oUri.Build(oStr, acpl::Uri::vlUrl) == true);
	Test(StringCompare(oStr.Utf8(), "sch3m3://127.0.0.1") == 0);
	
	Test(oUri.SetHost("::127.0.0.1") == true); // IPv6
	Test(oUri.Build(oStr, acpl::Uri::vlUrl) == true);
	Test(StringCompare(oStr.Utf8(), "sch3m3://[::127.0.0.1]") == 0);
	
	oUri.SetPort(1234); // host + port num
	Test(oUri.Build(oStr, acpl::Uri::vlUrl) == true);
	Test(StringCompare(oStr.Utf8(), "sch3m3://[::127.0.0.1]:1234") == 0);
	
	Test(oUri.SetUserInfo("us3r%20inf0") == true); // full authority (host + port num + user info)
	Test(oUri.Build(oStr, acpl::Uri::vlUrl) == true);
	Test(StringCompare(oStr.Utf8(), "sch3m3://us3r%20inf0@[::127.0.0.1]:1234") == 0);
	
	
	// Path
	
	Test(oUri.SetPath("/p4th") == true);
	Test(oUri.Build(oStr, acpl::Uri::vlUrl) == true);
	Test(StringCompare(oStr.Utf8(), "sch3m3://us3r%20inf0@[::127.0.0.1]:1234/p4th") == 0);
	
	Test(oUri.SetHost("") == true);
	Test(oUri.Build(oStr, acpl::Uri::vlUrl) == false);
	Test(oUri.Build(oStr, acpl::Uri::vlUri) == true);
	Test(StringCompare(oStr.Utf8(), "sch3m3://us3r%20inf0@:1234/p4th") == 0);
	
	Test(oUri.SetUserInfo("") == true);
	oUri.SetPort(0);
	Test(oUri.Build(oStr, acpl::Uri::vlUrl) == false);
	Test(oUri.Build(oStr, acpl::Uri::vlUri) == true);
	Test(StringCompare(oStr.Utf8(), "sch3m3:///p4th") == 0);
	
	oUri.HasAuthority(false);
	Test(oUri.Build(oStr, acpl::Uri::vlUrl) == false);
	Test(oUri.Build(oStr, acpl::Uri::vlUri) == true);
	Test(StringCompare(oStr.Utf8(), "sch3m3:/p4th") == 0);
	
	Test(oUri.SetScheme("") == true);
	Test(oUri.Build(oStr, acpl::Uri::vlUri) == false);
	Test(oUri.Build(oStr, acpl::Uri::vlUriRef) == true);
	Test(StringCompare(oStr.Utf8(), "/p4th") == 0);
	
	Test(oUri.SetPath(":p4th") == true);
	Test(oUri.Build(oStr, acpl::Uri::vlUriRef) == false);
	
	Test(oUri.SetScheme("sch3m3") == true);
	Test(oUri.Build(oStr, acpl::Uri::vlUri) == true);
	Test(StringCompare(oStr.Utf8(), "sch3m3::p4th") == 0);
	
	Test(oUri.SetScheme("") == true);
	Test(oUri.SetHost("d0m4in") == true);
	oUri.HasAuthority(true);
	Test(oUri.Build(oStr, acpl::Uri::vlUriRef) == false);
	
	Test(oUri.SetPath("/p4th") == true);
	Test(oUri.Build(oStr, acpl::Uri::vlUri) == false);
	Test(oUri.Build(oStr, acpl::Uri::vlUriRef) == true);
	Test(StringCompare(oStr.Utf8(), "//d0m4in/p4th") == 0);
	
	Test(oUri.SetPath("//p4th") == true);
	Test(oUri.Build(oStr, acpl::Uri::vlUri) == false);
	Test(oUri.Build(oStr, acpl::Uri::vlUriRef) == true);
	Test(StringCompare(oStr.Utf8(), "//d0m4in//p4th") == 0);
	
	Test(oUri.SetHost("") == true);
	oUri.HasAuthority(false);
	Test(oUri.Build(oStr, acpl::Uri::vlUriRef) == false);
	
	Test(oUri.SetPath("lookslikescheme:p4th/subdir") == true); // ':' appears in path before a '/' does and there's no scheme set
	Test(oUri.Build(oStr, acpl::Uri::vlUriRef) == false);
	
	Test(oUri.SetScheme("sch3m3") == true);
	Test(oUri.Build(oStr, acpl::Uri::vlUri) == true);
	Test(StringCompare(oStr.Utf8(), "sch3m3:lookslikescheme:p4th/subdir") == 0);
	
	
	// Query
	
	Test(oUri.SetQuery("qu3ry") == true);
	Test(oUri.Build(oStr, acpl::Uri::vlUri) == true);
	Test(StringCompare(oStr.Utf8(), "sch3m3:lookslikescheme:p4th/subdir?qu3ry") == 0);
	
	Test(oUri.SetPath("/") == true);
	Test(oUri.Build(oStr, acpl::Uri::vlUri) == true);
	Test(StringCompare(oStr.Utf8(), "sch3m3:/?qu3ry") == 0);
	
	Test(oUri.SetPath("") == true);
	Test(oUri.Build(oStr, acpl::Uri::vlUri) == true);
	Test(StringCompare(oStr.Utf8(), "sch3m3:?qu3ry") == 0);
	
	Test(oUri.SetScheme("") == true);
	Test(oUri.Build(oStr, acpl::Uri::vlUri) == false);
	Test(oUri.Build(oStr, acpl::Uri::vlUriRef) == true);
	Test(StringCompare(oStr.Utf8(), "?qu3ry") == 0);
	
	oUri.HasAuthority(true);
	Test(oUri.Build(oStr, acpl::Uri::vlUri) == false);
	Test(oUri.Build(oStr, acpl::Uri::vlUriRef) == true);
	Test(StringCompare(oStr.Utf8(), "//?qu3ry") == 0);
	
	Test(oUri.SetHost("d0m4in") == true);
	Test(oUri.Build(oStr, acpl::Uri::vlUri) == false);
	Test(oUri.Build(oStr, acpl::Uri::vlUriRef) == true);
	Test(StringCompare(oStr.Utf8(), "//d0m4in?qu3ry") == 0);
	
	Test(oUri.SetScheme("sch3m3") == true);
	Test(oUri.Build(oStr, acpl::Uri::vlUrl) == true); // is a valid URL
	Test(StringCompare(oStr.Utf8(), "sch3m3://d0m4in?qu3ry") == 0);
	
	
	// Fragment
	
	Test(oUri.SetFragment("fr4gm3nt") == true);
	Test(oUri.Build(oStr, acpl::Uri::vlUrl) == true);
	Test(StringCompare(oStr.Utf8(), "sch3m3://d0m4in?qu3ry#fr4gm3nt") == 0);
	
	Test(oUri.SetQuery("") == true);
	Test(oUri.Build(oStr, acpl::Uri::vlUrl) == true);
	Test(StringCompare(oStr.Utf8(), "sch3m3://d0m4in#fr4gm3nt") == 0);
	
	Test(oUri.SetHost("") == true);
	Test(oUri.Build(oStr, acpl::Uri::vlUrl) == false);
	Test(oUri.Build(oStr, acpl::Uri::vlUri) == true);
	Test(StringCompare(oStr.Utf8(), "sch3m3://#fr4gm3nt") == 0);
	
	oUri.HasAuthority(false);
	Test(oUri.Build(oStr, acpl::Uri::vlUri) == true);
	Test(StringCompare(oStr.Utf8(), "sch3m3:#fr4gm3nt") == 0);
	
	Test(oUri.SetScheme("") == true);
	Test(oUri.Build(oStr, acpl::Uri::vlUri) == false);
	Test(oUri.Build(oStr, acpl::Uri::vlUriRef) == true);
	Test(StringCompare(oStr.Utf8(), "#fr4gm3nt") == 0);
	
	
	return 0;
}

static int TestParseBuild()
{
	PrintFn();
	
	
	acpl::Uri oUri;
	acpl::String oStr;
	
	
	// URI-Refs
	
	Test(oUri.Parse("", acpl::Uri::vlUriRef) == true);
	Test(oUri.Build(oStr, acpl::Uri::vlUriRef) == true);
	Test(StringCompare(oStr.Utf8(), "") == 0);
	
	Test(oUri.Parse("#fr4gm3nt", acpl::Uri::vlUriRef) == true);
	Test(oUri.Build(oStr, acpl::Uri::vlUriRef) == true);
	Test(StringCompare(oStr.Utf8(), "#fr4gm3nt") == 0);
	
	Test(oUri.Parse("?qu3ry#fr4gm3nt", acpl::Uri::vlUriRef) == true);
	Test(oUri.Build(oStr, acpl::Uri::vlUriRef) == true);
	Test(StringCompare(oStr.Utf8(), "?qu3ry#fr4gm3nt") == 0);
	
	Test(oUri.Parse("p4th?qu3ry#fr4gm3nt", acpl::Uri::vlUriRef) == true);
	Test(oUri.Build(oStr, acpl::Uri::vlUriRef) == true);
	Test(StringCompare(oStr.Utf8(), "p4th?qu3ry#fr4gm3nt") == 0);
	
	Test(oUri.Parse("//d0m4in/p4th?qu3ry#fr4gm3nt", acpl::Uri::vlUriRef) == true);
	Test(oUri.Build(oStr, acpl::Uri::vlUriRef) == true);
	Test(StringCompare(oStr.Utf8(), "//d0m4in/p4th?qu3ry#fr4gm3nt") == 0);
	
	// URIs
	
	Test(oUri.Parse("sch3m3:///p4th?qu3ry#fr4gm3nt", acpl::Uri::vlUri) == true);
	Test(oUri.Build(oStr, acpl::Uri::vlUri) == true);
	Test(StringCompare(oStr.Utf8(), "sch3m3:///p4th?qu3ry#fr4gm3nt") == 0);
	
	Test(oUri.Parse("sch3m3:/p4th?qu3ry#fr4gm3nt", acpl::Uri::vlUri) == true);
	Test(oUri.Build(oStr, acpl::Uri::vlUri) == true);
	Test(StringCompare(oStr.Utf8(), "sch3m3:/p4th?qu3ry#fr4gm3nt") == 0);
	
	Test(oUri.Parse("sch3m3://D.23&47--23-3874/p4th?qu3ry#fr4gm3nt", acpl::Uri::vlUri) == true);
	Test(oUri.Build(oStr, acpl::Uri::vlUri) == true);
	Test(StringCompare(oStr.Utf8(), "sch3m3://D.23&47--23-3874/p4th?qu3ry#fr4gm3nt") == 0);
	
	Test(oUri.Parse("sch3m3://vD.2347--23-:3874/p4th?qu3ry#fr4gm3nt", acpl::Uri::vlUri) == true);
	Test(oUri.Build(oStr, acpl::Uri::vlUri) == true);
	Test(StringCompare(oStr.Utf8(), "sch3m3://vD.2347--23-:3874/p4th?qu3ry#fr4gm3nt") == 0);
	
	
	// URLs
	
	Test(oUri.Parse("sch3m3://[::127.0.0.1]/p4th?qu3ry#fr4gm3nt", acpl::Uri::vlUrl) == true);
	Test(oUri.Build(oStr, acpl::Uri::vlUrl) == true);
	Test(StringCompare(oStr.Utf8(), "sch3m3://[::127.0.0.1]/p4th?qu3ry#fr4gm3nt") == 0);
	
	Test(oUri.Parse("sch3m3://127.0.0.1/p4th?qu3ry#fr4gm3nt", acpl::Uri::vlUrl) == true);
	Test(oUri.Build(oStr, acpl::Uri::vlUrl) == true);
	Test(StringCompare(oStr.Utf8(), "sch3m3://127.0.0.1/p4th?qu3ry#fr4gm3nt") == 0);
	
	Test(oUri.Parse("sch3m3://d0m4in/p4th?qu3ry#fr4gm3nt", acpl::Uri::vlUrl) == true);
	Test(oUri.Build(oStr, acpl::Uri::vlUrl) == true);
	Test(StringCompare(oStr.Utf8(), "sch3m3://d0m4in/p4th?qu3ry#fr4gm3nt") == 0);
	
	Test(oUri.Parse("sch3m3://us3r%20inf0@d0m4in/p4th?qu3ry#fr4gm3nt", acpl::Uri::vlUrl) == true);
	Test(oUri.Build(oStr, acpl::Uri::vlUrl) == true);
	Test(StringCompare(oStr.Utf8(), "sch3m3://us3r%20inf0@d0m4in/p4th?qu3ry#fr4gm3nt") == 0);
	
	Test(oUri.Parse("sch3m3://us3r%20inf0@d0m4in:1234/p4th?qu3ry#fr4gm3nt", acpl::Uri::vlUrl) == true);
	Test(oUri.Build(oStr, acpl::Uri::vlUrl) == true);
	Test(StringCompare(oStr.Utf8(), "sch3m3://us3r%20inf0@d0m4in:1234/p4th?qu3ry#fr4gm3nt") == 0);
	
	Test(oUri.Parse("sch3m3://d0m4in:1234/p4th?qu3ry#fr4gm3nt", acpl::Uri::vlUrl) == true);
	Test(oUri.Build(oStr, acpl::Uri::vlUrl) == true);
	Test(StringCompare(oStr.Utf8(), "sch3m3://d0m4in:1234/p4th?qu3ry#fr4gm3nt") == 0);
	
	
	return 0;
}


SectionFuncMain(uri)
{
	if (argc < 1)
	{
		Test(TestPctEncDec() == 0);
		Test(TestKvStringParseBuild() == 0);
		Test(TestInitSetGet() == 0);
		Test(TestParseValidate() == 0);
		Test(TestBuildOnly() == 0);
		Test(TestParseBuild() == 0);
		
		PrintOut("All Passed!\n");
	}
	else
		return PrintUnrecParams(acplDbgSrcLoc, lSectionName, argc, argv);
	
	return 0;
}

SectionFuncParamsBegin(uri)
// SectionFuncParamsAdd("param [param [param ...]]", "description")
SectionFuncParamsEnd(uri)
