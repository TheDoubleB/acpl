#include "Tests.h"

#include "TestsNetCmn.inc.cpp"


#if   (acplCRuntime == acplCRuntimeGlibc)
#	define _local_SocketErrorDiff_NonBlocking_Connect_Fail acpl::Socket::leConnInProg
#elif (acplCRuntime == acplCRuntimeMscrt)
#	define _local_SocketErrorDiff_NonBlocking_Connect_Fail acpl::Socket::leWouldBlk
#else
#	error Error difference defines missing
#endif

#define _local_SocketErrStr_Case(N_CASE) \
	case acpl::Socket::N_CASE: return "acpl::Socket::"#N_CASE;
const char *_local_SocketErrStr(acpl::Socket::LastError nErr)
{
	switch (nErr)
	{
		_local_SocketErrStr_Case(leNone)
		_local_SocketErrStr_Case(leSysErr)
		_local_SocketErrStr_Case(leNoSupport)
		_local_SocketErrStr_Case(leIsSetup)
		_local_SocketErrStr_Case(leNotSetup)
		_local_SocketErrStr_Case(leIsOpened)
		_local_SocketErrStr_Case(leNotOpened)
		_local_SocketErrStr_Case(leNoPerm)
		_local_SocketErrStr_Case(leInvalProto)
		_local_SocketErrStr_Case(leAddrInUse)
		_local_SocketErrStr_Case(leIsBound)
		_local_SocketErrStr_Case(leWouldBlk)
		_local_SocketErrStr_Case(leIsConn)
		_local_SocketErrStr_Case(leNotConn)
		_local_SocketErrStr_Case(leConnRefused)
		_local_SocketErrStr_Case(leConnAbort)
		_local_SocketErrStr_Case(leConnReset)
		_local_SocketErrStr_Case(leConnTimeout)
		_local_SocketErrStr_Case(leConnInProg)
		_local_SocketErrStr_Case(leNetUnreach)
	}
	
	return "(unknown error)";
}

class _local_SocketTcp_ErrorTests : public acpl::SocketTcp
{
	public:
		inline bool Open() { return this->IOpen(); }
		inline bool Bind() { return this->IBind(); }
		inline bool Listen(acpl::UInt32 nMaxQueueLen)  { return this->IListen(nMaxQueueLen); }
		inline bool Accept(acpl::Socket &nSocket) { return this->IAccept(nSocket); }
		inline bool Connect() { return this->IConnect(); }
		inline bool SetHdrIncl(bool nHdrIncl) { return this->ISetHdrIncl(nHdrIncl); }
};

class _local_ServerTcp_SockImpl : public _local_ServerTcpBase
{
		acplHideClassCopyI(_local_ServerTcp_SockImpl, _local_ServerTcpBase(0))
	
	private:
		bool mCliSockEcho;
		bool mCliSockBlocking;
	
	public:
		inline _local_ServerTcp_SockImpl(acpl::UInt32 nMaxQueueLen, bool nCliSockEcho) : _local_ServerTcpBase(nMaxQueueLen), mCliSockEcho(nCliSockEcho), mCliSockBlocking(false) {  }
		inline virtual ~_local_ServerTcp_SockImpl() { }
		
		inline void SetCliSockBlocking(bool nCliSockBlocking) { this->mCliSockBlocking = nCliSockBlocking; }
		
