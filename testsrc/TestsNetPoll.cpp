#include "Tests.h"

#include "TestsNetCmn.inc.cpp"

#include "../src/net/Poll.h"


#define _local_PollErrStr_Case(N_CASE) \
	case acpl::Poll::N_CASE: return "acpl::Poll::"#N_CASE;
const char *_local_PollErrStr(acpl::Poll::LastError nErr)
{
	switch (nErr)
	{
		_local_PollErrStr_Case(leNone)
		_local_PollErrStr_Case(leSysErr)
		_local_PollErrStr_Case(leNoSupport)
		_local_PollErrStr_Case(leIsOpened)
		_local_PollErrStr_Case(leNotOpened)
		_local_PollErrStr_Case(leIsAdded)
		_local_PollErrStr_Case(leNotAdded)
		_local_PollErrStr_Case(leIsEmpty)
		_local_PollErrStr_Case(leSockInval)
	}
	
	return "(unknown error)";
}


class _local_ServerTcp_PollImpl : public _local_ServerTcpBase
{
		acplHideClassCopyI(_local_ServerTcp_PollImpl, _local_ServerTcpBase(0))
	
	private:
		bool mCliSockWrite;
		acpl::UInt32 mCliSockSbcMsec; // Sleep-before-close (in msec)
	
	public:
		inline _local_ServerTcp_PollImpl(acpl::UInt32 nMaxQueueLen) : _local_ServerTcpBase(nMaxQueueLen), mCliSockWrite(false), mCliSockSbcMsec(0) {  }
		inline virtual ~_local_ServerTcp_PollImpl() { }
		
		inline void SetCliSockWrite(bool nCliSockWrite) { this->mCliSockWrite = nCliSockWrite; }
		inline void SetCliSockSbcMsec(acpl::UInt32 nCliSockSbcMsec) { this->mCliSockSbcMsec = nCliSockSbcMsec; }
		
		inline virtual void Main()
		{
			acpl::SocketTcp oCliSock;
			
			while (this->IsStopSig() != true)
			{
				if (this->Accept(oCliSock) == true)
				{
					if (this->mCliSockWrite == true)
					{
						acpl::SizeT oBytesWritten = 0;
						if (oCliSock.Write("srvwrite", 8, oBytesWritten) != true)
							PrintErr("Write failed\n")
					}
					
					if (this->mCliSockSbcMsec > 0)
						acpl::Time::SleepMsec(this->mCliSockSbcMsec);
					
					oCliSock.Close();
				}
				else
					acpl::Time::SleepMsec(20);
			}
		}
};

class _local_ClientTcpConnector : public acpl::Thread
{
		acplHideClassCopyI(_local_ClientTcpConnector, acpl::Thread())
	
	private:
		acpl::UInt8 mRepeat;
		acpl::SocketTcp mSock;
		acpl::UInt32 mSbcMsec; // Sleep-before-close (in msec)
	
	public:
		inline _local_ClientTcpConnector(acpl::UInt8 nRepeat) : mRepeat(nRepeat), mSbcMsec(0) {  }
		inline virtual ~_local_ClientTcpConnector() {  }
		
		inline bool Setup(const acpl::Ipv4Addr &nAddr, const acpl::NetworkPort &nPort) { return this->mSock.Setup(nAddr, nPort); }
		inline bool Setup(const acpl::Ipv6Addr &nAddr, const acpl::NetworkPort &nPort) { return this->mSock.Setup(nAddr, nPort); }
		inline void SetSbcMsec(acpl::UInt32 nSbcMsec) { this->mSbcMsec = nSbcMsec; }
		
		inline virtual bool Init() { return this->mSock.IsSetup(); }
		inline virtual void Main()
		{
			while (this->mRepeat-- != 0)
			{
				if (this->mSock.Connect() == true)
				{	
					if (this->mSbcMsec > 0)
						acpl::Time::SleepMsec(this->mSbcMsec);
					
					this->mSock.Close();
				}
			}
		}
};




