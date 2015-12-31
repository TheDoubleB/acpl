#ifndef ACPL_CP_MSCRT_NETWORK_INC_CPP
#define ACPL_CP_MSCRT_NETWORK_INC_CPP

#include <winsock2.h>
#include <ws2tcpip.h>

#include "../Network.inc.cpp"
#include "../../net/Poll.h"

namespace acpl
{
	
	namespace cp
	{
#if defined(_acplNetworkInit)
		// 
		// WSA singleton
		// 
		
		static class _local_net_wsa_t
		{
			public:
				_local_net_wsa_t() { WSAData wsaData = {}; WSAStartup(MAKEWORD(2, 2), &wsaData); }
				~_local_net_wsa_t() { WSACleanup(); }
		} lWsaSingleton;
#endif
		
		
		// 
		// acpl::Socket "hack" class
		// 
		
		class _local_net_socket_hack_t : public acpl::Socket
		{
			public:
				inline const acpl::Socket::Handle &GetHandle() const { return acpl::Socket::GetHandle(); }
		};
		
		
		// 
		// Resolver functions
		// 
		
		static inline acpl::DomainName::ResolveStatus _local_net_addrinfo_resolve(const acpl::String &nDomainName, struct addrinfo *&nAddrInfos)
		{
			switch (getaddrinfo(nDomainName.Utf8(), NULL, NULL, &nAddrInfos))
			{
				case 0:
					return acpl::DomainName::rsSuccess;
				
				//   EAI_NODATA: // (equivalent to EIA_NONAME) domain name exists, but no addresses are defined
				case EAI_NONAME: // domain name does not exist
					return acpl::DomainName::rsNotFound;
				
				case EAI_AGAIN: // DNS temporary failure; try again later
					return acpl::DomainName::rsDnsTempFail;
				
				case EAI_FAIL: // DNS permanent failure
					return acpl::DomainName::rsDnsPermFail;
				
				default:
					return acpl::DomainName::rsSystemError;
			}
		}
		
		static inline acpl::DomainName::ResolveStatus _net_address_resolve(const acpl::String &nDomainName, acpl::DomainName::Ipv4AddrList *nAddrList1, acpl::DomainName::Ipv6AddrList *nAddrList2)
		{
			struct addrinfo *oAddrInfos = NULL;
			acpl::DomainName::ResolveStatus oRet = acpl::cp::_local_net_addrinfo_resolve(nDomainName, oAddrInfos);
			
			if (oRet == acpl::DomainName::rsSuccess)
			{
				for (struct addrinfo *oCurInfo = oAddrInfos; oCurInfo != NULL; oCurInfo = oCurInfo->ai_next)
					if (oCurInfo->ai_addr != NULL)
					{
						if (oCurInfo->ai_family == AF_INET  && nAddrList1 != NULL)
							acpl::pi::_local_net_ipvxaddr_add(reinterpret_cast<acpl::UInt8 *>(&reinterpret_cast<sockaddr_in *>(oCurInfo->ai_addr)->sin_addr.s_addr), *nAddrList1);
						else
						if (oCurInfo->ai_family == AF_INET6 && nAddrList2 != NULL)
							acpl::pi::_local_net_ipvxaddr_add(reinterpret_cast<sockaddr_in6 *>(oCurInfo->ai_addr)->sin6_addr.s6_addr, *nAddrList2);
					}
			}
			
			if (oAddrInfos != NULL)
				freeaddrinfo(oAddrInfos);
			
			return oRet;
		}
		
		
		// 
		// Socket functions
		// 
		
		static inline void _local_net_sockaddr_fill(struct sockaddr_in &nSockAddr, const acpl::Ipv4Addr &nAddr, const acpl::NetworkPort &nPort)
		{
			nSockAddr.sin_family = AF_INET;
			acpl::Mem::BaseCopy(&nSockAddr.sin_port, nPort.Data(), sizeof(nSockAddr.sin_port));
			acpl::Mem::BaseCopy(&nSockAddr.sin_addr.s_addr, nAddr.Data(), sizeof(nSockAddr.sin_addr.s_addr));
		}
		
