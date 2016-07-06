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
	// Forward-declaration
	// 
	
	template <class tType>
	class Num;
	
	
	// 
	// Floating-point type manipulation class
	// 
	
	class Float
	{
		private:
			template <class tFloatType, class tBinRep>
			union Rep
			{
				tFloatType uFloat;
				tBinRep uBin;
			};
		
		public:
			typedef long double Largest; // largest floating-point type
			struct Parts
			{
				typedef acpl::UInt64 Mantissa;
				typedef acpl::UInt16 Exponent;
				typedef acpl::UInt8 Sign;
				
				acpl::Float::Parts::Mantissa sMan;
				acpl::Float::Parts::Exponent sExp;
				acpl::Float::Parts::Sign sSign;
			};
		
		private:
			static inline void I_TypeCheck(const acpl::Float32 &) { }
			static inline void I_TypeCheck(const acpl::Float64 &) { }
			static inline void I_TypeCheck(const acpl::Float::Largest &) { }
			
			template <class tType>
			static inline tType I_Create(acpl::UInt8 nSign, acpl::UInt16 nExp, acpl::UInt64 nMan)
			{
				if (sizeof(tType) == sizeof(acpl::Float32))
					return static_cast<tType>(acpl::Float::Create<acpl::Float32>(nSign, nExp, nMan));
				else
				if (sizeof(tType) == sizeof(acpl::Float64))
					return static_cast<tType>(acpl::Float::Create<acpl::Float64>(nSign, nExp, nMan));
				else
					return static_cast<tType>(acpl::Float::Create<acpl::Float::Largest>(nSign, nExp, nMan));
			}
		
		protected:
			template<class tNumType> friend class Num;
			
			template <class tType>
			static inline tType I_Min()
			{
				return acpl::Float::I_Create<tType>(1, 0x7FFE, acpl::Const::UI64(0xFFFFFFFF, 0xFFFFFFFF));
			}
			
			template <class tType>
			static inline tType I_Max()
			{
				return acpl::Float::I_Create<tType>(0, 0x7FFE, acpl::Const::UI64(0xFFFFFFFF, 0xFFFFFFFF));
			}
			
			template <class tType>
			static inline tType I_QNaN()
			{
				return static_cast<tType>(acpl::Float::Create<acpl::Float64>(0, 0x7FF, acpl::Const::UI64(0x00080000, 0x00000000)));
			}
			
			template <class tType>
			static inline tType I_SNaN()
			{
				acpl::Float64 oSNaN = acpl::Float::Create<acpl::Float64>(0, 0x7FF, acpl::Const::UI64(0x00040000, 0x00000000));
				acpl::Float::Parts oParts;
				acpl::Float::GetParts(oSNaN, oParts);
				
				return ((oParts.sMan == acpl::Const::UI64(0x00040000, 0x00000000)) ? static_cast<tType>(oSNaN) : acpl::Float::I_QNaN<tType>());
			}
		
		public:
			static inline bool HasExtPrec()
			{
				return (sizeof(acpl::Float::Largest) > sizeof(acpl::Float64));
			}
			
			static inline void GetParts(acpl::Float32 nVal, acpl::Float::Parts &nParts)
			{
				acpl::Float::Rep<acpl::Float32, acpl::UInt32> oRep = { nVal };
				nParts.sSign = (oRep.uBin >> 31);
				nParts.sExp = ((oRep.uBin >> 23) & 0xFF);
				nParts.sMan =  (oRep.uBin & 0x7FFFFF);
			}
			
			static inline void GetParts(acpl::Float64 nVal, acpl::Float::Parts &nParts)
			{
				acpl::Float::Rep<acpl::Float64, acpl::UInt64> oRep = { nVal };
				nParts.sSign = (oRep.uBin >> 63);
				nParts.sExp = ((oRep.uBin >> 52) & 0x7FF);
				nParts.sMan =  (oRep.uBin & acpl::Const::UI64(0x000FFFFF, 0xFFFFFFFF));
			}
			
			static inline void GetParts(acpl::Float::Largest nVal, acpl::Float::Parts &nParts)
			{
				// if `long double` is the same size as `double`
				if (sizeof(acpl::Float::Largest) == sizeof(acpl::Float64))
					return acpl::Float::GetParts(static_cast<acpl::Float64>(nVal), nParts);
				
				// x86 extended precision (80 bits), always little-endian
				acpl::Float::Rep<acpl::Float::Largest, acpl::UInt64[2]> oRep = { nVal };
				nParts.sSign = ((oRep.uBin[1] >> 15) & 0x01);
				nParts.sExp = (oRep.uBin[1] & 0x7FFF);
				nParts.sMan = oRep.uBin[0];
			}
			
			static inline void SetParts(acpl::Float32 &nVal, acpl::UInt8 nSign, acpl::UInt16 nExp, acpl::UInt64 nMan)
			{
				acpl::Float::Rep<acpl::Float32, acpl::UInt32> oRep;
				oRep.uBin = 
					(static_cast<acpl::UInt32>(nSign) << 31) |
					(static_cast<acpl::UInt32>(nExp & 0xFF) << 23) |
					(nMan & 0x7FFFFF);
				nVal = oRep.uFloat;
			}
			
			static inline void SetParts(acpl::Float64 &nVal, acpl::UInt8 nSign, acpl::UInt16 nExp, acpl::UInt64 nMan)
			{
				acpl::Float::Rep<acpl::Float64, acpl::UInt64> oRep;
				oRep.uBin = 
					(static_cast<acpl::UInt64>(nSign) << 63) |
					(static_cast<acpl::UInt64>(nExp & 0x7FF) << 52) |
					(nMan & acpl::Const::UI64(0x000FFFFF, 0xFFFFFFFF));
				nVal = oRep.uFloat;
			}
			
			static inline void SetParts(acpl::Float::Largest &nVal, acpl::UInt8 nSign, acpl::UInt16 nExp, acpl::UInt64 nMan)
			{
				// if `long double` is the same size as `double`
				if (sizeof(acpl::Float::Largest) == sizeof(acpl::Float64))
				{
					acpl::Float64 oVal;
					acpl::Float::SetParts(oVal, nSign, nExp, nMan);
					nVal = oVal;
					return;
				}
				
				// x86 extended precision (80 bits), always little-endian
				acpl::Float::Rep<acpl::Float::Largest, acpl::UInt64[2]> oRep;
				oRep.uBin[1] =
					(static_cast<acpl::UInt64>(nSign & 0x01) << 15) |
					(static_cast<acpl::UInt64>(nExp & 0x7FFF));
				oRep.uBin[0] = nMan;
				nVal = oRep.uFloat;
			}
			
			template <class tType>
			static inline tType Create(acpl::UInt8 nSign, acpl::UInt16 nExp, acpl::UInt64 nMan)
			{
				tType oVal;
				acpl::Float::SetParts(oVal, nSign, nExp, nMan);
				return oVal;
			}
			
			template <class tType>
			static inline tType FractMin()
			{
				return acpl::Float::Create<tType>(0, 0, 1);
			}
			
			template <class tType>
			static inline tType Min()
			{
				return acpl::Float::Create<tType>(1, 0x7FFE, acpl::Const::UI64(0xFFFFFFFF, 0xFFFFFFFF));
			}
			
			template <class tType>
			static inline tType Max()
			{
				return acpl::Float::Create<tType>(0, 0x7FFE, acpl::Const::UI64(0xFFFFFFFF, 0xFFFFFFFF));
			}
			
			template <class tType>
			static inline tType Inf()
			{
				return acpl::Float::Create<tType>(0, 0x7FFF, acpl::Const::UI64(0x80000000, 0x00000000));
			}
			
			template <class tType>
			static inline tType NaN()
			{
				return acpl::Float::QNaN<tType>();
			}
			
			template <class tType>
			static inline tType QNaN()
			{
				acpl::Float::I_TypeCheck(tType(0.0));
				return acpl::Float::I_QNaN<tType>();
			}
			
			template <class tType>
			static inline tType SNaN()
			{
				acpl::Float::I_TypeCheck(tType(0.0));
				return acpl::Float::I_SNaN<tType>();
			}
			
			template <class tType>
			static inline bool IsNaN(tType nVal)
			{
				acpl::Float::Parts oParts;
				acpl::Float::Parts::Exponent oInfExp;
				
				acpl::Float::GetParts(acpl::Float::Inf<tType>(), oParts);
				oInfExp = oParts.sExp;
				
				acpl::Float::GetParts(nVal, oParts);
				return (oParts.sExp == oInfExp && (oParts.sMan & acpl::Const::UI64(0x7FFFFFFF, 0xFFFFFFFF)) != 0);
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
				return (static_cast<acpl::Float::Largest>(tType(1.5)) == static_cast<acpl::Float::Largest>(1.5));
			}
			
			static inline tType Min()
			{
				// Such a complicated expression for signed type is due to GCC's
				// and MSVC's overflow warning outcries and to avoid bitwise
				// shifting due to compatibility with the float types. It is
				// resolved at the time of compiling so this ridiculous
				// expressions have absolutely no strain on at run-time.
				if (IsFloat() == true)
					return acpl::Float::I_Min<tType>();
				else
				if (IsSigned() == true)
					return (((tType(1) * (static_cast<acpl::UInt64>(0x40) << ((((sizeof(tType) > sizeof(acpl::UInt64)) ? sizeof(acpl::UInt64) : sizeof(tType)) - 1) * 8))) * static_cast<tType>((IsSigned() == true) ? -1 : 1)) * 2);
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
					return acpl::Float::I_Max<tType>();
				else
				if (IsSigned() == true)
					return (tType(-1) - (((tType(1) * (static_cast<acpl::UInt64>(0x40) << ((((sizeof(tType) > sizeof(acpl::UInt64)) ? sizeof(acpl::UInt64) : sizeof(tType)) - 1) * 8))) * static_cast<tType>((IsSigned() == true) ? -1 : 1)) * 2));
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
