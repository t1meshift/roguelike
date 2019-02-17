// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include <catch2/catch.hpp>
#include <filesystem>
#include "../config.h"

TEST_CASE("Config root is read from JSON", "[config]") {
  ConfigRoot root("test_assets/custom_config.json");
  REQUIRE(root.characters().get("Knight").max_hp == 1234);
  REQUIRE(root.characters().get("Knight").damage == 9876);
}

TEST_CASE("Write config in non-existing file", "[config]") {
  ConfigRoot root("test_assets/nonexist.json");
  std::filesystem::path p("test_assets/nonexist.json");
  REQUIRE(std::filesystem::exists(p));
}

TEST_CASE("Malformed JSON throws exception", "[config]") {
  REQUIRE_THROWS(ConfigRoot("test_assets/malformed.json"));
}

TEST_CASE("Malformed character info throws exception", "[config]") {
  REQUIRE_THROWS(ConfigRoot("test_assets/malformed_character.json"));
}

TEST_CASE("Config in-game modifications are written", "[config]") {
  ConfigRoot root("test_assets/modifications.json");
  CharacterConfig conf{};
  conf.sym = 'Z';
  conf.max_hp = 1;
  conf.damage = 1;
  root.characters().add_character("TestChar", conf);
  root.save();

  configuru::Config c = configuru::parse_file("test_assets/modifications.json", configuru::JSON);
  REQUIRE((sym_t) c["characters"]["TestChar"]["sym"] == 'Z');
  REQUIRE((hp_t) c["characters"]["TestChar"]["max_hp"] == 1);
  REQUIRE((hp_t) c["characters"]["TestChar"]["damage"] == 1);
}

TEST_CASE("Config implicitly inits", "[config]") {
  REQUIRE_NOTHROW(Config::get());
}

TEST_CASE("Config reinits after implicit init", "[config]") {
  REQUIRE_NOTHROW(Config::init());
}