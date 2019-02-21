cmake_minimum_required(VERSION 3.13)
project(roguelike)

option(COV "Generate gcov data (in binaries too)" ON)

if (NOT DEFINED GRAPHICS_LIB)
    set(GRAPHICS_LIB basic_console)
endif(NOT DEFINED GRAPHICS_LIB)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

if(COV)
    add_compile_options("--coverage")
    link_libraries("--coverage")
endif(COV)

include_directories(external)

include(external/cmake_modules/CMakeRC.cmake)
include(external/cmake_modules/PVS-Studio.cmake)
find_package(Catch2 REQUIRED)

cmrc_add_resource_library(roguelike-res ALIAS rcfs::rc NAMESPACE rcfs
        default_config.json)

add_subdirectory(graphics)

add_executable(roguelike
        main.cpp

        graphics.h
        graphics.cpp
        map.h map.cpp
        characters.cpp characters.h
        map/types.h
        utils/PRNG.cpp utils/PRNG.h
        characters/types.h
        config.h config.cpp
        config/characters.h config/characters.cpp
        game.cpp game.h
        map/types.cpp
        scenes/game.cpp scenes/game.h
        scenes/base_scene.cpp scenes/base_scene.h
        graphics/kbd_keys.h
        visitors/attack_visitor.cpp visitors/attack_visitor.h
        visitors/wall_visitor.cpp visitors/wall_visitor.h
        visitors/win_cond_visitor.cpp visitors/win_cond_visitor.h)

add_executable(roguelike-tests
        tests/test_main.cpp
        tests/prng.cpp
        tests/config.cpp
        tests/characters.cpp
        tests/map.cpp
        tests/scene_game.cpp
        tests/game.cpp

        graphics.h
        graphics.cpp
        map.h map.cpp
        characters.cpp characters.h
        map/types.h
        utils/PRNG.cpp utils/PRNG.h
        characters/types.h
        config.h config.cpp
        config/characters.h config/characters.cpp
        game.cpp game.h
        map/types.cpp
        scenes/game.cpp scenes/game.h
        scenes/base_scene.cpp scenes/base_scene.h
        graphics/kbd_keys.h
        visitors/attack_visitor.cpp visitors/attack_visitor.h
        visitors/wall_visitor.cpp visitors/wall_visitor.h
        visitors/win_cond_visitor.cpp visitors/win_cond_visitor.h)
file(COPY tests/test_assets DESTINATION ${PROJECT_BINARY_DIR})

target_link_libraries(${PROJECT_NAME} PUBLIC graphics_${GRAPHICS_LIB})
target_link_libraries(${PROJECT_NAME} PRIVATE rcfs::rc)
target_link_libraries(${PROJECT_NAME} PUBLIC stdc++fs)

target_link_libraries(${PROJECT_NAME}-tests PUBLIC graphics_${GRAPHICS_LIB})
target_link_libraries(${PROJECT_NAME}-tests PRIVATE rcfs::rc)
target_link_libraries(${PROJECT_NAME}-tests PUBLIC stdc++fs)
target_link_libraries(${PROJECT_NAME}-tests PUBLIC Catch2::Catch2)

include(CTest)
include(Catch)
catch_discover_tests(roguelike-tests)

if(NOT DEFINED ENV{CI})
    pvs_studio_add_target(TARGET ${PROJECT_NAME}.analyze ALL
            MODE GA:1,2;OP:1,2
            ANALYZE ${PROJECT_NAME}
            OUTPUT FORMAT errorfile
            LOG target_${PROJECT_NAME}.err
            RECURSIVE)
endif(NOT DEFINED ENV{CI})