static int TestPollTypes()
{
	PrintFn();
	
	
	_local_SocketPlain oSock;
	acpl::Poll::ItemList oItems;
	acpl::Poll::EventList oEvents;
	
	
	// Poll::Item
	
	Test(oItems.Add<acpl::Socket &>(oSock, false, false) == true);
	Test(&oItems.GetLast().GetSocket() == &oSock);
	Test(oItems.GetLast().IsWatchingRead() == false);
	Test(oItems.GetLast().IsWatchingWrite() == false);
	
	Test(oItems.Add<acpl::Socket &>(oSock, true, false) == true);
	Test(&oItems.GetLast().GetSocket() == &oSock);
	Test(oItems.GetLast().IsWatchingRead() == true);
	Test(oItems.GetLast().IsWatchingWrite() == false);
	
	Test(oItems.Add<acpl::Socket &>(oSock, false, true) == true);
	Test(&oItems.GetLast().GetSocket() == &oSock);
	Test(oItems.GetLast().IsWatchingRead() == false);
	Test(oItems.GetLast().IsWatchingWrite() == true);
	
	
	// Poll::Event
	
	Test(oEvents.Add<acpl::Socket &>(oSock, false, false, false, false) == true);
	Test(&oEvents.GetLast().GetSocket() == &oSock);
	Test(oEvents.GetLast().CanRead() == false);
	Test(oEvents.GetLast().CanWrite() == false);
	Test(oEvents.GetLast().HasClosed() == false);
	Test(oEvents.GetLast().HasError() == false);
	
	Test(oEvents.Add<acpl::Socket &>(oSock, true, false, false, false) == true);
	Test(&oEvents.GetLast().GetSocket() == &oSock);
	Test(oEvents.GetLast().CanRead() == true);
	Test(oEvents.GetLast().CanWrite() == false);
	Test(oEvents.GetLast().HasClosed() == false);
	Test(oEvents.GetLast().HasError() == false);
	
	Test(oEvents.Add<acpl::Socket &>(oSock, false, true, false, false) == true);
	Test(&oEvents.GetLast().GetSocket() == &oSock);
	Test(oEvents.GetLast().CanRead() == false);
	Test(oEvents.GetLast().CanWrite() == true);
	Test(oEvents.GetLast().HasClosed() == false);
	Test(oEvents.GetLast().HasError() == false);
	
	Test(oEvents.Add<acpl::Socket &>(oSock, false, false, true, false) == true);
	Test(&oEvents.GetLast().GetSocket() == &oSock);
	Test(oEvents.GetLast().CanRead() == false);
	Test(oEvents.GetLast().CanWrite() == false);
	Test(oEvents.GetLast().HasClosed() == true);
	Test(oEvents.GetLast().HasError() == false);
	
	Test(oEvents.Add<acpl::Socket &>(oSock, false, false, false, true) == true);
	Test(&oEvents.GetLast().GetSocket() == &oSock);
	Test(oEvents.GetLast().CanRead() == false);
	Test(oEvents.GetLast().CanWrite() == false);
	Test(oEvents.GetLast().HasClosed() == false);
	Test(oEvents.GetLast().HasError() == true);
	
	
	return 0;
}

static int TestPollBasic()
{
	PrintFn();
	
	
	acpl::Poll oPoll;
	
	
	// Init
	
	Test(oPoll.GetLastError() == acpl::Poll::leNone);
	Test(oPoll.IsOpen() == false);
	oPoll.Close();
	Test(oPoll.IsOpen() == false);
	
	
	// Open/close
	
	Test(oPoll.IsOpen() == false);
	Test(oPoll.Open() == true);
	Test(oPoll.IsOpen() == true);
	Test(oPoll.Open() == false);
	Test(oPoll.GetLastError() == acpl::Poll::leIsOpened);
	oPoll.Close();
	Test(oPoll.IsOpen() == false);
	
	
	// Reset
	
	oPoll.ResetLastError();
	Test(oPoll.GetLastError() == acpl::Poll::leNone);
	
	
	return 0;
}

