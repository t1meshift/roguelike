// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include <catch2/catch.hpp>
#include "../characters.h"
#include "../config.h"

namespace chr = characters;
namespace vis = chr::visitors;

TEMPLATE_TEST_CASE("Character creation", "[character]",
    chr::Knight, chr::Princess, chr::Zombie, chr::Dragon, chr::AidKit, chr::Fireball) {
  Config::init();
  TestType c(map_point{1, 1});
  REQUIRE(!c.is_dead());
  REQUIRE(c.hp() == c.max_hp());
  REQUIRE(c.pos() == map_point{1, 1});
  REQUIRE(c.projectiles().empty());
}

TEMPLATE_TEST_CASE("Character basic parameters", "[character]",
    chr::Knight, chr::Princess, chr::Zombie, chr::Dragon, chr::AidKit, chr::Fireball) {
  Config::init();
  TestType c(map_point{0, 0});
  SECTION("HP does not exceed its maximum") {
    c.max_hp(1);
    c.hp(2);
    REQUIRE(c.hp() == 1);

    c.max_hp(10);
    c.hp(10);
    c.max_hp(8);
    REQUIRE(c.hp() == 8);
  }
  SECTION("Movement works") {
    c.place(2, 2);
    REQUIRE(c.pos() == map_point{2, 2});

    c.move(1, -1);
    REQUIRE(c.pos() == map_point{3, 1});
  }
}