		static inline void _local_net_sockaddr_fill(struct sockaddr_in6 &nSockAddr, const acpl::Ipv6Addr &nAddr, const acpl::NetworkPort &nPort)
		{
			nSockAddr.sin6_family = AF_INET6;
			acpl::Mem::BaseCopy(&nSockAddr.sin6_port, nPort.Data(), sizeof(nSockAddr.sin6_port));
			acpl::Mem::BaseCopy(&nSockAddr.sin6_addr.s6_addr, nAddr.Data(), sizeof(nSockAddr.sin6_addr.s6_addr));
		}
		
		static inline bool _local_net_socket_open(acpl::Socket::Handle &nHandle, const acpl::Socket::Address::Data &nAddrData, int nType, int nProtoId)
		{
			return ((nHandle.uPsui = socket(reinterpret_cast<const struct sockaddr *>(nAddrData)->sa_family, nType, nProtoId)) != INVALID_SOCKET);
		}
		
		static inline void _net_socket_address_init(acpl::Socket::Address::Data &nAddrData)
		{
			acpl::Mem::Set(nAddrData, 0, sizeof(nAddrData));
		}
		
		static inline bool _net_socket_address_is_setup(const acpl::Socket::Address::Data &nAddrData)
		{
			return (reinterpret_cast<const struct sockaddr *>(nAddrData)->sa_family != AF_UNSPEC);
		}
		
		static inline void _net_socket_address_fill(acpl::Socket::Address::Data &nAddrData, const acpl::Ipv4Addr &nAddr, const acpl::NetworkPort &nPort)
		{
			acpl::cp::_local_net_sockaddr_fill(*reinterpret_cast<struct sockaddr_in *>(nAddrData), nAddr, nPort);
		}
		
		static inline void _net_socket_address_fill(acpl::Socket::Address::Data &nAddrData, const acpl::Ipv6Addr &nAddr, const acpl::NetworkPort &nPort)
		{
			acpl::cp::_local_net_sockaddr_fill(*reinterpret_cast<struct sockaddr_in6 *>(nAddrData), nAddr, nPort);
		}
		
		static inline bool _net_socket_address_extract(const acpl::Socket::Address::Data &nAddrData, acpl::Ipv4Addr &nAddr, acpl::NetworkPort &nPort)
		{
			const struct sockaddr_in *oAddrData = reinterpret_cast<const struct sockaddr_in *>(nAddrData);
			if (oAddrData->sin_family == AF_INET)
			{
				acpl::Mem::BaseCopy(nPort.Data(), &oAddrData->sin_port, sizeof(oAddrData->sin_port));
				acpl::Mem::BaseCopy(nAddr.Data(), &oAddrData->sin_addr.s_addr, sizeof(oAddrData->sin_addr.s_addr));
				return true;
			}
			else
				return false;
		}
		
		static inline bool _net_socket_address_extract(const acpl::Socket::Address::Data &nAddrData, acpl::Ipv6Addr &nAddr, acpl::NetworkPort &nPort)
		{
			const struct sockaddr_in6 *oAddrData = reinterpret_cast<const struct sockaddr_in6 *>(nAddrData);
			if (oAddrData->sin6_family == AF_INET6)
			{
				acpl::Mem::BaseCopy(nPort.Data(), &oAddrData->sin6_port, sizeof(oAddrData->sin6_port));
				acpl::Mem::BaseCopy(nAddr.Data(), &oAddrData->sin6_addr.s6_addr, sizeof(oAddrData->sin6_addr.s6_addr));
				return true;
			}
			else
				return false;
		}
		
		static inline void _net_socket_init(acpl::Socket::Handle &nHandle)
		{
			nHandle.uPsui = INVALID_SOCKET;
		}

		static inline bool _net_socket_is_open(const acpl::Socket::Handle &nHandle)
		{
			return (nHandle.uPsui != INVALID_SOCKET);
		}
		
		static inline bool _net_socket_open(acpl::Socket::Handle &nHandle, const acpl::Socket::Address::Data &nAddrData, acpl::UInt8 nProtoId, const acpl::SocketTcp &)
		{
			return acpl::cp::_local_net_socket_open(nHandle, nAddrData, SOCK_STREAM, nProtoId);
		}
		
		static inline bool _net_socket_open(acpl::Socket::Handle &nHandle, const acpl::Socket::Address::Data &nAddrData, acpl::UInt8 nProtoId, const acpl::SocketUdp &)
		{
			return acpl::cp::_local_net_socket_open(nHandle, nAddrData, SOCK_DGRAM, nProtoId);
		}
		