static int TestPollAddRem()
{
	PrintFn();
	
	
	acpl::Poll oPoll;
	acpl::SocketUdp oSock;
	
	
	// Non-opened socket, non-opened poll
	
	Test(oPoll.IsAdded(oSock) == false);
	Test(oPoll.Add(oSock, false, false) == false);
	Test(oPoll.GetLastError() == acpl::Poll::leSockInval);
	Test(oPoll.Modify(oSock, false, false) == false);
	Test(oPoll.GetLastError() == acpl::Poll::leSockInval);
	oPoll.Remove(oSock);
	
	
	// Socket open
	
	Test(oSock.Setup(acpl::Ipv4Addr().SetLoopback(), acpl::NetworkPort(_local_TestPortMain)) == true);
	Test(oSock.Open() == true);
	
	
	// Opened socket, non-opened poll
	
	Test(oPoll.IsAdded(oSock) == false);
	Test(oPoll.Add(oSock, false, false) == false);
	Test(oPoll.GetLastError() == acpl::Poll::leNotOpened);
	Test(oPoll.Modify(oSock, false, false) == false);
	Test(oPoll.GetLastError() == acpl::Poll::leNotAdded);
	oPoll.Remove(oSock);
	
	
	// Poll open
	
	Test(oPoll.Open() == true);
	
	// Opened socket, opened poll
	
	Test(oPoll.Add(oSock, false, false) == true);
	Test(oPoll.IsAdded(oSock) == true);
	Test(oPoll.Add(oSock, false, false) == false);
	Test(oPoll.GetLastError() == acpl::Poll::leIsAdded);
	Test(oPoll.IsAdded(oSock) == true);
	Test(oPoll.Modify(oSock, false, false) == true);
	Test(oPoll.IsAdded(oSock) == true);
	oPoll.Remove(oSock);
	Test(oPoll.IsAdded(oSock) == false);
	Test(oPoll.Modify(oSock, false, false) == false);
	Test(oPoll.GetLastError() == acpl::Poll::leNotAdded);
	
	
	// Socket teardown
	
	oSock.Close();
	
	
	// Poll teardown
	
	oPoll.Close();
	
	
	return 0;
}

