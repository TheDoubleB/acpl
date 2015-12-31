#include "Tests.h"

#include "../src/Time.h"


static int TestTime()
{
	PrintFn();
	
	
	acpl::UInt64 oUtime1, oUtime2;
	acpl::TimeT oTimet1, oTimet2;
	
	
	// Time retriever functions
	
	oTimet1 = acpl::Time::UnixTime();
	Test(oTimet1 > 0);
	oTimet2 = acpl::Time::UnixTime();
	Test(oTimet2 > 0);
	Test(oTimet2 >= oTimet1);
	
	oUtime1 = acpl::Time::Monotone();
	Test(oUtime1 > 0);
	oUtime2 = acpl::Time::Monotone();
	Test(oUtime2 > 0);
	Test(oUtime2 >= oUtime1);
	
	
	// Sleep functions
	
	oUtime1 = acpl::Time::Monotone();
	acpl::Time::SleepSec(1); // 1 sec
	oUtime2 = acpl::Time::Monotone();
	Test(oUtime2 >= (oUtime1 + 1000000));
	
	oUtime1 = acpl::Time::Monotone();
	acpl::Time::SleepMsec(100); // 0.1 sec
	oUtime2 = acpl::Time::Monotone();
	Test(oUtime2 >= (oUtime1 + 100000));
	
	oUtime1 = acpl::Time::Monotone();
	acpl::Time::SleepUsec(100000); // 0.1 sec
	oUtime2 = acpl::Time::Monotone();
	Test(oUtime2 >= (oUtime1 + 100000));
	
	
	return 0;
}

static int _local_CompareDt(
		const acpl::DateTime &nDt,
		acpl::SInt32 nYear, acpl::UInt8 nMonth, acpl::UInt8 nDay,
		acpl::UInt16 nDoy, acpl::UInt16 nDow, bool nIsDst,
		acpl::UInt8 nHour, acpl::UInt8 nMin, acpl::UInt8 nSec)
{
	TestFM(nDt.GetYear()  == nYear,    "year: %d != %d", nDt.GetYear(),  nYear);
	TestFM(nDt.GetMonth() == nMonth,  "month: %u != %u", nDt.GetMonth(), nMonth);
	TestFM(nDt.GetDay()   == nDay,      "day: %u != %u", nDt.GetDay(),   nDay);
	
	TestFM(nDt.GetDayOfYear() == nDoy,  "doy: %u != %u", nDt.GetDayOfYear(), nDoy);
	TestFM(nDt.GetDayOfWeek() == nDow,  "dow: %u != %u", nDt.GetDayOfWeek(), nDow);
	TestFM(nDt.IsDst() == nIsDst,		"dst: %s != %s", ((nDt.IsDst() == true) ? "true" : "false"), ((nIsDst == true) ? "true" : "false"));
	
	TestFM(nDt.GetHour()  == nHour,    "hour: %u != %u", nDt.GetHour(),  nHour);
	TestFM(nDt.GetMin()   == nMin,   "minute: %u != %u", nDt.GetMin(),   nMin);
	TestFM(nDt.GetSec()   == nSec,   "second: %u != %u", nDt.GetSec(),   nSec);
	
	return 0;
}

