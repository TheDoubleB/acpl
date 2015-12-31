#ifndef ACPL_CP_MSCRT_PARALLEL_INC_CPP
#define ACPL_CP_MSCRT_PARALLEL_INC_CPP

#include <time.h>
#include <Windows.h>

#include "../../Thread.h"

#include "../Parallel.inc.cpp"

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
			return (static_cast<acpl::UInt64>(timeGetTime()) * static_cast<acpl::UInt64>(1000));
		}

		static inline void _time_microsleep(acpl::UInt64 nMicroSeconds)
		{
			::Sleep(static_cast<DWORD>(nMicroSeconds / static_cast<acpl::UInt64>(1000)));
		}
		
		static inline void _time_dt_from_timet(acpl::UInt64 &nDt, acpl::TimeT nTime, bool nIsLocal)
		{
			struct tm oTm;
			errno_t (*oConvFunc)(struct tm *, const time_t *) = ((nIsLocal == true) ? localtime_s : gmtime_s);
			
			if (oConvFunc(&oTm, &nTime) == 0)
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
			time_t (*oConvFunc)(struct tm *) = ((nIsLocal == true) ? mktime : _mkgmtime);
			
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
			time_t (*oConvFunc)(struct tm *) = ((nIsLocal == true) ? mktime : _mkgmtime);
			
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
		
		static inline bool _mutex_create(acpl::Mutex::Handle &nMutex)
		{
			return ((nMutex.uPtr = CreateMutex(NULL, false, NULL)) != NULL);
		}
		
		static inline bool _mutex_destroy(acpl::Mutex::Handle &nMutex)
		{
			if (nMutex.uPtr != NULL)
			{
				CloseHandle(nMutex.uPtr);
				nMutex.uPtr = NULL;
				return true;
			}
			else
				return false;
		}
		
		static inline bool _mutex_lock(acpl::Mutex::Handle &nMutex)
		{
			return (nMutex.uPtr != NULL && WaitForSingleObject(nMutex.uPtr, INFINITE) == WAIT_OBJECT_0);
		}
		
		static inline bool _mutex_trylock(acpl::Mutex::Handle &nMutex)
		{
			return (nMutex.uPtr != NULL && WaitForSingleObject(nMutex.uPtr, 0) == WAIT_OBJECT_0);
		}
		
		static inline bool _mutex_timedlock(acpl::Mutex::Handle &nMutex, acpl::UInt32 nMsecs)
		{
			return (nMutex.uPtr != NULL && WaitForSingleObject(nMutex.uPtr, nMsecs) == WAIT_OBJECT_0);
		}
		
		static inline bool _mutex_unlock(acpl::Mutex::Handle &nMutex)
		{
			return (nMutex.uPtr != NULL && ReleaseMutex(nMutex.uPtr) != 0);
		}
		
		
		// 
		// Condition functions
		// 
		
		static inline void _local_condition_cleanup(acpl::Condition::Handle &nCond)
		{
			if (nCond.uStruct.sEventSignal != NULL)
				CloseHandle(nCond.uStruct.sEventSignal);
			
			acpl::cp::_mutex_destroy(nCond.uStruct.sMutexWaiters);
			
			memset(&nCond.uStruct, 0, sizeof(acpl::Condition::Handle));
		}
		
		static inline bool _local_condition_wait_raw(acpl::Condition::Handle &nCond, acpl::Mutex::Handle &nMutex, DWORD nMsecs, bool &nWasTimeout)
		{
			bool oRet = false;
			nWasTimeout = false;
			
			// Setup
			acpl::cp::_mutex_lock(nCond.uStruct.sMutexWaiters);
			nCond.uStruct.sNumWaiters++;
			acpl::cp::_mutex_unlock(nCond.uStruct.sMutexWaiters);
			
			// Wait
			HANDLE oEventList[1] = { nCond.uStruct.sEventSignal };
			
			acpl::cp::_mutex_unlock(nMutex);
			DWORD oWaitRet = WaitForMultipleObjects(1, oEventList, FALSE, nMsecs);
			acpl::cp::_mutex_lock(nMutex);
			
			if (oWaitRet == WAIT_OBJECT_0)
				oRet = true;
			else
			if (oWaitRet == WAIT_TIMEOUT)
				nWasTimeout = true;
			
			// Cleanup
			acpl::cp::_mutex_lock(nCond.uStruct.sMutexWaiters);
			
			nCond.uStruct.sNumWaiters--;
			
			if (oWaitRet == WAIT_OBJECT_0 && nCond.uStruct.sNumToResume > 0)
				nCond.uStruct.sNumToResume--;
			
			if (nCond.uStruct.sNumToResume > 0)
				SetEvent(nCond.uStruct.sEventSignal);
			else
				ResetEvent(nCond.uStruct.sEventSignal);
			
			acpl::cp::_mutex_unlock(nCond.uStruct.sMutexWaiters);
			
			return oRet;
		}
		
		static inline bool _condition_create(acpl::Condition::Handle &nCond)
		{
			memset(&nCond.uStruct, 0, sizeof(acpl::Condition::Handle));
			
			if (acpl::cp::_mutex_create(nCond.uStruct.sMutexWaiters) == false)
			{
				acpl::cp::_local_condition_cleanup(nCond);
				return false;
			}
			
			nCond.uStruct.sNumWaiters = 0;
			nCond.uStruct.sNumToResume = 0;
			
			nCond.uStruct.sEventSignal = CreateEvent(NULL, FALSE, FALSE, NULL);
			if (nCond.uStruct.sEventSignal == NULL)
			{
				acpl::cp::_local_condition_cleanup(nCond);
				return false;
			}
			
			return true;
		}
		
		static inline bool _condition_destroy(acpl::Condition::Handle &nCond)
		{
			acpl::cp::_local_condition_cleanup(nCond);
			return true;
		}
		
		static inline bool _condition_wait(acpl::Condition::Handle &nCond, acpl::Mutex::Handle &nMutex)
		{
			bool oWasTimeout;
			return acpl::cp::_local_condition_wait_raw(nCond, nMutex, INFINITE, oWasTimeout);
		}
		
		static inline bool _condition_timedwait(acpl::Condition::Handle &nCond, acpl::Mutex::Handle &nMutex, acpl::UInt32 nMsecs, bool &nWasTimeout)
		{
			return acpl::cp::_local_condition_wait_raw(nCond, nMutex, nMsecs, nWasTimeout);
		}
		
		static inline bool _condition_signal(acpl::Condition::Handle &nCond)
		{
			acpl::cp::_mutex_lock(nCond.uStruct.sMutexWaiters);
			
			if (nCond.uStruct.sNumToResume < nCond.uStruct.sNumWaiters)
			{
				nCond.uStruct.sNumToResume++;
				SetEvent(nCond.uStruct.sEventSignal);
			}
			
			acpl::cp::_mutex_unlock(nCond.uStruct.sMutexWaiters);
			
			return true;
		}
		
		static inline bool _condition_broadcast(acpl::Condition::Handle &nCond)
		{
			acpl::cp::_mutex_lock(nCond.uStruct.sMutexWaiters);
			
			if (nCond.uStruct.sNumToResume < nCond.uStruct.sNumWaiters)
			{
				nCond.uStruct.sNumToResume = nCond.uStruct.sNumWaiters;
				SetEvent(nCond.uStruct.sEventSignal);
			}
			
			acpl::cp::_mutex_unlock(nCond.uStruct.sMutexWaiters);
			
			return true;
		}
		
		
		// 
		// Thread functions
		// 
		
		static inline DWORD WINAPI _local_thread_create_func(LPVOID nParam)
		{
			static_cast<acpl::Thread::StartStruct *>(nParam)->sFunc(static_cast<acpl::Thread::StartStruct *>(nParam)->sParam);
			return 0;
		}
		
		static inline bool _thread_create(acpl::Thread::Handle &nThread, acpl::Thread::StartStruct &nThreadStartStruct)
		{
			return ((nThread.uPtr = CreateThread(NULL, 0, acpl::cp::_local_thread_create_func, &nThreadStartStruct, 0, NULL)) != NULL);
		}
		
		static inline bool _thread_join(acpl::Thread::Handle &nThread)
		{
			if (nThread.uPtr != NULL)
			{
				WaitForSingleObject(nThread.uPtr, INFINITE);
				CloseHandle(nThread.uPtr);
				nThread.uPtr = NULL;
				return true;
			}
			else
				return false;
		}
		
		static inline bool _thread_detach(acpl::Thread::Handle &nThread)
		{
			if (nThread.uPtr != NULL)
			{
				CloseHandle(nThread.uPtr);
				nThread.uPtr = NULL;
				return true;
			}
			else
				return false;
		}
		
	}
}

#endif // ACPL_CP_MSCRT_PARALLEL_INC_CPP
