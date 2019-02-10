// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "map.h"
map::map() {
  width_ = 0;
  height_ = 0;
}

map::map(map_size_t width, map_size_t height, map_generator gen) {
  width_ = width;
  height_ = height;
  regenerate(gen);
}

map_size_t map::width() const {
  return width_;
}
map_size_t map::height() const {
  return height_;
}


void map::regenerate(map_generator gen) {
  map_container container = gen(width_, height_);
  characters_.clear();
  hero_ = container.hero;
  characters_ = container.characters;
}
void map::resize(map_size_t w, map_size_t h) {
  width_ = w;
  height_ = h;
}

std::shared_ptr<characters::Character> &map::hero() {
  return hero_;
}
chars_container &map::characters() {
  return characters_;
}
std::shared_ptr<characters::Character> map::at(map_size_t x, map_size_t y) {
  for (auto it = characters_.cbegin(); it != characters_.cend(); ++it) {
    auto &chr = *it;
    if (chr == nullptr) {
      characters_.erase(it);
      return nullptr;
    }
    if (chr->pos().x == x && chr->pos().y == y)
      return chr;
  }
  return nullptr;
}
std::shared_ptr<characters::Character> map::at(const map_point& pos) {
  return at(pos.x, pos.y);
}

namespace map_generators {
map_container box(map_size_t w, map_size_t h) {
  typedef enum {
    floor = 0,
    wall
  } map_block;
  using std::vector;
  map_container result;
  auto blocks = vector<vector<map_block>>(h, vector<map_block>(w, map_block::floor));

  // draw borders of the box
  for (map_size_t i = 0; i < w; ++i) {
    blocks[0][i] = map_block::wall;
    blocks[h-1][i] = map_block::wall;
  }
  for (map_size_t i = 0; i < h; ++i) {
    blocks[i][0] = map_block::wall;
    blocks[i][w-1] = map_block::wall;
  }

  // spawn the player somewhere
  PRNG rand;
  map_point pos_hero{}, pos_princess{};
  pos_hero.x = rand.get<map_size_t>(1, w - 2);
  pos_hero.y = rand.get<map_size_t>(1, h - 2);
  pos_princess.x = rand.get<map_size_t>(1, w - 2);
  pos_princess.y = rand.get<map_size_t>(1, h - 2);

  result.hero = std::make_shared<characters::Knight>(pos_hero);
  result.characters.push_back(std::make_shared<characters::Princess>(pos_princess));
  for (map_size_t i = 0; i < h; ++i) {
    for (map_size_t j = 0; j < w; ++j) {
      if (blocks[i][j] == map_block::wall) {
        result.characters.push_back(std::make_shared<characters::Wall>(j, i));
      }
    }
  }

  for (int i = 0; i < 10; ++i) {
    map_point pos_zombie{};
    pos_zombie.x = rand.get<map_size_t>(1, w - 2);
    pos_zombie.y = rand.get<map_size_t>(1, h - 2);
    if (rand.get(1, 100) >= 50)
      result.characters.push_back(std::make_shared<characters::Zombie>(pos_zombie));
    else
      result.characters.push_back(std::make_shared<characters::Dragon>(pos_zombie));
  }

  return result;
};
}

bool operator<(const std::shared_ptr<characters::Character> &lhs, const std::shared_ptr<characters::Character> &rhs) {
  auto pos_l = lhs->pos();
  auto pos_r = rhs->pos();
  if (pos_l.x == pos_r.x)
    return pos_l.y < pos_r.y;
  return pos_l.x < pos_r.x;
}