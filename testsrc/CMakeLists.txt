# 
# Sources
# 

set(TEST_SOURCE_FILES "")
macro(add_source_file SOURCE_FILE)
	set(TEST_SOURCE_FILES ${TEST_SOURCE_FILES} "${SOURCE_FILE}")
endmacro(add_source_file)

add_source_file("Main.cpp")
add_source_file("TestsAdhoc.cpp")
add_source_file("TestsArray.cpp")
add_source_file("TestsBits.cpp")
add_source_file("TestsCharsets.cpp")
add_source_file("TestsDefines.cpp")
add_source_file("TestsEndian.cpp")
add_source_file("TestsFsDir.cpp")
add_source_file("TestsFsFile.cpp")
add_source_file("TestsFsPath.cpp")
add_source_file("TestsIo.cpp")
add_source_file("TestsMemory.cpp")
add_source_file("TestsNetAddr.cpp")
add_source_file("TestsNetHeader.cpp")
add_source_file("TestsNetPoll.cpp")
add_source_file("TestsNetSock.cpp")
add_source_file("TestsRaii.cpp")
add_source_file("TestsString.cpp")
add_source_file("TestsThreads.cpp")
add_source_file("TestsTime.cpp")
add_source_file("TestsTypes.cpp")
add_source_file("TestsUri.cpp")


# 
# Building
# 

add_executable(testacpl ${TEST_SOURCE_FILES})

if (BUILD_SHARED_LIBS)
	target_link_libraries(testacpl acpl)
else (BUILD_SHARED_LIBS)
	target_link_libraries(testacpl acpl ${ACPL_DEPLIBRARIES})
endif (BUILD_SHARED_LIBS)
