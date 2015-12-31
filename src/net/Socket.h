#ifndef ACPL_NET_SOCKET_H
#define ACPL_NET_SOCKET_H

#include "Address.h"

namespace acpl
{
	class SocketTcp;
	class SocketUdp;
	class SocketRaw;
	class Poll;
	
	class Socket
	{
			acplHideClassCopy(Socket);
		
		public:
			typedef union
			{
				int uInt;
				acpl::SizeT uPsui;
			} Handle;
			
			enum LastError
			{
				leNone,
				leSysErr,
				leNoSupport,
				leIsSetup,
				leNotSetup,
				leIsOpened,
				leNotOpened,
				leNoPerm,
				leInvalProto,
				leAddrInUse,
				leIsBound,
				leWouldBlk,
				leIsConn,
				leNotConn,
				leConnRefused,
				leConnAbort,
				leConnReset,
				leConnTimeout,
				leConnInProg,
				leNetUnreach
			};
			
			class Address
			{
				public:
					typedef acpl::UInt8 Data[28];
				
				private:
					acpl::Socket::Address::Data mData;
				
				protected:
					friend class acpl::Socket;
					inline acpl::Socket::Address::Data &GetData() { return this->mData; }
					inline const acpl::Socket::Address::Data &GetData() const { return this->mData; }
					bool Setup(const acpl::Ipv4Addr &nAddr, const acpl::NetworkPort &nPort);
					bool Setup(const acpl::Ipv6Addr &nAddr, const acpl::NetworkPort &nPort);
					void Reset();
				
				public:
					Address();
					inline virtual ~Address() { this->Reset(); }
					
					bool IsSetup() const;
					bool Get(acpl::Ipv4Addr &nAddr, acpl::NetworkPort &nPort) const;
					bool Get(acpl::Ipv6Addr &nAddr, acpl::NetworkPort &nPort) const;
			};
		
		private:
			mutable acpl::Enum<acpl::Socket::LastError, acpl::UInt8> mLastError;
			acpl::Socket::Address mAddress;
			acpl::Socket::Handle mHandle;
			
			template <class tAddrType>
			inline bool ISetup(const tAddrType &nAddr, const acpl::NetworkPort &nPort) { if (this->mAddress.Setup(nAddr, nPort) == true) { return true; } else { this->mLastError = acpl::Socket::leIsSetup; return false; } }
			
			void Init();
		
		protected:
			friend class acpl::Poll;
			inline Socket() { this->Init(); this->ResetLastError(); }
			
			inline const acpl::Socket::Handle &GetHandle() const { return this->mHandle; }
			
			bool IOpen(const acpl::SocketTcp &nSocket);
			bool IOpen(const acpl::SocketUdp &nSocket);
			bool IOpen(const acpl::SocketRaw &nSocket, acpl::UInt8 nProtoId);
			bool IBind();
			bool IListen(acpl::UInt32 nMaxQueueLen);
			bool IAccept(acpl::Socket &nSocket) const;
			bool IConnect();
			
			// TODO: various setsockopt()
			bool ISetHdrIncl(bool nHdrIncl);
			// bool IIgmpJoin(const acpl::Ipv4Addr &nAddr);
			// bool IIgmpJoin(const acpl::Ipv6Addr &nAddr);
			// bool IIgmpLeave(const acpl::Ipv4Addr &nAddr);
			// bool IIgmpLeave(const acpl::Ipv6Addr &nAddr);
			
			bool IWrite(const void *nData, acpl::SizeT nDataSize, acpl::SizeT &nBytesWritten) const;
			bool IRead(void *nData, acpl::SizeT nDataSize, acpl::SizeT &nBytesRead) const;
			bool IRead(void *nData, acpl::SizeT nDataSize, acpl::SizeT &nBytesRead, acpl::Socket::Address &nAddress) const;
		
		public:
			inline virtual ~Socket() { this->Close(); this->Reset(); }
			
