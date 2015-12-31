#ifndef ACPL_RAII_H
#define ACPL_RAII_H

#include "Macros.h"
#include "Types.h"

namespace acpl
{
	
	template <void (*tFunc)()>
	class RaiiFunc0
	{
			acplHideClassCopy(RaiiFunc0<tFunc>);
		
		private:
			bool mEnabled;
		
		public:
			RaiiFunc0()
				: mEnabled(true)
			{}
			~RaiiFunc0()
			{
				if (this->mEnabled == true)
					tFunc();
			}
			
			void Enable()
			{
				this->mEnabled = true;
			}
			void Disable()
			{
				this->mEnabled = false;
			}
		
	};
	
	template <class tRet, tRet (*tFunc)()>
	class RaiiFuncR0
	{
			acplHideClassCopy(acplMacroVa(RaiiFuncR0<tRet, tFunc>));
		
		private:
			bool mEnabled;
		
		public:
			RaiiFuncR0()
				: mEnabled(true)
			{}
			~RaiiFuncR0()
			{
				if (this->mEnabled == true)
					tFunc();
			}
			
			void Enable()
			{
				this->mEnabled = true;
			}
			void Disable()
			{
				this->mEnabled = false;
			}
		
	};
	
	template <class tParam, void (*tFunc)(tParam)>
	class RaiiFunc1
	{
			acplHideClassCopyIa(acplMacroVa(RaiiFunc1<tParam, tFunc>), nRaiiFunc1, mParam(nRaiiFunc1.mParam));
		
		private:
			bool mEnabled;
			tParam &mParam;
		
		public:
			RaiiFunc1(tParam &nParam)
				: mEnabled(true), mParam(nParam)
			{}
			~RaiiFunc1()
			{
				if (this->mEnabled == true)
					tFunc(this->mParam);
			}
			
			void Enable()
			{
				this->mEnabled = true;
			}
			void Disable()
			{
				this->mEnabled = false;
			}
		
	};
	
	template <class tRet, class tParam, tRet (*tFunc)(tParam)>
	class RaiiFuncR1
	{
			acplHideClassCopyIa(acplMacroVa(RaiiFuncR1<tRet, tParam, tFunc>), nRaiiFuncR1, mParam(nRaiiFuncR1.mParam));
		
		private:
			bool mEnabled;
			tParam &mParam;
		
		public:
			RaiiFuncR1(tParam &nParam)
				: mEnabled(true), mParam(nParam)
			{}
			~RaiiFuncR1()
			{
				if (this->mEnabled == true)
					tFunc(this->mParam);
			}
			
			void Enable()
			{
				this->mEnabled = true;
			}
			void Disable()
			{
				this->mEnabled = false;
			}
		
	};
	
	template <class tParam, class tAddParam, void (*tFunc)(tParam, tAddParam)>
	class RaiiFunc2
	{
			acplHideClassCopyIa(acplMacroVa(RaiiFunc2<tParam, tAddParam, tFunc>), nRaiiFunc2, mParam(nRaiiFunc2.mParam), mAddParam(nRaiiFunc2.mAddParam));
		
		private:
			bool mEnabled;
			tParam &mParam;
			tAddParam &mAddParam;
		
		public:
			RaiiFunc2(tParam &nParam, tAddParam &nAddParam)
				: mEnabled(true), mParam(nParam), mAddParam(nAddParam)
			{}
			~RaiiFunc2()
			{
				if (this->mEnabled == true)
					tFunc(this->mParam, this->mAddParam);
			}
			
			void Enable()
			{
				this->mEnabled = true;
			}
			void Disable()
			{
				this->mEnabled = false;
			}
		
	};
	
	template <class tRet, class tParam, class tAddParam, tRet (*tFunc)(tParam, tAddParam)>
	class RaiiFuncR2
	{
			acplHideClassCopyIa(acplMacroVa(RaiiFuncR2<tRet, tParam, tAddParam, tFunc>), nRaiiFuncR2, mParam(nRaiiFuncR2.mParam), mAddParam(nRaiiFuncR2.mAddParam));
		
		private:
			bool mEnabled;
			tParam &mParam;
			tAddParam &mAddParam;
		
		public:
			RaiiFuncR2(tParam &nParam, tAddParam &nAddParam)
				: mEnabled(true), mParam(nParam), mAddParam(nAddParam)
			{}
			~RaiiFuncR2()
			{
				if (this->mEnabled == true)
					tFunc(this->mParam, this->mAddParam);
			}
			
			void Enable()
			{
				this->mEnabled = true;
			}
			void Disable()
			{
				this->mEnabled = false;
			}
		
	};
	
	template <class tClass, void (tClass::*tMethod)()>
	class RaiiMethod0
	{
			acplHideClassCopyIa(acplMacroVa(RaiiMethod0<tClass, tMethod>), nRaiiMethod0, mClass(nRaiiMethod0.mClass));
		
		private:
			bool mEnabled;
			tClass &mClass;
		
		public:
			RaiiMethod0(tClass &nClass)
				: mEnabled(true), mClass(nClass)
			{}
			~RaiiMethod0()
			{
				if (this->mEnabled == true)
					(this->mClass.*tMethod)();
			}
			
