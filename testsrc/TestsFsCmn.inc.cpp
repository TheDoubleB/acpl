#include "../src/Defines.h"


#if   (acplCRuntime == acplCRuntimeGlibc)

#	define _local_PathcharPx(N_PARAM) N_PARAM
#	define _local_PathcharFmt "%s"
#	define _local_DirPerm(N_ACCMODE) N_ACCMODE
#	define _local_FilePerm(N_ACCMODE) N_ACCMODE

#elif (acplCRuntime == acplCRuntimeMscrt)

#	define _local_PathcharPx(N_PARAM) L ## N_PARAM
#	define _local_PathcharFmt "%ls"
#	define _local_DirPerm(N_ACCMODE) 0777
#	define _local_FilePerm(N_ACCMODE) 0666

#else

#	error Add platform specific definitions.

#endif
