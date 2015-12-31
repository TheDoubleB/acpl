#ifndef ACPL_CP_GLIBC_FILESYS_INC_CPP
#define ACPL_CP_GLIBC_FILESYS_INC_CPP

#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>

#include "../../fs/Dir.h"
#include "../../fs/File.h"

namespace acpl
{
	
	namespace cp
	{
		// 
		// Global
		// 
		
		static inline bool _local_fs_str_len(const char *nStr, acpl::SizeT &nLen) // <<< NOTE: This function includes the NULL terminator in the nLen as well !!! ; returns true if nStr is non-NULL
		{
			nLen = 0;
			if (nStr != NULL)
			{
				while (nStr[nLen++] != '\0') ;
				return true;
			}
			else
				return false;
		}
		
		
		// 
		// Path
		// 
		
		static inline bool _local_fs_path_stat(const acpl::Path::String::Holder &nPath, bool nResolveLink, acpl::Dir::Entry::InfoStat &nInfoStat)
		{
			struct stat oSs;
			int (*oStatFn)(const char *, struct stat *) = ((nResolveLink == true) ? stat : lstat); // lstat() stats the actual link (used for dir entry listing), stat() stats the file/dir it points to
			
			if (oStatFn(nPath.Data(), &oSs) == 0)
			{
				nInfoStat.sIsSet = true;
				
				switch ((oSs.st_mode & S_IFMT))
				{
					case S_IFREG:
						nInfoStat.sType = acpl::Dir::Entry::tFile;
						break;
					
					case S_IFDIR:
						nInfoStat.sType = acpl::Dir::Entry::tDir;
						break;
					
					case S_IFLNK:
						nInfoStat.sType = acpl::Dir::Entry::tSymLink;
						break;
					
					case S_IFIFO:
						nInfoStat.sType = acpl::Dir::Entry::tPipe;
						break;
					
					case S_IFSOCK:
						nInfoStat.sType = acpl::Dir::Entry::tSocket;
						break;
					
					case S_IFCHR:
						nInfoStat.sType = acpl::Dir::Entry::tDevChar;
						break;
					
					case S_IFBLK:
						nInfoStat.sType = acpl::Dir::Entry::tDevBlock;
						break;
					
					default:
						nInfoStat.sType = acpl::Dir::Entry::tUnknown;
						break;
				}
				
				nInfoStat.sPerm.SetValue(oSs.st_mode); // NOTE: Will be trimmed to 07777 by the Permission class
				nInfoStat.sSize = oSs.st_size;
				nInfoStat.sTimeCre = oSs.st_ctime;
				nInfoStat.sTimeMod = oSs.st_mtime;
				nInfoStat.sTimeAcc = oSs.st_atime;
				
				return true;
			}
			else
			{
				nInfoStat.sIsSet = false;
				return false;
			}
		}
		
		static inline bool _fs_path_is_absolute(const acpl::Path::String::Holder &nPath)
		{
			return (nPath.Length() > 0 && nPath.Data()[0] == '/');
		}
		
		static inline bool _fs_path_is_root_dir(const acpl::Path::String::Holder &nPath)
		{
			return (acpl::cp::_fs_path_is_absolute(nPath) == true && (nPath.Length() == 1 || nPath.Data()[1] == '\0'));
		}
		
		static inline bool _fs_path_char_is_delim(acpl::Pathchar nChr)
		{
			return (nChr == '/');
		}
		
		static inline acpl::SizeT _fs_path_get_dir_delim_len()
		{
			return 1;
		}
		static inline const acpl::Pathchar *_fs_path_get_dir_delim()
		{
			return "/";
		}
		
		static inline acpl::SizeT _fs_path_get_current_dir_len()
		{
			return 1;
		}
		static inline const acpl::Pathchar *_fs_path_get_current_dir()
		{
			return ".";
		}
		
		static inline acpl::SizeT _fs_path_get_parent_dir_len()
		{
			return 2;
		}
		static inline const acpl::Pathchar *_fs_path_get_parent_dir()
		{
			return "..";
		}
		
		static inline bool _fs_path_elem_is_current_dir(const acpl::Path::String::Holder &nPath, acpl::SizeT nOff)
		{
			return (nPath.Length() > nOff && nPath.Data()[nOff++] == '.' && (nPath.Length() == nOff || nPath.Data()[nOff] == '\0' || nPath.Data()[nOff] == '/')); // <<< NOTE: '++' with nOff
		}
		
