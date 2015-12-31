#include "Tests.h"

#include "../src/Io.h"


class _local_IoChannelImpl : public acpl::IoChannel
{
	
	private:
		acpl::UInt8 mCntRead;
		acpl::UInt8 mCntWrite;
	
	public:
		_local_IoChannelImpl()
		{
			this->Reset();
		}
		
		void Reset()
		{
			this->mCntRead = 0;
			this->mCntWrite = 0;
		}
		
		acpl::UInt8 GetCntRead()
		{
			return this->mCntRead;
		}
		
		acpl::UInt8 GetCntWrite()
		{
			return this->mCntWrite;
		}
		
		virtual bool Read(void *nData, acpl::SizeT nDataSize, acpl::SizeT &nBytesRead)
		{
			if (nDataSize > 0)
			{
				*static_cast<acpl::UInt8 *>(nData) = 0x47;
				nBytesRead = 1;
			}
			else
				nBytesRead = 0;
			
			this->mCntRead++;
			
			return true;
		}
		
		virtual bool Write(const void *, acpl::SizeT nDataSize, acpl::SizeT &nBytesWritten)
		{
			if (nDataSize > 0)
				nBytesWritten = 1;
			else
				nBytesWritten = 0;
			
			this->mCntWrite++;
			
			return true;
		}
	
};


static int TestIoChannel()
{
	PrintFn();
	
	
	_local_IoChannelImpl oTioc;
	acpl::UInt8 oBfr[10];
	
	
	oTioc.Reset();
	Test(oTioc.ReadAll(oBfr, sizeof(oBfr)) == true);
	Test(oTioc.WriteAll(oBfr, sizeof(oBfr)) == true);
	
	
	return 0;
}

static int TestIoNull()
{
	PrintFn();
	
	
	acpl::IoNull oNio;
	acpl::UInt8 oBfr[10];
	acpl::SizeT oBytesProc;
	
	
	Test(oNio.Read(oBfr, sizeof(oBfr), oBytesProc) == false);
	Test(oNio.Write(oBfr, sizeof(oBfr), oBytesProc) == true);
	Test(oBytesProc == sizeof(oBfr));
	
	
	return 0;
}

static int TestIoStd()
{
	PrintFn();
	
	
	acpl::IoStd oSio;
	acpl::SizeT oBytesProc;
	
	
	// Can't test Read, ReadAll, ReadIn, and ReadInAll
	
	Test(acpl::IoStd::WriteOut("IoStd::WriteOut\n", 16, oBytesProc) == true);
	Test(acpl::IoStd::WriteOutAll("IoStd::WriteOutAll\n", 19) == true);
	Test(acpl::IoStd::WriteErr("IoStd::WriteErr\n", 16, oBytesProc) == true);
	Test(acpl::IoStd::WriteErrAll("IoStd::WriteErrAll\n", 19) == true);
	
	Test(oSio.WriteOut("oSio.WriteOut\n", 14, oBytesProc) == true);
	Test(oSio.WriteOutAll("oSio.WriteOutAll\n", 17) == true);
	Test(oSio.WriteErr("oSio.WriteErr\n", 14, oBytesProc) == true);
	Test(oSio.WriteErrAll("oSio.WriteErrAll\n", 17) == true);
	
	Test(oSio.Write("oSio.Write\n", 11, oBytesProc) == true);
	Test(oSio.WriteAll("oSio.WriteAll\n", 14) == true);
	
	
	return 0;
}


SectionFuncMain(io)
{
	if (argc < 1)
	{
		Test(TestIoChannel() == 0);
		Test(TestIoNull() == 0);
		Test(TestIoStd() == 0);
		
		PrintOut("All Passed!\n");
	}
	else
		return PrintUnrecParams(acplDbgSrcLoc, lSectionName, argc, argv);
	
	return 0;
}

SectionFuncParamsBegin(io)
// SectionFuncParamsAdd("param [param [param ...]]", "description")
SectionFuncParamsEnd(io)