		inline virtual void Main()
		{
			acpl::Ipv4Addr oLocAddr4;
			acpl::Ipv6Addr oLocAddr6;
			acpl::NetworkPort oLocPort;
			
			if (this->GetAddress().Get(oLocAddr4, oLocPort) == true)
				PrintOut("TCP/IPv4; listen; ip=%s port=%05u\n", acpl::String::IpAddrRef(oLocAddr4).Utf8(), oLocPort.Get())
			else
			if (this->GetAddress().Get(oLocAddr6, oLocPort) == true)
				PrintOut("TCP/IPv6; listen; ip=%s port=%05u\n", acpl::String::IpAddrRef(oLocAddr6).Utf8(), oLocPort.Get())
			
			
			acpl::SocketTcp oCliSock;
			acpl::Ipv4Addr oRemAddr4;
			acpl::Ipv6Addr oRemAddr6;
			acpl::NetworkPort oRemPort;
			
			
			while (this->IsStopSig() != true)
			{
				if (this->Accept(oCliSock) == true)
				{
					if (oCliSock.GetAddress().Get(oRemAddr4, oRemPort) == true)
						PrintOut("TCP/IPv4; accept; ip=%s port=%05u\n", acpl::String::IpAddrRef(oRemAddr4).Utf8(), oRemPort.Get())
					else
					if (oCliSock.GetAddress().Get(oRemAddr6, oRemPort) == true)
						PrintOut("TCP/IPv6; accept; ip=%s port=%05u\n", acpl::String::IpAddrRef(oRemAddr6).Utf8(), oRemPort.Get())
					
					
					oCliSock.SetBlocking(this->mCliSockBlocking);
					
					
					acpl::Time::SleepMsec(20);
					
					if (this->mCliSockEcho == true)
					{
						_local_EchoBuffer oBfr;
						acpl::SizeT oBytesRead = 0;
						acpl::SizeT oBytesWritten = 0;
						
						if (oCliSock.Read(oBfr, sizeof(oBfr) - 1, oBytesRead) == true)
						{
							oBfr[oBytesRead] = '\0';
							PrintOut("          read;  done; len=%u data=%s\n", oBytesRead, oBfr)
						}
						else
						{
							PrintErr("          read;  failed\n")
							_local_SetEchoBuffer(oBfr, "echofail");
							oBfr[8] = '\0';
							oBytesRead = 8;
						}
						
						if (oCliSock.Write(oBfr, oBytesRead, oBytesWritten) == true)
							PrintOut("          write; done; len=%u\n", oBytesWritten)
						else
							PrintErr("          write; failed\n")
					}
					
					oCliSock.Close();
				}
				else
					acpl::Time::SleepMsec(20);
			}
		}
};

class _local_ServerUdp_SockImpl : public _local_ServerUdpBase
{
	acplHideClassCopyI(_local_ServerUdp_SockImpl, _local_ServerUdpBase());
	
	public:
		inline _local_ServerUdp_SockImpl() : _local_ServerUdpBase() { }
		inline virtual ~_local_ServerUdp_SockImpl() { }
		
		inline virtual void Main()
		{
			acpl::Ipv4Addr oBindAddr4;
			acpl::Ipv6Addr oBindAddr6;
			acpl::NetworkPort oBindPort;
			
			if (this->GetRdAddress().Get(oBindAddr4, oBindPort) == true)
				PrintOut("UDP/IPv4; bind;  on    ip=%s port=%05u\n", acpl::String::IpAddrRef(oBindAddr4).Utf8(), oBindPort.Get())
			else
			if (this->GetRdAddress().Get(oBindAddr6, oBindPort) == true)
				PrintOut("UDP/IPv6; bind;  on    ip=%s port=%05u\n", acpl::String::IpAddrRef(oBindAddr6).Utf8(), oBindPort.Get())
			
			
			acpl::Socket::Address oRdAddress;
			acpl::Ipv4Addr oRdAddr4;
			acpl::Ipv6Addr oRdAddr6;
			acpl::NetworkPort oRdPort;
			acpl::Ipv4Addr oWrAddr4;
			acpl::Ipv6Addr oWrAddr6;
			acpl::NetworkPort oWrPort;
			
			while (this->IsStopSig() != true)
			{
				acpl::Time::SleepMsec(20);
				
				_local_EchoBuffer oBfr;
				acpl::SizeT oBytesRead = 0;
				acpl::SizeT oBytesWritten = 0;
				
				if (this->Read(oBfr, sizeof(oBfr) - 1, oBytesRead, oRdAddress) == true)
				{
					oBfr[oBytesRead] = '\0';
					
					if (oRdAddress.Get(oRdAddr4, oRdPort) == true)
						PrintOut("UDP/IPv4; read;  from  ip=%s port=%05u\n", acpl::String::IpAddrRef(oRdAddr4).Utf8(), oRdPort.Get())
					else
					if (oRdAddress.Get(oRdAddr6, oRdPort) == true)
						PrintOut("UDP/IPv6; read;  from  ip=%s port=%05u\n", acpl::String::IpAddrRef(oRdAddr6).Utf8(), oRdPort.Get())
					
					PrintOut("          read;  done; len=%u data=%s\n", oBytesRead, oBfr)
				}
				else
				{
					PrintErr("UDP/IPvX; read;  failed\n")
					_local_SetEchoBuffer(oBfr, "echofail");
					oBfr[8] = '\0';
					oBytesRead = 8;
				}
				
				if (this->Write(oBfr, oBytesRead, oBytesWritten) == true)
				{
					if (this->GetWrAddress().Get(oWrAddr4, oWrPort) == true)
						PrintOut("UDP/IPv4; write; to    ip=%s port=%05u\n", acpl::String::IpAddrRef(oWrAddr4).Utf8(), oWrPort.Get())
					else
					if (this->GetWrAddress().Get(oWrAddr6, oWrPort) == true)
						PrintOut("UDP/IPv6; write; to    ip=%s port=%05u\n", acpl::String::IpAddrRef(oWrAddr6).Utf8(), oWrPort.Get())
					
					PrintOut("          write; done; len=%u data=%s\n", oBytesWritten, oBfr)
				}
				else
					PrintErr("UDP/IPvX; write; failed\n")
			}
		}
};


