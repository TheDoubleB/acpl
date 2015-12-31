#ifndef ACPL_CP_GLIBC_PARALLEL_INC_CPP
#define ACPL_CP_GLIBC_PARALLEL_INC_CPP

#include <time.h>
#include <pthread.h>
#include <errno.h>

#include "../../Thread.h"

#include "../Parallel.inc.cpp" // just for editing sake

namespace acpl
{
	namespace cp
	{
		
		// 
		// Time functions
		// 
		
		static inline acpl::TimeT _time_ts()
		{
			return time(NULL);
		}
		
		static inline acpl::UInt64 _time_monotone()
		{
			struct timespec oTimeSpec;
			clock_gettime(CLOCK_MONOTONIC, &oTimeSpec);
			
			return ((static_cast<acpl::UInt64>(oTimeSpec.tv_sec) * static_cast<acpl::UInt64>(1000000)) +
					(static_cast<acpl::UInt64>(oTimeSpec.tv_nsec) / static_cast<acpl::UInt64>(1000)));
		}
		
		static inline void _time_microsleep(acpl::UInt64 nMicroSeconds)
		{
			struct timespec oTimeSpec;
			oTimeSpec.tv_sec = (nMicroSeconds / static_cast<acpl::UInt64>(1000000));
			oTimeSpec.tv_nsec = static_cast<acpl::UInt64>(1000) * (nMicroSeconds % static_cast<acpl::UInt64>(1000000));
			
			nanosleep(&oTimeSpec, NULL);
		}
		
		static inline void _time_dt_from_timet(acpl::UInt64 &nDt, acpl::TimeT nTime, bool nIsLocal)
		{
			struct tm oTm;
			struct tm *(*oConvFunc)(const time_t *, struct tm *) = ((nIsLocal == true) ? localtime_r : gmtime_r);
			
			if (oConvFunc(&nTime, &oTm) != NULL)
			{
				acpl::pi::_time_dt_clear(nDt);
				acpl::pi::_time_dt_set_sec(nDt, oTm.tm_sec);
				acpl::pi::_time_dt_set_min(nDt, oTm.tm_min);
				acpl::pi::_time_dt_set_hour(nDt, oTm.tm_hour);
				acpl::pi::_time_dt_set_day(nDt, oTm.tm_mday);
				acpl::pi::_time_dt_set_month(nDt, oTm.tm_mon + 1);
				acpl::pi::_time_dt_set_year(nDt, oTm.tm_year + 1900);
				acpl::pi::_time_dt_set_day_of_week(nDt, (oTm.tm_wday != 0) ? oTm.tm_wday : 7);
				acpl::pi::_time_dt_set_day_of_year(nDt, oTm.tm_yday);
				acpl::pi::_time_dt_set_is_dst(nDt, oTm.tm_isdst > 0);
			}
		}

		static inline void _time_dt_to_timet(acpl::TimeT &nTime, acpl::UInt64 nDt, bool nIsLocal)
		{
			struct tm oTm;
			time_t (*oConvFunc)(struct tm *) = ((nIsLocal == true) ? mktime : timegm);
			
			oTm.tm_sec = acpl::pi::_time_dt_get_sec(nDt);
			oTm.tm_min = acpl::pi::_time_dt_get_min(nDt);
			oTm.tm_hour = acpl::pi::_time_dt_get_hour(nDt);
			oTm.tm_mday = acpl::pi::_time_dt_get_day(nDt);
			oTm.tm_mon = (acpl::pi::_time_dt_get_month(nDt) - 1);
			oTm.tm_year = (acpl::pi::_time_dt_get_year(nDt) - 1900);
			oTm.tm_isdst = ((acpl::pi::_time_dt_get_is_dst(nDt) == true) ? 1 : 0);
			oTm.tm_wday = 0;
			oTm.tm_yday = 0;
			
			nTime = oConvFunc(&oTm);
		}

		static inline void _time_dt_normalize(acpl::UInt64 &nDt, bool nIsLocal)
		{
			struct tm oTm;
			time_t (*oConvFunc)(struct tm *) = ((nIsLocal == true) ? mktime : timegm);
			
			oTm.tm_sec = acpl::pi::_time_dt_get_sec(nDt);
			oTm.tm_min = acpl::pi::_time_dt_get_min(nDt);
			oTm.tm_hour = acpl::pi::_time_dt_get_hour(nDt);
			oTm.tm_mday = acpl::pi::_time_dt_get_day(nDt);
			oTm.tm_mon = (acpl::pi::_time_dt_get_month(nDt) - 1);
			oTm.tm_year = (acpl::pi::_time_dt_get_year(nDt) - 1900);
			oTm.tm_isdst = -1;
			oTm.tm_wday = 0;
			oTm.tm_yday = 0;
			
			if (oConvFunc(&oTm) != static_cast<acpl::TimeT>(-1))
			{
				acpl::pi::_time_dt_clear(nDt);
				acpl::pi::_time_dt_set_sec(nDt, oTm.tm_sec);
				acpl::pi::_time_dt_set_min(nDt, oTm.tm_min);
				acpl::pi::_time_dt_set_hour(nDt, oTm.tm_hour);
				acpl::pi::_time_dt_set_day(nDt, oTm.tm_mday);
				acpl::pi::_time_dt_set_month(nDt, oTm.tm_mon + 1);
				acpl::pi::_time_dt_set_year(nDt, oTm.tm_year + 1900);
				acpl::pi::_time_dt_set_day_of_week(nDt, (oTm.tm_wday != 0) ? oTm.tm_wday : 7);
				acpl::pi::_time_dt_set_day_of_year(nDt, oTm.tm_yday);
				acpl::pi::_time_dt_set_is_dst(nDt, oTm.tm_isdst > 0);
			}
		}
		
		
		// 
		// Mutex functions
		// 
		
