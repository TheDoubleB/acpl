#ifndef ACPL_CP_FILESYS_INC_CPP
#define ACPL_CP_FILESYS_INC_CPP


namespace acpl
{
	// 
	// Common types for file system functions
	// 
	
	namespace pi
	{
		enum _fs_path_get_last_error_context_t
		{
			_fs_path_glec_resolve,	// _fs_path_resolve() context
			_fs_path_glec_setperm,	// _fs_path_setperm() context
			_fs_path_glec_stat		// _fs_path_stat() context
		};
		
		enum _fs_dir_get_last_error_context_t
		{
			_fs_dir_glec_list_entries,	// _fs_dir_list_get_first() and _fs_dir_list_get_next() context
			_fs_dir_glec_create,		// _fs_dir_create() context
			_fs_dir_glec_remove			// _fs_dir_remove() context
		};
		
		enum _fs_file_get_last_error_context_t
		{
			_fs_file_glec_open,		// _fs_file_open()
			_fs_file_glec_unlink,	// _fs_file_unlink()
			_fs_file_glec_getoff,	// _fs_file_getoff()
			_fs_file_glec_setoff,	// _fs_file_setoff()
			_fs_file_glec_write,	// _fs_file_write()
			_fs_file_glec_read		// _fs_file_read()
		};
	}
	
}


#if   (acplCRuntime == acplCRuntimeGlibc)
#	include "glibc/Filesys.inc.cpp"
#elif (acplCRuntime == acplCRuntimeMscrt)
#	include "mscrt/Filesys.inc.cpp"
#else
#	error Inclusion of platform-specific `Filesys.inc.cpp` required
#endif


#endif // ACPL_CP_FILESYS_INC_CPP