template <class tAddrType>
static inline int _local_TestTcpConn(const tAddrType &nBindAddr, const tAddrType &nLoopbackAddr)
{
	_local_ServerTcp_SockImpl oSrv(50, false);
	acpl::SocketTcp oCli;
	acpl::NetworkPort oRemPort(_local_TestPortMain);
	
	
	// Setup
	
	Test(oSrv.Setup(nBindAddr, oRemPort) == true);
	Test(oCli.Setup(nLoopbackAddr, oRemPort) == true);
	
	
	// Blocking
	
	Test(oSrv.Start() == true);
	Test(oSrv.Listen(50) == false); // listen was called during Start() call
	Test(oSrv.GetLastError() == acpl::Socket::leIsOpened);
	
	Test(oCli.Connect() == true);
	acpl::Time::SleepMsec(50);
	Test(oCli.Connect() == false);
	Test(oCli.GetLastError() == acpl::Socket::leIsOpened);
	oCli.Close();
	
	Test(oCli.Connect() == true);
	acpl::Time::SleepMsec(50);
	oCli.Close();
	
	oSrv.StopAsync();
	
	Test(oCli.Connect() == true); // to release blocked Accept()
	acpl::Time::SleepMsec(50);
	oCli.Close();
	
	Test(oCli.Connect() == false);
	Test(oCli.GetLastError() == acpl::Socket::leConnRefused);
	
	
	// Non-blocking
	
	Test(oSrv.Start() == true);
	Test(oSrv.SetBlocking(false) == true);
	Test(oSrv.Listen(50) == false); // listen was called during Start() call
	Test(oSrv.GetLastError() == acpl::Socket::leIsOpened);
	
	Test(oCli.Connect() == true);
	acpl::Time::SleepMsec(50);
	Test(oCli.Connect() == false);
	Test(oCli.GetLastError() == acpl::Socket::leIsOpened);
	oCli.Close();
	
	Test(oCli.Connect() == true);
	acpl::Time::SleepMsec(50);
	oCli.Close();
	
	Test(oCli.Connect() == true);
	acpl::Time::SleepMsec(50);
	oCli.Close();
	
	oSrv.Stop();
	
	Test(oCli.Connect() == false);
	Test(oCli.GetLastError() == acpl::Socket::leConnRefused);
	
	
	return 0;
}
template <class tAddrType>
static inline int _local_TestTcpConn()
{
	Test(_local_TestTcpConn(tAddrType().SetAny(), tAddrType().SetLoopback()) == 0);
	Test(_local_TestTcpConn(tAddrType().SetLoopback(), tAddrType().SetLoopback()) == 0);
	return 0;
}

