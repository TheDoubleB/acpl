#ifndef ACPL_BITS_H
#define ACPL_BITS_H

namespace acpl
{
	
	class Bits
	{
		
		private:
			inline Bits() { }
			inline virtual ~Bits() { }
		
		public:
			template <class tType, class tTypeCompat>
			static inline tType Get(const tType &nVar, const tTypeCompat &nMask)
			{
				return (nVar & static_cast<tType>(nMask));
			}
			
			template <class tType, class tTypeCompat>
			static inline bool AllSet(const tType &nVar, const tTypeCompat &nMask)
			{
				return ((nVar & static_cast<tType>(nMask)) == static_cast<tType>(nMask));
			}
			
			template <class tType, class tTypeCompat>
			static inline bool AnySet(const tType &nVar, const tTypeCompat &nMask)
			{
				return ((nVar & static_cast<tType>(nMask)) != static_cast<tType>(0));
			}
			
			template <class tType, class tTypeCompat>
			static inline void Set(tType &nVar, const tTypeCompat &nMask, bool nVal)
			{
				if (nVal == true)
					nVar |= static_cast<tType>(nMask);
				else
					nVar &= ~static_cast<tType>(nMask);
			}
		
	};
	
}

#endif // ACPL_BITS_H
