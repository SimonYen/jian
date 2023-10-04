cmake_minimum_required(VERSION 3.20.0)

project(
	"jian"
	VERSION 0.1
	)

#添加编译参数 pthread
set(CMAKE_CXX_FLAGS -pthread)

#添加源文件
aux_source_directory(${PROJECT_SOURCE_DIR}/src SRC)
#添加头文件
include_directories(${PROJECT_SOURCE_DIR}/include)
#编译动态库
add_library(${PROJECT_NAME} SHARED ${SRC})
#设置库的输出路径
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)


add_executable(server server.cpp ${SRC})
add_executable(client client.cpp ${SRC})