template <class tAddrType>
static inline int _local_TestTcpComm(const tAddrType &nBindAddr, const tAddrType &nLoopbackAddr)
{
	_local_ServerTcp_SockImpl oSrv(50, true);
	acpl::SocketTcp oCli;
	acpl::NetworkPort oRemPort(_local_TestPortMain);
	
	_local_EchoBuffer oBfr;
	acpl::SizeT oBytesWritten, oBytesRead;
	
	
	// Setup
	
	Test(oSrv.Setup(nBindAddr, oRemPort) == true);
	Test(oCli.Setup(nLoopbackAddr, oRemPort) == true);
	
	
	// Blocking
	
	_local_SetEchoBuffer(oBfr, "+blk");
	
	oSrv.SetCliSockBlocking(true);
	Test(oSrv.Start() == true);
	Test(oSrv.SetBlocking(false) == true);
	
	Test(oCli.Connect() == true);
	
	acpl::Time::SleepMsec(50); // So that server locks on blocking `Read()`
	
	Test(oCli.Write(oBfr, 4, oBytesWritten) == true);
	Test(oBytesWritten == 4);
	
	Test(oCli.Read(oBfr, 4, oBytesRead) == true);
	Test(oBytesRead == 4);
	
	Test(_local_CmpEchoBuffer(oBfr, "+blk") == true);
	
	oCli.Close();
	
	oSrv.Stop();
	
	
	// Non-blocking
	
	acpl::Time::SleepMsec(10); // Needs a bit of sleep because MSCRT doesn't always release the address quick enough with exclusive use
	_local_SetEchoBuffer(oBfr, "-blk");
	
	oSrv.SetCliSockBlocking(false);
	Test(oSrv.Start() == true);
	Test(oSrv.SetBlocking(false) == true);
	
	Test(oCli.Connect() == true);
	
	// Must continue immediately (no sleep) because of server's non-blocking `Read()`
	
	Test(oCli.Write(oBfr, 4, oBytesWritten) == true);
	Test(oBytesWritten == 4);
	
	Test(oCli.Read(oBfr, 4, oBytesRead) == true);
	Test(oBytesRead == 4);
	
	Test(_local_CmpEchoBuffer(oBfr, "-blk") == true);
	
	oCli.Close();
	
	oSrv.Stop();
	
	
	// Non-blocking
	
	acpl::Time::SleepMsec(10); // Needs a bit of sleep because MSCRT doesn't always release the address quick enough with exclusive use
	_local_SetEchoBuffer(oBfr, "-blk");
	
	oSrv.SetCliSockBlocking(false);
	Test(oSrv.Start() == true);
	Test(oSrv.SetBlocking(false) == true);
	
	Test(oCli.Connect() == true);
	
	acpl::Time::SleepMsec(50); // So that server fails on non-blocking `Read()`
	
	// No write
	
	Test(oCli.Read(oBfr, 8, oBytesRead) == true);
	Test(oBytesRead == 8);
	
	Test(_local_CmpEchoBuffer(oBfr, "echofail") == true);
	
	oCli.Close();
	
	oSrv.Stop();
	
	
	return 0;
}
template <class tAddrType>
static inline int _local_TestTcpComm()
{
	Test(_local_TestTcpComm(tAddrType().SetAny(), tAddrType().SetLoopback()) == 0);
	acpl::Time::SleepMsec(50); // MSCRT does not properly synchronously close sockets; even tried with the DisconnectEx(). TODO: Needs more study.
	Test(_local_TestTcpComm(tAddrType().SetLoopback(), tAddrType().SetLoopback()) == 0);
	return 0;
}