		static inline bool _fs_path_elem_is_parent_dir(const acpl::Path::String::Holder &nPath, acpl::SizeT nOff)
		{
			return (nPath.Length() > (nOff + 1) && nPath.Data()[nOff++] == '.' && nPath.Data()[nOff++] == '.' && (nPath.Length() == nOff || nPath.Data()[nOff] == '\0' || nPath.Data()[nOff] == '/')); // <<< NOTE: '++' with nOff
		}
		
		static inline acpl::SizeT _fs_path_elem_rel_with_drive_offset(const acpl::Path::String::Holder &)
		{
			// Only applies to MSCRT and relative paths containing drive letter as well (e.g.: "C:relative\\path")
			return 0;
		}
		
		static inline bool _fs_path_resolve(acpl::Path::String::Holder &nPath)
		{
			char *oAbsPath = realpath(nPath.Data(), NULL);
			acpl::SizeT oLen = 0;
			
			if (acpl::cp::_local_fs_str_len(oAbsPath, oLen) != true)
				return false;
			
			nPath.Clear().Apnd(oAbsPath, oLen);
			
			acpl::Mem::Free(oAbsPath);
			return true;
		}
		
		static inline bool _fs_path_setperm(const acpl::Path::String::Holder &nPath, const acpl::Path::Permission &nPerm)
		{
			return (chmod(nPath.Data(), nPerm.GetValue()) == 0);
		}
		
		static inline void _fs_path_umask(acpl::UInt16 nVal)
		{
			umask(nVal);
		}
		
		static inline bool _fs_path_stat(const acpl::Path::String::Holder &nPath, bool nResolveLink, acpl::Dir::Entry::InfoStat &nInfoStat)
		{
			return acpl::cp::_local_fs_path_stat(nPath, nResolveLink, nInfoStat);
		}
		
		static inline acpl::Path::LastError _fs_path_get_last_error(acpl::pi::_fs_path_get_last_error_context_t)
		{
			switch (errno)
			{
				case 0:
					return acpl::Path::leNone;
				
				case EPERM:
				case EACCES:
					return acpl::Path::leNoPerm;
				
				case ELOOP:
					return acpl::Path::leSymlinkLoop;
				
				case ENAMETOOLONG:
					return acpl::Path::leTooLong;
				
				case ENOENT:
					return acpl::Path::leNonExist;
				
				case ENOTDIR:
					return acpl::Path::leNotDir;
				
				default:
					break; // acpl::Path::leSysErr
			}
			
			return acpl::Path::leSysErr;
		}
		
		
		// 
		// Dir
		// 
		
		typedef DIR *_fs_dir_handle_t;
		
		static inline bool _local_fs_dir_get_next_entry(const acpl::cp::_fs_dir_handle_t &nHandle, bool nStat, acpl::Path::String::Holder &nPathMbl, acpl::Dir::Entry::InfoBasic &nInfoBasic, acpl::Dir::Entry::InfoStat &nInfoStat)
		{
			struct dirent oDe, *oDeRes;
			
			if (readdir_r(nHandle, &oDe, &oDeRes) != 0)
				return false;
			else
			if (oDeRes == NULL)
			{
				errno = 0; // NOTE: Reset errno for a subsequent call to get_last_error
				return false;
			}
			
			
			nInfoBasic.sName.Set(acpl::String::Ref(oDe.d_name, true));
			
			switch (oDe.d_type)
			{
				case DT_REG:
					nInfoBasic.sTypeRef = acpl::Dir::Entry::tFile;
					break;
				
				case DT_DIR:
					nInfoBasic.sTypeRef = acpl::Dir::Entry::tDir;
					break;
				
				case DT_LNK:
					nInfoBasic.sTypeRef = acpl::Dir::Entry::tSymLink;
					break;
				
				case DT_FIFO:
					nInfoBasic.sTypeRef = acpl::Dir::Entry::tPipe;
					break;
				
				case DT_SOCK:
					nInfoBasic.sTypeRef = acpl::Dir::Entry::tSocket;
					break;
				
				case DT_CHR:
					nInfoBasic.sTypeRef = acpl::Dir::Entry::tDevChar;
					break;
				
				case DT_BLK:
					nInfoBasic.sTypeRef = acpl::Dir::Entry::tDevBlock;
					break;
				
				default:
					nInfoBasic.sTypeRef = acpl::Dir::Entry::tUnknown;
					break;
			}
			
			nInfoBasic.sIsHidden = (nInfoBasic.sName.GetStr()[0] == '.');
			
			
			if (nStat == true)
			{
				acpl::SizeT oPathLen = nPathMbl.Length(); // with NULL-terminator
				acpl::SizeT oNameLen = 0; // with NULL-terminator
				
				nPathMbl.Data()[nPathMbl.Length() - 1] = '/';
				
				while (oDe.d_name[oNameLen++] != '\0') ; // find NULL-terminator
				
				nPathMbl.Apnd(oDe.d_name, oNameLen); // append with NULL-terminator
				
				acpl::cp::_local_fs_path_stat(nPathMbl, false, nInfoStat);
				
				// Restore the path for reuse
				nPathMbl.SetLength(oPathLen);
				nPathMbl.Data()[nPathMbl.Length() - 1] = '\0';
			}
			
			
			return true;
		}
		
