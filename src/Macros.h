#ifndef ACPL_MACROS_H
#define ACPL_MACROS_H


#define acplMacroVa(...) \
	__VA_ARGS__


#define acplHideClassAsop(N_CLASS_NAME) \
	public: \
		operator const N_CLASS_NAME &() const { return *this; } \
	private: \
		N_CLASS_NAME &operator=(const N_CLASS_NAME &) { return *this; }

#define acplHideClassCopy(N_CLASS_NAME) \
	acplHideClassAsop(acplMacroVa(N_CLASS_NAME)) \
	N_CLASS_NAME(const N_CLASS_NAME &) {}

#define acplHideClassCopyI(N_CLASS_NAME, ...) \
	acplHideClassAsop(acplMacroVa(N_CLASS_NAME)) \
	N_CLASS_NAME(const N_CLASS_NAME &) : __VA_ARGS__ {}

#define acplHideClassCopyIa(N_CLASS_NAME, N_CLASS_ARG_NAME, ...) \
	acplHideClassAsop(acplMacroVa(N_CLASS_NAME)) \
	N_CLASS_NAME(const N_CLASS_NAME &N_CLASS_ARG_NAME) : __VA_ARGS__ {}


#endif // ACPL_MACROS_H
