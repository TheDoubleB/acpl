#ifndef ACPL_CP_GLIBC_NETWORK_INC_CPP
#define ACPL_CP_GLIBC_NETWORK_INC_CPP

#include "../Network.inc.cpp"

#include "../../net/Poll.h"
#include "../../Bits.h"

#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <errno.h>

namespace acpl
{
	
	namespace cp
	{
		// 
		// Resolver functions
		// 
		
		static inline acpl::DomainName::ResolveStatus _local_net_addrinfo_resolve(const acpl::String &nDomainName, struct addrinfo *&nAddrInfos)
		{
			switch (getaddrinfo(nDomainName.Utf8(), NULL, NULL, &nAddrInfos))
			{
				case 0:
					return acpl::DomainName::rsSuccess;
				
				case EAI_NODATA: // domain name exists, but no addresses are defined
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
			return ((nHandle.uInt = socket(reinterpret_cast<const struct sockaddr *>(nAddrData)->sa_family, nType, nProtoId)) != -1);
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
			nHandle.uInt = -1;
		}
		
		static inline bool _net_socket_is_open(const acpl::Socket::Handle &nHandle)
		{
			return (nHandle.uInt != -1);
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
			shutdown(nHandle.uInt, SHUT_RDWR);
		}
		
		static inline void _net_socket_close(const acpl::Socket::Handle &nHandle)
		{
			close(nHandle.uInt);
		}
		
		static inline bool _net_socket_set_blocking(const acpl::Socket::Handle &nHandle, bool nBlocking)
		{
			int oVal = fcntl(nHandle.uInt, F_GETFL);
			if (oVal != -1)
			{
				acpl::Bits::Set(oVal, O_NONBLOCK, !nBlocking);
				return (fcntl(nHandle.uInt, F_SETFL, oVal) == 0);
			}
			else
				return false;
		}
		
		static inline bool _net_socket_set_reuse_addr(const acpl::Socket::Handle &nHandle)
		{
			int oOptVal = 1;
			return (setsockopt(nHandle.uInt, SOL_SOCKET, SO_REUSEADDR, &oOptVal, sizeof(int)) == 0);
		}
		
		static inline bool _net_socket_set_excl_addr_use(const acpl::Socket::Handle &)
		{
			// SO_EXCLUSIVEADDRUSE is only used in MSCRT
			return true;
		}
		
		static inline bool _net_socket_set_hdr_incl(const acpl::Socket::Handle &nHandle, bool nHdrIncl)
		{
			int oOptVal = ((nHdrIncl == true) ? 1 : 0);
			return (setsockopt(nHandle.uInt, IPPROTO_IP, IP_HDRINCL, &oOptVal, sizeof(int)) == 0);
		}
		
		static inline bool _net_socket_bind(const acpl::Socket::Handle &nHandle, const acpl::Socket::Address::Data &nAddrData)
		{
			return (bind(nHandle.uInt, reinterpret_cast<const struct sockaddr *>(nAddrData), sizeof(nAddrData)) == 0);
		}
		
		static inline bool _net_socket_listen(const acpl::Socket::Handle &nHandle, acpl::UInt32 nMaxQueueLen)
		{
			return (listen(nHandle.uInt, (nMaxQueueLen & 0x7FFFFFFF)) == 0);
		}
		
		static inline bool _net_socket_accept(const acpl::Socket::Handle &nHandle, acpl::Socket::Handle &nRemoteHandle, acpl::Socket::Address::Data &nRemoteAddrData)
		{
			socklen_t oRemoteDataSize = sizeof(nRemoteAddrData);
			nRemoteHandle.uInt = accept(nHandle.uInt, reinterpret_cast<struct sockaddr *>(nRemoteAddrData), &oRemoteDataSize);
			return (acpl::cp::_net_socket_is_open(nRemoteHandle) == true);
		}
		
		static inline bool _net_socket_connect(const acpl::Socket::Handle &nHandle, const acpl::Socket::Address::Data &nAddrData)
		{
			return (connect(nHandle.uInt, reinterpret_cast<const struct sockaddr *>(nAddrData), sizeof(nAddrData)) == 0);
		}
		
		static inline bool _net_socket_write(const acpl::Socket::Handle &nHandle, const acpl::Socket::Address::Data &nAddrData, const void *nData, acpl::SizeT nDataSize, acpl::SizeT &nBytesWritten)
		{
			nBytesWritten = static_cast<acpl::SizeT>(sendto(
				nHandle.uInt,
				nData,
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
				nHandle.uInt,
				nData,
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
			socklen_t oRemoteDataSize = sizeof(nRemoteAddrData);
			nBytesRead = static_cast<acpl::SizeT>(recvfrom(
				nHandle.uInt,
				nData,
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
			switch (errno)
			{
				case 0:
					return acpl::Socket::leNone;
				
				case EAGAIN: // equivalent to EWOULDBLOCK
					if (nGlec == acpl::pi::_net_socket_glec_connect)
						break; // acpl::Socket::leSysErr
					else
						return acpl::Socket::leWouldBlk;
				
				case EPERM:
				case EACCES:
					return acpl::Socket::leNoPerm;
				
				case EINVAL: // this error is mostly triggered by invalid parameters, which in most cases are hard coded in ACPL anyway and should/must not trigger this error
					if (nGlec == acpl::pi::_net_socket_glec_open)
						return acpl::Socket::leInvalProto;
					else
					if (nGlec == acpl::pi::_net_socket_glec_bind)
						return acpl::Socket::leIsBound;
					else
						break; // acpl::Socket::leSysErr
				
				case EBADF:
				case ENOTSOCK:
					return acpl::Socket::leNotOpened;
				
				case EPROTONOSUPPORT:
					return acpl::Socket::leInvalProto;
				
				case EADDRINUSE:
					return acpl::Socket::leAddrInUse;
				
				case EISCONN:
					return acpl::Socket::leIsConn;
				
				case ENOTCONN:
					return acpl::Socket::leNotConn;
				
				case ECONNREFUSED:
					return acpl::Socket::leConnRefused;
				
				case ECONNABORTED:
					return acpl::Socket::leConnAbort;
				
				case ECONNRESET:
					return acpl::Socket::leConnReset;
				
				case ETIMEDOUT:
					return acpl::Socket::leConnTimeout;
				
				case EALREADY:
				case EINPROGRESS:
					return acpl::Socket::leConnInProg;
				
				case ENETUNREACH:
					return acpl::Socket::leNetUnreach;
				
				default:
					break; // acpl::Socket::leSysErr
			}
			
			return acpl::Socket::leSysErr;
		}
		
		
		// 
		// Poll functions
		// 
		
		static inline bool _local_net_poll_set_cloexec(const acpl::Poll::Handle &nHandle, bool nCloExec)
		{
			int oVal = fcntl(nHandle.uInt, F_GETFD);
			if (oVal != -1)
			{
				acpl::Bits::Set(oVal, FD_CLOEXEC, !nCloExec);
				return (fcntl(nHandle.uInt, F_SETFD, oVal) == 0);
			}
			else
				return false;
		}
		
		static inline bool _local_net_poll_ctl(int oOperation, const acpl::Poll::Handle &nPollHandle, const acpl::Socket::Handle &nSockHandle, bool nWatchRead, bool nWatchWrite, acpl::Socket *nSocketPtr)
		{
			epoll_event oEpe;
			oEpe.events = ((nWatchRead == true) ? (EPOLLIN | EPOLLPRI) : 0) | ((nWatchWrite == true) ? (EPOLLOUT | 0) : 0) | (EPOLLERR | EPOLLHUP | EPOLLRDHUP); // <<< NOTE: `(EPOLLOUT | 0)` is to make gcc stop whining
			oEpe.data.ptr = nSocketPtr;
			
			return (epoll_ctl(nPollHandle.uInt, oOperation, nSockHandle.uInt, &oEpe) == 0);
		}
		
		static inline void _net_poll_init(acpl::Poll::Handle &nHandle)
		{
			nHandle.uInt = -1;
		}
		
		static inline bool _net_poll_is_open(const acpl::Poll::Handle &nHandle)
		{
			return (nHandle.uInt != -1);
		}
		
		static inline bool _net_poll_open(acpl::Poll::Handle &nHandle)
		{
			nHandle.uInt = epoll_create(128);
			if (acpl::cp::_net_poll_is_open(nHandle) == true)
			{
				if (acpl::cp::_local_net_poll_set_cloexec(nHandle, true) == true)
					return true;
				else
				{
					close(nHandle.uInt);
					nHandle.uInt = -1;
					return false;
				}
			}
			else
				return false;
		}
		
		static inline void _net_poll_close(const acpl::Poll::Handle &nHandle)
		{
			close(nHandle.uInt);
		}
		
		static inline bool _net_poll_add(const acpl::Poll::Handle &nPollHandle, const acpl::Socket::Handle &nSockHandle, bool nWatchRead, bool nWatchWrite, acpl::Socket &nSocket)
		{
			return acpl::cp::_local_net_poll_ctl(EPOLL_CTL_ADD, nPollHandle, nSockHandle, nWatchRead, nWatchWrite, &nSocket);
		}
		
		static inline bool _net_poll_mod(const acpl::Poll::Handle &nPollHandle, const acpl::Socket::Handle &nSockHandle, bool nWatchRead, bool nWatchWrite, acpl::Socket &nSocket)
		{
			return acpl::cp::_local_net_poll_ctl(EPOLL_CTL_MOD, nPollHandle, nSockHandle, nWatchRead, nWatchWrite, &nSocket);
		}
		
		static inline void _net_poll_rem(const acpl::Poll::Handle &nPollHandle, const acpl::Socket::Handle &nSockHandle)
		{
			acpl::cp::_local_net_poll_ctl(EPOLL_CTL_DEL, nPollHandle, nSockHandle, false, false, NULL);
		}
		
		static inline bool _net_poll_wait(
			const acpl::Poll::Handle &nHandle,
			acpl::Poll::ItemList &nItems,
			acpl::Poll::Event::Data &nEventData,
			acpl::UInt32 nMsecs,
			acpl::Poll::EventList &nEvents)
		{
			if (nEventData.SetLength(nItems.Count() * sizeof(epoll_event)) != true)
				return false;
			
			epoll_event *oEvts = reinterpret_cast<epoll_event *>(nEventData.Data()); 
			int oRet = epoll_wait(nHandle.uInt, oEvts, nItems.Count(), nMsecs);
			
			for (int i = oRet; i-- > 0; )
			{
				acpl::Socket *oSocketPtr = static_cast<acpl::Socket *>(oEvts[i].data.ptr);
				uint32_t oEv = oEvts[i].events;
				
				nEvents.Add<acpl::Socket &>( // NOTE: The first template parameter has to be explicitly specified because some weird stuff happens otherwise
						*oSocketPtr,
						((oEv & (EPOLLIN | EPOLLPRI)) != 0), // read ready
						((oEv & (EPOLLOUT)) != 0), // write ready
						((oEv & (EPOLLHUP | EPOLLRDHUP)) != 0), // hang-up (close)
						((oEv & (EPOLLERR)) != 0) // error
					);
			}
			
			return (oRet >= 0 || errno == EINTR);
		}
		
		static inline acpl::Poll::LastError _net_poll_get_last_error(acpl::pi::_net_poll_get_last_error_context_t)
		{
			switch (errno)
			{
				case 0:
					return acpl::Poll::leNone;
				
				case EINVAL: // this error talks about either empty queue, which is handled in the frontend, or invalid epoll fd, which should/must not happen, even if the fd is -1 (in which case EBADF is triggered)
					break; // acpl::Poll::leSysErr
				
				case EBADF:
					return acpl::Poll::leNotOpened;
				
				case ENOENT:
					return acpl::Poll::leNotAdded;
				
				default:
					break; // acpl::Poll::leSysErr
			}
			
			return acpl::Poll::leSysErr;
		}
	
	}
}

#endif // ACPL_CP_GLIBC_NETWORK_INC_CPP