static int TestPollWaitSingle()
{
	PrintFn();
	
	
	acpl::Ipv4Addr oAddr;
	acpl::NetworkPort oPort;
	
	_local_ServerTcp_PollImpl oSrv(50);
	acpl::SocketTcp oCli;
	
	acpl::Poll oPoll;
	acpl::Poll::EventList oPollEvs;
	
	_local_EchoBuffer oBfr;
	acpl::SizeT oBytesRead;
	
	
	// Setup
	
	oAddr.SetLoopback();
	oPort.Set(_local_TestPortMain);
	
	Test(oSrv.Setup(oAddr, oPort) == true);
	Test(oCli.Setup(oAddr, oPort) == true);
	
	
	// Start
	Test(oSrv.Start() == true);
	Test(oSrv.SetBlocking(false) == true);
	Test(oPoll.Open() == true);
	
	
	// Empty poll
	
	Test(oPoll.Wait(100, oPollEvs) == false);
	Test(oPoll.GetLastError() == acpl::Poll::leIsEmpty);
	
	
	// Poll on single closed socket (server drop through, no server write, poll for none, poll catch)
	
	oSrv.SetCliSockWrite(false);
	oSrv.SetCliSockSbcMsec(0); // drop through
	
	Test(oCli.Connect() == true);
	Test(oPoll.Add(oCli, false, false) == true);
	oCli.Close();
	
	Test(oPoll.Wait(100, oPollEvs) == true);
	
	Test(oPollEvs.Count() == 0);
	
	Test(oPoll.IsAdded(oCli) == true);
	Test(oPoll.Modify(oCli, false, false) == false);
	Test(oPoll.GetLastError() == acpl::Poll::leSockInval);
	
	oPoll.Remove(oCli);
	Test(oPoll.IsAdded(oCli) == false);
	
	
	// Single client, server drop through, no server write, poll for none, poll catch
	
	Test(oCli.Connect() == true);
	Test(oPoll.Add(oCli, false, false) == true);
	
	Test(oPoll.Wait(100, oPollEvs) == true);
	
	Test(oPollEvs.Count() == 1);
	Test(&oPollEvs.GetLast().GetSocket() == &oCli);
	Test(oPollEvs.GetLast().CanRead() == false);
	Test(oPollEvs.GetLast().CanWrite() == false);
	Test(oPollEvs.GetLast().HasClosed() == true); // has closed
	Test(oPollEvs.GetLast().HasError() == false);
	
	oPoll.Remove(oCli);
	oCli.Close();
	
	
	// Single client, server sleep, no server write, poll for none, poll timeout
	
	oSrv.SetCliSockWrite(false);
	oSrv.SetCliSockSbcMsec(50); // sleep
	
	Test(oCli.Connect() == true);
	Test(oPoll.Add(oCli, false, false) == true);
	
	Test(oPoll.Wait(10, oPollEvs) == true);
	
	Test(oPollEvs.Count() == 0);
	
	oPoll.Remove(oCli);
	oCli.Close();
	
	
	// Single client, server drop through, no server write, poll for write, poll catch
	
	oSrv.SetCliSockWrite(false);
	oSrv.SetCliSockSbcMsec(0); // drop through
	
	Test(oCli.Connect() == true);
	Test(oPoll.Add(oCli, false, true) == true);
	
	Test(oPoll.Wait(100, oPollEvs) == true);
	
	Test(oPollEvs.Count() == 1);
	Test(&oPollEvs.GetLast().GetSocket() == &oCli);
	Test(oPollEvs.GetLast().CanRead() == false);
	Test(oPollEvs.GetLast().CanWrite() == true);
	Test(oPollEvs.GetLast().HasClosed() == false);
	Test(oPollEvs.GetLast().HasError() == false);
	
	Test(oPoll.Modify(oCli, false, false) == true); // check only for hang-up (close)
	Test(oPoll.Wait(100, oPollEvs) == true);
	
	Test(oPollEvs.Count() == 1);
	Test(&oPollEvs.GetLast().GetSocket() == &oCli);
	// don't test for read or write
	Test(oPollEvs.GetLast().HasClosed() == true);
	Test(oPollEvs.GetLast().HasError() == false);
	
	oPoll.Remove(oCli);
	oCli.Close();
	
	
	// Single client, server drop through, no server write, poll for rdwr, poll catch
	
	oSrv.SetCliSockWrite(false);
	oSrv.SetCliSockSbcMsec(0); // drop through
	
	Test(oCli.Connect() == true);
	Test(oPoll.Add(oCli, true, true) == true);
	
	Test(oPoll.Wait(100, oPollEvs) == true);
	
	Test(oPollEvs.Count() == 1);
	Test(&oPollEvs.GetLast().GetSocket() == &oCli);
	Test(oPollEvs.GetLast().CanRead() == false); // even though we're polling for read as well, server wrote nothing
	Test(oPollEvs.GetLast().CanWrite() == true);
	Test(oPollEvs.GetLast().HasClosed() == false);
	Test(oPollEvs.GetLast().HasError() == false);
	
	Test(oPoll.Modify(oCli, false, false) == true); // check only for hang-up (close)
	Test(oPoll.Wait(100, oPollEvs) == true);
	
	Test(oPollEvs.Count() == 1);
	Test(&oPollEvs.GetLast().GetSocket() == &oCli);
	// don't test for read or write
	Test(oPollEvs.GetLast().HasClosed() == true);
	Test(oPollEvs.GetLast().HasError() == false);
	
	oPoll.Remove(oCli);
	oCli.Close();

	
	
	// Single client, server drop through, server writes, poll for rdwr, poll catch
	
	oSrv.SetCliSockWrite(true); // server write
	oSrv.SetCliSockSbcMsec(0); // drop through
	
	Test(oCli.Connect() == true);
	Test(oPoll.Add(oCli, true, true) == true);
	
	Test(oPoll.Wait(100, oPollEvs) == true);
	
	Test(oPollEvs.Count() == 1);
	Test(&oPollEvs.GetLast().GetSocket() == &oCli);
	Test(oPollEvs.GetLast().CanRead() == false); // server write doesn't arrive quick enough
	Test(oPollEvs.GetLast().CanWrite() == true);
	Test(oPollEvs.GetLast().HasClosed() == false);
	Test(oPollEvs.GetLast().HasError() == false);
	
	Test(oPoll.Modify(oCli, false, false) == true); // check only for hang-up (close)
	Test(oPoll.Wait(100, oPollEvs) == true);
	
	Test(oPollEvs.Count() == 1);
	Test(&oPollEvs.GetLast().GetSocket() == &oCli);
	// don't test for read or write
	Test(oPollEvs.GetLast().HasClosed() == true);
	Test(oPollEvs.GetLast().HasError() == false);
	
	Test(oPoll.Modify(oCli, true, false) == true); // check only for read
	Test(oPoll.Wait(100, oPollEvs) == true);
	
	Test(oPollEvs.Count() == 1);
	Test(&oPollEvs.GetLast().GetSocket() == &oCli);
	Test(oPollEvs.GetLast().CanRead() == true); // server write arrives, read ready
	// don't test for write or closed
	Test(oPollEvs.GetLast().HasError() == false);
	
	// NOTE: Can't test this because of Glibc/MSCRT differences
// 	Test(oPoll.Wait(100, oPollEvs) == true);
// 	
// 	Test(oPollEvs.Count() == 1);
// 	Test(&oPollEvs.GetLast().GetSocket() == &oCli);
// 	Test(oPollEvs.GetLast().CanRead() == true); // server write still pending, but because of level-triggered poll (yes, even on MSCRT with the beautiful hack) read is still ready
// 	// don't test for write or closed
// 	Test(oPollEvs.GetLast().HasError() == false);
// 	
// 	Test(oPoll.Wait(100, oPollEvs) == true);
// 	
// 	Test(oPollEvs.Count() == 1);
// 	Test(&oPollEvs.GetLast().GetSocket() == &oCli);
// 	Test(oPollEvs.GetLast().CanRead() == true); // server write still pending, just for the lulz of it
// 	// don't test for write or closed
// 	Test(oPollEvs.GetLast().HasError() == false);
	
	Test(oCli.Read(oBfr, sizeof(oBfr), oBytesRead) == true); // read is ready
	Test(oBytesRead == 8);
	oBfr[oBytesRead] = '\0';
	Test(_local_CmpEchoBuffer(oBfr, "srvwrite") == true);
	
	oPoll.Remove(oCli);
	oCli.Close();
	
	
	// Stop
	
	oCli.Close();
	oSrv.Stop();
	oPoll.Close();
	
	
	return 0;
}

