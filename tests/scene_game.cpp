// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include <catch2/catch.hpp>
#include "../scenes/game.h"
#include "../graphics/kbd_keys.h"
#include "../graphics.h"

// TODO CTest run each test separately (I guess). Need to do something with init
TEST_CASE("Game scene is created") {
  scenes::game g(5, 5);
}

TEST_CASE("Game scene throws an exception when created with wrong sizes") {
  REQUIRE_THROWS(scenes::game(0, 0));
}

TEST_CASE("Key controls work") {
  scenes::game g(10, 10);
  auto &m = g.current_map();
  for (auto &mob : m.characters()) {
    if (typeid(mob) == typeid(characters::Princess)) {
      mob->place(0, 0);
    } else {
      mob->hp(0);
    }
  }
  g.tick();
  m.hero()->place(5, 5);
  SECTION("Movement keys are handled") {
    g.input(Key::ARROW_UP);
    REQUIRE(m.hero()->pos() == map_point{5, 4});
    g.input(Key::ARROW_LEFT);
    REQUIRE(m.hero()->pos() == map_point{4, 4});
    g.input(Key::ARROW_DOWN);
    REQUIRE(m.hero()->pos() == map_point{4, 5});
    g.input(Key::ARROW_RIGHT);
    REQUIRE(m.hero()->pos() == map_point{5, 5});
  }
  SECTION("Throwing fireballs works") {
    auto &prs = g.hero_projectiles();

    g.input(Key::THROW_UP);
    REQUIRE(prs.size() == 1);
    REQUIRE(prs[0]->pos() == m.hero()->pos()); // TODO: I think I'll make a flag for projectile's first tick.
    prs[0]->tick(map_point{0, 0});
    REQUIRE(prs[0]->pos() == map_point{5, 4});
    g.tick();
    REQUIRE(prs.empty());

    g.input(Key::THROW_DOWN);
    REQUIRE(prs.size() == 1);
    REQUIRE(prs[0]->pos() == m.hero()->pos()); // TODO: the same as above
    prs[0]->tick(map_point{0, 0});
    REQUIRE(prs[0]->pos() == map_point{5, 6});
    g.tick();
    REQUIRE(prs.empty());

    g.input(Key::THROW_LEFT);
    REQUIRE(prs.size() == 1);
    REQUIRE(prs[0]->pos() == m.hero()->pos()); // TODO: the same as above
    prs[0]->tick(map_point{0, 0});
    REQUIRE(prs[0]->pos() == map_point{4, 5});
    g.tick();
    REQUIRE(prs.empty());

    g.input(Key::THROW_RIGHT);
    REQUIRE(prs.size() == 1);
    REQUIRE(prs[0]->pos() == m.hero()->pos()); // TODO: the same as above
    prs[0]->tick(map_point{0, 0});
    REQUIRE(prs[0]->pos() == map_point{6, 5});
    g.tick();
    REQUIRE(prs.empty());
  }
}