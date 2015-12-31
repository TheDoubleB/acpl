#include "Tests.h"

#include "../src/Thread.h"
#include "../src/Time.h"


#if   (acplCRuntime == acplCRuntimeGlibc)
#	include <pthread.h>
#endif


class _local_ThreadInfinite : public acpl::Thread
{
	private:
		acpl::UInt8 mSleepIntMsec;
		
		virtual void Main()
		{
			while (this->IsStopSig() != true)
			{
				PrintOut("Thread:Infinite:Iteration\n");
				acpl::Time::SleepMsec(this->mSleepIntMsec);
			}
		}
		
		virtual bool Init()      { PrintOut("Thread:Infinite:Init\n"); return true; }
		virtual void Terminate() { PrintOut("Thread:Infinite:Terminate\n"); }
		virtual void Finalize()  { PrintOut("Thread:Infinite:Finalize\n"); }
	
	public:
		_local_ThreadInfinite(acpl::UInt8 nSleepIntMsec) : mSleepIntMsec(nSleepIntMsec) {  }
		virtual ~_local_ThreadInfinite() {  }
};

class _local_ThreadFinite : public acpl::Thread
{
	private:
		acpl::UInt8 mSleepIntMsec;
		acpl::UInt8 mIterNum;
		
		virtual void Main()
		{
			for (acpl::UInt8 i = 0; i < this->mIterNum; i++)
			{
				PrintOut("Thread:Finite:Iteration (%u)\n", i);
				acpl::Time::SleepMsec(this->mSleepIntMsec);
			}
		}
		
		virtual bool Init()      { PrintOut("Thread:Finite:Init\n"); return true; }
		virtual void Terminate() { PrintOut("Thread:Finite:Terminate\n"); }
		virtual void Finalize()  { PrintOut("Thread:Finite:Finalize\n"); }
	
	public:
		_local_ThreadFinite(acpl::UInt8 nSleepIntMsec, acpl::UInt8 nIterNum) : mSleepIntMsec(nSleepIntMsec), mIterNum(nIterNum) {  }
		virtual ~_local_ThreadFinite() {  }
};

class _local_ThreadWithMutex : public acpl::Thread
{
	private:
		acpl::UInt8 mSleepIntMsec;
		acpl::Mutex mMutex;
		
		virtual void Main()
		{
			while (this->IsStopSig() != true)
			{
				PrintOut("Thread:WithMutex:Iteration (Lock)\n");
				this->mMutex.Lock();
				PrintOut("Thread:WithMutex:Iteration (Unlock)\n");
				this->mMutex.Unlock();
				PrintOut("Thread:WithMutex:Iteration (Sleep)\n");
				acpl::Time::SleepMsec(this->mSleepIntMsec);
			}
		}
		
		virtual bool Init()      { PrintOut("Thread:WithMutex:Init\n"); return true; }
		virtual void Terminate() { PrintOut("Thread:WithMutex:Terminate\n"); }
		virtual void Finalize()  { PrintOut("Thread:WithMutex:Finalize\n"); }
	
	public:
		_local_ThreadWithMutex(acpl::UInt8 nSleepIntMsec) : mSleepIntMsec(nSleepIntMsec) {  }
		virtual ~_local_ThreadWithMutex() {  }
		
		acpl::Mutex &GetMutex() { return this->mMutex; }
};

class _local_ThreadConditionWait : public acpl::Thread
{
	private:
		acpl::UInt8 mWaitMsec;
		bool mWasWaitOk;
		bool mWasTimeout;
		acpl::Condition &mCond;
		acpl::Mutex &mMutex;
		
		virtual void Main()
		{
			PrintOut("Thread:ConditionWait:Iteration (Lock)\n");
			this->mMutex.Lock();
			
			this->mWasWaitOk = false;
			this->mWasTimeout = false;
			
			if (this->mWaitMsec == 0)
			{
				PrintOut("Thread:ConditionWait:Iteration (Wait)\n");
				this->mWasWaitOk = this->mCond.Wait(this->mMutex);
			}
			else
			{
				PrintOut("Thread:ConditionWait:Iteration (TimedWait)\n");
				this->mWasWaitOk = this->mCond.TimedWait(this->mMutex, this->mWaitMsec, this->mWasTimeout);
			}
			
			PrintOut("Thread:ConditionWait:Iteration (Unlock)\n");
			this->mMutex.Unlock();
		}
		
		virtual bool Init()      { PrintOut("Thread:ConditionWait:Init\n"); return true; }
		virtual void Terminate() { PrintOut("Thread:ConditionWait:Terminate\n"); }
		virtual void Finalize()  { PrintOut("Thread:ConditionWait:Finalize\n"); }
	
