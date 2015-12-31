#include "../src/net/Socket.h"
#include "../src/Thread.h"
#include "../src/Time.h"

// According to IANA, ports 6584-6587 are unassigned
#define _local_TestPortMain 6584
#define _local_TestPortAlt  6585


typedef acpl::UInt8 _local_EchoBuffer[33];


static inline const char *_local_BoolToStr(bool nVal)
{
	return ((nVal == true) ? "true" : "false");
}

static inline void _local_SetEchoBuffer(_local_EchoBuffer &nBfr, const char *nStr)
{
	for (acpl::UInt8 i = 0; i < (sizeof(_local_EchoBuffer) - 1) && nStr[i] != '\0'; i++)
		nBfr[i] = nStr[i];
}

static inline bool _local_CmpEchoBuffer(const _local_EchoBuffer &nBfr, const char *nStr)
{
	for (acpl::UInt8 i = 0; i < (sizeof(_local_EchoBuffer) - 1) && nStr[i] != '\0'; i++)
		if (nBfr[i] != nStr[i])
			return false;
	
	return true;
}


class _local_SocketPlain : public acpl::Socket
{
	
};

class _local_ServerTcpBase : public acpl::Thread, public acpl::SocketTcp
{
		acplHideClassCopyI(_local_ServerTcpBase, acpl::Thread(), acpl::SocketTcp())
	
	private:
		acpl::UInt32 mMaxQueueLen;
	
	public:
		inline _local_ServerTcpBase(acpl::UInt32 nMaxQueueLen) : mMaxQueueLen(nMaxQueueLen) { }
		inline virtual ~_local_ServerTcpBase() { }
		
		inline virtual bool Init() { return this->Listen(this->mMaxQueueLen); }
		inline virtual void Finalize() { this->Close(); }
};

class _local_SocketUdpTwoWay : public acpl::IoChannel
{
		acplHideClassCopyI(_local_SocketUdpTwoWay, acpl::IoChannel());
	
	private:
		acpl::SocketUdp mRdSock;
		acpl::SocketUdp mWrSock;
	
	protected:
		acpl::SocketUdp &GetRdSock() { return this->mRdSock; }
		const acpl::SocketUdp &GetRdSock() const { return this->mRdSock; }
		acpl::SocketUdp &GetWrSock() { return this->mWrSock; }
		const acpl::SocketUdp &GetWrSock() const { return this->mWrSock; }
	
	public:
		inline _local_SocketUdpTwoWay() { }
		inline virtual ~_local_SocketUdpTwoWay() { }
		
		inline acpl::Socket::Address &GetRdAddress() { return this->mRdSock.GetAddress(); }
		inline const acpl::Socket::Address &GetRdAddress() const { return this->mRdSock.GetAddress(); }
		inline acpl::Socket::Address &GetWrAddress() { return this->mWrSock.GetAddress(); }
		inline const acpl::Socket::Address &GetWrAddress() const { return this->mWrSock.GetAddress(); }
		
		inline bool Setup(const acpl::Ipv4Addr &nRdAddr, const acpl::NetworkPort &nRdPort, const acpl::Ipv4Addr &nWrAddr, const acpl::NetworkPort &nWrPort) { return (this->mRdSock.Setup(nRdAddr, nRdPort) == true && this->mWrSock.Setup(nWrAddr, nWrPort) == true); }
		inline bool Setup(const acpl::Ipv6Addr &nRdAddr, const acpl::NetworkPort &nRdPort, const acpl::Ipv6Addr &nWrAddr, const acpl::NetworkPort &nWrPort) { return (this->mRdSock.Setup(nRdAddr, nRdPort) == true && this->mWrSock.Setup(nWrAddr, nWrPort) == true); }
		inline bool IsSetup() const { return (this->mRdSock.IsSetup() == true && this->mWrSock.IsSetup() == true); }
		inline bool Reset() { return (this->mRdSock.Reset() == true && this->mWrSock.Reset() == true); }
		
		inline bool Open() { return (this->mRdSock.Bind() == true && this->mWrSock.Open() == true); }
		inline bool IsOpen() const { return (this->mRdSock.IsOpen() == true && this->mWrSock.IsOpen() == true); }
		inline void Close() { this->mRdSock.Close(); this->mWrSock.Close(); }
		
		inline bool SetBlocking(bool nBlocking) { return (this->mRdSock.SetBlocking(nBlocking) == true && this->mWrSock.SetBlocking(nBlocking) == true); }
		
		inline acpl::Socket::LastError RdGetLastError() const { return this->mRdSock.GetLastError(); }
		inline acpl::Socket::LastError WrGetLastError() const { return this->mWrSock.GetLastError(); }
		
		inline virtual bool Write(const void *nData, acpl::SizeT nDataSize, acpl::SizeT &nBytesWritten) { return this->mWrSock.Write(nData, nDataSize, nBytesWritten); }
		inline virtual bool Read(void *nData, acpl::SizeT nDataSize, acpl::SizeT &nBytesRead) { return this->mRdSock.Read(nData, nDataSize, nBytesRead); }
		inline bool Read(void *nData, acpl::SizeT nDataSize, acpl::SizeT &nBytesRead, acpl::Socket::Address &nAddress) { return this->mRdSock.Read(nData, nDataSize, nBytesRead, nAddress); }
};
class _local_ServerUdpBase : public acpl::Thread, public _local_SocketUdpTwoWay
{
		acplHideClassCopyI(_local_ServerUdpBase, acpl::Thread(), _local_SocketUdpTwoWay());
	
	public:
		inline _local_ServerUdpBase() { }
		inline virtual ~_local_ServerUdpBase() { }
		
		inline virtual bool Init() { return this->Open(); }
		inline virtual void Finalize() { this->Close(); }
};

class _local_SocketRaw : public acpl::SocketRaw
{
	public:
		inline _local_SocketRaw() { }
		inline virtual ~_local_SocketRaw() { }
		
		inline bool Open(acpl::UInt8 nProtoId, bool nHdrIncl)
		{
			if (this->IOpen(nProtoId) == true && this->ISetHdrIncl(nHdrIncl) == true)
				return true;
			else
			{
				this->Close();
				return false;
			}
		}
		
		inline virtual bool Write(const void *nData, acpl::SizeT nDataSize, acpl::SizeT &nBytesWritten) { return this->IWrite(nData, nDataSize, nBytesWritten); }
		inline virtual bool Read(void *nData, acpl::SizeT nDataSize, acpl::SizeT &nBytesRead) { return this->IRead(nData, nDataSize, nBytesRead); }
		inline bool Read(void *nData, acpl::SizeT nDataSize, acpl::SizeT &nBytesRead, acpl::Socket::Address &nAddress) const { return this->IRead(nData, nDataSize, nBytesRead, nAddress); }
};
