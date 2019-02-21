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
  scenes::game g(10, 10, map_generators::box);
  auto &m = g.current_map();
  for (auto &mob : m.characters()) {
    if (typeid(*mob) == typeid(characters::Princess)) {
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

TEST_CASE("Border tests") {
  map_generator empty_map = [](map_size_t w, map_size_t h) -> map_container {
    map_container r;
    r.hero = std::make_shared<characters::Knight>(1, 1);
    return r;
  };
  scenes::game g(10, 10, empty_map);
  auto &m = g.current_map();
  auto &prs = g.hero_projectiles();
  g.tick();
  SECTION("Trying to go out of borders must return hero to his previous place") {
    m.hero()->place(0, 0);
    g.tick();
    g.input(Key::ARROW_UP);
    REQUIRE(m.hero()->pos() == map_point{0, 0});
    g.input(Key::ARROW_LEFT);
    REQUIRE(m.hero()->pos() == map_point{0, 0});

    m.hero()->place(9, 9);
    g.tick();
    g.input(Key::ARROW_DOWN);
    REQUIRE(m.hero()->pos() == map_point{9, 9});
    g.input(Key::ARROW_RIGHT);
    REQUIRE(m.hero()->pos() == map_point{9, 9});
  }
  SECTION("Trying to spawn a projectile out of map borders must fail") {
    m.hero()->place(0, 0);
    g.tick();
    g.input(Key::THROW_UP);
    REQUIRE(prs.size() == 0);
    g.input(Key::THROW_LEFT);
    REQUIRE(prs.size() == 0);

    m.hero()->place(9, 9);
    g.tick();
    g.input(Key::THROW_DOWN);
    REQUIRE(prs.size() == 0);
    g.input(Key::THROW_RIGHT);
    REQUIRE(prs.size() == 0);
  }
}

TEST_CASE("Offsets are calculated correctly") {
  map_generator empty_map = [](map_size_t w, map_size_t h) -> map_container {
    map_container r;
    r.hero = std::make_shared<characters::Knight>(w/2, h/2);
    return r;
  };
  const map_size_t map_w = graphics::width() * 2;
  const map_size_t map_h = graphics::height() * 2;
  scenes::game g(map_w, map_h, empty_map);
  auto &m = g.current_map();
  auto &h = m.hero();

  map_point offset;
  const auto field_w = graphics::width();
  const auto field_h = graphics::height() - 1;
  const auto center_rect_w = field_w / 3;
  const auto center_rect_h = field_h / 3;
  //const auto center_rect_x = offset.x + (field_w - center_rect_w) / 2;
  //const auto center_rect_y = offset.y + (field_h - center_rect_h) / 2;
  SECTION("A hero is at map borders") {
    h->place(0, 0);
    g.tick();
    g.render();
    offset = g.offset();
    REQUIRE(offset.x == 0);
    REQUIRE(offset.y == 0);

    h->place(map_w - 1, map_h - 1);
    g.tick();
    g.render();
    offset = g.offset();
    REQUIRE(offset.x == map_w - field_w);
    REQUIRE(offset.y == map_h - field_h);

    h->place(0, map_h - 1);
    g.tick();
    g.render();
    offset = g.offset();
    REQUIRE(offset.x == 0);
    REQUIRE(offset.y == map_h - field_h);

    h->place(map_w - 1, 0);
    g.tick();
    g.render();
    offset = g.offset();
    REQUIRE(offset.x == map_w - field_w);
    REQUIRE(offset.y == 0);
  }
}