	public:
		_local_ThreadConditionWait(acpl::Condition &nCond, acpl::Mutex &nMutex) : mCond(nCond), mMutex(nMutex) { this->mWaitMsec = 0; this->mWasWaitOk = false; this->mWasTimeout = false; }
		virtual ~_local_ThreadConditionWait() {  }
		
		void SetWaitMsec(acpl::UInt8 nMsec) { this->mWaitMsec = nMsec; }
		bool WasWaitOk() const { return this->mWasWaitOk; }
		bool WasTimeout() const { return this->mWasTimeout; }
};


static int TestParallelTypes()
{
	PrintFn();
	
	
#if   (acplCRuntime == acplCRuntimeGlibc)
	
	// NOTE: Test needed only for glibc pthread types
	Test(sizeof(acpl::Mutex::Handle::uBytes) >= sizeof(pthread_mutex_t));
	Test(sizeof(acpl::Condition::Handle::uBytes) >= sizeof(pthread_cond_t));
	Test(sizeof(acpl::Thread::Handle::uUli) == sizeof(pthread_t));
	
#endif
	
	
	return 0;
}

static int TestMutexPlain()
{
	PrintFn();
	
	
	acpl::Mutex oMutex;
	
	// acpl::Mutex uses recursive locking mechanism, so the same thread can lock
	// the mutex multiple times (but has to unlock it the same amount of times)
	Test(oMutex.Lock() == true);
	Test(oMutex.TryLock() == true);
	Test(oMutex.TimedLock(10) == true);
	Test(oMutex.Unlock() == true);
	Test(oMutex.Unlock() == true);
	Test(oMutex.Unlock() == true);
	
	
	return 0;
}

static int TestConditionPlain()
{
	PrintFn();
	
	
	acpl::Condition oCond;
	acpl::Mutex oMutex;
	bool oWasTimeout = false;
	
	
	Test(oMutex.Lock() == true);
	Test(oCond.TimedWait(oMutex, 100) == false);
	Test(oCond.TimedWait(oMutex, 100, oWasTimeout) == false);
	Test(oMutex.Unlock() == true);
	Test(oWasTimeout == true);
	Test(oCond.Signal() == true);
	Test(oCond.Broadcast() == true);
	
	
	
	return 0;
}

static int TestThreadPlain()
{
	PrintFn();
	
	
	// Infinite: Start + Stop (synchronous)
	
	PrintOut(">>> Infinite: Start + Stop (synchronous)\n");
	_local_ThreadInfinite oThreadInf(100);
	Test(oThreadInf.IsRunning() == false);
	Test(oThreadInf.Start() == true);
	Test(oThreadInf.IsRunning() == true);
	acpl::Time::SleepMsec(300);
	Test(oThreadInf.IsRunning() == true);
	oThreadInf.Stop();
	acpl::Time::SleepMsec(50);
	Test(oThreadInf.IsRunning() == false);
	
	
	// Infinite: Start + Stop (asynchronous)
	
	PrintOut(">>> Infinite: Start + Stop (asynchronous)\n");
	Test(oThreadInf.Start() == true);
	Test(oThreadInf.IsRunning() == true);
	acpl::Time::SleepMsec(300);
	Test(oThreadInf.IsRunning() == true);
	oThreadInf.StopAsync();
	acpl::Time::SleepMsec(150);
	Test(oThreadInf.IsRunning() == false);
	
	
	// Finite: Start + (self-stop)
	
	PrintOut(">>> Finite: Start + (self-stop)\n");
	_local_ThreadFinite oThreadFin(100, 5);
	Test(oThreadFin.IsRunning() == false);
	Test(oThreadFin.Start() == true);
	Test(oThreadFin.IsRunning() == true);
	acpl::Time::SleepMsec(550);
	Test(oThreadFin.IsRunning() == false);
	
	
	// Finite: Start + (useless) Stop (synchronous)
	
	PrintOut(">>> Finite: Start + (useless) Stop (synchronous)\n");
	Test(oThreadFin.Start() == true);
	Test(oThreadFin.IsRunning() == true);
	acpl::Time::SleepMsec(50);
	oThreadFin.Stop(); // will not have any effect (will still block until finish) because the thread does not rely on `IsStopSig()` status and will finish as nothing happened
	Test(oThreadFin.IsRunning() == false);
	
	
	// Finite: Start + (useless) Stop (asynchronous)
	
	PrintOut(">>> Finite: Start + (useless) Stop (asynchronous)\n");
	Test(oThreadFin.Start() == true);
	Test(oThreadFin.IsRunning() == true);
	oThreadFin.StopAsync(); // will not have any effect (will not block until finish) because the thread does not rely on `IsStopSig()` status and will finish as nothing happened
	acpl::Time::SleepMsec(550);
	Test(oThreadFin.IsRunning() == false);
	
	
	return 0;
}

