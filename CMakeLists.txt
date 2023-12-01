cmake_minimum_required(VERSION 3.11)
project(GPTimeSender CXX)
set(CMAKE_CXX_STANDARD 20)

set(CONAN_BUILD_INFO "${CMAKE_BINARY_DIR}/conanbuildinfo.cmake")
if(EXISTS ${CONAN_BUILD_INFO})
	include(${CONAN_BUILD_INFO})
	conan_basic_setup(TARGETS)
else()
	# Если файла нет, то выполняем команды конфигурации conan
	if(CMAKE_BUILD_TYPE STREQUAL "Release")
		execute_process(COMMAND conan install .. --build=missing -s build_type=Release -s compiler.libcxx=libstdc++11)
	elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
		execute_process(COMMAND conan install .. --build=missing -s build_type=Debug -s compiler.libcxx=libstdc++11)
	endif()
endif()

add_executable(gp_time_sender
	src/utest/test.cpp
	src/utest/test.h

	src/json/json.cpp
	src/json/json.h

	src/math/math.cpp
	src/math/math.h

	src/sql/sql.cpp
	src/sql/sql.h

	src/sender/sender.cpp
	src/sender/sender.h

	src/com/com_impl.cpp
	src/com/com_line.h
	src/com/com_args.h

	src/app/application.cpp
	src/app/application.h
	src/app/domain.h
	
	src/main.cpp
)

link_directories(${CONAN_LIB_DIRS_LIBCURL})

target_include_directories(gp_time_sender PUBLIC CONAN_PKG::libcurl) 
target_link_libraries(gp_time_sender CONAN_PKG::libcurl) 

link_directories(${CONAN_LIB_DIRS_SQLITE3})

target_include_directories(gp_time_sender PUBLIC CONAN_PKG::sqlite3) 
target_link_libraries(gp_time_sender CONAN_PKG::sqlite3) 