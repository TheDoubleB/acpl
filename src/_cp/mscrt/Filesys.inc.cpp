#ifndef ACPL_CP_MSCRT_FILESYS_INC_CPP
#define ACPL_CP_MSCRT_FILESYS_INC_CPP

#include <windows.h>
#include <sys/stat.h>
#include <io.h>
#include <direct.h>

#include "../../fs/Dir.h"
#include "../../fs/File.h"

namespace acpl
{
	
	namespace cp
	{
		// 
		// Global
		// 
		
		typedef WIN32_FIND_DATAW _local_fs_dir_entry_t; // needs to be the DATAW one because we're dealing with WCHAR paths
		typedef HANDLE _fs_dir_handle_t;
		
		static inline bool _local_fs_str_len(const WCHAR *nStr, acpl::SizeT &nLen) // <<< NOTE: This function includes the NULL terminator in the nLen as well !!! ; returns true if nStr is non-NULL
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
		
		static inline bool _local_path_char_is_delim(acpl::Pathchar nChr) // true if char is a delimiter
		{
			return (nChr == L'\\' || nChr == L'/');
		}
		
		static inline bool _local_path_char_is_drive(acpl::Pathchar nChr) // true if char is a drive letter
		{
			return ((nChr <= L'Z' && nChr >= L'A') || (nChr <= L'z' && nChr >= L'a'));
		}
		
		static inline bool _local_path_is_absolute(const acpl::Path::String::Holder &nPath, bool nCheckIsRootDir)
		{
			// Absolute dir starts with:
			// 	* OPT1="${DRIVE}:\\"
			// 	* OPT2="${DRIVE}:/"
			// 	* OPT3="\\"
			// 	* OPT4="/"
			// 	* OPT5="\\\\${SERVNAME}"
			
			if (nPath.Length() > 0)
			{
				if (nPath.Data()[0] == L'/')
				{
					// OPT4
					return (nCheckIsRootDir == false || nPath.Length() == 1 || nPath.Data()[1] == L'\0');
				}
				else
				if (nPath.Data()[0] == L'\\')
				{
					if (nPath.Length() > 1 && nPath.Data()[1] != L'\0')
					{
						if (nPath.Data()[1] == L'\\')
						{
							if (nPath.Length() > 2 && nPath.Data()[2] != L'\0') // <<< NOTE: This if needs to be nested!
							{
								if (nCheckIsRootDir == false)
								{
									// OPT5 (root or non-root)
									return true;
								}
								else
								{
									acpl::SizeT oCurPos = 2;
									while (oCurPos < nPath.Length())
									{
										if (nPath.Data()[oCurPos] == L'\0')
										{
											// OPT5 (root-only)
											return true;
										}
										else
										if (acpl::cp::_local_path_char_is_delim(nPath.Data()[oCurPos]) == true)
										{
											return false;
										}
										oCurPos++;
									}
									
									// OPT5 (root-only)
									return true;
								}
							}
						}
						else
						{
							// OPT3 (non-root)
							return (nCheckIsRootDir == false);
						}
					}
					else
					{
						// OPT3 (root-only)
						return true;
					}
				}
				else
				if (acpl::cp::_local_path_char_is_drive(nPath.Data()[0]) == true &&
					nPath.Length() > 2 &&
					nPath.Data()[1] == L':' &&
					acpl::cp::_local_path_char_is_delim(nPath.Data()[2]) == true)
				{
					// OPT1 or OPT2
					return (nCheckIsRootDir == false || nPath.Length() == 3 || nPath.Data()[3] == L'\0');
				}
			}
			
			return false;
		}
		