		static inline bool _net_socket_open(acpl::Socket::Handle &nHandle, const acpl::Socket::Address::Data &nAddrData, acpl::UInt8 nProtoId, const acpl::SocketRaw &)
		{
			return acpl::cp::_local_net_socket_open(nHandle, nAddrData, SOCK_RAW, nProtoId);
		}
		
		static inline void _net_socket_shutdown(const acpl::Socket::Handle &nHandle)
		{
			shutdown(nHandle.uPsui, SD_BOTH);
		}
		
		static inline void _net_socket_close(const acpl::Socket::Handle &nHandle)
		{
	// 		/*GUID oDxGuid = WSAID_DISCONNECTEX;
	// 		LPFN_DISCONNECTEX oDisconnectEx;
	// 		DWORD oCopied;
	// 		if (WSAIoctl(nHandle.uPsui, SIO_GET_EXTENSION_FUNCTION_POINTER, &oDxGuid, sizeof(oDxGuid), &oDisconnectEx, sizeof(oDisconnectEx), &oCopied, 0, 0) == 0)
	// 			oDisconnectEx(nHandle.uPsui, NULL, 0, 0);*/
			
			closesocket(nHandle.uPsui);
		}
		
		static inline bool _net_socket_set_blocking(const acpl::Socket::Handle &nHandle, bool nBlocking)
		{
			u_long oVal = ((nBlocking == false) ? 1 : 0);
			return (ioctlsocket(nHandle.uPsui, FIONBIO, &oVal) == 0);
		}
		
		static inline bool _net_socket_set_reuse_addr(const acpl::Socket::Handle &)
		{
			// SO_REUSEADDR in MSCRT means something completely different
			return true;
		}
		
		static inline bool _net_socket_set_excl_addr_use(const acpl::Socket::Handle &nHandle)
		{
			int oOptVal = 1;
			return (setsockopt(nHandle.uPsui, SOL_SOCKET, SO_EXCLUSIVEADDRUSE, reinterpret_cast<char *>(&oOptVal), sizeof(int)) == 0);
		}
		
		static inline bool _net_socket_set_hdr_incl(const acpl::Socket::Handle &nHandle, bool nHdrIncl)
		{
			int oOptVal = ((nHdrIncl == true) ? 1 : 0);
			return (setsockopt(nHandle.uPsui, IPPROTO_IP, IP_HDRINCL, reinterpret_cast<char *>(&oOptVal), sizeof(int)) == 0);
		}
		
		static inline bool _net_socket_bind(const acpl::Socket::Handle &nHandle, const acpl::Socket::Address::Data &nAddrData)
		{
			return (bind(nHandle.uPsui, reinterpret_cast<const struct sockaddr *>(nAddrData), sizeof(nAddrData)) == 0);
		}
		
		static inline bool _net_socket_listen(const acpl::Socket::Handle &nHandle, acpl::UInt32 nMaxQueueLen)
		{
			return (listen(nHandle.uPsui, (nMaxQueueLen & 0x7FFFFFFF)) == 0);
		}
		
		static inline bool _net_socket_accept(const acpl::Socket::Handle &nHandle, acpl::Socket::Handle &nRemoteHandle, acpl::Socket::Address::Data &nRemoteAddrData)
		{
			int oRemoteDataSize = sizeof(nRemoteAddrData);
			nRemoteHandle.uPsui = accept(nHandle.uPsui, reinterpret_cast<struct sockaddr *>(nRemoteAddrData), &oRemoteDataSize);
			return (acpl::cp::_net_socket_is_open(nRemoteHandle) == true);
		}
		
		static inline bool _net_socket_connect(const acpl::Socket::Handle &nHandle, const acpl::Socket::Address::Data &nAddrData)
		{
			return (connect(nHandle.uPsui, reinterpret_cast<const struct sockaddr *>(nAddrData), sizeof(nAddrData)) == 0);
		}
		
		static inline bool _net_socket_write(const acpl::Socket::Handle &nHandle, const acpl::Socket::Address::Data &nAddrData, const void *nData, acpl::SizeT nDataSize, acpl::SizeT &nBytesWritten)
		{
			nBytesWritten = static_cast<acpl::SizeT>(sendto(
				nHandle.uPsui,
				static_cast<const char *>(nData),
				nDataSize,
				0,
				reinterpret_cast<const struct sockaddr *>(nAddrData),
				sizeof(nAddrData)));
			
			if (static_cast<acpl::SSizeT>(nBytesWritten) >= 0)
				return true;
			else
			{
				nBytesWritten = 0;
				return false;
			}
		}
		
