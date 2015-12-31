#include "Tests.h"

#include "TestsFsCmn.inc.cpp"

#include "../src/fs/Dir.h"
#include "../src/Time.h"


static bool _local_DirFilterListEntries(const acpl::Dir::Entry &nEntry)
{
	const acpl::Pathchar *oName = nEntry.GetName().GetStr();
	
	if (oName[0] == _local_PathcharPx('.'))
	{
		if (oName[1] == _local_PathcharPx('\0'))
			return false;
		else
		if (oName[1] == _local_PathcharPx('.') && oName[2] == _local_PathcharPx('\0'))
			return false;
	}
	
	return true;
}

static int _local_TestExeDirEntries(const acpl::Dir::EntryList &nEntries, bool nStatted, bool nFiltered)
{
	// NOTE: There should be at least 3 entries (not in any particular order):
	//   * [ DIR  ] "."
	//   * [ DIR  ] ".."
	//   * [ FILE ] "testacpl" or "testacpl.exe"
	// 
	// NOTE: nStatted notifies whether or not entries have been statted
	// NOTE: nFiltered notifies whether or not "." and ".." should be filtered out
	
	bool oCurDirFound = false;
	bool oParentDirFound = false;
	bool oExeFileFound = false;
	acpl::TimeT oNow = acpl::Time::UnixTime();
	
	Test(nEntries.Count() >= static_cast<acpl::SizeT>((nFiltered == true) ? 1 : 3));
	
	for (acpl::SizeT i = 0; i < nEntries.Count(); i++)
	{
		const acpl::Dir::Entry &oCurEntry = nEntries.Get(i);
		const acpl::Pathchar *oCurName = oCurEntry.GetName().GetStr();
		const acpl::Path::Permission &oCurAm = oCurEntry.GetPerm();
		
		// Check for statted status first
		TestFM(oCurEntry.IsStatted() == nStatted, "name=" _local_PathcharFmt "", oCurName);
		
		if (oCurName[0] == _local_PathcharPx('.'))
		{
			if (oCurName[1] == _local_PathcharPx('\0'))
			{
				TestFM(oCurEntry.GetType() == acpl::Dir::Entry::tDir, "name=" _local_PathcharFmt "", oCurName);
				oCurDirFound = true;
				
				if (nStatted == true) // Current dir has to be at least (drwx------)
					TestFM((oCurAm.CanUsrRead() == true && oCurAm.CanUsrWrite() == true && oCurAm.CanUsrExec() == true) != 0, "name=" _local_PathcharFmt "", oCurName)
			}
			else
			if (oCurName[1] == _local_PathcharPx('.') && oCurName[2] == _local_PathcharPx('\0'))
			{
				TestFM(oCurEntry.GetType() == acpl::Dir::Entry::tDir, "name=" _local_PathcharFmt "", oCurName);
				oParentDirFound = true;
				
				if (nStatted == true) // Parent dir has to be at least (dr-x------)
					TestFM((oCurAm.CanUsrRead() == true && oCurAm.CanUsrExec() == true) != 0, "name=" _local_PathcharFmt "", oCurName)
			}
		}
		else
		if (StringCompare(oCurName, AppName) == 0)
		{
			TestFM(oCurEntry.GetType() == acpl::Dir::Entry::tFile, "name=" _local_PathcharFmt "", oCurName);
			oExeFileFound = true;
			
			if (nStatted == true) // This exe file has to be at least (-rwx------)
				TestFM((oCurAm.CanUsrRead() == true && oCurAm.CanUsrWrite() == true && oCurAm.CanUsrExec() == true) != 0, "name=" _local_PathcharFmt "", oCurName)
		}
		
		if (nStatted == true)
		{
			if (oCurEntry.GetType() == acpl::Dir::Entry::tFile)
				TestFM(oCurEntry.GetSize() > 0, "name=" _local_PathcharFmt "", oCurName)
			
			TestFM(oCurEntry.GetTimeCre() > 0, "name=" _local_PathcharFmt "", oCurName);
			TestFM(oCurEntry.GetTimeCre() <= oNow, "name=" _local_PathcharFmt "", oCurName);
			TestFM(oCurEntry.GetTimeMod() > 0, "name=" _local_PathcharFmt "", oCurName);
			TestFM(oCurEntry.GetTimeMod() <= oNow, "name=" _local_PathcharFmt "", oCurName);
			TestFM(oCurEntry.GetTimeAcc() > 0, "name=" _local_PathcharFmt "", oCurName);
			TestFM(oCurEntry.GetTimeAcc() <= oNow, "name=" _local_PathcharFmt "", oCurName);
		}
	}
	
	
	Test(oExeFileFound == true);
	
	if (nFiltered == true)
	{
		Test(oCurDirFound == false);
		Test(oParentDirFound == false);
	}
	else
	{
		Test(oCurDirFound == true);
		Test(oParentDirFound == true);
	}
	
	return 0;
}


static int TestBasics()
{
	PrintFn();
	
	
	acpl::Dir oDir("testdir"); // no system-specific delimiters
	
	Test(StringCompare(oDir.GetPath().GetStr(), "testdir") == 0);
	
	
	return 0;
}

