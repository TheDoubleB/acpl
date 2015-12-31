#include "Thread.h"

#include "_cp/Parallel.inc.cpp"

// 
// Mutex
// 

acpl::Mutex::Mutex()
{
	acpl::cp::_mutex_create(this->mHandle);
}

acpl::Mutex::~Mutex()
{
	acpl::cp::_mutex_destroy(this->mHandle);
}


bool acpl::Mutex::Lock()
{
	return acpl::cp::_mutex_lock(this->mHandle);
}

bool acpl::Mutex::TryLock()
{
	return acpl::cp::_mutex_trylock(this->mHandle);
}

bool acpl::Mutex::TimedLock(acpl::UInt32 nMsecs)
{
	return acpl::cp::_mutex_timedlock(this->mHandle, nMsecs);
}

bool acpl::Mutex::Unlock()
{
	return acpl::cp::_mutex_unlock(this->mHandle);
}


// 
// RaiiMutex
// 

acpl::RaiiMutex::RaiiMutex(acpl::Mutex &nRef, bool nAutoLock)
:	mEnabled(true),
	mLocked(false), // auto lock will set this to true (if nAutoLock is true of course)
	mRef(nRef)
{
	if (nAutoLock == true)
		this->Lock();
}

acpl::RaiiMutex::~RaiiMutex()
{
	if (this->mEnabled == true)
		this->Unlock();
}


void acpl::RaiiMutex::Enable()
{
	this->mEnabled = true;
}

void acpl::RaiiMutex::Disable()
{
	this->mEnabled = false;
}


void acpl::RaiiMutex::Lock()
{
	if (this->mLocked != true)
	{
		this->mRef.Lock();
		this->mLocked = true;
	}
}

void acpl::RaiiMutex::Unlock()
{
	if (this->mLocked != false)
	{
		this->mLocked = false;
		this->mRef.Unlock();
	}
}


//
// Condition
//

acpl::Condition::Condition()
{
	acpl::cp::_condition_create(this->mHandle);
}

acpl::Condition::~Condition()
{
	acpl::cp::_condition_destroy(this->mHandle);
}


bool acpl::Condition::Signal()
{
	return acpl::cp::_condition_signal(this->mHandle);
}

bool acpl::Condition::Broadcast()
{
	return acpl::cp::_condition_broadcast(this->mHandle);
}


bool acpl::Condition::Wait(acpl::Mutex &nMutex)
{
	return acpl::cp::_condition_wait(this->mHandle, nMutex.mHandle);
}

bool acpl::Condition::TimedWait(acpl::Mutex &nMutex, acpl::UInt32 nMsecs)
{
	bool oWasTimeout;
	return acpl::cp::_condition_timedwait(this->mHandle, nMutex.mHandle, nMsecs, oWasTimeout);
}

bool acpl::Condition::TimedWait(acpl::Mutex &nMutex, acpl::UInt32 nMsecs, bool &nWasTimeout)
{
	return acpl::cp::_condition_timedwait(this->mHandle, nMutex.mHandle, nMsecs, nWasTimeout);
}


//
// Thread
//

// mStatus structure:
//     * (5) reserved
//     * (1) stop_active flag
//     * (1) stop_signal flag
//     * (1) running flag

#define _local_Thread_Status_Running 0x01
#define _local_Thread_Status_StopSig 0x02
#define _local_Thread_Status_StopAct 0x04

// PUBLIC

acpl::Thread::Thread()
:	mStatus(0x00)
{
	this->mThreadStartStruct.sFunc = acpl::Thread::StartThread;
	this->mThreadStartStruct.sParam = this;
}

acpl::Thread::~Thread()
{
	
}


bool acpl::Thread::IsRunning() const
{
	acpl::RaiiMutex oRm(this->mStatusMutex);
	
	return ((this->mStatus & _local_Thread_Status_Running) != 0);
}

bool acpl::Thread::Start()
{
	acpl::RaiiMutex oRm(this->mStatusMutex);
	
	if (this->mStatus == 0x00 && this->Init() == true)
	{
		if (acpl::cp::_thread_create(this->mHandle, this->mThreadStartStruct) == true)
		{
			this->mStatus |= _local_Thread_Status_Running;
			return true;
		}
		else
		{
			this->Finalize();
			return false;
		}
	}
	else
		return false;
}

void acpl::Thread::Stop()
{
	this->Finish(true);
}

void acpl::Thread::StopAsync()
{
	this->mStatusMutex.Lock();
	this->mStatus |= _local_Thread_Status_StopSig;
	this->Terminate();
	this->mStatusMutex.Unlock();
}


// PROTECTED

bool acpl::Thread::IsStopSig() const
{
	acpl::RaiiMutex oRm(this->mStatusMutex);
	
	return ((this->mStatus & _local_Thread_Status_StopSig) != 0);
}


bool acpl::Thread::Init()
{
	return true;
}

void acpl::Thread::Terminate()
{
	
}

void acpl::Thread::Finalize()
{
	
}


// PRIVATE

void acpl::Thread::StartThread(void *nThread) // static
{
	static_cast<acpl::Thread *>(nThread)->Main();
	static_cast<acpl::Thread *>(nThread)->Finish(false);
}

void acpl::Thread::Finish(bool nClean)
{
	acpl::RaiiMutex oRm(this->mStatusMutex);
	
	// if thread is running but stop procedure is not active
	if ((this->mStatus & (_local_Thread_Status_StopAct | _local_Thread_Status_Running)) == _local_Thread_Status_Running)
	{
		// activate stop procedure and set stop signal (whether or not it is already set)
		this->mStatus |= (_local_Thread_Status_StopAct | _local_Thread_Status_StopSig);
		
		if (nClean == true)
		{
			this->Terminate();
			oRm.Unlock();
			acpl::cp::_thread_join(this->mHandle);
			oRm.Lock();
		}
		else
		{
			// `Terminate()` is called in `StopAsync()`
			acpl::cp::_thread_detach(this->mHandle);
		}
		
		this->Finalize();
		
		this->mStatus = 0x00;
	}
}