		static inline bool _net_socket_read(const acpl::Socket::Handle &nHandle, void *nData, acpl::SizeT nDataSize, acpl::SizeT &nBytesRead)
		{
			nBytesRead = static_cast<acpl::SizeT>(recv(
				nHandle.uPsui,
				static_cast<char *>(nData),
				nDataSize,
				0));
			
			if (static_cast<acpl::SSizeT>(nBytesRead) >= 0)
				return true;
			else
			{
				nBytesRead = 0;
				return false;
			}
		}
		
		static inline bool _net_socket_read(const acpl::Socket::Handle &nHandle, void *nData, acpl::SizeT nDataSize, acpl::SizeT &nBytesRead, acpl::Socket::Address::Data &nRemoteAddrData)
		{
			int oRemoteDataSize = sizeof(nRemoteAddrData);
			nBytesRead = static_cast<acpl::SizeT>(recvfrom(
				nHandle.uPsui,
				static_cast<char *>(nData),
				nDataSize,
				0,
				reinterpret_cast<struct sockaddr *>(nRemoteAddrData),
				&oRemoteDataSize));
			
			if (static_cast<acpl::SSizeT>(nBytesRead) >= 0)
				return true;
			else
			{
				nBytesRead = 0;
				return false;
			}
		}
		
		static inline acpl::Socket::LastError _net_socket_get_last_error(acpl::pi::_net_socket_get_last_error_context_t nGlec)
		{
			switch (WSAGetLastError())
			{
				case WSAEWOULDBLOCK:
					return acpl::Socket::leWouldBlk;
				
				case WSAEACCES:
					return acpl::Socket::leNoPerm;
				
				case WSAEINVAL: // this error is mostly triggered by invalid parameters, which in most cases are hard coded in ACPL anyway and should/must not trigger this error
					if (nGlec == acpl::pi::_net_socket_glec_open)
						return acpl::Socket::leInvalProto;
					else
					if (nGlec == acpl::pi::_net_socket_glec_bind)
						return acpl::Socket::leIsBound;
					else
						break; // acpl::Socket::leSysErr
				
				case WSAEBADF:
				case WSAENOTSOCK:
					return acpl::Socket::leNotOpened;
				
				case WSAEPROTONOSUPPORT:
					return acpl::Socket::leInvalProto;
				
				case WSAEADDRINUSE:
					return acpl::Socket::leAddrInUse;
				
				case WSAEISCONN:
					return acpl::Socket::leIsConn;
				
				case WSAENOTCONN:
					return acpl::Socket::leNotConn;
				
				case WSAECONNREFUSED:
					return acpl::Socket::leConnRefused;
				
				case WSAECONNABORTED:
					return acpl::Socket::leConnAbort;
				
				case WSAECONNRESET:
					return acpl::Socket::leConnReset;
				
				case WSAETIMEDOUT:
					return acpl::Socket::leConnTimeout;
				
				case WSAEALREADY:
				case WSAEINPROGRESS:
					return acpl::Socket::leConnInProg;
				
				case WSAENETUNREACH:
					return acpl::Socket::leNetUnreach;
				
				default:
					break; // acpl::Socket::leSysErr
			}
			
			return acpl::Socket::leSysErr;
		}
		
		
		// 
		// Poll functions
		// 
		
		static inline bool _local_net_poll_ctl(const acpl::Poll::Handle &nPollHandle, const acpl::Socket::Handle &nSockHandle, bool nWatchAny, bool nWatchRead, bool nWatchWrite)
		{
			long oEvents = ((nWatchAny == true) ? (FD_CLOSE) : 0) | ((nWatchRead == true) ? (FD_READ | FD_OOB | FD_ACCEPT) : 0) | ((nWatchWrite == true) ? (FD_WRITE | FD_CONNECT) : 0);
			return (WSAEventSelect(nSockHandle.uPsui, nPollHandle.uPtr, oEvents) == 0);
		}
		
		static inline void _net_poll_init(acpl::Poll::Handle &nHandle)
		{
			nHandle.uPtr = NULL;
		}
		
