cmake_minimum_required(VERSION 3.13)
project(roguelike)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

enable_testing()
add_compile_options("--coverage")
link_libraries("--coverage")
include_directories(external)

include(external/cmake_modules/CMakeRC.cmake)
include(external/cmake_modules/PVS-Studio.cmake)
find_package(Catch2 REQUIRED)

cmrc_add_resource_library(roguelike-res ALIAS rcfs::rc NAMESPACE rcfs
        default_config.json)

add_executable(roguelike
        main.cpp

        graphics.cpp
        graphics.h
        graphics/basic_console.h
        graphics/internal/graphics_include_protector.h
        map.h map.cpp
        characters.cpp characters.h
        map/types.h
        utils/PRNG.cpp utils/PRNG.h
        characters/types.h
        config.h config.cpp
        config/characters.h config/characters.cpp
        game.cpp game.h
        graphics/basic_console.cpp graphics/ncurses.cpp
        map/types.cpp
        scenes/game.cpp scenes/game.h
        scenes/base_scene.cpp scenes/base_scene.h)

add_executable(tests
        tests/test_main.cpp
        tests/prng.cpp
        tests/characters.cpp

        graphics.cpp
        graphics.h
        graphics/basic_console.h
        graphics/internal/graphics_include_protector.h
        map.h map.cpp
        characters.cpp characters.h
        map/types.h
        utils/PRNG.cpp utils/PRNG.h
        characters/types.h
        config.h config.cpp
        config/characters.h config/characters.cpp
        game.cpp game.h
        graphics/basic_console.cpp graphics/ncurses.cpp
        map/types.cpp
        scenes/game.cpp scenes/game.h
        scenes/base_scene.cpp scenes/base_scene.h)

target_link_libraries(roguelike PRIVATE rcfs::rc)
target_link_libraries(roguelike PUBLIC stdc++fs)
target_link_libraries(tests Catch2::Catch2)

pvs_studio_add_target(TARGET ${PROJECT_NAME}.analyze ALL
        MODE GA:1,2;OP:1,2
        ANALYZE ${PROJECT_NAME}
        OUTPUT FORMAT errorfile
        LOG target_${PROJECT_NAME}.err)