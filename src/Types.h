#ifndef ACPL_TYPES_H
#define ACPL_TYPES_H

#include "Defines.h"


#if   (acplCRuntime == acplCRuntimeGlibc)
#	include <stdint.h>
#	include <stdlib.h>
#elif (acplCRuntime == acplCRuntimeMscrt)
#	include <stddef.h> // Needed for definition of NULL, other types are simulated here.
#else
#	error Include files for this CRT.
#endif


namespace acpl
{
	
	// 
	// Platform-specific types
	// 
	
#if   (acplCRuntime == acplCRuntimeGlibc)
	
	typedef int8_t  SInt8;
	typedef int16_t SInt16;
	typedef int32_t SInt32;
	typedef int64_t SInt64;
	
	typedef uint8_t  UInt8;
	typedef uint16_t UInt16;
	typedef uint32_t UInt32;
	typedef uint64_t UInt64;
	
	typedef float  Float32;
	typedef double Float64;
	
	typedef size_t  SizeT;
	typedef ssize_t SSizeT;
	typedef off_t   OffT;
	
	typedef time_t TimeT;
	
	typedef char Pathchar;
	
#elif (acplCRuntime == acplCRuntimeMscrt)
	
	typedef __int8  SInt8;
	typedef __int16 SInt16;
	typedef __int32 SInt32;
	typedef __int64 SInt64;
	
	typedef unsigned __int8  UInt8;
	typedef unsigned __int16 UInt16;
	typedef unsigned __int32 UInt32;
	typedef unsigned __int64 UInt64;
	
	typedef float  Float32;
	typedef double Float64;
	
#	if defined(_WIN64)
		typedef unsigned __int64 SizeT;
		typedef __int64          SSizeT;
#	else
		typedef unsigned long    SizeT;
		typedef long             SSizeT;
#	endif
	
	typedef __int64 OffT;
	
	typedef __int64 TimeT;
	
	typedef wchar_t Pathchar;
	
#else 
	
#	error Type definitions for this CRT.
	
	// This is necessary for doxygen documentation; compilers should never get up-to this point.
	
	typedef __PLATFORM_SPECIFIC_TYPE__ SInt8;
	typedef __PLATFORM_SPECIFIC_TYPE__ SInt16;
	typedef __PLATFORM_SPECIFIC_TYPE__ SInt32;
	typedef __PLATFORM_SPECIFIC_TYPE__ SInt64;
	
	typedef __PLATFORM_SPECIFIC_TYPE__ UInt8;
	typedef __PLATFORM_SPECIFIC_TYPE__ UInt16;
	typedef __PLATFORM_SPECIFIC_TYPE__ UInt32;
	typedef __PLATFORM_SPECIFIC_TYPE__ UInt64;
	
	typedef __PLATFORM_SPECIFIC_TYPE__ Float32;
	typedef __PLATFORM_SPECIFIC_TYPE__ Float64;
	
	typedef __PLATFORM_SPECIFIC_TYPE__ SizeT;
	typedef __PLATFORM_SPECIFIC_TYPE__ SSizeT;
	typedef __PLATFORM_SPECIFIC_TYPE__ OffT;
	
	typedef __PLATFORM_SPECIFIC_TYPE__ TimeT;
	
	typedef __PLATFORM_SPECIFIC_TYPE__ Pathchar;
	
#endif
	
	
	// 
	// Platform-independant types
	// 
	
	typedef acpl::UInt32 Unichar;
	
	
	// 
	// Other
	// 
	
	class Const
	{
		public:
			static inline acpl::UInt64 UI64(acpl::UInt32 nUpper, acpl::UInt32 nLower)
			{
				return ((static_cast<acpl::UInt64>(nUpper) << 32) | static_cast<acpl::UInt64>(nLower));
			}
			
			static inline acpl::SInt64 SI64(acpl::UInt32 nUpper, acpl::UInt32 nLower)
			{
				return static_cast<acpl::SInt64>(acpl::Const::UI64(nUpper, nLower));
			}
	};
	
	
	// 
	// Numeric type information class
	// 
	
