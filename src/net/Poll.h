#ifndef ACPL_NET_POLL_H
#define ACPL_NET_POLL_H

#include "Socket.h"

namespace acpl
{
	
	class Poll
	{
			acplHideClassCopy(Poll);
		
		public:
			typedef union
			{
				int uInt;
				void *uPtr;
			} Handle;
			
			enum LastError
			{
				leNone,
				leSysErr,
				leNoSupport,
				leIsOpened,
				leNotOpened,
				leIsAdded,
				leNotAdded,
				leIsEmpty,
				leSockInval
			};
		
			class Item
			{
					acplHideClassCopyIa(Item, nItem, mSocket(nItem.mSocket));
				
				private:
					acpl::Socket &mSocket;
					bool mWatchRead;
					bool mWatchWrite;
				
				public:
					inline Item(acpl::Socket &nSocket, bool nWatchRead, bool nWatchWrite) : mSocket(nSocket), mWatchRead(nWatchRead), mWatchWrite(nWatchWrite) { }
					inline virtual ~Item() { }
					
					inline acpl::Socket &GetSocket() { return this->mSocket; }
					inline const acpl::Socket &GetSocket() const { return this->mSocket; }
					inline bool IsWatchingRead() const { return this->mWatchRead; }
					inline bool IsWatchingWrite() const { return this->mWatchWrite; }
				
			};
			
			class Event
			{
					acplHideClassCopyIa(Event, nEvent, mSocket(nEvent.mSocket));
				
				public:
					typedef acpl::Mem::Block<acpl::UInt8>::Type Data;

				private:
					acpl::Socket &mSocket;
					bool mCanRead;
					bool mCanWrite;
					bool mHasClosed;
					bool mHasError;
				
				public:
					inline Event(acpl::Socket &nSocket, bool nCanRead, bool nCanWrite, bool nHasClosed, bool nHasError)
						: mSocket(nSocket), mCanRead(nCanRead), mCanWrite(nCanWrite), mHasClosed(nHasClosed), mHasError(nHasError) { }
					inline virtual ~Event() { }
					
					inline acpl::Socket &GetSocket() { return this->mSocket; }
					inline const acpl::Socket &GetSocket() const { return this->mSocket; }
					inline bool CanRead() const { return this->mCanRead; }
					inline bool CanWrite() const { return this->mCanWrite; }
					inline bool HasClosed() const { return this->mHasClosed; }
					inline bool HasError() const { return this->mHasError; }

			};
			
			typedef acpl::ArrayEdaNone<acpl::Poll::Item>::Type ItemList;
			typedef acpl::ArrayEdaNone<acpl::Poll::Event>::Type EventList;
		
		private:
			mutable acpl::Enum<acpl::Poll::LastError, acpl::UInt8> mLastError;
			acpl::Poll::Handle mHandle;
			acpl::Poll::ItemList mItems;
			acpl::Poll::Event::Data mEventData;
		
		public:
			Poll();
			inline virtual ~Poll() { this->Close(); }
			
			inline acpl::Poll::LastError GetLastError() const { return this->mLastError.Value(); }
			inline void ResetLastError() { this->mLastError = acpl::Poll::leNone; }
			
			bool IsOpen() const;
			bool Open();
			void Close();
			
			bool IsAdded(const acpl::Socket &nSocket) const;
			bool Add(acpl::Socket &nSocket, bool nWatchRead, bool nWatchWrite);
			bool Modify(acpl::Socket &nSocket, bool nWatchRead, bool nWatchWrite);
			void Remove(const acpl::Socket &nSocket);
			
			bool Wait(acpl::UInt32 nMsecs, acpl::Poll::EventList &nEvents);
		
	};
	
}

#endif // ACPL_NET_POLL_H
