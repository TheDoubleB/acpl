if (MSVC)
	set(ACPL_DEPLIBRARIES "winmm" "ws2_32")
else()
	find_package(Threads REQUIRED)
	set(ACPL_DEPLIBRARIES "rt" "${CMAKE_THREAD_LIBS_INIT}")
endif()


find_path(ACPL_INCLUDE_DIR acpl
		/usr/include
		/usr/local/include
		/usr/pkg/include
		${CMAKE_INSTALL_PREFIX}/include
	)

find_library(ACPL_LIBRARY acpl
		/usr/lib
		/usr/local/lib
		/usr/pkg/lib
		${CMAKE_INSTALL_PREFIX}/lib
	)


if(ACPL_INCLUDE_DIR AND ACPL_LIBRARY)
   set(ACPL_FOUND TRUE)
endif()


if(ACPL_FOUND)
	
	if(NOT ACPL_FIND_QUIETLY)
		message(STATUS "Found acpl: ${ACPL_LIBRARY}")
	endif()
	
	# Add additional include dirs
	# No additional include dirs required
	
	# Add additional libraries
	# TODO: Should ACPL_DEPLIBRARIES even be included in ACPL_LIBRARY? If you are linking it statically, you should specify deps separately.
	set(ACPL_LIBRARY ${ACPL_LIBRARY} ${ACPL_DEPLIBRARIES})
	
else()
	
	if(ACPL_FIND_REQUIRED)
		message(FATAL_ERROR "Could not find acpl")
	endif()
	
endif()

