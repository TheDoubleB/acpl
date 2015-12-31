#include "Tests.h"

#include "TestsFsCmn.inc.cpp"

#include "../src/fs/File.h"


typedef acpl::UInt8 _local_Data[8];
void _local_DataReset(_local_Data &nData)
{
	for (acpl::SizeT i = sizeof(_local_Data); i-- > 0; )
		nData[i] = 0;
}
void _local_DataSet(_local_Data &nData, const char *nStr, acpl::SizeT nStrSize)
{
	for (acpl::SizeT i = sizeof(_local_Data); i-- > 0; )
		nData[i] = ((i < nStrSize) ? nStr[i] : 0);
}
bool _local_DataIsZero(const _local_Data &nData)
{
	for (acpl::SizeT i = sizeof(_local_Data); i-- > 0; )
		if (nData[i] != 0)
			return false;
	
	return true;
}
bool _local_DataCmp(const _local_Data &nData, const char *nStr, acpl::SizeT nStrSize)
{
	for (acpl::SizeT i = sizeof(_local_Data); i-- > 0; )
		if (i < nStrSize && nData[i] != nStr[i])
			return false;
	
	return true;
}
#define _local_DataSetCmp(N_DATA, N_CONSTDATA, N_DATASIZE) \
	_local_DataSet(N_DATA, N_CONSTDATA, N_DATASIZE); \
	Test(_local_DataCmp(N_DATA, N_CONSTDATA, N_DATASIZE) == true);


static int TestBasics()
{
	PrintFn();
	
	
	static const char *oConstData = "abcdefghi"; // 9 chars + NULL terminator
	_local_Data oData;
	
	_local_DataReset(oData);
	Test(_local_DataIsZero(oData) == true);
	
	_local_DataSetCmp(oData, oConstData, 0);
	_local_DataSetCmp(oData, oConstData, 1);
	_local_DataSetCmp(oData, oConstData, 2);
	_local_DataSetCmp(oData, oConstData, 3);
	_local_DataSetCmp(oData, oConstData, 4);
	_local_DataSetCmp(oData, oConstData, 5);
	_local_DataSetCmp(oData, oConstData, 6);
	_local_DataSetCmp(oData, oConstData, 7);
	_local_DataSetCmp(oData, oConstData, 8);
	_local_DataSetCmp(oData, oConstData, 9);
	_local_DataSetCmp(oData, oConstData, 10);
	
	
	acpl::File oFile("_testfile");
	
	Test(StringCompare(oFile.GetPath().GetStr(), "_testfile") == 0);
	
	
	return 0;
}

static int TestOpenCloseUnlink()
{
	PrintFn();
	
	
	acpl::File oFile;
	acpl::Path::Stat oStat;
	
	Test(oFile.IsOpen() == false);
	oFile.Close();
	Test(oFile.IsOpen() == false);
	
	Test(oFile.Open(acpl::File::amReadOnly) == false);
	Test(oFile.GetLastError() == acpl::File::lePathEmpty);
	
	Test(oFile.Unlink() == false);
	Test(oFile.GetLastError() == acpl::File::lePathEmpty);
	
	oFile.GetPath().Set(acpl::String::Ref(gArgv0, true)).Parent().Child("_testfile"); // Set file path to test file name
	
	if (oStat.DoStat(oFile.GetPath()) == true)
		Test(oFile.Unlink() == true);
	
	Test(oFile.Open(acpl::File::amNone) == false);
	Test(oFile.GetLastError() == acpl::File::leInvalAm);
	
	Test(oFile.Open(acpl::File::amReadOnly) == false);
	Test(oFile.GetLastError() == acpl::File::leNonExist);
	
	Test(oFile.Open(acpl::File::amReadOnly, true) == false); // attempt to open+truncate file witout write access (this fails before any (file)system call)
	Test(oFile.GetLastError() == acpl::File::leNoWrite);
	// NOTE: actual truncation will be tested in read/write tests
	
	Test(oFile.Open(acpl::File::amReadOnly, false, true, 0600) == true); // create the file with 0600 permission
	Test(oFile.IsOpen() == true);
	oFile.Close();
	Test(oFile.IsOpen() == false);
	Test(oStat.DoStat(oFile.GetPath()) == true);
	Test(oStat.GetPerm().GetValue() == _local_FilePerm(0600));
	
	Test(oFile.Open(acpl::File::amReadOnly, false, true, 0600) == true); // open previously created file (create params will be ignored)
	Test(oFile.IsOpen() == true);
	Test(oFile.Open(acpl::File::amReadOnly) == false); // try to reopen file
	Test(oFile.GetLastError() == acpl::File::leIsOpen);
	Test(oFile.IsOpen() == true);
	oFile.Close();
	Test(oFile.IsOpen() == false);
	
	Test(oFile.Unlink() == true);
	
	acpl::Path::SetUmask(0); // we need to check whether file will get the full 666 permission
	
	Test(oFile.Open(acpl::File::amReadOnly, false, true, 0666) == true); // create the file with 0600 permission
	Test(oFile.IsOpen() == true);
	oFile.Close();
	Test(oFile.IsOpen() == false);
	Test(oStat.DoStat(oFile.GetPath()) == true);
	Test(oStat.GetPerm().GetValue() == _local_FilePerm(0666));
	
	Test(oFile.GetPath().SetPerm(0644) == true);
	Test(oStat.DoStat(oFile.GetPath()) == true);
	Test(oStat.GetPerm().GetValue() == _local_FilePerm(0644));
	
	Test(oFile.Unlink() == true);
	Test(oStat.DoStat(oFile.GetPath()) == false);
	
	Test(oFile.GetPath().SetPerm(0644) == false);
	Test(oFile.GetPath().GetLastError() == acpl::Path::leNonExist);
	
	
	return 0;
}