static int TestPollWaitAccept()
{
	PrintFn();
	
	
	acpl::Ipv4Addr oAddr;
	acpl::NetworkPort oPort;
	
	acpl::SocketTcp oSrv;
	acpl::SocketTcp oCli;
	_local_ClientTcpConnector oConnr(1);
	
	acpl::Poll oPoll;
	acpl::Poll::EventList oPollEvs;
	
	
	// Setup
	
	oAddr.SetLoopback();
	oPort.Set(_local_TestPortMain);
	
	Test(oSrv.Setup(oAddr, oPort) == true);
	Test(oConnr.Setup(oAddr, oPort) == true);
	oConnr.SetSbcMsec(150);
	
	
	// Start
	
	Test(oSrv.Listen(50) == true);
	Test(oPoll.Open() == true);
	Test(oPoll.Add(oSrv, false, false) == true);
	Test(oConnr.Start() == true);
	
	
	// 
	
	Test(oPoll.Wait(100, oPollEvs) == true);
	Test(oPollEvs.Count() == 0); // didn't poll for read/accept, no result
	
	Test(oPoll.Modify(oSrv, true, false) == true);
	Test(oPoll.Wait(100, oPollEvs) == true);
	
	Test(oPollEvs.Count() == 1);
	Test(&oPollEvs.GetLast().GetSocket() == &oSrv);
	Test(oPollEvs.GetLast().CanRead() == true); // ready to accept registers as CanRead
	Test(oPollEvs.GetLast().CanWrite() == false);
	Test(oPollEvs.GetLast().HasClosed() == false);
	Test(oPollEvs.GetLast().HasError() == false);
	
	// NOTE: Unfortunately, if polling the oSrv again, can accept will not signal on MSCRT.
	
	Test(oSrv.Accept(oCli) == true);
	Test(oPoll.Add(oCli, true, true) == true); // poll client socket
	Test(oPoll.Wait(100, oPollEvs) == true);
	
	Test(oPollEvs.Count() == 1);
	Test(&oPollEvs.GetLast().GetSocket() == &oCli);
	Test(oPollEvs.GetLast().CanRead() == false);
	Test(oPollEvs.GetLast().CanWrite() == true);
	Test(oPollEvs.GetLast().HasClosed() == false);
	Test(oPollEvs.GetLast().HasError() == false);
	
	Test(oPoll.Modify(oCli, false, false) == true); // poll client socket only for close
	Test(oPoll.Wait(150, oPollEvs) == true);
	
	Test(oPollEvs.Count() == 1);
	Test(&oPollEvs.GetLast().GetSocket() == &oCli);
	// don't test for read or write
	Test(oPollEvs.GetLast().HasClosed() == true);
	Test(oPollEvs.GetLast().HasError() == false);
	
	oCli.Close();
	
	
	// Stop
	
	oConnr.Stop(); // this has to be present because Main() function does not end fast enough
	oPoll.Close();
	oSrv.Close();
	
	
	return 0;
}


SectionFuncMain(netpoll)
{
	if (argc < 1)
	{
		Test(TestPollTypes() == 0);
		Test(TestPollBasic() == 0);
		Test(TestPollAddRem() == 0);
		Test(TestPollWaitSingle() == 0);
		acpl::Time::SleepMsec(50); // MSCRT does not properly synchronously close sockets; even tried with the DisconnectEx(). TODO: Needs more study.
		Test(TestPollWaitAccept() == 0);
		
		PrintOut("All Passed!\n");
	}
	else
		return PrintUnrecParams(acplDbgSrcLoc, lSectionName, argc, argv);
	
	return 0;
}

SectionFuncParamsBegin(netpoll)
// SectionFuncParamsAdd("param [param [param ...]]", "description")
SectionFuncParamsEnd(netpoll)