template <class tAddrType>
static inline int _local_TestUdpComm(const tAddrType &nBindAddr, const tAddrType &nLoopbackAddr)
{
	_local_ServerUdp_SockImpl oSrv;
	_local_SocketUdpTwoWay oCli;
	acpl::NetworkPort oPort1(_local_TestPortMain);
	acpl::NetworkPort oPort2(_local_TestPortAlt);
	
	_local_EchoBuffer oBfr;
	acpl::SizeT oBytesWritten, oBytesRead;
	
	
	// Setup
	
	Test(oSrv.Setup(nBindAddr, oPort1, nLoopbackAddr, oPort2) == true);
	Test(oCli.Setup(nLoopbackAddr, oPort2, nLoopbackAddr, oPort1) == true);
	
	
	// Blocking
	
	_local_SetEchoBuffer(oBfr, "+blk");
	
	Test(oSrv.Start() == true);
	Test(oSrv.SetBlocking(true) == true);
	
	Test(oCli.Open() == true);
	Test(oCli.Open() == false);
	Test(oCli.RdGetLastError() == acpl::Socket::leIsOpened);
	// oCli.WrGetLastError doesn't even matter
	
	acpl::Time::SleepMsec(50); // So that server locks on blocking `Read()`
	oSrv.StopAsync(); // So that server will exit main loop and not relock on blocking `Read()`
	
	Test(oCli.Write(oBfr, 4, oBytesWritten) == true);
	Test(oBytesWritten == 4);
	
	Test(oCli.Read(oBfr, 4, oBytesRead) == true);
	Test(oBytesRead == 4);
	
	Test(_local_CmpEchoBuffer(oBfr, "+blk") == true);
	
	oCli.Close();
	
	while (oSrv.IsRunning() == true) // Wait until server stops
		acpl::Time::SleepMsec(10);
	
	
	// Non-blocking
	
	_local_SetEchoBuffer(oBfr, "-blk");
	
	Test(oSrv.Start() == true);
	Test(oSrv.SetBlocking(false) == true);
	
	Test(oCli.Open() == true);
	
	// Must continue immediately (no sleep) because of server's non-blocking `Read()`
	
	Test(oCli.Write(oBfr, 4, oBytesWritten) == true);
	Test(oBytesWritten == 4);
	
	acpl::Time::SleepMsec(10);
	oSrv.StopAsync(); // So that server will exit main loop
	
	Test(oCli.Read(oBfr, 4, oBytesRead) == true);
	Test(oBytesRead == 4);
	
	Test(_local_CmpEchoBuffer(oBfr, "-blk") == true);
	
	oCli.Close();
	
	while (oSrv.IsRunning() == true) // Wait until server stops
		acpl::Time::SleepMsec(10);
	
	
	// Non-blocking
	
	_local_SetEchoBuffer(oBfr, "-blk");
	
	Test(oSrv.Start() == true);
	Test(oSrv.SetBlocking(false) == true);
	
	Test(oCli.Open() == true);
	
	// No write
	
	acpl::Time::SleepMsec(10);
	oSrv.StopAsync(); // So that server will exit main loop
	
	acpl::Time::SleepMsec(40); // So that server fails on non-blocking `Read()`
	
	Test(oCli.Read(oBfr, 8, oBytesRead) == true);
	Test(oBytesRead == 8);
	
	Test(_local_CmpEchoBuffer(oBfr, "echofail") == true);
	
	oCli.Close();
	
	while (oSrv.IsRunning() == true) // Wait until server stops
		acpl::Time::SleepMsec(10);
	
	
	return 0;
}
template <class tAddrType>
static inline int _local_TestUdpComm()
{
	Test(_local_TestUdpComm(tAddrType().SetAny(), tAddrType().SetLoopback()) == 0);
	Test(_local_TestUdpComm(tAddrType().SetLoopback(), tAddrType().SetLoopback()) == 0);
	return 0;
}


static int TestPlainSocket()
{
	PrintFn();
	
	
	_local_SocketPlain oPs;
	acpl::Ipv4Addr oAddr4;
	acpl::Ipv6Addr oAddr6;
	acpl::NetworkPort oPort;
	
	
	// Init
	
	Test(oPs.IsSetup() == false);
	Test(oPs.GetAddress().Get(oAddr4, oPort) == false);
	Test(oPs.GetAddress().Get(oAddr6, oPort) == false);
	
	Test(oPs.IsOpen() == false);
	oPs.Close();
	
	Test(oPs.SetBlocking(true) == false);
	Test(oPs.SetBlocking(false) == false);
	
	
	// Setup (IPv4)
	
	Test(oPs.Setup(acpl::Ipv4Addr(10, 20, 100, 200), acpl::NetworkPort(1234)) == true);
	Test(oPs.Setup(acpl::Ipv4Addr(10, 20, 100, 200), acpl::NetworkPort(1234)) == false); // already set-up
	Test(oPs.GetLastError() == acpl::Socket::leIsSetup);
	Test(oPs.IsSetup() == true);
	Test(oPs.GetAddress().Get(oAddr4, oPort) == true);
	Test(oAddr4.Data()[0] == 10);
	Test(oAddr4.Data()[1] == 20);
	Test(oAddr4.Data()[2] == 100);
	Test(oAddr4.Data()[3] == 200);
	Test(oPort.Get() == 1234);
	Test(oPs.GetAddress().Get(oAddr6, oPort) == false);
	Test(oPs.Reset() == true);
	Test(oPs.IsSetup() == false);
	Test(oPs.GetAddress().Get(oAddr4, oPort) == false);
	Test(oPs.GetAddress().Get(oAddr6, oPort) == false);
	
	
	// Setup (IPv6)
	
	Test(oPs.Setup(acpl::Ipv6Addr(0xFEDC, 0xBA98, 0x7654, 0x3210, 0xFEDC, 0xBA98, 0x7654, 0x3210), acpl::NetworkPort(1234)) == true);
	Test(oPs.Setup(acpl::Ipv6Addr(0xFEDC, 0xBA98, 0x7654, 0x3210, 0xFEDC, 0xBA98, 0x7654, 0x3210), acpl::NetworkPort(1234)) == false); // already set-up
	Test(oPs.GetLastError() == acpl::Socket::leIsSetup);
	Test(oPs.IsSetup() == true);
	Test(oPs.GetAddress().Get(oAddr4, oPort) == false);
	Test(oPs.GetAddress().Get(oAddr6, oPort) == true);
	Test(oAddr6.Data()[ 0] == 0xFE);
	Test(oAddr6.Data()[ 1] == 0xDC);
	Test(oAddr6.Data()[ 2] == 0xBA);
	Test(oAddr6.Data()[ 3] == 0x98);
	Test(oAddr6.Data()[ 4] == 0x76);
	Test(oAddr6.Data()[ 5] == 0x54);
	Test(oAddr6.Data()[ 6] == 0x32);
	Test(oAddr6.Data()[ 7] == 0x10);
	Test(oAddr6.Data()[ 8] == 0xFE);
	Test(oAddr6.Data()[ 9] == 0xDC);
	Test(oAddr6.Data()[10] == 0xBA);
	Test(oAddr6.Data()[11] == 0x98);
	Test(oAddr6.Data()[12] == 0x76);
	Test(oAddr6.Data()[13] == 0x54);
	Test(oAddr6.Data()[14] == 0x32);
	Test(oAddr6.Data()[15] == 0x10);
	Test(oPort.Get() == 1234);
	Test(oPs.Reset() == true);
	Test(oPs.IsSetup() == false);
	Test(oPs.GetAddress().Get(oAddr4, oPort) == false);
	Test(oPs.GetAddress().Get(oAddr6, oPort) == false);
	
	
	return 0;
}