static int TestThreadWithMutex()
{
	PrintFn();
	
	
	_local_ThreadWithMutex oThread(100);
	
	
	// Start + Stop (synchronous)
	
	PrintOut(">>> Start + Stop (synchronous)\n");
	Test(oThread.Start() == true)
	Test(oThread.IsRunning() == true);
	Test(oThread.GetMutex().Lock() == true);
	Test(oThread.IsRunning() == true);
	acpl::Time::SleepMsec(100);
	Test(oThread.IsRunning() == true);
	Test(oThread.GetMutex().Unlock() == true);
	Test(oThread.IsRunning() == true);
	acpl::Time::SleepMsec(200);
	oThread.Stop();
	Test(oThread.IsRunning() == false);
	
	
	// Start + Stop (asynchronous)
	
	PrintOut(">>> Start + Stop (asynchronous)\n");
	Test(oThread.Start() == true)
	Test(oThread.IsRunning() == true);
	acpl::Time::SleepMsec(50);
	Test(oThread.GetMutex().Lock() == true);
	Test(oThread.IsRunning() == true);
	oThread.StopAsync();
	Test(oThread.IsRunning() == true);
	Test(oThread.GetMutex().Unlock() == true);
	acpl::Time::SleepMsec(150);
	Test(oThread.IsRunning() == false);
	
	
	return 0;
}

