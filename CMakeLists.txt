cmake_minimum_required(VERSION 3.10)

project(rtsr VERSION 1.0)
add_executable(${PROJECT_NAME}
	source/display.c
	source/geometry.c
	source/main.c
	source/mesh.c
	source/salloc.c
	source/triangle.c
	source/vector.c
	C:/devel/c-dsna/source/Array.c
    )

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

include_directories(C:/devel/vcpkg/installed/x64-windows/include)
include_directories(C:/devel/c-dsna/include)

target_link_libraries(${PROJECT_NAME} C:/devel/vcpkg/installed/x64-windows/lib/SDL2.lib)