			void Enable()
			{
				this->mEnabled = true;
			}
			void Disable()
			{
				this->mEnabled = false;
			}
		
	};
	
	template <class tClass, class tRet, tRet (tClass::*tMethod)()>
	class RaiiMethodR0
	{
			acplHideClassCopyIa(acplMacroVa(RaiiMethodR0<tClass, tRet, tMethod>), nRaiiMethodR0, mClass(nRaiiMethodR0.mClass));
		
		private:
			bool mEnabled;
			tClass &mClass;
		
		public:
			RaiiMethodR0(tClass &nClass)
				: mEnabled(true), mClass(nClass)
			{}
			~RaiiMethodR0()
			{
				if (this->mEnabled == true)
					(this->mClass.*tMethod)();
			}
			
			void Enable()
			{
				this->mEnabled = true;
			}
			void Disable()
			{
				this->mEnabled = false;
			}
		
	};
	
	template <class tClass, class tParam, void (tClass::*tMethod)(tParam)>
	class RaiiMethod1
	{
			acplHideClassCopyIa(acplMacroVa(RaiiMethod1<tClass, tParam, tMethod>), nRaiiMethod1, mClass(nRaiiMethod1.mClass), mParam(nRaiiMethod1.mParam));
		
		private:
			bool mEnabled;
			tClass &mClass;
			tParam &mParam;
		
		public:
			RaiiMethod1(tClass &nClass, tParam &nParam)
				: mEnabled(true), mClass(nClass), mParam(nParam)
			{}
			~RaiiMethod1()
			{
				if (this->mEnabled == true)
					(this->mClass.*tMethod)(this->mParam);
			}
			
			void Enable()
			{
				this->mEnabled = true;
			}
			void Disable()
			{
				this->mEnabled = false;
			}
		
	};
	
	template <class tClass, class tRet, class tParam, tRet (tClass::*tMethod)(tParam)>
	class RaiiMethodR1
	{
			acplHideClassCopyIa(acplMacroVa(RaiiMethodR1<tClass, tRet, tParam, tMethod>), nRaiiMethodR1, mClass(nRaiiMethodR1.mClass), mParam(nRaiiMethodR1.mParam));
		
		private:
			bool mEnabled;
			tClass &mClass;
			tParam &mParam;
		
		public:
			RaiiMethodR1(tClass &nClass, tParam &nParam)
				: mEnabled(true), mClass(nClass), mParam(nParam)
			{}
			~RaiiMethodR1()
			{
				if (this->mEnabled == true)
					(this->mClass.*tMethod)(this->mParam);
			}
			
			void Enable()
			{
				this->mEnabled = true;
			}
			void Disable()
			{
				this->mEnabled = false;
			}
		
	};
	
	template <class tClass, class tParam, class tAddParam, void (tClass::*tMethod)(tParam, tAddParam)>
	class RaiiMethod2
	{
			acplHideClassCopyIa(acplMacroVa(RaiiMethod2<tClass, tParam, tAddParam, tMethod>), nRaiiMethod2, mClass(nRaiiMethod2.mClass), mParam(nRaiiMethod2.mParam), mAddParam(nRaiiMethod2.mAddParam));
		
		private:
			bool mEnabled;
			tClass &mClass;
			tParam &mParam;
			tAddParam &mAddParam;
		
		public:
			RaiiMethod2(tClass &nClass, tParam &nParam, tAddParam &nAddParam)
				: mEnabled(true), mClass(nClass), mParam(nParam), mAddParam(nAddParam)
			{}
			~RaiiMethod2()
			{
				if (this->mEnabled == true)
					(this->mClass.*tMethod)(this->mParam, this->mAddParam);
			}
			
			void Enable()
			{
				this->mEnabled = true;
			}
			void Disable()
			{
				this->mEnabled = false;
			}
		
	};
	
	template <class tClass, class tRet, class tParam, class tAddParam, tRet (tClass::*tMethod)(tParam, tAddParam)>
	class RaiiMethodR2
	{
			acplHideClassCopyIa(acplMacroVa(RaiiMethodR2<tClass, tRet, tParam, tAddParam, tMethod>), nRaiiMethodR2, mClass(nRaiiMethodR2.mClass), mParam(nRaiiMethodR2.mParam), mAddParam(nRaiiMethodR2.mAddParam));
		
		private:
			bool mEnabled;
			tClass &mClass;
			tParam &mParam;
			tAddParam &mAddParam;
		
		public:
			RaiiMethodR2(tClass &nClass, tParam &nParam, tAddParam &nAddParam)
				: mEnabled(true), mClass(nClass), mParam(nParam), mAddParam(nAddParam)
			{}
			~RaiiMethodR2()
			{
				if (this->mEnabled == true)
					(this->mClass.*tMethod)(this->mParam, this->mAddParam);
			}
			
			void Enable()
			{
				this->mEnabled = true;
			}
			void Disable()
			{
				this->mEnabled = false;
			}
		
	};
	
}

#endif // ACPL_RAII_H
