# 
# Macro to process build type
# 
macro(acpl_process_build_type N_DEFAULT_BUILD_TYPE)
	if (DEFINED CMAKE_CONFIGURATION_TYPES)
		message(STATUS "Build type disabled; multi-configuration mode")
	else()
		set(O_BT_MATCH_REGEX "^([Dd][Ee][Bb][Uu][Gg]|[Rr][Ee][Ll][Ee][Aa][Ss][Ee]|[Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
		
		if(NOT ${N_DEFAULT_BUILD_TYPE} MATCHES ${O_BT_MATCH_REGEX})
			message(FATAL_ERROR "Unrecognized provided default build type: \"${N_DEFAULT_BUILD_TYPE}\" (possible values are: \"Debug\", \"Release\", \"RelWithDebInfo\")")
		endif()
		
		if(DEFINED CMAKE_BUILD_TYPE AND NOT ${CMAKE_BUILD_TYPE} MATCHES "^$")
			if(${CMAKE_BUILD_TYPE} MATCHES ${O_BT_MATCH_REGEX})
				set(CMAKE_BUILD_TYPE "${CMAKE_BUILD_TYPE}" CACHE STRING "Choose the type of build, options are: Debug, Release, RelWithDebInfo." FORCE)
				message(STATUS "Build type: \"${CMAKE_BUILD_TYPE}\" (specified)")
			else()
				message(FATAL_ERROR "Unrecognized build type: \"${CMAKE_BUILD_TYPE}\" (possible values are: \"Debug\", \"Release\", \"RelWithDebInfo\")")
			endif()
		else()
			set(CMAKE_BUILD_TYPE ${N_DEFAULT_BUILD_TYPE} CACHE STRING "Choose the type of build, options are: Debug, Release, RelWithDebInfo." FORCE)
			message(STATUS "Build type: \"${CMAKE_BUILD_TYPE}\" (default)")
		endif()
	endif()
endmacro(acpl_process_build_type)


# 
# Macro to create an empty file (if it does not exist) with N_NAME file name in N_DIR directory
# 
macro(acpl_create_file N_NAME N_DIR)
	set(O_FILE_FOUND "O_FILE_FOUND-NOTFOUND")
	find_file(O_FILE_FOUND ${N_NAME} ${N_DIR})
	if(NOT O_FILE_FOUND)
		message(STATUS "File not found; creating: ${N_DIR}/${N_NAME}")
		file(WRITE "${N_DIR}/${N_NAME}" "")
	endif()
endmacro(acpl_create_file)
