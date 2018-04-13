cmake_minimum_required(VERSION 3.4)
project(jsoncpp)

set(TARGET_NAME jsoncpp)

#if(CMAKE_CL_64)
#	set(CMAKE_DEBUG_POSTFIX _x64d)
#	set(CMAKE_RELEASE_POSTFIX _x64)
#else()
	set(CMAKE_DEBUG_POSTFIX d)
#endif()

set(src_files
	autolink.h
	config.h
	features.h
	forwards.h
	json.h
	json_batchallocator.h
	json_reader.cpp
	json_internalarray.inl
	json_internalmap.inl
	json_valueiterator.inl
	json_reader.cpp
	json_value.cpp
	json_writer.cpp
	reader.h
	value.h
	writer.h
)
include_directories(.)
add_library(${TARGET_NAME}  ${src_files} )

install(
	TARGETS ${TARGET_NAME}
	ARCHIVE
	DESTINATION lib
)

install(
	FILES autolink.h 
	config.h 
	features.h 
	forwards.h json.h 
	json_batchallocator.h 
	json_internalarray.inl
	json_internalmap.inl
	json_valueiterator.inl
	reader.h
	value.h
	writer.h
	DESTINATION include
)