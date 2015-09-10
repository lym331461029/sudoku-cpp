cmake_minimum_required(VERSION 2.6)


set(SUDOKU_SRC
	Sudoku.cpp
	main.cpp
	SudokuElem.cpp
)

add_executable(sudoku ${SUDOKU_SRC})
set(EXECUTABLE_OUTPUT_PATH .)

set(CXX_FLAGS
 -g
 # -DVALGRIND
 # -DMUDUO_STD_STRING
 #-DCHECK_PTHREAD_RETURN_VALUE
 #-D_FILE_OFFSET_BITS=64
 #-Wall
 #-Wextra
 #-Werror
 #-Wconversion
 -Wno-unused-parameter
 -Wold-style-cast
 -Woverloaded-virtual
 -Wpointer-arith
 -Wshadow
 -Wwrite-strings
 -march=native
 # -MMD
 # -std=c++0x
 -rdynamic
 )


string(REPLACE ";" " " CMAKE_CXX_FLAGS "${CXX_FLAGS}")
set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_FLAGS_DEBUG "-O0")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -finline-limit=1000 -DNDEBUG")

#include_directories(${APPPATH}/include)


find_library(BOOST_THREAD NAMES boost_thread )
find_library(BOOST_SYSTEM NAMES boost_system )


target_link_libraries(sudoku pthread dl rt
	${BOOST_THREAD}
	${BOOST_SYSTEM}
	)


