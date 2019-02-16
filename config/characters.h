#pragma once
#include <map>
#include <visit_struct/visit_struct.hpp>
#include <configuru.hpp>
#include "../characters/types.h"

struct CharacterConfig {
  sym_t sym;
  hp_t max_hp;
  hp_t damage;
};
VISITABLE_STRUCT(CharacterConfig,
    sym,
    max_hp,
    damage);

class CharactersConfig {
 public:
  void add_character(std::string char_name, CharacterConfig char_config);
  CharacterConfig& get(const std::string &key);
  configuru::Config state() const;
 private:
  std::map<std::string, CharacterConfig> chars_;
};