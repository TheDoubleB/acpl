#ifndef ACPL_CP_BASIC_INC_CPP
#define ACPL_CP_BASIC_INC_CPP


#if   (acplCRuntime == acplCRuntimeGlibc)
#	include "glibc/Basic.inc.cpp"
#elif (acplCRuntime == acplCRuntimeMscrt)
#	include "mscrt/Basic.inc.cpp"
#else
#	error Inclusion of platform-specific `Basic.inc.cpp` required
#endif


#endif // ACPL_CP_BASIC_INC_CPP
