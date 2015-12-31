#ifndef ACPL_THREAD_H
#define ACPL_THREAD_H

#include "Macros.h"
#include "Types.h"

namespace acpl
{
	
	class Mutex
	{
			acplHideClassCopy(Mutex);
		
		public:
			typedef union
			{
				acpl::UInt8 uBytes[40];
				void *uPtr;
			} Handle;
		
		private:
			friend class Condition;
			acpl::Mutex::Handle mHandle;
		
		protected:
			
		
		public:
			Mutex();
			virtual ~Mutex();
			
			bool Lock();
			bool TryLock();
			bool TimedLock(acpl::UInt32 nMsecs);
			bool Unlock();
		
	};
	
	class RaiiMutex
	{
			acplHideClassCopyIa(RaiiMutex, nRaiiMutex, mRef(nRaiiMutex.mRef));
		
		private:
			bool mEnabled;
			bool mLocked;
			acpl::Mutex &mRef;
		
		protected:
			
		
		public:
			RaiiMutex(acpl::Mutex &nRef, bool nAutoLock = true);
			~RaiiMutex();
			
			void Enable();
			void Disable();
			
			void Lock();
			void Unlock();
		
	};
	
	class Condition
	{
			acplHideClassCopy(Condition);
		
		public:
			typedef union
			{
				acpl::UInt8 uBytes[48];
				struct
				{
					acpl::Mutex::Handle sMutexWaiters;
					acpl::SizeT sNumWaiters;
					acpl::SizeT sNumToResume;
					void *sEventSignal;
				} uStruct;
			} Handle;
		
		private:
			acpl::Condition::Handle mHandle;
		
		public:
			Condition();
			virtual ~Condition();
			
			bool Signal();
			bool Broadcast();
			
			bool Wait(acpl::Mutex &nMutex);
			bool TimedWait(acpl::Mutex &nMutex, acpl::UInt32 nMsecs);
			bool TimedWait(acpl::Mutex &nMutex, acpl::UInt32 nMsecs, bool &nWasTimeout);
		
	};
	
	class Thread
	{
		
		acplHideClassCopy(Thread);
		
		public:
			typedef union
			{
				acpl::SizeT uUli;
				void *uPtr;
			} Handle;
			struct StartStruct
			{
				void (*sFunc)(void *);
				void *sParam;
			};
		
		private:
			acpl::UInt8 mStatus;
			mutable acpl::Mutex mStatusMutex;
			acpl::Thread::Handle mHandle;
			acpl::Thread::StartStruct mThreadStartStruct;
			
			static void StartThread(void *nThread);
			void Finish(bool nClean);
		
		protected:
			bool IsStopSig() const;
			
			virtual void Main() = 0;
			virtual bool Init();
			virtual void Terminate();
			virtual void Finalize();
		
		public:
			Thread();
			virtual ~Thread();
			
			bool IsRunning() const;
			bool Start();
			void Stop();
			void StopAsync();
		
	};
	
}

#endif // ACPL_THREAD_H