		static inline bool _fs_dir_list_get_first(acpl::cp::_fs_dir_handle_t &nHandle, bool nStat, acpl::Path::String::Holder &nPathMbl, acpl::Dir::Entry::InfoBasic &nInfoBasic, acpl::Dir::Entry::InfoStat &nInfoStat)
		{
			nHandle = opendir(nPathMbl.Data());
			return (nHandle != NULL && acpl::cp::_local_fs_dir_get_next_entry(nHandle, nStat, nPathMbl, nInfoBasic, nInfoStat) == true);
		}
		
		static inline bool _fs_dir_list_get_next(const acpl::cp::_fs_dir_handle_t &nHandle, bool nStat, acpl::Path::String::Holder &nPathMbl, acpl::Dir::Entry::InfoBasic &nInfoBasic, acpl::Dir::Entry::InfoStat &nInfoStat)
		{
			return acpl::cp::_local_fs_dir_get_next_entry(nHandle, nStat, nPathMbl, nInfoBasic, nInfoStat);
		}
		
		static inline void _fs_dir_list_end(acpl::cp::_fs_dir_handle_t &nHandle)
		{
			closedir(nHandle);
			nHandle = NULL;
		}
		
		static inline bool _fs_dir_create(const acpl::Path::String::Holder &nPath, const acpl::Path::Permission &nPerm)
		{
			return (mkdir(nPath.Data(), nPerm.GetValue()) == 0);
		}
		
		static inline bool _fs_dir_remove(const acpl::Path::String::Holder &nPath)
		{
			return (rmdir(nPath.Data()) == 0);
		}
		
		static inline acpl::Dir::LastError _fs_dir_get_last_error(acpl::pi::_fs_dir_get_last_error_context_t)
		{
			switch (errno)
			{
				case 0:
					return acpl::Dir::leNone;
				
				case EPERM:
				case EACCES:
					return acpl::Dir::leNoPerm;
				
				case ELOOP:
					return acpl::Dir::leSymlinkLoop;
				
				case ENAMETOOLONG:
					return acpl::Dir::lePathTooLong;
				
				case EEXIST:
					return acpl::Dir::leExists;
				
				case ENOENT:
					return acpl::Dir::leNonExist;
				
				case ENOTDIR:
					return acpl::Dir::leNotDir;
				
				case ENOTEMPTY:
					return acpl::Dir::leNotEmpty;
				
				default:
					break; // acpl::Dir::leSysErr
			}
			
			return acpl::Dir::leSysErr;
		}
		
		
		// 
		// File
		// 
		
		static inline void _fs_file_init(acpl::File::Handle &nHandle)
		{
			nHandle.uInt = -1;
		}
		
		static inline bool _fs_file_is_open(const acpl::File::Handle &nHandle)
		{
			return (nHandle.uInt != -1);
		}
		
		static inline bool _fs_file_open(acpl::File::Handle &nHandle, const acpl::Path::String::Holder &nPath, acpl::File::AccessMode nMode, bool nTruncate, bool nCreate, const acpl::Path::Permission &nPerm)
		{
			int oFlags = O_LARGEFILE;
			
			// Access mode
			switch (nMode)
			{
				case acpl::File::amNone:
					break;
				
				case acpl::File::amReadOnly:
					oFlags |= O_RDONLY;
					break;
				
				case acpl::File::amWriteOnly:
					oFlags |= O_WRONLY;
					break;
				
				case acpl::File::amReadWrite:
					oFlags |= O_RDWR;
					break;
			}
			
			if (nTruncate == true)
				oFlags |= O_TRUNC;
			
			if (nCreate == true)
				oFlags |= O_CREAT;
			
			return ((nHandle.uInt = open(nPath.Data(), oFlags, nPerm.GetValue())) != -1);
		}
		