		static inline pthread_mutex_t *_local_mutex_t_get(acpl::Mutex::Handle &nMutex)
		{
			return reinterpret_cast<pthread_mutex_t *>(nMutex.uBytes);
		}
		
		static inline bool _mutex_create(acpl::Mutex::Handle &nMutex)
		{
			pthread_mutexattr_t oMutexAttr;
			return (pthread_mutexattr_init(&oMutexAttr) == 0 &&
					pthread_mutexattr_settype(&oMutexAttr, PTHREAD_MUTEX_RECURSIVE) == 0 &&
					pthread_mutex_init(acpl::cp::_local_mutex_t_get(nMutex), &oMutexAttr) == 0
				);
		}
		
		static inline bool _mutex_destroy(acpl::Mutex::Handle &nMutex)
		{
			return (pthread_mutex_destroy(acpl::cp::_local_mutex_t_get(nMutex)) == 0);
		}
		
		static inline bool _mutex_lock(acpl::Mutex::Handle &nMutex)
		{
			return (pthread_mutex_lock(acpl::cp::_local_mutex_t_get(nMutex)) == 0);
		}
		
		static inline bool _mutex_trylock(acpl::Mutex::Handle &nMutex)
		{
			return (pthread_mutex_trylock(acpl::cp::_local_mutex_t_get(nMutex)) == 0);
		}
		
		static inline bool _mutex_timedlock(acpl::Mutex::Handle &nMutex, acpl::UInt32 nMsecs)
		{
			struct timespec oTimer;
			clock_gettime(CLOCK_REALTIME, &oTimer);
			oTimer.tv_sec  +=  (nMsecs / 1000);
			oTimer.tv_nsec += ((nMsecs % 1000) * 1000000);
			if (oTimer.tv_nsec > 1000000000)
			{
				oTimer.tv_sec++;
				oTimer.tv_nsec -= 1000000000;
			}
			
			return (pthread_mutex_timedlock(acpl::cp::_local_mutex_t_get(nMutex), &oTimer) == 0);
		}
		
		static inline bool _mutex_unlock(acpl::Mutex::Handle &nMutex)
		{
			return (pthread_mutex_unlock(acpl::cp::_local_mutex_t_get(nMutex)) == 0);
		}
		
		
		// 
		// Condition functions
		// 
		
		static inline pthread_cond_t *_local_condition_t_get(acpl::Condition::Handle &nCond)
		{
			return reinterpret_cast<pthread_cond_t *>(nCond.uBytes);
		}
		
		static inline bool _condition_create(acpl::Condition::Handle &nCond)
		{
			return (pthread_cond_init(acpl::cp::_local_condition_t_get(nCond), NULL) == 0);
		}

		static inline bool _condition_destroy(acpl::Condition::Handle &nCond)
		{
			return (pthread_cond_destroy(acpl::cp::_local_condition_t_get(nCond)) == 0);
		}

		static inline bool _condition_wait(acpl::Condition::Handle &nCond, acpl::Mutex::Handle &nMutex)
		{
			return (pthread_cond_wait(acpl::cp::_local_condition_t_get(nCond), acpl::cp::_local_mutex_t_get(nMutex)) == 0);
		}

		static inline bool _condition_timedwait(acpl::Condition::Handle &nCond, acpl::Mutex::Handle &nMutex, acpl::UInt32 nMsecs, bool &nWasTimeout)
		{
			struct timespec oTimer;
			clock_gettime(CLOCK_REALTIME, &oTimer);
			oTimer.tv_sec  +=  (nMsecs / 1000);
			oTimer.tv_nsec += ((nMsecs % 1000) * 1000000);
			if (oTimer.tv_nsec > 1000000000)
			{
				oTimer.tv_sec++;
				oTimer.tv_nsec -= 1000000000;
			}
			
			int oRetVal = pthread_cond_timedwait(acpl::cp::_local_condition_t_get(nCond), acpl::cp::_local_mutex_t_get(nMutex), &oTimer);
			if (oRetVal == 0)
			{
				nWasTimeout = false;
				return true;
			}
			else
			{
				nWasTimeout = (oRetVal == ETIMEDOUT);
				return false;
			}
		}

		static inline bool _condition_signal(acpl::Condition::Handle &nCond)
		{
			return (pthread_cond_signal(acpl::cp::_local_condition_t_get(nCond)) == 0);
		}

		static inline bool _condition_broadcast(acpl::Condition::Handle &nCond)
		{
			return (pthread_cond_broadcast(acpl::cp::_local_condition_t_get(nCond)) == 0);
		}
		
		
		// 
		// Thread functions
		// 
		
		static inline void *_local_thread_create_func(void *nParam)
		{
			static_cast<acpl::Thread::StartStruct *>(nParam)->sFunc(static_cast<acpl::Thread::StartStruct *>(nParam)->sParam);
			return NULL;
		}
		
		static inline bool _thread_create(acpl::Thread::Handle &nThread, acpl::Thread::StartStruct &nThreadStartStruct)
		{
			return (pthread_create(&nThread.uUli, NULL, acpl::cp::_local_thread_create_func, &nThreadStartStruct) == 0);
		}
		
		static inline bool _thread_join(acpl::Thread::Handle &nThread)
		{
			return (pthread_join(nThread.uUli, NULL) == 0);
		}
		
		static inline bool _thread_detach(acpl::Thread::Handle &nThread)
		{
			return (pthread_detach(nThread.uUli) == 0);
		}
		
	}
}

#endif // ACPL_CP_GLIBC_PARALLEL_INC_CPP