static int TestListEtries()
{
	PrintFn();
	
	
	acpl::Dir oDir;
	acpl::Dir::EntryList oEntries;
	
	
	// Init
	
	Test(oDir.GetLastError() == acpl::Dir::leNone);
	
	
	// Empty path
	Test(oDir.ListEntries(oEntries, false, NULL) == false);
	Test(oDir.GetLastError() == acpl::Dir::lePathEmpty);
	
	
	// Non-existant path
	oDir.GetPath().Set("_nonexistantpath");
	Test(oDir.ListEntries(oEntries, false, NULL) == false);
	Test(oDir.GetLastError() == acpl::Dir::leNonExist);
	
	
	// This test exe
	oDir.GetPath().Set(acpl::String::Ref(gArgv0, true)); // Set dir path to this exe
	Test(oDir.ListEntries(oEntries, false, NULL) == false);
	Test(oDir.GetLastError() == acpl::Dir::leNotDir);
	
	
	// Entries in dir of this test exe
	oDir.GetPath().Parent(); // Set dir path to this exe's parent dir
	Test(oDir.ListEntries(oEntries, false, NULL) == true);
	Test(_local_TestExeDirEntries(oEntries, false, false) == 0);
	
	
	// Entries in dir of this test exe with "." and ".." filtered out
	Test(oDir.ListEntries(oEntries, false, _local_DirFilterListEntries) == true);
	Test(_local_TestExeDirEntries(oEntries, false, true) == 0);
	
	
	// Statted entries in dir of this test exe (including "." and "..")
	Test(oDir.ListEntries(oEntries, true, NULL) == true);
	Test(_local_TestExeDirEntries(oEntries, true, false) == 0);
	
	
	// Reset Error
	oDir.ResetLastError();
	Test(oDir.GetLastError() == acpl::Dir::leNone);
	
	
	return 0;
}

static int TestCreateRemove()
{
	PrintFn();
	
	
	acpl::Dir oDir;
	acpl::Path::Stat oStat;
	
	
	Test(oDir.Create() == false);
	Test(oDir.GetLastError() == acpl::Dir::lePathEmpty);
	
	Test(oDir.Remove() == false);
	Test(oDir.GetLastError() == acpl::Dir::lePathEmpty);
	
	oDir.GetPath().Set(acpl::String::Ref(gArgv0, true)).Parent().Child("_testdir").Child("_testsubdir"); // Set dir path to test dir names
	
	if (oStat.DoStat(oDir.GetPath()) == true)
		Test(oDir.Remove() == true);
	
	oDir.GetPath().Parent();
	if (oStat.DoStat(oDir.GetPath()) == true)
		Test(oDir.Remove() == true);
	
	Test(oDir.Create(0700) == true);
	Test(oStat.DoStat(oDir.GetPath()) == true);
	Test(oStat.GetPerm().GetValue() == _local_DirPerm(0700));
	
	Test(oDir.Create(0700) == false);
	Test(oDir.GetLastError() == acpl::Dir::leExists);
	
	acpl::Path::SetUmask(0); // we need to check whether dir will get the full 777 permission
	
	oDir.GetPath().Child("_testsubdir");
	Test(oDir.Create(0777) == true);
	Test(oStat.DoStat(oDir.GetPath()) == true);
	Test(oStat.GetPerm().GetValue() == _local_DirPerm(0777));
	
	Test(oDir.GetPath().SetPerm(0755) == true);
	Test(oStat.DoStat(oDir.GetPath()) == true);
	Test(oStat.GetPerm().GetValue() == _local_DirPerm(0755));
	
	oDir.GetPath().Parent();
	Test(oDir.Remove() == false); // Cannot remove a non-empty directory (it includes a sub-directory "_testsubdir")
	Test(oDir.GetLastError() == acpl::Dir::leNotEmpty);
	
	oDir.GetPath().Child("_testsubdir");
	Test(oDir.Remove() == true);
	Test(oStat.DoStat(oDir.GetPath()) == false);
	
	Test(oDir.GetPath().SetPerm(0755) == false);
	Test(oDir.GetPath().GetLastError() == acpl::Path::leNonExist);
	
	oDir.GetPath().Parent();
	Test(oDir.Remove() == true);
	Test(oStat.DoStat(oDir.GetPath()) == false);
	
	Test(oDir.Remove() == false);
	Test(oDir.GetLastError() == acpl::Dir::leNonExist);
	
	// TODO: Test/decide what to do as far as error handling for last element of '.' or '..'
	
	return 0;
}


SectionFuncMain(fsdir)
{
	if (argc < 1)
	{
		Test(TestBasics() == 0);
		Test(TestListEtries() == 0);
		Test(TestCreateRemove() == 0);
		
		PrintOut("All Passed!\n");
	}
	else
		return PrintUnrecParams(acplDbgSrcLoc, lSectionName, argc, argv);
	
	return 0;
}

SectionFuncParamsBegin(fsdir)
// SectionFuncParamsAdd("param [param [param ...]]", "description")
SectionFuncParamsEnd(fsdir)
