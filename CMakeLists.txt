# 
# Project definition
# 

project(acpl C CXX)
cmake_minimum_required(VERSION 2.6)

set(CMAKE_MODULE_PATH ${CMAKE_INSTALL_PREFIX}/CMake)

include(AcplUtils.cmake)
acpl_process_build_type("Release")


# 
# Packages
# 

if (NOT MSVC)
	find_package(Threads REQUIRED)
endif()


# 
# Defines
# 

set(ACPL_DEF_BUILDPROC "_acplBuildProcess")

if (BUILD_SHARED_LIBS)
	set(ACPL_DEF_BUILTAS "_acplBuiltAsShared")
else (BUILD_SHARED_LIBS)
	set(ACPL_DEF_BUILTAS "_acplBuiltAsStatic")
endif (BUILD_SHARED_LIBS)


# 
# Compiler flags
# 

if(CMAKE_COMPILER_IS_GNUCC)
	
	add_definitions(-D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 -D${ACPL_DEF_BUILDPROC} -D${ACPL_DEF_BUILTAS})
	
	set(CMAKE_CXX_FLAGS "-pedantic -fPIC -Wall -Wextra -Wnon-virtual-dtor -Woverloaded-virtual -Wno-long-long -Wno-variadic-macros")
	set(CMAKE_CXX_FLAGS_DEBUG "-g3 -gdwarf-2")
	set(CMAKE_CXX_FLAGS_RELEASE "-g -O2")
	set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-g3 -O2 -gdwarf-2")
	
	set(ACPL_DEPLIBRARIES "rt" "${CMAKE_THREAD_LIBS_INIT}")
	
elseif(MSVC)
	
	# WARNING: MSVC DLL linking is not currently supported
	# TODO: Added acplDll{,No}Export tags
	# NOTE: For, DDLs there (might?) have to be /LD{,d} instead of /MT{,d} options
	if (BUILD_SHARED_LIBS)
		message(FATAL_ERROR "Shared (DLL) linking for ACPL is currently not supported.")
	endif (BUILD_SHARED_LIBS)
	
	add_definitions(/D${ACPL_DEF_BUILDPROC} /D${ACPL_DEF_BUILTAS})
	
	# WARNING: All these flags are somehow put together in multi-configuration, what!?
	# TODO: Is it even possible to compile with /Za (ansi)!? Tons of errors in MSVC headers.
	set(CMAKE_CXX_FLAGS "/W3") # /W3 (level 3 warnings; /Wall or even /W4 put out nonesensical warnings)
	set(CMAKE_CXX_FLAGS_DEBUG "/MTd /Zi") # this needs /ZI, not /Zi ?
	set(CMAKE_CXX_FLAGS_RELEASE "/MT /Zi /O2 /GL")
	set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "/MTd /Zi /O2 /GL")
	
	set(ACPL_DEPLIBRARIES "winmm" "ws2_32")
	
else()
	
	message(FATAL_ERROR "Unsupported compiler")
	
endif()


# 
# Sub directories
# 

add_subdirectory(src)
add_subdirectory(testsrc)


# 
# Installation
# 

install(FILES
		AcplUtils.cmake
		FindAcpl.cmake
	DESTINATION ${CMAKE_INSTALL_PREFIX}/CMake)