static int TestReadWriteSeek()
{
	PrintFn();
	
	
	acpl::File oFile;
	acpl::Path::Stat oStat;
	acpl::OffT oOffset;
	acpl::SizeT oRwBytes;
	static const char *oConstData = "abcdefghi"; // 9 chars + NULL terminator
	_local_Data oData;
	
	Test(oFile.GetOffset(oOffset) == false);
	Test(oFile.GetLastError() == acpl::File::leNotOpen);
	Test(oFile.SetOffset(0, acpl::File::whCurrent, oOffset) == false);
	Test(oFile.GetLastError() == acpl::File::leNotOpen);
	Test(oFile.Write(NULL, 0, oRwBytes) == false);
	Test(oFile.GetLastError() == acpl::File::leNotOpen);
	Test(oFile.Read(NULL, 0, oRwBytes) == false);
	Test(oFile.GetLastError() == acpl::File::leNotOpen);
	
	oFile.GetPath().Set(acpl::String::Ref(gArgv0, true)).Parent().Child("_testfile"); // Set file path to test file name
	
	if (oStat.DoStat(oFile.GetPath()) == true)
		Test(oFile.Unlink() == true);
	
	Test(oFile.Open(acpl::File::amReadOnly, false, true, 0600) == true); // create the file with 0600 permission
	Test(oFile.IsOpen() == true);
	
	Test(oFile.GetOffset(oOffset) == true);
	Test(oOffset == 0);
	Test(oFile.SetOffset(0, acpl::File::whStart, oOffset) == true);
	Test(oOffset == 0);
	Test(oFile.SetOffset(0, acpl::File::whCurrent, oOffset) == true);
	Test(oOffset == 0);
	Test(oFile.SetOffset(0, acpl::File::whEnd, oOffset) == true);
	Test(oOffset == 0);
	Test(oFile.GetOffset(oOffset) == true);
	Test(oOffset == 0);
	
	Test(oFile.SetOffset(-1, acpl::File::whStart, oOffset) == false);
	Test(oFile.GetLastError() == acpl::File::leInvalOff);
	Test(oFile.SetOffset(-1, acpl::File::whCurrent, oOffset) == false);
	Test(oFile.GetLastError() == acpl::File::leInvalOff);
	Test(oFile.SetOffset(1, acpl::File::whCurrent, oOffset) == true); // Can move beyond the end
	Test(oOffset == 1);
	Test(oFile.GetOffset(oOffset) == true);
	Test(oOffset == 1);
	Test(oFile.SetOffset(1, acpl::File::whEnd, oOffset) == true);
	Test(oOffset == 1);
	Test(oFile.GetOffset(oOffset) == true);
	Test(oOffset == 1);
	Test(oFile.SetOffset(0, acpl::File::whStart, oOffset) == true);
	Test(oOffset == 0);
	Test(oFile.GetOffset(oOffset) == true);
	Test(oOffset == 0);
	// NOTE: testing with some ridiculously high value for offset (like `acpl::Num<acpl::OffT>::Max()`) cannot be done because glibc's seek fails but MSCRT allows it, but any subsequent read/write fails after that.
	// TODO: ^ Separate the tests on glibc/MSCRT parts
	
	_local_DataReset(oData);
	Test(oFile.Write(oData, sizeof(oData), oRwBytes) == false);
	Test(oFile.GetLastError() == acpl::File::leNoWrite);
	Test(oRwBytes == 0);
	Test(oFile.Read(oData, sizeof(oData), oRwBytes) == true);
	Test(oRwBytes == 0);
	Test(oFile.GetOffset(oOffset) == true);
	Test(oOffset == 0);
	
	oFile.Close();
	Test(oFile.Open(acpl::File::amReadWrite) == true); // open previously created file with R+W access
	Test(oFile.IsOpen() == true);
	
	_local_DataSet(oData, oConstData, sizeof(oData));
	Test(oFile.Write(oData, sizeof(oData), oRwBytes) == true);
	Test(oRwBytes == sizeof(oData));
	Test(oFile.GetOffset(oOffset) == true);
	Test(static_cast<acpl::SizeT>(oOffset) == sizeof(oData));
	
	_local_DataReset(oData);
	Test(oFile.SetOffset(0, acpl::File::whStart, oOffset) == true);
	Test(oOffset == 0);
	Test(oFile.Read(oData, sizeof(oData), oRwBytes) == true);
	Test(oRwBytes == sizeof(oData));
	Test(oFile.GetOffset(oOffset) == true);
	Test(static_cast<acpl::SizeT>(oOffset) == sizeof(oData));
	Test(_local_DataCmp(oData, oConstData, sizeof(oData)) == true);
	
	oFile.Close();
	Test(oFile.Open(acpl::File::amReadOnly) == true); // open previously created file with RO access
	Test(oFile.IsOpen() == true);
	
	_local_DataReset(oData);
	Test(oFile.SetOffset(0, acpl::File::whStart, oOffset) == true);
	Test(oOffset == 0);
	Test(oFile.Read(oData, sizeof(oData), oRwBytes) == true);
	Test(oRwBytes == sizeof(oData));
	Test(oFile.GetOffset(oOffset) == true);
	Test(static_cast<acpl::SizeT>(oOffset) == sizeof(oData));
	Test(_local_DataCmp(oData, oConstData, sizeof(oData)) == true);
	
	oFile.Close();
	Test(oFile.Open(acpl::File::amReadWrite, true) == true); // open previously created file with R+W access + truncate
	Test(oFile.IsOpen() == true);
	
	Test(oFile.Read(oData, sizeof(oData), oRwBytes) == true);
	Test(oRwBytes == 0);
	Test(oFile.GetOffset(oOffset) == true);
	Test(oOffset == 0);
	
	Test(oFile.SetOffset(sizeof(oData), acpl::File::whStart, oOffset) == true);
	Test(oOffset == sizeof(oData));
	Test(oFile.GetOffset(oOffset) == true);
	Test(oOffset == sizeof(oData));
	
	_local_DataSet(oData, oConstData, sizeof(oData));
	Test(oFile.Write(oData, sizeof(oData), oRwBytes) == true);
	Test(oRwBytes == sizeof(oData));
	Test(oFile.GetOffset(oOffset) == true);
	Test(static_cast<acpl::SizeT>(oOffset) == (sizeof(oData) * 2));
	
	Test(oFile.SetOffset(0, acpl::File::whStart, oOffset) == true);
	Test(oOffset == 0);
	Test(oFile.GetOffset(oOffset) == true);
	Test(oOffset == 0);
	
	_local_DataReset(oData);
	Test(oFile.Read(oData, sizeof(oData), oRwBytes) == true);
	Test(oRwBytes == sizeof(oData));
	Test(oFile.GetOffset(oOffset) == true);
	Test(oOffset == sizeof(oData));
	Test(_local_DataIsZero(oData) == true);
	
	_local_DataReset(oData);
	Test(oFile.Read(oData, sizeof(oData), oRwBytes) == true);
	Test(oRwBytes == sizeof(oData));
	Test(oFile.GetOffset(oOffset) == true);
	Test(oOffset == (sizeof(oData) * 2));
	Test(_local_DataCmp(oData, oConstData, sizeof(oData)) == true);
	
	Test(oFile.SetOffset(sizeof(oData)) == true);
	Test(oFile.GetOffset(oOffset) == true);
	Test(oOffset == sizeof(oData));
	Test(oFile.SetOffset(-1, acpl::File::whCurrent) == true);
	Test(oFile.GetOffset(oOffset) == true);
	Test(oOffset == (sizeof(oData) - 1));
	Test(oFile.SetOffset(-1, acpl::File::whEnd) == true);
	Test(oFile.GetOffset(oOffset) == true);
	Test(oOffset == ((sizeof(oData) * 2) - 1));
	
	oFile.Close();
	Test(oFile.Unlink() == true);
	Test(oStat.DoStat(oFile.GetPath()) == false);
	
	
	return 0;
}


SectionFuncMain(fsfile)
{
	if (argc < 1)
	{
		Test(TestBasics() == 0);
		Test(TestOpenCloseUnlink() == 0);
		Test(TestReadWriteSeek() == 0);
		
		PrintOut("All Passed!\n");
	}
	else
		return PrintUnrecParams(acplDbgSrcLoc, lSectionName, argc, argv);
	
	return 0;
}

SectionFuncParamsBegin(fsfile)
// SectionFuncParamsAdd("param [param [param ...]]", "description")
SectionFuncParamsEnd(fsfile)