		static inline bool _net_poll_is_open(const acpl::Poll::Handle &nHandle)
		{
			return (nHandle.uPtr != NULL);
		}
		
		static inline bool _net_poll_open(acpl::Poll::Handle &nHandle)
		{
			return ((nHandle.uPtr = CreateEvent(NULL, FALSE, FALSE, NULL)) != NULL);
		}
		
		static inline void _net_poll_close(const acpl::Poll::Handle &nHandle)
		{
			CloseHandle(nHandle.uPtr);
		}
		
		static inline bool _net_poll_add(const acpl::Poll::Handle &nPollHandle, const acpl::Socket::Handle &nSockHandle, bool nWatchRead, bool nWatchWrite, acpl::Socket &)
		{
			return acpl::cp::_local_net_poll_ctl(nPollHandle, nSockHandle, true, nWatchRead, nWatchWrite);
		}
		
		static inline bool _net_poll_mod(const acpl::Poll::Handle &nPollHandle, const acpl::Socket::Handle &nSockHandle, bool nWatchRead, bool nWatchWrite, acpl::Socket &)
		{
			return acpl::cp::_local_net_poll_ctl(nPollHandle, nSockHandle, true, nWatchRead, nWatchWrite);
		}
		
		static inline void _net_poll_rem(const acpl::Poll::Handle &nPollHandle, const acpl::Socket::Handle &nSockHandle)
		{
			acpl::cp::_local_net_poll_ctl(nPollHandle, nSockHandle, false, false, false);
		}
		
		static inline bool _net_poll_wait(
			const acpl::Poll::Handle &nHandle,
			acpl::Poll::ItemList &nItems,
			acpl::Poll::Event::Data &,
			acpl::UInt32 nMsecs,
			acpl::Poll::EventList &nEvents)
		{
			if (WSAWaitForMultipleEvents(1, &nHandle.uPtr, TRUE, nMsecs, TRUE) == WSA_WAIT_EVENT_0)
			{
				for (acpl::SizeT i = nItems.Count(); i-- != 0; )
				{
					acpl::cp::_local_net_socket_hack_t *oSocketPtr = reinterpret_cast<acpl::cp::_local_net_socket_hack_t *>(&nItems.Get(i).GetSocket());
					WSANETWORKEVENTS oEvStruct;
					long &oEv = oEvStruct.lNetworkEvents;
					
					if (WSAEnumNetworkEvents(oSocketPtr->GetHandle().uPsui, NULL, &oEvStruct) == 0 && oEv != 0)
					{
						nEvents.Add<acpl::Socket &, bool, bool, bool, bool>( // NOTE: The template parameters have to be explicitly specified because some weird stuff happens otherwise
								*oSocketPtr,
								((oEv & (FD_READ | FD_OOB | FD_ACCEPT)) != 0), // read ready
								((oEv & (FD_WRITE | FD_CONNECT)) != 0), // write ready
								((oEv & (FD_CLOSE)) != 0), // hang-up (close)
								false // error (no flag for error available)
							);
						
						// This will have to be resolved in a different way.
	// 					// NOTE: These are hacks for simulating level-triggered polling by calling re-enabling functions without actually reading or writing anything
	// 					// NOTE: FD_ACCEPT unfortunately cannot be (at least not to my knowledge) re-enabled using the same tactics
	// 					if ((oEv & (FD_READ | FD_OOB)) != 0)
	// 						recv(oSocketPtr->GetHandle().uPsui, NULL, 0, 0);
	// 					if ((oEv & FD_WRITE) != 0)
	// 						send(oSocketPtr->GetHandle().uPsui, NULL, 0, 0);
					}
				}
			}
			
			return true; // there is no error case that would require false return value
		}
		
		static inline acpl::Poll::LastError _net_poll_get_last_error(acpl::pi::_net_poll_get_last_error_context_t /*nGlec*/)
		{
			switch (WSAGetLastError())
			{
				case WSAEINVAL: // all other parameters should/must be in order, including socket handle
				case WSAEBADF:
				case WSA_INVALID_HANDLE:
					return acpl::Poll::leNotOpened;
				
				default:
					break; // acpl::Poll::leSysErr
			}
			
			return acpl::Poll::leSysErr;
		}
	
	}
}

#endif // ACPL_CP_MSCRT_NETWORK_INC_CPP
