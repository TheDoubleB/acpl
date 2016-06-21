# 
# Project
# 

project(ACPL C CXX)
cmake_minimum_required(VERSION 2.8)
string(TOLOWER "${PROJECT_NAME}" PROJECT_NAME_LC)


# 
# Project version
# 

set(ACPL_VERSION_MAJOR 1)
set(ACPL_VERSION_MINOR 0)
set(ACPL_VERSION_PATCH 0)
set(ACPL_VERSION "${ACPL_VERSION_MAJOR}.${ACPL_VERSION_MINOR}.${ACPL_VERSION_PATCH}")


# 
# No in-source building
# 
if("${CMAKE_BINARY_DIR}" STREQUAL "${CMAKE_SOURCE_DIR}")
	file(REMOVE_RECURSE "CMakeFiles" "CMakeCache.txt")
	message(FATAL_ERROR "In-source building not allowed.")
endif()


# 
# 
# 

include("${PROJECT_NAME}Utils.cmake")
acpl_process_build_type("Release")


# 
# Suffixes
# 

set(ACPL_LIB_DEBUG_SUFFIX "d")
if("${CMAKE_SIZEOF_VOID_P}" MATCHES "8")
	set(ACPL_LIB_DIR_SUFFIX "64")
else()
    set(ACPL_LIB_DIR_SUFFIX "")
endif()


# 
# CMake module paths
# 

set(CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH}" "${CMAKE_INSTALL_PREFIX}")
if (WIN32 AND NOT CYGWIN)
	set(ACPL_INSTALL_CMAKE_SUBDIR "CMake")
	set(ACPL_INSTALL_DIR_REL2CMAKE "..")
else()
	set(ACPL_INSTALL_CMAKE_SUBDIR "lib${ACPL_LIB_DIR_SUFFIX}/cmake/${PROJECT_NAME}")
	set(ACPL_INSTALL_DIR_REL2CMAKE "../../..")
endif()


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
	
	set(CMAKE_CXX_FLAGS					"${CMAKE_CXX_FLAGS} -pedantic -fPIC -Wall -Wextra -Wnon-virtual-dtor -Woverloaded-virtual -Wno-long-long -Wno-variadic-macros")
	set(CMAKE_CXX_FLAGS_DEBUG			"${CMAKE_CXX_FLAGS_DEBUG} -g3")
	set(CMAKE_CXX_FLAGS_RELEASE			"${CMAKE_CXX_FLAGS_RELEASE} -g")
	set(CMAKE_CXX_FLAGS_RELWITHDEBINFO	"${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -g3")
	
	set(ACPL_DEPLIBRARIES "rt" "${CMAKE_THREAD_LIBS_INIT}")
	
elseif(MSVC)
	
	# WARNING: MSVC DLL linking is not currently supported
	# TODO: Added acplDll{,No}Export tags
	if (BUILD_SHARED_LIBS)
		message(FATAL_ERROR "Shared (DLL) linking for ACPL is currently not supported.")
	endif (BUILD_SHARED_LIBS)
	
	add_definitions(/D${ACPL_DEF_BUILDPROC} /D${ACPL_DEF_BUILTAS})
	
	# WARNING: All these flags are somehow put together in multi-configuration, what!?
	# TODO: Is it even possible to compile with /Za (ansi)!? Tons of errors in MSVC headers.
	set(CMAKE_CXX_FLAGS					"${CMAKE_CXX_FLAGS} /W3") # /W3 (level 3 warnings; /Wall or even /W4 put out nonesensical warnings)
	set(CMAKE_CXX_FLAGS_DEBUG			"${CMAKE_CXX_FLAGS_DEBUG}")
	set(CMAKE_CXX_FLAGS_RELEASE			"${CMAKE_CXX_FLAGS_RELEASE}")
	set(CMAKE_CXX_FLAGS_RELWITHDEBINFO	"${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")
	
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
# Configures
# 

configure_file("${PROJECT_NAME}Config.cmake.in"        "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"        @ONLY)
configure_file("${PROJECT_NAME}ConfigVersion.cmake.in" "${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake" @ONLY)


# 
# Installation
# 

install(FILES
		"${PROJECT_NAME}Utils.cmake"
		"${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
		"${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
	DESTINATION "${CMAKE_INSTALL_PREFIX}/${ACPL_INSTALL_CMAKE_SUBDIR}")