static int TestThreadConditionWait()
{
	PrintFn();
	
	
	acpl::Condition oCond;
	acpl::Mutex oMutex;
	_local_ThreadConditionWait oThread(oCond, oMutex);
	_local_ThreadConditionWait oThread2(oCond, oMutex);
	_local_ThreadConditionWait oThread3(oCond, oMutex);
	
	
	// Single: Wait + Signal
	
	Test(oThread.Start() == true);
	acpl::Time::SleepMsec(100);
	Test(oCond.Signal() == true);
	acpl::Time::SleepMsec(50);
	Test(oThread.WasWaitOk() == true);
	Test(oThread.IsRunning() == false);
	
	
	// Single: Wait + Broadcast
	
	Test(oThread.Start() == true);
	acpl::Time::SleepMsec(100);
	Test(oCond.Broadcast() == true);
	acpl::Time::SleepMsec(50);
	Test(oThread.WasWaitOk() == true);
	Test(oThread.IsRunning() == false);
	
	
	// Set wait interval (engages TimedWait mode)
	oThread.SetWaitMsec(100);
	
	
	// Single: TimedWait + Signal
	
	Test(oThread.Start() == true);
	acpl::Time::SleepMsec(50);
	Test(oCond.Signal() == true);
	acpl::Time::SleepMsec(50);
	Test(oThread.WasWaitOk() == true);
	Test(oThread.IsRunning() == false);
	
	
	// Single: TimedWait + Broadcast
	
	Test(oThread.Start() == true);
	acpl::Time::SleepMsec(50);
	Test(oCond.Broadcast() == true);
	acpl::Time::SleepMsec(50);
	Test(oThread.WasWaitOk() == true);
	Test(oThread.IsRunning() == false);
	
	
	// Single: TimedWait (timeout) + Signal
	
	Test(oThread.Start() == true);
	acpl::Time::SleepMsec(150);
	Test(oCond.Signal() == true);
	acpl::Time::SleepMsec(50);
	Test(oThread.WasWaitOk() == false);
	Test(oThread.WasTimeout() == true);
	Test(oThread.IsRunning() == false);
	
	
	// Single: TimedWait (timeout) + Broadcast
	
	Test(oThread.Start() == true);
	acpl::Time::SleepMsec(150);
	Test(oCond.Broadcast() == true);
	acpl::Time::SleepMsec(50);
	Test(oThread.WasWaitOk() == false);
	Test(oThread.WasTimeout() == true);
	Test(oThread.IsRunning() == false);
	
	
	// Reset wait interval (engages Wait mode)
	oThread.SetWaitMsec(0);
	
	
	// Multi: Wait + Signal (multiple)
	
	Test(oThread.Start() == true);
	Test(oThread2.Start() == true);
	Test(oThread3.Start() == true);
	acpl::Time::SleepMsec(100);
	Test(oCond.Signal() == true);
	Test(oCond.Signal() == true);
	Test(oCond.Signal() == true);
	acpl::Time::SleepMsec(50);
	Test(oThread.WasWaitOk() == true);
	Test(oThread.IsRunning() == false);
	Test(oThread2.WasWaitOk() == true);
	Test(oThread2.IsRunning() == false);
	Test(oThread3.WasWaitOk() == true);
	Test(oThread3.IsRunning() == false);
	
	
	// Multi: Wait + Broadcast
	
	Test(oThread.Start() == true);
	Test(oThread2.Start() == true);
	Test(oThread3.Start() == true);
	acpl::Time::SleepMsec(100);
	Test(oCond.Broadcast() == true);
	acpl::Time::SleepMsec(50);
	Test(oThread.WasWaitOk() == true);
	Test(oThread.IsRunning() == false);
	Test(oThread2.WasWaitOk() == true);
	Test(oThread2.IsRunning() == false);
	Test(oThread3.WasWaitOk() == true);
	Test(oThread3.IsRunning() == false);
	
	
	// Set wait interval (engages TimedWait mode)
	oThread.SetWaitMsec(100);
	oThread2.SetWaitMsec(100);
	oThread3.SetWaitMsec(100);
	
	
	// Multi: TimedWait + Signal (multiple)
	
	Test(oThread.Start() == true);
	Test(oThread2.Start() == true);
	Test(oThread3.Start() == true);
	acpl::Time::SleepMsec(50);
	Test(oCond.Signal() == true);
	Test(oCond.Signal() == true);
	Test(oCond.Signal() == true);
	acpl::Time::SleepMsec(50);
	Test(oThread.WasWaitOk() == true);
	Test(oThread.IsRunning() == false);
	Test(oThread2.WasWaitOk() == true);
	Test(oThread2.IsRunning() == false);
	Test(oThread3.WasWaitOk() == true);
	Test(oThread3.IsRunning() == false);
	
	
	// Multi: TimedWait + Broadcast
	
	Test(oThread.Start() == true);
	Test(oThread2.Start() == true);
	Test(oThread3.Start() == true);
	acpl::Time::SleepMsec(50);
	Test(oCond.Broadcast() == true);
	acpl::Time::SleepMsec(50);
	Test(oThread.WasWaitOk() == true);
	Test(oThread.IsRunning() == false);
	Test(oThread2.WasWaitOk() == true);
	Test(oThread2.IsRunning() == false);
	Test(oThread3.WasWaitOk() == true);
	Test(oThread3.IsRunning() == false);
	
	
	// Multi: TimedWait (timeout) + Signal (multiple)
	
	Test(oThread.Start() == true);
	Test(oThread2.Start() == true);
	Test(oThread3.Start() == true);
	acpl::Time::SleepMsec(150);
	Test(oCond.Signal() == true);
	Test(oCond.Signal() == true);
	Test(oCond.Signal() == true);
	acpl::Time::SleepMsec(50);
	Test(oThread.WasWaitOk() == false);
	Test(oThread.WasTimeout() == true);
	Test(oThread.IsRunning() == false);
	Test(oThread2.WasWaitOk() == false);
	Test(oThread2.WasTimeout() == true);
	Test(oThread2.IsRunning() == false);
	Test(oThread3.WasWaitOk() == false);
	Test(oThread3.WasTimeout() == true);
	Test(oThread3.IsRunning() == false);
	
	
	// Multi: TimedWait (timeout) + Broadcast
	
	Test(oThread.Start() == true);
	Test(oThread2.Start() == true);
	Test(oThread3.Start() == true);
	acpl::Time::SleepMsec(150);
	Test(oCond.Broadcast() == true);
	acpl::Time::SleepMsec(50);
	Test(oThread.WasWaitOk() == false);
	Test(oThread.WasTimeout() == true);
	Test(oThread.IsRunning() == false);
	Test(oThread2.WasWaitOk() == false);
	Test(oThread2.WasTimeout() == true);
	Test(oThread2.IsRunning() == false);
	Test(oThread3.WasWaitOk() == false);
	Test(oThread3.WasTimeout() == true);
	Test(oThread3.IsRunning() == false);
	
	
	return 0;
}


SectionFuncMain(threads)
{
	if (argc < 1)
	{
		Test(TestParallelTypes() == 0);
		Test(TestMutexPlain() == 0);
		Test(TestConditionPlain() == 0);
		Test(TestThreadPlain() == 0);
		Test(TestThreadWithMutex() == 0);
		Test(TestThreadConditionWait() == 0);
		
		PrintOut("All Passed!\n");
	}
	else
		return PrintUnrecParams(acplDbgSrcLoc, lSectionName, argc, argv);
	
	return 0;
}

SectionFuncParamsBegin(threads)
// SectionFuncParamsAdd("param [param [param ...]]", "description")
SectionFuncParamsEnd(threads)
