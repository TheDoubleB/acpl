#include "Socket.h"

// Used for WSA init singleton class. Only defined here and nowhere else.
#define _acplNetworkInit

#include "../Memory.h"
#include "../_cp/Network.inc.cpp"


// 
// Socket
// 


// INTERNAL

template <class tSocketType>
static inline bool _local_Socket_Open(acpl::Socket::Handle &nHandle, const acpl::Socket::Address::Data &nAddrData, acpl::UInt8 nProtoId, const tSocketType &nArgSocket, const acpl::Socket &nThisSocket, acpl::Enum<acpl::Socket::LastError, acpl::UInt8> &nLastError)
{
	if (&nThisSocket != &nArgSocket)
		nLastError = acpl::Socket::leSysErr;
	else
	if (acpl::cp::_net_socket_address_is_setup(nAddrData) != true)
		nLastError = acpl::Socket::leNotSetup;
	else
	if (acpl::cp::_net_socket_is_open(nHandle) == true)
		nLastError = acpl::Socket::leIsOpened;
	else
	if (acpl::cp::_net_socket_open(nHandle, nAddrData, nProtoId, nArgSocket) == true)
		return true;
	else
		nLastError = acpl::cp::_net_socket_get_last_error(acpl::pi::_net_socket_glec_open);
	
	return false;
}


// PUBLIC

bool acpl::Socket::IsOpen() const
{
	return acpl::cp::_net_socket_is_open(this->mHandle);
}

void acpl::Socket::Close()
{
	if (acpl::cp::_net_socket_is_open(this->mHandle) == true)
	{
		acpl::cp::_net_socket_shutdown(this->mHandle);
		acpl::cp::_net_socket_close(this->mHandle);
		acpl::cp::_net_socket_init(this->mHandle);
	}
}


bool acpl::Socket::SetBlocking(bool nBlocking)
{
	if (acpl::cp::_net_socket_set_blocking(this->mHandle, nBlocking) == true)
		return true;
	else
		this->mLastError = acpl::cp::_net_socket_get_last_error(acpl::pi::_net_socket_glec_set_blocking);
	
	return false;
}


// PROTECTED

bool acpl::Socket::IOpen(const acpl::SocketTcp &nSocket)
{
	return _local_Socket_Open(this->mHandle, this->mAddress.GetData(), 0, nSocket, *this, this->mLastError);
}

bool acpl::Socket::IOpen(const acpl::SocketUdp &nSocket)
{
	return _local_Socket_Open(this->mHandle, this->mAddress.GetData(), 0, nSocket, *this, this->mLastError);
}

bool acpl::Socket::IOpen(const acpl::SocketRaw &nSocket, acpl::UInt8 nProtoId)
{
	return _local_Socket_Open(this->mHandle, this->mAddress.GetData(), nProtoId, nSocket, *this, this->mLastError);
}

bool acpl::Socket::IBind()
{
	if (acpl::cp::_net_socket_set_reuse_addr(this->mHandle) != true)
		this->mLastError = acpl::cp::_net_socket_get_last_error(acpl::pi::_net_socket_glec_set_reuse_addr);
	else
	if (acpl::cp::_net_socket_set_excl_addr_use(this->mHandle) != true)
		this->mLastError = acpl::cp::_net_socket_get_last_error(acpl::pi::_net_socket_glec_set_excl_addr_use);
	else
	if (acpl::cp::_net_socket_bind(this->mHandle, this->mAddress.GetData()) == true)
		return true;
	else
		this->mLastError = acpl::cp::_net_socket_get_last_error(acpl::pi::_net_socket_glec_bind);
	
	return false;
}

bool acpl::Socket::IListen(acpl::UInt32 nMaxQueueLen)
{
	if (acpl::cp::_net_socket_listen(this->mHandle, nMaxQueueLen) == true)
		return true;
	else
		this->mLastError = acpl::cp::_net_socket_get_last_error(acpl::pi::_net_socket_glec_listen);
	
	return false;
}

bool acpl::Socket::IAccept(acpl::Socket &nSocket) const
{
	nSocket.Close();
	acpl::cp::_net_socket_address_init(nSocket.mAddress.GetData());
	
	if (acpl::cp::_net_socket_accept(this->mHandle, nSocket.mHandle, nSocket.mAddress.GetData()) == true)
		return true;
	else
		this->mLastError = acpl::cp::_net_socket_get_last_error(acpl::pi::_net_socket_glec_accept);
	
	return false;
}

