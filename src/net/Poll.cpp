#include "Poll.h"

#include "../_cp/Network.inc.cpp"

// 
// Poll
// 

// INTERNAL

static inline bool _local_Poll_FindItem(const acpl::Poll::ItemList &nItemList, const acpl::Socket &nSocket, acpl::SizeT &nIdx)
{
	for (nIdx = nItemList.Count(); nIdx-- != 0; )
		if (&nItemList.Get(nIdx).GetSocket() == &nSocket)
			return true;
	
	return false;
}

static inline bool _local_Poll_FindItem(const acpl::Poll::ItemList &nItemList, const acpl::Socket &nSocket)
{
	acpl::SizeT oIdx;
	return _local_Poll_FindItem(nItemList, nSocket, oIdx);
}


// PUBLIC

acpl::Poll::Poll()
{
	this->ResetLastError();
	acpl::cp::_net_poll_init(this->mHandle);
}


bool acpl::Poll::IsOpen() const
{
	return acpl::cp::_net_poll_is_open(this->mHandle);
}

bool acpl::Poll::Open()
{
	if (acpl::cp::_net_poll_is_open(this->mHandle) == true)
		this->mLastError = acpl::Poll::leIsOpened;
	else
	if (acpl::cp::_net_poll_open(this->mHandle) == true)
		return true;
	else
		this->mLastError = acpl::cp::_net_poll_get_last_error(acpl::pi::_net_poll_glec_open);
	
	return false;
}

void acpl::Poll::Close()
{
	if (acpl::cp::_net_poll_is_open(this->mHandle) == true)
	{
		acpl::cp::_net_poll_close(this->mHandle);
		acpl::cp::_net_poll_init(this->mHandle);
	}
	
	this->mItems.Clear();
}


bool acpl::Poll::IsAdded(const acpl::Socket &nSocket) const
{
	acpl::SizeT oIdx;
	return _local_Poll_FindItem(this->mItems, nSocket, oIdx);
}

bool acpl::Poll::Add(acpl::Socket &nSocket, bool nWatchRead, bool nWatchWrite)
{
	if (nSocket.IsOpen() != true)
		this->mLastError = acpl::Poll::leSockInval;
	else
	if (_local_Poll_FindItem(this->mItems, nSocket) != false)
		this->mLastError = acpl::Poll::leIsAdded;
	else
	if (acpl::cp::_net_poll_add(this->mHandle, nSocket.GetHandle(), nWatchRead, nWatchWrite, nSocket) == true)
	{
		this->mItems.Add<acpl::Socket &>(nSocket, nWatchRead, nWatchWrite); // NOTE: The first template parameter has to be explicitly specified because some weird stuff happens otherwise
		return true;
	}
	else
		this->mLastError = acpl::cp::_net_poll_get_last_error(acpl::pi::_net_poll_glec_add);
	
	return false;
}

bool acpl::Poll::Modify(acpl::Socket &nSocket, bool nWatchRead, bool nWatchWrite)
{
	acpl::SizeT oIdx;
	if (nSocket.IsOpen() != true)
		this->mLastError = acpl::Poll::leSockInval;
	else
	if (_local_Poll_FindItem(this->mItems, nSocket, oIdx) != true)
		this->mLastError = acpl::Poll::leNotAdded;
	else
	if (acpl::cp::_net_poll_mod(this->mHandle, nSocket.GetHandle(), nWatchRead, nWatchWrite, nSocket) == true)
	{
		acpl::Poll::Item oItem(nSocket, nWatchRead, nWatchWrite);
		this->mItems.Swap(oIdx, oItem);
		return true;
	}
	else
		this->mLastError = acpl::cp::_net_poll_get_last_error(acpl::pi::_net_poll_glec_mod);
	
	return false;
}

void acpl::Poll::Remove(const acpl::Socket &nSocket)
{
	acpl::SizeT oIdx;
	if (_local_Poll_FindItem(this->mItems, nSocket, oIdx) == true)
	{
		acpl::cp::_net_poll_rem(this->mHandle, nSocket.GetHandle());
		this->mItems.Remove(oIdx);
	}
}


bool acpl::Poll::Wait(acpl::UInt32 nMsecs, acpl::Poll::EventList &nEvents)
{
	nEvents.Clear();
	
	if (this->mItems.Count() == 0)
		this->mLastError = acpl::Poll::leIsEmpty;
	else
	if (acpl::cp::_net_poll_wait(this->mHandle, this->mItems, this->mEventData, nMsecs, nEvents) == true)
		return true;
	else
		this->mLastError = acpl::cp::_net_poll_get_last_error(acpl::pi::_net_poll_glec_wait);
	
	return false;
}
