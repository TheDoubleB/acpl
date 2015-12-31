#ifndef ACPL_ENDIAN_H
#define ACPL_ENDIAN_H

#include "Macros.h"
#include "Types.h"

// Big == Big endian
// Lit == Little endian
// Nat == Native (system) endian

namespace acpl
{
	class Endian
	{
			acplHideClassCopy(Endian);
		
		private:
			union IdentType { acpl::UInt16 uValue; acpl::UInt8 uBytes[sizeof(acpl::UInt16)]; };
		
		private:
			inline Endian() { }
			inline ~Endian() { }
			
			template <class tIntType>
			static inline tIntType IFromBig(acpl::SizeT nOff, const void *nBfr, acpl::SizeT nCount)
			{
				return ((nCount >= (nOff + 1)) ? (static_cast<tIntType>(static_cast<const acpl::UInt8 *>(nBfr)[nOff]) << ((nCount - (nOff + 1)) * 8)) : 0);
			}
			
			template <class tIntType>
			static inline tIntType IFromLittle(acpl::SizeT nOff, const void *nBfr, acpl::SizeT nCount)
			{
				return ((nCount >= (nOff + 1)) ? (static_cast<tIntType>(static_cast<const acpl::UInt8 *>(nBfr)[nOff]) << (((sizeof(tIntType) >= (nOff + 1)) ? nOff : 0) * 8)) : 0);
			}
			
			template <class tIntType>
			static inline void IToBig(acpl::SizeT nOff, tIntType nVal, acpl::UInt64 nMask, void *nBfr, acpl::SizeT nCount)
			{
				if (nCount >= (nOff + 1))
					static_cast<acpl::UInt8 *>(nBfr)[nOff] = ((
							static_cast<acpl::UInt8 *>(nBfr)[nOff] & (~nMask >> ((nCount - (nOff + 1)) * 8))) |
							static_cast<acpl::UInt8>((nVal & nMask) >> ((nCount - (nOff + 1)) * 8))
						);
			}
			
			template <class tIntType>
			static inline void IToLittle(acpl::SizeT nOff, tIntType nVal, acpl::UInt64 nMask, void *nBfr, acpl::SizeT nCount)
			{
				if (nCount >= (nOff + 1))
					static_cast<acpl::UInt8 *>(nBfr)[nOff] = ((
							static_cast<acpl::UInt8 *>(nBfr)[nOff] & (~nMask >> (((sizeof(tIntType) >= 1) ? nOff : 0) * 8))) |
							static_cast<acpl::UInt8>((nVal & nMask) >> (((sizeof(tIntType) >= 1) ? nOff : 0) * 8))
						);
			}
		
		public:
			static inline bool IsNativeBig()
			{
				acpl::Endian::IdentType oVal = { 0x1234 };
				return (*oVal.uBytes == 0x12);
			}
			
			static inline bool IsNativeLittle()
			{
				acpl::Endian::IdentType oVal = { 0x1234 };
				return (*oVal.uBytes == 0x34);
			}
			
			
			template <class tIntType>
			static inline tIntType FromBig(const void *nBfr, acpl::SizeT nCount = sizeof(tIntType))
			{
				if (nCount > sizeof(tIntType))
					nCount = sizeof(tIntType);
				
				return (
						acpl::Endian::IFromBig<tIntType>(0, nBfr, nCount) |
						acpl::Endian::IFromBig<tIntType>(1, nBfr, nCount) |
						acpl::Endian::IFromBig<tIntType>(2, nBfr, nCount) |
						acpl::Endian::IFromBig<tIntType>(3, nBfr, nCount) |
						acpl::Endian::IFromBig<tIntType>(4, nBfr, nCount) |
						acpl::Endian::IFromBig<tIntType>(5, nBfr, nCount) |
						acpl::Endian::IFromBig<tIntType>(6, nBfr, nCount) |
						acpl::Endian::IFromBig<tIntType>(7, nBfr, nCount)
					);
			}
			
			template <class tIntType>
			static inline void FromBig(tIntType &nVal, const void *nBfr, acpl::SizeT nCount = sizeof(tIntType))
			{
				nVal = acpl::Endian::FromBig<tIntType>(nBfr, nCount);
			}
			
			template <class tIntType>
			static inline tIntType FromBig(acpl::UInt64 nMask, const void *nBfr, acpl::SizeT nCount = sizeof(tIntType))
			{
				return (acpl::Endian::FromBig<tIntType>(nBfr, nCount) & nMask);
			}
			
			template <class tIntType>
			static inline void FromBig(tIntType &nVal, acpl::UInt64 nMask, const void *nBfr, acpl::SizeT nCount = sizeof(tIntType))
			{
				nVal = (acpl::Endian::FromBig<tIntType>(nBfr, nCount) & nMask);
			}
			
