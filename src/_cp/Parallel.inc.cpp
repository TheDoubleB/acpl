#ifndef ACPL_CP_PARALLEL_INC_CPP
#define ACPL_CP_PARALLEL_INC_CPP

#include "../Time.h"

namespace acpl
{
	// 
	// (Common) time functions
	// WARNING: Has to be before platform includes!
	// 
	
	namespace pi
	{
		static inline bool _time_dt_is_clear(acpl::UInt64 nDt)
		{
			return ((nDt & acpl::Const::UI64(0x7FFFFFFF, 0xFFFFFFFF)) == 0);
		}
		
		static inline void _time_dt_clear(acpl::UInt64 &nDt)
		{
			nDt &= acpl::Const::UI64(0x80000000, 0x00000000);
		}
		
		static inline bool _time_dt_is_local(acpl::UInt64 nDt)
		{
			return ((nDt >> 63) != 0);
		}
		
		static inline void _time_dt_set_local(acpl::UInt64 &nDt, bool nIsLocal)
		{
			if (nIsLocal == true)
				nDt |= acpl::Const::UI64(0x80000000, 0x00000000);
			else
				nDt &= acpl::Const::UI64(0x7FFFFFFF, 0xFFFFFFFF);
		}
		
		static inline acpl::UInt16 _time_dt_get_day_of_year(acpl::UInt64 nDt)
		{
			return ((nDt >> 54) & 0x1FF);
		}
		
		static inline void _time_dt_set_day_of_year(acpl::UInt64 &nDt, acpl::UInt16 nDayOfYear)
		{
			nDt = (nDt & acpl::Const::UI64(0x803FFFFF, 0xFFFFFFFF)) | (static_cast<acpl::UInt64>(nDayOfYear & 0x1FF) << 54);
		}
		
		static inline acpl::UInt8 _time_dt_get_day_of_week(acpl::UInt64 nDt)
		{
			return ((nDt >> 51) & 0x7);
		}
		
		static inline void _time_dt_set_day_of_week(acpl::UInt64 &nDt, acpl::UInt8 nDayOfWeek)
		{
			nDt = (nDt & acpl::Const::UI64(0xFFC7FFFF, 0xFFFFFFFF)) | (static_cast<acpl::UInt64>(nDayOfWeek & 0x7) << 51);
		}
		
		static inline acpl::SInt32 _time_dt_get_year(acpl::UInt64 nDt)
		{
			return (((nDt >> 28) & 0x7FFFFF) * (((nDt & 0x08000000) == 0) ? 1 : -1));
		}
		
		static inline void _time_dt_set_year(acpl::UInt64 &nDt, acpl::SInt32 nYear)
		{
			if (nYear >= 0)
				nDt = (nDt & acpl::Const::UI64(0xFFF80000, 0x07FFFFFF)) | ((static_cast<acpl::UInt64>(nYear) & 0x7FFFFF) << 28);
			else
				nDt = (nDt & acpl::Const::UI64(0xFFF80000, 0x07FFFFFF)) | ((static_cast<acpl::UInt64>(nYear * -1) & 0x7FFFFF) << 28) | 0x08000000;
		}
		
		static inline acpl::UInt8 _time_dt_get_month(acpl::UInt64 nDt)
		{
			return ((nDt >> 23) & 0xF);
		}
		
		static inline void _time_dt_set_month(acpl::UInt64 &nDt, acpl::UInt8 nMonth)
		{
			nDt = (nDt & acpl::Const::UI64(0xFFFFFFFF, 0xF87FFFFF)) | ((nMonth & 0xF) << 23);
		}
		
		static inline acpl::UInt8 _time_dt_get_day(acpl::UInt64 nDt)
		{
			return ((nDt >> 18) & 0x1F);
		}
		
		static inline void _time_dt_set_day(acpl::UInt64 &nDt, acpl::UInt8 nDay)
		{
			nDt = (nDt & acpl::Const::UI64(0xFFFFFFFF, 0xFF83FFFF)) | ((nDay & 0x1F) << 18);
		}
		
		static inline bool _time_dt_get_is_dst(acpl::UInt64 nDt)
		{
			return ((nDt & 0x00020000) == 0); // negated DST flag
		}
		
		static inline void _time_dt_set_is_dst(acpl::UInt64 &nDt, bool nIsDst)
		{
			if (nIsDst == true)
				nDt &= acpl::Const::UI64(0xFFFFFFFF, 0xFFFDFFFF);
			else
				nDt |= 0x00020000;
		}
		
		static inline acpl::UInt8 _time_dt_get_hour(acpl::UInt64 nDt)
		{
			return ((nDt >> 12) & 0x1F);
		}
		
		static inline void _time_dt_set_hour(acpl::UInt64 &nDt, acpl::UInt8 nHour)
		{
			nDt = (nDt & acpl::Const::UI64(0xFFFFFFFF, 0xFFFE0FFF)) | ((nHour & 0x1F) << 12);
		}
		
		static inline acpl::UInt8 _time_dt_get_min(acpl::UInt64 nDt)
		{
			return ((nDt >> 6) & 0x3F);
		}
		
		static inline void _time_dt_set_min(acpl::UInt64 &nDt, acpl::UInt8 nMin)
		{
			nDt = (nDt & acpl::Const::UI64(0xFFFFFFFF, 0xFFFFF03F)) | ((nMin & 0x3F) << 6);
		}
		
		static inline acpl::UInt8 _time_dt_get_sec(acpl::UInt64 nDt)
		{
			return (nDt & 0x3F);
		}
		
		static inline void _time_dt_set_sec(acpl::UInt64 &nDt, acpl::UInt8 nSec)
		{
			nDt = (nDt & acpl::Const::UI64(0xFFFFFFFF, 0xFFFFFFC0)) | (nSec & 0x3F);
		}
	}
}


#if   (acplCRuntime == acplCRuntimeGlibc)
#	include "glibc/Parallel.inc.cpp"
#elif (acplCRuntime == acplCRuntimeMscrt)
#	include "mscrt/Parallel.inc.cpp"
#else
#	error Inclusion of platform-specific `Parallel.inc.cpp` required
#endif


#endif // ACPL_CP_PARALLEL_INC_CPP