			inline acpl::Socket::LastError GetLastError() const { return this->mLastError.Value(); }
			inline void ResetLastError() { this->mLastError = acpl::Socket::leNone; }
			
			inline acpl::Socket::Address &GetAddress() { return this->mAddress; }
			inline const acpl::Socket::Address &GetAddress() const { return this->mAddress; }
			inline bool Setup(const acpl::Ipv4Addr &nAddr, const acpl::NetworkPort &nPort) { return this->ISetup(nAddr, nPort); }
			inline bool Setup(const acpl::Ipv6Addr &nAddr, const acpl::NetworkPort &nPort) { return this->ISetup(nAddr, nPort); }
			inline bool IsSetup() const { return this->mAddress.IsSetup(); }
			inline bool Reset() { if (this->IsOpen() != true) { this->mAddress.Reset(); return true; } else { this->mLastError = acpl::Socket::leIsOpened; return false; } }
			
			bool IsOpen() const;
			void Close();
			
			bool SetBlocking(bool nBlocking);
	};
	
	class SocketTcp : public acpl::Socket, public acpl::IoChannel
	{
			acplHideClassCopyI(SocketTcp, acpl::Socket(), acpl::IoChannel());
		
		protected:
			inline bool IOpen() { return acpl::Socket::IOpen(*this); }
		
		public:
			inline SocketTcp() { }
			inline virtual ~SocketTcp() { }
			
			inline bool Listen(acpl::UInt32 nMaxQueueLen)
			{
				if (this->IOpen() == true)
				{
					if (this->IBind() == true && this->IListen(nMaxQueueLen) == true)
						return true;
					else
						this->Close();
				}
				return false;
			}
			
			inline bool Accept(acpl::SocketTcp &nSocket) const { return this->IAccept(nSocket); }
			
			inline bool Connect()
			{
				if (this->IOpen() == true)
				{
					if (this->IConnect() == true)
						return true;
					else
						this->Close();
				}
				return false;
			}
			
			inline virtual bool Write(const void *nData, acpl::SizeT nDataSize, acpl::SizeT &nBytesWritten) { return this->IWrite(nData, nDataSize, nBytesWritten); }
			inline virtual bool Read(void *nData, acpl::SizeT nDataSize, acpl::SizeT &nBytesRead) { return this->IRead(nData, nDataSize, nBytesRead); }
	};
	
	class SocketUdp : public acpl::Socket, public acpl::IoChannel
	{
			acplHideClassCopyI(SocketUdp, acpl::Socket(), acpl::IoChannel());
		
		protected:
			inline bool IOpen() { return acpl::Socket::IOpen(*this); }
		
		public:
			inline SocketUdp() { }
			inline virtual ~SocketUdp() { }
			
			inline bool Bind()
			{
				if (this->IOpen() == true)
				{
					if (this->IBind() == true)
						return true;
					else
						this->Close();
				}
				return false;
			}
			
			inline bool Open() { return this->IOpen(); }
			
			inline virtual bool Write(const void *nData, acpl::SizeT nDataSize, acpl::SizeT &nBytesWritten) { return this->IWrite(nData, nDataSize, nBytesWritten); }
			inline virtual bool Read(void *nData, acpl::SizeT nDataSize, acpl::SizeT &nBytesRead) { return this->IRead(nData, nDataSize, nBytesRead); }
			inline bool Read(void *nData, acpl::SizeT nDataSize, acpl::SizeT &nBytesRead, acpl::Socket::Address &nAddress) { return this->IRead(nData, nDataSize, nBytesRead, nAddress); }
	};
	
	class SocketRaw : public acpl::Socket
	{
			acplHideClassCopyI(SocketRaw, acpl::Socket());
		
		protected:
			inline bool IOpen(acpl::UInt8 nProtoId) { return acpl::Socket::IOpen(*this, nProtoId); }
		
		public:
			inline SocketRaw() { }
			inline virtual ~SocketRaw() { }
	};
}

#endif // ACPL_NET_SOCKET_H