static int TestLastErrors()
{
	PrintFn();
	
	acpl::Ipv4Addr oAddr;
	oAddr.SetLoopback();
	acpl::NetworkPort oPort(_local_TestPortMain);
	_local_SocketTcp_ErrorTests oTSock, oTSockAlt;
	acpl::SocketTcp oCli;
	acpl::UInt8 oBfr[1] = { 0 };
	
	
	// Blank
	
	Test(oTSock.GetLastError() == acpl::Socket::leNone);
	
	
	// Tests on non-setup socket and Setup()
	
	Test(oTSock.Open() == false);
	Test(oTSock.GetLastError() == acpl::Socket::leNotSetup);
	
	Test(oTSock.Setup(oAddr, oPort) == true);
	Test(oTSock.Setup(oAddr, oPort) == false);
	Test(oTSock.GetLastError() == acpl::Socket::leIsSetup);
	
	
	// Tests on non-opened socket and Open()
	
	Test(oTSock.Bind() == false);
	Test(oTSock.GetLastError() == acpl::Socket::leNotOpened);
	Test(oTSock.Setup(oAddr, oPort) == false); // "reset" last error
	Test(oTSock.Listen(100) == false);
	Test(oTSock.GetLastError() == acpl::Socket::leNotOpened);
	Test(oTSock.Setup(oAddr, oPort) == false); // "reset" last error
	Test(oTSock.SetBlocking(false) == false);
	Test(oTSock.GetLastError() == acpl::Socket::leNotOpened);
	Test(oTSock.Setup(oAddr, oPort) == false); // "reset" last error
	Test(oTSock.Accept(oCli) == false);
	Test(oTSock.GetLastError() == acpl::Socket::leNotOpened);
	Test(oTSock.Setup(oAddr, oPort) == false); // "reset" last error
	Test(oTSock.Connect() == false);
	Test(oTSock.GetLastError() == acpl::Socket::leNotOpened);
	Test(oTSock.Setup(oAddr, oPort) == false); // "reset" last error
	Test(oTSock.SetHdrIncl(true) == false);
	Test(oTSock.GetLastError() == acpl::Socket::leNotOpened);
	Test(oTSock.Setup(oAddr, oPort) == false); // "reset" last error
	Test(oTSock.WriteAll(oBfr, sizeof(oBfr)) == false);
	Test(oTSock.GetLastError() == acpl::Socket::leNotOpened);
	Test(oTSock.Setup(oAddr, oPort) == false); // "reset" last error
	Test(oTSock.ReadAll(oBfr, sizeof(oBfr)) == false);
	Test(oTSock.GetLastError() == acpl::Socket::leNotOpened);
	Test(oTSock.Setup(oAddr, oPort) == false); // "reset" last error
	// TODO: Other tests on non-opened socket?
	
	Test(oTSock.Open() == true);
	Test(oTSock.Open() == false);
	Test(oTSock.GetLastError() == acpl::Socket::leIsOpened);
	
	
	// Various functions on opened socket
	
	Test(oTSock.Bind() == true);
	Test(oTSock.Bind() == false);
	Test(oTSock.GetLastError() == acpl::Socket::leIsBound);
	oTSock.Close();
	
	acpl::Time::SleepMsec(50); // MSCRT does not properly synchronously close sockets; even tried with the DisconnectEx(). TODO: Needs more study.
	
	Test(oTSock.Open() == true);
	// Apparently, you can call Listen() on a non-bound socket multiple times and get a `true` every time... wut?? I don't even...
	Test(oTSock.Bind() == true);
	Test(oTSock.Listen(100) == true);
	// Apparently, you can call Listen() on a bound socket multiple times and get a `true` every time as well... alrighty then?
	// Setup(), Open(), Bind(), and try Listen() on Alt socket to the same address
	Test(oTSockAlt.Setup(oAddr, oPort) == true);
	Test(oTSockAlt.Open() == true);
	Test(oTSockAlt.Bind() == false);
	Test(oTSockAlt.GetLastError() == acpl::Socket::leAddrInUse);
	// Apparently, you can call Listen() on a non-bound socket multiple times and get a `true` every time... !even though the same address is already Listen()ed on! *sigh*
	oTSock.Close();
	oTSockAlt.Close();
	
	acpl::Time::SleepMsec(50); // MSCRT does not properly synchronously close sockets; even tried with the DisconnectEx(). TODO: Needs more study.
	
	Test(oTSock.Open() == true);
	Test(oTSock.Bind() == true);
	Test(oTSock.Listen(100) == true);
	Test(oTSock.SetBlocking(false) == true);
	Test(oTSock.Accept(oCli) == false);
	Test(oTSock.GetLastError() == acpl::Socket::leWouldBlk);
	oTSock.Close();
	
	Test(oTSock.Open() == true);
	Test(oTSock.Connect() == false);
	Test(oTSock.GetLastError() == acpl::Socket::leConnRefused);
	Test(oTSock.SetBlocking(false) == true);
	Test(oTSock.Connect() == false);
	Test(oTSock.GetLastError() == _local_SocketErrorDiff_NonBlocking_Connect_Fail);
	oTSock.Close();
	
	
	// Reset
	
	oTSock.ResetLastError();
	Test(oTSock.GetLastError() == acpl::Socket::leNone);
	
	
	return 0;
}