	template <class tType>
	class Num
	{
		public:
			static inline bool IsSigned()
			{
				// The `<=` is used instead of just `<` because GCC goes bonkers
				// when `tType` is an unsigned type. The expression itself will
				// never equal 0, so `<=` is the same as `<` and the GCC warning
				// is gone.
				return (tType(-1) <= 0);
			}
			
			static inline bool IsFloat()
			{
				return ((tType(1.1) * 10) == 11);
			}
			
			static inline tType Min()
			{
				// Such a complicated expression for signed type is due to GCC's
				// and MSVC's overflow warning outcries and to avoid bitwise
				// shifting due to compatibility with the float types. It is
				// resolved at the time of compiling so this ridiculous
				// expressions have absolutely no strain on at run-time.
				if (IsFloat() == true)
				{
					union { acpl::UInt64 uInt; tType uType; } oFiu = { ((sizeof(tType) == 4) ? acpl::Const::UI64(0x00000000, 0xFF7FFFFF) : acpl::Const::UI64(0xFFEFFFFF, 0xFFFFFFFF)) };
					return oFiu.uType;
				}
				else
				if (IsSigned() == true)
					return (((tType(1) * (static_cast<acpl::UInt64>(0x40) << ((sizeof(tType) - 1) * 8))) * static_cast<tType>((IsSigned() == true) ? -1 : 1)) * 2);
				else
					return 0;
			}
			
			static inline tType Max()
			{
				// Such a complicated expression for signed type is due to GCC's
				// and MSVC's overflow warning outcries and to avoid bitwise
				// shifting due to compatibility with the float types. It is
				// resolved at the time of compiling so this ridiculous
				// expressions have absolutely no strain on at run-time.
				if (IsFloat() == true)
				{
					union { acpl::UInt64 uInt; tType uType; } oFiu = { ((sizeof(tType) == 4) ? acpl::Const::UI64(0x00000000, 0x7F7FFFFF) : acpl::Const::UI64(0x7FEFFFFF, 0xFFFFFFFF)) };
					return oFiu.uType;
				}
				else
				if (IsSigned() == true)
					return (tType(-1) - (((tType(1) * (static_cast<acpl::UInt64>(0x40) << ((sizeof(tType) - 1) * 8))) * static_cast<tType>((IsSigned() == true) ? -1 : 1)) * 2));
				else
					return tType(-1);
			}
	};
	
	
	// 
	// Enumerator container class
	// 
	
	template <class tEnumType, class tHolderType>
	class Enum
	{
		private:
			tHolderType mHolder;
		
		public:
			inline Enum() { }
			inline Enum(const acpl::Enum<tEnumType, tHolderType> &nObj) : mHolder(nObj.mHolder) { }
			inline Enum(const tEnumType &nVal) : mHolder(static_cast<tHolderType>(nVal)) { }
			inline virtual ~Enum() { }
			
			inline tEnumType Value() const { return static_cast<tEnumType>(this->mHolder); }
			
			inline acpl::Enum<tEnumType, tHolderType> &operator=(const acpl::Enum<tEnumType, tHolderType> &nObj) { this->mHolder = nObj.mHolder; return *this; }
			
			inline bool operator==(const acpl::Enum<tEnumType, tHolderType> &nObj) { return (this->mHolder == nObj.mHolder); }
			inline bool operator!=(const acpl::Enum<tEnumType, tHolderType> &nObj) { return (this->mHolder != nObj.mHolder); }
			
			inline bool operator< (const acpl::Enum<tEnumType, tHolderType> &nObj) { return (this->mHolder <  nObj.mHolder); }
			inline bool operator> (const acpl::Enum<tEnumType, tHolderType> &nObj) { return (this->mHolder >  nObj.mHolder); }
			
			inline bool operator<=(const acpl::Enum<tEnumType, tHolderType> &nObj) { return (this->mHolder <= nObj.mHolder); }
			inline bool operator>=(const acpl::Enum<tEnumType, tHolderType> &nObj) { return (this->mHolder >= nObj.mHolder); }
	};

}


#endif // ACPL_TYPES_H
