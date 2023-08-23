cmake_minimum_required(VERSION 3.10)

set(ProjectNameParam "DefaultProject" CACHE STRING "Command line argument containing the project name.")
project(${ProjectNameParam} VERSION 1.0 LANGUAGES C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

add_executable(${PROJECT_NAME}
	source/display.c
	source/geometry.c
	source/main.c
	source/mesh.c
	source/salloc.c
	source/triangle.c
	source/vector.c
)
include_directories(source/)

add_subdirectory(extern)
target_link_libraries(${PROJECT_NAME} PUBLIC haikal)

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
    # VCPKG
    set_target_properties(${PROJECT_NAME} PROPERTIES VS_GLOBAL_VcpkgEnabled true)
    # SDL2
    find_package(SDL2 CONFIG REQUIRED)
    target_link_libraries(${PROJECT_NAME}
            PRIVATE
            $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
            $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
    )
    # target_link_libraries(${PROJECT_NAME} PRIVATE ${SDL2_LIBRARIES})
    find_package(sdl2-gfx CONFIG REQUIRED)
    target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2_gfx)
endif()

if (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
    # Find and link SDL2
    find_package(SDL2 REQUIRED)
    target_include_directories(${PROJECT_NAME} PRIVATE ${SDL2_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})
    # Find and link SDL2_gfx
    find_package(SDL2_gfx REQUIRED)
    target_include_directories(${PROJECT_NAME} PRIVATE ${SDL2_GFX_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} ${SDL2_GFX_LIBRARIES})
endif()

if (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
    # Find and link SDL2
    find_package(SDL2 REQUIRED)
    target_include_directories(${PROJECT_NAME} PRIVATE ${SDL2_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} -lm -ldl ${SDL2_LIBRARIES})
    # Find and link SDL2_gfx
    find_package(SDL2_gfx REQUIRED)
    target_include_directories(${PROJECT_NAME} PRIVATE ${SDL2_GFX_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} -lm -ldl ${SDL2_GFX_LIBRARIES})
endif()