static int TestTcpIpv4Conn()
{
	PrintFn();
	Test(_local_TestTcpConn<acpl::Ipv4Addr>() == 0);
	return 0;
}

static int TestTcpIpv6Conn()
{
	PrintFn();
	Test(_local_TestTcpConn<acpl::Ipv6Addr>() == 0);
	return 0;
}

static int TestTcpIpv4Comm()
{
	PrintFn();
	Test(_local_TestTcpComm<acpl::Ipv4Addr>() == 0);
	return 0;
}

static int TestTcpIpv6Comm()
{
	PrintFn();
	Test(_local_TestTcpComm<acpl::Ipv6Addr>() == 0);
	return 0;
}

static int TestUdpIpv4Comm()
{
	PrintFn();
	Test(_local_TestUdpComm<acpl::Ipv4Addr>() == 0);
	return 0;
}

static int TestUdpIpv6Comm()
{
	PrintFn();
	Test(_local_TestUdpComm<acpl::Ipv6Addr>() == 0);
	return 0;
}


SectionFuncMain(netsock)
{
	if (argc < 1)
	{
		Test(TestPlainSocket() == 0);
		Test(TestLastErrors()  == 0);
		Test(TestTcpIpv4Conn() == 0);
		Test(TestTcpIpv6Conn() == 0);
		Test(TestTcpIpv4Comm() == 0);
		Test(TestTcpIpv6Comm() == 0);
		Test(TestUdpIpv4Comm() == 0);
		Test(TestUdpIpv6Comm() == 0);
		
		PrintOut("All Passed!\n");
	}
	else
		return PrintUnrecParams(acplDbgSrcLoc, lSectionName, argc, argv);
	
	return 0;
}

SectionFuncParamsBegin(netsock)
// SectionFuncParamsAdd("param [param [param ...]]", "description")
SectionFuncParamsEnd(netsock)
