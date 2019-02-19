// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include <catch2/catch.hpp>
#include <typeinfo>
#include "../map.h"

TEST_CASE("Map is generated successfully", "[map]") {
  {
    map test_map(20, 30, map_generators::box);
    REQUIRE(test_map.width() == 20);
    REQUIRE(test_map.height() == 30);
  }
  {
    map test_map(64, 64, map_generators::snake);
    REQUIRE(test_map.width() == 64);
    REQUIRE(test_map.height() == 64);
  }
}

TEST_CASE("Map throws an exception on wrong size", "[map]") {
  REQUIRE_THROWS(map(0, 0, map_generators::box));
}

TEST_CASE("Generating map with custom generator", "[map][map_gen]") {
  map_generator test_gen = [](map_size_t w, map_size_t h) -> map_container {
    map_container result;
    result.hero = std::make_shared<characters::Knight>(0, 0);
    result.characters.push_back(std::make_shared<characters::Princess>(w-1, h-1));
    return result;
  };
  map test_map(2, 2, test_gen);
  REQUIRE(typeid(*(test_map.hero())) == typeid(characters::Knight));
  REQUIRE(test_map.hero()->pos() == map_point{0, 0});
  REQUIRE(test_map.characters().size() == 1);
  REQUIRE(test_map.characters()[0]->pos() == map_point{1, 1});
  REQUIRE(typeid(*(test_map.characters()[0])) == typeid(characters::Princess));
}