		static inline void _fs_file_close(const acpl::File::Handle &nHandle)
		{
			close(nHandle.uInt);
		}
		
		static inline bool _fs_file_unlink(const acpl::Path::String::Holder &nPath)
		{
			return (unlink(nPath.Data()) == 0);
		}
		
		static inline bool _fs_file_getoff(const acpl::File::Handle &nHandle, acpl::OffT &nOffset)
		{
			return ((nOffset = lseek(nHandle.uInt, 0, SEEK_CUR)) != -1);
		}
		
		static inline bool _fs_file_setoff(const acpl::File::Handle &nHandle, acpl::File::Whence nWhence, acpl::OffT nOffset, acpl::OffT &nNewOffset)
		{
			int oWhence = 0;
			switch (nWhence)
			{
				case acpl::File::whStart:
					oWhence = SEEK_SET;
					break;
				
				case acpl::File::whCurrent:
					oWhence = SEEK_CUR;
					break;
				
				case acpl::File::whEnd:
					oWhence = SEEK_END;
					break;
			}
			
			return ((nNewOffset = lseek(nHandle.uInt, nOffset, oWhence)) != -1);
		}
		
		static inline bool _fs_file_write(const acpl::File::Handle &nHandle, const void *nData, acpl::SizeT nDataSize, acpl::SizeT &nBytesWritten)
		{
			nBytesWritten = static_cast<acpl::SizeT>(write(nHandle.uInt, nData, nDataSize));
			
			if (static_cast<acpl::SSizeT>(nBytesWritten) >= 0)
				return true;
			else
			{
				nBytesWritten = 0;
				return false;
			}
		}
		
		static inline bool _fs_file_read(const acpl::File::Handle &nHandle, void *nData, acpl::SizeT nDataSize, acpl::SizeT &nBytesRead)
		{
			nBytesRead = static_cast<acpl::SizeT>(read(nHandle.uInt, nData, nDataSize));
			
			if (static_cast<acpl::SSizeT>(nBytesRead) >= 0)
				return true;
			else
			{
				nBytesRead = 0;
				return false;
			}
		}
		
		static inline acpl::File::LastError _fs_file_get_last_error(acpl::pi::_fs_file_get_last_error_context_t nGlec)
		{
			switch (errno)
			{
				case 0:
					return acpl::File::leNone;
				
				// NOTE: currently, there is no support for files opened with O_NONBLOCK
				// case EAGAIN: // equivalent to EWOULDBLOCK
				
				case EINVAL:
					if (nGlec == acpl::pi::_fs_file_glec_write)
						return acpl::File::leNoWrite;
					else
					if (nGlec == acpl::pi::_fs_file_glec_read)
						return acpl::File::leNoRead;
					else
					if (nGlec == acpl::pi::_fs_file_glec_setoff)
						return acpl::File::leInvalOff;
					else
						break; // acpl::File::leSysErr
				
				case EBADF: // if file descriptor is invalid/unopened the front has already checked it
					if (nGlec == acpl::pi::_fs_file_glec_write)
						return acpl::File::leNoWrite;
					else
					if (nGlec == acpl::pi::_fs_file_glec_read)
						return acpl::File::leNoRead;
					else
						break; // acpl::File::leSysErr
				
				case EPERM:
				case EACCES:
				case EROFS: // read-only FS
					return acpl::File::leNoPerm;
				
				case ELOOP:
					return acpl::File::leSymlinkLoop;
				
				case ENAMETOOLONG:
					return acpl::File::lePathTooLong;
				
				case EEXIST:
					return acpl::File::leExists;
				
				case ENOENT:
					return acpl::File::leNonExist;
				
				case EISDIR:
					return acpl::File::leIsDir;
				
				case ENOTDIR:
					return acpl::File::leNotDir;
				
				case EBUSY:
				case ETXTBSY:
					return acpl::File::leInUse;
				
				case EOVERFLOW:
					return acpl::File::leTooLarge;
				
				default:
					break; // acpl::File::leSysErr
			}
			
			return acpl::File::leSysErr;
		}
	}
	
}


#endif // ACPL_CP_GLIBC_FILESYS_INC_CPP
