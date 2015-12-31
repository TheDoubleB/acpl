#include "Time.h"

#include "_cp/Parallel.inc.cpp"

//
// Time
//

acpl::TimeT acpl::Time::UnixTime() // static
{
	return acpl::cp::_time_ts();
}

acpl::UInt64 acpl::Time::Monotone() // static
{
	return acpl::cp::_time_monotone();
}


void acpl::Time::SleepSec(acpl::UInt32 nSecs) // static
{
	acpl::cp::_time_microsleep(static_cast<acpl::UInt64>(nSecs) * static_cast<acpl::UInt64>(1000000));
}

void acpl::Time::SleepMsec(acpl::UInt32 nMsecs) // static
{
	acpl::cp::_time_microsleep(static_cast<acpl::UInt64>(nMsecs) * static_cast<acpl::UInt64>(1000));
}

void acpl::Time::SleepUsec(acpl::UInt64 nUsecs) // static
{
	acpl::cp::_time_microsleep(nUsecs);
}



// 
// DateTime
// 

// Description of `acpl::DateTime::mDt` member (first number in brackets is the
// number of bits used by the field):
//    
//     ( 1) - is local flag       (0 = UTC, 1 = local)
//     ( 9) - day (of the year)   (1 - 366)
//     ( 3) - day (of the week)   (1 - 7)
//     (23) - year                (8M)
//     ( 1) - year sign (+/-)
//     ( 4) - month               (1 - 12)
//     ( 5) - day (of the month)  (1 - 31)
//     ( 1) - negated DST flag
//     ( 5) - hours               (0 - 23)
//     ( 6) - minutes             (0 - 59)
//     ( 6) - seconds             (0 - 60)

acpl::DateTime::DateTime()
{
	this->mDt = 0;
}

acpl::DateTime::DateTime(bool nIsLocal)
{
	this->mDt = 0;
	acpl::pi::_time_dt_set_local(this->mDt, nIsLocal);
}

acpl::DateTime::~DateTime()
{
	
}


bool acpl::DateTime::IsClear() const
{
	return acpl::pi::_time_dt_is_clear(this->mDt);
}

acpl::DateTime &acpl::DateTime::Clear()
{
	acpl::pi::_time_dt_clear(this->mDt);
	return *this;
}

acpl::DateTime &acpl::DateTime::Set(const acpl::DateTime &nDateTime)
{
	this->mDt = nDateTime.mDt;
	return *this;
}

acpl::DateTime &acpl::DateTime::Now()
{
	acpl::cp::_time_dt_from_timet(this->mDt, acpl::cp::_time_ts(), acpl::pi::_time_dt_is_local(this->mDt));
	return *this;
}

acpl::TimeT acpl::DateTime::GetUnixTime() const
{
	acpl::TimeT oVal;
	acpl::cp::_time_dt_to_timet(oVal, this->mDt, acpl::pi::_time_dt_is_local(this->mDt));
	return oVal;
}

acpl::DateTime &acpl::DateTime::SetUnixTime(acpl::TimeT nUnixTime)
{
	acpl::cp::_time_dt_from_timet(this->mDt, nUnixTime, acpl::pi::_time_dt_is_local(this->mDt));
	return *this;
}


acpl::DateTime &acpl::DateTime::Normalize()
{
	acpl::cp::_time_dt_normalize(this->mDt, acpl::pi::_time_dt_is_local(this->mDt));
	return *this;
}


bool acpl::DateTime::IsUtc() const
{
	return !acpl::pi::_time_dt_is_local(this->mDt);
}

acpl::DateTime &acpl::DateTime::ToUtc()
{
	if (acpl::pi::_time_dt_is_clear(this->mDt) != true &&
		acpl::pi::_time_dt_is_local(this->mDt) == true)
	{
		acpl::TimeT oVal;
		acpl::cp::_time_dt_to_timet(oVal, this->mDt, true);
		acpl::cp::_time_dt_from_timet(this->mDt, oVal, false);
	}
	
	acpl::pi::_time_dt_set_local(this->mDt, false);
	return *this;
}

bool acpl::DateTime::IsLocal() const
{
	return acpl::pi::_time_dt_is_local(this->mDt);
}

acpl::DateTime &acpl::DateTime::ToLocal()
{
	if (acpl::pi::_time_dt_is_clear(this->mDt) != true &&
		acpl::pi::_time_dt_is_local(this->mDt) == false)
	{
		acpl::TimeT oVal;
		acpl::cp::_time_dt_to_timet(oVal, this->mDt, false);
		acpl::cp::_time_dt_from_timet(this->mDt, oVal, true);
	}
	
	acpl::pi::_time_dt_set_local(this->mDt, true);
	return *this;
}