static int TestDateTime()
{
	PrintFn();
	
	
	acpl::DateTime oDt, oDtAlt;
	acpl::TimeT oTimet;
	
	
	// Init
	
	Test(oDt.IsClear() == true);
	Test(oDt.IsUtc() == true);
	Test(oDt.IsLocal() == false);
	Test(oDt.GetYear() == 0);
	Test(oDt.GetMonth() == 0);
	Test(oDt.GetDayOfYear() == 0);
	Test(oDt.GetDayOfWeek() == 0);
	Test(oDt.GetDay() == 0);
	Test(oDt.IsDst() == true);
	Test(oDt.GetHour() == 0);
	Test(oDt.GetMin() == 0);
	Test(oDt.GetSec() == 0);
	
	
	// To{Utc,Local} test on clear
	
	oDt.ToLocal();
	Test(oDt.IsClear() == true);
	Test(oDt.IsLocal() == true);
	oDt.ToUtc();
	Test(oDt.IsClear() == true);
	Test(oDt.IsUtc() == true);
	
	
	// To{Utc,Local} test on live
	
	oDt.Clear().ToLocal().Now();
	oTimet = oDt.GetUnixTime();
	oDt.ToLocal();
	Test(oTimet == oDt.GetUnixTime());
	
	oDt.Clear().ToUtc().Now();
	oTimet = oDt.GetUnixTime();
	oDt.ToUtc();
	Test(oTimet == oDt.GetUnixTime());
	
	oDt.Clear().ToLocal().Now();
	oTimet = oDt.GetUnixTime();
	oDt.ToUtc().ToLocal();
	Test(oTimet == oDt.GetUnixTime());
	
	oDt.Clear().ToUtc().Now();
	oTimet = oDt.GetUnixTime();
	oDt.ToLocal().ToUtc();
	Test(oTimet == oDt.GetUnixTime());
	
	
	// Setters brute force
	
	for (acpl::UInt32 i = 0; i <= 0x7FFFFF; i++)
	{
		oDt.SetYear(i).SetMonth(i).SetDay(i).SetHour(i).SetMin(i).SetSec(i);
		
		TestFM(oDt.GetYear()	== static_cast<acpl::SInt32>(i), "i=%u", i);
		TestFM(oDt.GetMonth()	== (i & 0x0F), "i=%u", i);
		TestFM(oDt.GetDay()		== (i & 0x1F), "i=%u", i);
		TestFM(oDt.GetHour()	== (i & 0x1F), "i=%u", i);
		TestFM(oDt.GetMin()		== (i & 0x3F), "i=%u", i);
		TestFM(oDt.GetSec()		== (i & 0x3F), "i=%u", i);
		
		oDt.SetDate(i, i, i).SetTime(i, i, i);
		
		TestFM(oDt.GetYear()	== static_cast<acpl::SInt32>(i), "i=%u", i);
		TestFM(oDt.GetMonth()	== (i & 0x0F), "i=%u", i);
		TestFM(oDt.GetDay()		== (i & 0x1F), "i=%u", i);
		TestFM(oDt.GetHour()	== (i & 0x1F), "i=%u", i);
		TestFM(oDt.GetMin()		== (i & 0x3F), "i=%u", i);
		TestFM(oDt.GetSec()		== (i & 0x3F), "i=%u", i);
	}
	
	for (acpl::SInt32 i = (0x7FFFFF * -1); i < 0 ; i++)
	{
		oDt.SetYear(i);
		TestFM(oDt.GetYear() == i, "i=%d", i);
	}
	
	for (acpl::SInt32 i = 0x7FFFFF; i >= 0 ; i--)
	{
		oDt.SetYear(i);
		TestFM(oDt.GetYear() == i, "i=%d", i);
	}
	
	
	// From && to unix time
	
	oDt.Clear();
	oDt.ToUtc();
	oTimet = acpl::Time::UnixTime(); // in UTC
	oDt.SetUnixTime(oTimet);
	Test(oTimet == oDt.GetUnixTime());
	oDt.ToLocal();
	Test(oTimet == oDt.GetUnixTime());
	
	oTimet = acpl::Time::UnixTime(); // in UTC
	oDt.SetUnixTime(oTimet);
	Test(oTimet == oDt.GetUnixTime());
	oDt.ToUtc();
	Test(oTimet == oDt.GetUnixTime());
	
	
	// Normalize
	
	oDt.Clear();
	oDt.SetDate(2012, 7, 2);
	oDt.SetTime(23, 59, 59);
	oDt.Normalize();
	Test(_local_CompareDt(oDt, 2012, 7, 2, 183, 1, false, 23, 59, 59) == 0);
	oDt.SetSec(60);
	oDt.Normalize();
	Test(_local_CompareDt(oDt, 2012, 7, 3, 184, 2, false, 0, 0, 0) == 0);
	
	oDt.Clear();
	oDt.ToLocal();
	oDt.SetDate(2012, 7, 2);
	oDt.SetTime(23, 59, 59);
	oDt.Normalize();
	Test(_local_CompareDt(oDt, 2012, 7, 2, 183, 1, true, 23, 59, 59) == 0);
	oDt.SetSec(60);
	oDt.Normalize();
	Test(_local_CompareDt(oDt, 2012, 7, 3, 184, 2, true, 0, 0, 0) == 0);
	
	oDt.Clear();
	oDt.SetDate(2011, 2, 28);
	oDt.SetTime(23, 59, 59);
	oDt.Normalize();
	Test(_local_CompareDt(oDt, 2011, 2, 28, 58, 1, false, 23, 59, 59) == 0);
	oDt.SetSec(60);
	oDt.Normalize();
	Test(_local_CompareDt(oDt, 2011, 3, 1, 59, 2, false, 0, 0, 0) == 0);
	
	oDt.Clear();
	oDt.ToUtc();
	oDt.SetDate(2011, 2, 28);
	oDt.SetTime(23, 59, 59);
	oDt.Normalize();
	Test(_local_CompareDt(oDt, 2011, 2, 28, 58, 1, false, 23, 59, 59) == 0);
	oDt.SetSec(60);
	oDt.Normalize();
	Test(_local_CompareDt(oDt, 2011, 3, 1, 59, 2, false, 0, 0, 0) == 0);
	
	
	return 0;
}