			template <class tIntType>
			static inline tIntType FromLittle(const void *nBfr, acpl::SizeT nCount = sizeof(tIntType))
			{
				if (nCount > sizeof(tIntType))
					nCount = sizeof(tIntType);
				
				return (
						acpl::Endian::IFromLittle<tIntType>(0, nBfr, nCount) |
						acpl::Endian::IFromLittle<tIntType>(1, nBfr, nCount) |
						acpl::Endian::IFromLittle<tIntType>(2, nBfr, nCount) |
						acpl::Endian::IFromLittle<tIntType>(3, nBfr, nCount) |
						acpl::Endian::IFromLittle<tIntType>(4, nBfr, nCount) |
						acpl::Endian::IFromLittle<tIntType>(5, nBfr, nCount) |
						acpl::Endian::IFromLittle<tIntType>(6, nBfr, nCount) |
						acpl::Endian::IFromLittle<tIntType>(7, nBfr, nCount)
					);
			}
			
			template <class tIntType>
			static inline void FromLittle(tIntType &nVal, const void *nBfr, acpl::SizeT nCount = sizeof(tIntType))
			{
				nVal = acpl::Endian::FromLittle<tIntType>(nBfr, nCount);
			}
			
			template <class tIntType>
			static inline tIntType FromLittle(acpl::UInt64 nMask, const void *nBfr, acpl::SizeT nCount = sizeof(tIntType))
			{
				return (acpl::Endian::FromLittle<tIntType>(nBfr, nCount) & nMask);
			}
			
			template <class tIntType>
			static inline void FromLittle(tIntType &nVal, acpl::UInt64 nMask, const void *nBfr, acpl::SizeT nCount = sizeof(tIntType))
			{
				nVal = (acpl::Endian::FromLittle<tIntType>(nBfr, nCount) & nMask);
			}
			
			
			template <class tIntType>
			static inline void ToBig(tIntType nVal, acpl::UInt64 nMask, void *nBfr, acpl::SizeT nCount = sizeof(tIntType))
			{
				if (nCount > sizeof(tIntType))
					nCount = sizeof(tIntType);
				
				acpl::Endian::IToBig<tIntType>(0, nVal, nMask, nBfr, nCount);
				acpl::Endian::IToBig<tIntType>(1, nVal, nMask, nBfr, nCount);
				acpl::Endian::IToBig<tIntType>(2, nVal, nMask, nBfr, nCount);
				acpl::Endian::IToBig<tIntType>(3, nVal, nMask, nBfr, nCount);
				acpl::Endian::IToBig<tIntType>(4, nVal, nMask, nBfr, nCount);
				acpl::Endian::IToBig<tIntType>(5, nVal, nMask, nBfr, nCount);
				acpl::Endian::IToBig<tIntType>(6, nVal, nMask, nBfr, nCount);
				acpl::Endian::IToBig<tIntType>(7, nVal, nMask, nBfr, nCount);
			}
			
			template <class tIntType>
			static inline void ToBig(tIntType nVal, void *nBfr, acpl::SizeT nCount = sizeof(tIntType))
			{
				acpl::Endian::ToBig<tIntType>(nVal, acpl::Const::UI64(0xFFFFFFFF, 0xFFFFFFFF), nBfr, nCount);
			}
			
			template <class tIntType>
			static inline void ToLittle(tIntType nVal, acpl::UInt64 nMask, void *nBfr, acpl::SizeT nCount = sizeof(tIntType))
			{
				if (nCount > sizeof(tIntType))
					nCount = sizeof(tIntType);
				
				acpl::Endian::IToLittle<tIntType>(0, nVal, nMask, nBfr, nCount);
				acpl::Endian::IToLittle<tIntType>(1, nVal, nMask, nBfr, nCount);
				acpl::Endian::IToLittle<tIntType>(2, nVal, nMask, nBfr, nCount);
				acpl::Endian::IToLittle<tIntType>(3, nVal, nMask, nBfr, nCount);
				acpl::Endian::IToLittle<tIntType>(4, nVal, nMask, nBfr, nCount);
				acpl::Endian::IToLittle<tIntType>(5, nVal, nMask, nBfr, nCount);
				acpl::Endian::IToLittle<tIntType>(6, nVal, nMask, nBfr, nCount);
				acpl::Endian::IToLittle<tIntType>(7, nVal, nMask, nBfr, nCount);
			}
			
			template <class tIntType>
			static inline void ToLittle(tIntType nVal, void *nBfr, acpl::SizeT nCount = sizeof(tIntType))
			{
				acpl::Endian::ToLittle<tIntType>(nVal, acpl::Const::UI64(0xFFFFFFFF, 0xFFFFFFFF), nBfr, nCount);
			}
	};
}

#endif // ACPL_ENDIAN_H
