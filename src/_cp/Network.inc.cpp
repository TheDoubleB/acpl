#ifndef ACPL_CP_NETWORK_INC_CPP
#define ACPL_CP_NETWORK_INC_CPP


namespace acpl
{
	namespace pi
	{
		
		// 
		// Common local functions for resolver functions
		// 
		
		template <class tType>
		static inline acpl::SizeT _local_net_ipvxaddr_size(const tType &)
		{
			return sizeof(typename tType::Type);
		}
		
		template <class tType>
		static inline bool _local_net_ipvxaddr_is_equal(const acpl::UInt8 *nBytes, tType &nIpvxAddr)
		{
			for (acpl::SizeT i = acpl::pi::_local_net_ipvxaddr_size(nIpvxAddr); i-- != 0; )
				if (nBytes[i] != nIpvxAddr.Data()[i])
					return false;
			
			return true;
		}
		
		template <class tType>
		static inline void _local_net_ipvxaddr_add(const acpl::UInt8 *nBytes, tType &nIpvxAddrList)
		{
			for (acpl::SizeT i = nIpvxAddrList.Count(); i-- != 0; )
				if (acpl::pi::_local_net_ipvxaddr_is_equal(nBytes, nIpvxAddrList.Get(i)) == true)
					return;
			
			if (nIpvxAddrList.Add() == true)
				acpl::Mem::Copy(nIpvxAddrList.GetLast().Data(), nBytes, acpl::pi::_local_net_ipvxaddr_size(nIpvxAddrList.GetFirst()));
		}
		
		
		// 
		// Common types for network functions
		// 
		
		enum _net_socket_get_last_error_context_t
		{
			_net_socket_glec_open,				// _net_socket_open() context
			_net_socket_glec_set_reuse_addr,	// _net_socket_set_reuse_addr() context
			_net_socket_glec_set_excl_addr_use,	// _net_socket_set_excl_addr_use() context
			_net_socket_glec_bind,				// _net_socket_bind() context
			_net_socket_glec_listen,			// _net_socket_listen() context
			_net_socket_glec_set_blocking,		// _net_socket_set_blocking() context
			_net_socket_glec_accept,			// _net_socket_listen() context
			_net_socket_glec_connect,			// _net_socket_connect() context
			_net_socket_glec_set_hdr_incl,		// _net_socket_set_hdr_incl() context
			_net_socket_glec_write,				// _net_socket_write() context
			_net_socket_glec_read				// _net_socket_read() context
		};
		
		enum _net_poll_get_last_error_context_t
		{
			_net_poll_glec_open,	// _net_poll_open() context
			_net_poll_glec_add,		// _net_poll_add() context
			_net_poll_glec_mod,		// _net_poll_mod() context
			_net_poll_glec_rem,		// _net_poll_rem() context
			_net_poll_glec_wait		// _net_poll_wait() context
		};
		
	}
}


#if   (acplCRuntime == acplCRuntimeGlibc)
#	include "glibc/Network.inc.cpp"
#elif (acplCRuntime == acplCRuntimeMscrt)
#	include "mscrt/Network.inc.cpp"
#else
#	error Inclusion of platform-specific `Network.inc.cpp` required
#endif


#endif // ACPL_CP_NETWORK_INC_CPP
