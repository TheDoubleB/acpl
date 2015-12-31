#ifndef ACPL_TIME_H
#define ACPL_TIME_H

#include "Macros.h"
#include "Types.h"

namespace acpl
{
	class Time
	{
			acplHideClassCopy(Time);
		
		private:
			inline Time() { }
			inline ~Time() { }
		
		public:
			static acpl::TimeT UnixTime();
			static acpl::UInt64 Monotone();
			
			static void SleepSec(acpl::UInt32 nSecs);
			static void SleepMsec(acpl::UInt32 nMsecs);
			static void SleepUsec(acpl::UInt64 nUsecs);
		
	};
	
	class DateTime
	{
			acplHideClassCopy(DateTime);

		private:
			acpl::UInt64 mDt;
			
			inline void Set(acpl::TimeT) { } // what you're looking for is `SetUnixTime()`
		
		public:
			DateTime();
			DateTime(bool nIsLocal);
			virtual ~DateTime();
			
			bool IsClear() const;
			acpl::DateTime &Clear();
			acpl::DateTime &Set(const acpl::DateTime &nDateTime);
			acpl::DateTime &Now();
			acpl::TimeT GetUnixTime() const;
			acpl::DateTime &SetUnixTime(acpl::TimeT nUnixTime);
			
			acpl::DateTime &Normalize();
			
			bool IsUtc() const;
			acpl::DateTime &ToUtc();
			bool IsLocal() const;
			acpl::DateTime &ToLocal();
			
			acpl::DateTime &SetDate(acpl::SInt32 nYear, acpl::UInt8 nMonth, acpl::UInt8 nDay);
			acpl::SInt32 GetYear() const;
			acpl::DateTime &SetYear(acpl::SInt32 nYear);
			acpl::UInt8 GetMonth() const;
			acpl::DateTime &SetMonth(acpl::UInt8 nMonth);
			acpl::UInt16 GetDayOfYear() const;
			acpl::UInt8 GetDayOfWeek() const;
			acpl::UInt8 GetDay() const;
			acpl::DateTime &SetDay(acpl::UInt8 nDay);
			bool IsDst() const;
			acpl::DateTime &SetTime(acpl::UInt8 nHour, acpl::UInt8 nMin, acpl::UInt8 nSec);
			acpl::UInt8 GetHour() const;
			acpl::DateTime &SetHour(acpl::UInt8 nHour);
			acpl::UInt8 GetMin() const;
			acpl::DateTime &SetMin(acpl::UInt8 nMin);
			acpl::UInt8 GetSec() const;
			acpl::DateTime &SetSec(acpl::UInt8 nSec);
			
			// TODO: Add `{AddTo,SubFrom}{Date,Year,Month,Day,Time,Hour,Min,Sec}` methods
	};
	
	
	class Timer
	{
			acplHideClassCopy(Timer);
		
		private:
			acpl::UInt64 mTimeoutAbs;
		
		public:
			Timer();
			virtual ~Timer();
			
			void SetupSec(acpl::UInt32 nSecs);
			void SetupMsec(acpl::UInt32 nMsecs);
			void SetupUsec(acpl::UInt64 nUsecs);
			bool IsElapsed() const;
		
	};
}

#endif // ACPL_TIME_H