bool acpl::Socket::IConnect()
{
	if (acpl::cp::_net_socket_connect(this->mHandle, this->mAddress.GetData()) == true)
		return true;
	else
		this->mLastError = acpl::cp::_net_socket_get_last_error(acpl::pi::_net_socket_glec_connect);
	
	return false;
}


bool acpl::Socket::ISetHdrIncl(bool nHdrIncl)
{
	if (acpl::cp::_net_socket_set_hdr_incl(this->mHandle, nHdrIncl) == true)
		return true;
	else
		this->mLastError = acpl::cp::_net_socket_get_last_error(acpl::pi::_net_socket_glec_set_hdr_incl);
	
	return false;
}


bool acpl::Socket::IWrite(const void *nData, acpl::SizeT nDataSize, acpl::SizeT &nBytesWritten) const
{
	if (acpl::cp::_net_socket_write(this->mHandle, this->mAddress.GetData(), nData, nDataSize, nBytesWritten) == true)
		return true;
	else
		this->mLastError = acpl::cp::_net_socket_get_last_error(acpl::pi::_net_socket_glec_write);
	
	return false;
}

bool acpl::Socket::IRead(void *nData, acpl::SizeT nDataSize, acpl::SizeT &nBytesRead) const
{
	if (acpl::cp::_net_socket_read(this->mHandle, nData, nDataSize, nBytesRead) == true)
		return true;
	else
		this->mLastError = acpl::cp::_net_socket_get_last_error(acpl::pi::_net_socket_glec_read);
	
	return false;
}

bool acpl::Socket::IRead(void *nData, acpl::SizeT nDataSize, acpl::SizeT &nBytesRead, acpl::Socket::Address &nAddress) const
{
	if (acpl::cp::_net_socket_read(this->mHandle, nData, nDataSize, nBytesRead, nAddress.GetData()) == true)
		return true;
	else
		this->mLastError = acpl::cp::_net_socket_get_last_error(acpl::pi::_net_socket_glec_read);
	
	return false;
}


// PRIVATE

void acpl::Socket::Init()
{
	acpl::cp::_net_socket_init(this->mHandle);
}



// 
// Socket::Address
// 

// INTERNAL

template <class tAddrType>
static inline bool _local_Socket_Address_Setup(acpl::Socket::Address::Data &nAddrData, const acpl::Socket::Address &nAddress, const tAddrType &nAddr, const acpl::NetworkPort &nPort)
{
	if (nAddress.IsSetup() == false)
	{
		acpl::cp::_net_socket_address_init(nAddrData);
		acpl::cp::_net_socket_address_fill(nAddrData, nAddr, nPort);
		return true;
	}
	else
		return false;
}

template <class tAddrType>
static inline bool _local_Socket_Address_GetSetup(const acpl::Socket::Address::Data &nAddrData, tAddrType &nAddr, acpl::NetworkPort &nPort)
{
	nAddr.Reset();
	nPort.Set(0);
	return (acpl::cp::_net_socket_address_extract(nAddrData, nAddr, nPort) == true);
}


// PUBLIC

acpl::Socket::Address::Address()
{
	acpl::cp::_net_socket_address_init(this->mData);
}


bool acpl::Socket::Address::IsSetup() const
{
	return acpl::cp::_net_socket_address_is_setup(this->mData);
}

bool acpl::Socket::Address::Get(acpl::Ipv4Addr &nAddr, acpl::NetworkPort &nPort) const
{
	return _local_Socket_Address_GetSetup(this->mData, nAddr, nPort);
}

bool acpl::Socket::Address::Get(acpl::Ipv6Addr &nAddr, acpl::NetworkPort &nPort) const
{
	return _local_Socket_Address_GetSetup(this->mData, nAddr, nPort);
}


// PROTECTED

bool acpl::Socket::Address::Setup(const acpl::Ipv4Addr &nAddr, const acpl::NetworkPort &nPort)
{
	return _local_Socket_Address_Setup(this->mData, *this, nAddr, nPort);
}

bool acpl::Socket::Address::Setup(const acpl::Ipv6Addr &nAddr, const acpl::NetworkPort &nPort)
{
	return _local_Socket_Address_Setup(this->mData, *this, nAddr, nPort);
}

void acpl::Socket::Address::Reset()
{
	acpl::cp::_net_socket_address_init(this->mData);
}



// 
// SocketTcp
// 
