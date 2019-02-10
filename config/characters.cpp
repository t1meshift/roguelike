// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "characters.h"

void CharactersConfig::add_character(std::string char_name, CharacterConfig char_config) {
  chars_.insert_or_assign(char_name, char_config);
}

CharacterConfig &CharactersConfig::get(const std::string &key) {
  return chars_.at(key);
}
configuru::Config CharactersConfig::state() const {
  auto chars_state = configuru::Config::object();
  for (const auto &char_state : chars_) {
    chars_state.emplace(char_state.first,
        configuru::serialize(char_state.second));
  }

  return chars_state;
}