static int TestTimer()
{
	PrintFn();
	
	
	acpl::Timer oTimer;
	
	
	Test(oTimer.IsElapsed() == true);
	
	oTimer.SetupSec(1);
	Test(oTimer.IsElapsed() == false);
	acpl::Time::SleepMsec(500);
	Test(oTimer.IsElapsed() == false);
	acpl::Time::SleepMsec(501);
	Test(oTimer.IsElapsed() == true);
	acpl::Time::SleepMsec(50);
	Test(oTimer.IsElapsed() == true);
	
	oTimer.SetupMsec(100);
	Test(oTimer.IsElapsed() == false);
	acpl::Time::SleepMsec(50);
	Test(oTimer.IsElapsed() == false);
	acpl::Time::SleepMsec(51);
	Test(oTimer.IsElapsed() == true);
	acpl::Time::SleepMsec(5);
	Test(oTimer.IsElapsed() == true);
	
	oTimer.SetupUsec(100000);
	Test(oTimer.IsElapsed() == false);
	acpl::Time::SleepMsec(50);
	Test(oTimer.IsElapsed() == false);
	acpl::Time::SleepMsec(51);
	Test(oTimer.IsElapsed() == true);
	acpl::Time::SleepMsec(5);
	Test(oTimer.IsElapsed() == true);
	
	
	return 0;
}

static void _local_PrintDtComponents(const acpl::DateTime &nDt)
{
	PrintOut("Components:\n");
	PrintOut("    Zone:  %s\n", ((nDt.IsLocal() == true) ? "Local" : "UTC"));
	PrintOut("    Unix:  %ld\n", nDt.GetUnixTime());
	PrintOut("    Date:  %d-%02u-%02u (doy=%u, dow=%u)\n", nDt.GetYear(), nDt.GetMonth(), nDt.GetDay(), nDt.GetDayOfYear(), nDt.GetDayOfWeek());
	PrintOut("    Time:  %02u:%02u:%02u (dst=%s)\n", nDt.GetHour(), nDt.GetMin(), nDt.GetSec(), ((nDt.IsDst() == true) ? "true" : "false"));
}

static int PrintDateTimeNow(bool nIsLocal)
{
	acpl::DateTime oDt(nIsLocal);
	oDt.Now();
	
	_local_PrintDtComponents(oDt);
	
	return 0;
}

static int PrintDateTimeNowBoth()
{
	acpl::DateTime oDt;
	
	oDt.ToLocal();
	oDt.Now();
	Test(oDt.IsUtc() == false);
	Test(oDt.IsLocal() == true);
	_local_PrintDtComponents(oDt);
	
	oDt.Clear();
	oDt.ToUtc();
	oDt.Now();
	Test(oDt.IsUtc() == true);
	Test(oDt.IsLocal() == false);
	_local_PrintDtComponents(oDt);
	
	oDt.ToLocal();
	_local_PrintDtComponents(oDt);
	
	oDt.ToUtc();
	_local_PrintDtComponents(oDt);
	
	return 0;
}


SectionFuncMain(time)
{
	if (argc < 1)
	{
		Test(TestTime() == 0);
		Test(TestDateTime() == 0);
		Test(TestTimer() == 0);
		
		PrintOut("All Passed!\n");
	}
	else
	if ((argc == 1 || argc == 2) && StringCompare(argv[0], "dtnow") == 0)
	{
		if (argc == 2)
		{
			if (StringCompare(argv[1], "utc") == 0)
				Test(PrintDateTimeNow(false) == 0)
			else
			if (StringCompare(argv[1], "local") == 0)
				Test(PrintDateTimeNow(true) == 0)
			else
			{
				PrintErr(acplDbgSrcLoc ": error: unrecognized zone parameter: %s\n", argv[1]);
				return 1;
			}
		}
		else
			Test(PrintDateTimeNowBoth() == 0);
	}
	else
		return PrintUnrecParams(acplDbgSrcLoc, lSectionName, argc, argv);
	
	return 0;
}

SectionFuncParamsBegin(time)
SectionFuncParamsAdd("\"dtnow\"", "printout of DateTime::Now() function")
// SectionFuncParamsAdd("param [param [param ...]]", "description")
SectionFuncParamsEnd(time)
