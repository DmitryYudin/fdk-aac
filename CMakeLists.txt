cmake_minimum_required(VERSION 3.14)

project(fdk-aac)

source_group(src REGULAR_EXPRESSION ".*\\.[ch].*")

if(CMAKE_CONFIGURATION_TYPES) # https://stackoverflow.com/questions/31661264/cmake-generators-for-visual-studio-do-not-set-cmake-configuration-types
    set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Debug/Release only" FORCE)
endif()

#
# Library
#
add_library(fdk-aac STATIC)

foreach(X IN ITEMS
	libAACdec
	libAACenc
	libArithCoding
	libDRCdec
	libFDK
	libMpegTPDec
	libMpegTPEnc
	libPCMutils
	libSACdec
	libSACenc
	libSBRdec
	libSBRenc
	libSYS 
)
	# Pick up with absolute path name. Otherwise target_sources() will fail to constuct folder structure
	# when this project included into the other project as dependency (i.e. through add_subdirectory(...))
	file(GLOB ${X}_SRC "${X}/src/*.[hc]*" "${X}/include/*.[hc]*")
	source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/${X} PREFIX ${X} FILES ${${X}_SRC})

	target_include_directories(fdk-aac PRIVATE ${X}/include)
	target_sources(fdk-aac PRIVATE ${${X}_SRC})

	if(${CMAKE_PROJECT_NAME} STREQUAL ${PROJECT_NAME})
		list(LENGTH ${X}_SRC numFiles)
		message(STATUS "${X} - ${numFiles} file(s) added")
	endif()
endforeach()

target_include_directories(fdk-aac PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/libAACenc/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/libAACdec/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/libSYS/include>
)

#
# Tests
#
if(${CMAKE_PROJECT_NAME} STREQUAL ${PROJECT_NAME})
	message(STATUS "Using preconfigured FFmpeg library")
	add_subdirectory(../ffmpeg-builder third_party/ffmpeg)

    set(aac-enc_SRC aac-enc.c wavreader.c)
    set(aac-dec_SRC aac-dec.c wavwriter.c)
    set(test-encode-decode_SRC test-encode-decode.c wavreader.c)
    set(m4a-dec_SRC m4a-dec wavwriter.c)
    set(m4a-enc_SRC m4a-enc wavreader.c)

	foreach(X IN ITEMS
    	aac-enc
	    aac-dec
    	test-encode-decode
	    m4a-dec
    	m4a-enc
	)
	    add_executable(${X})
	    target_sources(${X} PRIVATE ${${X}_SRC})
	    target_include_directories(${X} PRIVATE win32)
	    target_compile_definitions(${X} PRIVATE _CRT_SECURE_NO_WARNINGS)
	    target_link_libraries(${X} fdk-aac)
	endforeach()

    target_link_libraries(m4a-dec libavformat libavcodec libavutil)
    target_link_libraries(m4a-enc libavformat libavcodec libavutil)
endif()