acpl::DateTime &acpl::DateTime::SetDate(acpl::SInt32 nYear, acpl::UInt8 nMonth, acpl::UInt8 nDay)
{
	acpl::pi::_time_dt_set_year(this->mDt, nYear);
	acpl::pi::_time_dt_set_month(this->mDt, nMonth);
	acpl::pi::_time_dt_set_day(this->mDt, nDay);
	return *this;
}

acpl::SInt32 acpl::DateTime::GetYear() const
{
	return acpl::pi::_time_dt_get_year(this->mDt);
}

acpl::DateTime &acpl::DateTime::SetYear(acpl::SInt32 nYear)
{
	acpl::pi::_time_dt_set_year(this->mDt, nYear);
	return *this;
}

acpl::UInt8 acpl::DateTime::GetMonth() const
{
	return acpl::pi::_time_dt_get_month(this->mDt);
}

acpl::DateTime &acpl::DateTime::SetMonth(acpl::UInt8 nMonth)
{
	acpl::pi::_time_dt_set_month(this->mDt, nMonth);
	return *this;
}

acpl::UInt16 acpl::DateTime::GetDayOfYear() const
{
	return acpl::pi::_time_dt_get_day_of_year(this->mDt);
}

acpl::UInt8 acpl::DateTime::GetDayOfWeek() const
{
	return acpl::pi::_time_dt_get_day_of_week(this->mDt);
}

acpl::UInt8 acpl::DateTime::GetDay() const
{
	return acpl::pi::_time_dt_get_day(this->mDt);
}

acpl::DateTime &acpl::DateTime::SetDay(acpl::UInt8 nDay)
{
	acpl::pi::_time_dt_set_day(this->mDt, nDay);
	return *this;
}

bool acpl::DateTime::IsDst() const
{
	return acpl::pi::_time_dt_get_is_dst(this->mDt);
}

acpl::DateTime &acpl::DateTime::SetTime(acpl::UInt8 nHour, acpl::UInt8 nMin, acpl::UInt8 nSec)
{
	acpl::pi::_time_dt_set_hour(this->mDt, nHour);
	acpl::pi::_time_dt_set_min(this->mDt, nMin);
	acpl::pi::_time_dt_set_sec(this->mDt, nSec);
	return *this;
}

acpl::UInt8 acpl::DateTime::GetHour() const
{
	return acpl::pi::_time_dt_get_hour(this->mDt);
}

acpl::DateTime &acpl::DateTime::SetHour(acpl::UInt8 nHour)
{
	acpl::pi::_time_dt_set_hour(this->mDt, nHour);
	return *this;
}

acpl::UInt8 acpl::DateTime::GetMin() const
{
	return acpl::pi::_time_dt_get_min(this->mDt);
}

acpl::DateTime &acpl::DateTime::SetMin(acpl::UInt8 nMin)
{
	acpl::pi::_time_dt_set_min(this->mDt, nMin);
	return *this;
}

acpl::UInt8 acpl::DateTime::GetSec() const
{
	return acpl::pi::_time_dt_get_sec(this->mDt);
}

acpl::DateTime &acpl::DateTime::SetSec(acpl::UInt8 nSec)
{
	acpl::pi::_time_dt_set_sec(this->mDt, nSec);
	return *this;
}


// 
// Timer
// 

acpl::Timer::Timer()
{
	this->mTimeoutAbs = 0;
}

acpl::Timer::~Timer()
{
	this->mTimeoutAbs = 0;
}

void acpl::Timer::SetupSec(acpl::UInt32 nSecs)
{
	this->mTimeoutAbs = (acpl::Time::Monotone() + (static_cast<acpl::UInt64>(nSecs) * static_cast<acpl::UInt64>(1000000)));
}

void acpl::Timer::SetupMsec(acpl::UInt32 nMsecs)
{
	this->mTimeoutAbs = (acpl::Time::Monotone() + (static_cast<acpl::UInt64>(nMsecs) * static_cast<acpl::UInt64>(1000)));
}

void acpl::Timer::SetupUsec(acpl::UInt64 nUsecs)
{
	this->mTimeoutAbs = (acpl::Time::Monotone() + nUsecs);
}

bool acpl::Timer::IsElapsed() const
{
	return (acpl::Time::Monotone() > this->mTimeoutAbs);
}