		static inline bool _local_fs_path_stat(const acpl::Path::String::Holder &nPath, acpl::Dir::Entry::InfoStat &nInfoStat)
		{
			struct __stat64 oSs;
			if (_wstat64(nPath.Data(), &oSs) == 0)
			{
				nInfoStat.sIsSet = true;
				
				switch ((oSs.st_mode & _S_IFMT))
				{
					case _S_IFREG:
						nInfoStat.sType = acpl::Dir::Entry::tFile;
						break;
					
					case _S_IFDIR:
						nInfoStat.sType = acpl::Dir::Entry::tDir;
						break;
					
					case _S_IFCHR:
						nInfoStat.sType = acpl::Dir::Entry::tDevChar;
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
			return acpl::cp::_local_path_is_absolute(nPath, false);
		}
		
		static inline bool _fs_path_is_root_dir(const acpl::Path::String::Holder &nPath)
		{
			return acpl::cp::_local_path_is_absolute(nPath, true);
		}
		
		static inline bool _fs_path_char_is_delim(acpl::Pathchar nChr)
		{
			return acpl::cp::_local_path_char_is_delim(nChr);
		}
		
		static inline acpl::SizeT _fs_path_get_dir_delim_len()
		{
			return 1;
		}
		static inline const acpl::Pathchar *_fs_path_get_dir_delim()
		{
			return L"\\";
		}
		
		static inline acpl::SizeT _fs_path_get_current_dir_len()
		{
			return 1;
		}
		static inline const acpl::Pathchar *_fs_path_get_current_dir()
		{
			return L".";
		}
		
		static inline acpl::SizeT _fs_path_get_parent_dir_len()
		{
			return 2;
		}
		static inline const acpl::Pathchar *_fs_path_get_parent_dir()
		{
			return L"..";
		}
		
		static inline bool _fs_path_elem_is_current_dir(const acpl::Path::String::Holder &nPath, acpl::SizeT nOff)
		{
			return (nPath.Length() > nOff && nPath.Data()[nOff++] == L'.' && (nPath.Length() == nOff || nPath.Data()[nOff] == L'\0' || acpl::cp::_local_path_char_is_delim(nPath.Data()[nOff]) == true)); // <<< NOTE: '++' with nOff
		}
		
		static inline bool _fs_path_elem_is_parent_dir(const acpl::Path::String::Holder &nPath, acpl::SizeT nOff)
		{
			return (nPath.Length() > (nOff + 1) && nPath.Data()[nOff++] == L'.' && nPath.Data()[nOff++] == L'.' && (nPath.Length() == nOff || nPath.Data()[nOff] == L'\0' || acpl::cp::_local_path_char_is_delim(nPath.Data()[nOff]) == true)); // <<< NOTE: '++' with nOff
		}
		
		static inline acpl::SizeT _fs_path_elem_rel_with_drive_offset(const acpl::Path::String::Holder &nPath)
		{
			// Only applies to MSCRT and relative paths containing drive letter as well (e.g.: "C:relative\\path")
			if (nPath.Length() > 1 && acpl::cp::_local_path_char_is_drive(nPath.Data()[0]) == true && nPath.Data()[1] == L':' && (nPath.Length() == 2 ||  acpl::cp::_local_path_char_is_delim(nPath.Data()[2]) != true))
				return 2;
			else
				return 0;
		}
		
		static inline bool _fs_path_resolve(acpl::Path::String::Holder &nPath)
		{
			WCHAR *oAbsPath;
			if (acpl::Mem::Alloc(oAbsPath, 4096) != true)
				return false;
			
			acpl::cp::_local_fs_dir_entry_t oDe;
			acpl::cp::_fs_dir_handle_t oHandle;
			
			if (GetFullPathNameW(nPath.Data(), 4096, oAbsPath, NULL) > 0 && (oHandle = FindFirstFileW(nPath.Data(), &oDe)) != INVALID_HANDLE_VALUE)
			{
				FindClose(oHandle);
				
				acpl::SizeT oLen = 0;
				acpl::cp::_local_fs_str_len(oAbsPath, oLen);
				
				nPath.Clear().Apnd(oAbsPath, oLen);
				
				acpl::Mem::Free(oAbsPath);
				return true;
			}
			else
			{
				acpl::Mem::Free(oAbsPath);
				return false;
			}
		}
		
		static inline bool _fs_path_setperm(const acpl::Path::String::Holder &nPath, const acpl::Path::Permission &nPerm)
		{
			return (_wchmod(nPath.Data(), nPerm.GetValue()) == 0);
		}
		
		static inline void _fs_path_umask(acpl::UInt16 nVal)
		{
			int oOldMode;
			_umask_s(nVal, &oOldMode);
		}
		
		static inline bool _fs_path_stat(const acpl::Path::String::Holder &nPath, bool, acpl::Dir::Entry::InfoStat &nInfoStat)
		{
			return acpl::cp::_local_fs_path_stat(nPath, nInfoStat);
		}
		
		static inline acpl::Path::LastError _fs_path_get_last_error(acpl::pi::_fs_path_get_last_error_context_t)
		{
			switch(GetLastError())
			{
				case 0:
					return acpl::Path::leNone;
				
				case ERROR_ACCESS_DENIED:
					return acpl::Path::leNoPerm;
				
				case ERROR_BUFFER_OVERFLOW: // NOTE: maybe not this one? to be decided
				case ERROR_FILENAME_EXCED_RANGE:
					return acpl::Path::leTooLong;
				
				case ERROR_FILE_NOT_FOUND:
					return acpl::Path::leNonExist;
				
				case ERROR_PATH_NOT_FOUND:
				case ERROR_DIRECTORY:
					return acpl::Path::leNotDir;
				
				default:
					break; // acpl::Path::leSysErr
			}
			
			return acpl::Path::leSysErr;
		}
		
		
		// 
		// Dir
		// 
		
		static inline bool _local_fs_dir_entry_proc(const acpl::cp::_local_fs_dir_entry_t &nDe, bool nStat, acpl::Path::String::Holder &nPathMbl, acpl::Dir::Entry::InfoBasic &nInfoBasic, acpl::Dir::Entry::InfoStat &nInfoStat)
		{
			nInfoBasic.sName.Set(nDe.cFileName);
			
			if ((nDe.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0)
				nInfoBasic.sTypeRef = acpl::Dir::Entry::tDir;
			else
				nInfoBasic.sTypeRef = acpl::Dir::Entry::tFile;
			
			nInfoBasic.sIsHidden = ((nDe.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) != 0);
			
			if (nStat == true)
			{
				acpl::SizeT oPathLen = nPathMbl.Length(); // with NULL-terminator
				acpl::SizeT oNameLen = 0; // with NULL-terminator
				
				nPathMbl.Data()[nPathMbl.Length() - 1] = '\\';
				
				while (nDe.cFileName[oNameLen++] != '\0') ; // find NULL-terminator
				
				nPathMbl.Apnd(nDe.cFileName, oNameLen); // append with NULL-terminator
				
				acpl::cp::_local_fs_path_stat(nPathMbl, nInfoStat);
				
				// Rrestore the path for reuse
				nPathMbl.SetLength(oPathLen);
				nPathMbl.Data()[nPathMbl.Length() - 1] = '\0';
			}
			
			return true;
		}
		
		static inline bool _fs_dir_list_get_first(acpl::cp::_fs_dir_handle_t &nHandle, bool nStat, acpl::Path::String::Holder &nPathMbl, acpl::Dir::Entry::InfoBasic &nInfoBasic, acpl::Dir::Entry::InfoStat &nInfoStat)
		{
			acpl::cp::_local_fs_dir_entry_t oDe;
			
			if ((nHandle = FindFirstFileW(nPathMbl.Data(), &oDe)) == INVALID_HANDLE_VALUE) // Checks whether the path even exists
				return false;
			FindClose(nHandle);
			
			nPathMbl.CutRight(1);
			nPathMbl.Apnd(L"\\*", 3);
			
			nHandle = FindFirstFileW(nPathMbl.Data(), &oDe);
			
			// Restore to only the path
			nPathMbl.CutRight(3);
			nPathMbl.Apnd(L"", 1);
			
			return (nHandle != INVALID_HANDLE_VALUE && acpl::cp::_local_fs_dir_entry_proc(oDe, nStat, nPathMbl, nInfoBasic, nInfoStat) == true);
		}
		
		static inline bool _fs_dir_list_get_next(const acpl::cp::_fs_dir_handle_t &nHandle, bool nStat, acpl::Path::String::Holder &nPathMbl, acpl::Dir::Entry::InfoBasic &nInfoBasic, acpl::Dir::Entry::InfoStat &nInfoStat)
		{
			acpl::cp::_local_fs_dir_entry_t oDe;
			return (FindNextFileW(nHandle, &oDe) != 0 && acpl::cp::_local_fs_dir_entry_proc(oDe, nStat, nPathMbl, nInfoBasic, nInfoStat) == true);
		}
		
		static inline void _fs_dir_list_end(acpl::cp::_fs_dir_handle_t &nHandle)
		{
			FindClose(nHandle);
			nHandle = INVALID_HANDLE_VALUE;
		}
		
		static inline bool _fs_dir_create(const acpl::Path::String::Holder &nPath, const acpl::Path::Permission &nPerm)
		{
			return (_wmkdir(nPath.Data()) == 0);
		}
		
		static inline bool _fs_dir_remove(const acpl::Path::String::Holder &nPath)
		{
			return (_wrmdir(nPath.Data()) == 0);
		}
		
		static inline acpl::Dir::LastError _fs_dir_get_last_error(acpl::pi::_fs_dir_get_last_error_context_t)
		{
			switch(GetLastError())
			{
				case 0:
					return acpl::Dir::leNone;
				
				case ERROR_NO_MORE_FILES:
					return acpl::Dir::leNone;
				
				case ERROR_ACCESS_DENIED:
					return acpl::Dir::leNoPerm;
				
				case ERROR_BUFFER_OVERFLOW: // NOTE: maybe not this one? to be decided
				case ERROR_FILENAME_EXCED_RANGE:
					return acpl::Dir::lePathTooLong;
				
				case ERROR_ALREADY_EXISTS:
					return acpl::Dir::leExists;
				
				case ERROR_FILE_NOT_FOUND:
					return acpl::Dir::leNonExist;
				
				case ERROR_PATH_NOT_FOUND:
				case ERROR_DIRECTORY:
					return acpl::Dir::leNotDir;
				
				case ERROR_DIR_NOT_EMPTY:
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
			nHandle.uVptr = INVALID_HANDLE_VALUE;
		}
		
		static inline bool _fs_file_is_open(const acpl::File::Handle &nHandle)
		{
			return (nHandle.uVptr != INVALID_HANDLE_VALUE);
		}
		
		static inline bool _fs_file_open(acpl::File::Handle &nHandle, const acpl::Path::String::Holder &nPath, acpl::File::AccessMode nMode, bool nTruncate, bool nCreate, const acpl::Path::Permission &/*nPerm*/)
		{
			DWORD oAccess = 0;
			switch (nMode)
			{
				case acpl::File::amNone:
					break;
				
				case acpl::File::amReadOnly:
					oAccess = GENERIC_READ;
					break;
				
				case acpl::File::amWriteOnly:
					oAccess =  GENERIC_WRITE;
					break;
				
				case acpl::File::amReadWrite:
					oAccess = (GENERIC_READ | GENERIC_WRITE);
					break;
			}
			
			DWORD oCreatDisp = 0;
			if (nTruncate == true)
				oCreatDisp = ((nCreate == true) ? CREATE_ALWAYS : TRUNCATE_EXISTING);
			else
			if (nCreate == true)
				oCreatDisp = OPEN_ALWAYS;
			else
				oCreatDisp = OPEN_EXISTING;
			
			return ((nHandle.uVptr = CreateFileW(nPath.Data(), oAccess, 0, NULL, oCreatDisp, 0, NULL)) != INVALID_HANDLE_VALUE);
		}
		
		static inline void _fs_file_close(const acpl::File::Handle &nHandle)
		{
			CloseHandle(nHandle.uVptr);
		}
		
		static inline bool _fs_file_unlink(const acpl::Path::String::Holder &nPath)
		{
			return (DeleteFileW(nPath.Data()) != 0);
		}
		
		static inline bool _fs_file_getoff(const acpl::File::Handle &nHandle, acpl::OffT &nOffset)
		{
			LARGE_INTEGER oOffIn, oOffOut;
			oOffIn.QuadPart = 0;
			
			if (SetFilePointerEx(nHandle.uVptr, oOffIn, &oOffOut, FILE_CURRENT) != 0)
			{
				nOffset = oOffOut.QuadPart;
				return true;
			}
			else
				return false;
		}
		
		static inline bool _fs_file_setoff(const acpl::File::Handle &nHandle, acpl::File::Whence nWhence, acpl::OffT nOffset, acpl::OffT &nNewOffset)
		{
			DWORD oWhence = 0;
			switch (nWhence)
			{
				case acpl::File::whStart:
					oWhence = FILE_BEGIN;
					break;
				
				case acpl::File::whCurrent:
					oWhence = FILE_CURRENT;
					break;
				
				case acpl::File::whEnd:
					oWhence = FILE_END;
					break;
			}
			
			LARGE_INTEGER oOffIn, oOffOut;
			oOffIn.QuadPart = nOffset;
			
			if (SetFilePointerEx(nHandle.uVptr, oOffIn, &oOffOut, oWhence) != 0)
			{
				nNewOffset = oOffOut.QuadPart;
				return true;
			}
			else
				return false;
		}
		
		static inline bool _fs_file_write(const acpl::File::Handle &nHandle, const void *nData, acpl::SizeT nDataSize, acpl::SizeT &nBytesWritten)
		{
			DWORD oBytesWritten;
			if (WriteFile(nHandle.uVptr, nData, nDataSize, &oBytesWritten, NULL) != 0)
			{
				nBytesWritten = oBytesWritten;
				return true;
			}
			else
			{
				nBytesWritten = 0;
				return false;
			}
		}
		
		static inline bool _fs_file_read(const acpl::File::Handle &nHandle, void *nData, acpl::SizeT nDataSize, acpl::SizeT &nBytesRead)
		{
			DWORD oBytesRead;
			if (ReadFile(nHandle.uVptr, nData, nDataSize, &oBytesRead, NULL) != 0)
			{
				nBytesRead = oBytesRead;
				return true;
			}
			else
			{
				nBytesRead = 0;
				return false;
			}
		}
		
		static inline acpl::File::LastError _fs_file_get_last_error(acpl::pi::_fs_file_get_last_error_context_t nGlec)
		{
			switch(GetLastError())
			{
				case 0:
					return acpl::File::leNone;
				
				case ERROR_ACCESS_DENIED:
					if (nGlec == acpl::pi::_fs_file_glec_write)
						return acpl::File::leNoWrite;
					else
					if (nGlec == acpl::pi::_fs_file_glec_read)
						return acpl::File::leNoRead;
					else
						return acpl::File::leNoPerm;
				
				case ERROR_BUFFER_OVERFLOW: // NOTE: maybe not this one? to be decided
				case ERROR_FILENAME_EXCED_RANGE:
					return acpl::File::lePathTooLong;
				
				case ERROR_FILE_EXISTS:
				case ERROR_ALREADY_EXISTS:
					return acpl::File::leExists;
				
				case ERROR_FILE_NOT_FOUND:
					return acpl::File::leNonExist;
				
				case ERROR_SEEK:
				case ERROR_NEGATIVE_SEEK:
				case ERROR_SEEK_ON_DEVICE:
					return acpl::File::leInvalOff;
				
				default:
					break; // acpl::File::leSysErr
			}
			
			return acpl::File::leSysErr;
		}
	}
	
}


#endif // ACPL_CP_MSCRT_FILESYS_INC_CPP
