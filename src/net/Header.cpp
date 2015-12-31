#include "Header.h"


// 
// IpHeader
// 



// 
// IpHeader::Checksum
// 

// INTERNAL

static inline acpl::UInt16 _local_IpHeader_Checksum_Proc(acpl::UInt32 nSum)
{
	return ~static_cast<acpl::UInt32>((nSum & 0xFFFF) + (nSum >> 16));
}


// PUBLIC

void acpl::IpHeader::Checksum::Add(const void *nBfr, acpl::SizeT nBfrLen)
{
	// NOTE: Big/little endian issue
	//   Bytes in buffer pointed to by `nBfr` should be taken in big endian
	//   order, but on little endian CPU (thanks to the `oCurPtr` assignment),
	//   the bytes are actually taken in little endian (reverse) order. This
	//   would be a problem, but because of the 2-byte folding (see:
	//   `_local_IpHeader_Checksum_Proc`) the problem is gone. `this->mSum` value
	//   will be different on big and little endian systems, but the end result
	//   (the actual checksum) will be the same. The only real issue was with
	//   the "stray byte" situation, that's why a trick with the union is used.
	
	const acpl::UInt16 *oCurPtr = reinterpret_cast<const acpl::UInt16 *>(nBfr);
	const acpl::UInt16 *oEndPtr = oCurPtr + (nBfrLen / 2);
	
	while (oCurPtr < oEndPtr)
		this->mSum += *oCurPtr++;
	
	// If `nBfrLen` was an odd number, there is one "stray" byte to take into
	// account
	if ((nBfrLen % 2) != 0)
	{
		union { acpl::UInt8 uVal8; acpl::UInt16 uVal16; } oLastVal = { *reinterpret_cast<const acpl::UInt8 *>(oCurPtr) };
		this->mSum += oLastVal.uVal16;
	}
}

void acpl::IpHeader::Checksum::Calc(acpl::IpHeader::Checksum::DataType &nCsum)
{
	nCsum[0] = 0;
	nCsum[1] = 0;
	
	union { acpl::UInt16 uVal; acpl::UInt8 uBytes[2]; } oCsum;
	oCsum.uVal = _local_IpHeader_Checksum_Proc(this->mSum);
	
	nCsum[0] = oCsum.uBytes[0];
	nCsum[1] = oCsum.uBytes[1];
}

bool acpl::IpHeader::Checksum::Verify()
{
	return (_local_IpHeader_Checksum_Proc(this->mSum) == 0);
}



// 
// Ipv4Header
// 
