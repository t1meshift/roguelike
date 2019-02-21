// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include <catch2/catch.hpp>
#include "../scenes/base_scene.h"
#include "../game.h"
#include "../graphics/kbd_keys.h"

class final_test_scene : public scenes::base_scene {
 public:
  void input(int key) override {
    if (key != Key::NOTHING) {
      key_pressed_ = true;
    }
  }
  void tick() override {
    if (key_pressed_) {
      finished_ = true;
    }
  }
  void render() override {}
 private:
  bool key_pressed_ = false;
};

class test_scene : public scenes::base_scene {
 public:
  void input(int key) override {
    key_pressed_ = key;
  }
  void tick() override {
    if (key_pressed_ == Key::THROW_UP) {
      next_scene_ = std::make_shared<final_test_scene>();
      finished_ = true;
    }
    key_pressed_ = Key::NOTHING;
  }
  void render() override {
    rendered_ = true;
  }
  bool rendered() const {
    return rendered_;
  }
 private:
  int key_pressed_ = Key::NOTHING;
  bool rendered_ = false;
};


TEST_CASE("Scene container handles all commands") {
  auto s = std::make_shared<test_scene>();
  game g(s);
  SECTION("Key handler works") {
    g.input(Key::NOTHING);
    g.tick();
    g.render();
    REQUIRE(s->rendered()); // if the state is switched, it will return false

    g.input(Key::THROW_UP);
    g.tick();
    g.render();
    REQUIRE(s->finished());
    REQUIRE(!g.terminated());

    g.input(Key::ARROW_UP);
    g.tick();
    g.render();
    REQUIRE(g.terminated());

    REQUIRE_NOTHROW(g.input(Key::NOTHING));
    REQUIRE_NOTHROW(g.tick());
    REQUIRE_NOTHROW(g.render());
  }
}